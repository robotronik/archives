/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#ifndef _ASSER_H
#define _ASSER_H

#define PERIOD 1																// [dizieme de ms] Max : 16 sans préscaler sur le timer 1
#define PERIOD_TIMER (unsigned short)4000 * PERIOD								// (Fosc = 80 MHz et Fcy = Fosc/2)

void asser_delta();
void asser_alpha();

#endif
