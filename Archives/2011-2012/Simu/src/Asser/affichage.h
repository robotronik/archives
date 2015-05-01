
#ifndef _AFFICHAGE_
#define _AFFICHAGE_

#include "prog2.h"

/*------------------------------- declaration fonctions ---------------------*/


Robot * configuration_initiale_robot(Uint32 couleur, Coord position);
void tourner_a_90(Robot* R);
void afficher_robot(Robot* R);

/*-------------------- deplacement robot simu -----------*/

void affiche(int isImmediate);

float avancer(float deplacement, Robot *R);
float tourner(float rotation, Robot* R);

#endif
