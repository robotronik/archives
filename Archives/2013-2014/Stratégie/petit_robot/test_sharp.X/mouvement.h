#ifndef MOUVEMENT_H
#define MOUVEMENT_H

/** \file mouvement.h
 *  \brief Contient les fonctions qui permettent de controler les déplacement du robot.
 *  \note fait le lien avec la carte asser
 *  \todo Les fonctions bouger, avancer et reculer sont trop compliquée et devraient être simplifié
 */

#include "main.h"
#include "ailleurs.h"

/** Permet de réinitialiser les coordonnées du robot.
 * Avance le robot sur une courte distance jusqu'à ce qu'il rentre en butée puis enregistre les coordonnées.
 * de la nouvelle position dans l'asser.
 */
void caller(Coordonne_mm postCallageX, Coordonne_mm postCallageY);

/** Déplace le robot en direction d'une point en dont les coordonnées sont absolues par rapport à la table.
 */
void bouger(Coordonne_mm x, Coordonne_mm y);

/** Déplace le robot en direction d'une point en dont les coordonnées sont absolues par rapport à la table.
 */
void avancer(Coordonne_mm x, Coordonne_mm y);

/** Déplace le robot en direction d'une point en dont les coordonnées sont absolues par rapport à la table.
 */
void reculer(Coordonne_mm x, Coordonne_mm y);

/// Angle maximum considéré comme étant valide
#define ANGLE_MAX 1

#endif /* MOUVEMENT_H */

