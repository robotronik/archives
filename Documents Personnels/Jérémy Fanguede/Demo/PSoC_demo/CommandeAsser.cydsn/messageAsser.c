#include "main.h"
#include "messageAsser.h"

/*typedef union {
	unsigned char tab[6];
	T_Pos champs;
} T_Pos_TabChar;*/

unsigned char dataAsser[9];
volatile boolean isAsserDataOK = FALSE;
volatile boolean isAsserReady = FALSE;
volatile int nbAsserDone = 0;

extern boolean isPurple;

#define TOUR	(int)22538 		// Voir "asser.h"

/** Constantes : Commandes **/

// 								0 <= ALPHA < MAX_ALPHA
#define MAX_ALPHA				0x80	// A copier dans test_asser.c
#define XY_RAPIDE				0x80	// A copier dans test_asser.c
#define XY_COURBE				0x81	//				"
#define SUPPR					0x82	// Supprime les N dernières commandes
#define FIN_MATCH				0x83
#define RED						0x84
#define PURPLE					0x85
#define TEST					0x86	// Connexion bonne
#define POS						0x87
#define CALLE_G					0x88
#define CALLE_H					0x89
#define CALLE_D					0x8A
#define GET_COEF_FREIN			0x8E
#define GET_COEF_FREIN_ALPHA	0x8F
#define GET_COEF_FREIN_ALPHA2	0x90
#define GET_COMMANDE			0x92
#define TEST_BRANCHEMENT		0x93
#define ACK						0x94
#define SET_MIN_SPEED			0x95
#define GET_MIN_SPEED			0x96

/** Constantes : Reponses **/

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

/** Fonctions (mm, °) **/

void UART_Asser_PutChar_Sniffed(uint8_t byte) {
#if IS_SNIFFING
	UART_XBee_PutChar(byte);
#endif // IS_SNIFFING
	UART_Asser_PutChar(byte);
}

void sendXY_RAPIDE(unsigned int x, unsigned int y, unsigned int precision) {
    unsigned short xAsser = (x*0xFFFF)/3000;
    if(isPurple)
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
    if(isPurple)
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
    if(isPurple)
    	a = ((unsigned long) a + (TOUR >> 1)) % TOUR;
	else
		a = TOUR - a;
	precision = (unsigned long) precision * TOUR / 360;
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

void sendTEST() {
	UART_Asser_PutChar_Sniffed(TEST);
}

void updatePos() {
	UART_Asser_PutChar_Sniffed(POS);
}

T_Pos getPos() {
	T_Pos *pos = (T_Pos *) &dataAsser;
	pos->x = (unsigned long) pos->x*3000/0xFFFF;
	pos->y = 2000 - (unsigned short) ((unsigned long) pos->y*3000/0xFFFF);
	pos->angle = (unsigned long) pos->angle*360/TOUR;
    if(isPurple) {
        pos->x = 3000 - pos->x;
    	pos->angle = (180 + pos->angle) % 360;
	} else {
    	pos->angle = 360 - pos->angle;
	}
	return *pos;
}

T_Pos getWaitPos() {
	updatePos();
	while(!isAsserDataOK); isAsserDataOK = FALSE;
	return getPos();
}

void calleStart() {
	if (isPurple) 
		UART_Asser_PutChar_Sniffed(CALLE_G);
	else
		UART_Asser_PutChar_Sniffed(CALLE_D);
	sendXY_RAPIDE(100, 100, 5);
	sendALPHA(90, 2);
	// Attendre des DONE
	UART_Asser_PutChar_Sniffed(CALLE_H);
	sendXY_RAPIDE(100, 100, 5);
	sendALPHA(180, 2);
}

void UART_Asser_RX_Handler(uint8_t inByte) {
	static int nbOctRestant = 0;
	static uint8_t commande;
	
	if (nbOctRestant) {
		nbOctRestant --;
		dataAsser[nbOctRestant] = inByte;
		if (commande == GET_COMMANDE && nbOctRestant == 8) {
			if (dataAsser[8] < MAX_ALPHA) nbOctRestant = 3;
			else if (dataAsser[8] != XY_RAPIDE && dataAsser[8] != XY_COURBE) nbOctRestant = 0;
		}
		if (!nbOctRestant)
			isAsserDataOK = TRUE;
	} else {
		commande = inByte;
		switch(commande) {
		case TEST:
			if (isAsserReady) {
				UART_XBee_PutChar('R');
				UART_Asser_PutChar_Sniffed(ACK);
				while (1);
				break;
			}
			isAsserReady = TRUE;
			UART_Asser_PutChar_Sniffed(ACK);
			break;
		case ACK:
			isAsserReady = TRUE;
			break;
		case POS:
			nbOctRestant = 6;
			break;
		case DONE:
			nbAsserDone ++;
			break;
		case TEST_BRANCHEMENT:
			UART_XBee_PutChar('b');
			break;
		case GET_COEF_FREIN:
		case GET_COEF_FREIN_ALPHA:
		case GET_COEF_FREIN_ALPHA2:
		case GET_MIN_SPEED:
			nbOctRestant = 2;
			break;
		case GET_COMMANDE:
			nbOctRestant = 9;
		}
	}
}