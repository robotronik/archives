#include "sharp.h"
#include "main.h"
#include "comAsser.h"




#define  SAMP_BUFF_SIZE	 		8		// Size of the input buffer per analog input
#define  NUM_CHS2SCAN			1		// Number of channels enabled for channel scan


volatile int stopSharp=0;
extern volatile int requestGetXY;
extern volatile int doneAsser;
extern volatile int requestGetAlpha;
extern volatile char forceIgnoreSharp;



/*=============================================================================
ADC INITIALIZATION FOR CHANNEL SCAN
=============================================================================*/
void initSharp(void)
{

		AD1CON1bits.FORM   = 3;		// Data Output Format: Signed Fraction (Q15 format)
		AD1CON1bits.SSRC   = 2;		// Sample Clock Source: GP Timer starts conversion
		AD1CON1bits.ASAM   = 1;		// ADC Sample Control: Sampling begins immediately after conversion
		AD1CON1bits.AD12B  = 0;		// 10-bit ADC operation

		AD1CON2bits.CSCNA = 1;		// Scan Input Selections for CH0+ during Sample A bit
		AD1CON2bits.CHPS  = 0;		// Converts CH0

		AD1CON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock
		AD1CON3bits.ADCS = 63;		// ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*64 = 1.6us (625Khz)
									// ADC Conversion Time for 10-bit Tc=12*Tab = 19.2us

		AD1CON2bits.SMPI    = (NUM_CHS2SCAN-1);	// 2 ADC Channel is scanned

		//AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
                AD1CSSLbits.CSS0=1;                     // Enable AN0 for channel scan
                

 		//AD1PCFGH/AD1PCFGL: Port Configuration Register
		AD1PCFGL=0xFFFF;
		AD1PCFGLbits.PCFG0 = 0;		// AN0 as Analog Input
  


                IFS0bits.AD1IF = 0;			// Clear the A/D interrupt flag bit
                IEC0bits.AD1IE = 1;			// Enable A/D interrupt
                AD1CON1bits.ADON = 1;		// Turn on the A/D converter



}

/*=============================================================================
Timer 3 is setup to time-out every 125 microseconds (8Khz Rate). As a result, the module
will stop sampling and trigger a conversion on every Timer3 time-out, i.e., Ts=125us.
=============================================================================*/
void initTimerSharp()
{
        TMR3 = 0x0000;//Met le timer à 0
        PR3 = 4999;
        IFS0bits.T3IF = 0;
        IEC0bits.T3IE = 0;

        //Start Timer 3
        T3CONbits.TON = 1;

}

/*=============================================================================
ADC INTERRUPT SERVICE ROUTINE
=============================================================================*/
int  ain0Buff[SAMP_BUFF_SIZE];
int  ain1Buff[SAMP_BUFF_SIZE];

int  scanCounter=0;
int  sampleCounter=0;

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{

	switch (scanCounter)
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
            if(stopSharp==0 && forceIgnoreSharp==0) {
            stopSharp = 1;
            TMR1=0x0000;
            T1CONbits.TON=1;
            //LATBbits.LATB5=1;
            }

            
        }        
        else
        {
            T1CONbits.TON=0;
            stopSharp=0;
           // LATBbits.LATB5=0;
            
        }

    IFS0bits.AD1IF = 0;		// Clear the ADC1 Interrupt Flag

}