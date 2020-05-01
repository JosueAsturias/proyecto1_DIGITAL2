/*
 * File:   Stepper3.c
 * Author: Josué Castillo Lou 
 * Este código es para un motor stepper bipolar (2 bobinas: 4 cables) 
 * MOTOR STEPPER MITSUMI M42SP-4TKC (3.6°/Paso)
 * Para el correcto funcionamiento de este codigo en el Stepper hay que utilizar un PUENTE H "L293D" (un Puente-H para cada bobina) 
 * Created on 10 de marzo de 2020, 01:29 PM 
 */ 


//-------- Configuracion Inicial ---------------// 
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


//-------- Librerías a utilizar ---------------// 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "STEPPER_MITSUMI.h"    //Librería para MOTOR STEPPER MITSUMI M42SP-4TKC (3.6°/Paso)

#define _XTAL_FREQ 1000000      // 4MHz


//------- Definición de Variables Globales ----// 
uint8_t count = 0; 
uint8_t flag = 0; 
uint8_t ESTADO = 0; 
uint8_t ESTADO2 = 0; 
uint8_t ESTADO3 = 0; 
int i = 0 ; 

unsigned int Paso2BLE_CW [4] = {  //Array para secuencia de pasoso en sentido horario (CW --> Counter Wise)
       // ABCD
        0b1010, //1
        0b0110, //2
        0b0101, //3
        0b1001, //4
    };

unsigned int Paso2BLE_CCW [4] = {  //Array para secuencia de pasos en sentido anti-horario (CCW --> Counter Clock Wise)
        0b1001, //1
        0b0101, //2
        0b0110, //3
        0b1010, //4  
    };

// Prototipos de Funciones 
void config_PORTS(void);
void OSC_config(uint32_t frecuencia);

void main(void) {
    config_PORTS();
   
    
    while (1){

        if (PORTAbits.RA0 == 1){        //RUTINA para antirebote
            ESTADO = 1;
            __delay_us(250);
            
          
            if (ESTADO == 1 && PORTAbits.RA0 == 0) {
                 Stepper_CW(360);
                 __delay_ms(1000);
                 Stepper_CW(180);
                 __delay_ms(1000);
                 Stepper_CW(90);
                 __delay_ms(1000);
                 Stepper_CW(45);
                 __delay_ms(1000);
                 Stepper_CW(15);
                 __delay_ms(1000);

            }
            ESTADO = 0;  
        }
        
        
        if (PORTAbits.RA1 == 1){        //RUTINA para antirebote
            ESTADO2 = 1;
            __delay_us(250);
            
          
            if (ESTADO2 == 1 && PORTAbits.RA1 == 0) {
                
                for(i=0; i<8; i++){         //
                    PORTD = Paso2BLE_CW[count];
                    __delay_ms(20);   
                    count++;
                
                    if (count == 4) {
                        __delay_ms(10);  
                        count = 0;
                    }
                    
                }
                   
            }
            ESTADO2 = 0;  
        }
        
        
        if (PORTAbits.RA2 == 1){        //RUTINA para antirebote
            ESTADO3 = 1;
            __delay_us(250);
            
            if (ESTADO3 == 1 && PORTAbits.RA2 == 0) {
                Stepper_CCW(360);
                __delay_ms(1000);
                Stepper_CCW(180);
                __delay_ms(1000);
                Stepper_CCW(90);
                __delay_ms(1000);
                Stepper_CCW(45);
                __delay_ms(1000);
                Stepper_CCW(15);
                __delay_ms(1000);
                   
            }
            ESTADO3 = 0;  
        }
        

    } 
    return;
}



void config_PORTS(void){ 
    OSC_config(_XTAL_FREQ);
    ANSEL =  ANSELH = 0;
    TRISD = 0x00;          //A, B, C ,D Bobinas del Motor Steper
    TRISA = 0b00001111;   
    
    PORTA = 0x00;          //Inicialización de Puertos
    PORTB = 0x00;
    PORTC = 0x00;  
    PORTD = 0x00;
    PORTE = 0x00;
 
  
   }

void OSC_config(uint32_t frecuencia){
    switch(frecuencia){
        case 8000000:
            OSCCONbits.IRCF = 0b111;
            break;
        case 4000000:
            OSCCONbits.IRCF = 0b110;
            break;
        case 2000000:
            OSCCONbits.IRCF = 0b101;
            break;
        case 1000000:
            OSCCONbits.IRCF = 0b100;
            break;
        case 500000:
            OSCCONbits.IRCF = 0b011;
            break;
        case 250000:
            OSCCONbits.IRCF = 0b010;
            break;
        case 125000:
            OSCCONbits.IRCF = 0b001;
            break;
        case 32000:
            OSCCONbits.IRCF = 0b000;
            break;
        default:
            OSCCONbits.IRCF = 0b110;
    }
}



