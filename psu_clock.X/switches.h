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
#define KNOBC 1


//#define IFS0_T4IF (1<<3)

void switchInit(void);
int switchKnobValue(int channel);
int switchValues(void);

#endif	/* SWITCHES_H */

