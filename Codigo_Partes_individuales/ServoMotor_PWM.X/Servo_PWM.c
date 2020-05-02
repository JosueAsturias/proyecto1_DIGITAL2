/*
 * File:   Servo_PWM.c
 * Author: Josue Castillo Lou
 * Mover 2 Servomotores con el modo PWM (CCP1 y CCP2)
 * Created on 11 de marzo de 2020, 12:46 PM 
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
#include <pic16f887.h>

#define _XTAL_FREQ 1000000      // 1MHz


//------- Definición de Variables Globales ----// 
uint8_t ESTADO = 0; 
uint8_t ESTADO2 = 0; 
uint8_t ESTADO3 = 0; 



// Prototipos de Funciones 
void config_PORTS(void);
void OSC_config(uint32_t frecuencia);
void init_PWM_1 (void);
void init_PWM_2 (void);

void Servo_0(void);
void Servo_90(void);
void Servo_180(void);

void Servo1_grados(uint8_t Servo1_grados);
void Servo2_grados(uint8_t Servo2_grados);

void main(void) {
    config_PORTS();
    init_PWM_1();
    init_PWM_2();
   
    
    while (1){
     

        if (PORTEbits.RE0 == 1){        //RUTINA para antirebote
            ESTADO = 1;
            __delay_us(250);
            
          
            if (ESTADO == 1 && PORTEbits.RE0 == 0) {    //Rutina servo 1 (prueba)
                Servo1_grados(0);
                __delay_ms(1000);
                Servo1_grados(10);
                __delay_ms(1000);
                Servo1_grados(30);
                __delay_ms(1000);
                Servo1_grados(45);
                
                __delay_ms(1000);
                Servo1_grados(90);
                __delay_ms(1000);
                Servo1_grados(120);
                __delay_ms(1000);
                Servo1_grados(150);
                __delay_ms(1000);
                Servo1_grados(180);
  

            }
            ESTADO = 0;  
        }
        
        
        if (PORTEbits.RE1 == 1){        ////Servo 1 a 90° (Prueba física-correcta)
            ESTADO2 = 1;
            __delay_us(250);
            
          
            if (ESTADO2 == 1 && PORTEbits.RE1 == 0) {
                //CCPR2L = 47; //calculado para 90°
                Servo1_grados(90);

                
    
            }
            ESTADO2 = 0;  
        }
        
        
        
        if (PORTEbits.RE2 == 1){        // //Rutina servo 2 (prueba)
            ESTADO3 = 1;
            __delay_us(250);
            
            if (ESTADO3 == 1 && PORTEbits.RE2 == 0) {
                 Servo2_grados(0);
                __delay_ms(1000);
                Servo2_grados(10);
                __delay_ms(1000);
                Servo2_grados(30);
                __delay_ms(1000);
                Servo2_grados(45);
                
                __delay_ms(1000);
                Servo2_grados(90);
                __delay_ms(1000);
                Servo2_grados(120);
                __delay_ms(1000);
                Servo2_grados(150);
                __delay_ms(1000);
                Servo2_grados(180);
                

                   
            }
            ESTADO3 = 0;  
        }
          
        

    } 
    return;
}



void config_PORTS(void){ 
    OSC_config(_XTAL_FREQ);
    ANSEL = 0; //SIN APAGAR ANSEL = 0 ; NO SIRVEN PWM's
    //ANSELH = 0;  
    TRISC = 0XFF;
    TRISE = 0b11111111;
    
    PORTA = 0x00;          //Inicialización de Puertos
    PORTB = 0x00;
    PORTC = 0x00;  
    PORTD = 0x00;
    PORTE = 0x00;
 
  
   }

void init_PWM_1(void){            // Se configura el CCP2 --> RC1 !!!
    TRISCbits.TRISC1 = 1;        // (PASO#1) ESTABLECER RC1-CCP2 COMO ENTRADA
    
    //PR2 = 155;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    PR2 = 200;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1;      // (PASO#3) MODO PWM		
    
                              
    CCPR2L = 0b00011011;         // MSB del duty cicle (27 decimal)  (PASO#4 Estableciendo ciclo de trabajo)
    CCP2CONbits.DC2B0 = 1;       // LSB del duty cicle
    CCP2CONbits.DC2B1 = 1;       //(10 bits de resolución CCPR2L + DC2B0 Y DC2B1)

    PIR1bits.TMR2IF = 0;         //(PASO#5.1) Borrando bandera del TMR2 / Config. e Inicializar TMR2
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;       //(PASO#5.2) PRESCALER 1:16 para el TMR2 		 
    
    T2CONbits.TMR2ON = 1;        //(PASO#5.3) HABILITAMOS EL TMR2
    
    while(PIR1bits.TMR2IF == 1){ //Se espera a que el TMR2 haga un overflow para resetear la bandera del mismo
        PIR1bits.TMR2IF = 0;     //(PASO#6.1) se resetea la bandera del TMR2
    }
     
    TRISCbits.TRISC1 = 0;        // (PASO#6.2) RC1-CCP2 SALIDA PWM --> (Habilitamos la salida del PWM espués de que haya un nuevo ciclo empezado)
}

void init_PWM_2(void){           // Se configura el CCP1 --> RC2 !!! (ESTA CONFIG ES DISTINTA AL OTRO CCP)
    TRISCbits.TRISC2 = 1;        // (PASO#1) ESTABLECER RC2-CCP1 COMO ENTRADA
    
    //PR2 = 155;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    PR2 = 200;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;      // (PASO#3) MODO PWM: P1A, P1C active-high; P1B, P1D active-high	

    CCP1CONbits.P1M1   = 0;
    CCP1CONbits.P1M0   = 0;      // Modo PWM "Single Output" !!!! *******************************************
    
                              
    CCPR2L = 0b00011011;         // MSB del duty cicle (27 decimal)  (PASO#4 Estableciendo ciclo de trabajo)
    CCP1CONbits.DC1B0 = 1;       // LSB del duty cicle
    CCP1CONbits.DC1B1 = 1;       //(10 bits de resolución CCPR2L + DC2B0 Y DC2B1)

    PIR1bits.TMR2IF = 0;         //(PASO#5.1) Borrando bandera del TMR2 / Config. e Inicializar TMR2
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;       //(PASO#5.2) PRESCALER 1:16 para el TMR2 		 
    
    T2CONbits.TMR2ON = 1;        //(PASO#5.3) HABILITAMOS EL TMR2
    
    while(PIR1bits.TMR2IF == 1){ //Se espera a que el TMR2 haga un overflow para resetear la bandera del mismo
        PIR1bits.TMR2IF = 0;     //(PASO#6.1) se resetea la bandera del TMR2
    }
     
    TRISCbits.TRISC2 = 0;        // (PASO#6.2) RC1-CCP2 SALIDA PWM --> (Habilitamos la salida del PWM espués de que haya un nuevo ciclo empezado)
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

void Servo_0(void){
    CCPR2L = 10; //calculado para 0°
}

void Servo_90(void){
    CCPR2L = 25; //calculado para 90°
}

void Servo_180(void){
     CCPR2L = 40; //calculado para 180°
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


