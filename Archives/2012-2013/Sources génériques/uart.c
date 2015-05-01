/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/

#include <p33FJ128MC802.h>
#include "uart.h"

/*------------------------------------ variables and define -----------------*/

unsigned char rxBuffer[RX_BUFFER_SIZE];
volatile unsigned short indexRxBuffer = 0;
volatile unsigned short rxBufferLength = 0;

#ifdef INT_UART_TX
unsigned char txBuffer[TX_BUFFER_SIZE];
volatile unsigned short indexTxBuffer = 0;
volatile unsigned short txBufferLength = 0;
#endif //#ifdef INT_UART_TX



/*------------------------------------ low level functions ------------------*/

void UART_putc(unsigned char c)
{
#ifdef INT_UART_TX
	if(txBufferLength || U1STAbits.UTXBF) {		// Si la file contient une donnée ou que le buffer du périphérique est plein
		while (txBufferLength >= TX_BUFFER_SIZE);
		IEC0bits.U1TXIE = 0;	// Désactivation de l'interruption pour modifier les variables globales
		txBuffer[(indexTxBuffer + txBufferLength) % TX_BUFFER_SIZE] = c;
		txBufferLength ++;
		IEC0bits.U1TXIE = 1;	// Activation de l'interruption
		return;
	} else U1TXREG = c;
	
#else //#ifdef INT_UART_TX
	
	while (U1STAbits.UTXBF);
	U1TXREG = c;
#endif //#ifdef INT_UART_TX
	
	return;
}

void UART_send_tab(unsigned char *addr, unsigned char size) {
	addr += size;
	for (;size > 0; size --) {
		addr --;
		UART_putc(*addr);
	}
}

int UART_getc(unsigned char *byte) {
	if (rxBufferLength) {
		*byte = rxBuffer[indexRxBuffer];
		IEC0bits.U1RXIE = 0;	// Désactivation de l'interruption pour modifier les variables globales
		rxBufferLength --;
		indexRxBuffer ++; indexRxBuffer %= RX_BUFFER_SIZE;
		IEC0bits.U1RXIE = 1;	// Activation de l'interruption
		return 1;
	}
	return 0;
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
	if(U1STAbits.URXDA == 1) {
		rxBuffer[(indexRxBuffer + rxBufferLength) % RX_BUFFER_SIZE] = U1RXREG;
		rxBufferLength ++;
	}	
	//if(U1STAbits.URXDA == 1) message_processing(U1RXREG);
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
void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void)
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
	
	while (txBufferLength && !U1STAbits.UTXBF) {		// Si le buffer du module n'est pas plein
		U1TXREG = txBuffer[indexTxBuffer];
		txBufferLength --;
		indexTxBuffer ++; indexTxBuffer %= TX_BUFFER_SIZE;
	}
}
#endif //#ifdef INT_UART_TX
