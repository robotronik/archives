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

#include "..\..\sources\messageCommun.h"
#include "Boolean.h"
#ifdef COLLECTEUR
	#include "../../Collecteur/src/PSoCGlue.h"
#elif defined(SIMU)	// #ifndef COLLECTEUR
	#include "../../SimuAsserPSoC/src/PSoCGlue.h"
#else
	#include <device.h>
#endif	// #ifndef COLLECTEUR
#include "main.h"
#include "init.h"
#include "asser.h"
#include "DMA.h"

static commande_t lastRequest;
static unsigned short lastXRequest, lastYRequest;
static boolean areLastXYRequestValid = FALSE;
static unsigned short lastAlphaRequest;
static boolean isLastAlphaRequestValid = FALSE;

static void sendStringOf(unsigned char *tab, int length) {
	unsigned long val = 0;
	int i;
	for (i=length; i>0;) {
		i --;
		val <<= 8;
		val |= tab[i];
	}
	if (!val) UART_XBee_PutChar('0');
	else {
		for (i=1; i<=val; i*=10);
		for (i/=10; i>0; i/=10) {
			UART_XBee_PutChar('0' + val/i%10);
		}
	}
}

static void displayPos() {
	T_Pos pos = getPos();
	sendStringOf((unsigned char *) &(pos.x), 2);
	UART_XBee_PutChar(' ');
	sendStringOf((unsigned char *) &(pos.y), 2);
	UART_XBee_PutChar(' ');
	sendStringOf((unsigned char *) &(pos.angle), 2);
}

static inline void getVal(commande_t command) {
	if (!isRxAsserBypassed) {
		sendCommandByte(command);
		lastRequest = command;
	}
}

static inline void dispayVal(commande_t command) {
	sendStringOf(dataAsser, nbArgAsserToPSoC[command]);
}

static void displayCurrentCmd() { // Données non converties en [mm ; °]
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
	case ALPHA:
		UART_XBee_PutChar('a');
	}
	if (dataAsser[8] == ALPHA) {
		UART_XBee_PutChar(' ');
		sendStringOf(dataAsser + 2, 2);
		UART_XBee_PutChar(' ');
		sendStringOf(dataAsser, 2);
	} else if (dataAsser[8] == XY_RAPIDE || dataAsser[8] == XY_COURBE) {
		UART_XBee_PutChar(' ');
		sendStringOf(dataAsser + 6, 2);
		UART_XBee_PutChar(' ');
		sendStringOf(dataAsser + 4, 2);
		UART_XBee_PutChar(' ');
		sendStringOf(dataAsser, 4);
	}
}

static void addressUART(char source, char config) {
	if (source == 'A') {
		switch (config) {
		case '4':
			isTxAsserSniffed = !isTxAsserSniffed;
			break;
		case '3':
			isRxAsserBypassed = FALSE;
			DMA_Enable(DMA_ASSER_TO_XBEE);
			if (!UART_Asser_RX_isr_GetState()) {
				UART_Asser_RX_isr_ClearPending();
				UART_Asser_RX_isr_Enable();
			}
			break;
		case '2':
			isRxAsserBypassed = TRUE;
			DMA_Enable(DMA_ASSER_TO_XBEE);
			UART_Asser_RX_isr_Disable();
			break;
		default:
			isRxAsserBypassed = FALSE;
			if (!UART_Asser_RX_isr_GetState()) {
				UART_Asser_RX_isr_ClearPending();
				UART_Asser_RX_isr_Enable();
			}
			DMA_Disable(DMA_ASSER_TO_XBEE);
		}
	} else {
		switch (config) {
		case '2':
			DMA_Enable(DMA_XBEE_TO_ASSER);
			UART_XBee_RX_isr_Disable();
			break;
		default:
			if (!UART_XBee_RX_isr_GetState()) {
				UART_XBee_RX_isr_ClearPending();
				UART_XBee_RX_isr_Enable();
			}
			DMA_Disable(DMA_XBEE_TO_ASSER);
		}
	}
}

void test(void) {
	if (isRed != Couleur_Read()) {
		isRed = Couleur_Read();
		if (isRed)	UART_XBee_PutString("Rouge ");
		else		UART_XBee_PutString("Violet ");
#if SEND_COLOR
		if (isRed)	sendCommandByte(RED);
		else		sendCommandByte(PURPLE);
#endif // #if SEND_COLOR
	}

	while (hasDone()) {
		// Fait suivre les fins de déplacements au PC
		if (!isRxAsserBypassed) UART_XBee_PutChar('D');
	}
	if (isAsserDataOK()) {
		switch (lastRequest) {
			case GET_POS:
				UART_XBee_PutChar('p');
				displayPos();
				break;
			case GET_COMMANDE:
				displayCurrentCmd();
				break;
			default:
				dispayVal(lastRequest);
		}
	}
}

void boucle_test(void) {
	UART_XBee_PutString(" Test ");
	while(1) test();
}

void test_message_processing(void) {
	unsigned char inMessage = UART_XBee_ReadRxData();
	static unsigned char nbArgTotal = 0;
	static unsigned char nbArgRecu = 0;
	static unsigned char commande[6];
	static boolean isAcquiringAsserInputs = FALSE;
	
	if (nbArgRecu >= nbArgTotal) {	// Premier octet de la commande
		switch(inMessage)
		{
			case 's':
				{
					unsigned char arg = 255;
					sendCommand(SUPPR, &arg, 1);
				}
				areLastXYRequestValid = FALSE;
				isLastAlphaRequestValid = FALSE;
				break;
			case 'm':
				getVal(GET_MIN_SPEED);
				break;
			case 'g':
				getVal(GET_DEBUG_SETTING);
				break;
			case 'e':
			case 'f':
			case 'F':
				commande[0] = inMessage;
				nbArgTotal = 1;
				nbArgRecu = 0;
				break;
			case 'M':
				commande[0] = inMessage;
				nbArgTotal = 2;
				nbArgRecu = 0;
				break;
			case 'a':
			case 'A':
			case 'G':
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
				getVal(GET_POS);
				break;
			case 'P':
				sendCommandByte(PAUSE);
				break;
			case 'd':
				getVal(GET_COMMANDE);
				break;
			case 't':
				areLastXYRequestValid = FALSE;
				isLastAlphaRequestValid = FALSE;
				sendCommandByte(TEST_BRANCHEMENT);
				break;
			case 'T':
				commande[0] = inMessage;
				nbArgTotal = 1;
				nbArgRecu = 0;
		}
		if (nbArgRecu >= nbArgTotal && !isRxAsserBypassed) UART_XBee_PutChar('v');
	} else {
		nbArgRecu ++;
		commande[nbArgRecu] = inMessage;
		if (nbArgRecu == nbArgTotal) {
			T_Pos pos;
			switch (commande[0]) {
				case 'r':
					lastXRequest = ((unsigned short)commande[1] << 8) | commande[2];
					lastYRequest = ((unsigned short)commande[3] << 8) | commande[4];
					areLastXYRequestValid = TRUE;
					isLastAlphaRequestValid = FALSE;
					sendXY_RAPIDE(lastXRequest, lastYRequest, commande[5]);
					break;
				case 'R':
					isLastAlphaRequestValid = FALSE;
					if (areLastXYRequestValid) {
						lastXRequest += ((unsigned short)commande[1] << 8) | commande[2];
						lastYRequest += ((unsigned short)commande[3] << 8) | commande[4];
						sendXY_RAPIDE(lastXRequest, lastYRequest, commande[5]);
					} else if (!isRxAsserBypassed) {
						pos = updateAndWaitPos();
						pos.x += ((unsigned short)commande[1] << 8) | commande[2];
						pos.y += ((unsigned short)commande[3] << 8) | commande[4];
						sendXY_RAPIDE(pos.x, pos.y, commande[5]);
					}
					break;
				case 'c':
					areLastXYRequestValid = TRUE;
					isLastAlphaRequestValid = FALSE;
					lastXRequest = ((unsigned short)commande[1] << 8) | commande[2];
					lastYRequest = ((unsigned short)commande[3] << 8) | commande[4];
					sendXY_COURBE(lastXRequest, lastYRequest, commande[5]);
					break;
				case 'C':
					isLastAlphaRequestValid = FALSE;
					if (areLastXYRequestValid) {
						lastXRequest += ((unsigned short)commande[1] << 8) | commande[2];
						lastYRequest += ((unsigned short)commande[3] << 8) | commande[4];
						sendXY_COURBE(lastXRequest, lastYRequest, commande[5]);
					} else if (!isRxAsserBypassed) {
						pos = updateAndWaitPos();
						pos.x += ((unsigned short)commande[1] << 8) | commande[2];
						pos.y += ((unsigned short)commande[3] << 8) | commande[4];
						sendXY_COURBE(pos.x, pos.y, commande[5]);
					}
					break;
				case 'a':
					lastAlphaRequest = ((unsigned short)commande[1] << 8) | commande[2];
					isLastAlphaRequestValid = TRUE;
					sendALPHA(lastAlphaRequest, ((unsigned short)commande[3] << 8) | commande[4]);
					break;
				case 'A':
					if (isLastAlphaRequestValid) {
						lastAlphaRequest += 360 + (((unsigned short)commande[1] << 8) | commande[2]);
						lastAlphaRequest %= 360;
						sendXY_COURBE(lastXRequest, lastYRequest, commande[5]);
					} else if (!isRxAsserBypassed) {
						pos = updateAndWaitPos();
						pos.angle += 360 + (((unsigned short)commande[1] << 8) | commande[2]);
						pos.angle %= 360;
						sendALPHA(pos.angle, ((unsigned short)commande[3] << 8) | commande[4]);
					}
					break;
				case 'e':
					areLastXYRequestValid = FALSE;
					isLastAlphaRequestValid = FALSE;
					switch (commande[1]) {
						case 'g':
							sendCommandByte(CALLE_G);
							break;
						case 'd':
							sendCommandByte(CALLE_D);
							break;
						case 'h':
							sendCommandByte(CALLE_H);
					}
					break;
				case 'f':
					switch (commande[1]) {
						case '1':
							getVal(GET_COEF_FREIN);
							break;
						case '2':
							getVal(GET_COEF_FREIN_ALPHA);
							break;
						case '3':
							getVal(GET_ACCEL_MAX);
					}
					break;
				case 'F':
					if (nbArgTotal == 1)
						if (commande[1] == '4')	nbArgTotal ++;
						else					nbArgTotal += 2;
					else
						switch (commande[1]) {
							case '1':
								sendCommand(SET_COEF_FREIN, commande + 2, 2);
								break;
							case '2':
								sendCommand(SET_COEF_FREIN_ALPHA, commande + 2, 2);
								break;
							case '3':
								sendCommand(SET_ACCEL_MAX, commande + 2, 1);
						}
					break;
				case 'G':
					sendCommand(SET_DEBUG_SETTING, commande + 1, 4);
					break;
				case 'T':
					if (commande[1] == 'C') {
						isAcquiringAsserInputs = !isAcquiringAsserInputs;
						if (isAcquiringAsserInputs) addressUART('A', '2');
						else addressUART('A', '1');
						sendCommandByte(GET_QEI);
					} else if (nbArgTotal == 1) nbArgTotal ++;
					else addressUART(commande[1], commande[2]);
			}
			if (nbArgRecu >= nbArgTotal && !isRxAsserBypassed) UART_XBee_PutChar('v');
		}
	}
}

/* [] END OF FILE */
