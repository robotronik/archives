#include "main.h"
#include "sharp.h"
#include "comAsser.h"

#define SAMP_BUFF_SIZE  8        // Size of the input buffer per analog input
#define NUM_CHS2SCAN   2         // Number of channels enabled for channel scan

static int s_ain1Buff[SAMP_BUFF_SIZE]; ///<contient les valeurs utilisé par le sharp1
static int s_ain2Buff[SAMP_BUFF_SIZE]; ///<contient les valeurs utilisé par le sharp2 --> à faire

static int s_scanCounter=0;
static int s_sampleCounter=0;

static int s_proximite_sharp1;
static int s_proximite_sharp2;

static int stopSharp;
extern volatile int requestGetXY;
extern volatile int doneAsser;
extern volatile int requestGetAlpha;
extern volatile char forceIgnoreSharp;


/* Fonctions utilisées uniquement dans ce module */

/** Initialise le timer utilisé par les sharps
 *  \note De manière régulière les sharps génèrent une interruption
 */
void initTimerSharp();

/*=============================================================================
  ADC INITIALIZATION FOR CHANNEL SCAN
  =============================================================================*/
void initSharp(void)
{
    TRIS_SHARP_AVANT   = 1;
    TRIS_SHARP_ARRIERE = 1;

    /*Config Sharp 1*/
    AD1CON1bits.FORM   = 0;        // Data Output Format: Signed Fraction (Q15 format)
    AD1CON1bits.SSRC   = 2;        // Sample Clock Source: GP Timer starts conversion
    AD1CON1bits.ASAM   = 1;        // ADC Sample Control: Sampling begins immediately after conversion
    AD1CON1bits.AD12B  = 0;        // 10-bit ADC operation


    AD1CON2bits.CSCNA = 1;        // Scan Input Selections for CH0+ during Sample A bit
    AD1CON2bits.CHPS  = 0;        // Converts CH0
/*ajouté le 25_05_14 debut*/
//    AD1CON2bits.ALTS = 1 ; //Uses channel input selects for Sample A on first sample and Sample B on next sample
//    AD1CON1bits.ADDMABM = 1;/* DMA sbuffers are written in the order of conversion. The module provides an address to the DMA
/*channel that is the same as the address used for the non-DMA stand-alone buffer*/
//    AD1CON4bits.DMABL = 0 ;// Allocates 1 word of buffer to each analog input/**/
    
/*ajouté le 25_05_14 fin*/
    AD1CON3bits.ADRC = 0;        // ADC Clock is derived from Systems Clock
    AD1CON3bits.ADCS = 63;        // ADC Conversion Clock Tad=Tcy*(ADCS+1)= (1/40M)*64 = 1.6us (625Khz)
    // ADC Conversion Time for 10-bit Tc=12*Tab = 19.2us

    AD1CON2bits.SMPI    = (NUM_CHS2SCAN-1);    // 2 ADC Channel is scanned

    //AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
    AD1CSSLbits.CSS0=1;                     // Enable AN0 for channel scan
    //AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
    AD1CSSLbits.CSS1=1;                     // Enable AN1 for channel scan

    // Initialize MUXA Input Selection
    //_CH0SA = 0 ;// Select AN0 for CH1 +ve input
    // Initialize MUXB Input Selection
    //_CH0SB = 1 ;// Select AN1 for CH1 +ve input

    //AD1PCFGH/AD1PCFGL: Port Configuration Register
    AD1PCFGL=0xFFFF;
    AD1PCFGLbits.PCFG0 = 0;        // AN0 as Analog Input
    AD1PCFGLbits.PCFG1 = 0;        // AN1 as Analog Input
   

    IFS0bits.AD1IF = 0;            // Clear the A/D interrupt flag bit
    IEC0bits.AD1IE = 1;            // Enable A/D interrupt
    AD1CON1bits.ADON = 1;        // Turn on the A/D converter

    initTimerSharp();

}

/*=============================================================================
  Timer 3 is setup to time-out every 125 microseconds (8Khz Rate). As a result, the module
  will stop sampling and trigger a conversion on every Timer3 time-out, i.e., Ts=125us.
  =============================================================================*/
void initTimerSharp()
{
    TMR_SHARP = 0x0000;//Met le timer � 0
    PR_SHARP  = 4999;
    IF_SHARP = 0;
    IE_SHARP = 0;

    //Start Timer 3
    TCONbits_SHARP.TON = 1;
}

/*=============================================================================
  ADC INTERRUPT SERVICE ROUTINE
  =============================================================================*/

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {

    IEC0bits.AD1IE = 0 ;
     _RB4 = 1 - _RB4;
    switch (s_scanCounter) {
     
        case 1:
            s_ain2Buff[s_sampleCounter]=ADC1BUF0;
            break;/*ajouté le 25_05_14 fin*/
        case 0:
            s_ain1Buff[s_sampleCounter]=ADC1BUF0;
            break;

        default:
            break;
    }
    //send_xbee_string("test", 4);
//    send_xbee_long((long)ADC1BUF0);
//    pause_ms(300) ;
//    UART_xbee_putc('\r');
//    pause_ms(300) ;
    
    s_scanCounter++;
    if(s_scanCounter==NUM_CHS2SCAN) {
        s_scanCounter=0;
        s_sampleCounter++;
    }

    if(s_sampleCounter==SAMP_BUFF_SIZE){
        s_sampleCounter=0;
    }

    s_proximite_sharp1 = s_ain2Buff[s_sampleCounter];
    s_proximite_sharp2 = s_ain1Buff[s_sampleCounter];

    // arret d'urgence, on est vraiment trop près
    if(s_proximite_sharp1 > DISTANCE_ARRET_URGENCE) {sendStop() ;
        if(stopSharp==0 && forceIgnoreSharp==0) {
            stopSharp       = 1;
            TMR_EVITEMMENT  = 0x0000;
            T1CONbits.TON   = 1;
            

            //LATBbits.LATB5=1;
        }
    }
    if(s_proximite_sharp2 > DISTANCE_ARRET_URGENCE) {sendStop() ;
       if(stopSharp==0 && forceIgnoreSharp==0) {
            stopSharp       = 1;
            TMR_EVITEMMENT  = 0x0000;
            T1CONbits.TON   = 1;
            sendStop() ;

            //LATBbits.LATB5=1;
        }
    }
 //      /*ajouté le 25_05_14 fin*/
    else {
        T1CONbits.TON=0;
        stopSharp=0;
        // LATBbits.LATB5=0;
    }

    IFS0bits.AD1IF = 0;        // Clear the ADC1 Interrupt Flag
    IEC0bits.AD1IE = 1 ;
}

int getDistance(Sharp sharp) {
    switch (sharp) {
        case SHARP_1:
            return s_proximite_sharp1; ///< \todo utiliser le bon sharp
        case SHARP_2:
            return s_proximite_sharp2; ///< \todo utiliser le bon sharp
        default:
            return 0xFFFF;
    }
}
