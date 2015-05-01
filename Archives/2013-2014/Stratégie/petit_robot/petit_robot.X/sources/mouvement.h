#ifndef MOUVEMENT_H
#define MOUVEMENT_H

/** \file mouvement.h
 *  \brief Contient les fonctions qui permettent de controler les déplacement du robot.
 *  \note fait le lien avec la carte asser
 */

/** On prend en compte l'erreur ERR_SECUR_XY de l'asser pour ne pas gérer inutilement des angles zarbs ou de petits déplacements.
 */
#define ERR_SECUR_XY 20

/** Angle minimal de rotation possible, donc pas de rotation si on est trop proche de l'angle de la cible
	\todo Tester l'angle min, ici définie au pif.
*/
#define ROTATION_MIN 2

/* Renvoie vraie si les bumpers détecte une colision (par exemple pour tester si le robot est callé
 * \todo renommer en estEnColision()
 */
bool colision();

/* Renvoie vraie si les bumpers détecte une colision (par exemple pour tester si le robot est callé
 * \todo renommer en estEnColision()
 */
bool colision();

/** Permet de réinitialiser les coordonnées du robot.
 * Avance le robot sur une courte distance jusqu'à ce qu'il rentre en butée puis enregistre les coordonnées et l'angle
 * de la nouvelle position dans l'asser.
 */
void caller(Coordonne_mm postCallageX, Coordonne_mm postCallageY);

/** Déplace le robot en direction d'un point dont les coordonnées sont absolues par rapport à la table.
 */
void bouger(Coordonne_mm x, Coordonne_mm y, TypeMvt mouvement);


#endif /* MOUVEMENT_H */

