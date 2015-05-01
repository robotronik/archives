#include "cluster.h"

LesClusters clusters;

void clusterize(EObjectif* objectifs, int nombre)
{
    clusters.nbClusters = 0;

    unsigned char i;
    for(i=0; i<nombre; i++)
    {
        // Si le point est a ajouter
        if(eObjectifATraiter(&(objectifs[i])))
            ajouterEObjectif(&(objectifs[i]), i ,objectifs);
    }
}

void ajouterEObjectif(EObjectif* objectif, unsigned char id, EObjectif* tabObj)
{
    // Calcul du one hot
    U64 oneHot = 0x8000000000000000;
    oneHot = oneHot >> id;

    // Parcours des clusters
    unsigned char cluster = 0xFF;
    unsigned char i;
    for(i=0; i<clusters.nbClusters; i++)
    {
        if(appartientAuCluster(objectif, i, tabObj))
        {
            if(cluster == 0xFF)
            {
                cluster = i;
            }
            else
            {
                fusionnerClusters(cluster, i); // Ne pas oublier de recopier
            }
        }
    }


    // Cas de creation de nouveau cluster
    if(cluster == 0xFF)
    {
        Cluster c;
        c.objectifs = oneHot;
        c.nbObjectifs = 1;
        c.nbPoints = eObjectifToPoint(objectif);
        c.x = objectif->X;
        c.y = objectif->Y;
        c.poids = 0;
        clusters.nbClusters++;
        clusters.v[clusters.nbClusters-1] = c;
    }
    else
    {
        clusters.v[cluster].nbPoints += eObjectifToPoint(objectif);
        clusters.v[cluster].x = (clusters.v[cluster].x * clusters.v[cluster].nbObjectifs + objectif->X)/(clusters.v[cluster].nbObjectifs+1);
        clusters.v[cluster].y = (clusters.v[cluster].y * clusters.v[cluster].nbObjectifs + objectif->Y)/(clusters.v[cluster].nbObjectifs+1);
        clusters.v[cluster].nbObjectifs++;
        clusters.v[cluster].objectifs |= oneHot;
    }

}

char appartientAuCluster(EObjectif* objectif, unsigned char cluster, EObjectif* tabObj)
{
    unsigned char i;
    U64 id = 0x8000000000000000;

    for(i = 0;i<NB_OBJECTIFS; i++)
    {
        if(clusters.v[cluster].objectifs & id)
        {
            if(normeSup(objectif->X, objectif->Y, tabObj[i].X, tabObj[i].Y) <= DISTANCE_CLUSTER)
                return 1;
        }
        id = id >> 1;
    }

    return 0;
}

void fusionnerClusters(unsigned char destination, unsigned char source)
{
    clusters.v[destination].x = (clusters.v[destination].nbObjectifs*clusters.v[destination].x + clusters.v[source].nbObjectifs*clusters.v[source].x)/
                                (clusters.v[source].nbObjectifs + clusters.v[destination].nbObjectifs);
    clusters.v[destination].y = (clusters.v[destination].nbObjectifs*clusters.v[destination].y + clusters.v[source].nbObjectifs*clusters.v[source].y)/
                                (clusters.v[source].nbObjectifs + clusters.v[destination].nbObjectifs);

    clusters.v[destination].nbObjectifs += clusters.v[source].nbObjectifs;
    clusters.v[destination].nbPoints += clusters.v[source].nbPoints;
    clusters.v[destination].objectifs |= clusters.v[source].objectifs;

    // On recopie maintenant
    clusters.nbClusters--;
    unsigned char i;
    for(i=source; i<clusters.nbClusters; i++)
    {
        clusters.v[i] = clusters.v[i+1];
    }
}

char eObjectifATraiter(EObjectif* objectif)
{
    switch(objectif->type)
    {
        #ifdef MAP_ENABLE
        case 5 :
        #endif
        case 4 : case 6 : return 0;
        default : break;
    }

    // On regarde si on est dans la zone bonne
    if(objectif->grabbed || estDansZone(objectif->X,objectif->Y)) // Cale
        return 0;
    // On regarde si on n'est pas dans une zone interdite ou inaccessible
    if(estDansCapitainerieAdverse(objectif->X,objectif->Y) || estDansCaleInaccessibleAdverse(objectif->X, objectif->Y))
        return 0;
    return 1;
}

