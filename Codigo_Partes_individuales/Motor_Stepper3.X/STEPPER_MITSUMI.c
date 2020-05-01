/*
 * File:   STEPPER_MITSUMI.c
 * Author: Josué Castillo Lou
 *
 * Created on 11 de marzo de 2020, 12:15 PM
 */

#include <xc.h>
#include <stdint.h>
#include "STEPPER_MITSUMI.h"
#define _XTAL_FREQ 4000000      // 4MHz
int i = 0 ; 

void Stepper_CW(uint16_t Step_grados_cw){            //sentido horario (CW --> Counter Wise)
    switch(Step_grados_cw){
        case 360:
            for(i=0; i<25; i++){             // 360° horario 
                    PORTD = 0b1010; //1
                    __delay_ms(10);
                    PORTD = 0b0110; //2
                    __delay_ms(10);
                    PORTD = 0b0101; //3
                    __delay_ms(10);
                    PORTD = 0b1001; //4
                    __delay_ms(10);
                    
                };
            break;
            
        case 180:
            for(i=0; i<12; i++){             // 180° horario 
                    PORTD = 0b1010; //1
                    __delay_ms(10);
                    PORTD = 0b0110; //2
                    __delay_ms(10);
                    PORTD = 0b0101; //3
                    __delay_ms(10);
                    PORTD = 0b1001; //4
                    __delay_ms(10);
                    
                };
            break;
            
        case 90:
            for(i=0; i<6; i++){				// 90° horario 
                    PORTD = 0b1010; //1
                    __delay_ms(10);
                    PORTD = 0b0110; //2
                    __delay_ms(10);
                    PORTD = 0b0101; //3
                    __delay_ms(10);
                    PORTD = 0b1001; //4
                    __delay_ms(10);
    
                };
            break;
            
        case 45:
            for(i=0; i<3; i++){             // 45° horario 
                    PORTD = 0b1010; //1
                    __delay_ms(10);
                    PORTD = 0b0110; //2
                    __delay_ms(10);
                    PORTD = 0b0101; //3
                    __delay_ms(10);
                    PORTD = 0b1001; //4
                    __delay_ms(10);
                    
                };
            break;
            
        case 15:
            for(i=0; i<1; i++){             // 15° horario 
                    PORTD = 0b1010; //1
                    __delay_ms(10);
                    PORTD = 0b0110; //2
                    __delay_ms(10);
                    PORTD = 0b0101; //3
                    __delay_ms(10);
                    PORTD = 0b1001; //4
                    __delay_ms(10);
                    
                };
            break;
            
        default:
            for(i=0; i<2; i++){             //
                    PORTD = 0b1010; //1
                    __delay_ms(10);
                    PORTD = 0b0110; //2
                    __delay_ms(10);
                    PORTD = 0b0101; //3
                    __delay_ms(10);
                    PORTD = 0b1001; //4
                    __delay_ms(10);
                    
                };
            
    }
}

void Stepper_CCW(uint16_t Step_grados_ccw){            //sentido anti-horario (CCW --> Counter Clock Wise)
    switch(Step_grados_ccw){
        case 360:
            for(i=0; i<25; i++){             //360° anti-horario
                    PORTD = 0b1001; //1
                    __delay_ms(10);
                    PORTD = 0b0101; //2
                    __delay_ms(10);
                    PORTD = 0b0110; //3
                    __delay_ms(10);
                    PORTD = 0b1010; //4
                    __delay_ms(10);  
                };
            break;
            
        case 180:
            for(i=0; i<12; i++){             //180° anti-horario
                    PORTD = 0b1001; //1
                    __delay_ms(10);
                    PORTD = 0b0101; //2
                    __delay_ms(10);
                    PORTD = 0b0110; //3
                    __delay_ms(10);
                    PORTD = 0b1010; //4
                    __delay_ms(10);  
                };
            break;
            
        case 90:
            for(i=0; i<6; i++){				//90° anti-horario
                    PORTD = 0b1001; //1
                    __delay_ms(10);
                    PORTD = 0b0101; //2
                    __delay_ms(10);
                    PORTD = 0b0110; //3
                    __delay_ms(10);
                    PORTD = 0b1010; //4
                    __delay_ms(10);  
                };
            break;
            
        case 45:
            for(i=0; i<3; i++){             //45° anti-horario
                    PORTD = 0b1001; //1
                    __delay_ms(10);
                    PORTD = 0b0101; //2
                    __delay_ms(10);
                    PORTD = 0b0110; //3
                    __delay_ms(10);
                    PORTD = 0b1010; //4
                    __delay_ms(10);  
                };
            break;
            
        case 15:
            for(i=0; i<1; i++){             //15° anti-horario
                    PORTD = 0b1001; //1
                    __delay_ms(10);
                    PORTD = 0b0101; //2
                    __delay_ms(10);
                    PORTD = 0b0110; //3
                    __delay_ms(10);
                    PORTD = 0b1010; //4
                    __delay_ms(10);  
                };
            break;
            
        default:
            for(i=0; i<2; i++){             
                    PORTD = 0b1001; //1
                    __delay_ms(10);
                    PORTD = 0b0101; //2
                    __delay_ms(10);
                    PORTD = 0b0110; //3
                    __delay_ms(10);
                    PORTD = 0b1010; //4
                    __delay_ms(10);  
                };
            
    }
}

