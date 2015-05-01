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

#include "constantes.h"
#include "comAsser.h"
#include "comRpi.h"
#include "Depart.h"
#include "comXbee.h"
#include "LED_Debug.h"
#include "Timer_Fin.h"
#include "porte.h"
#include "forge.h"
#include "isr_Fin.h"
#include "Timer_Fin.h"
#include "TimerResetFin.h"
#include "comRpi.h"
#include "comXbee.h"
#include "UnHzISR.h"


#include "deplacement.h"

enum StateGo { PRE_INIT, INIT, DO_OBJ, STOP, REPRISE };
typedef enum StateGo StateGo;


/* transitions */
extern int obstacleMAE;
extern int repriseMAE;
extern int recalculMAE;
extern int libererMAE;
extern int tirette;
extern int evitementMAE;

/* variables globales robot */
extern volatile int doneAsser;
extern Pos  pDepart;
extern volatile Pos robotNous;

extern volatile int requestGetAlpha;
extern volatile int requestGetXY;

extern Mission missionCourante;
extern Mission missionReprise;

/* variables locales à la machine */
//static int numeroActionCourante = 0;

/* Etat initial : PRE_INIT */
static StateGo curState = PRE_INIT, nextState = PRE_INIT;

/*
void initMissionCourante(void) // une premiere mission pour tester
{
	//Pos p = {1500, 1500, 0};
	//forgerMissionVerreNous(&missionCourante, pDepart);
	//forgerMissionGateau(&missionCourante, pDepart);
	forgerMissionCarre(&missionCourante, pDepart);
	*/
	
	
	/*int i;
	missionCourante.t = CHOPER_VERRE;
	missionCourante.fini = 0;
	for ( i = 0 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		missionCourante.tabAction[i].waitFor = NULL;
	for ( i = 0 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		missionCourante.tabAction[i].t = NULL_ACTION;	
	genererActionsXYFromPos(missionCourante.tabAction, pDepart, p);*/
	/*int i;
	for ( i = 0 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		missionCourante.tabAction[i].waitFor = NULL;
		
	missionCourante.t = CHOPER_VERRE;
	missionCourante.fini = 0;
	missionCourante.tabAction[0].t = DEPL;
    missionCourante.tabAction[0].dep.t = DELTA;
        missionCourante.tabAction[0].dep.val = 1200;
        missionCourante.tabAction[1].t = DEPL;
        missionCourante.tabAction[1].dep.t = ALPHA;
        missionCourante.tabAction[1].dep.val = 90;
        missionCourante.tabAction[2].t = DEPL;
        missionCourante.tabAction[2].dep.t = DELTA;
        missionCourante.tabAction[2].dep.val = 250;
        missionCourante.tabAction[3].t = DEPL;
        missionCourante.tabAction[3].dep.t = ALPHA;
        missionCourante.tabAction[3].dep.val = 90;
        missionCourante.tabAction[4].t = DEPL;
        missionCourante.tabAction[4].dep.t = DELTA;
        missionCourante.tabAction[4].dep.val = 1150;
        missionCourante.tabAction[5].t = DEPL;
        missionCourante.tabAction[5].dep.t = ALPHA;
        missionCourante.tabAction[5].dep.val = 180;
        missionCourante.tabAction[6].t = AUTRE_ACTION;
        missionCourante.tabAction[6].proc = &libererVerre;
        missionCourante.tabAction[7].t = DEPL;
        missionCourante.tabAction[7].dep.t = DELTA;
        missionCourante.tabAction[7].dep.val = 250;
        missionCourante.tabAction[7].waitFor = &libererMAE;
        missionCourante.tabAction[8].t = AUTRE_ACTION;
        missionCourante.tabAction[8].proc = &fermerPorte;
        for ( i = 9 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
                missionCourante.tabAction[i].t = NULL_ACTION;*/
	/*missionCourante.tabAction[0].t = DEPL;
	missionCourante.tabAction[0].dep.t = DELTA;
	missionCourante.tabAction[0].dep.val = 2500;
	missionCourante.tabAction[1].t = DEPL;
	missionCourante.tabAction[1].dep.t = ALPHA;
	missionCourante.tabAction[1].dep.val = 90;
	missionCourante.tabAction[2].t = DEPL;
	missionCourante.tabAction[2].dep.t = DELTA;
	missionCourante.tabAction[2].dep.val = 1200;
	missionCourante.tabAction[3].t = DEPL;
	missionCourante.tabAction[3].dep.t = ALPHA;
	missionCourante.tabAction[3].dep.val = 90;
	missionCourante.tabAction[4].t = DEPL;
	missionCourante.tabAction[4].dep.t = DELTA;
	missionCourante.tabAction[4].dep.val = 300;
	missionCourante.tabAction[5].t = DEPL;
	missionCourante.tabAction[5].dep.t = ALPHA;
	missionCourante.tabAction[5].dep.val = 90;
	missionCourante.tabAction[6].t = DEPL;
	missionCourante.tabAction[6].dep.t = DELTA;
	missionCourante.tabAction[6].dep.val = 1100;
	missionCourante.tabAction[7].t = DEPL;
	missionCourante.tabAction[7].dep.t = ALPHA;
	missionCourante.tabAction[7].dep.val = -90;
	missionCourante.tabAction[8].t = DEPL;
	missionCourante.tabAction[8].dep.t = DELTA;
	missionCourante.tabAction[8].dep.val = 2150;
	missionCourante.tabAction[9].t = DEPL;
	missionCourante.tabAction[9].dep.t = ALPHA;
	missionCourante.tabAction[9].dep.val = 180;
	missionCourante.tabAction[10].t = AUTRE_ACTION;
	missionCourante.tabAction[10].proc = &libererVerre;
	missionCourante.tabAction[11].t = DEPL;
	missionCourante.tabAction[11].dep.t = DELTA;
	missionCourante.tabAction[11].dep.val = 250;
	missionCourante.tabAction[11].waitFor = &libererMAE;
	missionCourante.tabAction[12].t = AUTRE_ACTION;
	missionCourante.tabAction[12].proc = &fermerPorte;
	for ( i = 13 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		missionCourante.tabAction[i].t = NULL_ACTION;*/
	/*missionCourante.tabAction[0].t = DEPL;
	missionCourante.tabAction[0].dep.t = DELTA;
	missionCourante.tabAction[0].dep.val = 2000;
	missionCourante.tabAction[1].t = DEPL;
	missionCourante.tabAction[1].dep.t = ALPHA;
	missionCourante.tabAction[1].dep.val = 90;
	missionCourante.tabAction[2].t = DEPL;
	missionCourante.tabAction[2].dep.t = DELTA;
	missionCourante.tabAction[2].dep.val = 500;//250;
	missionCourante.tabAction[3].t = DEPL;
	missionCourante.tabAction[3].dep.t = ALPHA;
	missionCourante.tabAction[3].dep.val = 90;
	missionCourante.tabAction[4].t = DEPL;
	missionCourante.tabAction[4].dep.t = DELTA;
	missionCourante.tabAction[4].dep.val = 250;//250;
	missionCourante.tabAction[5].t = DEPL;
	missionCourante.tabAction[5].dep.t = ALPHA;
	missionCourante.tabAction[5].dep.val = 90;
	missionCourante.tabAction[6].t = DEPL;
	missionCourante.tabAction[6].dep.t = DELTA;
	missionCourante.tabAction[6].dep.val = 250;//250;
	missionCourante.tabAction[7].t = DEPL;
	missionCourante.tabAction[7].dep.t = ALPHA;
	missionCourante.tabAction[7].dep.val = -90;
	missionCourante.tabAction[8].t = DEPL;
	missionCourante.tabAction[8].dep.t = DELTA;
	missionCourante.tabAction[8].dep.val = 1750;
	missionCourante.tabAction[9].t = DEPL;
	missionCourante.tabAction[9].dep.t = ALPHA;
	missionCourante.tabAction[9].dep.val = 180;
	missionCourante.tabAction[10].t = AUTRE_ACTION;
	missionCourante.tabAction[10].proc = &libererVerre;
	missionCourante.tabAction[11].t = DEPL;
	missionCourante.tabAction[11].dep.t = DELTA;
	missionCourante.tabAction[11].dep.val = 250;
	//missionCourante.tabAction[7].wait = 1;
	missionCourante.tabAction[11].waitFor = &libererMAE;
	missionCourante.tabAction[12].t = AUTRE_ACTION;
	missionCourante.tabAction[12].proc = &fermerPorte;
	for ( i = 13 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		missionCourante.tabAction[i].t = NULL_ACTION;*/
//}


void doAction(Action action)
{	
	//int diff;
	/* Pour l'action ATTENTE */
	//static unsigned int  cntCur, cntDeb;
	//static int attStat = 0;
	if (action.waitFor !=NULL && *(action.waitFor) != 0) // si une wait et que waitFor != 0 on reste sur cette action
	{
		//numeroActionCourante--;
		missionCourante.numero--; //Pas possible de faire de wait for sur une mission reprise
		//PRINTF("wait %d->%d\r",missionCourante.numero,missionCourante.numero-1);
	}
	else
	{
		PRINTF("####AC_OK####:%d\r\n",missionCourante.numero);
		if (action.procInt != NULL)
        	(*(action.procInt))(action.argProc);
		switch (action.t)
		{
			case DEPL :
				if (action.dep.t == ALPHA)
				{
					sendAlpha(action.dep.val);
					//getAlpha(); //DEBUG
					
				}
				else if (action.dep.t == DELTA)
				{
					sendDelta(action.dep.val);
					//getAlpha(); //DEBUG
					
				}
				else //ALPHA_ABS
				{
					sendAlphaAbs(action.dep.val);
					//getAlpha(); //DEBUG					
				}
				break;
			case AUTRE_ACTION :
				(*(action.proc))();
				DEBUG_PRINTF("proc %p",action.proc);
				//getAlpha(); //DEBUG
				break;
			case NOP :
				DEBUG_PRINTF("Nop");
				//getAlpha(); //DEBUG
				//rien
				break;
			case XY_POS:// les 2 actions suivantes seront écrasées
				getAlpha();
				//while(requestGetAlpha);
				waitRequestGetAlphaSafe();
				getXY();
				waitRequestGetXYSafe();
				//while(requestGetXY);
				if (missionReprise.t != NULL_MISSION) // cas de reprise style A*
					genererActionsXYFromPos(&(missionReprise.tabAction[missionReprise.numero+1]),
						robotNous, action.pFinal);
				else // cas normal
					genererActionsXYFromPos(&(missionCourante.tabAction[missionCourante.numero+1]),
						robotNous, action.pFinal);
				DEBUG_PRINTF("XY_POS %d,%d",action.pFinal.x,action.pFinal.y);
				break;
			case XY_POS_LONG:
				getAlpha();
				//while(requestGetAlpha);
				waitRequestGetAlphaSafe();
				getXY();
				//while(requestGetXY);
				waitRequestGetXYSafe();
				if (missionReprise.t != NULL_MISSION) // cas de reprise style A*
					genererActionsXYFromPosLong(&(missionReprise.tabAction[missionReprise.numero+1]),
							robotNous, action.pFinal);
				else
					genererActionsXYFromPosLong(&(missionCourante.tabAction[missionCourante.numero+1]),
							robotNous, action.pFinal);
				DEBUG_PRINTF("XY_POS_LONG %d,%d",action.pFinal.x,action.pFinal.y);
				break;
			case GET_POS_DO:
				getAlpha();
				//while(requestGetAlpha);
				waitRequestGetAlphaSafe();
				getXY();
				//while(requestGetXY);
				waitRequestGetXYSafe();
				DEBUG_PRINTF("GET_XY_POS");
				break;
			case XY_ALPHA_FINAL:// les 3 actions suivantes seront écrasées
				getAlpha();
				//while(requestGetAlpha);
				waitRequestGetAlphaSafe();
				getXY();
				//while(requestGetXY);
				waitRequestGetXYSafe();
				if (missionReprise.t != NULL_MISSION) // cas de reprise style A*
					genererActionsXYFromPosAlphaFinal(&(missionReprise.tabAction[missionReprise.numero+1]),
							robotNous, action.pFinal);
				else
					genererActionsXYFromPosAlphaFinal(&(missionCourante.tabAction[missionCourante.numero+1]),
							robotNous, action.pFinal);
				DEBUG_PRINTF("XY_ALPHA_FINAL %d,%d,%d",action.pFinal.x,action.pFinal.y,action.pFinal.alpha);
				break;
			case CALLAGE_DEPL:
				if (action.dep.t == DELTA)
				{
					sendDelta(action.dep.val);
					DEBUG_PRINTF("delta callage");
				}
				else
					DEBUG_PRINTF("##########BUG#########");
				break;
			case ATTENTE://en ms
				/*
				if (attStat ==0)
				{
					cntDeb = Timer_Fin_ReadCounter();
					attStat = 1;
					DEBUG_PRINTF("attente deb : %u", cntDeb);
				}
				else
				{
					cntCur = Timer_Fin_ReadCounter();
					if ((cntDeb - cntCur) >= (unsigned int)(action.dep.val/2));
					{	//car cnt 2ms(500Hz)
						DEBUG_PRINTF("attente fin : %u	, val : %d", cntCur, action.dep.val/2);
						attStat = 0;
						break;
					}
				}
				missionCourante.numero--;
				/*/
				CyDelay(action.dep.val);
				//*/
				break;
			case SET_A:
				sendSetAlpha(action.dep.val);
				DEBUG_PRINTF("set alpha");
				break;
			case SET_X_CAL:
				sendSetXY(action.dep.val,-1);
				DEBUG_PRINTF("set x callage");
				break;
			case SET_Y_CAL:
				sendSetXY(-1,action.dep.val);
				DEBUG_PRINTF("set y callage");
				break;
			case NULL_ACTION :
				missionCourante.fini = 1;//inutile mais prog def, onvadire
				break;
			default :
				break;
		}
	}
		

}

void machineGo(void)
{
	static int sortieReprise = 0;
	switch (curState)
	{
		case PRE_INIT:
			//init fait dans le main en fait
			tirette = !(Depart_Read());
			//sendRpiCouleur();
			if (tirette)
			{
				sendCouleurRobot2();
				nextState = INIT;
			}
			else
				nextState = PRE_INIT;
			break;
		case INIT:
			DEBUG_XBEE("__INIT_GO__\r\n");
			
			Timer_Fin_Start();//on lance le timer de 90s
			UnHzISR_Start();
			//On envoit la tirette au pi.
			sendRpiTirette();
			sendTiretteRobot2();
			fermerPorte();
			nextState = DO_OBJ;
			break;
		case DO_OBJ:
			if (missionCourante.t != NULL_MISSION )
			{
				if ( missionCourante.tabAction[missionCourante.numero].t != NULL_ACTION)
				{
					if (missionCourante.numero == 0)
					{
						//DEBUG_XBEE("__DO_ACT0__\r\n");
						doAction(missionCourante.tabAction[missionCourante.numero]);
						missionCourante.numero++;
					}
					else 
					{
						if (missionCourante.tabAction[missionCourante.numero-1].t == DEPL)
						{
						
							if (doneAsser || sortieReprise)
							{
									doneAsser = 0;
									sortieReprise = 0;
									doAction(missionCourante.tabAction[missionCourante.numero]);
								missionCourante.numero++;
							}
						}
						else
						{
							sortieReprise = 0;
							doAction(missionCourante.tabAction[missionCourante.numero]);
							missionCourante.numero++;
						}	
					}
				}
				else
				{
					missionCourante.fini = 1;
				}
			}
			if (obstacleMAE)
				nextState = STOP;
			else
				nextState = DO_OBJ;		
			break;
		case STOP:
			DEBUG_XBEE("__STOP_GO_\r\n");
			CyDelay(50);
			sendStop();
			CyDelay(50);
			nextState = REPRISE;
			repriseMAE  = 0;
			sortieReprise = 0;
			break;
		case REPRISE:
			//DEBUG_XBEE("_REPRISE_GO\r\n");
			if (missionReprise.t != NULL_MISSION )
			{
				if ( missionReprise.tabAction[missionReprise.numero].t != NULL_ACTION)
				{
					if (missionReprise.numero == 0)
					{
						doAction(missionReprise.tabAction[missionReprise.numero]);
						missionReprise.numero++;
					}
					else 
					{
						if (missionReprise.tabAction[missionReprise.numero-1].t == DEPL)
						{
						
							if (doneAsser)
							{
									DEBUG_PRINTF("Depl fini");
									doneAsser = 0;
									doAction(missionReprise.tabAction[missionReprise.numero]);
								missionReprise.numero++;
							}
						}
						else
						{
							//DEBUG_XBEE("__DO_ACTAU__");
							doAction(missionReprise.tabAction[missionReprise.numero]);
							missionReprise.numero++;
						}	
					}
				}
				else
				{
					missionReprise.fini = 1;
					evitementMAE = 0;
				}
			}
			//recalculMAE = 1;
			if (repriseMAE)
			{
				initMission(&missionReprise);//RAZ
				sortieReprise = 1;
				PRINTF("Fin de la reprise\r");
				PRINTF("missionCourante.t = %d\r", missionCourante.t);
				PRINTF("missionCourante.tab = %d\r", missionCourante.tabAction[missionCourante.numero].t);
				PRINTF("missionCourante.tab-1 = %d\r", missionCourante.tabAction[missionCourante.numero-1].t);
				nextState = DO_OBJ;
			}
			else
				nextState = REPRISE;
			break;
	}	
	curState = nextState;

}

/* [] END OF FILE */
