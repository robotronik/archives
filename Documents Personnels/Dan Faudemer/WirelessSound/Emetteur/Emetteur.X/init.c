#include <pic18f4550.h>
#include "init.h"

/* Define pragma config*/

/* Usb clock from primary oscillator*/
//#pragma config USBDIV = 1
/* Use internal oscillator and HS for usb */
#pragma config FOSC = INTOSC_HS
/* Switch to internal oscillator if fail clock in HS*/
#pragma config FCMEN = OFF

/*Disable USB voltage regulator*/
#pragma config VREGEN = OFF

/*Disable watch dog*/
#pragma config WDT = OFF




void init_clock()
{
    /* 8 Mhz Internal clock => Fcy = 2Mhz*/
    OSCCONbits.IRCF = 0b111;
}


void init_ADC()
{
    /* ADC on AN2*/
    ADCON0bits.CHS = 2;
    
    /* VSS & VDD ref */
    ADCON1bits.VCFG1 = 0 ;
    ADCON1bits.VCFG0= 0;

    /* AN2, AN1 & AN0 are an anlog input*/
    ADCON1bits.PCFG = 0b1100;

    ADCON2bits.ADCS = 0; /* Fadc = Fosc/2 */



    // Tad ( Frequency )
    ADCON2bits.ADCS = 0b001; // Maximum frequency 10Mhz

    /* Acquisition time (load the capacitor)*/
    ADCON2bits.ACQT =0b001; // 2 * TAD = 40 us /* TODO : check that */


    /*Clear interrupt flag*/
    PIR1bits.ADIF =0;
    /* Activate interruption on ADC*/
    PIE1bits.ADIE = 1;
    /* Enable interrupts*/
    INTCONbits.GIE =1;



    /* Enable ADC */
    ADCON0bits.ADON = 1;


    /* Start ADC*/
//    ADCON0bits.GO = 1; it is timer 2 that start the adc
    
    

}

/*Timer for ADC value*/
void init_timer ()
{
    T2CONbits.TMR2ON = 0; // Timer 2 is off
    T2CONbits.TOUTPS = 0; //No postscaller
    T2CONbits.T2CKPS = 0; //Prescaler = 1 ( F=2Mhz (Fosc/4))

    //Enable TMR2 & PR2 match interupt
    PIE1bits.TMR2IE = 1;

    PR2 = 50; //2Mhz /50 = 40khz

//Start timer 2
    T2CONbits.TMR2ON = 1;
    
}

/* Init debug led */
void init_led() {
//RD5 as list full 
    TRISDbits.RD5 = 0;
    LED_FULL = 0;
    TRISDbits.RD6 = 0;
    LED_DEBUG2 = 0;

    TRISDbits.RD7 = 0;
    LED_DEBUG3 = 0;

}

void init_spi()
{
    /* SPI Control configuration*/
    SSPCON1bits.SSPEN = 1;
    SSPCON1bits.CKP = 0;

    SSPCON1bits.SSPM = 0 ;// Fosc/4 = 2Mhz (125 ko/s)

    /*Interruption control*/

    /* CSN is an output*/
    TRISBbits.RB3 = 0;
    CSN = 1; /* Chit not selected*/


    /* Chip eanable CE = 1*/
    TRISBbits.RB3 = 0;
    LATBbits.LB3 = 1;
    
}