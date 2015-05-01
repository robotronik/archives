/*-----------------------------------------------------*
 * Protocole de communication avec la carte Asser      *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_COMMUN_H
#define _MESSAGE_COMMUN_H

#define TOUR	(long)16307 			// [tck/tr] = 2*1440[tcks]*LARGEUR/diamètre roue(=41mm)

typedef enum {
/** Asser <-> PSoC (0 to 8) **/
	CONNECT, ACK_CONNECT, GET_POS, GET_COEF_FREIN, GET_COEF_FREIN_ALPHA, 
	GET_MIN_SPEED, GET_ACCEL_MAX, GET_DEBUG_SETTING, GET_COMMANDE, 
/** PSoC -> Asser (9 to 19) **/
	RED, PURPLE, FIN_MATCH, ALPHA, XY_RAPIDE, XY_COURBE, CALLE_G, CALLE_H, 
	CALLE_D, PAUSE, SUPPR, SET_COEF_FREIN, SET_COEF_FREIN_ALPHA, 
	SET_ACCEL_MAX, TEST_BRANCHEMENT, SET_DEBUG_SETTING, GET_QEI, 
/** Asser -> PSoC (1A to 1B) **/
	DONE, START, 
/** Nombre des commandes precedentes **/
	NB_CMD, 
/** Interne a l'asser, valeurs de deplacement differentes des commandes externes **/
	CALLE_G2 = NB_CMD, CALLE_H2, CALLE_D2, REGLAGE_CMD_MIN1, REGLAGE_CMD_MIN2, 
	REGLAGE_CMD_MIN3, FREINAGE
} commande_t;
	
extern const int nbArgPSoCToAsser[NB_CMD];
extern const int nbArgAsserToPSoC[NB_CMD];
	
#endif
