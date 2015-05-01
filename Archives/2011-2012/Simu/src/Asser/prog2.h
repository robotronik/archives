
#ifndef _MAIN_
#define _MAIN_


/*------------------------------- include -----------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>



/*------------------------------- define ------------------------------------*/



#define rad                     ((float)3.14159265358979323846/180)
#define degre                   ((float)180/3.14159265358979323846)
#define TAILLE_TABLE_LARGEUR    ((float)300)
#define TAILLE_TABLE_HAUTEUR    ((float)210)
#define LARGEUR_FENETRE         ((float)900)
#define HAUTEUR_FENETRE         ((float)630)
#define RAPPORT_LARGEUR         ((float)(LARGEUR_FENETRE/TAILLE_TABLE_LARGEUR))
#define RAPPORT_HAUTEUR         ((float)(HAUTEUR_FENETRE/TAILLE_TABLE_HAUTEUR))
#define EPAISSEUR_PIXEL         ((float)2)

#define RAPPORT_VITESSE_SIMU_D  ((float)8000/50*FREQ_ASSER) 			// 2*4000 -> 50 cm/s
#define RAPPORT_VITESSE_SIMU_A  ((float)8000/50*FREQ_ASSER/degre*13.2)	// R = 13,2 cm

#define FREQ_AFFICH				((unsigned int)50)
#define TMP_FLIP_SIMU           ((unsigned int)1000/FREQ_AFFICH)		// ms
#define FREQ_ASSER				((unsigned int)4000)					// Asser a 4 kHz





/*----------- enable coresponding printf by uncomment -----------------------*/

//#define START_UP





/*------------------------------------ functions ----------------------------*/

void init();
void pause(int doAffiche);
void eventListener(int isPaused);
void oneStepSimu();






/*------------------------------- struct ------------------------------------*/


typedef struct Coord
{
float x;
float y;
} Coord ;

typedef struct Robot
{
    float direction_robot;
    Coord position_robot;
    Coord angle_1;
    Coord angle_2;
    Coord angle_3;
    Coord angle_4;
    Coord angle_inter_1;
    Coord angle_inter_2;
    Coord angle_inter_3;
    Coord angle_inter_4;
    Coord arc_cercle_1_1;
    Coord arc_cercle_1_2;
    Coord arc_cercle_2_1;
    Coord arc_cercle_2_2;
    Coord direction_1;
    Coord direction_2;
    Coord centre_arc_1;
    Coord centre_arc_2;
    Coord pince_avant_1;
    Coord pince_avant_2;
    Coord pince_arriere_1;
    Coord pince_arriere_2;
    float rayon_arc;
} Robot ;


#endif
