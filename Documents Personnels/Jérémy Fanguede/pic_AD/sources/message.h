/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_H
#define _MESSAGE_H

#define NB_MAX_COMMANDES 128
extern unsigned char commande[NB_MAX_COMMANDES];
extern short nbCommande;
extern short indexCommande;

void message_processing(unsigned char inMessage);	// Appelée à l'interruption de l'UART

/** Interne **/

#define CALLE_G2			0x82
#define CALLE_H2			0x83
#define CALLE_D2			0x84
#define FREINAGE			0x85

/** Commandes **/

// 								0 <= ALPHA < MAX_ALPHA
#define MAX_ALPHA				0x80
#define XY_RAPIDE				0x80
#define XY_COURBE				0x81
#define SUPPR					0x82	// Supprime les N dernières commandes
#define FIN_MATCH				0x83
#define RED						0x84
#define PURPLE					0x85
#define TEST					0x86	// Connexion bonne
#define POS						0x87
#define CALLE_G					0x88
#define CALLE_H					0x89
#define CALLE_D					0x8A
#define SET_COEF_FREIN			0x8B
#define SET_COEF_FREIN_ALPHA	0x8C
#define SET_COEF_FREIN_ALPHA2	0x8D
#define GET_COEF_FREIN			0x8E
#define GET_COEF_FREIN_ALPHA	0x8F
#define GET_COEF_FREIN_ALPHA2	0x90
#define PAUSE					0x91
#define GET_COMMANDE			0x92
#define TEST_BRANCHEMENT		0x93
#define ACK						0x94
#define SET_MIN_SPEED			0x95
#define GET_MIN_SPEED			0x96

#define AD_ALPHA				0x98
#define AD_DELTA				0x99

/** Reponses **/

#define DONE					0x00	// Déplacement terminé
//#define TEST					0x86	// Connexion bonne
//#define POS					0x87
//#define GET_COEF_FREIN		0x8E
//#define GET_COEF_FREIN_ALPHA	0x8F
//#define GET_COEF_FREIN_ALPHA2	0x90
//#define GET_COMMANDE			0x92
//#define TEST_BRANCHEMENT		0x93
//#define ACK					0x94
//#define GET_MIN_SPEED			0x96

#define chargeCommandeSuiv {											\
	nbCommande --;														\
	commandeCourante = commande[indexCommande];							\
	switch (commandeCourante) {											\
	case CALLE_G :														\
	case CALLE_H :														\
	case CALLE_D :														\
		break;															\
	case XY_RAPIDE :													\
	case XY_COURBE :													\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consigneX = (unsigned short) commande[indexCommande] << 8;		\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consigneX |= commande[indexCommande];							\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consigneY = (unsigned short) commande[indexCommande] << 8;		\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consigneY |= commande[indexCommande];							\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consPrecision = (unsigned long) commande[indexCommande] << 24;	\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consPrecision |= (unsigned long) commande[indexCommande] << 16;	\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consPrecision |= (unsigned short) commande[indexCommande] << 8;	\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consPrecision |= commande[indexCommande];						\
		break;															\
	default :	/* Asser alpha */										\
		consAlpha = (unsigned short) commande[indexCommande] << 8;		\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consAlpha |= commande[indexCommande];							\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consPrecision = (unsigned short) commande[indexCommande] << 8;	\
		indexCommande ++; indexCommande %= NB_MAX_COMMANDES;			\
		consPrecision |= commande[indexCommande];						\
	}																	\
	indexCommande ++; indexCommande %= NB_MAX_COMMANDES;				\
	goto choice;	/* Retourne dans getErreurs */						\
}

#endif
