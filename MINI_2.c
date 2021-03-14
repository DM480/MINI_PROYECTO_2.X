/*
 * File:   Mini_Proyecto_2.c
 * Author: Diego Sebasrtian Mazariegos Guzman
 * Carne:  18808
 * Inst:   UVG
 * Curso:  Digital 2
 *
 * Created on March 10, 2021, 12:13 AM
 */

#include <pic16f887.h>
#include <xc.h>
#include "config.h"
#include "I2C.h"
//#include "PRESS.h"
#include "BMP.h"
#include "EUSART.h"
#define  _XTAL_FREQ 4000000

//******************************************************************************
//                              VARIABLES
//******************************************************************************

/*char ADD_SENSOR_W = 0x68;    // Este contiene la direccion del mi acelerometro
char TEMP       = 0;
char DATO_LED   = 0;*/
 signed long temperature;
 uint32_t var;
 char dec;
 char uni;
 char p_dec1;
 char p_dec2;
 char RX_RECEP;
 
//******************************************************************************
//                     PROTOTIPOS DE FUNCIONES
//******************************************************************************

void setup (void);
void __interrupt() RECEP (void);
void TRANS (void);

//******************************************************************************
//                           CONFIGURACION
//******************************************************************************

void setup (void) {
    
    ANSEL  = 0;     // Puerto del AN0 al AN7 los declare como digitales
    ANSELH = 0;     // Puerto del AN8 al AN13 los declare como digitales
    
    TRISA = 0;
    TRISB = 0;
    PORTB = 0;
    
    TRISC = 0;
    //TRISCbits.TRISC3 = 0;  // Se configura este pin que tambien es SCL como una salida para la señal de reloj
    //TRISCbits.TRISC7 = 1;  // Se configura este pin como entrada, ya que es RX el encargado de la recepcion de datos en la comunicacion serial
    
    TRISD = 0;
    PORTD = 0;
    
    TRISE = 0;
    PORTE = 0;
    
    I2C_Master_Init(100000);
    
    void EUSART (void);
    
    /*TXSTAbits.BRGH    = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRG             = 25;
    
    // TRANSMISION
    
    TXSTAbits.TXEN = 1;    // Habilitamos la transmision de datos
    TXSTAbits.SYNC = 0;    // Seleccionamos que la comunicacion sea asincrona
    RCSTAbits.SPEN = 1;    // Habilitamos los puertos para la comunicacion serial
    TXSTAbits.TX9  = 0;    // Se transmiten 9 bits
    
    
    // RECEPCION
    
    RCSTAbits.CREN = 1;    // Habilitamos la transmision de datos
    RCSTAbits.RX9  = 0;    // Hay una recepcion de 9 bits
    
    // INTERRUPCIONES
    
    PIE1bits.RCIE = 1;     // Habilitamos las interrupciones de EUSART
    PIE1bits.TXIE = 0;     // Habilitamos la interrupcion para la transmision de datos
    PIR1bits.RCIF = 0;     // Apagamos a la bandera de la interrupicion de recepcion de EUSART
    
    INTCONbits.GIE = 1;    // Habilitamos la inteerupcion global
    
    return;*/
}

//******************************************************************************
//                     DEFINICION DE INTERRUPCION
//******************************************************************************

void __interrupt() RECEP (void) {
    if (PIR1bits.RCIF == 1) {
        RX_RECEP = S_DATA(); //Aqui recibimos el dato de la recepcion
        PIR1bits.RCIF = 0;
    }
}

//******************************************************************************
//                 DEFINICION DE TRANSFROMACION DE TEMPERATURA
//******************************************************************************
    
    void TRANS (void){
        
        var = temperature;
        dec = var / 1000;
        var = var - (dec * 1000);
        uni = var / 100;
        var = var - (uni * 100);
        p_dec1 = var / 10;
        var = var - (p_dec1 * 10);
        p_dec2 = var;
        
    }

/*void __interrupt() Inter (void){
    
    if (PIR1bits.RCIF == 1){
        
        PIR1bits.RCIF = 0;     // El buffer todavia no esta lleno
        DATO_LED      = RCREG;
        
    }
    
    if (DATO_LED == 0x0A){
        
        PORTE = 0;
        
    } else if (DATO_LED == 0x0B) {
        
        PORTE = 1;
        
    } else if (DATO_LED == 0x0C){
        
        PORTE = 0;
        
    } else if (DATO_LED == 0x0D){
        
        PORTE = 2;
        
    }
    
    return;
    
}*/

//******************************************************************************
//                           CODIGO PRINCIPAL
//******************************************************************************

void main(void) {
    
    setup();
    
    while(1) {
        
    BMP280_begin(MODE_NORMAL, SAMPLING_X1, SAMPLING_X1, FILTER_OFF, STANDBY_0_5);
        
    BMP280_readTemperature(&temperature);
    
    TRANS();
    SS ("#");
    S_DATA(dec);
    S_DATA(uni);
    SS (".");
    S_DATA(p_dec1);
    S_DATA(p_dec2);
    
    if (RX_RECEP == 0B00000000){  // 0 en decimal
        
        PORTDbits.RD2 = 0;    // Apagamos la LED
        
    }
    else if (RX_RECEP == 0B00000001){  // 1 en decimal
        
        PORTDbits.RD2 = 1;    // Encendemos la LED
        
    }
    else if (RX_RECEP == 0B00000010){  // 2 en decimal
        
        PORTDbits.RD3 = 0;    // Apagamos la LED
        
    }
    else if (RX_RECEP == 0B00000011){  // 3 en decimal
        
        PORTDbits.RD3 = 1;    // // Encendemos la LED
        
    }
    
        /*TXREG = TEMP;
        I2C_Master_Start();        // Se inicia la comunicacion I2C
        I2C_Master_Write(0x76);    // Se coloca la direccion del sensor y el bit de escritura
        I2C_Master_Write(0xF9);    // Se coloca el registro que queremos leer
        I2C_Master_Write(0xF8);    // pero el registro en el cual queremos hacer lectura es de20 bits
        I2C_Master_Write(0xD7);    // los 20 bits se dividen en 3 registros
        I2C_Master_Stop();         // Paramos la comunicacion
        __delay_ms(200);
        
        I2C_Master_Start();        // Se inicia la comunicacion I2C
        I2C_Master_Write(0x77);    // Colocamos la direccion del sensor y el bit de lectura
        TEMP = I2C_Master_Read(0); // Guardamos en una variable la lectura de los 3 registros que forman 20 bits
        I2C_Master_Stop();         // Paramos la comunicacion
        __delay_ms(200);*/
        //PORTB = PRESION_BMP(0xF7);
        //PORTB = PRESION_BMP(0xF8);
}

}