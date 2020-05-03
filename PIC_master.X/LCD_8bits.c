/*
 * File:   LCD_8bits.c
 * Author: josue
 *
 * Created on February 5, 2020, 12:37 PM
 */


#include <xc.h>
#include <stdint.h>
#include "LCD_8bits.h"

#define _XTAL_FREQ 4000000   // cambiar de acuerdo a la configuracion del OSC

#define RS PORTAbits.RA0
#define EN PORTAbits.RA1

#define DATA_PORT PORTD 


void LCD_Cmd(uint8_t comando){
    RS = 0;
    __delay_ms(5);
    EN = 1;
    __delay_ms(5);
    DATA_PORT = comando;
    __delay_ms(5);
    EN = 0;
    __delay_ms(5);
}

void LCD_clear(void){
    LCD_Cmd(0x00);
    LCD_Cmd(0x01);
    __delay_ms(4);
}

void LCD_home(void){
    LCD_Cmd(0x00);
    LCD_Cmd(0x02);
    __delay_ms(4);
}

void LCD_init(void){
    __delay_ms(20);
    LCD_Cmd(0x30);
    __delay_ms(5);
    LCD_Cmd(0x30);
    __delay_us(100);
    LCD_Cmd(0x30);
    __delay_us(100);
    LCD_Cmd(0x38);
    __delay_us(53);
    LCD_Cmd(0x08);
    __delay_us(53);
    LCD_Cmd(0x01);
    __delay_ms(3);
    LCD_Cmd(0x06);
    __delay_us(53);
    LCD_Cmd(0x0C);
    __delay_us(53);
}

void LCD_Write_Character(char caracter){
    RS = 1;
    // LCD_Port(caracter);
    DATA_PORT = caracter;
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void LCD_Write_String(char *a){
    int n;
    for(n = 0; a[n] != '\0'; n++){
        LCD_Write_Character(a[n]);
    }
}

void LCD_Set_Cursor(uint8_t linea, uint8_t columna){
    uint8_t corrimiento = 0;
    switch (linea){
        case 1:
            corrimiento = 0x80 + columna;
            LCD_Cmd(corrimiento);
            break;
        case 2:
            corrimiento = 0x80 + 0x40 + columna;
            LCD_Cmd(corrimiento);
            break;
        default:
            LCD_Cmd(0x80);
    }
}

void LCD_Shift_links(){
    LCD_Cmd(0x18);
}

void LCD_Shift_rechts(){
    LCD_Cmd(0x1C);
}

void LCD_Cursor_rechts(uint8_t espacios){
    for (uint8_t n = 0; n <= espacios; n++){
        LCD_Cmd(0x14);
    }
}

void LCD_Cursor_links(uint8_t espacios){
    for (uint8_t n = 0; n <= espacios; n++){
        LCD_Cmd(0x10);
    }
}

void LCD_Create_Char(uint8_t charnum, const uint8_t * chardata){
	uint8_t n;
	charnum &= 0x07;
	LCD_Cmd(0x40 | (charnum << 3));
	for (n = 0; n < 8; n++)
		LCD_Write_Character(chardata[n]);
}

//char uint_to_char(uint8_t numero){
//    char numChr = 214;
//    switch (numero){
//        case 0:
//            numChr = 48;
//            break;
//        case 1:
//            numChr = 49;
//            break;
//        case 2:
//            numChr = 50;
//            break;
//        case 3:
//            numChr = 51;
//            break;
//        case 4:
//            numChr = 52;
//            break;
//        case 5:
//            numChr = 53;
//            break;
//        case 6:
//            numChr = 54;
//            break;
//        case 7:
//            numChr = 55;
//            break;
//        case 8:
//            numChr = 56;
//            break;
//        case 9:
//            numChr = 57;
//            break;
//        case 10:
//            numChr = 65;
//            break;
//        case 11:
//            numChr = 66;
//            break;
//        case 12:
//            numChr = 67;
//            break;
//        case 13:
//            numChr = 68;
//            break;
//        case 14:
//            numChr = 69;
//            break;
//        case 15:
//            numChr = 70;
//            break;
//        default:
//            numChr = 214;
//    }
//    return(numChr);
//}

uint16_t * uint_to_array(uint8_t numero){
    uint16_t resultado[3] = {0,0,0};
    resultado[0] = numero/100;
    uint8_t centenas = resultado[0];
    resultado[1] = (numero - (centenas *100))/10;
    uint8_t decenas = resultado[1];
    resultado[2] = numero -(centenas*100+decenas*10);
    return(resultado);
}