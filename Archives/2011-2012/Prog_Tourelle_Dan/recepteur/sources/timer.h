#ifndef TIMER_H
#define TIMER_H

#include <p33FJ128MC802.h>

#define TIME_OUT ((unsigned short)15000)

#define stopTimer() T1CONbits.TON=0
// Donne le temps ecoule depuis le start
#define getDtus() (unsigned short)(TMR1/4)
#define timeOut() (getDtus()>TIME_OUT)

//#define CONV_US 266  // Calcule a l oscillo

// Fonction a appeler au debut du programme
void initTimer();

// (Re)demarre le timer pour un nouveau calcul du temps
void startTimer();


#endif // TIMER_H defined
