/*
 * File:   MAIN_MOV_1.c
 * Author: MIGUEL GARCIA
 *
 * Created on 3 de mayo de 2020, 15:46
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#define _XTAL_FREQ 1000000


#define ADELANTE 0b10101010
#define IZQUIERDA 0b10011001 
#define DERECHA 0b01100110

//variables a recibir
uint8_t velocidad = 0;
uint8_t largo = 0;
uint8_t ancho = 0;
uint8_t recibir;
uint8_t recibir_ind = 0;

uint8_t indicador = 0;
uint8_t indicador_mov = 1;
uint8_t contador = 0;
uint8_t cont_obs = 0;

uint8_t muestras = 0;
uint8_t contador_m = 1;

// Variables a mandar
uint8_t atras;
uint8_t adelante;
uint8_t motor;
short z;

void config(void);
void OSC_config(uint32_t frecuencia);
void init_PWM_2 (void);
void OSC_config(uint32_t frecuencia);
void Servo2_grados(uint8_t Servo2_grados);
void select_velocidad(uint8_t vel);
void cambiar_fila_derecha(void);
void cambiar_fila_izquierda(void);
void init_ADC(uint8_t channel);
void dis_atr(void);
void dis_ade(void);
void I2C_Slave_Init(short address);
void obstaculo (void);
void esperar(uint8_t vel);
void regresar (void);

void __interrupt() ISR(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            recibir = SSPBUF;             // Guardar en recibir1 el valor del buffer de recepción
            if (recibir_ind == 0){
                velocidad = recibir;
            }
            else if(recibir_ind == 1){
                largo = recibir;
            }
            else if(recibir_ind == 2){
                ancho = recibir;
                if(velocidad>0 && velocidad <5){
                    indicador_mov = 1; 
                }
            }
            
            recibir_ind++;
            if (recibir_ind == 3){
                recibir_ind = 0;
            }
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if (contador == 0){
                SSPBUF = adelante;
            }
            else if (contador == 1){
                SSPBUF = atras;
            }
            else if (contador == 2){
                SSPBUF = motor;
            }
            contador++;
            if (contador == 3){
                contador = 0;
            }
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    return;
}

void main(void) {
    config();
    I2C_Slave_Init(0x30);
    select_velocidad(2);
    while(1){
        PORTB = 0xFF;
        dis_ade();
        __delay_ms(300);
        PORTB = 0x00;
        dis_atr();
        __delay_ms(300);
        
        if (indicador_mov==1){
            select_velocidad(velocidad);
            muestras = largo*ancho;
            while(muestras > 1){
                PORTD = 0;
                motor = 0xF0;
                __delay_ms(5000);
                PORTD = ADELANTE;
                motor = 0x00;
                if (muestras > 1){
                    if(contador_m == largo){
                        if (muestras !=1){
                            if(indicador==0){
                                cambiar_fila_derecha();
                                contador_m = 1;
                            }
                            else if(indicador == 1){
                                cambiar_fila_izquierda();
                                contador_m = 1;
                            }
                        }
                    }
                    else {    
                        for(int i = 0;i<6;i++){
                            dis_ade();
                            esperar(velocidad);
                            dis_atr();
                            esperar(velocidad);
                        }
                        while(adelante <20){
                            obstaculo();
                            dis_ade();
                            __delay_ms(300);
                            dis_atr();
                        }
                        if(cont_obs>0){
                            regresar();
                        }
                        contador_m++;
                        PORTD = 0;
                    }

                }
                muestras--;
            }
            contador_m = 1;
            indicador_mov = 0;
            indicador = 0;
        }
        
    }

    return;
}

void regresar (void){
    PORTD = ADELANTE;
    for(int i = 0;i<6;i++){
        dis_ade();
        esperar(velocidad);
        dis_atr();
       esperar(velocidad);
    }
    PORTD = 0;
    __delay_ms(2000);
    PORTD = IZQUIERDA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(500);
    PORTD = ADELANTE;
   while(cont_obs>0){
        
        __delay_ms(2000);
        PORTA--;
        cont_obs--;
    }
    PORTD = 0;
    __delay_ms(1000);
    PORTD = DERECHA;
    __delay_ms(1000);
    PORTD=0;
    muestras--;
    cont_obs = 0;
}

void obstaculo (void){
    PORTD = 0;
    __delay_ms(2000);
    PORTD = DERECHA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    PORTD = ADELANTE;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    PORTD = IZQUIERDA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    cont_obs++;
    PORTA++;
}

void cambiar_fila_derecha(void){
    PORTD = 0;
    __delay_ms(2000);
    PORTD = DERECHA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    PORTD = ADELANTE;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    PORTD = DERECHA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    indicador = 1;
}

void cambiar_fila_izquierda(void){
    PORTD = 0;
    __delay_ms(2000);
    PORTD = IZQUIERDA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    PORTD = ADELANTE;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    PORTD = IZQUIERDA;
    __delay_ms(2000);
    PORTD = 0;
    __delay_ms(2000);
    indicador = 0;
}


void config(void){
    OSC_config(_XTAL_FREQ);
    PORTD = 0;
    TRISD = 0;
    PORTB = 0;
    TRISB = 0;
    ANSELH = 0;
    PORTE = 0b1111;
    TRISE = 0b1111;
    PORTA = 0;
    TRISA = 0;
    ANSEL = 0b01100000;
    init_PWM_2();
    init_ADC(5);
    
    
}


void dis_ade(void){
    init_ADC(0x05);
    PIR1bits.ADIF = 0;
    adelante = ADRESH;
    __delay_ms(10);
}

void dis_atr(void){
    init_ADC(0x06);
    PIR1bits.ADIF = 0;
    atras = ADRESH;
    __delay_ms(10);
}
void init_ADC(uint8_t channel){
    ADCON0bits.ADCS1 = 0; 
    ADCON0bits.ADCS0 = 1;           //    FOSC/8
    
    
    switch(channel){
        case 0:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;    ///AN0
            break;
        case 1:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;    ///AN1
            break;
        case 2:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;    ///AN2
            break;
        case 3:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;    ///AN3
            break;
        case 4:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;    ///AN4
            break;
        case 5:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;    ///AN5
            break;
        case 6:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;    ///AN6
            break;
        case 7:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;    ///AN7
            break;        
        case 8:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;    ///AN8
            break;
        case 9:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;    ///AN9
            break;
        case 10:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;    ///AN10
            break;
        case 11:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;    ///AN11
            break;            
        case 12:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;    ///AN12
            break;
        case 13:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;    ///AN13
            break;
        case 14:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;    ///CVREF
            break;
        case 15:
            ADCON0bits.CHS3 = 1;
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;    ///FIXED REF
            break;    
        default:
            ADCON0bits.CHS3 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;    ///AN0
            break;
    }

    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON0bits.ADON = 1;
//    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    //INTCONbits.GIE = 0;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1){        
    }   
    //INTCONbits.GIE = 1;
}


void select_velocidad(uint8_t vel){
    switch(vel){
        case 1:    
            Servo2_grados(30);//velocidad lenta
            break;
             
        case 2:
            Servo2_grados(90);//velocidad media
            break;    
        case 3:
            Servo2_grados(180);//velocidad rapida
            break;
    }
}

void esperar(uint8_t vel){
    switch(vel){
        case 1:    
            __delay_ms(1000);
            break;
             
        case 2:
            __delay_ms(500);
            break;    
        case 3:
            __delay_ms(333);
            break;
    }
}

void init_PWM_2(void){           // Se configura el CCP1 --> RC2 !!! (ESTA CONFIG ES DISTINTA AL OTRO CCP)
    TRISCbits.TRISC2 = 1;        // (PASO#1) ESTABLECER RC2-CCP1 COMO ENTRADA
    
    //PR2 = 155;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    PR2 = 200;                   // (PASO#2) COLOCAR EL VALOR DEL PERIODO DE MI SEÑAL 20mS (se le carga al tmr2)   
    
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;      // (PASO#3) MODO PWM: P1A, P1C active-high; P1B, P1D active-high	

    CCP1CONbits.P1M1   = 0;
    CCP1CONbits.P1M0   = 0;      // Modo PWM "Single Output" !!!! *******************************************
    
                              
    CCPR2L = 0b00011011;         // MSB del duty cicle (27 decimal)  (PASO#4 Estableciendo ciclo de trabajo)
    CCP1CONbits.DC1B0 = 1;       // LSB del duty cicle
    CCP1CONbits.DC1B1 = 1;       //(10 bits de resolución CCPR2L + DC2B0 Y DC2B1)

    PIR1bits.TMR2IF = 0;         //(PASO#5.1) Borrando bandera del TMR2 / Config. e Inicializar TMR2
    T2CONbits.T2CKPS1 = 1;
    T2CONbits.T2CKPS0 = 1;       //(PASO#5.2) PRESCALER 1:16 para el TMR2 		 
    
    T2CONbits.TMR2ON = 1;        //(PASO#5.3) HABILITAMOS EL TMR2
    
    while(PIR1bits.TMR2IF == 1){ //Se espera a que el TMR2 haga un overflow para resetear la bandera del mismo
        PIR1bits.TMR2IF = 0;     //(PASO#6.1) se resetea la bandera del TMR2
    }
     
    TRISCbits.TRISC2 = 0;        // (PASO#6.2) RC1-CCP2 SALIDA PWM --> (Habilitamos la salida del PWM espués de que haya un nuevo ciclo empezado)
}


void OSC_config(uint32_t frecuencia){
    switch(frecuencia){
        case 8000000:
            OSCCONbits.IRCF = 0b111;
            break;
        case 4000000:
            OSCCONbits.IRCF = 0b110;
            break;
        case 2000000:
            OSCCONbits.IRCF = 0b101;
            break;
        case 1000000:
            OSCCONbits.IRCF = 0b100;
            break;
        case 500000:
            OSCCONbits.IRCF = 0b011;
            break;
        case 250000:
            OSCCONbits.IRCF = 0b010;
            break;
        case 125000:
            OSCCONbits.IRCF = 0b001;
            break;
        case 32000:
            OSCCONbits.IRCF = 0b000;
            break;
        default:
            OSCCONbits.IRCF = 0b110;
    }
}


void Servo2_grados(uint8_t Servo2_grados){
    switch(Servo2_grados){
        case 0:                             /////////////////////////////////
            CCPR1L = 10; //calculado para 0°
            break;
             
        case 10:
            CCPR1L = 12;
            break;
             
        case 30:
            CCPR1L = 14;
            break;
             
        case 45:
            CCPR1L = 18; //calculado para 45°
            break;
            
        case 90:
            CCPR1L = 25; //calculado para 90° //////////////////////////////
            break;
            
        case 120:
            CCPR1L = 30; //calculado para 
            break;
            
        case 135:
            CCPR1L = 33; //calculado para °
            break;
            
        case 150:
            CCPR1L = 37; //calculado para °
            break;
            
        case 180:                           ///////////////////////////////
            CCPR1L = 40; //calculado para 180°
            break;
            
        default:
            CCPR1L = 10; //calculado para 0°
            
    }
}

void I2C_Slave_Init(short address) 
{
  SSPSTAT = 0x80;    
  SSPADD = address; //Setting address
  SSPCON = 0x36;    //As a slave device
  SSPCON2 = 0x01;
  TRISC3 = 1;       //Setting as input as given in datasheet
  TRISC4 = 1;       //Setting as input as given in datasheet
  GIE = 1;          //Global interrupt enable
  PEIE = 1;         //Peripheral interrupt enable
  SSPIF = 0;        //Clear interrupt flag
  SSPIE = 1;        //Synchronous serial port interrupt enable
}