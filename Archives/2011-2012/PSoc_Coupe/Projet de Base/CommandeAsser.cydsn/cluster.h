#ifndef CLUSTER_H_INCLUDED
#define CLUSTER_H_INCLUDED

#include "cst.h"
#include "config.h"
#include "outils.h"

#include "espion.h"

#define DISTANCE_CLUSTER 200

void clusterize(EObjectif* objectifs, int nombre); // On envoie un tableau de taille nombre
void ajouterEObjectif(EObjectif* objectif, unsigned char id, EObjectif* tabObj);
char appartientAuCluster(EObjectif* objectif, unsigned char cluster, EObjectif* tabObj);
void fusionnerClusters(unsigned char destination, unsigned char source);
char eObjectifATraiter(EObjectif* objectif);

extern LesClusters clusters;

int calculITempsRestant(Cluster* cluster);

#endif // CLUSTER_H_INCLUDED
