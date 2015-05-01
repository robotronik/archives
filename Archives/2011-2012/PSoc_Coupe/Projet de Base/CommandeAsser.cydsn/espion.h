#ifndef ESPION_H_INCLUDED
#define ESPION_H_INCLUDED

#include "config.h"

#include "cst.h"
#include "recTourelle.h"
#include "comAsser.h"


/// TODO
#define PORTEE_BLOQUANTE 600  // distance avec un robot adverse qui nous bloque : changer en ; (Treaction - Vcourante/Coeff_Rampe)*Vcourante
#define DISTANCE_MIN2 precisionTourelle[0]*precisionTourelle[0] // Distance au CARRE a laquelle on considere que le robot adverse prend l'objectif
#define V_MIN 1000000 // Vitesse au carre suffisante pour dire que le robot adverse est en mouvement (mm/s)2
#define MEMOIRE_CHEMIN 32
#define PORTEE_ADVERSE 300*300 // Au carre
#define PROXIMITE_ADVERSE (PORTEE_BLOQUANTE*PORTEE_BLOQUANTE)
#define PROXIMITE_ADVERSE_FOR_COMP PROXIMITE_ADVERSE/100  // repassage au cm


#define X_ROBOT_ADVERSE eRobotAdverse.X
#define Y_ROBOT_ADVERSE eRobotAdverse.Y

#include "config.h"
#include "outils.h"


#ifdef SIMU
    #include <cstdio>
    #include <cstdlib>
    #include <fstream>
#endif


typedef enum
{
    GO_OBJ, GO_BASE, GO_OUR_BASE, GO_US, BLOCK_US, DEPOSE, GRAB, GARDE, BLOCK
} TypeStateAdverse;

typedef struct
{
    char charge; // Possede un jeton
    int data;

} DataState;

typedef struct {
	int X;
	int Y;
	int XDir;
	int YDir;
	long long int VMax2; // fixe par recTourelle
	long long int t; // 41 bits necessaires ? voir recTourelle /// TODO A VERIFIER
	TypeStateAdverse state;
	DataState data;

	int XOri; // ¨Pour les macros mouvements
	int YOri;
	int XDirOri;  // Le precedent
	int YDirOri;

} RobotAdverse;

typedef struct {
	int X;
	int Y;
	int XDir;
	int YDir;
	long long int t; // 41 bits necessaires ? voir recTourelle /// TODO A VERIFIER
	DataState data;

	int XOri; // ¨Pour les macros mouvements
	int YOri;
	int XDirOri;  // Le precedent
	int YDirOri;

} RobotANous;

RobotAdverse getERobotAdverse();

void initEspion();
void espion();

void traiterNouvellePosAdverse(EntreeFilePos pos);

/// TODO
void traiterNouvellePosANous(unsigned int x, unsigned int y);


void initRobotAdverse();
void updateRobotAdverse(EntreeFilePos pos);
void updateStateRobotAdverse(RobotAdverse* suivant);
char changementDirOrigine(RobotAdverse suivant);
char doGrabAdverse(int x, int y);
void deposerJeton(int id);

char estDansZone(int x, int y);
char estProcheZone(int x, int y);
char estDansZoneAdverse(int x, int y);
char estDansCapitainerieAdverse(int x, int y);
char estDansCaleInaccessibleAdverse(int x, int y);
char estProcheZoneAdverse(int x, int y);
char estProcheDeNous(int x, int y);

int eObjectifToPoint(EObjectif* objectif);
void genJeton();

void repositionnerJetons();

extern RobotAdverse eRobotAdverse;

#ifdef SIMU
void afficher_espion();
void afficher_state(TypeStateAdverse state);
#endif

extern EObjectif eObjectifs[NB_OBJECTIFS];
extern int xRobot;
extern int yRobot;

#endif // ESPION_H_INCLUDED
