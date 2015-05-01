#ifndef COMCARTO_H_INCLUDED
#define COMCARTO_H_INCLUDED

#include "cst.h"
#include "config.h"
#ifdef SIMU
#include "../Carto/help_main.h"
#endif
#include "outils.h"
#include "espion.h"
#include "signaux.h"

#ifndef SIMU
#define sendMessageCarto(m) UART_Carto_PutChar(m)
#endif

#define TAILLE_CACHE 16
#define NB_RECEP_MAX 10

//#define DEUX_ROBOTS_ADVERSES

#define TIME_OUT 1000  // Une seconde c'est trop

struct _EntreeCache
{
    unsigned int x;
    unsigned int y;
    unsigned int longueur;
};
typedef struct _EntreeCache EntreeCache;

struct _Cache
{
    unsigned char courant;
    EntreeCache v[TAILLE_CACHE];
};
typedef struct _Cache Cache;


#ifdef SIMU
extern void sendMessageCarto(unsigned char c);
#endif

/*
    Remplit le cache
    Debute la boucle infinie de rafraichissement
*/
void initComCarto();
void startRefreshCarto();

// Appele par le traitant
void traiterMessageCarto(unsigned char m);
void traiterTrame(Trame t);

// Intelligent
// Adversaire est pour le moment un booleen
/// TODO
unsigned int getChemin(int xa, int ya, char adversaire);

// Physique
// Appelee automatiquement
void askChemin(unsigned int xd, unsigned int yd, unsigned int xa, unsigned int ya, unsigned int xAdv, unsigned int yAdv);
// Non appelee automatiquement
// Cette fonction doit etre quasi bloquante, avec timeout et tout et tout
// car elle ne doit etre appelee qu'elle n a pas de file
// La reception ne rattrape pas les erreurs de la carto du nombre de trames
void askLongueurChemin(unsigned int xd, unsigned int yd, unsigned int xa, unsigned int ya, unsigned int xAdv, unsigned int yAdv);

void renvoyerDerniereCommande();

void testCarto();

// Fonctions de file
void pushFileDepl(FileDepl* file, Depl entree);
Depl popFileDepl(FileDepl* file);
char isEmptyFileDepl(FileDepl file);
char isFullFileDepl(FileDepl file);
void viderFile(FileDepl* file);
void addRecepToDepl(FileDepl* depl);

extern FileDepl fileRecepCarto;

#endif // COMCARTO_H_INCLUDED
