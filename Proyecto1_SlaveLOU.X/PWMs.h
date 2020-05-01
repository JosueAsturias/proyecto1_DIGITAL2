/*
 * File:   PWMs.c
 * Author: Josue
 *
 * Created on 15 de marzo de 2020, 09:12 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __PWMs_H
#define	__PWMs_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

void init_PWM_1 (void);     //Inicialización CCP2 -- > RC1
void init_PWM_2 (void);     //Inicialización CCP1 -- > RC2 
void Servo1_grados(uint8_t Servo1_grados);
void Servo2_grados(uint8_t Servo2_grados);


#endif	/* XC_HEADER_TEMPLATE_H */

