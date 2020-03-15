/* 
 * File:   IMU.h 
 * Author: Josue Asturias
 * Comments:
 * Revision history: 
 */
 
#ifndef _IMU_H
#define	_IMU_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "I2C.h"

void IMU_init(void);
int16_t Acc_X(void);

int16_t Acc_Y(void);

int16_t Acc_Z(void);

int16_t Gy_X(void);

int16_t Gy_Y(void);

int16_t Gy_Z(void);




#endif	/* _IMU_H */

