#include "choixObjectif.h"

static int f_tempsRestant = DEFAUT_F_TEMPS_RESTANT;
static int f_adversaire = DEFAUT_F_ADVERSAIRE;
static int f_tempsRemplissage = DEFAUT_F_TEMPS_REMPLISSAGE;
static int f_nbPoints = DEFAUT_F_NBPOINTS;

Cluster objectifCourant;

void choisirObjectif()
{
    // On fait les calculs pour chaque objectif
    int totalITempsRestant = 0;
    int totalIAdversaire = 0;
    int totalITempsRemplissage = 0;
    int totalINbPoints = 0;

    unsigned char i;
    for(i=0; i<clusters.nbClusters; i++)
    {
        // Les infos
        /// TODO FAIRE UN MIN SI L'ADV A DEUX ROBOTS
        clusters.v[i].tempsAccesAdverse = getChemin(clusters.v[i].x, clusters.v[i].y, 1);
        clusters.v[i].tempsAcces = getChemin(clusters.v[i].x, clusters.v[i].y, 0);


        totalIAdversaire += calculIAdversaire(&(clusters.v[i]));
        totalITempsRemplissage += calculITempsRemplissage(&(clusters.v[i]));
        totalITempsRestant += calculITempsRestant(&(clusters.v[i]));
        totalINbPoints += calculINbPoints(&(clusters.v[i]));
    }

    if(totalIAdversaire == 0)
        totalIAdversaire = 1;
    if(totalITempsRemplissage == 0)
        totalITempsRemplissage = 1;
    if(totalITempsRestant == 0)
        totalITempsRestant = 1;
    if(totalINbPoints == 0)
        totalINbPoints = 1;

    unsigned char retour = 255;

    for(i=0; i<clusters.nbClusters; i++)
    {
        clusters.v[i].poids = 0;
        if(totalITempsRestant)
            clusters.v[i].poids += (f_tempsRestant*(clusters.v[i].i_tempsRestant * 100))/totalITempsRestant;
        if(totalIAdversaire)
            clusters.v[i].poids += (f_adversaire*(clusters.v[i].i_adversaire * 100))/totalIAdversaire;
        if(totalITempsRemplissage)
            clusters.v[i].poids += (f_tempsRemplissage*(clusters.v[i].i_tempsRemplissage * 100))/totalITempsRemplissage;
        if(totalINbPoints)
            clusters.v[i].poids += (f_nbPoints*(clusters.v[i].i_nbPoints * 100))/totalINbPoints;
        if(retour == 255 || clusters.v[retour].poids > clusters.v[i].poids)
            retour = i;
    }

    // On copie retour
    objectifCourant = clusters.v[retour];
    signaux.objectifDispo = 1;
}

int calculITempsRestant(Cluster* cluster)
{
    if(((cluster->i_tempsRemplissage+PRECISION_DISTANCE_CARTO) / NOTRE_VITESSE_MOYENNE)/1000 > (END_MATCH - getTime())) // Si on peut pas l'atteindre a temps
        cluster->i_tempsRestant = 1;
    else
        cluster->i_tempsRestant = 0;

    return cluster->i_tempsRestant;
}

int calculIAdversaire(Cluster* cluster)
{

    // On prend le temps d'acces a l objectif
    // on le compare au temps d acces a l objectif par l adversaire

    /// TODO
    // Si l'objectif est celui du robot adverse
	// Prendre en compte la VMax du robot adverse
    if(0)
    {
        if(cluster->tempsAccesAdverse < cluster->tempsAcces + PRECISION_DISTANCE_CARTO)
        {
            cluster->i_adversaire = 0; // On peut l'avoir avant lui

        }
        else
        {
            cluster->i_adversaire = 10;  // L'adversaire atteint l'objectif avant nous
        }
    }
    else
    {
        if(cluster->tempsAccesAdverse < cluster->tempsAcces + PRECISION_DISTANCE_CARTO)
        {
            cluster->i_adversaire = 3; // On peut l'avoir avant lui

        }
        else
        {
            cluster->i_adversaire = 8;  // L'adversaire peut atteindre l'objectif avant nous
        }
    }

    return cluster->i_adversaire;
}

int calculITempsRemplissage(Cluster* cluster)
{
    // Temps d'acces + si au moins un jeton : temps de retour
    cluster->i_tempsRemplissage = cluster->tempsAcces;

    /// TODO, prendre en compte le fait que ce ne soit pas un jeton
    if(1) // Si il y a un jeton
    {
        cluster->i_tempsRemplissage += cluster->nbObjectifs*MARGE_SUPPLEMENTAIRE_REMPLISSAGE_PAR_OBJ;

        /// TODO : Ajouter le retour, si il y a de quoi

    }

    return cluster->i_tempsRemplissage;
}

int calculINbPoints(Cluster* cluster)
{
    cluster->i_nbPoints = cluster->nbPoints;

    return cluster->i_nbPoints;
}
