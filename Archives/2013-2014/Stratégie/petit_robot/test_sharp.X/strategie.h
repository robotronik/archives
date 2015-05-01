#ifndef STRATEGIE_H
#define STRATEGIE_H

#include "main.h"

/** \file strategie.h
 *  \brief S’occupe de la statégie du robot. Les objectifs sont défénis dans un tableau d’objectifs
 *  \todo La stucture objectif devrait être défini dans ce fichier
 */

/** Stratégie d'évitement d'obstacle.
 * \warning Cette fonction est actuellement blocante.
 */
void evitementObstacle();

/** Cherche le prochain objectif et débute le déplacement vers lui
 * Le prochain objectif est soit l'objectif en cour si il y a eu une interruption de parcourt (tel qu'un obstactle),
 * soit l'objectif suivant.
 * \note Vérifie qu'il reste des objectifs.
 */
void chercheObjectif(Objectif obj[], int nbObj, Couleur couleur);

/** Toute la statégie du petit robot.
 */
void parcourt(Objectif objPetitRobot[], int nbObj, Couleur couleur);


#endif /* STRATEGIE_H */
