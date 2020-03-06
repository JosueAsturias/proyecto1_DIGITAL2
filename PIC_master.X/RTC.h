/* 
 * File:   RTC.h
 * Author: Josue Asturias
 * Comments:
 * Revision history: 
 */
// usar junto con I2C.h 

#ifndef __RTC_H
#define	__RTC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

int BCD_a_DEC(int numBCD);
/****** convierte de BCD a decimal *****
 ****** -------------------------- *****/

int DEC_a_BCD(int numDEC);
/****** convierte de Decimal a BCD *****
 ****** -------------------------- *****/

void Zeit_Datum_Set(void);
/****** ajustar hora y fecha   ****
 ****** --------------------   *****/

void get_Time(void);
/****** obtiene hora y fecha   ****
 ****** --------------------   *****/

uint8_t get_Temp();
/****** obtiene temperatura    ****
 ****** ----valor entero----   *****/

#endif	/* __RTC_H */