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

#include "objectifs.h"
#include "constantes.h"
#include "forge.h"
#include "comAsser.h"

extern Mission missionCourante;
extern volatile Pos robotNous;

extern volatile int requestGetAlpha;
extern volatile int requestGetXY;

extern int pileDe4;
extern int nbVerreEmpile;
extern int passeSurTous;
extern int passeSurTousN;

extern volatile int t45s;
extern volatile int t45sN;
extern volatile int t60s;
extern volatile int t60sN;

Objectifs objectifs;


void initObjectifs(void)
{
	int i;
	objectifs.fini = 0;
	objectifs.numero = 0;
	for ( i = 0; i < NB_OBJECTIFS ; i++)
	{
		objectifs.tab[i].t = NULL_MISSION;	
		objectifs.tab[i].onlyIf = NULL;
		objectifs.tab[i].endIf = NULL;
	}
	/*
	objectifs.tab[0].t = CHOPER_VERRE;
	//objectifs.tab[1] = BOUGIES;
	objectifs.tab[1].t = RAMENE_VERRE;
	objectifs.tab[2].t = CHOPER_VERRE_ADV;
	objectifs.tab[3].t = RAMENE_VERRE;
	/*/
	//objectifs.tab[0].t = BOUGIES;
	//objectifs.tab[1].t = AUTRE_MISSION;
	//objectifs.tab[2].t = AUTRE_MISSION;
	//objectifs.tab[3].t = CADEAUX;
	//*/
	
	//Réalisation d'un carre
	//objectifs.tab[0] = CARRE;
	
	//Réalisation de demitours
	//objectifs.tab[0] = DEMI_TOURS;
	
	

	/*objectifs.tab[1] = CHOPER_VERRE;
	objectifs.tab[2] = RAMENE_VERRE;
	objectifs.tab[3] = AUTRE_MISSION;*/
	
	//*/
	// Scenario : on va chercher les verres, quand on en a 4 on les rammene
	// et on recommence 3 fois maxi (3 piles de 4)
	// la troisième fois est facultative (si l'adversiare en a pris, par ex)
	// puis on va se caler pour faire le gateau
	// enfin on redépose une pile si nécéssaire
	objectifs.tab[0].t = TOUT_DROIT;
	
	objectifs.tab[1].t = VERRES;
	objectifs.tab[1].endIfSoft = &pileDe4;
	
	objectifs.tab[2].t = RAMENE_VERRE;
	objectifs.tab[2].onlyIf = &nbVerreEmpile;
	
	objectifs.tab[3].t = VERRES;
	objectifs.tab[3].endIfSoft = &pileDe4;
	
	objectifs.tab[4].t = RAMENE_VERRE;
	objectifs.tab[4].onlyIf = &nbVerreEmpile;
	
	objectifs.tab[5].t = VERRES;
	objectifs.tab[5].endIfSoft = &pileDe4;
	objectifs.tab[5].onlyIf = &passeSurTousN;//pas necessaire
	
	objectifs.tab[6].t = RAMENE_VERRE;
	objectifs.tab[6].onlyIf = &nbVerreEmpile;
	
	// Au cas où 
	objectifs.tab[7].t = VERRES;
	objectifs.tab[7].onlyIf = &passeSurTousN;//pas necessaire
	//Au cas où
	objectifs.tab[8].t = RAMENE_VERRE;
	objectifs.tab[8].onlyIf = &nbVerreEmpile;
	
	objectifs.tab[9].t = CALLAGE_HAUT;
	
	objectifs.tab[10].t = CALLAGE_BUFFET;
	
	objectifs.tab[11].t = BOUGIES_FACILES;
	objectifs.tab[11].onlyIf = (int *)(&t45s); 
	
	objectifs.tab[12].t = BOUGIES;
	objectifs.tab[12].onlyIf = (int *)(&t45sN); 
	
	//On sait jamais
	objectifs.tab[13].t = RAMENE_VERRE;
	objectifs.tab[13].onlyIf = &nbVerreEmpile;
	
	objectifs.tab[14].t = RATISSAGE;
	objectifs.tab[14].endIfSoft = &nbVerreEmpile;
	
	objectifs.tab[15].t = RAMENE_VERRE;
	objectifs.tab[15].onlyIf = &nbVerreEmpile;
	
	/*/
	objectifs.tab[0].t = CALLAGE_HAUT;
	objectifs.tab[1].t = CALLAGE_BUFFET;
	objectifs.tab[2].t = BOUGIES_FACILES;
	//objectifs.tab[0].t = VERRES_4;
	//objectifs.tab[1].t = RAMENE_VERRE;
	//objectifs.tab[2].t = VERRES_4;
	//objectifs.tab[3].t = RAMENE_VERRE;
	//objectifs.tab[4].t = RAMENE_VERRE;
	
	//*/
	forgerObjectif(&missionCourante,objectifs.tab[objectifs.numero]);

}

void forgerObjectif(Mission * m, Objectif o)
{
	switch (o.t)
	{
		case NULL_MISSION:		
			initMission(m);
			break;
		case VERRES:		
			//forgerMissionVerreNous(m,robotNous);
			forgerMissionVerre(m,robotNous);
			break;
		case VERRES_4:		
			forgerMissionVerre4(m,robotNous);
			break;
		/*case CHOPER_VERRE_ADV:		
			forgerMissionVerreAdv(m,robotNous);
			break;*/
		case RAMENE_VERRE:		
			forgerMissionDeposerPile(m,robotNous);
			break;
		case BOUGIES:		
			forgerMissionGateau(m,robotNous);
			break;
		case BOUGIES_FACILES:		
			forgerMissionBougiesFaciles(m,robotNous);
			break;
		case RATISSAGE:
			forgerMissionRatissage(m,robotNous);
			break;
		case CALLAGE_HAUT :
			forgerMissionCallageHaut(m, robotNous);
			break;
		case CALLAGE_BUFFET :
			forgerMissionCallageBuffet(m, robotNous);
			break;
		case TOUT_DROIT :
			forgerMissionToutDroit(m, robotNous);
			break;

		case AUTRE_MISSION://DEBUG
			forgerMissionAuDepart(m,robotNous);
			break;
		case CARRE ://DEBUG
			forgerMissionCarre(m, robotNous);
			break;
		case DEMI_TOURS ://DEBUG
			forgerMissionDemiTours(m, robotNous);
			break;
			
		default:
			initMission(m);
			PRINTF("Erreur : pas d'objectif correspondant\r\n");
			break;
	}

}


/* [] END OF FILE */
