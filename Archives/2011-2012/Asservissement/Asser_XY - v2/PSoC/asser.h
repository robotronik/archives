/*-----------------------------------------------------*
 * Protocole de communication avec l'uart              *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "Boolean.h"
#include "..\..\sources\messageCommun.h"

#define PRECISION_XY 1
#define PRECISION_ENCHAINE	300
#define PRECISION_ALPHA 1

	// /!\ Big endian : x_L = tab[4] ; x_H = tab[5]
typedef struct S_Pos {
	unsigned short angle;
	unsigned short y;
	unsigned short x;
} T_Pos;

/** Variables globales **/

extern boolean isRxAsserBypassed;	// Connection directe de l'asser à l'XBee
extern boolean isTxAsserSniffed;

extern unsigned char dataAsser[9];

/** Fonctions (mm, °) **/

void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision);
void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision);
void sendALPHA(unsigned short a, unsigned short precision);
void sendSUPPR(unsigned char nbCommande);	// Supprime les N dernières commandes
void sendFIN_MATCH();
void sendRED();
void sendPURPLE();
void sendGET_POS();
void sendCommand(commande_t command, unsigned char *argTab, int argLength);
inline void sendCommandByte(commande_t command);

void connectAsser();
T_Pos getPos();
T_Pos updateAndWaitPos();
void calleStart();

void UART_Asser_RX_Handler(unsigned char inByte);

boolean isAsserDataOK();			// Passe à FALSE lorsqu'il retourne TRUE
									// /!\ NE DEMANDER QU'UNE DONNÉE À LA FOIS !
void waitAsserData();				// Passe à FALSE
boolean hasDone();					// Décrémente le nombre de done en attente lorsqu'il retourne TRUE
void waitDone(unsigned int nbDone);	// Décrémente le nombre de done en attente

#endif
