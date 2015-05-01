#include "main.h"
#include "timer.h"
#include "debug.h"
#include "comAsser.h"

/** interruption effectué au bout des 90s
 * \warning l’arrêt des moteurs est non testé
 */
void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void);

/////////////////////////////////////////////////////////////////

void initTimer()
{
    TCONbits_MATCH_A.TSIDL = 0;
    TCONbits_MATCH_B.TSIDL = 0;
    TCONbits_MATCH_A.TGATE = 0;
    TCONbits_MATCH_A.TCS   = 0;
    TCONbits_MATCH_A.TCKPS = 0b11;//Prescaller 256

    TMR_MATCH_A = 0; //Reset du conteur
    TMR_MATCH_B = 0;
    TCONbits_MATCH_A.T32 = 1; // Sur 32 bits

    // Le mach se termine au bout de NB_TIC_MATCH (sur 32 bits).
    PR_MATCH_A = NB_TIC_MATCH &  0x0000FFFF; //  poid faible
    PR_MATCH_B = NB_TIC_MATCH >> 16;         //  poid fort

    TCONbits_MATCH_A.TCS = 0;

    TCONbits_MATCH_A.TON = 1; // On démarre le timer (sur 32 bits)
    TCONbits_MATCH_B.TON = 1;
    IP_MATCH_B = 0x01; // Set Timer1 Interrupt Priority Level
    IF_MATCH_B = 0; // Clear Timer1 Interrupt Flag
    IE_MATCH_B = 1; //Enable interupt
}

unsigned long int get_time() {
    Int16_32 temp;
    temp.higher = TMR_MATCH_B;
    temp.lower = TMR_MATCH_A;
    return temp.all;
};


void start_timer_match() {
    TMR_MATCH_A = 0; // Reset du compteur
    TMR_MATCH_B = 0;
    TCONbits_MATCH_A.TON = 1; // On démarre le timer (sur 32 bits)
    TCONbits_MATCH_B.TON = 1;
    affiche_debug(DEBUG_MSG | 0b10010100);
}

bool timeEnd() {
    return get_time() > NB_TIC_MATCH;
}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) {

    TCONbits_MATCH_A.TON = 0;
    TCONbits_MATCH_B.TON = 0;

    // On stop tout
    while(1) {
        sendStop();
        affiche_debug(DEBUG_MSG | 0b01010101);
    }

    IF_MATCH_B = 0;    // Clear flag
}

