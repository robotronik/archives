#include "main.h"
#include "sharp.h"
#include "comAsser.h"
#include "comXbee.h"
#include "debug.h"
#include "timer.h"
#include "servo.h"

/** \todo COMMENTÉ POUR PERMETTRE LA COMPILATION, à vérifier que c’était bien à faire
 * #include <p33FJ128MC802.h>
 * _FOSCSEL(FNOSC_FRCPLL & IESO_ON)
 * _FOSC(FCKSM_CSECMD & IOL1WAY_ON & OSCIOFNC_ON & POSCMD_NONE)
 * _FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
 */

//extern volatile enum EtatMachine etatSuivant;
extern volatile int doneAsser;

void init_osc();
void initTimerEvite();

void init() {

    // Défaut toutes les pattes sont en entrée sauf PCD et PGC (qui sont concervés)
    TRISA = 0xFFFF;
    TRISB = TRISB | 0xFFFC;

    init_osc();
    //__builtin_write_OSCCONL(OSCCON & ~(0x40));    // Débloquage des RPIN et RPOR
    initDebug();
    UART_init_asser();
    UART_init_xbee();
 //   initSharp();
    initTimer();
    initTimerEvite();
    initPWM();
    //__builtin_write_OSCCONL(OSCCON | 0x40);        // Rebloquage des RPIN et RPOR
}

void init_osc() {
    // Init de la PLL : 7,37 -> 79,22 MHz !!!
    CLKDIVbits.PLLPRE = 0;      // (FRC) /2
    PLLFBD = 41;                // (FRC/2) *43
    CLKDIVbits.PLLPOST = 0;     // (FRC/2*43) /2

    __builtin_write_OSCCONH((OSCCONH | 1 )& 0xF9);  // Choix de l'horloge FRC avec PLL
    __builtin_write_OSCCONL(OSCCONL | 1);           // Changement d'horloge
    //while (!OSCCONbits.LOCK);                     // Attend le bloquage de la PLL (debug)

    while (OSCCONbits.OSWEN);                         // Attend le changement
}

void initTimerEvite() {
    TMR_EVITEMMENT              = 0x0000;//Met le timer à 0
    PR_EVITEMMENT               = 15000;
    TCONbits_EVITEMMENT.TCKPS   = 0b01;//prescaler 8

    IP_EVITEMMENT  = 0x01; // Set Timer1 Interrupt Priority Level
    IF_EVITEMMENT  = 0;   // Clear Timer1 Interrupt Flag
    IE_EVITEMMENT  = 1;   //Enable interupt

    //Start Timer 1
    //TCON_EVITEMMENTbits.TON = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
// TODO
    //etatSuivant=EVITER;
    IF_EVITEMMENT=0;
}
