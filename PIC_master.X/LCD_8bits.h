/* 
 * File:   LCD_8bits.h
 * Author: Josue Asturias
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __LCD_8BITS_H
#define	__LCD_8BITS_H

#include <xc.h> // include processor files - each processor file is guarded.  


void LCD_Cmd(uint8_t comando);
void LCD_clear(void);
void LCD_home(void);
void LCD_init(void);
void LCD_Write_Character(char caracter);
void LCD_Write_String(char *a);
void LCD_Set_Cursor(uint8_t linea, uint8_t columna);
void LCD_Shift_links();
void LCD_Shift_rechts();
void LCD_Cursor_rechts(uint8_t espacios);
void LCD_Cursor_links(uint8_t espacios);
void LCD_Create_Char(uint8_t charnum, const uint8_t * chardata);
//char uint_to_char(uint8_t numero);
uint16_t * uint_to_array(uint8_t numero);

#endif	