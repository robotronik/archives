#ifndef CALIBREUR_H
#define CALIBREUR_H

#include "ADC.h"
#include "timer.h"

/*
	3 ms pour un calcul du pourcentage de pris, soit 3 mini-trames pleines et creuses
	25us pour chopper un oui, donc 120 reponses
*/

extern volatile unsigned short seuil;

void calibrer();

#endif // CALIBREUR_H defined
