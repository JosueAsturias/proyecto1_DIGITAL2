/*
 * File:   Slave_Lou.c
 * Author: Josué Castillo Lou
 * Este PIC Esclavo es para el proyecto #1 del curso digital 2
 * Funciones Slave_lou: Mover 2 Servomotores, mover 1 motor Stepper e implementar un sensor de humedad YL-69 
 * Created on 15 de marzo de 2020, 07:15 PM ...
 */

//-------- Configuracion Inicial ---------------// 
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
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
#include <pic16f887.h>
#include "I2C.h"                //Librería I2C del Slave 1  
#include "PWMs.h"                //Librería para los 2 PWMs (CCP1 & CCP2) 
#include "STEPPER_MITSUMI.h"    //Librería para MOTOR STEPPER MITSUMI M42SP-4TKC (3.6°/Paso)


#define _XTAL_FREQ 1000000      // 1MHz

//------- Definición de Variables Globales ----// 
uint8_t Humedad_H = 0; 

uint8_t z = 0;              //variable para el I2C


uint8_t recibir = 0;
uint8_t indicador = 0;

// Prototipos de Funciones 
void config_PORTS(void);
void config_ADC(void);
void OSC_config(uint32_t frecuencia);

void __interrupt() ISR(void) {
    if (ADIF) {                                         //Se revisa la bandera del ADC "ADIF" que se enciende cada vez que hace una conversión!
        Humedad_H = ADRESH ;
        PORTB = Humedad_H;
        PIR1bits.ADIF = 0;                              //Se apaga la bandera cada vez que convierte                            
    }
    
    
    if(PIR1bits.SSPIF == 1){                            //Interrupción I2C

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            recibir = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            
            if (recibir == 0xF0){
                indicador =1;
            }
            __delay_us(250); }
            
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = Humedad_H;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    
  
    
}


void main(void) {
    OSC_config(_XTAL_FREQ);
    config_PORTS();
    init_PWM_1();               //Se inicializa el CCP2 --> RC1
    init_PWM_2();               //Se inicializa el CCP1 --> RC2
    config_ADC();               //ADC Para sensor de humedad YL69              
    I2C_Slave_Init(0x20);       //Le asignamos la dirección 0x20 al Slave_Lou 

    while(1){
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1){   
        }

        Servo1_grados(0);   
        Servo2_grados(0);
        
        if(PORTEbits.RE0 == 1){
            Stepper_CCW(90);
            __delay_ms(10);
            for (int i = 0; i<60; i++){
                Servo1_grados(45);
                __delay_ms(10);
                Servo2_grados(180);
                ADCON0bits.GO = 1;
                while(ADCON0bits.GO == 1){   
                }
            }
            __delay_ms(500);
            Stepper_CW(90);
            Servo1_grados(0);   
            Servo2_grados(0);
            indicador = 0;
            while(PORTEbits.RE0 == 1);
        }

    }     
    return;
}


void config_PORTS(void){ 
    OSC_config(_XTAL_FREQ);
    //ANSELH = 0; 
    PORTA = 0;
    TRISA = 0b00000001;    // RA0 como entrada 
    ANSEL = 0b00000001;
    
    PORTB = 0;
    TRISB = 0x00; 
    ANSELH = 0;
    
    PORTD = 0;
    TRISD = 0x00;          //A, B, C ,D Bobinas del Motor Steper
    
    PORTE = 0;
    TRISE = 0b1111; 
    
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

void config_ADC(void){
    
    //ANSELbits.ANS0   = 1;   //Encendemos el ANSEL_L para ANALOG_INPUT en RA0 (El módulo ADC tiene 4 registros: ADRESH,ADRESL. ADCON0 Y ADCON registros de control)
    //TRISAbits.TRISA0 = 1;   //Entrada Analógica Asociada (RA0) para la conversión ADC
    
    ADCON0bits.CHS0  = 0;   //Seleccionamos el CANAL para usar el RA0    
    ADCON0bits.CHS0  = 0;   // CHS<3:0> 4 Bits de selección de canal ya que son 14 puertos analogicos 
    ADCON0bits.CHS0  = 0;
    ADCON0bits.CHS0  = 0;
    
    ADCON1bits.ADFM  = 0;   //Justificación a la izquierda << para usar ANSEL_L
    ADCON1bits.VCFG0 = 0;   //VDD COMO REFERENCIA VREF+ (5V)
    ADCON1bits.VCFG1 = 0;   //GND COMO REFERENCIA VREF- (0v)
    
    ADCON0bits.ADCS1 = 0;   //Seleccionamos el FOSC/2 correspondiente a los 1MHz del XTAL_FREQ (2uS)
    ADCON0bits.ADCS0 = 0; 
    
    //Config_Interrupcion ADC 
    INTCONbits.GIE   = 1;   //Se habilitan las Configuraciones Globales
    INTCONbits.PEIE  = 1;   //Interrupción Periférica del ADC (ON)
    PIE1bits.ADIE    = 1;   //Habilita la Interrupción del ADC
    PIR1bits.ADIF    = 0;   //InterruptADC_Flag (Se enciende cada vez que termina una conversión)
    
    ADCON0bits.ADON  = 1;   // Habilitamos el Módulo ADC en general
}


