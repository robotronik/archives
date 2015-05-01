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
#include "callage.h"
#include "comAsser.h"
#include "constantes.h"
#include "Verre_Present.h"
#include <device.h>
#include <math.h>
#include "bougies.h"
#include "porte.h"

extern volatile int derapageN;
extern volatile int requestGetAlpha;
extern volatile int requestGetXY;

extern volatile Pos robotNous;
extern volatile int bumper2N;

void callageDepart(void)
{
	DEBUG_PRINTF("DEBUT CALLAGE");
	while(Verre_Present_Read());
	sendPresPrecis();
	sendAlphaAbs(180);
	waitDoneAsser();
	sendDelta(500);
	/*CyDelay(150);
	resetDerapage();
	while(derapageN);*/
	while(bumper2N);
	sendStop();
	resetDoneAsser();
	sendDelta(50);
	CyDelay(100);
	//resetDerapage();
	sendSetAlpha(180);
	CyDelay(50);
	sendSetXY(RAYON_AV, -1);
	CyDelay(50);
	
	sendStop();
	resetDoneAsser();
	
	getAlpha();
	waitRequestGetAlphaSafe();
	//while(requestGetAlpha);
	getXY();
	waitRequestGetXYSafe();
	//while(requestGetXY);
	
	sendDelta(-(200-RAYON_AV));
	waitDoneAsser();
	sendAlphaAbs(-90);
	waitDoneAsser();
	sendDelta(600);
	//CyDelay(150);
	//resetDerapage();
	///while(derapageN);
	while(bumper2N);
	sendStop();
	resetDoneAsser();
	sendDelta(50);
	CyDelay(100);
	//resetDerapage();
	sendSetAlpha(-90);
	CyDelay(50);
	sendSetXY(-1, RAYON_AV+100);//100 = taille buffet
	CyDelay(50);
	sendStop();
	resetDoneAsser();
	
	getAlpha();
	waitRequestGetAlphaSafe();
	//while(requestGetAlpha);
	getXY();
	waitRequestGetXYSafe();
	//while(requestGetXY);
	
	sendDelta(-400);
	waitDoneAsser();
	sendAlphaAbs(0);
	waitDoneAsser();
	
	getAlpha();
	waitRequestGetAlphaSafe();
	getXY();
	waitRequestGetXYSafe();
	//while(requestGetXY);
	
	sendDelta(170-robotNous.x);
	waitDoneAsser();
	
	CyDelay(50);
	getAlpha();
	waitRequestGetAlphaSafe();
	//while(requestGetAlpha);
	getXY();
	waitRequestGetXYSafe();
	//while(requestGetXY);
	
	// on vise le gateau
	sendAlphaAbs((int)(RAD_TO_DEG*(atan2((double)(HAUTEUR-robotNous.y),(double)(X_CENTRE_GATEAU-robotNous.x)))-2));
	waitDoneAsser();

	//toucherAvecPorte();// sert à rien avec le freeWheel
	
	getAlpha();
	waitRequestGetAlphaSafe();
	//while(requestGetAlpha);
	getXY();
	waitRequestGetXYSafe();
	//while(requestGetXY);
	
	CyDelay(50);
	sendPresGros();
	CyDelay(50);
	resetDoneAsser();
	sendFreeWheel();
	//resetDerapage();
	
	DEBUG_PRINTF("FIN CALLAGE");
}

/* [] END OF FILE */
