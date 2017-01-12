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




#if defined(__32MX430F064H__)
#define SW_0_STATE ((PORTE&0b10)&&1)
#define SW_1_STATE ((PORTE&0b01)&&1)
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

    #define KNOBV_ADCCH 5
    #define KNOBA_ADCCH 24
    #define KNOBTRM_ADCCH 4
#else //MX250
#define SW_0_STATE ((PORTC&0b010)&&1)
#define SW_1_STATE (!(PORTC&0b100))

#define SW_MASK_POWER       0b000010
#define SW_MASK_LIMIT       0b000001
#define SW_MASK_POWER_DN    0b001000
#define SW_MASK_LIMIT_DN    0b000100
#define SW_MASK_POWER_UP    0b100000
#define SW_MASK_LIMIT_UP    0b010000

#define SW_GPIO_INIT() { \
    TRISCSET = (1<<1);\
    ANSELCCLR = (1<<1); \
    TRISCSET = (1<<2); \
    ANSELCCLR = (1<<2); \
    }
    //KNOBV     RB3     AN5
    //KNOBA     RD1 AN24      RC0     AN6
    //KNOBTRIM  RB2     AN4
#define KNOB_GPIO_INIT() { \
    ANSELBSET = (1<<3)|(1<<2); \
    TRISBSET = (1<<3)|(1<<2); \
    ANSELCSET = (1<<0); \
    TRISCSET = (1<<0); \
    }

    #define KNOBV_ADCCH 5
    #define KNOBA_ADCCH 6
    #define KNOBTRM_ADCCH 4
#endif


void switchInit(void);

void switchLatch(void);
void switchRead(void);
int switchKnobValue(int channel);

int switchGet(int mask);
int switchGetKnob(int channel);


//the 'virtual encoder' allows us to 
//use the 11 turn voltage pot almost as 
//an infinitely rotatable encoder

int switchVirtGet(void);
void switchVertSet(int value);

#endif	/* SWITCHES_H */

