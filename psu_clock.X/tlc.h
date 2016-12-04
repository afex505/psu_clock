/* 
 * File:   tlc.h
 * Author: C15020
 *
 * Created on August 3, 2016, 1:26 PM
 */

#ifndef TLC_H
#define	TLC_H


#include <xc.h>
#include "plib.h" 

#define IFS1_SPI1TX_IF (1<<5)
#define IEC1_SPI1TX_IE (1<<5)

#define SPI1STAT_SPITBF (1<<1)

#define XLAT_DOWN() (LATDCLR = (1<<9))
#define XLAT_UP() (LATDSET = (1<<9))

void tlcInit(void);
void tlcSetChannel(int channel, int value);
void tlcClr(void);
void tlcUpdate(void);
void tlcAssemble(int changeFlags);

#endif	/* TLC_H */

