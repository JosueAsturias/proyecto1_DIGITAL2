/*
 * File:   Temperatura_I2C.c
 * Author: Miguel García
 *
 * Created on March 9, 2020, 11:36 AM
 */


#include <xc.h>
#include <stdint.h>
#include "I2C.h"
#include "Temperatura_I2C.h"



uint16_t temp_objeto(void){
    uint16_t temp_MSB;
    uint8_t temp_LSB;
    uint8_t PEC;
    uint16_t temp;
    I2C_Master_Start();
    I2C_Master_Write(0x00);
    I2C_Master_Write(0x07);
    //I2C_Master_Stop();

    I2C_Master_Start();
    I2C_Master_Write(0x01);
    temp_LSB = I2C_Master_Read(0);
    temp_MSB = I2C_Master_Read(0);
    PEC = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(50);

    temp_MSB = temp_MSB <<8;
    temp = (((temp_MSB+temp_LSB)*0.2)-273.15)/100;
    return(temp);
    }

uint16_t temp_ambiente(void){
    uint16_t temp_MSB;
    uint8_t temp_LSB;
    uint8_t PEC;
    uint16_t temp;
    I2C_Master_Start();
    I2C_Master_Write(0x00);
    I2C_Master_Write(0x06);
    //I2C_Master_Stop();

    I2C_Master_Start();
    I2C_Master_Write(0x01);
    temp_LSB = I2C_Master_Read(0);
    temp_MSB = I2C_Master_Read(0);
    PEC = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(50);

    temp_MSB = temp_MSB <<8;
    temp = (((temp_MSB+temp_LSB)*0.2)-273.15)/100;
    return(temp);
}
	//temperatura ambiente dirección 0x06
	//temperatura objeto dirección 0x07