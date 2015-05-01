#include "Boolean.h"
#ifdef COLLECTEUR
	#include "../../Collecteur/src/PSoCGlue.h"
#elif defined(SIMU)	// #ifndef COLLECTEUR
	#include "../../SimuAsserPSoC/src/PSoCGlue.h"
#else
	#include <device.h>
#endif	// #ifndef COLLECTEUR
#include "asser.h"
#include "init.h"

unsigned char dataAsser[9];
volatile boolean isAsserDataOKVar = FALSE;
static volatile boolean isAsserReady = FALSE;	// Modif uniquement par UART_Asser_RX_isr
volatile int nbAsserDone = 0;
boolean isRxAsserBypassed = FALSE;	// Connection directe de l'asser à l'XBee
boolean isTxAsserSniffed = FALSE;

/** Fonctions (mm, °) **/

static void UART_Asser_PutChar_Sniffed(unsigned char byte) {
	if (isTxAsserSniffed) UART_XBee_PutChar(byte);
	UART_Asser_PutChar(byte);
}

void sendCommand(commande_t command, unsigned char *argTab, int argLength) {
	UART_Asser_PutChar_Sniffed(command);
	int i;
	for (i=0; i<argLength; i++)
		UART_Asser_PutChar_Sniffed(argTab[i]);
}

inline void sendCommandByte(commande_t command) {
	sendCommand(command, NULL, 0);
}

void connectAsser() {
	while (!isAsserReady) {
		UART_Asser_PutChar_Sniffed(CONNECT);
		CyDelay(10);
	}
	UART_XBee_PutChar('R');
}

void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision) {
    unsigned short xAsser = (x*0xFFFF)/3000;
    if(!isRed)
        xAsser = 0xFFFF - xAsser;
    unsigned short yAsser = ((2000-y)*0xFFFF)/3000;
	UART_Asser_PutChar_Sniffed(XY_RAPIDE);
	UART_Asser_PutChar_Sniffed(xAsser >> 8);
	UART_Asser_PutChar_Sniffed(xAsser);
	UART_Asser_PutChar_Sniffed(yAsser >> 8);
	UART_Asser_PutChar_Sniffed(yAsser);
	unsigned long p = (unsigned long)0xFFFF*precision/3000;
	p *= p;
	UART_Asser_PutChar_Sniffed(p >> 24);
	UART_Asser_PutChar_Sniffed(p >> 16);
	UART_Asser_PutChar_Sniffed(p >> 8);
	UART_Asser_PutChar_Sniffed(p);
}

void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision) {
    unsigned short xAsser = (x*0xFFFF)/3000;
    if(!isRed)
        xAsser = 0xFFFF - xAsser;
    unsigned short yAsser = ((2000-y)*0xFFFF)/3000;
	UART_Asser_PutChar_Sniffed(XY_COURBE);
	UART_Asser_PutChar_Sniffed(xAsser >> 8);
	UART_Asser_PutChar_Sniffed(xAsser);
	UART_Asser_PutChar_Sniffed(yAsser >> 8);
	UART_Asser_PutChar_Sniffed(yAsser);
	unsigned long p = (unsigned long)0xFFFF*precision/3000;
	p *= p;
	UART_Asser_PutChar_Sniffed(p >> 24);
	UART_Asser_PutChar_Sniffed(p >> 16);
	UART_Asser_PutChar_Sniffed(p >> 8);
	UART_Asser_PutChar_Sniffed(p);
}

void sendALPHA(unsigned short a, unsigned short precision) {
	a = (unsigned long) a * TOUR / 360;
    if(isRed)
		a = TOUR - a;
	else
    	a = ((unsigned long) a + (TOUR >> 1)) % TOUR;
	precision = (unsigned long) precision * TOUR / 360;
	UART_Asser_PutChar_Sniffed(ALPHA);
	UART_Asser_PutChar_Sniffed(a >> 8);
	UART_Asser_PutChar_Sniffed(a);
	UART_Asser_PutChar_Sniffed(precision >> 8);
	UART_Asser_PutChar_Sniffed(precision);
}

void sendSUPPR(unsigned char nbCommande) {	// Supprime les N dernières commandes
	UART_Asser_PutChar_Sniffed(SUPPR);
	UART_Asser_PutChar_Sniffed(nbCommande);
}

void sendFIN_MATCH() {
	UART_Asser_PutChar_Sniffed(FIN_MATCH);
}

void sendRED() {
	UART_Asser_PutChar_Sniffed(RED);
}

void sendPURPLE() {
	UART_Asser_PutChar_Sniffed(PURPLE);
}

void sendGET_POS() {
	UART_Asser_PutChar_Sniffed(GET_POS);
}

T_Pos getPos() {
	T_Pos *pos = (T_Pos *) &dataAsser;
	pos->x = (unsigned long) pos->x*3000/0xFFFF;
	pos->y = 2000 - (unsigned short) ((unsigned long) pos->y*3000/0xFFFF);
	pos->angle = (unsigned long) pos->angle*360/TOUR;
    if(isRed) {
    	pos->angle = 360 - pos->angle;
	} else {
        pos->x = 3000 - pos->x;
    	pos->angle = (180 + pos->angle) % 360;
	}
	return *pos;
}

T_Pos updateAndWaitPos() {
	sendGET_POS();
	waitAsserData();
	return getPos();
}

void calleStart() {
	UART_Asser_PutChar_Sniffed(CALLE_H);
	sendXY_RAPIDE(1500, 1800, 100);
	sendALPHA(180, PRECISION_ALPHA);
	waitDone(1);
	if (isRed)
		UART_Asser_PutChar_Sniffed(CALLE_D);
	else
		UART_Asser_PutChar_Sniffed(CALLE_G);
	waitDone(1);
	sendXY_RAPIDE(200, 1800, 100);
	waitDone(1);
	
	// Avance à l'avant de la zone de départ
	sendXY_RAPIDE(300, 1750, PRECISION_XY);
	waitDone(1);
	sendALPHA(180, PRECISION_ALPHA);
	waitDone(3);
}

void UART_Asser_RX_Handler(unsigned char inByte) {
	static int nbOctRestant = 0;
	static unsigned char commande;
	
	if (!nbOctRestant) {
		commande = inByte;
		nbOctRestant = nbArgAsserToPSoC[commande];
		switch(commande) {
		case CONNECT:
			UART_Asser_PutChar_Sniffed(ACK_CONNECT);
			break;
		case ACK_CONNECT:
			isAsserReady = TRUE;
			break;
		case DONE:
			nbAsserDone ++;
			break;
		case TEST_BRANCHEMENT:
			UART_XBee_PutChar('b');
			break;
		case START:
			if (isAsserReady) {
				UART_XBee_PutString("Reset asser");
				while (1);
			}
		}
	} else {
		nbOctRestant --;
		dataAsser[nbOctRestant] = inByte;
		if (commande == GET_COMMANDE && nbOctRestant == 8) {
			if (dataAsser[8] == ALPHA) nbOctRestant = 4;
			else if (dataAsser[8] != XY_RAPIDE && dataAsser[8] != XY_COURBE) nbOctRestant = 0;
		}
		if (!nbOctRestant)
			isAsserDataOKVar = TRUE;
	}
}

boolean isAsserDataOK() {
	if (!isAsserDataOKVar) return FALSE;
	isAsserDataOKVar = FALSE;
	return TRUE;
}

void waitAsserData() {
	while (!isAsserDataOKVar);
	isAsserDataOKVar = FALSE;
}

boolean hasDone() {
	if (isRxAsserBypassed || !nbAsserDone) return FALSE;
	UART_Asser_RX_isr_Disable();
	nbAsserDone --;
	UART_Asser_RX_isr_Enable();
	return TRUE;
}

void waitDone(unsigned int nbDone) {
	if (!isRxAsserBypassed) {
		while(nbAsserDone < nbDone) CyDelay(10);
		UART_Asser_RX_isr_Disable();
		nbAsserDone -= nbDone;
		UART_Asser_RX_isr_Enable();
	}
}
