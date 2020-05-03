/* 
 * File:   UART.h
 * Author: josue
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __UART_H
#define	__UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
void uart_init();
uint8_t uartRC_Read();
void uartTX_Write(char dato);
void uartTX_Write_Str(char * string);

#endif	/* XC_HEADER_TEMPLATE_H */