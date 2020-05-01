/*
 * File:   STEPPER_MITSUMI.c
 * Author: Josué Castillo Lou
 *
 * Created on 11 de marzo de 2020, 12:15 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __STEPPER_MITSUMI_H
#define	__STEPPER_MITSUMI_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void Stepper_CW(uint16_t Step_grados_cw);
void Stepper_CCW(uint16_t Step_grados_ccw);

#endif	/* XC_HEADER_TEMPLATE_H */

