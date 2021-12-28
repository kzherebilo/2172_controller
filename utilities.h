/* 
 * File:   utilities.h
 * Author: Konstantin
 *
 * Created on December 19, 2021, 7:08 PM
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#include "settings.h"
#include "custom_types.h"

#define SET_BIT(reg, bit)           ((reg) |= (1 << (bit)))
#define CLR_BIT(reg, bit)           ((reg) &= ~(1 << (bit)))
#define TEST_BIT(reg, bit)          ((reg) & (1 << (bit)))

#define UNIT_TURN_ON()              (CLR_BIT(GPIO, 5))
#define UNIT_TURN_OFF()             (SET_BIT(GPIO, 5))
#define ENABLE_INTERRUPTS()         (INTCON |= 0xC0)
#define DISABLE_INTERRUPTS()        (INTCON &= ~(0xC0))
#define TMR0_CLR_INT_FLAG()         (CLR_BIT(INTCON, 2))
#define TMR0_IS_INT_SOURCE()        (TEST_BIT(INTCON, 2))
#define TMR0_PRELOAD(value)         (TMR0 = (value))
#define ADC_CLR_INT_FLAG()          (CLR_BIT(PIR1, 6))
#define ADC_IS_INT_SOURCE()         (TEST_BIT(PIR1, 6))
#define ADC_START_CONVERSION()      (SET_BIT(ADCON0, 1))
#define ADC_GET_RESULT()            (uint16_t)((ADRESH << 8) | ADRESL)
#define ADC_GET_CHANNEL()           (uint8_t)((ADCON0 & 0x0C) >> 2)

#define ADC_SET_NEXT_CHANNEL() {                                               \
    if (ADC_GET_CHANNEL() == ADC_LAST_CHANNEL) {                               \
        ADCON0 &= ~(0x0C);                                                     \
    } else {                                                                   \
        ADCON0 += (1 << 2);                                                    \
    }                                                                          \
}

#define INIT_POWER() {                                                         \
    /* Clear POR and BOR bit in PCON register:                               */\
    /* - b1 (nPOR)   => ON  - No POR occured                                 */\
    SET_BIT(PCON, 1);                                                          \
    /* - b0 (nBOR)   => ON  - No BOR occured                                 */\
    SET_BIT(PCON, 0);                                                          \
}

#define INIT_GPIO() {                                                          \
    /* Configure TRISIO:                                                     */\
    /* - b5 (TRISIO5)=> OFF - GP5 is used as output to shut down the unit    */\
    CLR_BIT(TRISIO, 5);                                                        \
    /* Congifure WPU:                                                        */\
    /* - disable all individual pull-ups                                     */\
    WPU &= ~(0x37);                                                            \
}

#define INIT_ADC() {                                                           \
    /* Configure ANSEL                                                       */\
    /* - b6:4 (ADCS) => 1   - ADC conversion clock is set to Fosc/8 (2us)    */\
    CLR_BIT(ANSEL, 6);                                                         \
    CLR_BIT(ANSEL, 5);                                                         \
    SET_BIT(ANSEL, 4);                                                         \
    /* - b3 (ANS3)   => OFF - Pin GP4/AN3 is assigned as digital input       */\
    CLR_BIT(ANSEL, 3);                                                         \
    /* - b2 (ANS2)   => ON  - Pin GP2/AN2 is assigned as analog input        */\
    SET_BIT(ANSEL, 2);                                                         \
    /* - b1 (ANS1)   => ON  - Pin GP1/AN1 is assigned as analog input        */\
    SET_BIT(ANSEL, 1);                                                         \
    /* - b0 (ANS0)   => ON  - Pin GP0/AN0 is assigned as analog input        */\
    SET_BIT(ANSEL, 0);                                                         \
    /* Configure ADCON0                                                      */\
    /* - b7 (ADFM)   => ON  - ADC right justified result                     */\
    SET_BIT(ADCON0, 7);                                                        \
    /* - b6 (VCFG)   => OFF - ADC ref voltage is Vdd                         */\
    CLR_BIT(ADCON0, 6);                                                        \
    /* - b0 (ADON)   => ON  - ADC turned on                                  */\
    SET_BIT(ADCON0, 0);                                                        \
    /* Configure PIE1 register                                               */\
    /* - b6 (ADIE)   => ON  - Enable the ADC interrupt                       */\
    SET_BIT(PIE1, 6);                                                          \
}

#define INIT_TIMER0() {                                                        \
    /* Configure OPTION_REG:                                                 */\
    /* - b5 (T0CS)   => OFF - Clock source is internal clock Fosc/4          */\
    CLR_BIT(OPTION_REG, 5);                                                    \
    /* - b3 (PSA)    => OFF - Prescaler is assigned to the TIMER0            */\
    CLR_BIT(OPTION_REG, 3);                                                    \
    /* - b2:0 (PS2:0)=> 5   - Prescaler rate is set to 1:64                  */\
    /* thus resulting in Fosc/256 timer clock and 0.000064s period @ 4MHz    */\
    SET_BIT(OPTION_REG, 2);                                                    \
    CLR_BIT(OPTION_REG, 1);                                                    \
    SET_BIT(OPTION_REG, 0);                                                    \
    /* Configure INTCON:                                                     */\
    /* - b5 (T0IE)   => ON  - Enable TIMER0 interrupt                        */\
    SET_BIT(INTCON, 5);                                                        \
}

#endif	/* UTILITIES_H */

