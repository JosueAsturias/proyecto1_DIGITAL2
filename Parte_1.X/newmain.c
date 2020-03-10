/*
 * File:   Proyecto_1_2.c
 * Author: MIGUEL GARCÍA
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "funciones.h"
#define _XTAL_FREQ 8000000

uint16_t distancia_1;
uint16_t distancia_2;
char print_lcd[16];
char print_lcd_1[16];

void main(void) {
    oscillator(7);
    TRISA = 0b00010010;
    ANSEL = 0;   
    PORTB = 0x00;
    TRISB = 0x00;
    ANSELH = 0x00;
    PORTD = 0x00;
    TRISD = 0x00;
    
    Lcd_Init();
    Lcd_Clear();
    
    T1CON = 0x10;
    
    while(1){
        distancia_1 = Distancia();
        
        
        if (distancia_1>= 2 && distancia_1<=400){
            //Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("DIS_1 = ");
            sprintf(print_lcd, "%i",distancia_1);
            Lcd_Write_String(print_lcd);
            Lcd_Write_String(" cm     ");
        }
        else{
            //Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("FUERA DE RANGO     ");
        }
        
        __delay_ms(100);
        distancia_2 = Distancia_1();
        
        if (distancia_2>= 2 && distancia_2<=400){
            //Lcd_Clear();
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("DIS_2 = ");
            sprintf(print_lcd_1, "%i",distancia_2);
            Lcd_Write_String(print_lcd_1);
            Lcd_Write_String(" cm     ");
        }
        else{
            //Lcd_Clear();
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("FUERA DE RANGO     ");
        }        
        __delay_ms(500);
    }
    return;
}


