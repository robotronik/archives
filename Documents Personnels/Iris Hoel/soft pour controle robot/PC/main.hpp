
#ifndef _MAIN_
#define _MAIN_


/*------------------------------- include -----------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <math.h>
#include <Windows.h>



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

#define MAX_JOYSTICK            ((int)32600)
#define RAPPORT_VITESSE_SIMU    ((float)((MAX_JOYSTICK*20)/TMP_FLIP_SIMU))
#define MAX_REEL                ((int)4000)

#define TMP_FLIP_SIMU           ((unsigned int)20)
#define TMP_ONE_STEP_REEL       ((unsigned int)200)
#define TMP_CHECK_UART_RX       ((unsigned int)40)
//#define TMP_FLIP_UART_RX        ((unsigned int)10)


/* code */
#define MOTOR                   ((unsigned int)19)





/*------------------------------- declaration fonctions ---------------------*/


void process(unsigned int code, int data);





/*----------- enable coresponding printf by uncomment -----------------------*/

//#define UART_CMD
#define XBEE_ERROR
#define START_UP







/*------------------------------- struct ------------------------------------*/


typedef struct Coord
{
float x;
float y;
} Coord ;

typedef struct Robot
{
    float direction_robot;
    SDL_Surface *robot;
    Uint32 couleur;
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
