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

#include "bougies.h"
#include <math.h>

#define DEG_TO_RAD (3.14159265359/180.0)

extern Couleur couleur;

Bougie bougiesBas[NB_BOUGIES_BAS];
Bougie bougiesHaut[NB_BOUGIES_HAUT];

void initGateau(void)
{
	/*int i;
	double angle = ANGLE_ORIGINE_BAS;
	for (i = NB_BOUGIES_BAS-1 ; i >= 0 ; i-- )
	{
		bougiesBas[i].p.x = X_CENTRE_GATEAU - RAYON*cos(angle*DEG_TO_RAD);
		bougiesBas[i].p.y = (double)HAUTEUR - RAYON*sin(angle*DEG_TO_RAD);
		angle += INCREMENT_ANGLE_BOUGIES_BAS;
	}
	
	angle = ANGLE_ORIGINE_HAUT;
	for (i = NB_BOUGIES_HAUT-1 ; i >= 0 ; i-- )
	{
		bougiesBas[i].p.x = X_CENTRE_GATEAU - RAYON*cos(angle*DEG_TO_RAD);
		bougiesBas[i].p.y = (double)HAUTEUR - RAYON*sin(angle*DEG_TO_RAD);
		angle += INCREMENT_ANGLE_BOUGIES_HAUT;
	}*/
	int i;
	double angle = ANGLE_ORIGINE_BAS;
	
	for (i = 0 ; i < NB_BOUGIES_BAS ; i++)
	{
		bougiesBas[i].state  = ALLUME;
	}
	for (i = 0 ; i < NB_BOUGIES_HAUT ; i++)
	{
		bougiesHaut[i].state  = ALLUME;
	}
	
	for ( i = 0; i < NB_BOUGIES_BAS; i++)
	{
#define CORRECTION 5
		if (couleur == BLEU)
		{
			bougiesBas[i].p.x = X_CENTRE_GATEAU - RAYON_BLEU*cos((angle+CORRECTION)*DEG_TO_RAD);// - 2° de correction empirique
			bougiesBas[i].p.y = HAUTEUR - RAYON_BLEU*sin((angle+CORRECTION)*DEG_TO_RAD);
		}
		else
		{
			bougiesBas[i].p.x = X_CENTRE_GATEAU - RAYON_ROUGE*cos((angle+CORRECTION)*DEG_TO_RAD);// - 2° de correction empirique
			bougiesBas[i].p.y = HAUTEUR - RAYON_ROUGE*sin((angle+CORRECTION)*DEG_TO_RAD);
		}
		bougiesBas[i].p.alpha = (90.0+angle+CORRECTION); // angle de la tangente
		angle += INCREMENT_ANGLE_BOUGIES_BAS;
	}
	/*for ( i = NB_BOUGIES_BAS/2 ; i < NB_BOUGIES_BAS ; i++)
	{
		bougiesBas[i].p.x =  X_CENTRE_GATEAU + (X_CENTRE_GATEAU - bougiesBas[i-((i-6)*2+1)].p.x);
		bougiesBas[i].p.y =  bougiesBas[i-((i-6)*2+1)].p.y;
		bougiesBas[i].p.alpha = -bougiesBas[i-((i-6)*2+1)].p.alpha;
	}*/
	angle = ANGLE_ORIGINE_HAUT;
	for ( i = 0; i < NB_BOUGIES_HAUT/2; i++)
	{
		if (couleur == BLEU)
		{
			bougiesHaut[i].p.x = X_CENTRE_GATEAU - RAYON_BLEU*cos(angle*DEG_TO_RAD);
			bougiesHaut[i].p.y = HAUTEUR - RAYON_BLEU*sin(angle*DEG_TO_RAD);
		}
		else
		{
			bougiesHaut[i].p.x = X_CENTRE_GATEAU - RAYON_ROUGE*cos(angle*DEG_TO_RAD);
			bougiesHaut[i].p.y = HAUTEUR - RAYON_ROUGE*sin(angle*DEG_TO_RAD);
		}
		bougiesHaut[i].p.alpha = (90.0+angle);
		angle += INCREMENT_ANGLE_BOUGIES_HAUT;
		if (i == 0)
			bougiesHaut[i].c = BLEU;
	}
	for ( i = NB_BOUGIES_HAUT/2 ; i < NB_BOUGIES_HAUT ; i++)
	{
		bougiesHaut[i].p.x =  X_CENTRE_GATEAU + (X_CENTRE_GATEAU - bougiesHaut[i-((i-4)*2+1)].p.x);
		bougiesHaut[i].p.y =  bougiesHaut[i-((i-4)*2+1)].p.y;
		bougiesHaut[i].p.alpha = -bougiesHaut[i-((i-4)*2+1)].p.alpha;
		if ( i == NB_BOUGIES_HAUT-1 )
			bougiesHaut[i].c = ROUGE;		
	}
	
	/* les blanc de qualif */
	bougiesBas[4].c = BLANC;
	bougiesBas[5].c = BLANC;
	bougiesBas[6].c = BLANC;
	bougiesBas[7].c = BLANC;
				
	#ifdef DEBUG_MODE
		for ( i = NB_BOUGIES_BAS-1 ; i >=0 ;i--)
			PRINTF("bBas[%d] %d,%d\r",i,bougiesBas[i].p.x,bougiesBas[i].p.y);
		for ( i = NB_BOUGIES_HAUT-1 ; i >=0 ;i--)
			PRINTF("bHaut[%d] %d,%d\r",i,bougiesHaut[i].p.x,bougiesHaut[i].p.y);
	
	#endif //DEBUG_MODE
	
}

void majToEteinteBas(int indice)
{
	bougiesBas[indice].state = ETEINTE;
}

void majToEteinteHaut(int indice)
{
	bougiesHaut[indice].state = ETEINTE;
}

/* [] END OF FILE */
