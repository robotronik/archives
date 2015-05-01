/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/


#include "main.h"
#include "comXbee.h"

#include "debug.h"


/*------------------------------------ variables and define -----------------*/

#define TX_BUFFER_SIZE 64

extern volatile unsigned char tx2Buffer[TX_BUFFER_SIZE];
extern volatile short indexTx2Buffer;
extern volatile short tx2BufferLength;

/*------------------------------------ low level functions ------------------*/

int UART_xbee_putc(unsigned char c)
{
#ifdef INT_UART_TX
    if(U2STAbits.UTXBF) {        // Si le buffer du module est plein
        tx2Buffer[(indexTx2Buffer + tx2BufferLength) % TX_BUFFER_SIZE] = c;
        tx2BufferLength ++;
IFS1bits.U2TXIF = 0;
IEC1bits.U2TXIE = 1;    // Activation de l'interruption sur l'envoie
        return -1;
    } else U2TXREG = c;

#else //#ifdef INT_UART_TX

    while (U1STAbits.UTXBF);
    U1TXREG = c;
#endif //#ifdef INT_UART_TX

    /*
    static short cnt;
    while (U1STAbits.UTXBF) cnt++;    // Si le buffer du module est plein
    U1TXREG = c;//*/
IEC1bits.U2TXIE = 0;    // Desactivation de l'interruption sur l'envoie
    return 0;
}
void send_xbee_string(char* str, int lenght)
{
    int i = 0 ;
   // UART_xbee_putc('') ;
   while (i < lenght)
    {
        UART_xbee_putc(str[i]) ;
        pause_ms(200) ;
        i++;
   }
}
int send_xbee_long(long message) {
    if (UART_xbee_putc(message >> 24)) return 0;
    pause_ms(300) ;
    if (UART_xbee_putc(message >> 16)) return 1;
    pause_ms(300) ;
    if (UART_xbee_putc(message >> 8)) return 2;
    pause_ms(300) ;
    if (UART_xbee_putc(message)) return 3;
    pause_ms(300) ;
    return 4;
}

int send_xbee_short(short message) {
    if (UART_xbee_putc(message >> 8)) return 0;
    if (UART_xbee_putc(message)) return 1;
    return 2;
}


void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{

    // affiche_debug(DEBUG_MSG | 0b11001010);

    IFS1bits.U2RXIF = 0; // On s'acquitte de l'interruption

    if(U2STAbits.FERR == 1)    // Erreurs ?
        return ;
    /* must clear the overrun error to keep uart receiving */
    if(U2STAbits.OERR == 1)
    {
        U2STAbits.OERR = 0;
        return ;
    }

    /* get the data */
    if(U2STAbits.URXDA == 1) message_processing_xbee(U2RXREG);

    // affiche_debug(DEBUG_MSG | 0b11010101);
}

/*void __attribute__((interrupt, auto_psv)) _U1ErrInterrupt(void)
{
    IFS4bits.U1EIF = 0; // On s'acquitte de l'interruption
    if(U1STAbits.FERR == 1)    // Erreurs ?
        return ;
    // must clear the overrun error to keep uart receiving
    if(U1STAbits.OERR == 1)
        U1STAbits.OERR = 0;
    //error();

}*/

#ifdef INT_UART_TX
void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0; // On s'acquitte de l'interruption

    if(U2STAbits.FERR == 1)    // Erreurs ?
        return ;
    // must clear the overrun error to keep uart receiving
    if(U2STAbits.OERR == 1)
    {
        U2STAbits.OERR = 0;
        return ;
    }

    /*if (txBufferSize) {
        U1TXREG = txBufferLength + 128;
        txBufferSize --;
        pause_s(1);
    }*/
    int i=0;
    while (tx2BufferLength && (i<4)) {
        U2TXREG = tx2Buffer[indexTx2Buffer];
        tx2BufferLength --;
        indexTx2Buffer ++; indexTx2Buffer %= TX_BUFFER_SIZE;
        i ++;
    }
}
#endif //#ifdef INT_UART_TX
