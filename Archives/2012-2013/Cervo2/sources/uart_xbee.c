/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/


#include "main.h"
#include "comXbee.h"


/*------------------------------------ variables and define -----------------*/

#ifdef INT_UART_TX
#define TX_BUFFER_SIZE 64
unsigned char tx2Buffer[TX_BUFFER_SIZE];
volatile short indexTx2Buffer = 0;
volatile short tx2BufferLength = 0;
#endif //#ifdef INT_UART_TX

/*------------------------------------ low level functions ------------------*/

int UART_xbee_putc(unsigned char c)
{
#ifdef INT_UART_TX
	if(U2STAbits.UTXBF) {		// Si le buffer du module est plein
		tx2Buffer[(indexTx2Buffer + tx2BufferLength) % TX_BUFFER_SIZE] = c;
		tx2BufferLength ++;
IFS1bits.U2TXIF = 0;
IEC1bits.U2TXIE = 1;	// Activation de l'interruption sur l'envoie
		return -1;
	} else U2TXREG = c;

#else //#ifdef INT_UART_TX

	while (U1STAbits.UTXBF);
	U1TXREG = c;
#endif //#ifdef INT_UART_TX

	/*
	static short cnt;
	while (U1STAbits.UTXBF) cnt++;	// Si le buffer du module est plein
	U1TXREG = c;//*/
IEC1bits.U2TXIE = 0;	// Desactivation de l'interruption sur l'envoie
	return 0;
}

int send_xbee_long(long message) {
	if (UART_xbee_putc(message >> 24)) return 0;
	if (UART_xbee_putc(message >> 16)) return 1;
	if (UART_xbee_putc(message >> 8)) return 2;
	if (UART_xbee_putc(message)) return 3;
	return 4;
}

int send_xbee_short(short message) {
	if (UART_xbee_putc(message >> 8)) return 0;
	if (UART_xbee_putc(message)) return 1;
	return 2;
}


void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
	IFS1bits.U2RXIF = 0; // On s'acquitte de l'interruption

	if(U2STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U2STAbits.OERR == 1)
	{
		U2STAbits.OERR = 0;
		return ;
	}

	/* get the data */
	if(U2STAbits.URXDA == 1) message_processing_xbee(U2RXREG);

}

/*void __attribute__((interrupt, auto_psv)) _U1ErrInterrupt(void)
{
	IFS4bits.U1EIF = 0; // On s'acquitte de l'interruption
	if(U1STAbits.FERR == 1)	// Erreurs ?
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

	if(U2STAbits.FERR == 1)	// Erreurs ?
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
