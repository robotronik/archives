#ifndef MAIN_H
#define MAIN_H

/** \file main.h
 * \brief Contient tout ce qui est à inclure globalement dans le projet
 * \note est à inclure dans tout les fichiers compilé du projet
 */

/** Compatibilitée avec le C89…
 */
#ifndef bool
typedef enum {false, true} bool;
#endif

/** \todo À déplacer ou il y en a besoin */
#include <math.h> // pour les calculs d'angles
#ifndef PI
#define PI 3.1415
#endif


typedef int Coordonne_mm;       ///< Coordonnées en mm
typedef int Distance_mm;        ///< Distance en mm
typedef float Angle;            ///< Angle en degré

/** Coordonnées utiles de la coupe de 2014
 * \note Le coin de référence est en haut à gauche
 * \todo distances à définir
 */
typedef enum {
    // donnees du robot
    COTE    = 88,
    AVANT   = 60,
    ARRIERE = 95,
    // uniquement en x
    EST     = 3000,
    OUEST   = 0,
    COIN_ROUGE = OUEST,
    COIN_JAUNE = EST,
    // uniquement en y
    NORD    = 0,
    SUD     = 2000,
    BAC     = 295 + AVANT,
    DEPART  = 600,
    // commun
    COIN    = 370,
    ARBRE   = 275,
    // mouvements
    DISTANCE_EVITEMENT = 00000000000000000000000000000000000000000000000,
    DISTANCE_CALLAGE   = 00000000000000000000000000000000000000000000000,
    SORTIR_ZONE_DEPART = 00000000000000000000000000000000000000000000000
} COORDONNEES_2014_mm;

/** Les couleurs de départ
 */
typedef enum {
    JAUNE,
    ROUGE,
    COULEUR_DEPART,
    COULEUR_OPPOSE
} Couleur;

/** Type de mouvement pour le robot
 */
enum typeMvt {
    AVANCER,            /// < côté plat en avant
    RECULER,            /// < pointe en avant
    BOUGER,             /// < sens pour avancer indiférent
    CALLER              /// < pour caller le robot contre un mur et mettre à jour ses coordonées
};
typedef enum typeMvt typeMvt;

/** Vitesse de déplacement
 */
typedef enum {
    RAPIDE,
    LENT
} TypeVitesse;

/** Un objectif à atteindre
 */
typedef struct {
    typeMvt typeMvt;            /// < Type de mouvement (avancer, reculer, ou quelconque)
    TypeVitesse TypeVitesse;    /// < Vittesse de déplacement
    void (*mvt) (Coordonne_mm x, Coordonne_mm y);            /// < Fonction de mouvement à appliquer (ex : avancer, reculer, bouger, caller, ...)
    Coordonne_mm nextX;            /// < Coordonnées du point à atteindre par rapport à la couleur de référence
    Coordonne_mm nextY;
} Objectif;


///////////////////////////////////////////////////////
//  TOUT CE QUI SUIT PROVIENT DES ANNÉES PRÉCÉDANTES //
///////////////////////////////////////////////////////


#include <p33FJ128MC802.h>
#include "comAsser.h"
#include "init.h"

#define INT_UART_TX
#define LARGEUR 324                             // [mm] distance entre roues codeuses

typedef struct
{
int type; //0=alpha, 1=delta
double valeur;
}Action;


int abs(int n);
void verifierPosition();
void verifierSharp();
void faire(Action mouvement);
void machine();
int dansTable(int,int);
// Structure associant 2 shorts dans un long pour les QEI
typedef union {
    long all;
    struct {
        short low;
        short high;
    } part;
} T_dividedLong;

typedef union {
    unsigned long all;
    struct {
        short low;
        unsigned short high;
    } part;
} T_dividedULong;

 #include "debug.h"
 #include "init.h"
 #include "uart.h" // couche basse, autonome pour ne pas surcharger le reste

struct Pos {
    int x;
    int y;
    int alpha;
};
typedef struct Pos Pos;

void aller(Pos position);

#endif /* MAIN_H */
