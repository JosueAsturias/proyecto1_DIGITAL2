/*
 * File:   UART.c
 * Author: jotas
 *
 * Created on February 9, 2020, 2:36 PM
 */


#include <xc.h>
#include "UART.h"
#include <stdint.h>
#define _XTAL_FREQ 4000000

void uart_init(){
    TXSTAbits.SYNC = 0;   //asincrono
    TXSTAbits.BRGH = 1;  
    BAUDCTLbits.BRG16 = 1;  //8 BITS BAURD RATE GENERATOR
    SPBRG = 103;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;     //habilitamos recibir datos
    RCSTAbits.RX9 = 0;      //8 bits
    RCSTAbits.CREN = 1;     //habilitamos la recepción de datos
    
    TXSTAbits.TXEN = 1;     //habilitamos la transmisión de datos 
}



uint8_t uartRC_Read(){
    return RCREG;
}

void uartTX_Write(uint8_t dato){
    TXREG = dato;
    while(PIR1bits.TXIF == 0);
}

void uartTX_Write_Str(char * string){
    int n;
    for (n=0; string[n] != '\n'; n++){
        uartTX_Write(string[n]);
    }
}