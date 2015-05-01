/*-----------------------------------------------------*
 * Protocole de communication avec la carte Asser      *
 *-----------------------------------------------------*/
 
#include "messageCommun.h"

// Nombres d'arguments de chaque commande, index�s par la commande (cf enum du .h)
const int nbArgAsserToPSoC[NB_CMD] = {
	0, 0, 6, 2, 2, 
	2, 1, 4, 9 /*ou moins suivant la commande*/, 
	-1, -1, -1, -1, -1, -1, -1, -1, 
	-1, -1, -1, -1, -1, 
	-1, -1, -1, -1, 
	0, 0
};
	
const int nbArgPSoCToAsser[NB_CMD] = {
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 
	0, 0, 0, 4, 8, 8, 0, 0, 
	0, 0, 1, 2, 2, 
	1, 0, 4, 0, 
	-1, -1
};
