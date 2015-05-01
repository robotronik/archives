/*-----------------------------------------------------*
 * Initialisation registre et variables                *
 *-----------------------------------------------------*/





#include "main.h"
#include "servo.h"
#include "sharp.h"
#include "machine.h"
_FOSCSEL(FNOSC_FRCPLL & IESO_ON)
_FOSC(FCKSM_CSECMD & IOL1WAY_ON & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)

extern volatile enum EtatMachine etatSuivant;

void init_osc() {
	// Init de la PLL : 7,37 -> 79,23 MHz !!!
	CLKDIVbits.PLLPRE = 0;		// (FRC) /2
	PLLFBD = 41;				// (FRC/2) *43
	CLKDIVbits.PLLPOST = 0;		// (FRC/2*43) /2
	
	__builtin_write_OSCCONH((OSCCONH | 1 )& 0xF9);	// Choix de l'horloge FRC avec PLL
	__builtin_write_OSCCONL(OSCCONL | 1);			// Changement d'horloge
	//while (!OSCCONbits.LOCK);						// Attend le bloquage de la PLL (debug)
	


	while (OSCCONbits.OSWEN); 						// Attend le changement
}

void io_init(void)
{
	_TRISB5 = 0;	// DEL en mode output
        _TRISB4 = 0;
	_RB5 = 1;	 	
}

void UART_init_asser()
{
	AD1PCFGLbits.PCFG5 = 1;	// Désactivation de l'entrée analogique !!!
	RPOR1bits.RP2R = 3;							// Tx1 -> RP2
	RPINR18bits.U1RXR = 3;						// RP3 -> Rx1

	//U1MODEbits.LPBACK = 1;	// Debug : Tx1 -> Rx1

	// Low speed : BRG = 79,23 MHz / 32 / Baudrate - 1
	U1MODEbits.BRGH = 1;	// High speed : BRG = 79,23 MHz / 8 / Baudrate - 1
	U1BRG = 85; 			// BAUD Rate Setting for 115200 gives 115160 bauds
	
#ifdef INT_UART_TX
	U1STAbits.UTXISEL1 = 1;	// Interrupt on empty FIFO, last byte is being sent
	U1STAbits.UTXISEL0 = 0;	//                      "
#endif //#ifdef INT_UART_TX

	IFS0bits.U1RXIF = 0; 	// On evite des interruptions à l'activation
	IEC0bits.U1RXIE = 1;	// Activation de l'interruption sur réceptions
#ifdef INT_UART_TX
	IFS0bits.U1TXIF = 0;
	IEC0bits.U1TXIE = 1;	// Activation de l'interruption sur l'envoie
#endif //#ifdef INT_UART_TX
	//IFS4bits.U1EIF = 0;
	//IEC4bits.U1EIE = 1;	// Activation de l'interruption sur erreurs

	U1MODEbits.UARTEN = 1; 	// Enable UART
	U1STAbits.UTXEN = 1; 	// Enable UART TX
}



void UART_init_xbee()
{
    TRISBbits.TRISB7 = 1;	

    // Aterminer 
	RPOR4bits.RP8R = 0b00101;				// Tx2 -> RP8
	RPINR19bits.U2RXR = 7;					// RP7 -> Rx2


	// Low speed : BRG = 79,23 MHz / 32 / Baudrate - 1
	U2MODEbits.BRGH = 1;	// High speed : BRG = 79,23 MHz / 8 / Baudrate - 1
	U2BRG = 85; 			// BAUD Rate Setting for 115200 gives 115160 bauds

#ifdef INT_UART_TX
	U2STAbits.UTXISEL1 = 1;	// Interrupt on empty FIFO, last byte is being sent
	U2STAbits.UTXISEL0 = 0;	//                      "
#endif //#ifdef INT_UART_TX

	IFS1bits.U2RXIF = 0; 	// On evite des interruptions à l'activation
	IEC1bits.U2RXIE = 1;	// Activation de l'interruption sur réceptions
#ifdef INT_UART_TX
	IFS1bits.U2TXIF = 0;
	IEC1bits.U2TXIE = 1;	// Activation de l'interruption sur l'envoie
#endif //#ifdef INT_UART_TX
	//IFS4bits.U1EIF = 0;
	//IEC4bits.U1EIE = 1;	// Activation de l'interruption sur erreurs

	U2MODEbits.UARTEN = 1; 	// Enable UART
	U2STAbits.UTXEN = 1; 	// Enable UART TX
}

void timer_init()
{
	
	
	T4CONbits.TSIDL = 1;
	T4CONbits.TGATE  = 0 ;
	T4CONbits.TCKPS = 0b11;//Prescaller 256
	
	TMR4 = 0; //Reset du conteur
        TMR5 = 0;
        T4CONbits.T32 = 1;
	
	//période de 20 ms
	PR5 = 207;//25000; //Réglage de la fréquence de l'horloge. (Fcy = (40Mhz/256)=
        PR4 = 0;
	
	T4CONbits.TCS = 0;
	
	IPC7bits.T5IP = 0x01; // Set Timer1 Interrupt Priority Level
        IFS1bits.T5IF = 0; // Clear Timer1 Interrupt Flag
        IEC1bits.T5IE = 1; //Enable interupt
	

}

void initTimerEvite()
{
        TMR1 = 0x0000;//Met le timer à 0
        PR1 = 15000;
        IFS0bits.T1IF = 0;
        IEC0bits.T1IE = 0;
        T1CONbits.TCKPS=0b01;//prescaler 8

        IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
        IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
        IEC0bits.T1IE = 1; //Enable interupt

        //Start Timer 1
        //T1CONbits.TON = 1;

}


void init()
{
    init_osc();
	//__builtin_write_OSCCONL(OSCCON & ~(0x40));	// Débloquage des RPIN et RPOR
    io_init();
    UART_init_asser();
    UART_init_xbee();
    initPWM();
    initTimerSharp();
    initSharp();
    timer_init();
    initTimerEvite();
	//__builtin_write_OSCCONL(OSCCON | 0x40);		// Rebloquage des RPIN et RPOR
}


void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void)
{
    while(1)
    {
        sendStop();
        pause_ms(500);
    }
/*	switch (scanCounter)
	{
		case 0:
			ain0Buff[sampleCounter]=ADC1BUF0;
			break;

		default:
			break;

	}

	scanCounter++;
	if(scanCounter==NUM_CHS2SCAN)	{
		scanCounter=0;
		sampleCounter++;
	}

	if(sampleCounter==SAMP_BUFF_SIZE)
		sampleCounter=0;


        if(ain0Buff[sampleCounter]>1500)
        {
            stopSharp=1;


        }
        else
        {
            stopSharp=0;

        }

    IFS0bits.AD1IF = 0;		// Clear the ADC1 Interrupt Flag
*/
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    etatSuivant=EVITER;

    IFS0bits.T1IF=0;

}