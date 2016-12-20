/* 
 * File:   main.h
 * Author: C15020
 *
 * Created on July 27, 2015, 9:21 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "plib.h" //only for systemconfigperformance()!)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "uart.h"
#include "rtcc.h"
#include "dac.h"
#include "switches.h"
#include <sys/attribs.h>
#include "settings.h"


#define IFS0_T1IF (1<<4)
#define IFS0_T4IF (1<<19)
#define IFS0_T5IF (1<<24)

#define LED1 LATCbits.LATC5
#define LED2 LATCbits.LATC6
#define LED3 LATCbits.LATC4
#define LED4 LATCbits.LATC3

#define LED1ON() LATCCLR = 0b00100000;
#define LED2ON() LATCCLR = 0b01000000;
#define LED3ON() LATCCLR = 0b00010000;
#define LED4ON() LATCCLR = 0b00001000;

#define LED1OFF() LATCSET = 0b00100000;
#define LED2OFF() LATCSET = 0b01000000;
#define LED3OFF() LATCSET = 0b00010000;
#define LED4OFF() LATCSET = 0b00001000;

#define LED1TGL() LATCINV = 0b00100000;
#define LED2TGL() LATCINV = 0b01000000;
#define LED3TGL() LATCINV = 0b00010000;
#define LED4TGL() LATCINV = 0b00001000;

#define LEDSOFF() LATCSET = 0b01111000;

#define LEDTRISCLR TRISCCLR
#define LEDMASK 0b01111000



enum state{
    sm_undefined,       //a
    sm_init,            //b
    sm_trans_to_cal,    //c
    sm_cal,             //d
    sm_leaving_cal,     //e
    sm_trans_to_clk,    //f
    sm_clk,             //g
    sm_leaving_clk,     //h
};

void init(void);

void initTMR(void);


inline unsigned int __attribute__((always_inline)) SYSTEMConfigPerformance(unsigned int);
        

#endif	/* MAIN_H */

