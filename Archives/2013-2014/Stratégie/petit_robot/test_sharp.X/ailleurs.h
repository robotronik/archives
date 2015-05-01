
#ifndef AILLEURS_H
#define AILLEURS_H


#include "main.h"


/** \file main.h
 * \brief Contient tout ce qui doit être inclus dans tout le projet.
 * \note Doit être inclu par tout les fichiers compilés.
 */

/** À exécuter quand le robot est prêt. Actuellement ne fait rien.
 * \note Sans doute à virer
 */
void set_ready();

/* Attend que le match démarre
 */
void wait_match_start();

/* Renvoie vrai si le match est terminé
 */
bool timeEnd();

/* Renvoie vrai si l’espace derière le robot est dégagé
 */
bool personneDerriere();

/* Renvoie vrai si l’espace devant le robot est dégagé
 */
bool personneDevant();

/* Renvoie vraie si les bumpers détecte une colision (par exemple pour tester si le robot est callé
 * \todo renommer en estEnColision()
 */
bool colision();

/* Donne la position en absolue de l’angle du robot en degré
 * \todo Quelle est la référence ? sur la droite, comme en trigo ?
 */
Angle getAlphaAbs();

/* Renvoie la position sur l’axe x du robot
 * \todo Fusionner avec getPosY()
 */
Coordonne_mm getPosX();

/* Renvoie la position sur l’axe y du robot
 * \todo Fusionner avec getPosX()
 */
Coordonne_mm getPosY();

/* Renvoie vraie si l’assert à terminée son job ! (le robot à atteind sa destination
 */
bool assertAtteint();



#endif /* AILLEURS_H */
