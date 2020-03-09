/*
 * File:   funciones.c
 * Author: Mguel García
 *
 * Created on 8 de marzo de 2020, 19:26
 */

#include <xc.h>
#include <stdint.h>
#include "funciones.h"
#define _XTAL_FREQ 1000000
#define OUT_PWM PORTAbits.RA0


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
    
    PORTAbits.RA2 = 1;
    __delay_us(10);
    PORTAbits.RA2 = 0;
    while(!PORTAbits.RA1){
    }
    T1CONbits.TMR1ON = 1;
    while(PORTAbits.RA1){
    }
    T1CONbits.TMR1ON = 0;    
    
    a = (TMR1L | (TMR1H<<8));
    a = a/7.35;
    a = a + 1;
    
  return (a); //Return result
}

