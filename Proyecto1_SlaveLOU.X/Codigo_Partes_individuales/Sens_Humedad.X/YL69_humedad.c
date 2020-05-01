/*
 * File:   YL69_humedad.c
 * Author: Josue
 *
 * Created on 14 de marzo de 2020, 09:23 PM
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

// -------- PUERTOS DE LA LCD -----------------
#define RS RA3
#define EN RA4
#define D0 RB0
#define D1 RB1
#define D2 RB2
#define D3 RB3
#define D4 RB4
#define D5 RB5
#define D6 RB6
#define D7 RB7


//-------- Librerías a utilizar ---------------// 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "Library_LCD.h"

#define _XTAL_FREQ 1000000      // 4MHz


//------- Definición de Variables Globales ----// 
uint16_t Humedad_H = 0; 
uint16_t Humedad_L = 0; 
uint16_t *voltaje_map = 0; 
//float    Sens_Humedad = 0; 
//uint16_t Sens_Humedad_inv = 0; 
//float    LCD_Humedad =0;

uint16_t Sens_Humedad = 0; 
uint16_t LCD_Humedad  = 0; 
uint8_t uni  = 0; 
uint8_t dec  = 0; 
uint8_t cen  = 0; 

uint8_t ESTADO = 0; 
uint8_t ESTADO2 = 0; 
uint8_t ESTADO3 = 0; 


char centena = 0;               //variables para mostrar en la LCD
char decena = 0;
char unidad = 0;



// Prototipos de Funciones 
void config_PORTS(void);
void config_ADC(void);
void OSC_config(uint32_t frecuencia);
void LCD_WRITE_INT(unsigned int numero);
void LCD_WRITE_Float(float numero2);
uint16_t *mapeo(uint8_t valor, uint8_t limReal, uint8_t limSup);


void __interrupt() ISR(void) {
    if (ADIF) {                                         //La bandera del ADC "ADIF" se enciende cada vez que hace una conversión!
        PIR1bits.ADIF = 0;                              //Se apaga la bandera cada vez que convierte
        
        
                                  
    }
  
    
}


void main(void) {
    config_PORTS();
    config_ADC();
    
    LCD_iniciar();                      //Inicializamos la LCD
    LCD_CLEAR();
    LCD_CURSOR(1,1);
    LCD_STRING ("Sensor Hum. YL69");
    
  
     // Loop infinito
    while (1){
        
        LCD_CURSOR(1,1);
        LCD_STRING ("Sensor Hum. YL69");
        
        ADCON0bits.GO = 1;
        Humedad_H = ADRESH ;       
        Humedad_L = ADRESL ;
        Sens_Humedad = (Humedad_H << 2) | (Humedad_L >> 6);  //Variable que concatena los 10Bits del ADC
        //Sens_Humedad_inv = (!Sens_Humedad) & (0x3FF); //0x3FF --> 1023
        //LCD_Humedad = (100*Sens_Humedad)/1023;
        
        
        voltaje_map = mapeo(Humedad_H, 255,1);               //mapeo con ADC de  8Bits 
        cen = voltaje_map[0];                                // se guardan en dos variables de diferente tipo para propositos distintos
        dec = voltaje_map[1];
        uni = voltaje_map[2];
        
        centena = uint_to_char(cen);                         // ej: decena (tipo char --> para LCD) y dec (tipo uint8 --> para comparacion en if's)
        decena  = uint_to_char(dec);         
        unidad  = uint_to_char(uni);
       
        LCD_CURSOR(2,1);
        LCD_CHAR(centena);
        LCD_CHAR(decena);
        LCD_CHAR(unidad);
        LCD_CHAR('%');
        
       // LCD_CLEAR();
        
        
        if (PORTEbits.RE2 == 1){        //RUTINA para antirebote
            ESTADO3 = 1;
            __delay_us(250);
            
            if (ESTADO3 == 1 && PORTEbits.RE2 == 0) {
                PORTCbits.RC2 = 1;

            }
            ESTADO3 = 0;  
        }
        
        
        if (Humedad_H == 0xFF){                    //100%
            LCD_CURSOR(2,7);
            LCD_STRING("  0%");
            PORTCbits.RC0 = 0;
            PORTCbits.RC1 = 1;
             
        }
        
        
        if ((dec ==  9 || dec == 0)  && (uni >= 6 || uni == 0) ){              
            LCD_CURSOR(2,7);
            LCD_STRING("  0%");

        }
        
        if ( dec ==  9  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 5 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 10%");
            }

        }
        
        if ( dec ==  8  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 20%");
            }

        }
        
        if ( dec ==  7  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 30%");
            }

        }
        
        if ( dec ==  6  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 40%");
            }

        }
        
        if ( dec ==  5  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 50%");
            }

        }
        
        if ( dec ==  4  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 60%");
            }

        }
        
        if ( dec ==  3  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 70%");
            }

        }
        
        if ( dec ==  2  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 80%");
            }

        }
        
        if ( dec ==  1  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING(" 90%");
            }

        }
        
        if ( dec ==  0  ){                 // rango entre 89 > # >= 80 para 10%  Humedad
            if(uni >= 0 && uni < 9 ){
            LCD_CURSOR(2,7);
            LCD_STRING("100%");
            }

        }
        
        
        if (cen == 0 ){                    //100%
            if(dec == 0 && uni == 0 ){
            LCD_CURSOR(2,7);
            LCD_STRING("100%");
            PORTCbits.RC0 = 1;
            PORTCbits.RC1 = 0;
            }
        }
         
        
        
        /*
        else {
            LCD_CLEAR();
            LCD_CURSOR(2,7);
        }
        */

        
       
        
        
        
    }
    

    return;
}


void config_PORTS(void){ 
    OSC_config(_XTAL_FREQ);
    TRISD = 0x00;    
    TRISC = 0x00;    
    TRISA = 0b00000001;    // RA0 como entrada 
    ANSELbits.ANS5 = 0;
    ANSELbits.ANS6 = 0;
    ANSELbits.ANS7 = 0;
    
    ANSELbits.ANS3 = 0;
    ANSELbits.ANS4 = 0;
    TRISAbits.TRISA3 = 0;           //PIN RS (LCD)
    TRISAbits.TRISA4 = 0;           //PIN EN (LCD)
    TRISB = 0;                      //Puerto para la LCD
    TRISE = 0xFF;          //PORTE todo entradas 
    
    PORTA = 0x00;          //Inicialización de Puertos
    PORTB = 0x00;
    PORTC = 0x00;  
    PORTD = 0x00;
    PORTE = 0x00;
 
  
   }

void config_ADC(void){
    ADCON0bits.ADON  = 1;   // Habilitamos el Módulo ADC en general
    
    
    ANSELbits.ANS0   = 1;   //Encendemos el ANSEL_L para ANALOG_INPUT en RA0 (El módulo ADC tiene 4 registros: ADRESH,ADRESL. ADCON0 Y ADCON registros de control)
    TRISAbits.TRISA0 = 1;   //Entrada Analógica Asociada (RA0) para la conversión ADC
    
    ADCON0bits.CHS0  = 0;   //Seleccionamos el CANAL para usar el RA0    
    ADCON0bits.CHS0  = 0;   // CHS<3:0> 4 Bits de selección de canal ya que son 14 puertos analogicos 
    ADCON0bits.CHS0  = 0;
    ADCON0bits.CHS0  = 0;
    
    ADCON1bits.ADFM  = 0;   //Justificación a la izquierda << para usar ANSEL_L
    ADCON1bits.VCFG0 = 0;   //VDD COMO REFERENCIA VREF+ (5V)
    ADCON1bits.VCFG1 = 0;   //GND COMO REFERENCIA VREF- (0v)
    
    ADCON0bits.ADCS1 = 1;   //Seleccionamos el FOSC/8 correspondiente a los 4MHz del XTAL_FREQ (2uS)
    ADCON0bits.ADCS0 = 0; 
    
    //Config_Interrupcion ADC 
    INTCONbits.GIE   = 1;   //Se habilitan las Configuraciones Globales
    INTCONbits.PEIE  = 1;   //Interrupción Periférica del ADC (ON)
    PIE1bits.ADIE    = 1;   //Habilita la Interrupción del ADC
    PIR1bits.ADIF    = 0;   //InterruptADC_Flag (Se enciende cada vez que termina una conversión)
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

void LCD_WRITE_INT(unsigned int numero){
    char buffer [16];
    sprintf (buffer, "%d", numero); //Convierte el int en string
    LCD_STRING(buffer);
}

void LCD_WRITE_Float(float numero2){
    char buffer2 [16];
    sprintf (buffer2, "%.2f", numero2); //Convierte el int en string
    LCD_STRING(buffer2);
}

uint16_t *mapeo(uint8_t valor, uint8_t limPIC, uint8_t limFisico){                 //Valor = ValADC , LimPIC es el límite que entiende el PIC 255, limFisico = 5Volts alimentacion max!
    uint16_t result[3] = {0,0,0};  // cen.dec.uni  [cen, dec, uni]   
    uint16_t dividendo = valor*limFisico;
    while (limPIC <= dividendo){
        result[0] = result[0] + 1;
        dividendo = dividendo - limPIC;
    }
    dividendo = dividendo *10;
    while (limPIC <= dividendo){
        result[1] = result[1] +1;
        dividendo = dividendo - limPIC;
    }
    dividendo = dividendo *10;
    while (limPIC <= dividendo){
        result[2] = result[2] +1;
        dividendo = dividendo - limPIC;
    }
    
    return(result);
}