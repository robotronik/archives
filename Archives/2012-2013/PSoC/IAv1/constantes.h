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

//lire le README.txt pour comprendre 

#ifndef CONSTANTES_H
#define CONSTANTES_H

/* A commenter pour supprimer les traces */
//#define DEBUG_MODE 

#ifdef DEBUG_MODE
#include "UART_XBee.h"
#include <stdio.h>
#define DEBUG_XBEE(v) UART_XBee_PutString(v);
extern char debugBuff[100];
#define DEBUG_PRINTF(message, ...) do { sprintf( debugBuff, __FILE__ ":%d:" message "\r", __LINE__, ##__VA_ARGS__ ); \
							UART_XBee_PutString(debugBuff); } while (0);
#define PRINTF(message, ...) do { sprintf( debugBuff, message , ##__VA_ARGS__ ); \
							UART_XBee_PutString(debugBuff); } while (0);
							
#else //DEBUG_MODE
#define DEBUG_XBEE(v)
#define DEBUG_PRINTF(message, ...)
#define PRINTF(message, ...)
#include <stdio.h>
#endif //DEBUG_MODE

#define p2(v) ((v)*(v))//puissance 2 == au carré

#define TEMPS_RESET 5 // temps niveau des reset de timer (en ms)
					  // doit au moins être de la période de la clk timer

#define HAUTEUR 2000
#define LONGUEUR 3000

#define TAILLE_ROBOT 300
#define TAILLE_ADV 400 //300

#define RAYON_AV 106//110 //105 //103

#define NB_ACTIONS_PAR_MISSION 180
#define NB_OBJECTIFS 20

struct Pos {
	int x;
	int y;
	int alpha;
};
typedef struct Pos Pos;

enum Couleur {BLEU = 0, ROUGE = 1, BLANC};
typedef enum Couleur Couleur; 

enum TypeCmd {ALPHA, DELTA, ALPHA_ABS};
typedef enum TypeCmd TypeCmd;

enum TypeAction {NULL_ACTION, NOP, DEPL, AUTRE_ACTION, XY_POS, XY_POS_LONG, XY_ALPHA_FINAL,
				CALLAGE_DEPL, ATTENTE, GET_POS_DO, SET_A, SET_X_CAL, SET_Y_CAL};
typedef enum TypeAction TypeAction;

enum TypeMission {NULL_MISSION, VERRES, VERRES_4, RAMENE_VERRE, BOUGIES, BOUGIES_FACILES, RATISSAGE,
REPRISE_MISSION, AUTRE_MISSION, CALLAGE_HAUT, CALLAGE_BUFFET, TOUT_DROIT, CADEAUX, CARRE, DEMI_TOURS};
typedef enum TypeMission TypeMission;

struct Depl 
{
	TypeCmd t;
	int val;
};
typedef struct Depl Depl;

struct Action {
	TypeAction t;
	Depl dep;
	void (*proc)(void);
    void (*procInt)(int);
    int argProc;
	Pos pFinal;
	/* fait l'action que une fois que la valeur pointé vaut 0*/
	int * waitFor;
};
typedef struct Action Action;

struct Mission {
	TypeMission t;
	Action tabAction[NB_ACTIONS_PAR_MISSION];
	int numero;
	int fini;
};
typedef struct Mission Mission;

/* Décrit un objectif */ 
struct Objectif {
	TypeMission t;
	/* fait l'objectif quand la valeur pointé vaut 1*/
	int *onlyIf;
	/* arrete l'objectif quand la valeur pointé vaut 1*/
	int *endIf;
	/* arrete l'objectif quand la valeur pointé vaut 1, mais fini l'action en cours*/
	int *endIfSoft;
};
typedef struct Objectif Objectif;

/* Decrit une liste d'objectifs*/
struct Objectifs {
	
	Objectif tab[NB_OBJECTIFS];
	int numero;
	int fini;

};
typedef struct Objectifs Objectifs;


#endif




//[] END OF FILE
