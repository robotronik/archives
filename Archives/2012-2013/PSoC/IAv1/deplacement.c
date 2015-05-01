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

#include "deplacement.h"
#include "constantes.h"
#include "comAsser.h"
#include <math.h>
#include <stdlib.h>
#include "cartographie.h"

#define DEG_TO_RAD (3.14159265359/180.0)
#define RAD_TO_DEG (180.0/3.14159265359)

extern volatile Pos robotNous;

/* Genere l'instruction nécessaire pour se placer en un alpha absolu
à partir d'un alpha d'origine en Pos*/
//void genererActionAlphaFrom(Action * a, int alphaOr, int alphaDst)
//{


//}

int genererActionDelta( Action * a, int val, Pos pF)
{
	a->t = DEPL;
	(a->dep).t = DELTA;
	(a->dep).val = val;
	(a->pFinal) = pF;
	return 1;
}

int genererActionAlphaAbs( Action * a, int alpha, Pos pF)
{
	a->t = DEPL;
	(a->dep).t = ALPHA_ABS;
	(a->dep).val = alpha;
	(a->pFinal) = pF;
	(a->pFinal).alpha = alpha;
	return 1;

}

/* Genere l'instruction nécessaire pour se placer en un alpha absolu
à partir d'un alpha d'origine*/
int genererActionAlphaFrom(Action * a, int alphaOr, int alphaDst)
{
	/*
	int diff = alphaDst - alphaOr;
	diff %= 360; // c'est null, à améliorer avec le plus court, TODO, DONE
	if (diff > 180)
	{
		diff = diff-360;

	}
	else if (diff < -180)
	{
		diff = 360+diff;
	}
	a->t = DEPL;
	(a->dep).t = ALPHA;
	(a->dep).val = diff;
	(a->pFinal).alpha = alphaDst;
	/*/
	a->t = DEPL;
	(a->dep).t = ALPHA_ABS;
	(a->dep).val = alphaDst;
	(a->pFinal).alpha = alphaDst;//*/
	return 1;
}

/* Genere l'instruction nécessaire pour se placer en un alpha absolu
à partir de la position du robot (dans robotNous)*/
//obsolete
int genererActionAlpha( Action * a, int alpha)  //TODO : à définer
{
	return genererActionAlphaFrom(a, robotNous.alpha, alpha);
	
	/*int diff = alpha - robotNous.alpha;
	diff %= 360; // c'est null, à améliorer avec le plus court
	a->t = DEPL;
	(a->dep).t = ALPHA;
	(a->dep).val = diff;*/
}

/* Genere les deux instructions pour se placer en un x,y particulier (ds pDst)
à partir d'une position x,y,alpha (ds pOr), le alpha de dst n'est pas pris en 
compte*/
int genererActionsXYFromPos(Action tab[2], Pos pOr, Pos pDst)
{
	genererActionAlphaFrom(tab, pOr.alpha, RAD_TO_DEG*(atan2((double)pDst.y - (double)pOr.y,(double)pDst.x - (double)pOr.x)));
	tab[1].t = DEPL;
	tab[1].dep.t=DELTA;
	tab[1].dep.val = sqrt(abs(pDst.x - pOr.x)*abs(pDst.x - pOr.x)
									+ abs(pDst.y - pOr.y)*abs(pDst.y - pOr.y));
	tab[1].pFinal = pDst;
	tab[0].pFinal.x = pOr.x;
	tab[0].pFinal.y = pOr.y;
	tab[1].pFinal.alpha = tab[0].pFinal.alpha;
	return 2;

}

int genererActionsXYFromPosLong(Action tab[5], Pos pOr, Pos pDst)
{
	genererActionsXYFromPos(tab, pOr, pDst);
	if (tab[1].dep.val >= 110)//si trop grand on coupe
	{
		tab[1].dep.val = (int)((double)tab[1].dep.val*(0.75));
		tab[1].pFinal.x = (int)((double)tab[1].pFinal.x*(0.75));
		tab[1].pFinal.y = (int)((double)tab[1].pFinal.y*(0.75));
		genererActionsToXYPos(tab+2, pDst);//rempli 2,3,4
	}
	else
	{
		tab[2].t = NOP;
		tab[3].t = NOP;
		tab[4].t = NOP;
	}
	return 5;
}

int genererActionsXYFromPosAlphaFinal(Action tab[3], Pos pOr, Pos pDst)
{
	genererActionsXYFromPos(tab, pOr, pDst);
	tab[2].pFinal = pDst;
	genererActionAlphaFrom(tab+2, tab[1].pFinal.alpha, pDst.alpha);
	return 3;

}

//obsolete
int genererActionsXY(Action tab[2], int x, int y) //TODO : à definer
{
	Pos p = { x, y, 0};
	return genererActionsXYFromPos(tab, robotNous, p);
	/*genererActionAlpha(tab, atan((y - robotNous.y)/x - robotNous.x));
	tab[2].t = DEPL;
	tab[2].dep.t=DELTA;
	tab[2].dep.val = sqrt(abs(x - robotNous.x)*abs(x - robotNous.x) +
								abs(y - robotNous.y)*abs(y - robotNous.y));*/
}

int genererActionsToXYPos(Action tab[3], Pos pDst)
{
	tab[0].t = XY_POS;
	tab[0].pFinal = pDst;
	tab[1].pFinal = pDst;
	tab[2].pFinal = pDst;
	return 3;
}

int genererActionsToXYPosLong(Action tab[6], Pos pDst)
{
	tab[0].t = XY_POS_LONG;
	tab[0].pFinal = pDst;
	tab[1].pFinal = pDst;
	tab[2].pFinal = pDst;
	tab[3].pFinal = pDst;
	tab[4].pFinal = pDst;
	tab[4].pFinal = pDst;
	return 6;
}

int genererActionsToXYAlphaFinal(Action tab[4], Pos pDst)
{
	tab[0].t = XY_ALPHA_FINAL;
	tab[0].pFinal = pDst;
	tab[1].pFinal = pDst;
	tab[2].pFinal = pDst;
	tab[3].pFinal = pDst;
	return 4;
}

// tab ne doit pas être decalé ( uniquement tabAction par ex)
Pos findAJumpTo(Action * tab, int index, Pos pObst, int * newIndex)
{
	int i;
	if ( index == 0 )
		index = 1;
	for (i = index-1 ; i < NB_ACTIONS_PAR_MISSION ; i++)
	{
		if (tab[i].t != NULL_ACTION)
		{
			if (!pointDansDisque(tab[i].pFinal, pObst, TAILLE_ADV))
			{
				*newIndex = i+1;
				return tab[i].pFinal;
			}
		}

	}
	return (Pos){-1, -1, -1};
}	


/* [] END OF FILE */
