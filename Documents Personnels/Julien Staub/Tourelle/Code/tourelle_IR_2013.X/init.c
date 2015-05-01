#include "init.h"
#include "adc.h"
#include <p33EP512MU810.h>

extern int ** tab_adc_brut;

inline void init_adc(void)
{
    AD1CON1bits.FORM   = 0;		// Data Output Format: unsigned integer
    AD1CON1bits.SSRC   = 7;		// internal counter ends sampling and starts convertion
    AD1CON1bits.SSRCG  = 0;		// 
    AD1CON1bits.ASAM   = 1;		// ADC Sample Control: Sampling begins immediately after conversion
    AD1CON1bits.AD12B  = 0;		// 10-bit ADC operation

    AD1CON2bits.CSCNA = 1;		// incrément auto du canal à scanner
    AD1CON2bits.CHPS  = 0;		// Converts CH0

    AD1CON3bits.ADRC = 0;		// clock derived from systeme clock
    AD1CON3bits.ADCS = 5;	        // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/70M)*6 = 85.7ns > 76ns
                                        // ADC Conversion Time for 10-bit Tc=12*Tab =   (0.972222 MHz)
    //unused
    //AD1CON3bits.SAMC = 3;               // 2TAD for auto-sample time (min pout 10 bits convert)

    AD1CON1bits.ADDMABM = 0; 	// DMA buffers are built in scatter/gather mode
    AD1CON2bits.SMPI    = NB_RECEIVER -1 ;	// DMA increment adress after 32 sample/convertion
    AD1CON4bits.ADDMAEN  = 1;		// Converts in ADC1BUF0
    AD1CON4bits.DMABL =7; //4words of buffer// 7;      //128wods of buffer / analog input


		//AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
    AD1CSSH = 0xFFFF;	//all ADC pins enabled and have to be scanned
    AD1CSSL = 0xFFFF;

    ANSELA = 0xFFFF; //all pins analog
    ANSELB = 0xFFFF;
    ANSELC = 0xFFFF;
    ANSELD = 0xFFFF;
    ANSELE = 0xFFFF;
    ANSELG = 0xFFFF;


    IFS0bits.AD1IF = 0;			// Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 0;			// disable A/D interrupt
    AD1CON1bits.ADON = 1;		// Turn on the A/D converter

    
}

/*
//TODO : configurer la période
//le timer 3 va générer les interruptions pour démarer le sampling
inline void init_timer3(void)
{
    //timer3 fonctionne sur la cloch interne Fp = fosc/2 = 70Mhz
    TMR3 = 0x0000;
    PR3  = 279;	// Trigger ADC1 every 4µsec -> 7.813kHz/receiver
    IFS0bits.T3IF = 0;	// Clear Timer 3 interrupt
    IEC0bits.T3IE = 0;	// Disable Timer 3 interrupt

    //Start Timer 3
    T3CONbits.TON = 1;
}*/

inline void init_dma0(void)
{
	DMA0CONbits.AMODE = 2;   //l'ADC génére l'adresse (scatter/gather)
	DMA0CONbits.MODE  = 1;	 // Configure DMA one-shot no ping-pong
           //pour reenable le DMA, DMA0CON.CHEN = 1;
        DMA0CONbits.HALF = 0;
       

	DMA0PAD=(int)&ADC1BUF0;
	DMA0CNT=(NB_RECEIVER*128)-1; // TODO avec ou sans *128 ?

	DMA0REQ=13;	//DMA whith ADC0

	DMA0STAL = __builtin_dmaoffset(&tab_adc_brut);
	DMA0STAH = __builtin_dmapage(&tab_adc_brut);


	IFS0bits.DMA0IF = 0;			//Clear the DMA interrupt flag bit
        IEC0bits.DMA0IE = 1;			//Set the DMA interrupt enable bit

	DMA0CONbits.CHEN=1;


}

//copié et adapté de la section 7.1 des notes d'acpplication (DS70580)
//tourne à 140,03 MHz
inline void init_osc(void)
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD=74; // M=76
    CLKDIVbits.PLLPOST=0; // N2=2
    CLKDIVbits.PLLPRE=0; // N1=2
    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);
}


//tout input
inline void init_port(void)
{
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xFF;
    TRISE = 0xFF;
    TRISF = 0xFF;
    TRISG = 0xFF;
    TRISGbits.TRISG8=0;
    PORTGbits.RG8=1;
}
