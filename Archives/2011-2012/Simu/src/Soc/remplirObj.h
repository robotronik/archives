#ifndef REMPLIROBJ_H_INCLUDED
#define REMPLIROBJ_H_INCLUDED

#define X_RETURN 200
#define Y_RETURN 800
#define X_FIN_RETURN 350
#define Y_FIN_RETURN 800

#define X_MIN_TOTEM 802
#define Y_MIN_TOTEM 740
#define X_MAX_TOTEM 2198
#define Y_MAX_TOTEM 1260

#define X_MIN_LOSANGE 1320
#define X_MAX_LOSANGE 1680
#define Y_MIN_LOSANGE 180
#define Y_MAX_LOSANGE 420

#include "cluster.h"
#include "outils.h"
#include "espion.h"
#include "config.h"
#include "choixObjectif.h"
#include "comCarto.h"

#ifdef SIMU
//extern void setLog(int place, wchar_t texte[]);
#endif


struct _EntreeTotem
{
    unsigned char
        U : 1,  // 1 si c'est une entree middle , 0 si on vient d'un cote
        haut : 1,  // 1 si c'est une entree y>1000, 0 sinon
        left : 1,  // 1 si c'est une entree x<1500, 0 sinon
        unused : 5;
};
typedef struct _EntreeTotem EntreeTotem;


void initRemplirObj();

// Fonction pour la machine
void remplirCluster();
    void remplirClusterCircuitTotem();
    void remplirClusterCaleAdverse();
    void remplirClusterLosange();
    void remplirClusterUnique();
void remplirCheminAller(int xa, int ya); // Va remplir la file d'aller a partir de la position courante vers le position specifee
void remplirCheminRetour(int xd, int yd); // Va remplir la file de retour a partir de la position specifie

EntreeTotem totemGetEntree();


extern FileDepl fileDeplGo;
extern FileDepl fileDeplReturn;
extern Depl deplRepos;

#ifdef SIMU
extern void forcerCarto();
#endif

#endif // REMPLIROBJ_H_INCLUDED
