#ifndef CHOIXOBJECTIF_H_INCLUDED
#define CHOIXOBJECTIF_H_INCLUDED

#include "cluster.h"
#include "outils.h"
#include "signaux.h"

// Un facteur mis a zero n'est pas pris pas en compte
// Plus il est grand plus le parametre a de l'importance
// Simple Somme de Fi*Ii
// Les parametres se doivent d etre totalement independants
// le poids le plus faible est pris

#define DEFAUT_F_TEMPS_RESTANT 1
#define DEFAUT_F_ADVERSAIRE 0//4  // En principe 2 fois plus que le nombre de points
#define DEFAUT_F_TEMPS_REMPLISSAGE 1
#define DEFAUT_F_NBPOINTS -2
#define END_MATCH 90000 // millisecondes

#define MARGE_SUPPLEMENTAIRE_REMPLISSAGE_PAR_OBJ 10

#define PRECISION_DISTANCE_CARTO 10
#define NOTRE_VITESSE_MOYENNE 10  // mm par seconde

int calculITempsRestant(Cluster* cluster);
int calculIAdversaire(Cluster* cluster);
int calculITempsRemplissage(Cluster* cluster); // renvoie le temps probable de remplissage
int calculINbPoints(Cluster* cluster);

void choisirObjectif();

extern Cluster objectifCourant;

#endif // CHOIXOBJECTIF_H_INCLUDED
