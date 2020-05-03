/*
 * File:   RASP.c
 * Author: Miguel García
 *
 * Created on 2 de mayo de 2020, 18:10
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
#include "SPI.h"
#define _XTAL_FREQ 4000000
#define RS PORTAbits.RA0
#define EN PORTAbits.RA1

#define DATA PORTD


uint8_t distancia_ade = 0;
uint8_t distancia_atr = 0;
uint8_t humedad = 0;
uint8_t posicion = 0;
int8_t temp_amb = 0;
int8_t temp_obj = 0;
uint8_t hora = 0;
uint8_t minutos = 0;
uint8_t segundos = 0;

uint8_t contador = 0;
uint8_t contador1 = 0;
uint8_t recibir1 = 0;
uint16_t *obj_array;

void config(void);
void oscillator(uint8_t a);
void Lcd_Init(void);
void Lcd_Cmd(uint8_t a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(uint8_t a, uint8_t b);
void Lcd_Write_String(char *a);
void Lcd_Write_Char(char a);
uint16_t * uint_to_array(uint8_t numero);

void __interrupt() isr(void){
    
    if (INTCONbits.RBIF == 1){
        if (PORTBbits.RB0==0){    
            contador1++;
            if(contador1 == 4){
                contador1 = 0;
            }
        }
        
        INTCONbits.RBIF = 0;   
    }
    
    if (PIR1bits.SSPIF == 1){ 
        recibir1 = spiRead();
        
        if(recibir1 == 125){
            contador = 0;
        }
                
        if (contador == 1){
            spiWrite(0);
            hora = recibir1;
        }
        if (contador == 2){
            spiWrite(0);
            minutos = recibir1;
        }
        if (contador == 3){
            spiWrite(0);
            segundos = recibir1;
        }
        if (contador == 4){
            spiWrite(0);
            temp_amb = recibir1;
        }
        if (contador == 5){
            spiWrite(0);
            temp_obj = recibir1;
        }
        if (contador == 6){
            spiWrite(0);
            posicion = recibir1;
        }
        if (contador == 7){
            spiWrite(0);
            humedad = recibir1;
        }
        if (contador == 8){
            spiWrite(0);
            distancia_ade = recibir1;
        }
        if (contador == 9){
            spiWrite(0);
            distancia_atr = recibir1;
        }
        PIR1bits.SSPIF = 0;
        contador ++;
        }
    return;
}

void main(void) {
    config();
    while(1){
//        while (contador1 == 0){
            char seg_u = segundos%10;
            char seg_d = segundos/10;
            char min_u = minutos%10;
            char min_d = minutos/10;
            char Uhr_u = hora%10;
            char Uhr_d = hora/10;

            Lcd_Set_Cursor(1,4);
            Lcd_Write_Char(Uhr_d + '0');
            Lcd_Write_Char(Uhr_u + '0');
            Lcd_Write_Char(':');
            Lcd_Write_Char(min_d + '0');
            Lcd_Write_Char(min_u + '0');
            Lcd_Write_Char(':');
            Lcd_Write_Char(seg_d + '0');
            Lcd_Write_Char(seg_u + '0');

            char hume_u = humedad%10;
            char hume_d = humedad/10;
            Lcd_Set_Cursor(2,4);
            Lcd_Write_String("Humedad: ");
            Lcd_Write_Char(hume_d + '0');
            Lcd_Write_Char(hume_u + '0');
  //      }
        __delay_ms(2000);
        Lcd_Clear();
        //while (contador1 == 1){
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Ambiente: Suelo:");
            Lcd_Set_Cursor(2,2);
            Lcd_Write_Char(2);
            obj_array = uint_to_array(temp_amb);
            if (obj_array[0] == 0){
                Lcd_Write_Char(' ');
            }
            else {
                Lcd_Write_Char('0' + obj_array[0]);
            }
            Lcd_Write_Char('0' + obj_array[1]);
            Lcd_Write_Char('0' + obj_array[2]);
            Lcd_Write_Char(223);
            Lcd_Write_Char('C');
            
            Lcd_Set_Cursor(2,10);
            Lcd_Write_Char(2);
            obj_array = uint_to_array(temp_obj);
            if (obj_array[0] == 0){
                Lcd_Write_Char(' ');
            }
            else {
                Lcd_Write_Char('0' + obj_array[0]);
            }
            Lcd_Write_Char('0' + obj_array[1]);
            Lcd_Write_Char('0' + obj_array[2]);
            Lcd_Write_Char(223);
            Lcd_Write_Char('C');
            
        //}
        __delay_ms(2000);
        Lcd_Clear();
        //while (contador1 == 2){
            char ade_u = distancia_ade%10;
            char ade_d = distancia_ade/10;

            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Adelante: ");           
            Lcd_Write_Char(ade_d + '0');
            Lcd_Write_Char(ade_u + '0');
            Lcd_Write_String(" cm"); 
            
            char atr_u = distancia_atr%10;
            char atr_d = distancia_atr/10;            
            
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("Atras: ");
            Lcd_Write_Char(atr_d + '0');
            Lcd_Write_Char(atr_u + '0');
            Lcd_Write_String(" cm");            
        //}
        __delay_ms(2000);
        Lcd_Clear();
        //while (contador1 == 3){
            char pos_u = posicion%10;
            char pos_d = posicion/10;
            char pos_c = posicion/100;
            
            
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Inclinacion: ");   
            Lcd_Set_Cursor(2,6);
            if(posicion >100){
                Lcd_Write_Char(pos_c + '0');
            }
            Lcd_Write_Char(pos_d + '0');
            Lcd_Write_Char(pos_u + '0'); 
            Lcd_Write_Char(223);
        //}
        __delay_ms(2000);    
        Lcd_Clear();
    }
    return;
}


void config(void){
    oscillator(6);
    PORTD = 0;
    TRISD = 0;
    PORTA = 0;
    TRISA = 0;
    ANSEL = 0;
    PORTB = 0x00;
    TRISB = 0b11111111;
    ANSELH = 0x00;
    WPUB = 0b11111111;
    OPTION_REGbits.nRBPU = 0;  //Puertp B como entrada con PULL-UP
    IOCB = 0x03;                //Habilitamos las interrupciones para el puerto B en RB0 y RB1
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    PIR1bits.SSPIF = 0;
    Lcd_Init();
    Lcd_Clear();
    
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


void Lcd_Cmd(uint8_t a) {
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

void Lcd_Set_Cursor(uint8_t a, uint8_t b)
{
	uint8_t d;
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
    uint8_t n;
    for(n = 0; a[n] != '\0'; n++){
        Lcd_Write_Char(a[n]);
    }
}

uint16_t * uint_to_array(uint8_t numero){
    uint16_t resultado[3] = {0,0,0};
    resultado[0] = numero/100;
    uint8_t centenas = resultado[0];
    resultado[1] = (numero - (centenas *100))/10;
    uint8_t decenas = resultado[1];
    resultado[2] = numero -(centenas*100+decenas*10);
    return(resultado);
}