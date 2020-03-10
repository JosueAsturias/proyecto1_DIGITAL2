/* 
 * File:   Temperatura.h
 * Author: Miguel
 * Comments:
 * Revision history: 
 */
// usar junto con I2C.h 

#ifndef __TEMPERATURA_H
#define	__TEMPERATURA_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

int16_t temp_objeto(void);
int16_t temp_ambiente(void);

#endif	/* __TEMPERATURA_H */