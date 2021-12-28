/*
 * File:   main.c
 * Author: Konstantin
 *
 * Created on December 6, 2021, 9:23 AM
 */
/******************** PIC12F675 Configuration Bit Settings ********************/

// Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT
// pin, I/O function on GP5/OSC1/CLKIN)
#pragma config FOSC = INTRCIO
// Watchdog Timer Enable bit (WDT disabled)
#pragma config WDTE = OFF       
// Power-Up Timer Enable bit (PWRT disabled)
#pragma config PWRTE = OFF      
// GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config MCLRE = ON       
// Brown-out Detect Enable bit (BOD disabled)
#pragma config BOREN = OFF       
// Code Protection bit (Program Memory code protection is disabled)
#pragma config CP = OFF   
// Data Code Protection bit (Data memory code protection is disabled)
#pragma config CPD = OFF        

/******************************************************************************/

#include <xc.h>
#include "utilities.h"



/************************** USER-DEFINED PROTOTYPES ***************************/
extern void IICSend(void);


/*************************** USER-DEFINED VARIABLES ***************************/
UnitStatus_TypeDef      unitStatus;
UnitParams_TypeDef      unitParams;
uint32_t                ticks                       = 0;
uint32_t                timestampDelayed            = 0;
uint8_t                 cntRestartAttempts          = 0;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

void main(void) {
    INIT_POWER();
    INIT_GPIO();
    INIT_ADC();
    INIT_TIMER0();
    ENABLE_INTERRUPTS(); 

    while(1) {
        if (unitStatus.flags.isOverheating
                || unitStatus.flags.isOvervoltage
                || unitStatus.flags.isUndervoltage
                || unitStatus.flags.isUnitShutDown
                || (unitStatus.flags.isShortCircuit
                && unitStatus.flags.isTurnOnDelay)) {
            UNIT_TURN_OFF();
            unitStatus.flags.isUnitHalted = 1;
        } else {
            UNIT_TURN_ON();
            unitStatus.flags.isUnitHalted = 0;
        }
        if (unitStatus.flags.isUnitShutDown) continue;
        if ((unitStatus.flags.isShortCircuit && unitStatus.flags.isTurnOnDelay)
                || (unitStatus.flags.isOverload && unitStatus.flags.isTurnOnDelay)) {
            if (ticks > timestampDelayed) {
                unitStatus.flags.isTurnOnDelay = 0;
            }
        }
        if (unitParams.values.supplyVoltage < SUP_VOLT_MIN_LTHLD_CODE) {
            unitStatus.flags.isUndervoltage = 1;
        }
        if (unitParams.values.supplyVoltage > SUP_VOLT_MIN_UTHLD_CODE) {
            unitStatus.flags.isUndervoltage = 0;
        }
        if (unitParams.values.supplyVoltage > SUP_VOLT_MAX_UTHLD_CODE) {
            unitStatus.flags.isOvervoltage = 1;
        }
        if (unitParams.values.supplyVoltage < SUP_VOLT_MAX_LTHLD_CODE) {
            unitStatus.flags.isOvervoltage = 0;
        }
        if ((unitParams.values.unitTemperature > UNIT_TEMP_UTHLD_CODE)
                || (unitParams.values.unitTemperature < UNIT_TEMP_ERROR_CODE)) {
            unitStatus.flags.isOverheating = 1;
        }
        if (unitParams.values.unitTemperature < UNIT_TEMP_LTHLD_CODE) {
            unitStatus.flags.isOverheating = 0;
        }
        if (unitStatus.flags.isUnitHalted) continue;
        if (unitParams.values.outputVoltage < OUT_SC_VOLT_CODE) {
            unitStatus.flags.isShortCircuit = 1;
            unitStatus.flags.isTurnOnDelay = 1;
            cntRestartAttempts++;
            if (cntRestartAttempts > NUM_OF_RESTART_ATTEMPTS) {
                unitStatus.flags.isUnitShutDown = 1;
                continue;
            }
            timestampDelayed = ticks + DELAY_TO_TURN_BACK_ON;
            continue;
        } else {
            cntRestartAttempts = 0;
        }
        if (unitParams.values.outputVoltage < OUT_OVL_VOLT_CODE) {
            if (!unitStatus.flags.isOverload) {
                unitStatus.flags.isOverload = 1;
                timestampDelayed = ticks + DELAY_TO_TURN_OFF_OVL;
            } else {
                if (ticks > timestampDelayed) {
                    unitStatus.flags.isUnitShutDown = 1;
                }
            }
        } else {
            unitStatus.flags.isOverload = 0;
        }
    }
}



/************************* INTERRUPT SERVICE ROUTINE **************************/
void __interrupt() isr(void) {
    DISABLE_INTERRUPTS();
    if (TMR0_IS_INT_SOURCE()) {
        TMR0_CLR_INT_FLAG();
        TMR0_PRELOAD(TMR0_PRELOAD_VALUE);
        ticks++;
    }
    if (ADC_IS_INT_SOURCE()) {
        ADC_CLR_INT_FLAG();
        unitParams.arrayOfcodes[ADC_GET_CHANNEL()] = ADC_GET_RESULT();
        ADC_SET_NEXT_CHANNEL();
        ADC_START_CONVERSION();
    }
    ENABLE_INTERRUPTS();
    return;
}

/*************************** USER-DEFINED FUNCTIONS ***************************/

