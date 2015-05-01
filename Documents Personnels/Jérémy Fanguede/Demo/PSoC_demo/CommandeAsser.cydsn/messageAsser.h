/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "main.h"

/** Variables globales **/

	// /!\ Big endian : x_L = tab[4] ; x_H = tab[5]
typedef struct S_Pos {
	unsigned short angle;
	unsigned short y;
	unsigned short x;
} T_Pos;
	
	// Set (ou incrément) automatique, reset (ou décrément) à l'utilisation :
	// while(!isAsserDataOK); isAsserDataOK = FALSE;
	// while(nbAsserDone < N); nbAsserDone --;
extern volatile boolean isAsserReady;
extern volatile boolean isAsserDataOK;
extern volatile int nbAsserDone;

/** Fonctions (mm, °) **/

void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision);
void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision);
void sendALPHA(unsigned short a, unsigned short precision);
void sendSUPPR(unsigned char nbCommande);	// Supprime les N dernières commandes
void sendFIN_MATCH();
void sendRED();
void sendPURPLE();
void sendTEST();
void updatePos();
T_Pos getPos();
T_Pos getWaitPos();
void calleStart();

#endif
