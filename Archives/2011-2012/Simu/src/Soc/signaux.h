#ifndef SIGNAUX_H_INCLUDED
#define SIGNAUX_H_INCLUDED

#include "config.h"

struct _Signaux
{
    unsigned short
        besoinObj : 1, // 0 by blocked
        mouvRepos : 1,
        calculRetour : 1,
        objectifDispo : 1, // 0 by blocked
        blocked : 1,
        warnBlocked : 1,
        pathReceived : 1,
        finMouvement : 1,  // On a recu un DONE de l'asser
		posRecue : 1,
		timerOn : 1,
        unused : 6;
};
typedef struct _Signaux Signaux;

extern Signaux signaux;

struct _Testeur
{
    unsigned char
        test : 1,  // 1, on est en mode test
        asserOk : 1, // 1 si Ok
        cartoOk : 1,
        tourelleOk : 1,
        unused : 5;
};
typedef struct _Testeur Testeur;

void initSignaux();
void beginTest();
char isTest();
void endTest();

void validerAsser();
void validerCarto();
void validerTourelle();

#endif // SIGNAUX_H_INCLUDED
