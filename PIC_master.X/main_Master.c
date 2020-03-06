/*
 * File:   main_Master.c
 * Author: jotas
 *
 * Created on March 6, 2020, 8:23 AM
 */
// CONFIG1
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
#include "I2C.h"
#include "RTC.h"
#include "LCD_8bits.h"

#define _XTAL_FREQ 4000000
uint8_t estado = 0;
uint8_t seg = 0;
uint8_t min = 21;
uint8_t hora = 16;
uint8_t dia = 5;
uint8_t datum = 6;
uint8_t mes = 3;
uint8_t jahr = 20;

void display_Uhrzeit(uint8_t fila, uint8_t columna);
void display_Datum(uint8_t fila, uint8_t columna);

void main(void) {
    TRISD = 0;
    TRISC0 = 0;
    TRISC1 = 0;
    LCD_init();
    LCD_clear();
    I2C_Master_Init(100000);
    //Zeit_Datum_Set();
    get_Time();
    while(1){
        switch(estado){
            case 0:
                get_Time();
                display_Uhrzeit(2,4);
                display_Datum(1,3);
                break;
            case 1:
                LCD_Set_Cursor(1,0);
                LCD_Write_String("Temp. Hambiente:");
                LCD_Set_Cursor(2,6);
                LCD_Write_String("69");
                LCD_Write_Character(223);
                LCD_Write_Character('C');
                break;
            case 2:
                LCD_Set_Cursor(1,0);
                LCD_Write_String("Temp. del Suelo:");
                LCD_Set_Cursor(2,6);
                LCD_Write_String("20");
                LCD_Write_Character(223);
                LCD_Write_Character('C');
                break;
            case 3:
                LCD_Set_Cursor(1,0);
                LCD_Write_String("Humedad:");
                LCD_Set_Cursor(2,6);
                LCD_Write_String("80");
                LCD_Write_Character('%');
                break;
            case 4:
                LCD_Set_Cursor(1,0);
                LCD_Write_String("Atras: | Frente:");
                LCD_Set_Cursor(2,2);
                LCD_Write_String("3");
                LCD_Write_Character('m');
                LCD_Set_Cursor(2,7);
                LCD_Write_Character('|');
                LCD_Set_Cursor(2,11);
                LCD_Write_String("4");
                LCD_Write_Character('m');
                break;
            default:
                LCD_Set_Cursor(1,9);
                LCD_Write_String("ERROR");
        }
    }
    return;
}

/*Funcion que muestra hora:minutos:segundos obtenido del RTC*/
void display_Uhrzeit(uint8_t fila, uint8_t columna){
    char seg_u = seg%10;
    char seg_d = seg/10;
    char min_u = min%10;
    char min_d = min/10;
    char Uhr_u = hora%10;
    char Uhr_d = hora/10;
    
    LCD_Set_Cursor(fila, columna);
    LCD_Write_Character(Uhr_d + '0');
    LCD_Write_Character(Uhr_u + '0');
    LCD_Write_Character(':');
    LCD_Write_Character(min_d + '0');
    LCD_Write_Character(min_u + '0');
    LCD_Write_Character(':');
    LCD_Write_Character(seg_d + '0');
    LCD_Write_Character(seg_u + '0');
    
}

void display_Datum(uint8_t fila, uint8_t columna){
    char dia_u = datum%10;
    char dia_d = datum/10;
    char mes_u = mes%10;
    char mes_d = mes/10;
    char jahr_u = jahr%10;
    char jahr_d = jahr/10;
    
    LCD_Set_Cursor(fila, columna);
    LCD_Write_Character(dia_d + '0');
    LCD_Write_Character(dia_u + '0');
    LCD_Write_Character('/');
    LCD_Write_Character(mes_d + '0');
    LCD_Write_Character(mes_u + '0');
    LCD_Write_Character('/');
    LCD_Write_String("20");
    LCD_Write_Character(jahr_d + '0');
    LCD_Write_Character(jahr_u + '0');
    
}
