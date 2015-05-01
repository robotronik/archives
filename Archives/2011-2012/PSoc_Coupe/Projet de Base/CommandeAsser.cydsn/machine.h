#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "config.h"

/*
    La machine a etats du cerveau
*/

#ifdef SIMU
#include "../Soc/espion.h"
#endif

#include "cluster.h"
#include "comAsser.h"
#include "comCarto.h"
#include "choixObjectif.h"
#include "remplirObj.h"
#include "signaux.h"

#ifdef SIMU
//extern void setLog(int place, wchar_t texte[]);
#endif

enum _StateMouv
{
    IDLE, DEPL_GO, WAIT_END_DEPL_GO, DEPL_RETURN, WAIT_END_DEPL_RETURN,
    BLOCKED, DEPL_REPOS, RECALCUL_RETURN, WAIT_END_DEPL_REPOS, FIN_MATCH
};
typedef enum _StateMouv StateMouv;

enum _StateMind
{
    SPY1, SPY2, CLUSTER, CHOIX_OBJ, REMPLIR_OBJ, CALCUL_REPOS, CALCUL_RETOUR
};
typedef enum _StateMind StateMind;


void initPssoc();

// Fonction dans la boucle while infinie
void runMachine();

// Chaque etat correspond une fonction
    // Mouv
void stateIdle();
void stateDeplGo();
void stateWaitEndDeplGo();
void stateDeplReturn();
void stateWaitEndDeplReturn();
void stateBlocked();
    // Mind
void stateSpy1();
void stateSpy2();
void stateCluster();
void stateChoixObj();
void stateRemplirObj();

#ifdef SIMU
void afficherAutomate();
#endif

void sendEtat();



#endif // MACHINE_H_INCLUDED
