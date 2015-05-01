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
#include "Depart.h"
#include "comXbee.h"
#include "LED_Debug.h"
#include "Timer_Fin.h"
#include "sharp.h"
#include "deplacement.h"
#include "gestionBras.h"
#include "forge.h"
#include "objectifs.h"
#include "Timer_Fin.h"
#include "forge.h"

#include "Verre_Present.h"//debug carto

enum StateMind { PRE_INIT, INIT, CHOIX_OBJ, GET_POS, TEST_OBSTACLE, VERIF_OBJ, RECALCUL, CHANGER_OBJ, BLOCKED };
typedef enum StateMind StateMind;

extern volatile Pos robotNous;

extern Mission missionCourante;
//extern Mission mission2;
//extern Mission missionTmp;
extern Mission missionReprise;

extern volatile int requestGetAlpha;
extern volatile int requestGetXY;

extern Objectifs objectifs;


extern int repriseMAE;
//extern int recalculMAE;
extern int obstacleMAE;
extern int tirette;
extern int stopSharp;
extern int evitementMAE;

extern Pos posObstacle;

static int cpt = 0;
static unsigned int tempsObst;
static int astarEnable = 1;

static StateMind curState = PRE_INIT, nextState = PRE_INIT;
extern Pos  pDepart;
void initMission2(void) // une premiere mission pour tester
{
	//Pos p = {1200, 1050, 0};
	//initMission(&mission2);
	initMission(&missionReprise);
	//forgerMissionDeposerPile(&mission2, p); 
	//forgerMissionGateau(&mission2, p);
	/*mission2.numero=0;
	int i;
	for ( i = 0 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		mission2.tabAction[i].waitFor = NULL;
	mission2.t = BOUGIES;
	mission2.fini = 0;
	mission2.tabAction[0].t = AUTRE_ACTION;
	mission2.tabAction[0].proc = &starterBrasDroitBas;
	mission2.tabAction[1].t = AUTRE_ACTION;
	mission2.tabAction[1].proc = &armerBrasDroitBas;
	//genererActionsXY(mission2.tabAction+2, bougiesBas[3].p.x, bougiesBas[3].p.y);
	genererActionsXYFromPos(&(mission2.tabAction[2]), pDepart, bougiesBas[3].p);
	mission2.tabAction[4].t = AUTRE_ACTION;
	mission2.tabAction[4].proc = &taperBrasDroitHaut;
	for ( i = 5 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
		mission2.tabAction[i].t = NULL_ACTION;*/
}

void machineMind(void)
{
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
			DEBUG_XBEE("_INIT_MIND\r\n");
			nextState = CHOIX_OBJ;
			break;
		case CHOIX_OBJ:
			cpt  = (cpt+1);
			cpt %= 10000;
			if (missionReprise.fini)
			{
				DEBUG_PRINTF("Reprise fini");
				repriseMAE = 1;
				evitementMAE = 0;
				initMission(&missionReprise);
			}
			else if (missionCourante.fini)
			{
				if (objectifs.tab[objectifs.numero+1].onlyIf == NULL || *(objectifs.tab[objectifs.numero+1].onlyIf) )
				{
					DEBUG_PRINTF("mission fini");
					CyDelay(50);
					getAlpha();
					waitRequestGetAlphaSafe();
					//while(requestGetAlpha);
					getXY();
					//while(requestGetXY);
					waitRequestGetXYSafe();
					objectifs.numero++;
					CyDelay(50);
					forgerObjectif(&missionCourante,objectifs.tab[objectifs.numero]);
				}
				else //*(objectifs.tab[objectifs.numero].onlyIf) == 0
				{
					DEBUG_PRINTF("mission fini, suivante");
					objectifs.numero++;
				}	
			}
			nextState = GET_POS;
			break;
		case GET_POS:
			if (cpt == 0)//TODO
			{
				//if(!requestGetXY)
				//{
					PRINTF("GET POS\r");
					getXY();
					waitRequestGetXYSafe();
					//while(requestGetXY);
				//}
				//if(!requestGetAlpha)
				//{
					getAlpha();
					waitRequestGetAlphaSafe();
					//while(requestGetAlpha);
				//}
				//DEBUG_PRINTF("cnt%d",Timer_Fin_ReadCounter());
			}
			nextState = TEST_OBSTACLE;
			break;
		case TEST_OBSTACLE:
			if ((cpt%500) == 0)//TODO
			{
				checkPos();
			}
			if (stopSharp && !obstacleMAE)
			{//première fois obstacle
				tempsObst = 0;
				evitementMAE = 0;
				if (missionReprise.t != NULL_MISSION)//c'set une misison reprise
				{
					if (missionReprise.tabAction[missionReprise.numero-1].t == DEPL 
						&& missionReprise.tabAction[missionReprise.numero-1].dep.t == DELTA)
					{
						obstacleMAE = 1;
						DEBUG_PRINTF("detec reprise");
					}
					else
					{
						stopSharp = 0;//RAZ car pas delta
					}
				}
				else//missionCourante
				{
					if (missionCourante.tabAction[missionCourante.numero-1].t == DEPL 
						&& missionCourante.tabAction[missionCourante.numero-1].dep.t == DELTA)
					{
						obstacleMAE = 1;
						DEBUG_PRINTF("detec courante");
					}
					else
					{
						stopSharp = 0;//RAZ car pas delta
					}
				}
				//DEBUG_PRINTF("Obstacle detecté");
				//obstacleMAE = 1;
				//recalculMAE = 1;
				nextState = VERIF_OBJ;
			}
			else if (stopSharp && obstacleMAE)
			{//toujours le même obsatcle
				obstacleMAE = 1;
				tempsObst++;
				if (tempsObst >= 60000 && astarEnable)
				{//ça fait trop lomptemps qu'on attends on se déplace
					PRINTF("trop longtemps\r");
					//if(!requestGetXY)
					//{
						getXY();
						waitRequestGetXYSafe();
						//while(requestGetXY);
					//}
					//if(!requestGetAlpha)
					//{
						getAlpha();
						waitRequestGetAlphaSafe();
						//while(requestGetAlpha);
					//}
					if (forgerMissionCartoEvitementAstar(&missionReprise, robotNous, posObstacle)==1)// le ==1 est important
					{
						PRINTF("Ok forgerMissionCarto\r");
						evitementMAE = 1;
						obstacleMAE = 0;
						stopSharp = 0;
					}
					else
					{
						PRINTF("Echec forgerMissionCarto\r");
						evitementMAE = 0;
						obstacleMAE = 1;
						initMission(&missionReprise);
					}
					//while(Verre_Present_Read());
					tempsObst = 0;
				}
				else if (tempsObst > 60000)
				{
					tempsObst = 0;
				}
				nextState = VERIF_OBJ;
			}
			else if (!stopSharp && obstacleMAE /*&& !evitementMAE*/)
			{// plus d'obsracle
				DEBUG_PRINTF("Plus d'obstacle");
				missionReprise.t = REPRISE_MISSION;
				if (missionCourante.tabAction[missionCourante.numero-1].t == DEPL)
				{
					//if(!requestGetXY)
					//{
						getXY();
						//while(requestGetXY);
						waitRequestGetXYSafe();
					//}
					//if(!requestGetAlpha)
					//{
						getAlpha();
						//while(requestGetAlpha);
						waitRequestGetAlphaSafe();
					//}
					PRINTF("Recalcul pour finir l'action\r");
					int j = genererActionsXYFromPosAlphaFinal(missionReprise.tabAction, robotNous, 
								missionCourante.tabAction[missionCourante.numero-1].pFinal);
					genererNop(missionReprise.tabAction+j,
								missionCourante.tabAction[missionCourante.numero-1].pFinal);
				}
				obstacleMAE = 0;
				//recalculMAE = 0;
				nextState = VERIF_OBJ;	
			}
			else //RAS
			{//pas d'obsatcle TVB
				obstacleMAE = 0;
				evitementMAE = 0;
				nextState = VERIF_OBJ;
			}
			break;
		case VERIF_OBJ:
			if (!missionCourante.fini)
			{
				if (objectifs.tab[objectifs.numero].endIf != NULL)
				{
					if (*(objectifs.tab[objectifs.numero].endIf))
						missionCourante.fini = 1;
				}
				if (objectifs.tab[objectifs.numero].endIfSoft != NULL)
				{
					if (*(objectifs.tab[objectifs.numero].endIfSoft))
					{
						/* la prochine action est NOP puis NULL pour stop action suivante*/
						missionCourante.tabAction[missionCourante.numero].t = NOP;
						missionCourante.tabAction[missionCourante.numero].procInt = NULL;
						missionCourante.tabAction[missionCourante.numero].waitFor = NULL;
						missionCourante.tabAction[missionCourante.numero+1].t = NULL_ACTION;
					}
				}
			}
			nextState = CHOIX_OBJ;
			break;
		case RECALCUL:
			//calcul missionReprise dan tmp
			/*missionTmp.t = REPRISE_MISSION;
			genererActionsXYFromPosAlphaFinal(missionTmp.tabAction, robotNous, 
								missionCourante.tabAction[missionCourante.numero-1].pFinal);
			nextState = TEST_OBSTACLE;*/
			break;
		case CHANGER_OBJ:
			
			break;
		case BLOCKED:
			
			break;
	}	
	curState = nextState;

}

void enableAstar(void)
{
	astarEnable = 1;
}

void disableAstar(void)
{
	astarEnable = 0;
}

/* [] END OF FILE */
