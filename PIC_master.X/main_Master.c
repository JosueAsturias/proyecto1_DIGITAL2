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
#include <stdio.h>
#include "I2C.h"
#include "RTC.h"
#include "UART.h"
#include "LCD_8bits.h"
#include "Temperatura_I2C.h"
#include "IMU.h"

#define _XTAL_FREQ 4000000
// variables 

#define _ADDRESS_SLAVE_1_W 0x20
#define _ADDRESS_SLAVE_1_R 0x21   // slave chino
#define _ADDRESS_SLAVE_2_W 0x30
#define _ADDRESS_SLAVE_2_R 0x31   //salve miguel

#define EMPEZAR_S2 0x69
#define TOMAR_DATOS_SUELO 0x25

uint8_t estado = 0;
uint8_t seg = 0;
uint8_t min = 21;
uint8_t hora = 16;
uint8_t dia = 5;
uint8_t datum = 6;
uint8_t mes = 3;
uint8_t jahr = 20;
uint8_t velocidad = 0;
uint8_t humedad = 5;
uint8_t inclinacion = 4;
uint8_t d_frente = 102;
uint8_t d_atras = 102;
int8_t temperatura = 0;
int8_t temperatura_obj = 0;
uint16_t * obj_array;
uint8_t banderaBoton = 0;
uint8_t banderaPUSH1 = 0;
uint8_t banderaPUSH2 = 0;

uint8_t largo = 0;
uint8_t ancho = 0;
int16_t accZ = 0;
char sprintbuffer[];

// caracteres especiales en LCD:
const char arrowr[8] = {
    0x00,
    0x08,
    0x0C,
    0x0E,
    0x0F,
    0x0E,
    0x0C,
    0x08
};
const char arrowr_vacio[8] = {
    0x00,
    0x08,
    0x0C,
    0x0A,
    0x09,
    0x0A,
    0x0C,
    0x08
};
const char atilde[8] = {
    0b00010,
    0b00100,
    0b00000,
    0b01110,
    0b00001,
    0b01111,
    0b10001,
    0b01111
};
const char termometro[8] = {
    0x04,
    0x0A,
    0x0A,
    0x0E,
    0x0E,
    0x1F,
    0x1F,
    0x0E
};
const char gota[8] = {
  0x04,
  0x0A,
  0x0A,
  0x11,
  0x11,
  0x11,
  0x0E,
  0x00  
};
void display_Uhrzeit(uint8_t fila, uint8_t columna);
void display_Datum(uint8_t fila, uint8_t columna);
void mostrarLCD(uint8_t pantalla);
void pressBoton1(void);
void pressBoton2(void);
void SetUp(void);
void OSC_config(uint32_t frecuencia);
uint8_t ver_inclinacion(int16_t valor);

void __interrupt() ISR(void){
    if (INTCONbits.RBIF == 1 && INTCONbits.RBIE == 1){   //atencion IOCB
        INTCONbits.RBIF = 0;
        if (banderaBoton == 0){
            banderaBoton = 1;
            INTCONbits.RBIE = 0;
        }
    }
}

void main(void) {
    SetUp();
    while(1){
        /*Obtener valores de sensores -------------------*/
        get_Time();
        temperatura = temp_ambiente();
        temperatura_obj = temp_objeto();
        accZ = Acc_Z();
        inclinacion = ver_inclinacion(accZ);
        /*-----------------------------------------------*/
        mostrarLCD(estado);
        pressBoton1();
        pressBoton2();
        
        uartTX_Write(125);
        uartTX_Write(hora);
        uartTX_Write(min);
        uartTX_Write(seg);
        uartTX_Write(temperatura);
        uartTX_Write(temperatura_obj);  //signed int **
        uartTX_Write(inclinacion);
        uartTX_Write(humedad);
        uartTX_Write(d_frente);
        uartTX_Write(d_atras);
    }
    return;
}

void SetUp(void){
    TRISC = 0;
    PORTC = 0;
    TRISB = 0;
    OSC_config(_XTAL_FREQ);
    TRISB = 0b00000110;
    ANSELH = 0;
    WPUB = 0b00000110;
    OPTION_REGbits.nRBPU = 0;
    IOCB = 0b00000110;
    INTCONbits.RBIE = 1;
    INTCONbits.GIE = 1;
    TRISD = 0;
    TRISC0 = 0;
    TRISC1 = 0;
    LCD_init();
    LCD_Create_Char(0, atilde);
    LCD_Create_Char(1, arrowr);
    LCD_Create_Char(2, termometro);
    LCD_Create_Char(3, gota);
    LCD_Create_Char(4, arrowr_vacio);
    LCD_clear();
    uart_init(9600);
    I2C_Master_Init(100000);
    IMU_init();
    
    //Zeit_Datum_Set();
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

void mostrarLCD(uint8_t pantalla){
    switch(pantalla){
        case 0:
            display_Uhrzeit(2,4);
            display_Datum(1,3);
            break;
        case 1:
            LCD_Set_Cursor(1,0);
            LCD_Write_String("Ambiente: Suelo:");
            LCD_Set_Cursor(2,2);
            LCD_Write_Character(2);
            obj_array = uint_to_array(temperatura);
            if (obj_array[0] == 0){
                LCD_Write_Character(' ');
            }
            else {
                LCD_Write_Character('0' + obj_array[0]);
            }
            LCD_Write_Character('0' + obj_array[1]);
            LCD_Write_Character('0' + obj_array[2]);
            LCD_Write_Character(223);
            LCD_Write_Character('C');
            
            LCD_Set_Cursor(2,10);
            LCD_Write_Character(2);
            obj_array = uint_to_array(temperatura_obj);
            if (obj_array[0] == 0){
                LCD_Write_Character(' ');
            }
            else {
                LCD_Write_Character('0' + obj_array[0]);
            }
            
            if (obj_array[1] == 0 && obj_array[0] == 0){
                LCD_Write_Character(' ');
            }
            else{
                LCD_Write_Character('0' + obj_array[1]);
            }
            LCD_Write_Character('0' + obj_array[2]);
            LCD_Write_Character(223);
            LCD_Write_Character('C');
            break;
        case 2:
            LCD_Set_Cursor(1,0);
            LCD_Write_String("Inclinacion:");
            LCD_Set_Cursor(2, 5);
//            sprintf(sprintbuffer, " %d   ", accZ);
//            LCD_Write_String(sprintbuffer);
            
            LCD_Set_Cursor(2, 5);
            if(inclinacion == 0){
                LCD_Write_String("Estable   ");
            }
            else if (inclinacion == 90){
                LCD_Write_String("Peligro!   ");
            }
            else if (inclinacion == 180){
                LCD_Write_String("EMERGENCIA!  ");
            }
            
            break;
        case 3:
            LCD_Set_Cursor(1,0);
            LCD_Write_String("Humedad:");
            LCD_Set_Cursor(2,4);
            LCD_Write_Character(3);
            LCD_Write_Character(' ');
            uint8_t decenas_humedad = humedad/10;
            uint8_t unidades_humedad = humedad%10;
            if(decenas_humedad == 0){
                LCD_Write_Character(' ');
            }
            else{
                LCD_Write_Character('0'+decenas_humedad);
            }
            LCD_Write_Character('0'+unidades_humedad);
            LCD_Write_Character('%');
            break;
        case 4:
            LCD_Set_Cursor(1,0);
            LCD_Write_String("Atr");
            LCD_Write_Character(0);
            LCD_Write_String("s: | Frente:");
            LCD_Set_Cursor(2,2);
            if(d_frente < 100){
                uint8_t dec_frente = d_frente/10;
                uint8_t uni_frente = d_frente%10;
                if(dec_frente == 0){
                    LCD_Write_Character(' ');
                }
                else{
                    LCD_Write_Character('0' + dec_frente);
                }
                LCD_Write_Character('0' + uni_frente);
                LCD_Write_String("cm");
            }
            else{
                LCD_Write_String("----");
            }
            
            LCD_Set_Cursor(2,7);
            LCD_Write_Character('|');
            LCD_Set_Cursor(2,11);
            if(d_atras < 100){
                uint8_t dec_atras = d_atras/10;
                uint8_t uni_atras = d_atras%10;
                if(dec_atras == 0){
                    LCD_Write_Character(' ');
                }
                else{
                    LCD_Write_Character('0' + dec_atras);
                }
                LCD_Write_Character('0' + uni_atras);
                LCD_Write_String("cm");
            }
            else{
                LCD_Write_String("----");
            }
            break;
        case 5:
            LCD_Set_Cursor(1, 0);
            LCD_Write_String("Tomar datos:");
            LCD_Set_Cursor(2,1);
            LCD_Write_Character(velocidad + '0');
            LCD_Set_Cursor(2,8);
            LCD_Write_Character('0' + largo);
            LCD_Write_String("x ");
            LCD_Write_Character('0' + ancho);
            break;
        case 6:
            LCD_Set_Cursor(2,0);
            LCD_Write_Character(4);
            LCD_Write_Character(velocidad + '0');
            break;
        case 7:
            LCD_Set_Cursor(2,0);
            LCD_Write_Character(' ');
            LCD_Set_Cursor(2,7);
            LCD_Write_Character(4);
            LCD_Write_Character('0' + largo);
            break;
        case 8:
            LCD_Set_Cursor(2,7);
            LCD_Write_Character(' ');
            LCD_Set_Cursor(2,10);
            LCD_Write_Character(4);
            LCD_Write_Character('0' + ancho);
            break;
        case 9:
            LCD_Set_Cursor(2,10);
            LCD_Write_Character(' ');
            LCD_Set_Cursor(2,15);
            LCD_Write_Character(1);
            break;
        case 10:
            LCD_Set_Cursor(1,0);
            LCD_Write_String("Vamonos Perros!");
            //mandar instruccion a slave de movimiento
            I2C_Master_Start();
            I2C_Master_Write(_ADDRESS_SLAVE_2_W);
            I2C_Master_Write(EMPEZAR_S2);	
            I2C_Master_Write(largo);
            I2C_Master_Write(ancho);
            I2C_Master_Write(velocidad);
            I2C_Master_Stop();
            
            __delay_ms(500);
            LCD_clear();
            estado = 0;
            break;
        default:
            LCD_Set_Cursor(1,9);
            LCD_Write_String("ERROR");
    }
}

void pressBoton1(){
    if (banderaBoton == 1){
        if (banderaPUSH1 == 0){
            if (PORTBbits.RB1 == 0){
                switch (estado){
                    case 6:
                        velocidad ++;
                        if (velocidad > 3){
                            velocidad = 1;
                        }
                        break;
                    case 7:
                        largo ++;
                        if (largo > 8){
                            largo = 1;
                        }
                        break;
                    case 8:
                        ancho ++;
                        if (ancho > 8){
                            ancho = 1;
                        }
                        break;
                    default:
                        LCD_clear();
                    __delay_ms(10);
                    estado ++;
                    if (estado > 5){
                        estado = 0;
                    }
                }
                banderaBoton = 0;
                banderaPUSH1 = 1;
                INTCONbits.RBIE = 1;
            }  
        }  
    }
    if (banderaPUSH1 == 1){
        if (PORTBbits.RB1 == 1){
        __delay_ms(10);
        banderaPUSH1 = 0;
        }
    }    
}

void pressBoton2(void){
    if (banderaBoton == 1){
        if (banderaPUSH2 == 0){
            if (PORTBbits.RB2 == 0){
                switch(estado){
                    case 5:
                        estado = 6;
                        break;
                    case 6:
                        LCD_Set_Cursor(2,0);
                        LCD_Write_Character(1);
                        __delay_ms(100);
                        estado = 7;
                        break;
                    case 7:
                        LCD_Set_Cursor(2,7);
                        LCD_Write_Character(1);
                        __delay_ms(100);
                        estado = 8;
                        break;
                    case 8:
                        LCD_Set_Cursor(2,10);
                        LCD_Write_Character(1);
                        __delay_ms(100);
                        estado = 9;
                        break;
                    case 9:
                        LCD_clear();
                        estado = 10;
                        break;
                    default:
                        __delay_ms(10);
                }
            }
                banderaBoton = 0;
                banderaPUSH2 = 1;
                INTCONbits.RBIE = 1;
            }  
        }  
    if (banderaPUSH2 == 1){
        if (PORTBbits.RB2 == 1){
        __delay_ms(10);
        banderaPUSH2 = 0;
        }
    } 
}

uint8_t ver_inclinacion(int16_t valor){
    uint8_t posicion = 0;
    if (valor>1900){
        posicion = 0;
    }
    else if (valor<1900 && valor > -800){
        posicion = 90;
    }
    else if (valor < -800){
        posicion = 180;
    }
    return posicion;
}