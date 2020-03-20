/*
 * File:   PWMs.c
 * Author: Josue
 *
 * Created on 15 de marzo de 2020, 09:12 PM
 */


#include <xc.h>
#include <stdint.h>
#include "PWM.h"
#define _XTAL_FREQ 8000000
#define MOTOR_DERECHO_FRENTE_1 RD0
#define MOTOR_DERECHO_FRENTE_2 RD1

#define MOTOR_DERECHO_ATRAS_1 RD2
#define MOTOR_DERECHO_ATRAS_2 RD3

#define MOTOR_IZQUIERDO_FRENTE_1 RA6
#define MOTOR_IZQUIERDO_FRENTE_2 RA7

#define MOTOR_IZQUIERDO_ATRAS_1 RD4
#define MOTOR_IZQUIERDO_ATRAS_2 RD5


void init_PWM_2(void){            // Se configura el CCP2 --> RC1 !!!
    TRISCbits.TRISC1 = 1;        // (PASO#1) ESTABLECER RC1-CCP2 COMO ENTRADA
      
    PR2 = 255;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1;      // (PASO#3) MODO PWM		
    
                              
    CCPR2L = 0b10011001;         // MSB del duty cicle (27 decimal)  (PASO#4 Estableciendo ciclo de trabajo) 612*
    CCP2CONbits.DC2B0 = 0;       // LSB del duty cicle
    CCP2CONbits.DC2B1 = 0;       //(10 bits de resolución CCPR2L + DC2B0 Y DC2B1)

    PIR1bits.TMR2IF = 0;         //(PASO#5.1) Borrando bandera del TMR2 / Config. e Inicializar TMR2
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;       //(PASO#5.2) PRESCALER 1:16 para el TMR2 		 
    
    T2CONbits.TMR2ON = 1;        //(PASO#5.3) HABILITAMOS EL TMR2
    
    while(PIR1bits.TMR2IF == 1){ //Se espera a que el TMR2 haga un overflow para resetear la bandera del mismo
        PIR1bits.TMR2IF = 0;     //(PASO#6.1) se resetea la bandera del TMR2
    }
     
    TRISCbits.TRISC1 = 0;        // (PASO#6.2) RC1-CCP2 SALIDA PWM --> (Habilitamos la salida del PWM espués de que haya un nuevo ciclo empezado)
}

void init_PWM_1(void){           // Se configura el CCP1 --> RC2 !!! (ESTA CONFIG ES DISTINTA AL OTRO CCP)
    TRISCbits.TRISC2 = 1;        // (PASO#1) ESTABLECER RC2-CCP1 COMO ENTRADA
      
    //PR2 = 200;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;      // (PASO#3) MODO PWM: P1A, P1C active-high; P1B, P1D active-high	

    CCP1CONbits.P1M1   = 0;
    CCP1CONbits.P1M0   = 0;      // (PASO#3.1) Modo PWM "Single Output" !!!! ******************************************* DE ESTO SE DIFERENCIA EL CCP1 DEL CCP2
    
                              
    CCPR1L = 0b10011001;         // MSB del duty cicle (27 decimal)  (PASO#4 Estableciendo ciclo de trabajo)
    CCP1CONbits.DC1B0 = 0;       // LSB del duty cicle
    CCP1CONbits.DC1B1 = 0;       //(10 bits de resolución CCPR2L + DC2B0 Y DC2B1)

    PIR1bits.TMR2IF = 0;         //(PASO#5.1) Borrando bandera del TMR2 / Config. e Inicializar TMR2
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;       //(PASO#5.2) PRESCALER 1:16 para el TMR2 		 
    
    T2CONbits.TMR2ON = 1;        //(PASO#5.3) HABILITAMOS EL TMR2
    
    while(PIR1bits.TMR2IF == 1){ //Se espera a que el TMR2 haga un overflow para resetear la bandera del mismo
        PIR1bits.TMR2IF = 0;     //(PASO#6.1) se resetea la bandera del TMR2
    }
     
    TRISCbits.TRISC2 = 0;        // (PASO#6.2) RC1-CCP2 SALIDA PWM --> (Habilitamos la salida del PWM espués de que haya un nuevo ciclo empezado)
}

void Servo1_grados(uint8_t Servo1_grados){
    switch(Servo1_grados){
        case 0:                             /////////////////////////////////
            CCPR2L = 10; //calculado para 0°
            break;
             
        case 10:
            CCPR2L = 12;
            break;
             
        case 30:
            CCPR2L = 14;
            break;
             
        case 45:
            CCPR2L = 18; //calculado para 45°
            break;
            
        case 90:
            CCPR2L = 25; //calculado para 90° //////////////////////////////
            break;
            
        case 120:
            CCPR2L = 30; //calculado para 
            break;
            
        case 135:
            CCPR2L = 33; //calculado para °
            break;
            
        case 150:
            CCPR2L = 37; //calculado para °
            break;
            
        case 180:                           ///////////////////////////////
            CCPR2L = 40; //calculado para 180°
            break;
            
        default:
            CCPR2L = 10; //calculado para 0°
            
    }
}

void Servo2_grados(uint8_t Servo2_grados){
    switch(Servo2_grados){
        case 0:                             /////////////////////////////////
            CCPR1L = 10; //calculado para 0°
            break;
             
        case 10:
            CCPR1L = 12;
            break;
             
        case 30:
            CCPR1L = 14;
            break;
             
        case 45:
            CCPR1L = 18; //calculado para 45°
            break;
            
        case 90:
            CCPR1L = 25; //calculado para 90° //////////////////////////////
            break;
            
        case 120:
            CCPR1L = 30; //calculado para 
            break;
            
        case 135:
            CCPR1L = 33; //calculado para °
            break;
            
        case 150:
            CCPR1L = 37; //calculado para °
            break;
            
        case 180:                           ///////////////////////////////
            CCPR1L = 40; //calculado para 180°
            break;
            
        default:
            CCPR1L = 10; //calculado para 0°
            
    }
}

void robot_para_adelante(void){
    MOTOR_DERECHO_FRENTE_1 =1;
    MOTOR_DERECHO_FRENTE_2 =0;

    MOTOR_DERECHO_ATRAS_1 =1;
    MOTOR_DERECHO_ATRAS_2 =0;

    MOTOR_IZQUIERDO_FRENTE_1 =1;
    MOTOR_IZQUIERDO_FRENTE_2 =0;

    MOTOR_IZQUIERDO_ATRAS_1 =1;
    MOTOR_IZQUIERDO_ATRAS_2 =0;
}
void robot_para_atras(void){
    MOTOR_DERECHO_FRENTE_1 =0;
    MOTOR_DERECHO_FRENTE_2 =1;

    MOTOR_DERECHO_ATRAS_1 =0;
    MOTOR_DERECHO_ATRAS_2 =1;

    MOTOR_IZQUIERDO_FRENTE_1 =0;
    MOTOR_IZQUIERDO_FRENTE_2 =1;

    MOTOR_IZQUIERDO_ATRAS_1 =0;
    MOTOR_IZQUIERDO_ATRAS_2 =1;
}
void robot_CW90(void){
    robot_STOP();
    __delay_ms(500);    
    MOTOR_DERECHO_FRENTE_1 =0;
    MOTOR_DERECHO_FRENTE_2 =1;

    MOTOR_DERECHO_ATRAS_1 =0;
    MOTOR_DERECHO_ATRAS_2 =1;

    MOTOR_IZQUIERDO_FRENTE_1 =1;
    MOTOR_IZQUIERDO_FRENTE_2 =0;

    MOTOR_IZQUIERDO_ATRAS_1 =1;
    MOTOR_IZQUIERDO_ATRAS_2 =0;
    __delay_ms(440);
    robot_STOP();
}
void robot_CCW90(void){
    robot_STOP();
    __delay_ms(500);    
    MOTOR_DERECHO_FRENTE_1 =1;
    MOTOR_DERECHO_FRENTE_2 =0;

    MOTOR_DERECHO_ATRAS_1 =1;
    MOTOR_DERECHO_ATRAS_2 =0;

    MOTOR_IZQUIERDO_FRENTE_1 =0;
    MOTOR_IZQUIERDO_FRENTE_2 =1;

    MOTOR_IZQUIERDO_ATRAS_1 =0;
    MOTOR_IZQUIERDO_ATRAS_2 =1;
    __delay_ms(440);
    robot_STOP();
}
void robot_STOP(void){
    MOTOR_DERECHO_FRENTE_1 =0;
    MOTOR_DERECHO_FRENTE_2 =0;

    MOTOR_DERECHO_ATRAS_1 =0;
    MOTOR_DERECHO_ATRAS_2 =0;

    MOTOR_IZQUIERDO_FRENTE_1 =0;
    MOTOR_IZQUIERDO_FRENTE_2 =0;

    MOTOR_IZQUIERDO_ATRAS_1 =0;
    MOTOR_IZQUIERDO_ATRAS_2 =0;
}