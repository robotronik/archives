
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
#include "global.h"
#include "strategie.h"

extern Couleur g_couleur;
extern Objectif tabObjectif[20];
extern int g_strategie;

void strategie()
{
    if( JAUNE == g_couleur)
    {
        // Renverser le feux (1100, 2600)
    
        tabObjectif[0].pos.x = 600;
        tabObjectif[0].pos.y = 2770;
        tabObjectif[0].pos.alpha = 0;
		tabObjectif[0].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[0].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[0].actions[2].type = FIN;
    
        tabObjectif[1].pos.x = 600;
        tabObjectif[1].pos.y = 2600;
        tabObjectif[1].pos.alpha = -90;
		tabObjectif[1].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[1].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[1].actions[2].type = FIN;
    
        tabObjectif[2].pos.x = 1000;
        tabObjectif[2].pos.y = 2600;
        tabObjectif[2].pos.alpha = 0;
		tabObjectif[2].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[2].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[2].actions[2].type = FIN;
    
        // Faire tomber le feux (800, 3000)
    
        tabObjectif[3].pos.x = 800;
        tabObjectif[3].pos.y = 2600;
        tabObjectif[3].pos.alpha = 0;
		tabObjectif[3].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[3].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[3].actions[2].type = FIN;
    
        tabObjectif[4].pos.x = 800;
        tabObjectif[4].pos.y = 2800;
        tabObjectif[4].pos.alpha = -90;
		tabObjectif[4].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[4].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[4].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[4].actions[3].type = QUEUE;
        tabObjectif[4].actions[3].parametre = 1;
        tabObjectif[4].actions[4].type = FIN;
    
        tabObjectif[5].pos.x = 800;
        tabObjectif[5].pos.y = 2650;
        tabObjectif[5].pos.alpha = -90;
		tabObjectif[5].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[5].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[5].actions[2].type = QUEUE;
        tabObjectif[5].actions[2].parametre = 0;
        tabObjectif[5].actions[3].type = FIN;
    
        // Tirer sur le mamouth haut (y = 2250)
    
        tabObjectif[6].pos.x = 500;
        tabObjectif[6].pos.y = 2650;
        tabObjectif[6].pos.alpha = -180;
		tabObjectif[6].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[6].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[6].actions[2].type = FIN;
    
        tabObjectif[7].pos.x = 500;
        tabObjectif[7].pos.y = 2250;
        tabObjectif[7].pos.alpha = -90;
		tabObjectif[7].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[7].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[7].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[7].actions[3].type = SERVO_LANCEUR_DROIT;
        tabObjectif[7].actions[3].parametre = 1;
		tabObjectif[7].actions[4].type = MOUVEMENT_ALPHA;
        tabObjectif[7].actions[5].type = SERVO_LANCEUR_DROIT;
        tabObjectif[7].actions[5].parametre = 2;
        tabObjectif[7].actions[6].type = FIN;

        // Tirer sur le mamouth bas (y = 750)
    
		tabObjectif[8].pos.x = 500;
        tabObjectif[8].pos.y = 750;
        tabObjectif[8].pos.alpha = -90;
		tabObjectif[8].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[8].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[8].actions[2].type = FIN;
	
        tabObjectif[9].pos.x = 500;
        tabObjectif[9].pos.y = 750;
        tabObjectif[9].pos.alpha = 90;
        tabObjectif[9].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[9].actions[1].type = SERVO_LANCEUR_GAUCHE;
        tabObjectif[9].actions[1].parametre = 1;
        tabObjectif[9].actions[2].type = FIN;
    
        // Faire tomber le feux (2000, 1300)
    
        tabObjectif[10].pos.x = 500;
        tabObjectif[10].pos.y = 1175;
        tabObjectif[10].pos.alpha = 90;
		tabObjectif[10].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[10].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[10].actions[2].type = FIN;
    
        tabObjectif[11].pos.x = 1600;
        tabObjectif[11].pos.y = 1175;
        tabObjectif[11].pos.alpha = 0; 
		tabObjectif[11].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[11].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[11].actions[2].type = FIN;
    
        tabObjectif[12].pos.x = 1600;
        tabObjectif[12].pos.y = 1300;
        tabObjectif[12].pos.alpha = 90;
		tabObjectif[12].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[12].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[12].actions[2].type = FIN;
    
        tabObjectif[13].pos.x = 1800;
        tabObjectif[13].pos.y = 1300;
        tabObjectif[13].pos.alpha = 180;
		tabObjectif[13].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[13].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[13].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[13].actions[3].type = QUEUE;
        tabObjectif[13].actions[3].parametre = 1;
        tabObjectif[13].actions[4].type = FIN;
    
        tabObjectif[14].pos.x = 1650;
        tabObjectif[14].pos.y = 1300;
        tabObjectif[14].pos.alpha = 180;
		tabObjectif[14].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[14].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[14].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[14].actions[3].type = QUEUE;
        tabObjectif[14].actions[3].parametre = 0;
        tabObjectif[14].actions[4].type = FIN;
    
        // Lancer le filet sur le mamouth bas ou haut
    
        tabObjectif[15].pos.x = 1650;
        tabObjectif[15].pos.y = 1175;
        tabObjectif[15].pos.alpha = -90;
		tabObjectif[15].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[15].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[15].actions[2].type = FIN;
		
		tabObjectif[16].pos.x = 500;
        tabObjectif[16].pos.y = 1175;
        tabObjectif[16].pos.alpha = 180;
		tabObjectif[16].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[16].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[16].actions[2].type = FIN;
    
        if( 1 == g_strategie ) // tirer le mamouth haut (y = 2250)
        {
            tabObjectif[17].pos.x = 500;
            tabObjectif[17].pos.y = 2250;
            tabObjectif[17].pos.alpha = 90;
			tabObjectif[17].actions[0].type = MOUVEMENT_ALPHA;
            tabObjectif[17].actions[1].type = MOUVEMENT_DELTA;
			tabObjectif[17].actions[2].type = MOUVEMENT_ALPHA;
            tabObjectif[17].actions[3].type = FIN;
        
            tabObjectif[18].pos.x = 500;
            tabObjectif[18].pos.y = 2250;
            tabObjectif[18].pos.alpha = 90;
            tabObjectif[18].actions[0].type = FIN;
        }
        else // tirer le mamouth bas (y = 750)
        {
            tabObjectif[17].pos.x = 500;
            tabObjectif[17].pos.y = 750;
            tabObjectif[17].pos.alpha = -90;
			tabObjectif[17].actions[0].type = MOUVEMENT_ALPHA;
            tabObjectif[17].actions[1].type = MOUVEMENT_DELTA;
            tabObjectif[17].actions[2].type = FIN;
			
			tabObjectif[18].pos.x = 500;
            tabObjectif[18].pos.y = 750;
            tabObjectif[18].pos.alpha = 90;
			tabObjectif[18].actions[0].type = MOUVEMENT_ALPHA;
            tabObjectif[18].actions[1].type = FIN;

            tabObjectif[19].pos.x = 500;
            tabObjectif[19].pos.y = 750;
            tabObjectif[19].pos.alpha = 90;
            tabObjectif[19].actions[0].type = FIN;
        }
    }
    else
    {
        // Renverser le feux (1100, 400)
    
        tabObjectif[0].pos.x = 600;
        tabObjectif[0].pos.y = 230;
        tabObjectif[0].pos.alpha = 0;
		tabObjectif[0].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[0].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[0].actions[2].type = FIN;
    
        tabObjectif[1].pos.x = 600;
        tabObjectif[1].pos.y = 400;
        tabObjectif[1].pos.alpha = 90;
		tabObjectif[1].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[1].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[1].actions[2].type = FIN;
    
        tabObjectif[2].pos.x = 1000;
        tabObjectif[2].pos.y = 400;
        tabObjectif[2].pos.alpha = 0;
		tabObjectif[2].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[2].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[2].actions[2].type = FIN;
    
        // Faire tomber le feux (800, 0)
    
        tabObjectif[3].pos.x = 800;
        tabObjectif[3].pos.y = 400;
        tabObjectif[3].pos.alpha = 0;
		tabObjectif[3].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[3].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[3].actions[2].type = FIN;
    
        tabObjectif[4].pos.x = 800;
        tabObjectif[4].pos.y = 200;
        tabObjectif[4].pos.alpha = 90;
		tabObjectif[4].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[4].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[4].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[4].actions[3].type = QUEUE;
        tabObjectif[4].actions[3].parametre = 1;
        tabObjectif[4].actions[4].type = FIN;
    
        tabObjectif[5].pos.x = 800;
        tabObjectif[5].pos.y = 350;
        tabObjectif[5].pos.alpha = 90;
		tabObjectif[5].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[5].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[5].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[5].actions[3].type = QUEUE;
        tabObjectif[5].actions[3].parametre = 0;
        tabObjectif[5].actions[4].type = FIN;
    
        // Tirer sur le mamouth bas (y = 750)
    
        tabObjectif[6].pos.x = 500;
        tabObjectif[6].pos.y = 350;
        tabObjectif[6].pos.alpha = 180;
		tabObjectif[6].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[6].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[6].actions[2].type = FIN;
    
        tabObjectif[7].pos.x = 500;
        tabObjectif[7].pos.y = 750;
        tabObjectif[7].pos.alpha = 90;
		tabObjectif[7].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[7].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[7].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[7].actions[3].type = SERVO_LANCEUR_GAUCHE;
        tabObjectif[7].actions[3].parametre = 1;
		tabObjectif[7].actions[4].type = MOUVEMENT_ALPHA;
        tabObjectif[7].actions[5].type = SERVO_LANCEUR_GAUCHE;
        tabObjectif[7].actions[5].parametre = 2;
        tabObjectif[7].actions[6].type = FIN;
    
        // Tirer sur le mamouth haut (y = 2250)
    
        tabObjectif[8].pos.x = 500;
        tabObjectif[8].pos.y = 2250;
        tabObjectif[8].pos.alpha = 90;
		tabObjectif[8].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[8].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[8].actions[2].type = FIN;
		
		tabObjectif[9].pos.x = 500;
        tabObjectif[9].pos.y = 2250;
        tabObjectif[9].pos.alpha = -90;
        tabObjectif[9].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[9].actions[1].type = SERVO_LANCEUR_DROIT;
        tabObjectif[9].actions[1].parametre = 1;
        tabObjectif[9].actions[2].type = FIN;
    
        // Faire tomber le feux (2000, 1700)
    
        tabObjectif[10].pos.x = 500;
        tabObjectif[10].pos.y = 1825;
        tabObjectif[10].pos.alpha = -90;
		tabObjectif[10].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[10].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[10].actions[2].type = FIN;
    
        tabObjectif[11].pos.x = 1600;
        tabObjectif[11].pos.y = 1825;
        tabObjectif[11].pos.alpha = 0;
		tabObjectif[11].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[11].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[11].actions[2].type = FIN;
    
        tabObjectif[12].pos.x = 1600;
        tabObjectif[12].pos.y = 1700;
        tabObjectif[12].pos.alpha = -90;
		tabObjectif[12].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[12].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[12].actions[2].type = FIN;
    
        tabObjectif[13].pos.x = 1800;
        tabObjectif[13].pos.y = 1700;
        tabObjectif[13].pos.alpha = 180;
		tabObjectif[13].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[13].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[13].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[13].actions[3].type = QUEUE;
        tabObjectif[13].actions[3].parametre = 1;
        tabObjectif[13].actions[4].type = FIN;
    
        tabObjectif[14].pos.x = 1650;
        tabObjectif[14].pos.y = 1700;
        tabObjectif[14].pos.alpha = 180;
		tabObjectif[14].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[14].actions[1].type = MOUVEMENT_DELTA;
		tabObjectif[14].actions[2].type = MOUVEMENT_ALPHA;
        tabObjectif[14].actions[3].type = QUEUE;
        tabObjectif[14].actions[3].parametre = 0;
        tabObjectif[14].actions[4].type = FIN;
    
        // Lancer le filet sur le mamouth haut ou bas
    
        tabObjectif[15].pos.x = 1650;
        tabObjectif[15].pos.y = 1825;
        tabObjectif[15].pos.alpha = 90;
        tabObjectif[15].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[15].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[15].actions[2].type = FIN;
		
		tabObjectif[16].pos.x = 500;
        tabObjectif[16].pos.y = 1825;
        tabObjectif[16].pos.alpha = 180;
        tabObjectif[16].actions[0].type = MOUVEMENT_ALPHA;
        tabObjectif[16].actions[1].type = MOUVEMENT_DELTA;
        tabObjectif[16].actions[2].type = FIN;
    
        if( 0 == g_strategie ) // tirer sur le mamouth bas
        {    
            tabObjectif[17].pos.x = 500;
            tabObjectif[17].pos.y = 750;
            tabObjectif[17].pos.alpha = -90;
			tabObjectif[17].actions[0].type = MOUVEMENT_ALPHA;
            tabObjectif[17].actions[1].type = MOUVEMENT_DELTA;
            tabObjectif[17].actions[2].type = FIN;
        
            tabObjectif[18].pos.x = 500;
            tabObjectif[18].pos.y = 750;
            tabObjectif[18].pos.alpha = -90;
            tabObjectif[18].actions[0].type = FIN;
        }
        else // tirer sur le mamouth haut
        {
            tabObjectif[17].pos.x = 500;
            tabObjectif[17].pos.y = 2250;
            tabObjectif[17].pos.alpha = -90;
			tabObjectif[17].actions[0].type = MOUVEMENT_ALPHA;
            tabObjectif[17].actions[1].type = MOUVEMENT_DELTA;
            tabObjectif[17].actions[2].type = FIN;
        
            tabObjectif[18].pos.x = 500;
            tabObjectif[18].pos.y = 2250;
            tabObjectif[18].pos.alpha = -90;
            tabObjectif[18].actions[0].type = FIN;
        }
    }
}

/* [] END OF FILE */
