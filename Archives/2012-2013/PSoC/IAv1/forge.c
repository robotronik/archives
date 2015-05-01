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
#include "forge.h"
#include "constantes.h"
#include "table.h"
#include "deplacement.h"
#include "porte.h"
#include "bougies.h"
#include "gestionBras.h"
#include "comAsser.h"
#include "sharp.h"
#include "cartographie.h"
#include "machine.h"
#include <math.h>

extern Pos pDepart;
extern Couleur couleur;

extern Mission missionCourante;

extern Verre verres[NB_VERRES];
extern Bougie bougiesBas[NB_BOUGIES_BAS];
extern Bougie bougiesHaut[NB_BOUGIES_HAUT];

extern int libererMAE;
extern int finLiberMAEN;

extern volatile int finTimerBrasN;

extern volatile int derapageN;
extern volatile int bumperBuffetN;
extern volatile int bumper2N;

extern void  enableFailSafeEmp(void);
extern void disableFailSafeEmp(void);
extern void enableAstar(void);
extern void disableAstar(void);

Pos posPile[NB_PILES];
static int numPile = 0;

void initPiles(void)
{
	posPile[0].x = 200;
	posPile[0].y = 830;
	posPile[0].alpha = 0;
	posPile[1].x = 280;
	posPile[1].y = 650;
	posPile[1].alpha = 0;
	posPile[2].x = 305;
	posPile[2].y = 500;
	posPile[2].alpha = 0;
	posPile[3].x = 320;
	posPile[3].y = 350;
	posPile[3].alpha = 0;
	posPile[4].x = 350;
	posPile[4].y = 310;
	posPile[4].alpha = 0;

}

/* fusione un delta -> alpha 0 -> delta en delta*/ //not used
void  simplifierFusionDelta(Action * tab)
{
   int i;
   for (i = 2; i < NB_ACTIONS_PAR_MISSION ; i++)
   {
      if(tab[i].t == DEPL && tab[i-1].t == DEPL
         && tab[i-2].t == DEPL)
      {
         if (tab[i].dep.t == DELTA && tab[i-1].dep.t == ALPHA_ABS
             && tab[i-2].dep.t == DELTA)
         {
            if (tab[i-1].dep.val == tab[i-2].pFinal.alpha)
            {
               tab[i-1].t = NOP;
               tab[i-2].t = NOP;
               tab[i].dep.val = tab[i-2].dep.val + tab[i].dep.val;
			   tab[i].procInt = tab[i-1].procInt;
			   tab[i].argProc =  tab[i-1].argProc;
			   //if (i+2 < NB_ACTIONS_PAR_MISSION)
			   		//i += 2;//on saute pour fiare qu'un sur deux
            }
         }
         else if (tab[i].dep.t == DELTA && tab[i-1].dep.t == ALPHA
                  && tab[i-2].dep.t == DELTA)
         {
            if (tab[i-1].dep.val == 0)
            {
               tab[i-1].t = NOP;
               tab[i-2].t = NOP;
               tab[i].dep.val = tab[i-2].dep.val + tab[i].dep.val;
			   tab[i].procInt = tab[i-1].procInt;
			   tab[i].argProc =  tab[i-1].argProc;
            }
         }
      }
   }
}

int genererAutreAction(Action *a, void (*proc)(void), Pos pPrec)
{
	a->t = AUTRE_ACTION;
	a->proc = proc;
	a->pFinal = pPrec;
	return 1;
}

int genererNopWaitFor(Action *a, int * wait, Pos pPrec)
{
	a->t = NOP;
	a->waitFor = wait;
	a->pFinal = pPrec;
	return 1;
}

inline int genererNop(Action *a, Pos pPrec)
{
	return genererNopWaitFor(a, NULL, pPrec); 
}

int genererGetPos(Action *a, Pos pPrec)
{
	a->t = GET_POS_DO;
	a->pFinal = pPrec;
	return 1;
}

int genererSetXCal(Action *a, int val, Pos pPrec)
{
	a->t = SET_X_CAL;
	(a->dep).val = val;
	a->pFinal = pPrec;
	return 1;
}

int genererSetYCal(Action *a, int val, Pos pPrec)
{
	a->t = SET_Y_CAL;
	(a->dep).val = val;
	a->pFinal = pPrec;
	return 1;
}

int genererSetAlpha(Action *a, int val, Pos pPrec)
{
	a->t = SET_A;
	(a->dep).val = val;
	a->pFinal = pPrec;
	return 1;
}

int genererAttente(Action *a, int valms, Pos pPrec)
{
	a->t = ATTENTE;
	(a->dep).val = valms;
	a->pFinal = pPrec;
	return 1;
}

static void initTabAction(Action tab[NB_ACTIONS_PAR_MISSION])
{
	int i;
	for ( i = 0 ; i < NB_ACTIONS_PAR_MISSION ; i ++)
	{
		tab[i].waitFor = NULL;
		tab[i].t = NULL_ACTION;
		tab[i].procInt = NULL;
	}

}

void initMission(Mission * m)
{
	m->t = NULL_MISSION;
	m->fini = 0; //pas fini
	m->numero = 0;
	initTabAction(m->tabAction);

}

void forgerMissionVerre(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Verre, init : %d,%d",pInit.x,pInit.y);
	int i = 0, v;
	initMission(m);
	m->t = VERRES;
	i+=genererAutreAction(m->tabAction+i, &sendPresGros, pInit);
	for (v = 0 ; v < NB_VERRES ; v++)
	{
		if (i>= NB_ACTIONS_PAR_MISSION)
			DEBUG_XBEE("Trop_daction\n");
		if (verres[v].state == SUR_TABLE)
		{
			m->tabAction[i].procInt = &majToRecup;
			m->tabAction[i].argProc = v;//creerIndice(v,99,99);
			if ( i == 0 )
			{
				//i += genererActionsXYFromPos(m->tabAction+i, pInit, verres[v].p);
				i += genererActionsXYFromPosLong(m->tabAction+i, pInit, verres[v].p);
				
			}
			else
			{
				//i += genererActionsXYFromPos(m->tabAction+i, m->tabAction[i-1].pFinal, verres[v].p);
				//i += genererActionsToXYPos(m->tabAction+i, verres[v].p);
				i += genererActionsToXYPosLong(m->tabAction+i, verres[v].p);
			}
		}
	
	}
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
	if (i>= NB_ACTIONS_PAR_MISSION)
			PRINTF("########BUG#########\r      Trop_daction\r####################\r");
}

void forgerMissionVerre4(Mission * m, Pos pInit)// a refaire
{
	DEBUG_PRINTF("Mission Verre 4, init : %d,%d",pInit.x,pInit.y);
	int i = 0;
	///v;
	static  int av = 0;
	initMission(m);
	m->t = VERRES;
	/*tabVerre[ ] = {0,1,10,11,
					8,9,3,2,
					}*/
	if (av == 0)
	{
		i += genererActionsToXYPos(m->tabAction+i, verres[0].p);	
		i += genererActionsToXYPos(m->tabAction+i, verres[11].p);
		
		i += genererActionsToXYPos(m->tabAction+i, verres[8].p);
		i += genererActionsToXYPos(m->tabAction+i, verres[2].p);
		av++;
	}
	else
	{
		i += genererActionsToXYPos(m->tabAction+i, verres[4].p);	
		i += genererActionsToXYPos(m->tabAction+i, verres[7].p);
	}
	/*for (v = 0 ; v < NB_VERRES ; v++)
	{
		if (i>= NB_ACTIONS_PAR_MISSION)
			DEBUG_XBEE("Trop_daction\n");
		if (verres[v].state == SUR_TABLE)
		{
			m->tabAction[i].procInt = &majToRecup;
			m->tabAction[i].argProc = v;//creerIndice(v,99,99);
			if ( i == 0 )
			{
				i += genererActionsXYFromPos(m->tabAction+i, pInit, verres[v].p);
			}
			else
			{
				//i += genererActionsXYFromPos(m->tabAction+i, m->tabAction[i-1].pFinal, verres[v].p);
				i += genererActionsToXYPos(m->tabAction+i, verres[v].p);
			}
		}
	
	}*/
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
	//simplifierFusionDelta(m->tabAction);
}

/*
void forgerMissionVerreNous(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Verre, init : %d,%d",pInit.x,pInit.y);
	int i = 0, v;
	initMission(m);
	m->t = CHOPER_VERRE;
	for (v = 0 ; v < NB_VERRES/2 ; v++)
	{
		if (i>= NB_ACTIONS_PAR_MISSION)
			DEBUG_XBEE("Trop_daction\n");
		if (verres[v].state == SUR_TABLE)
		{
			m->tabAction[i].procInt = &majToRecup;
			m->tabAction[i].argProc = v;//creerIndice(v,99,99);
			if ( i == 0 )
				i += genererActionsXYFromPos(m->tabAction+i, pInit, verres[v].p);
			else
				i += genererActionsXYFromPos(m->tabAction+i, m->tabAction[i-1].pFinal, verres[v].p);
		}
	}
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
    simplifierFusionDelta(m->tabAction);
}*/
/*
void forgerMissionVerreAdv(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Verre Adv, init : %d,%d",pInit.x,pInit.y);
	int i = 0, v;
	initMission(m);
	m->t = CHOPER_VERRE_ADV;
	for (v = NB_VERRES/2 ; v < NB_VERRES ; v++)
	{
		if (i>= NB_ACTIONS_PAR_MISSION)
			DEBUG_XBEE("Trop_daction\n");
		if (verres[v].state == SUR_TABLE)
		{
			m->tabAction[i].procInt = &majToRecup;
			m->tabAction[i].argProc =v;// creerIndice(v,99,99);
			if ( i == 0 )
				i += genererActionsXYFromPos(m->tabAction+i, pInit, verres[v].p);
			else
				i += genererActionsXYFromPos(m->tabAction+i, m->tabAction[i-1].pFinal, verres[v].p);
		}
	}
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
    simplifierFusionDelta(m->tabAction);
}*/


void forgerMissionDeposerPile(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Depose pile, init : %d,%d",pInit.x,pInit.y);
	initMission(m);
	m->t = RAMENE_VERRE;
	int n = 0;
	Pos pivot = {700,650,0};
	n+=genererAutreAction(m->tabAction+n, &sendPresGros, pInit);
	n += genererActionsXYFromPos(m->tabAction+n,
					pInit, pivot);
	//mdoe KERNEL activé hahahahhahaha
	n+=genererAutreAction(m->tabAction+n, &disableDetection, m->tabAction[n-1].pFinal);
	n += genererActionsToXYAlphaFinal(m->tabAction+n,
					posPile[numPile]);
	/*if (numPile == 0) // on se cale
	{
		n+=genererAutreAction(m->tabAction+n, &sendPresPrecis, pInit);
		n += genererActionAlphaAbs(m->tabAction+n, 180, m->tabAction[n-1].pFinal);
		//mode KERNEL activé hahahahhahaha
		n+=genererAutreAction(m->tabAction+n, &disableDetection, m->tabAction[n-1].pFinal);
		
		n+=genererActionDelta(m->tabAction+n, 350,m->tabAction[n-1].pFinal );
		m->tabAction[n-1].t = CALLAGE_DEPL;

		n+=genererAutreAction(m->tabAction+n, &sendStop, m->tabAction[n-1].pFinal);
		m->tabAction[n-1].waitFor = (int*)(&bumper2N);
		
		n+=genererAutreAction(m->tabAction+n, &resetDoneAsser, m->tabAction[n-1].pFinal);
		
		n+=genererActionDelta(m->tabAction+n, 50,m->tabAction[n-1].pFinal );
		m->tabAction[n-1].t = CALLAGE_DEPL;
		
		n+=genererAttente(m->tabAction+n, 200, m->tabAction[n-1].pFinal);
		
		n+=genererSetAlpha(m->tabAction+n, 180,  m->tabAction[n-1].pFinal);
		n+=genererSetXCal(m->tabAction+n, RAYON_AV, m->tabAction[n-1].pFinal);
		
		
		n+=genererAutreAction(m->tabAction+n, &sendStop, m->tabAction[n-1].pFinal);
		
		n+=genererGetPos(m->tabAction+n,m->tabAction[n-1].pFinal);
		
		n+=genererAutreAction(m->tabAction+n, &disableFailSafeEmp, m->tabAction[n-1].pFinal);
		n+=genererActionDelta(m->tabAction+n, -190,m->tabAction[n-1].pFinal );
		n+=genererAutreAction(m->tabAction+n, &enableFailSafeEmp, m->tabAction[n-1].pFinal);
		
		m->tabAction[n-1].pFinal.alpha = 180;
		m->tabAction[n-1].pFinal.x = (RAYON_AV+190);
		//mode USER
		n+=genererAutreAction(m->tabAction+n, &enableDetection, m->tabAction[n-1].pFinal);
		n+=genererAutreAction(m->tabAction+n, &sendPresGros, pInit);
	
	}
	else
	{
		n += genererActionsToXYAlphaFinal(m->tabAction+n,
					posPile[numPile]);
	}*/
	numPile++;
	n += genererAutreAction(m->tabAction+n, &libererVerre,
							m->tabAction[n-1].pFinal);

	m->tabAction[n].t = DEPL;
	m->tabAction[n].dep.t = DELTA;
	m->tabAction[n].dep.val = DIST_LIBER;
	m->tabAction[n].waitFor = &libererMAE;
	m->tabAction[n].pFinal = m->tabAction[n-1].pFinal;
	m->tabAction[n].pFinal.x += DIST_LIBER;
	n++;
	n += genererNop(m->tabAction+n, m->tabAction[n-1].pFinal);
	//mode USER
	n+=genererAutreAction(m->tabAction+n, &enableDetection, m->tabAction[n-1].pFinal);
	//n += genererAutreAction(m->tabAction+n,&sendPresPrecis,m->tabAction[n-1].pFinal);
	n += genererNopWaitFor(m->tabAction+n, &finLiberMAEN, m->tabAction[n-1].pFinal);
	n += genererNop(m->tabAction+n, m->tabAction[n-1].pFinal);
}
#if 0
void forgerMissionGateau(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Gateau, init : %d,%d",pInit.x,pInit.y);
	int i = 0, b, s =0;
	initMission(m);
	m->t = BOUGIES;
	/*
	i += genererAutreAction(m->tabAction+i, &starterBrasGaucheBas,
							pInit);

	//for (b = NB_BOUGIES_BAS-1 ; b >= 0 ; b--)
	while ( bougiesBas[s].state != ALLUME)
		s++;
	if ( s	< NB_BOUGIES_BAS )
	{
		//i += genererActionsXYFromPosAlphaFinal(m->tabAction+i, pInit, bougiesBas[s].p);
		i += genererActionsToXYAlphaFinal(m->tabAction+i,bougiesBas[s].p);
		
		
		i += genererAutreAction(m->tabAction+i, &taperBrasGaucheBas,
								m->tabAction[i-1].pFinal);
		i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);
		for (b = s+1 ; b < NB_BOUGIES_BAS/2 ; b++)
		{
			if (i>= NB_ACTIONS_PAR_MISSION)
				DEBUG_XBEE("Trop_daction\n");
			if (bougiesBas[b].state == ALLUME)
			{

					i += genererAutreAction(m->tabAction+i, &armerBrasGaucheBas,
								m->tabAction[i-1].pFinal);
					i +=  genererActionsToXYAlphaFinal(m->tabAction+i,bougiesBas[b].p);
					
					
					i += genererAutreAction(m->tabAction+i, &taperBrasGaucheBas,
								m->tabAction[i-1].pFinal);
					i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);
			}
		
		}
	}
	i += genererAutreAction(m->tabAction+i, &replierBrasGaucheBas,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &stopBrasGaucheBas,
							m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&finTimerBrasN);
	/*/
	Pos pInter = {2500, 1200, 0};
	i += genererActionsXYFromPos(m->tabAction+i, pInit, pInter);
	i+=genererAutreAction(m->tabAction+i, &sendPresPrecis, pInter);
	i += genererAutreAction(m->tabAction+i, &starterBrasHaut,
							pInter);
	i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
							m->tabAction[i-1].pFinal);
	s =NB_BOUGIES_HAUT-1;
	while ( bougiesHaut[s].state != ALLUME || bougiesHaut[s].c != couleur)//TODO
		s--;
	if ( s	< NB_BOUGIES_HAUT && s >= 0)
	{
		/*i += genererActionsToXYAlphaFinal(m->tabAction+i,bougiesHaut[s].p);

		i += genererAutreAction(m->tabAction+i, &taperBrasHaut,
								m->tabAction[i-1].pFinal);
		i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);*/
		for (b = s ; b >= 0 ; b--)//b = s-1
		{
			if (i>= NB_ACTIONS_PAR_MISSION)
				DEBUG_XBEE("Trop_daction\n");
			if (bougiesHaut[b].state == ALLUME && (bougiesHaut[b].c == couleur || bougiesHaut[b].c == BLANC))//TODO
			{
					i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
								m->tabAction[i-1].pFinal);
					i +=  genererActionsToXYAlphaFinal(m->tabAction+i,bougiesHaut[b].p);
					
					
					i += genererAutreAction(m->tabAction+i, &taperBrasHaut,
								m->tabAction[i-1].pFinal);
					i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);
					m->tabAction[i].procInt = &majToEteinteHaut;
					m->tabAction[i].argProc =b;
					i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
			}
		
		}
	}
	i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
								m->tabAction[i-1].pFinal);
	i += genererActionDelta(m->tabAction+i, -100, m->tabAction[i-1].pFinal);
	i += genererActionAlphaAbs(m->tabAction+i, -90,  m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &replierBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &stopBrasHaut,
							m->tabAction[i-1].pFinal);
	
	
	//*/
	
	/*if (i>= NB_ACTIONS_PAR_MISSION)
			DEBUG_XBEE("Trop_daction\n");*/
	if (i>= NB_ACTIONS_PAR_MISSION)
			PRINTF("########BUG#########\r      Trop_daction\r####################\r");
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
}
#endif // 0
#if 1
static int doBougieHaut(Mission * m, int b, int idep)
{
	// pre condition : les bras sont armé 
	int i = idep;
	i +=  genererActionsToXYAlphaFinal(m->tabAction+i,bougiesHaut[b].p);	
	if (bougiesHaut[b].state == ALLUME && (bougiesHaut[b].c == couleur || bougiesHaut[b].c == BLANC))//TODO
	{
		PRINTF("la bougie %d Haut est à taper", b);
		i += genererAutreAction(m->tabAction+i, &taperBrasHaut,
													m->tabAction[i-1].pFinal);
		i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
													m->tabAction[i-1].pFinal);
		m->tabAction[i].procInt = &majToEteinteHaut;
		m->tabAction[i].argProc = b;
		i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
		i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
													m->tabAction[i-1].pFinal);
	}
	PRINTF("doBougiesHaut %d\r",b);
	return i;
}

static int doBougieBas(Mission * m, int b, int idep)
{
	// pre condition : les bras sont armé 
	int i = idep;
	i +=  genererActionsToXYAlphaFinal(m->tabAction+i,bougiesBas[b].p);	
	if (bougiesBas[b].state == ALLUME && (bougiesBas[b].c == couleur || bougiesBas[b].c == BLANC))//TODO
	{
		PRINTF("la bougie %d Bas est à taper", b);
		i += genererAutreAction(m->tabAction+i, &taperBrasBas,
													m->tabAction[i-1].pFinal);
		i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
													m->tabAction[i-1].pFinal);
		m->tabAction[i].procInt = &majToEteinteBas;
		m->tabAction[i].argProc = b;
		i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
		i += genererAutreAction(m->tabAction+i, &armerBrasBas,
													m->tabAction[i-1].pFinal);
	}
	PRINTF("doBougiesBas  %d\r",b);
	return i;
}

void forgerMissionGateau(Mission * m, Pos pInit)
{
	int h,b;
	int i =0;
	DEBUG_PRINTF("Mission Gateau, init : %d,%d",pInit.x,pInit.y);
	initMission(m);
	m->t = BOUGIES;
	i+=genererAutreAction(m->tabAction+i, &sendPresPrecis, pInit);
	i+=genererAutreAction(m->tabAction+i, &disableAstar, pInit);
	i += genererAutreAction(m->tabAction+i, &starterBrasHaut,
							pInit);
	i += genererAutreAction(m->tabAction+i, &starterBrasBas,
							pInit);
	i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &armerBrasBas,
							m->tabAction[i-1].pFinal);
	h =NB_BOUGIES_HAUT-2;
	//while ( bougiesHaut[h].state != ALLUME || bougiesHaut[h].c == !couleur)//TODO
	//	h--;
	b =NB_BOUGIES_BAS-2;
	//while ( bougiesBas[b].state != ALLUME || bougiesBas[b].c == !couleur)//TODO
	//	b--;
	while (b>=1 || h>=1)
	{
		if (h == 0)
		{
			PRINTF("h=-1  i=%d\r",i);
			i = doBougieBas(m, b, i);
			break;
		}
		else if (b == 0)
		{
			PRINTF("b=-1  i=%d\r",i);
			i = doBougieHaut(m, h, i);
			break;
		}
		
		if (bougiesBas[b].p.x > bougiesHaut[h].p.x)//bas plus pres
		{
			i = doBougieBas(m, b, i);
			b--;
		}
		else
		{
			i = doBougieHaut(m, h, i);
			h--;
		}
	}
	// on a tout fait sauf le deux derniere où on bump la bordure
	i +=  genererActionsToXYAlphaFinal(m->tabAction+i,(Pos){800,1725,90});	
	i += genererActionDelta(m->tabAction+i, 400, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].t = CALLAGE_DEPL;
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&bumper2N);
	
	i += genererAutreAction(m->tabAction+i, &taperBrasBas,
													m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &taperBrasHaut,
													m->tabAction[i-1].pFinal);
	i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
													m->tabAction[i-1].pFinal);
	m->tabAction[i].procInt = &majToEteinteBas;
	m->tabAction[i].argProc =0;
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
	m->tabAction[i].procInt = &majToEteinteHaut;
	m->tabAction[i].argProc =0;
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
	
	i += genererAutreAction(m->tabAction+i, &armerBrasBas,
													m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
													m->tabAction[i-1].pFinal);
													
	// on en profite pour se recaler
	i+=genererAutreAction(m->tabAction+i, &disableDetection, m->tabAction[i-1].pFinal);
	i += genererActionDelta(m->tabAction+i, 50, m->tabAction[i-1].pFinal);//on se colle bien 
	m->tabAction[i-1].t = CALLAGE_DEPL;
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&bumper2N);
	
	i+=genererGetPos(m->tabAction+i,m->tabAction[i-1].pFinal);
	
	i+=genererSetAlpha(m->tabAction+i, 90,  m->tabAction[i-1].pFinal);
	i+=genererSetYCal(m->tabAction+i, HAUTEUR-RAYON_AV, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].pFinal.y = HAUTEUR-RAYON_AV;
	m->tabAction[i-1].pFinal.alpha = 90;
	
	i+=genererAutreAction(m->tabAction+i, &resetDoneAsser, m->tabAction[i-1].pFinal);
	i += genererActionDelta(m->tabAction+i, -200, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].pFinal.y = m->tabAction[i-1].pFinal.y -200;
	i+=genererAutreAction(m->tabAction+i, &enableDetection, m->tabAction[i-1].pFinal);
	///
	// on se dégage
	i += genererActionAlphaAbs(m->tabAction+i, -90,  m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &replierBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &replierBrasBas,
							m->tabAction[i-1].pFinal);
	i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &stopBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &stopBrasBas,
							m->tabAction[i-1].pFinal);
							
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);

	if (i>= NB_ACTIONS_PAR_MISSION)
			PRINTF("\r########BUG#########\r    Trop_daction\r####################\r%d\r",i);

}


void forgerMissionBougiesFaciles(Mission * m, Pos pInit)// uniquement les bougies blanches + bleu sûr
{
	int h,b;
	int i =0;
	initMission(m);
	DEBUG_PRINTF("Mission Bougies Faciles, init : %d,%d",pInit.x,pInit.y);
	m->t = BOUGIES_FACILES;
	i+=genererAutreAction(m->tabAction+i, &sendPresPrecis, pInit);
	i+=genererAutreAction(m->tabAction+i, &disableAstar, pInit);
	i += genererAutreAction(m->tabAction+i, &starterBrasHaut,
							pInit);
	i += genererAutreAction(m->tabAction+i, &starterBrasBas,
							pInit);
	i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &armerBrasBas,
							m->tabAction[i-1].pFinal);
	//h =4;
	b =7;//8
	/*while (b>=4 || h>=4)
	{
		if (h <= 3)
		{
			i = doBougieBas(m, b, i);
			break;
		}
		else if (b <= 3)
		{
			i = doBougieHaut(m, h, i);
			break;
		}
		
		if (bougiesBas[b].p.x > bougiesHaut[h].p.x)//bas plus pres
		{
			i = doBougieBas(m, b, i);
			b--;
		}
		else
		{
			i = doBougieHaut(m, h, i);
			h--;
		}
	}*/
	while ( b >= 4)
	{
		i = doBougieBas(m, b, i);
		b--;
	}
	b = 0;
	h = 0;
	// une pos inter pour pas touché le gateau
	i += genererActionsToXYPos(m->tabAction+i, (Pos){1000, 1460, 0});
	
	// on a tout fait sauf le deux derniere où on bump la bordure
	i +=  genererActionsToXYAlphaFinal(m->tabAction+i,(Pos){800,1725,90});	
	i += genererActionDelta(m->tabAction+i, 400, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].t = CALLAGE_DEPL;
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&bumper2N);
	
	i += genererAutreAction(m->tabAction+i, &taperBrasBas,
													m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &taperBrasHaut,
													m->tabAction[i-1].pFinal);
	i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
													m->tabAction[i-1].pFinal);
	m->tabAction[i].procInt = &majToEteinteBas;
	m->tabAction[i].argProc =0;
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
	m->tabAction[i].procInt = &majToEteinteHaut;
	m->tabAction[i].argProc =0;
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
	
	i += genererAutreAction(m->tabAction+i, &armerBrasBas,
													m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &armerBrasHaut,
													m->tabAction[i-1].pFinal);
													
	// on en profite pour se recaler
	i+=genererAutreAction(m->tabAction+i, &disableDetection, m->tabAction[i-1].pFinal);
	i += genererActionDelta(m->tabAction+i, 50, m->tabAction[i-1].pFinal);//on se colle bien 
	m->tabAction[i-1].t = CALLAGE_DEPL;
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&bumper2N);
	
	i+=genererGetPos(m->tabAction+i,m->tabAction[i-1].pFinal);
	
	i+=genererSetAlpha(m->tabAction+i, 90,  m->tabAction[i-1].pFinal);
	i+=genererSetYCal(m->tabAction+i, HAUTEUR-RAYON_AV, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].pFinal.y = HAUTEUR-RAYON_AV;
	m->tabAction[i-1].pFinal.alpha = 90;
	
	i+=genererAutreAction(m->tabAction+i, &resetDoneAsser, m->tabAction[i-1].pFinal);
	i += genererActionDelta(m->tabAction+i, -200, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].pFinal.y = m->tabAction[i-1].pFinal.y -200;
	i+=genererAutreAction(m->tabAction+i, &enableDetection, m->tabAction[i-1].pFinal);
	///
	// on se dégage
	i += genererActionAlphaAbs(m->tabAction+i, -90,  m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &replierBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &replierBrasBas,
							m->tabAction[i-1].pFinal);
	i += genererNopWaitFor(m->tabAction+i, (int*)(&finTimerBrasN), 
								m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &stopBrasHaut,
							m->tabAction[i-1].pFinal);
	i += genererAutreAction(m->tabAction+i, &stopBrasBas,
							m->tabAction[i-1].pFinal);


							
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);

	if (i>= NB_ACTIONS_PAR_MISSION)
			PRINTF("########BUG#########\r      Trop_daction\r####################\r");

}
#endif //1
void forgerMissionAuDepart(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission AuDepart, init : %d,%d",pInit.x,pInit.y);
	int i = 0;
	initMission(m);
	m->t = AUTRE_MISSION;
	i+=genererAutreAction(m->tabAction+i, &sendPresPrecis, pInit);
	i+=genererActionsToXYPos(m->tabAction+i, (Pos){2166,1724,0});
	i += genererActionAlphaAbs(m->tabAction+i, 0, m->tabAction[i-1].pFinal);
	//genererActionsXYFromPosAlphaFinal(m->tabAction, pInit, pDepart);
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);

}

void forgerMissionCallageHaut(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Callage Haut, init : %d,%d",pInit.x,pInit.y);
	initMission(m);
	m->t = CALLAGE_HAUT;
	Pos pInter = {2300, 1200, 0};
	int i =0;
	i+=genererAutreAction(m->tabAction+i, &sendPresGros, pInter);
	i+=genererActionsToXYPosLong(m->tabAction+i, pInter);
	pInter.y = 1600;
	i+=genererAutreAction(m->tabAction+i, &sendPresPrecis, pInter);
	i+=genererActionsToXYPos(m->tabAction+i, pInter);
	i += genererActionAlphaAbs(m->tabAction+i, 90, m->tabAction[i-1].pFinal);
	//mdoe KERNEL activé hahahahhahaha
	i+=genererAutreAction(m->tabAction+i, &disableDetection, m->tabAction[i-1].pFinal);
	
	i+=genererActionDelta(m->tabAction+i, 700,m->tabAction[i-1].pFinal );
	m->tabAction[i-1].t = CALLAGE_DEPL;
	//m->tabAction[i-1].pFinal.y = 2000-RAYON_AV;
	
	//i+=genererAttente(m->tabAction+i, 200, m->tabAction[i-1].pFinal);//200 ms
	//i+=genererAutreAction(m->tabAction+i, &resetDerapage, m->tabAction[i-1].pFinal);
	
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&bumper2N);
	
	i+=genererAutreAction(m->tabAction+i, &resetDoneAsser, m->tabAction[i-1].pFinal);
	
	i+=genererActionDelta(m->tabAction+i, 50,m->tabAction[i-1].pFinal );
	m->tabAction[i-1].t = CALLAGE_DEPL;
	//i+=genererAutreAction(m->tabAction+i, &resetDerapage, m->tabAction[i-1].pFinal);
	
	i+=genererAttente(m->tabAction+i, 200, m->tabAction[i-1].pFinal);
	//i+=genererGetPos(m->tabAction+i,m->tabAction[i-1].pFinal);
	
	i+=genererSetAlpha(m->tabAction+i, 90,  m->tabAction[i-1].pFinal);
	i+=genererSetYCal(m->tabAction+i, HAUTEUR-RAYON_AV, m->tabAction[i-1].pFinal);
	
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	
	i+=genererGetPos(m->tabAction+i,m->tabAction[i-1].pFinal);
	
	i+=genererAutreAction(m->tabAction+i, &disableFailSafeEmp, m->tabAction[i-1].pFinal);
	i+=genererActionDelta(m->tabAction+i, -200,m->tabAction[i-1].pFinal );
	i+=genererAutreAction(m->tabAction+i, &enableFailSafeEmp, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].pFinal.alpha = 90;
	m->tabAction[i-1].pFinal.y = HAUTEUR-(RAYON_AV+200);
	//mode USER
	i+=genererAutreAction(m->tabAction+i, &enableDetection, m->tabAction[i-1].pFinal);
	
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);

}

void forgerMissionCallageBuffet(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Callage Buffet, init : %d,%d",pInit.x,pInit.y);
	initMission(m);
	m->t = CALLAGE_BUFFET;
	Pos pInter = {2400, 1805, 0};
	int i =0;
	i+=genererAutreAction(m->tabAction+i, &sendPresPrecis, pInter);
	i+=genererActionsToXYPos(m->tabAction+i, pInter); //alphaFinal
	//i+=genererActionsToXYAlphaFinal(m->tabAction+i, pInter);
	i+=genererActionAlphaAbs(m->tabAction+i, 0,m->tabAction[i-1].pFinal );
	//m->tabAction[i-1].t = CALLAGE_DEPL;
	
	i+=genererAttente(m->tabAction+i, 1500, m->tabAction[i-1].pFinal);

	//mdoe KERNEL activé hahahahhahaha
	i+=genererAutreAction(m->tabAction+i, &disableDetection, m->tabAction[i-1].pFinal);
	
	i+=genererActionDelta(m->tabAction+i, 150,m->tabAction[i-1].pFinal );
	m->tabAction[i-1].t = CALLAGE_DEPL;
	
	i+=genererAttente(m->tabAction+i, 100, m->tabAction[i-1].pFinal);
	
	i+=genererActionDelta(m->tabAction+i, 110,m->tabAction[i-1].pFinal );
	m->tabAction[i-1].t = CALLAGE_DEPL;
	
	i+=genererAttente(m->tabAction+i, 100, m->tabAction[i-1].pFinal);
	
	i+=genererActionDelta(m->tabAction+i, 305,m->tabAction[i-1].pFinal );
	m->tabAction[i-1].t = CALLAGE_DEPL;

	
	i+=genererAutreAction(m->tabAction+i, &sendStop, m->tabAction[i-1].pFinal);
	m->tabAction[i-1].waitFor = (int*)(&bumperBuffetN);
	
	i+=genererAutreAction(m->tabAction+i, &resetDoneAsser, m->tabAction[i-1].pFinal);
	//i+=genererAutreAction(m->tabAction+i, &resetDerapage, m->tabAction[i-1].pFinal);
	
	i+=genererGetPos(m->tabAction+i,m->tabAction[i-1].pFinal);
	
	i+=genererSetXCal(m->tabAction+i, LONGUEUR-(RAYON_AV+400), m->tabAction[i-1].pFinal);
	
	i+=genererGetPos(m->tabAction+i,m->tabAction[i-1].pFinal);
	
	i+=genererAutreAction(m->tabAction+i, &disableFailSafeEmp, m->tabAction[i-1].pFinal);
	i+=genererActionDelta(m->tabAction+i, -200,m->tabAction[i-1].pFinal );
	i+=genererAutreAction(m->tabAction+i, &enableFailSafeEmp, m->tabAction[i-1].pFinal);
	
	m->tabAction[i-1].pFinal.alpha = 0;
	m->tabAction[i-1].pFinal.x = LONGUEUR-(RAYON_AV+200);
	//mode USER
	i+=genererAutreAction(m->tabAction+i, &enableDetection, m->tabAction[i-1].pFinal);
	
	i += genererActionAlphaAbs(m->tabAction+i, 180, m->tabAction[i-1].pFinal);
	
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);

}

void forgerMissionRatissage(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission Ratissage, init : %d,%d",pInit.x,pInit.y);
	int i = 0,j;
	initMission(m);
	m->t = RATISSAGE;
	Pos tabCoor[ ] = {
		{700,450,0},
		{LONGUEUR-700,1200,0},
		{800,1050,0},
		{LONGUEUR-800,925,0},
		{800,675,0},
		{LONGUEUR-800,500} };
	for (j = 0 ; j < 6 ; j++)
	{
		i+=genererActionsToXYPos(m->tabAction+i, tabCoor[j]);
	}
	
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
}

void forgerMissionToutDroit(Mission * m, Pos pInit)
{
	DEBUG_PRINTF("Mission tout droit, init : %d,%d",pInit.x,pInit.y);
	int i = 0;
	initMission(m);
	m->t = TOUT_DROIT;
	
	i += genererActionDelta(m->tabAction+i, 150, (Pos){pInit.x+cos(pInit.alpha)*150, pInit.y+sin(pInit.alpha)*150, pInit.alpha});
	i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
}


/* m doit etre missionReprise, ne peut etre appeler par un objectif classique */ 
int forgerMissionCartoEvitementAstar(Mission *m, Pos pDep, Pos pObst)
{
	int i = 0, newNum, status;
	DEBUG_PRINTF("Mission Evitement Astar, init : %d,%d",pDep.x,pDep.y);
	initMission(m);
	m->t = REPRISE_MISSION;
	Pos pArr = findAJumpTo(missionCourante.tabAction, missionCourante.numero, pObst, &newNum);
	if (pArr.x != -1)
	{
		PRINTF("generation evitement réussi\r");
		PRINTF("numCour:%d newNum:%d\r",missionCourante.numero,newNum);
		i+=genererEvitementAstar(m->tabAction+i, pDep, pArr, pObst, &status);
		if (status == -1)
		{
			PRINTF("Echec i+=generationCarto\r");
			return -1;
		}
		i+=genererActionAlphaAbs(m->tabAction+i, pArr.alpha, m->tabAction[i-1].pFinal);
		m->tabAction[i].procInt = &changerNumMissionCourante;
		m->tabAction[i].argProc = newNum;
		i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
		i += genererNop(m->tabAction+i, m->tabAction[i-1].pFinal);
		return 1;
	}
	else
	{
		//i += genererNop(m->tabAction+i, pDep);
		PRINTF("ECHEC generation evitement\r");
		return -1;
	}

}

void forgerMissionCarre(Mission * m, Pos pInit)
{
	int i,j, nb_carre;
	nb_carre = 4;
	DEBUG_PRINTF("Mission Carre, init : %d,%d",pInit.x,pInit.y);
	initMission(m);
	m->t = CARRE;
	
	i=0;
	Pos dep = pInit;
	Pos arr = dep;
	i += genererActionAlphaAbs(m->tabAction+i, 0, pInit);
	i += genererActionDelta(m->tabAction+i, 2000, m->tabAction[i-1].pFinal);
	i += genererActionAlphaAbs(m->tabAction+i, 180, pInit);
	i += genererActionDelta(m->tabAction+i, 2000, m->tabAction[i-1].pFinal);
	i += genererActionAlphaAbs(m->tabAction+i, 0, pInit);
	for(j=0; j < nb_carre; j++) //Nombre de carre à réaliser
	{
		/*
		i += genererActionAlphaAbs(m->tabAction+i, 90, pInit);
		i += genererActionAlphaAbs(m->tabAction+i, 180, m->tabAction[i-1].pFinal);
		i += genererActionAlphaAbs(m->tabAction+i, -90, m->tabAction[i-1].pFinal);
		i += genererActionAlphaAbs(m->tabAction+i, 0, m->tabAction[i-1].pFinal);
		
		/*/
		dep = arr;
		arr.x = dep.x + 1000;
		arr.alpha = 90;
		i+=genererActionsToXYAlphaFinal(m->tabAction+i, arr);
		
		dep = arr;
		arr.y = dep.y +1000;
		arr.alpha = 180;
		i+=genererActionsToXYAlphaFinal(m->tabAction+i, arr);
		
		dep = arr;
		arr.x = dep.x - 1000;
		arr.alpha = -90;
		i+=genererActionsToXYAlphaFinal(m->tabAction+i,  arr);
		
		dep = arr;
		arr.y = dep.y - 1000;
		arr.alpha = 0;
		i+=genererActionsToXYAlphaFinal(m->tabAction+i,  arr);
		//*/
	}

}


void forgerMissionDemiTours(Mission * m, Pos pInit)
{
	int i,j, nb_tours;
	nb_tours = 10;

	DEBUG_PRINTF("Mission Demi Tour, init : %d,%d",pInit.x,pInit.y);
	initMission(m);
	m->t = DEMI_TOURS;
	
	i=0;

	for(j=0; j < nb_tours; j++) 
	{			
		i+=genererActionAlphaAbs(m->tabAction+i, -120, pInit);
		i+=genererActionAlphaAbs(m->tabAction+i, -240, pInit);
		i+=genererActionAlphaAbs(m->tabAction+i, 0, pInit);

	}

}


/* [] END OF FILE */
