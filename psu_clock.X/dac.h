/* 
 * File:   dac.h
 * Author: C15020
 *
 * Created on July 29, 2016, 1:46 PM
 */

#ifndef DAC_H
#define	DAC_H

#include <xc.h>
#include <stdint.h>

#define CS_SET() (LATBSET = (1<<9))
#define CS_CLR() (LATBCLR = (1<<9))

#define LDAC_SET() (LATASET = (1<<10))
#define LDAC_CLR() (LATACLR = (1<<10))


int dacSet(int voltage, int channel);
void dacInit(void);
void dacLoad(void);
void dacSetMax(int ch1, int ch2);


#endif	/* DAC_H */

