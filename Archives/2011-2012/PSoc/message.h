/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "main.h"

/** Variables globales **/

typedef union {
	unsigned char tab[6];
	struct {
		unsigned int x;
		unsigned int y;
		unsigned int angle;
	};
} T_Pos_TabChar;
extern T_Pos_TabChar posAsser;
extern boolean isAsserReady;
extern boolean hasAsserDone;
extern boolean isPosAsserOK;

/** Constantes : Reponses pour l'interruption **/

#define TEST				0x85	// Connexion bonne
#define POS					0x86
#define DONE				0x00	// Déplacement terminé

/** Fonctions **/

void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision);
void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision);
void sendALPHA(unsigned int a, unsigned char precision);
void sendSUPPR(unsigned char nbCommande);	// Supprime les N dernières commandes
void sendS_90();
void sendBLUE();
void sendRED();
void sendTEST();
void sendGET_POS();

T_Pos_TabChar *getLastPos();

#endif
