/* 
 * File:   custom_types.h
 * Author: Konstantin
 *
 * Created on December 23, 2021, 6:48 PM
 */

#ifndef CUSTOM_TYPES_H
#define	CUSTOM_TYPES_H

/**
 * @param dfdsfs
 */
typedef union {
    uint8_t statusWord;
    struct {
        unsigned isUndervoltage:1,
                 isOvervoltage:1,
                 isShortCircuit:1,
                 isOverload:1,
                 isOverheating:1,
                 isTurnOnDelay:1,
                 isUnitHalted:1,
                 isUnitShutDown:1;
    } flags;
} UnitStatus_TypeDef;

typedef union {
    uint16_t arrayOfcodes[3];
    struct {
        uint16_t supplyVoltage;
        uint16_t outputVoltage;
        uint16_t unitTemperature;
    } values;
} UnitParams_TypeDef;

#endif	/* CUSTOM_TYPES_H */

