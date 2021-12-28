/* 
 * File:   settings.h
 * Author: Konstantin
 *
 * Created on December 19, 2021, 7:11 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

// Given timer clock period of 0.000064s, the resulting overflow period is 0.01s
#define TMR0_PRELOAD_VALUE          100
#define ADC_CH_SUPPLY_VOLTAGE       0
#define ADC_CH_OUTPUT_VOLTAGE       1
#define ADC_CH_TEMPERATURE          2
#define ADC_LAST_CHANNEL            2

// Minimum supply voltage hysteresis: lower threshold, V
#define SUP_VOLT_MIN_LTHLD          19
// Minimum supply voltage hysteresis: upper threshold, V
#define SUP_VOLT_MIN_UTHLD          21
// Maximum supply voltage hysteresis: lower threshold, V
#define SUP_VOLT_MAX_LTHLD          29
// Maximum supply voltage hysteresis: upper threshold, V
#define SUP_VOLT_MAX_UTHLD          31
// Dimmed mode output voltage set point, V
#define OUT_VOLT_DIMMED_SP          14
// Dimmed mode output voltage set point, V
#define OUT_VOLT_BRIGHT_SP          24
// Output voltage set point depending on the selected mode of operation, V
#define OUT_VOLT_CUR_SP             (TEST_BIT(GPIO, 4) ? OUT_VOLT_BRIGHT_SP    \
                                    : OUT_VOLT_DIMMED_SP)
// Maximum output voltage considered a short-circuit condition, V
#define OUT_SC_VOLT                 1
// Maximum output voltage drop considered a overload condition, V
#define OUT_OVL_VOLT_DROP           2
// Delay before next attempt to turn the unit on after overload or
// short circuit is detected, x10 ms
#define DELAY_TO_TURN_BACK_ON       300
// short circuit is detected, x10 ms
#define DELAY_TO_TURN_OFF_OVL       300
// Number of attempts to restart the unit after overloaded detected
#define NUM_OF_RESTART_ATTEMPTS     5

// Volatage-to-code conversion coefficient
#define ADC_VOLT_TO_CODE            19
// Supply voltage levels transferred to ADC codes
#define SUP_VOLT_MIN_LTHLD_CODE     (SUP_VOLT_MIN_LTHLD * ADC_VOLT_TO_CODE)
#define SUP_VOLT_MIN_UTHLD_CODE     (SUP_VOLT_MIN_UTHLD * ADC_VOLT_TO_CODE)
#define SUP_VOLT_MAX_LTHLD_CODE     (SUP_VOLT_MAX_LTHLD * ADC_VOLT_TO_CODE)
#define SUP_VOLT_MAX_UTHLD_CODE     (SUP_VOLT_MAX_UTHLD * ADC_VOLT_TO_CODE)
// Output voltage levels transferred to ADC codes
#define OUT_SC_VOLT_CODE            (OUT_SC_VOLT * ADC_VOLT_TO_CODE)
#define OUT_OVL_VOLT_CODE           (ADC_VOLT_TO_CODE *                        \
                                    (OUT_VOLT_CUR_SP - OUT_OVL_VOLT_DROP))

// The following temeprature values are for reference only and not to be used
// within the main program
// Unit overheating temperature hysteresis: lower threshold, deg Celsius
//     UNIT_TEMP_LTHLD = 80
// Unit overheating temperature hysteresis: upper threshold, deg Celsius
//     UNIT_TEMP_UTHLD = 90
// From the datasheet for the TDK temperature sensor (ref # B59901D0070A040),
// the following resistance can be inferred, corresponding to the temperature
// leveles mentioned above, Ohm:
//     UNIT_TEMP_SENS_RES_LTHLD = 1000
//     UNIT_TEMP_SENS_RES_UTHLD = 10000
// By design, the temp sensor is connected in series with a resistor, Ohm:
//     ADC_TEMP_UPPER_RES = 3900
// The entire temperature-measuring circuit is displayed below:
//
//                 +5V
//                 ===
//                  |
//                 [ ]
//                 [ ] ADC_TEMP_UPPER_RES = 3.9 kOhm
//  MCU            [ ]
// ---------| 5     |
//  GP2/AN2 |-------x   
//          |       |
//                 [ ]
//                 [ ] TEMP_SENSOR
//                 [ ]
//                  |
//                 --- 
//
// Given the 10-bit ADC resolution and reference voltage
//     ADC_REF_VOLT = 5V,
// ADC codes for lower and upper levele can be calculated as follows:
//                         UNIT_TEMP_SENS_RES_LTHLD
//     (2^10) x ----------------------------------------------- ,
//               ADC_TEMP_UPPER_RES + UNIT_TEMP_SENS_RES_LTHLD
//
//                         UNIT_TEMP_SENS_RES_UTHLD
//     (2^10) x -----------------------------------------------
//               ADC_TEMP_UPPER_RES + UNIT_TEMP_SENS_RES_UTHLD
//
//
#define UNIT_TEMP_LTHLD_CODE        209
#define UNIT_TEMP_UTHLD_CODE        737
// From the thermistor datasheet, the minimum resistance ever possible is 70 Ohm
// Therefore the threshold to determine thermistor malfanction is set to 50 Ohm,
// which corrsponds to the following ADC code:
#define UNIT_TEMP_ERROR_CODE        13

#endif	/* SETTINGS_H */

