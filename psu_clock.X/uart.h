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

void uartInit(void);
void UART_putc(uint8_t data);
void UART_print_str(const char *str);
void UART_raw(char *buf, int numBytes);
void UART_print_dec(int ln, int x);
void UART_print_hex(int format, uint32_t x);


#endif	/* UART_H */

