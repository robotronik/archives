/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/


#include "main.h"
#include "message.h"

/*------------------------------------ variables and define -----------------*/

/*#define TX_BUFFER_SIZE_MAX 64
unsigned char txBuffer[TX_BUFFER_SIZE_MAX];
volatile short indexTxBuffer = 0;
volatile short txBufferSize = 0;
*/



/*------------------------------------ low level functions ------------------*/

int UART_putc(unsigned char c)
{
	/*
	if(U1STAbits.UTXBF) {		// Si le buffer du module est plein
		txBuffer[(indexTxBuffer + txBufferSize) % TX_BUFFER_SIZE_MAX] = c;
		txBufferSize ++;
	} else U1TXREG = c;
	
	if (U1STAbits.UTXBF) return -1;
	//*/
	
	//*
	while (U1STAbits.UTXBF);
	U1TXREG = c;//*/
	
	/*
	static short cnt;
	while (U1STAbits.UTXBF) cnt++;	// Si le buffer du module est plein
	U1TXREG = c;//*/
	return 0;
}

int send_long(long message) {
	if (UART_putc(message >> 24)) return 0;
	if (UART_putc(message >> 16)) return 1;
	if (UART_putc(message >> 8)) return 2;
	if (UART_putc(message)) return 3;
	return 4;
}


void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
	IFS0bits.U1RXIF = 0; // On s'acquitte de l'interruption

	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U1STAbits.OERR == 1)
	{
		U1STAbits.OERR = 0;
		return ;
	}

	/* get the data */
	if(U1STAbits.URXDA == 1) message_processing(U1RXREG);
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

// Buffer du module vidé
/*void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void)
{
	IFS0bits.U1TXIF = 0; // On s'acquitte de l'interruption
	
	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	// must clear the overrun error to keep uart receiving
	if(U1STAbits.OERR == 1)
	{
		U1STAbits.OERR = 0;
		return ;
	}
	
	*//*if (txBufferSize) {
		U1TXREG = txBufferSize + 128;
		txBufferSize --;
		pause_s(1);
	}*//*
	short i=0;
	while (txBufferSize && i<4) {
		U1TXREG = txBuffer[indexTxBuffer];
		indexTxBuffer ++; indexTxBuffer %= TX_BUFFER_SIZE_MAX;
		i ++;
		txBufferSize --;
	}
}*/
