
#ifndef _AFFICHAGE_
#define _AFFICHAGE_


/*------------------------------- include -----------------------------------*/


#include "main.hpp"



/*------------------------------- declaration fonctions ---------------------*/


Robot * configuration_initiale_robot(Uint32 couleur, Coord position);
float tourner_a_90(Robot* R);
void oneStepSimu();
void afficher_robot(Robot* R);

#endif
