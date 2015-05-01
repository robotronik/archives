#ifndef MAIN_H
#define MAIN_H

#include <p33FJ128MC802.h>
#include "mapping.h"

/** Utilisé dans le code de l’UART
 * \note je ne sais pas trop à quoi ça sert (robin, 2014)
 */
#define INT_UART_TX


/** \file main.h
 * \brief Contient tout ce qui est à inclure globalement dans le projet
 * \note est à inclure dans tout les fichiers compilé du projet
 */

/** Compatibilitée avec le C89…
 */
#ifndef bool
typedef enum {false, true} bool;
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
    DISTANCE_CALLAGE   = 100,
    SORTIR_ZONE_DEPART = 500
} COORDONNEES_2014_mm;

/** Les couleurs de départ
 */
typedef enum {
    JAUNE,
    ROUGE,
    COULEUR_DEPART,
    COULEUR_OPPOSE
} Couleur;

/** Sens du mouvement
 *  \note les valeur 0 et 1 sont volontaire pour pouvoir faire !SensMvt
 */
typedef enum {
    MARCHE_AVANT = 0,
    MARCHE_ARRIERE = 1
} SensMvt;

/** Type de mouvement pour le robot
 */
typedef enum {
    AVANCER = MARCHE_AVANT,     ///< côté plat en avant
    RECULER = MARCHE_ARRIERE,   ///< pointe en avant
    BOUGER,                     ///< sens pour avancer indiférent
    CALLER,                     ///< pour caller le robot contre un mur et mettre à jour ses coordonées
    ROTATION_HORAIRE,
    ROTATION_ANTI_HORAIRE
} TypeMvt;

/** Vitesse de déplacement
 */
typedef enum {
    RAPIDE,
    LENT
} TypeVitesse;

/** fréquence du processeur
 * \note Déterminé par init_osc()
 */
#define FREQUENCE_CLOCK 79227500

//////////////////
/*  Structures  */
//////////////////

typedef struct {
    int x;
    int y;
    int alpha;
} Pos;

/** Un objectif à atteindre
 */
typedef struct {
    TypeMvt typeMvt;            ///< Type de mouvement (avancer, reculer, ou quelconque)
 //   TypeVitesse TypeVitesse;    ///< Vittesse de déplacement
 //   void (*mvt) (Coordonne_mm x, Coordonne_mm y);           ///< Fonction de mouvement à appliquer (ex : avancer,
 //                                                           ///< reculer, bouger, caller, ...)
    Coordonne_mm nextX;         ///< Coordonnées du point à atteindre par rapport à la couleur de référence
    Coordonne_mm nextY;
} Objectif;

/** Les information sur le mouvement actuel
 */
typedef struct {
    SensMvt sensMvt;
    bool    enEvitement;
} InfoMvt;

#endif /* MAIN_H */
