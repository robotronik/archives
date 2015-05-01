#include "uart.h"




#define TX_BUFFER_SIZE_MAX 32 //à l'origine 16, mais ça marchait mal (envoi dans le desordre)
unsigned char txBuffer[TX_BUFFER_SIZE_MAX];
int indexTxBuffer = 0;
int txBufferSize = 0;

void UART_init()
{
    #ifndef SIMU
	U1MODEbits.PDSEL=0b00 ;// registre selection data parité
       AD1PCFGLbits.PCFG5 = 1;	// Désactivation de l'entrée analogique !!!
	RPOR1bits.RP2R = 3;							// Tx1 -> RP2
	RPINR18bits.U1RXR = 3;						// RP3 -> Rx1

	//U1MODEbits.LPBACK = 1;	// Debug : Tx1 -> Rx1

	U1MODEbits.BRGH = 1;	// High speed
	//U1BRG = 7; 			// BAUD Rate Setting for 115200 -> 115156 sans PLL
	U1BRG = 86; 			// BAUD Rate Setting for 115200 -> 114943 avec PLL
	//U1BRG = 1; 			// BAUD Rate Setting for 115200 -> 115156 sans PLL

	U1STAbits.UTXISEL1 = 1;	// Interrupt on empty FIFO, last byte is being sent
	U1STAbits.UTXISEL0 = 0;	// Second bit

	IFS0bits.U1RXIF = 0; 	// On evite des interruptions à l'activation
	IEC0bits.U1RXIE = 1;	// Activation de l'interruption sur réceptions
	IFS0bits.U1TXIF = 0;
	IEC0bits.U1TXIE = 1;	// Activation de l'interruption sur l'envoie
	IFS4bits.U1EIF = 0;
	IEC4bits.U1EIE = 1;		// Activation de l'interruption sur erreurs

	U1MODEbits.UARTEN = 1; 	// Enable UART
	U1STAbits.UTXEN = 1; 	// Enable UART TX
	#endif
}

void sendCoor(Coor list, int longueur, int longPlusChemin)
{
	static unsigned char numero = 0 ;
	numero++;
    Coor p = list;
    Trame tr;
    Trame tr2;
	tr.l =0;
	tr2.l =0;

	if ( !longPlusChemin )//càd uniquement longueur
	{
		tr.first.longueur = longueur;
		tr.first.numero = numero;
		tr.first.nb = 0;
		tr.first.code = 0b1011;
		send_message_carto(tr.l);
		return;
	}

	tr.first.longueur = longueur;
	tr.first.numero = numero;
	tr.first.nb = nbCoor(list);
	tr.first.code = 0b1011;
	send_message_carto(tr.l);
	
	while ( p != NULL)
    {
	    tr2.l = 0;
		tr2.data.y = p->y;
		tr2.data.x = p->x;
		tr2.data.numero = numero;
		tr2.data.code = 0b010;
		send_message_carto(tr2.l);
        
        if (p->queue != NULL)
        {
	        tr2.l = 0;
			tr2.data.y = p->queue->y;
			tr2.data.x = p->queue->x;
			tr2.data.numero = numero;
			tr2.data.code = 0b010;
			send_message_carto(tr2.l);
			p = p->queue;
		}
		p = p->queue;
    }
   
	
}

void UART_putc_carto(unsigned char c)
{
    #ifndef SIMU
	if(U1STAbits.UTXBF) {		// Si le buffer est plein
		txBuffer[(indexTxBuffer + txBufferSize) % TX_BUFFER_SIZE_MAX] = c;
		txBufferSize ++;
	} else U1TXREG = c;
	#else
    sendMessage(moduleCarto ,c, 0);
	#endif
}

void send_message_carto(long int message)
{
	/*#ifdef DEBUG
	printf("Assert send : %lx\n", message);
	#endif*/
	UART_putc_carto((char) (message >> 24));
	UART_putc_carto((char) (message >> 16));
	UART_putc_carto((char) (message >> 8));
	UART_putc_carto((char) message);
}

#ifndef SIMU

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
	if(U1STAbits.URXDA == 1) message_processing_carto(U1RXREG);
	/*{
		rx_buffer[rx_buffer_indice] = U1RXREG;
		rx_buffer_indice = (rx_buffer_indice + 1)%4;
		if(rx_buffer_indice == 0)
		{
			RX_IT_handler();
		}
//		rx_buffer_indice = (rx_buffer_indice + 1) %3;
//		RX_IT_handler();
	}*/
}

void __attribute__((interrupt, auto_psv)) _U1ErrInterrupt(void)
{
	IFS4bits.U1EIF = 0; // On s'acquitte de l'interruption
	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U1STAbits.OERR == 1)
		U1STAbits.OERR = 0;
	//error();

}

void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void)
{
	IFS0bits.U1TXIF = 0; // On s'acquitte de l'interruption

	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U1STAbits.OERR == 1)
	{
		U1STAbits.OERR = 0;
		return ;
	}

	int i=0;
	while (txBufferSize && i<4) {
		U1TXREG = txBuffer[indexTxBuffer];
		indexTxBuffer ++; indexTxBuffer %= TX_BUFFER_SIZE_MAX;
		i ++;
		txBufferSize --;
	}
}

#endif

