/* 
 * File:   switches.h
 * Author: C15020
 *
 * Created on August 3, 2016, 9:19 AM
 */

#ifndef SWITCHES_H
#define	SWITCHES_H

#include <xc.h>
//#include "plib.h"
#include <stdint.h>

#define KNOBTICK 20
#define KNOBV 0
#define KNOBA 1
#define KNOBTRM 2

#define KNOBV_ADCCH 5
#define KNOBA_ADCCH 24
#define KNOBTRM_ADCCH 4

    //SWLIM  SW2   RE0 / io1
    //SWPOW  SW1   RE1

#define SW_0_STATE ((PORTE&0b10)<<0)
#define SW_1_STATE ((PORTE&0b1)<<1)
#define SW_GPIO_INIT() { \
    TRISESET = (1<<1);\
    ANSELECLR = (1<<1); \
    TRISESET = (1<<0); \
    ANSELECLR = (1<<0); \
    }


    //KNOBV     RB3     AN5
    //KNOBA     RD1 AN24      RC0     AN6
    //KNOBTRIM  RB2     AN4
#define KNOB_GPIO_INIT() { \
    ANSELBSET = (1<<3)|(1<<2); \
    TRISBSET = (1<<3)|(1<<2); \
    ANSELDSET = (1<<1); \
    TRISDSET = (1<<1); \
    }

void switchInit(void);
int switchKnobValue(int channel);
int switchValues(void);

#endif	/* SWITCHES_H */

