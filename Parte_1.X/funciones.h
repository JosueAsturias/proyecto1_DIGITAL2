  
/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*
 * File:   funciones.h
 * Author: MIGUEL GARCÍA
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _FUNCIONES_H
#define	_FUNCIONES_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
// TODO Insert appropriate #include <>


//void init_PWM_1 (void);
//void servoRotate0(void);
//void servoRotate90(void);
//void servoRotate180(void);
uint16_t Distancia();
uint16_t Distancia_1();
void oscillator(uint8_t a);

//void Lcd_Init(void);
//void Lcd_Cmd(int a);
//void Lcd_Clear(void);
//void Lcd_Set_Cursor(int a, int b);
//void Lcd_Write_String(char *a);
//void Lcd_Write_Char(char a);

#endif	/* _FUNCIONES_H */