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

#if defined(__32MX430F064H__)
    #define XLAT_DOWN() (LATDCLR = (1<<9))
    #define XLAT_UP() (LATDSET = (1<<9))
    #define XLAT_TRIS   (TRISDbits.TRISD9)

    #define GSCLK_TRIS (TRISDbits.TRISD0)
    #define RP_GSCLK RPD0R
    #define RP_OC2 0b1011

    #define BLANK_TRIS TRISDbits.TRISD8
    #define RP_BLANK RPD8R
    #define RP_OC1 0b1100
    // SDO      RB8 (SDO1)
    // SCK      RD2 (SCK1))
    #define SDO1_TRIS TRISBbits.TRISB8
    #define SDO1_ANSEL_DN() {ANSELBbits.ANSB8=0;}
    #define SCK1_TRIS TRISDbits.TRISD2
    #define SCK1_ANSEL_DN() {ANSELDbits.ANSD2=0;}

    #define RP_SDO RPB8R
    #define RP_SDO1 0b1000

    #define IFS1_SPI1TX_IF (1<<5)
    #define IEC1_SPI1TX_IE (1<<5)
#else
    #define XLAT_DOWN() (LATACLR = (1<<0))
    #define XLAT_UP() (LATASET = (1<<0))
    #define XLAT_TRIS   (TRISAbits.TRISA0)

    #define GSCLK_TRIS (TRISCbits.TRISC8)
    #define RP_GSCLK RPC8R
    #define RP_OC2 0b0101

    #define BLANK_TRIS TRISCbits.TRISC7
    #define RP_BLANK RPC7R
    #define RP_OC1 0b0101

    // SDO      RB8 (SDO1)
    // SCK      RB14 (SCK1))

    #define SDO1_TRIS TRISBbits.TRISB8
    #define SDO1_ANSEL_DN() {;} 
    #define SCK1_TRIS TRISBbits.TRISB14
    #define SCK1_ANSEL_DN() {ANSELBbits.ANSB14=0;}//todo should be atomic

    #define RP_SDO RPB8R
    #define RP_SDO1 0b0011

    #define IFS1_SPI1TX_IF (1<<6)
    #define IEC1_SPI1TX_IE (1<<6)
#endif



#define SPI1STAT_SPITBF (1<<1)


void tlcInit(void);
void tlcSetChannel(int channel, int value);
void tlcClr(void);
void tlcUpdate(void);
void tlcAssemble(int changeFlags);

#endif	/* TLC_H */

