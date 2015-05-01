#ifndef COORDONEES_H
#define COORDONEES_H

#include "main.h"

/** \file coordonées.h
 * \brief Contient des fonctions relative aux coordonées de la table
 * \todo Sans doute à renommer ou réorganiser
 */

/** Renvoi la position en absolu que l'ont souhaite atteindre à partir d'une position relative.
 *  \param c La couleur qui sert de côté de référence.
 *  \see cOppose
 */
Coordonne_mm calculX(Couleur c, Coordonne_mm origine, Coordonne_mm relativeX);

/** Renvoie la couleur opposé à celle du départ.
 *  \note On pourrait tout à fait utiliser la syntaxe "!nextX" mais qui est moins lisible.
 *  \note La même chose sous forme de macro pour optimisation : #define C_OPPOSE(c) ((c) == JAUNE) ? ROUGE : JAUNE)
 */
Couleur cOppose (Couleur c);


#endif /* COORDONEES_H */
