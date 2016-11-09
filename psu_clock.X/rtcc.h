/* 
 * File:   rtcc.h
 * Author: C15020
 *
 * Created on July 18, 2016, 10:25 AM
 */

#ifndef RTCC_H
#define	RTCC_H


#include <xc.h>
#include <sys/attribs.h>
void rtccInit(void);
int rtccMin(void);
int rtccSec(void);
int rtccHr(void);

int rtccSetSec(int seconds);
int rtccSetMin(int minutes);
int rtccSetHr(int hours);
#endif	/* RTCC_H */

