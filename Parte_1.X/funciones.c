/*
 * File:   funciones.c
 * Author: Mguel García
 *
 * Created on 8 de marzo de 2020, 19:26
 */

#include <xc.h>
#include <stdint.h>
#include "funciones.h"
#define _XTAL_FREQ 8000000
#define OUT_PWM PORTAbits.RA0

#define TRIGGER1 PORTAbits.RA2
#define ECHO1 PORTAbits.RA1
#define TRIGGER2 PORTAbits.RA3
#define ECHO2 PORTAbits.RA4

#define RS PORTDbits.RD6
#define EN PORTDbits.RD7

#define D0 PORTBbits.RB0
#define D1 PORTBbits.RB1
#define D2 PORTBbits.RB2
#define D3 PORTBbits.RB3
#define D4 PORTBbits.RB4
#define D5 PORTBbits.RB5
#define D6 PORTBbits.RB6
#define D7 PORTBbits.RB7

#define DATA PORTB 

void init_PWM_1 (void){
    TRISCbits.TRISC1 = 1;       //RC1-CCP2 como entrada
    
    PR2 = 155;                  //Periodo de la señal
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1;     //Modo PWM
    
    CCPR2L = 0b00011011;        //MSB del duty cicle
    CCP2CONbits.DC2B0 = 1;      //LSB del duty cicle
    CCP2CONbits.DC2B1 = 1;

    PIR1bits.TMR2IF = 0;        //TIMER 2 
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;      //Prescaler 1:16
    
    T2CONbits.TMR2ON = 1;       //Habilitamos el TMR2
    
    while(PIR1bits.TMR2IF == 0){
    }
    PIR1bits.TMR2IF = 0; 
    
    TRISCbits.TRISC1 = 0;       //RC1-CCP2 como salida
}



void servoRotate0(void){
    uint8_t a;
    for(a=0;a<50;a++){
        OUT_PWM = 1;
        __delay_us(800);
        OUT_PWM = 0;
        __delay_us(19200);
    }
}

void servoRotate90(void){
    uint8_t a;
    for(a=0;a<50;a++){
        OUT_PWM = 1;
        __delay_us(1500);
        OUT_PWM = 0;
        __delay_us(18500);
    }
}

void servoRotate180(void){
    uint8_t a;
    for(a=0;a<50;a++){
        OUT_PWM = 1;
        __delay_us(2200);
        OUT_PWM = 0;
        __delay_us(17800);
    }
}

uint16_t Distancia()
{
    uint16_t a;
    TMR1L = 0;
    TMR1H = 0;
    
    TRIGGER1 = 1;
    __delay_us(10);
    TRIGGER1 = 0;
    while(!ECHO1){
    }
    T1CONbits.TMR1ON = 1;
    while(ECHO1){
    }
    T1CONbits.TMR1ON = 0;    
    
    a = (TMR1L | (TMR1H<<8));
    a = a - 69;
    a = a / 22;
    
  return (a); //Return result
}

uint16_t Distancia_1()
{
    uint16_t a;
    TMR1L = 0;
    TMR1H = 0;
    
    TRIGGER2 = 1;
    __delay_us(10);
    TRIGGER2 = 0;
    while(!ECHO2){
    }
    T1CONbits.TMR1ON = 1;
    while(ECHO2){
    }
    T1CONbits.TMR1ON = 0;    
    
    a = (TMR1L | (TMR1H<<8));
    a = a - 69;
    a = a / 22;
    
  return (a); //Return result
}


void oscillator(uint8_t a){
    switch(a){
        case 0:
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 0;   //31kHz
        case 1:
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;   //125kHz
        case 2:
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;   //250kHz
        case 3:
            OSCCONbits.IRCF2 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 1;   //500kHz
        case 4:
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 0;   //1MHz
        case 5:
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF0 = 1;   //2MHz
        case 6:
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;   //4MHz
        case 7:
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 1;   //8MHz
        default:
            OSCCONbits.IRCF2 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF0 = 0;   //4MHz
    }
}


void Lcd_Init(void) {
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_us(100);
    Lcd_Cmd(0x30);
    __delay_us(100);////////////////////////////////////
    Lcd_Cmd(0x38);
    __delay_us(53);
    Lcd_Cmd(0x08);
    __delay_us(53);
    Lcd_Cmd(0x01);
    __delay_ms(3);
    Lcd_Cmd(0x06);
    __delay_us(53);
    Lcd_Cmd(0x0C);
    __delay_us(53);
}

void Lcd_Cmd(int a) {
	RS = 0;             // => RS = 0
    __delay_ms(5);
    EN = 1;
    __delay_ms(5);
	DATA = a;
    __delay_ms(5);
    EN  = 0;             // => E = 0
    __delay_ms(5);
}

void Lcd_Clear(void)
{   
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x01);
    __delay_ms(4);
}

void Lcd_Set_Cursor(int a, int b)
{
	int d;
	if(a == 1)
	{
        d = 0x80 + b - 1;
		Lcd_Cmd(d);
	}
	else if(a == 2)
	{
		d = 0xC0 + b - 1;
		Lcd_Cmd(d);
	}
}


void Lcd_Write_Char(char a)
{   
    RS = 1;
    // LCD_Port(caracter);
    DATA = a;
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void Lcd_Write_String(char *a)
{
    int n;
    for(n = 0; a[n] != '\0'; n++){
        Lcd_Write_Char(a[n]);
    }
}
