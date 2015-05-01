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
#include "table.h"

Verre verres[NB_VERRES];

int passeSurTous = 0;
int passeSurTousN = 1;

void initVerre(void)
{
	int i;
	for (i = 0; i < NB_VERRES ; i++)
		verres[i].state = SUR_TABLE;
	/*	//DISPO 1 depart 0°
	verres[0].p.x = 900;
	verres[0].p.y = 550;
	verres[1].p.x = 1200;
	verres[1].p.y = 550;
	verres[2].p.x = 1350;
	verres[2].p.y = 800;
	verres[3].p.x = 1050;
	verres[3].p.y = 800;
	verres[4].p.x = 900;
	verres[4].p.y = 1050;
	verres[5].p.x = 1200;
	verres[5].p.y = 1050;
	
	verres[6].p.x = LONGUEUR -1200;
	verres[6].p.y = 1050;
	verres[7].p.x = LONGUEUR -900;
	verres[7].p.y = 1050;
	verres[8].p.x = LONGUEUR -1050;
	verres[8].p.y = 800;
	verres[9].p.x = LONGUEUR -1350;
	verres[9].p.y = 800;
	verres[10].p.x = LONGUEUR -1200;
	verres[10].p.y = 550;
	verres[11].p.x = LONGUEUR -900;
	verres[11].p.y = 550;
	/*/ //DISPO 2  Depart angle vers gateau
	verres[0].p.x = 900;
	verres[0].p.y = 1050;
	verres[1].p.x = 1200;
	verres[1].p.y = 1050;
	verres[2].p.x = 1350;
	verres[2].p.y = 800;
	verres[3].p.x = 1050;
	verres[3].p.y = 800;
	verres[4].p.x = 900;
	verres[4].p.y = 550;
	verres[5].p.x = 1200;
	verres[5].p.y = 550;
	

	verres[6].p.x = LONGUEUR -1200;
	verres[6].p.y = 550;
	verres[7].p.x = LONGUEUR -900;
	verres[7].p.y = 550;
	verres[8].p.x = LONGUEUR -1050;
	verres[8].p.y = 800;
	verres[9].p.x = LONGUEUR -1350;
	verres[9].p.y = 800;
	verres[10].p.x = LONGUEUR -1200;
	verres[10].p.y = 1050;
	verres[11].p.x = LONGUEUR -900;
	verres[11].p.y = 1050;
	
	
	
	/*verres[0].p.x = 900;
	verres[0].p.y = 550;
	verres[1].p.x = 1200;
	verres[1].p.y = 550;
	verres[2].p.x = LONGUEUR -1200;
	verres[2].p.y = 550;
	verres[3].p.x = LONGUEUR -900;
	verres[3].p.y = 550;
	
	verres[4].p.x = 1050;
	verres[4].p.y = 800;
	verres[5].p.x = 1350;
	verres[5].p.y = 800;
	verres[6].p.x = LONGUEUR -1050;
	verres[6].p.y = 800;
	verres[7].p.x = LONGUEUR -1350;
	verres[7].p.y = 800;
	
	verres[8].p.x = 900;
	verres[8].p.y = 1050;
	verres[9].p.x = 1200;
	verres[9].p.y = 1050;
	verres[10].p.x = LONGUEUR -1200;
	verres[10].p.y = 1050;
	verres[11].p.x = LONGUEUR -900;
	verres[11].p.y = 1050;*/

	//*/
	
	/*for (i = NB_VERRES/2 ; i < NB_VERRES ; i++)
	{
		verres[i].p.x = LONGUEUR - verres[i-NB_VERRES/2].p.x;
		verres[i].p.y = verres[i-NB_VERRES/2].p.y;
	}*/
}
/*
long creerIndice(int i1, int i2, int i3)
{
    //assert : 0 < ix < 100
    return (long)i1*10000 + i2*100 + i3;
}*/

/* Vérifie si on est passé par tous les verres */
void checkVerre(void)
{
	int i;
	for (i = 0 ; i< NB_VERRES ; i++)
	{
		if (verres[i].state != RECUP)
		{
			passeSurTous = 0;
			passeSurTousN = 1;
			return;
		}
	}
	passeSurTous = 1;
	passeSurTousN = 0;
}

void majToRecup(int indice)
{
    // indice = 112233
    /*int i1 = indice/10000;//
    int i2 = (indice/100)%100;//
    int i3 = indice%100;//
    if (i1 != 99)
    {
       verres[i1].state = RECUP;
    }
    if (i2 != 99)
    {
        verres[i2].state = RECUP;
    }
    if (i3 != 99)
    {
       verres[i3].state = RECUP;
    }*/
	verres[indice].state = RECUP;
	checkVerre();
}

/*long ajouter(long indice, long n)
{
    int i1 = indice/10000;//
    int i2 = (indice/100)%100;//
    int i3 = indice%100;//
    if (i1 == 99)
    {
       i1 = n;
    }
    else if (i2 == 99)
    {
       i2 = n;
    }
    else if (i3 == 99)
    {
       i3 = n;
    }
	return creerIndice(i1,i2,i3);

}*/


/* [] END OF FILE */
