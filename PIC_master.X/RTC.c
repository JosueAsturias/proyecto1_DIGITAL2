/*
 * File:   RTC.c
 * Author: josue Asturias
 *
 * Created on February 22, 2020, 6:39 PM
 */
//usar junto con I2C.h

#include <xc.h>
#include "RTC.h"
#ifndef __I2C_H
#include "I2C.h"
#endif

#define __XTAL_FREQ 4000000

//copiar y ajustar a lo deseado en main.c
int seg = 0;
int min = 0;
int hora = 0;
int dia = 1;
int datum = 23;
int mes = 2;
int jahr = 20;


int BCD_a_DEC(int numBCD){
    return (numBCD >> 4) * 10 + (numBCD & 0x0F);
    
}

int DEC_a_BCD(int numDEC){
    return ((numDEC / 10) << 4) + (numDEC % 10);
}

void Zeit_Datum_Set(void){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0);
    I2C_Master_Write(DEC_a_BCD(seg));
    I2C_Master_Write(DEC_a_BCD(min));
    I2C_Master_Write(DEC_a_BCD(hora));
    I2C_Master_Write(DEC_a_BCD(dia)); 
    I2C_Master_Write(DEC_a_BCD(datum));
    I2C_Master_Write(DEC_a_BCD(mes));
    I2C_Master_Write(DEC_a_BCD(jahr));
    I2C_Master_Stop();
}

void get_Time(void){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0); //ajusta adress RTC a 0x00
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    seg = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(1); //ajusta adress RTC a 0x01
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    min = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(2); //ajusta adress RTC a 0x02
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    hora = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(3); //ajusta adress RTC a 0x03
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    dia = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(4); //ajusta adress RTC a 0x04
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    datum = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(5); //ajusta adress RTC a 0x05
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    mes = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(6); //ajusta adress RTC a 0x06
    I2C_Master_Stop();
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    jahr = BCD_a_DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
    
//    I2C_Master_Start();
//    I2C_Master_Write(0xD1);
//    I2C_Master_Read(0);
//    I2C_Master_Stop();
}

uint8_t get_Temp(){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0x11);
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    uint8_t TempMSB = I2C_Master_Read(0);
    uint8_t Temp_LSB = I2C_Master_Read(0);
    I2C_Master_Stop();
    
    return TempMSB;
    
}
