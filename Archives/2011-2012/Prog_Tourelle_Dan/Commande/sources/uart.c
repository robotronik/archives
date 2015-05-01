/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/

#include "uart.h"
#include "init.h"
#define TX_BUFFER_SIZE_MAX 16
unsigned char txBuffer[TX_BUFFER_SIZE_MAX];
int indexTxBuffer = 0;
int txBufferSize = 0;
volatile unsigned char calibrage = 1; // Des l'allumage, on calibre
volatile unsigned char semit = 0;
volatile unsigned char toDoCalibrage = 0;

volatile unsigned char debugToSend = 0;

static void message_processing(unsigned char m);

void UART_init()
{
	AD1PCFGLbits.PCFG5 = 1;	// Désactivation de l'entrée analogique !!!
	RPOR1bits.RP2R = 3;							// Tx1 -> RP2
	RPINR18bits.U1RXR = 3;						// RP3 -> Rx1

	U1MODEbits.LPBACK = 0;	// Debug : Tx1 -> Rx1

	U1MODEbits.BRGH = 0;	// High speed
	//U1BRG = 7; 			// BAUD Rate Setting for 115200 -> 115156 sans PLL
	//U1BRG = 86; 			// BAUD Rate Setting for 115200 -> 114943 avec PLL
	U1BRG = 1; 			// BAUD Rate Setting for 115200 avec Frc sans PLL et BRGH 0 -> 115156 sans PLL
	
  	//Niveau d'interruption
	IPC2bits.U1RXIP = 0b001; 
	IPC3bits.U1TXIP = 0b101; 

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
}

void UART_putc(unsigned char c)
{
	if(U1STAbits.UTXBF) {		// Si le buffer est plein
		txBuffer[(indexTxBuffer + txBufferSize) % TX_BUFFER_SIZE_MAX] = c;
		txBufferSize ++;
	} else U1TXREG = c;
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


void sendEmitN(unsigned char c)
{
	c += 0b11100000;
	UART_putc(c);
}

static void message_processing(unsigned char m)
{
	return;
	if((m&0b11100000)  != 0b00100000)
		return;
	if(m == 0b00111111)
		semit = 1;
	else if((m&0b11111100)  == 0b00110000)	// Trame calibrage par le PSoc ADR.100.aCalibrer
	{
		toDoCalibrage = m&0b11;
	}
	else if((m&0b11111000)  == 0b00111000) // Trame calibrage done ADR.11.ADR_RECEP
	{
		switch(m&0b111)
		{
			case 0b100 : calibrage |= 0b01; return;
			case 0b101 : calibrage |= 0b10; return;
			default : return;
		}
	}
}

void sendPosUpdated()
{
	UART_putc(0b00011111);
}

void sendCalibrageDone()
{
	UART_putc(0b00001111);
}

void sendBeginCalibrage(unsigned char adr)
{
	unsigned c = adr << 5;
	c += 0b10000;
	UART_putc(c);
}
