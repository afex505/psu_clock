/* 
 * File:   uart.h
 * Author: C15020
 *
 * Created on July 15, 2016, 4:33 PM
 */
#include <xc.h>
#include <stdint.h>

#ifndef UART_H
#define	UART_H


#if defined(__32MX430F064H__)
    #define UART_TRIS TRISFbits.TRISF0
    #define UART_ANSEL_DN() {ANSELFbits.ANSF0=0;}
    #define RP_UART RPF0R
    #define RP_U2TX 0b0001
#else
    #define UART_TRIS TRISCbits.TRISC9
    #define UART_ANSEL_DN() {}
    #define RP_UART RPC9R
    #define RP_U2TX 0b0010
#endif
void uartInit(void);
void UART_putc(uint8_t data);
void UART_print_str(const char *str);
void UART_raw(char *buf, int numBytes);
void UART_print_dec(int ln, int x);
void UART_print_hex(int format, uint32_t x);


#endif	/* UART_H */

