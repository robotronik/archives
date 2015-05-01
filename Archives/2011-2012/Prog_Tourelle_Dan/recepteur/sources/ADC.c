#include "ADC.h"


void initADC()
{
    AD1PCFGLbits.PCFG0 = 0;			// AN0 as Analog Input

	AD1CON1bits.FORM = 0;			// Integer
	AD1CON1bits.ASAM = 0;			// ADC Sample Control: Sampling manually
	AD1CON1bits.SSRC = 0b111;		    // METTRE QUAND MM UNE PERIODE POUR QU IL PRENNE L ECHANTILLON
	AD1CON1bits.AD12B = 1;			// 12-bit ADC operation
	AD1CON1bits.SIMSAM = 0;

	AD1CON2bits.CHPS = 0;			// Converts CH0
				
	// Direct memory access (DMA)
	AD1CON1bits.ADDMABM = 1; 		// DMA buffers are built in conversion order mode
	AD1CON2bits.SMPI = 0;			// SMPI must be 0

	// References de tension
	AD1CON2bits.VCFG = 0;


    //AD1CHS0: A/D Input Select Register
    AD1CHS0bits.CH0SA = 0;			// MUXA +ve input selection (AN0) for CH0
	AD1CHS0bits.CH0NA = 0;			// MUXA -ve input selection (Vref-) for CH0

    //AD1PCFGH/AD1PCFGL: Port Configuration Register
	AD1CSSL = 0;				// 0=> Skip ANx for input scan

    
    IFS0bits.AD1IF = 0;				// Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 0;				// Do Not Enable A/D interrupt 
    AD1CON1bits.ADON = 1;			// Turn on the A/D converter	
}

short getADCValue ()
{
	AD1CHS0 = 0;	        // Connect RB0 as CH0 input 
					// 0=> Skip ANx for input scan
	
	//convertion analogique => numerique
	AD1CON1bits.SAMP = 1;  // start sampling
	
	while (!AD1CON1bits.DONE);

	short retour = ADC1BUF0;
	ADC1BUF0 = 0;

	return retour; // yes then get ADC value
}

void waitADCFrontMontant()
{

}
