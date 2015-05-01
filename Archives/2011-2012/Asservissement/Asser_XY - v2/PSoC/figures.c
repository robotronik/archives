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

#include "main.h"
#include "asser.h"
#include "servomoteur.h"

//#define WAIT_BETWEEN_FIGURES
#ifdef WAIT_BETWEEN_FIGURES
	#define	WAIT_TIRETTE	while (Depart_Read()); while (!Depart_Read())
#else	// #ifdef WAIT_BETWEEN_FIGURES
	#define	WAIT_TIRETTE	while (0)
#endif	// #ifdef WAIT_BETWEEN_FIGURES

void testXav() {
	sendXY_RAPIDE(536, 1750, 40);
	sendXY_RAPIDE(536, 1250, PRECISION_XY);
	waitDone(2);
	
	setServoGauche(140);
	setServoDroit(140);
	sendALPHA(0, PRECISION_ALPHA);
	waitDone(1);
	
	setServoGauche(160);
	setServoDroit(160);
	while (1) {
		sendXY_COURBE(36, 1750, PRECISION_XY);
		sendXY_RAPIDE(536, 1750, 40);
		sendXY_RAPIDE(536, 1250, PRECISION_XY);
		waitDone(3);
		
		setServoGauche(140);
		setServoDroit(140);
		sendALPHA(0, PRECISION_ALPHA);
		waitDone(1);
		
		setServoGauche(160);
		setServoDroit(160);
    }
}

void carres() {
	sendXY_RAPIDE(1000, 1500, PRECISION_XY);
	sendALPHA(180, PRECISION_ALPHA);
	while (1) {
		// Avance
		sendXY_RAPIDE(2000, 1500, PRECISION_XY);	waitDone(1);
		sendALPHA(90, PRECISION_ALPHA);				waitDone(1);
		sendXY_RAPIDE(2000, 500, PRECISION_XY);		waitDone(1);
		sendALPHA(0, PRECISION_ALPHA);				waitDone(1);
		sendXY_RAPIDE(1000, 500, PRECISION_XY);		waitDone(1);
		sendALPHA(270, PRECISION_ALPHA);			waitDone(1);
		sendXY_RAPIDE(1000, 1500, PRECISION_XY);	waitDone(1);
		
		// Recule
		sendALPHA(0, PRECISION_ALPHA);				waitDone(1);
		sendXY_RAPIDE(2000, 1500, PRECISION_XY);	waitDone(1);
		sendALPHA(270, PRECISION_ALPHA);			waitDone(1);
		sendXY_RAPIDE(2000, 500, PRECISION_XY);		waitDone(1);
		sendALPHA(180, PRECISION_ALPHA);			waitDone(1);
		sendXY_RAPIDE(1000, 500, PRECISION_XY);		waitDone(1);
		sendALPHA(90, PRECISION_ALPHA);				waitDone(1);
		sendXY_RAPIDE(1000, 1500, PRECISION_XY);	waitDone(1);
		
		// Avance
		sendALPHA(180, PRECISION_ALPHA);			waitDone(1);
	}
}

// r*sqrt(2)/2 = 293    =>    r = 414
void infini() {
	sendXY_RAPIDE(1500, 1000, PRECISION_XY);
	sendALPHA(180 - 45, PRECISION_ALPHA);
	while (1) {
		sendXY_RAPIDE(1500 + 293, 1000 - 293, PRECISION_ENCHAINE);		waitDone(1);
		sendXY_COURBE(1500 + 3*293, 1000 - 293, PRECISION_ENCHAINE);	waitDone(1);
		sendXY_COURBE(1500 + 3*293, 1000 + 293, PRECISION_ENCHAINE);	waitDone(1);
		sendXY_COURBE(1500 + 293, 1000 + 293, PRECISION_ENCHAINE);		waitDone(1);
		sendXY_RAPIDE(1500 - 293, 1000 - 293, PRECISION_ENCHAINE);		waitDone(1);
		sendXY_COURBE(1500 - 3*293, 1000 - 293, PRECISION_ENCHAINE);	waitDone(1);
		sendXY_COURBE(1500 - 3*293, 1000 + 293, PRECISION_ENCHAINE);	waitDone(1);
		sendXY_COURBE(1500 - 293, 1000 + 293, PRECISION_ENCHAINE);		waitDone(1);
	}
}

void testJul() {
	int i;
	
	sendXY_RAPIDE(770, 1900, PRECISION_XY);
	sendALPHA(105, PRECISION_ALPHA);				waitDone(2);
	UART_XBee_PutString(" AttenteCourbe ");
	WAIT_TIRETTE;
	// Demi-cercle de 70 cm de rayon
	sendXY_COURBE(2230, 1900, PRECISION_XY);
	
	sendXY_RAPIDE(2230, 1800, PRECISION_XY);		waitDone(2);
	sendALPHA(90, PRECISION_ALPHA);					waitDone(1);
	// 2 avance/recule d'1 m
	UART_XBee_PutString("AttenteMetre ");
	WAIT_TIRETTE;
	for (i=0; i<2; i++) {
		sendALPHA(90, PRECISION_ALPHA);
		sendXY_RAPIDE(2230, 800, PRECISION_XY);		waitDone(2);
		sendALPHA(90, PRECISION_ALPHA);
		sendXY_RAPIDE(2230, 1800, PRECISION_XY);	waitDone(2);
	}
	// Avance/recule de 10, 50 et 150 cm
	UART_XBee_PutString("Attente10cm ");
	WAIT_TIRETTE;
	sendXY_RAPIDE(2230, 1700, PRECISION_XY);
	sendALPHA(90, PRECISION_ALPHA);					waitDone(2);
	UART_XBee_PutString("Attente50cm ");
	WAIT_TIRETTE;
	sendXY_RAPIDE(2230, 1200, PRECISION_XY);
	sendALPHA(270, PRECISION_ALPHA);				waitDone(2);
	sendXY_RAPIDE(2230, 200, PRECISION_XY);
	sendALPHA(270, PRECISION_ALPHA);				waitDone(2);
	UART_XBee_PutString("Attente150cm ");
	WAIT_TIRETTE;
	sendXY_RAPIDE(2230, 1700, PRECISION_XY);
	sendALPHA(270, PRECISION_ALPHA);				waitDone(2);
	
	// 4 demi-tours
	for (i=0; i<2; i++) {
		sendALPHA(90, PRECISION_ALPHA);				waitDone(1);
		sendALPHA(270, PRECISION_ALPHA);			waitDone(1);
	}
	
	// Rotations de 20, 45 et 135°
	UART_XBee_PutString("Attente20Deg ");
	WAIT_TIRETTE;
	sendALPHA(290, PRECISION_ALPHA);				waitDone(1);
	UART_XBee_PutString("Attente45Deg ");
	WAIT_TIRETTE;
	sendALPHA(315, PRECISION_ALPHA);				waitDone(1);
	UART_XBee_PutString("Attente135Deg ");
	WAIT_TIRETTE;
	sendALPHA(45, PRECISION_ALPHA);					waitDone(1);
	
	UART_XBee_PutString("Infini ");
	WAIT_TIRETTE;
	infini();
}

/* [] END OF FILE */
