/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "messageAsser.h"
#include "servomoteur.h"

//// demo /////
static unsigned char svg = 100;
static unsigned char svd = 100;

extern unsigned char dataAsser[9];

extern void UART_Asser_PutChar_Sniffed(uint8_t byte);

// Importés de messageAsser.c :
#define MAX_ALPHA				0x80
#define XY_RAPIDE				0x80
#define XY_COURBE				0x81

// Importés de message.h du projet de la carte asser :
#define CALLE_G2				0x82
#define CALLE_H2				0x83
#define CALLE_D2				0x84
#define FREINAGE				0x85
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
#define SET_MIN_SPEED			0x95
#define GET_MIN_SPEED			0x96

static void sendStringOf(unsigned int val) {
	if (!val) UART_XBee_PutChar('0');
	else {
		int i;
		for (i=1; i<=val; i*=10);
		for (i/=10; i>0; i/=10) {
			UART_XBee_PutChar('0' + val/i%10);
		}
	}
}

static void sendPos() {
	T_Pos pos = getWaitPos();
	sendStringOf(pos.x);
	UART_XBee_PutChar('_');
	sendStringOf(pos.y);
	UART_XBee_PutChar('_');
	sendStringOf(pos.angle);
}

static void sendSET_COEF_FREIN(uint16_t val) {
	UART_Asser_PutChar_Sniffed(SET_COEF_FREIN);
	UART_Asser_PutChar_Sniffed(val >> 8);
	UART_Asser_PutChar_Sniffed(val);
}

static void sendSET_COEF_FREIN_ALPHA(uint16_t val) {
	UART_Asser_PutChar_Sniffed(SET_COEF_FREIN_ALPHA);
	UART_Asser_PutChar_Sniffed(val >> 8);
	UART_Asser_PutChar_Sniffed(val);
}

static void sendSET_COEF_FREIN_ALPHA2(uint16_t val) {
	UART_Asser_PutChar_Sniffed(SET_COEF_FREIN_ALPHA2);
	UART_Asser_PutChar_Sniffed(val >> 8);
	UART_Asser_PutChar_Sniffed(val);
}

static void sendSET_MIN_SPEED(uint16_t val) {
	UART_Asser_PutChar_Sniffed(SET_MIN_SPEED);
	UART_Asser_PutChar_Sniffed(val >> 8);
	UART_Asser_PutChar_Sniffed(val);
}

static void sendGET_COEF_FREIN() {
	UART_Asser_PutChar_Sniffed(GET_COEF_FREIN);
	while(!isAsserDataOK); isAsserDataOK = FALSE;
	sendStringOf(((uint16_t)dataAsser[1] << 8) | dataAsser[0]);
}

static void sendGET_COEF_FREIN_ALPHA() {
	UART_Asser_PutChar_Sniffed(GET_COEF_FREIN_ALPHA);
	while(!isAsserDataOK); isAsserDataOK = FALSE;
	sendStringOf(((uint16_t)dataAsser[1] << 8) | dataAsser[0]);
}

static void sendGET_COEF_FREIN_ALPHA2() {
	UART_Asser_PutChar_Sniffed(GET_COEF_FREIN_ALPHA2);
	while(!isAsserDataOK); isAsserDataOK = FALSE;
	sendStringOf(((uint16_t)dataAsser[1] << 8) | dataAsser[0]);
}

static void sendGET_MIN_SPEED() {
	UART_Asser_PutChar_Sniffed(GET_MIN_SPEED);
	while(!isAsserDataOK); isAsserDataOK = FALSE;
	sendStringOf(((uint16_t)dataAsser[1] << 8) | dataAsser[0]);
}

static void sendGET_COMMANDE() { // Données non converties en [mm ; °]
	UART_Asser_PutChar_Sniffed(GET_COMMANDE);
	while(!isAsserDataOK); isAsserDataOK = FALSE;
	switch (dataAsser[8]) {
	case XY_RAPIDE:
		UART_XBee_PutChar('r');
		break;
	case XY_COURBE:
		UART_XBee_PutChar('c');
		break;
	case CALLE_G:
	case CALLE_G2:
		UART_XBee_PutChar('c');
		UART_XBee_PutChar('g');
		break;
	case CALLE_H:
	case CALLE_H2:
		UART_XBee_PutChar('c');
		UART_XBee_PutChar('h');
		break;
	case CALLE_D:
	case CALLE_D2:
		UART_XBee_PutChar('c');
		UART_XBee_PutChar('d');
		break;
	case FREINAGE:
		UART_XBee_PutChar('f');
		break;
	default:
		UART_XBee_PutChar('a');
	}
	if (dataAsser[8] < MAX_ALPHA) {
		UART_XBee_PutChar('_');
		sendStringOf(((uint16_t)dataAsser[8] << 8) | dataAsser[2]);
		UART_XBee_PutChar('_');
		sendStringOf(((uint16_t)dataAsser[1] << 8) | dataAsser[0]);
	} else if (dataAsser[8] == XY_RAPIDE || dataAsser[8] == XY_COURBE) {
		UART_XBee_PutChar('_');
		sendStringOf(((uint16_t)dataAsser[7] << 8) | dataAsser[6]);
		UART_XBee_PutChar('_');
		sendStringOf(((uint16_t)dataAsser[5] << 8) | dataAsser[4]);
		UART_XBee_PutChar('_');
		sendStringOf(((uint32_t)dataAsser[3] << 24) | (dataAsser[2] << 16) | (dataAsser[1] << 8) | dataAsser[0]);
	}
}

void test_message_processing(void)
{
	unsigned char inMessage = UART_XBee_ReadRxData();
	static uint8_t nbArgTotal = 0;
	static uint8_t nbArgRecu = 0;
	static uint8_t commande[6];
	
	if (nbArgRecu < nbArgTotal) {
		nbArgRecu ++;
		commande[nbArgRecu] = inMessage;
		if (nbArgRecu == nbArgTotal) {
			T_Pos pos;
			switch (commande[0]) {
				case 'r':
					sendXY_RAPIDE(
						((uint16_t)commande[1] << 8) | commande[2], 
						((uint16_t)commande[3] << 8) | commande[4], 
						commande[5]);
					break;
				case 'R':
					pos = getWaitPos();
					pos.x += ((uint16_t)commande[1] << 8) | commande[2];
					pos.y += ((uint16_t)commande[3] << 8) | commande[4];
					sendXY_RAPIDE(pos.x, pos.y, commande[5]);
					break;
				case 'c':
					sendXY_COURBE(
						((uint16_t)commande[1] << 8) | commande[2], 
						((uint16_t)commande[3] << 8) | commande[4], 
						commande[5]);
					break;
				case 'C':
					pos = getWaitPos();
					pos.x += ((uint16_t)commande[1] << 8) | commande[2];
					pos.y += ((uint16_t)commande[3] << 8) | commande[4];
					sendXY_COURBE(pos.x, pos.y, commande[5]);
					break;
				case 'a':
					sendALPHA(
						((uint16_t)commande[1] << 8) | commande[2], 
						((uint16_t)commande[3] << 8) | commande[4]);
					break;
				case 'A':
					pos = getWaitPos();
					pos.angle += 360 + (((int16_t)commande[1] << 8) | commande[2]);
					pos.angle %= 360;
					sendALPHA(pos.angle, ((int16_t)commande[3] << 8) | commande[4]);
					break;
				case 'f':
					switch (commande[1]) {
						case '1':
							sendGET_COEF_FREIN();
							break;
						case '2':
							sendGET_COEF_FREIN_ALPHA();
							break;
						case '3':
							sendGET_COEF_FREIN_ALPHA2();
					}
					break;
				case 'F':
					switch (commande[1]) {
						case '1':
							sendSET_COEF_FREIN(((uint16_t)commande[2] << 8) | commande[3]);
							break;
						case '2':
							sendSET_COEF_FREIN_ALPHA(((uint16_t)commande[2] << 8) | commande[3]);
							break;
						case '3':
							sendSET_COEF_FREIN_ALPHA2(((uint16_t)commande[2] << 8) | commande[3]);
					}
					break;
				case 'M':
					sendSET_MIN_SPEED(((uint16_t)commande[1] << 8) | commande[2]);
			}
		}
	} else {
		switch(inMessage)
		{
			case 's':
				sendSUPPR(255);
				break;
			case 'm':
				sendGET_MIN_SPEED();
				break;
			case 'f':
				commande[0] = inMessage;
				nbArgTotal = 1;
				nbArgRecu = 0;
				break;
			case 'M':
				commande[0] = inMessage;
				nbArgTotal = 2;
				nbArgRecu = 0;
				break;
			case 'F':
				commande[0] = inMessage;
				nbArgTotal = 3;
				nbArgRecu = 0;
				break;
			case 'a':
			case 'A':
				commande[0] = inMessage;
				nbArgTotal = 4;
				nbArgRecu = 0;
				break;
			case 'r':
			case 'c':
			case 'R':
			case 'C':
				commande[0] = inMessage;
				nbArgTotal = 5;
				nbArgRecu = 0;
				break;
			case 'p':
				sendPos();
				break;
			case 'P':
				UART_Asser_PutChar_Sniffed(PAUSE);
				break;
			case 'd':
				sendGET_COMMANDE();
				break;
			case 'b':
				UART_Asser_PutChar_Sniffed(TEST_BRANCHEMENT);
			case 'z': //AVANCER
				UART_Asser_PutChar('a');
				break;
			case 'q': //GAUCHE
				UART_Asser_PutChar('g');
				break;
			case 'i': //DROITE
				UART_Asser_PutChar('d');
				break;
			case 'j': //RECULER
				UART_Asser_PutChar('r');
				break;
			case 'k': //STOP
				UART_Asser_PutChar('s');
				break;
			case 'u':
				svg += 2;
				setServoGauche(svg);//480
				break;
			case 'v':
				svg -= 2;
				setServoGauche(svg);//40
				break;
			case 'w':
				svd -= 2;
				setServoDroit(svd);//40
				break;
			case 'x':
				svd += 2;
				setServoDroit(svd);//180
				break;
			case 'H':
				UART_Asser_PutChar('s');
				UART_Asser_PutChar('a');
				CyDelay(600);
				UART_Asser_PutChar('s');
				CyDelay(300);
				UART_Asser_PutChar('d');
				CyDelay(1000);
				UART_Asser_PutChar('s');
				setServoDroit(170);
				//setServoGauche(170);
				CyDelay(500);
				setServoDroit(100);
				setServoGauche(100);
				UART_Asser_PutChar('g');
				CyDelay(1100);
				setServoDroit(100);
				CyDelay(300);
				setServoDroit(40);
				CyDelay(500);
				setServoDroit(100);
		}
	}
}

/* [] END OF FILE */
