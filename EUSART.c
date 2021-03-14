/*
 * File:   EUSART.c
 * Author: diegomazariegos
 *
 * Created on March 12, 2021, 1:11 PM
 */


#include <xc.h>
#define _XTAL_FREQ 4000000

void EUSART (void) {
   
    TXSTAbits.SYNC    = 0;     // Transmision asincrona
    TXSTAbits.TXEN    = 1;     // Habilitamos la transmision
    PIE1bits.TXIE     = 1;     // Habilitamos la interrupcion de transmision de datos
    RCSTAbits.SPEN    = 1;     // Habilitamos el puerto serial
    PIE1bits.RCIE     = 1;     // Habilitamos la interrupcion de recepcion de datos
    PIR1bits.RCIF     = 0;     // Habilitacion de la bandera para la interrupcion de datos, esta en cero porque esta se va a habilitar hasta que pase la interrupcion
    
    TXSTAbits.TX9     = 0;     // Desabilitamos la opcion de trabajar con 9 bits en la transmision
    RCSTAbits.RX9     = 0;     // Desabilitamos la opcion de trabajar con 9 bits en la recepcion
    BAUDCTLbits.BRG16 = 0;     // Estamos utilizando un Baud Rate de 8 bits
    
    SPBRG             = ((_XTAL_FREQ / 16) / 9600) - 1;
    TXSTAbits.BRGH    = 1;     // Estamos trabajando con un velocidad alta
    SPBRG             = 0;     // Estamos trabajando con un BAUD RATE DE 9600
    RCSTAbits.CREN    = 1;     // Habilitamos la recepcion continua
    
}

void S_DATA( char x ) {
    
    while ( !TXIF );
    TXREG = x;
    
}

void SS (char* x) {
    
    while( *x );
    S_DATA(*x++);
}
