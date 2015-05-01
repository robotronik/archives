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

#include "machine.h"
#include "comAsser.h"
#include "Depart.h"
#include "comXbee.h"
#include "LED_Debug.h"
#include "sharp.h"
#include "Timer_Fin.h"
#include "UART_XBee.h"
#include "constantes.h"


int obstacleMAE = 0;
int repriseMAE = 0;
int recalculMAE = 0;
int libererMAE = 0;
int evitementMAE = 0;
int tirette = 0;
int finLiberMAEN = 1; 


Mission missionCourante;
//Mission missionTmp;
//Mission mission2;
Mission missionReprise;


void changerNumMissionCourante(int nouveau)
{
	missionCourante.numero = nouveau;
	PRINTF("Changement num mc %d->%d\r",missionCourante.numero, nouveau);
}	


#if 0
//vieux truc de l'homologation
extern volatile int doneAsser;
extern volatile int stopSharp;

Depl tabDepl[NB_CMD];

static State curState = PRE_INIT, nextState = PRE_INIT;

// transitions //
int sharpOk = 0;


void initTabDepl(void)
{
	tabDepl[0].t = DELTA;
	tabDepl[0].val = 1200;
	tabDepl[1].t = ALPHA;
	tabDepl[1].val = 90;
	tabDepl[2].t = DELTA;
	tabDepl[2].val = 250;
	tabDepl[3].t = ALPHA;
	tabDepl[3].val = 90;
	tabDepl[4].t = DELTA;
	tabDepl[4].val = 1150;
}

void DoDepl(int num)
{
	if (tabDepl[num].t == ALPHA)
		sendAlpha(tabDepl[num].val);
	else
		sendDelta(tabDepl[num].val);

}

void machine(void)
{
	static int numDepl = 0;
	switch (curState)
	{
		case PRE_INIT:
			//init fait dans le main en fait
			tirette = !(Depart_Read());
			if (tirette)
				nextState = INIT;
			else
				nextState = PRE_INIT;	
			break;
		case INIT:
			UART_XBee_PutString("INIT");
			Timer_Fin_Start();
			LED_Debug_Write(255);
			nextState = WORK;
			break;
		case WORK:
			if (numDepl == 0)
			{
				UART_XBee_PutString("WORK0");
				DoDepl(numDepl);
				numDepl++;
			}
			if (doneAsser)
			{
				UART_XBee_PutString("WORKX");
				doneAsser = 0;
				if (numDepl < NB_CMD)
					DoDepl(numDepl);
				numDepl++;
			}	
			if (numDepl > NB_CMD)
				nextState = STOP;
			else
				nextState = TEST_SHARP;
			break;
		case TEST_SHARP:
			checkPos();
			sharpOk = !stopSharp;
			if (sharpOk)
				nextState = WORK;
			else
				nextState = STOP;	
			break;
		case STOP:
			UART_XBee_PutString("STOP");
			sendStop();
			nextState = PUITS;
			break;
		case PUITS:
			// nothing
			nextState = PUITS;
			break;
	}	
	curState = nextState;

}
#endif //0
/* [] END OF FILE */
