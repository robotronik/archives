#include "UART_Asser.h"
#include "main.h"
#include "message.h"

T_Pos_TabChar posAsser = {{0}};
boolean isAsserReady = FALSE;
boolean hasAsserDone = FALSE;
boolean isPosAsserOK = TRUE;

/** Constantes : Commandes **/

// 							0 <= ALPHA < 0x40
#define XY_RAPIDE			0x40
#define XY_COURBE			0x41
#define SUPPR				0x80	// Supprime les N dernières commandes
#define S_90				0x81
#define BLUE				0x83
#define RED					0x84
//#define TEST				0x85	// Connexion bonne
#define GET_POS				0x86

/** Fonctions **/

void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision) {
	UART_Asser_PutChar(XY_RAPIDE);
	UART_Asser_PutChar(x >> 8);
	UART_Asser_PutChar(x);
	UART_Asser_PutChar(y >> 8);
	UART_Asser_PutChar(y);
	unsigned long p = (unsigned long)precision * precision;
	UART_Asser_PutChar(p >> 24);
	UART_Asser_PutChar(p >> 16);
	UART_Asser_PutChar(p >> 8);
	UART_Asser_PutChar(p);
}

void sendXY_COURBE(unsigned int x, unsigned int y, unsigned int precision) {
	UART_Asser_PutChar(XY_COURBE);
	UART_Asser_PutChar(x >> 8);
	UART_Asser_PutChar(x);
	UART_Asser_PutChar(y >> 8);
	UART_Asser_PutChar(y);
	unsigned long p = (unsigned long)precision * precision;
	UART_Asser_PutChar(p >> 24);
	UART_Asser_PutChar(p >> 16);
	UART_Asser_PutChar(p >> 8);
	UART_Asser_PutChar(p);
}

void sendALPHA(unsigned int a, unsigned char precision) {
	UART_Asser_PutChar(a >> 8);
	UART_Asser_PutChar(a);
	UART_Asser_PutChar(precision);
}

void sendSUPPR(unsigned char nbCommande) {	// Supprime les N dernières commandes
	UART_Asser_PutChar(SUPPR);
	UART_Asser_PutChar(nbCommande);
}

void sendS_90() {
	UART_Asser_PutChar(S_90);
}

void sendBLUE() {
	UART_Asser_PutChar(BLUE);
}

void sendRED() {
	UART_Asser_PutChar(RED);
}

void sendTEST() {
	UART_Asser_PutChar(TEST);
}

void sendGET_POS() {
	UART_Asser_PutChar(GET_POS);
	isPosAsserOK=FALSE;
}
