#ifndef _MODELISATION_H
#define _MODELISATION_H

#include "../../sources/Boolean.h"

#define VBAT_MAX	13800				// [mV]
#define POIDS		5					// [kg] Poids total du robot

// u < VBAT_MAX et side = 0 ou 1 svp
// Retourne l'accélération linéaire en m/s² du centre de la roue par rapport au sol
inline double motors_run(int u, boolean side, double vit_sol);

#endif
