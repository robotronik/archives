#ifndef TIMER_H
#define TIMER_H

#define LONGUEUR_TRAME 0x01C0 // Discretized : 5ms

#include <p33FJ128MC802.h>

void initTimer();
void waitForTrame();
void waitForCalcul();

#endif // TIMER_H defined
