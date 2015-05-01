#ifndef CST_H_INCLUDED
#define CST_H_INCLUDED


#define RED 				0x83
#define PURPLE				0x84

#define NB_JETONS 45  // Nombre de jetons
#define NB_OBJECTIFS NB_JETONS  // Nombre de jetons

#define X_DEBUT 250
#define Y_DEBUT 1750

#define NB_DEPL_MAX 30

#define PRECISION_DEFAULT_PRECIS 20
#define PRECISION_DEFAULT_ALPHA_PRECIS 5
#define PRECISION_CARTO 50
#define PRECISION_CIRCUIT 5

#define TEST_CONNEXION


// Adressage tourelle

#define INDEX_TOURELLE_ADV_1 0
#define nombreTourelles 1
extern const unsigned char adrTourelles[nombreTourelles];

/*
    Peut changer au cours du temps pour l'accelerometre
    en mm
    /!\ si on remappe
*/
#define PRECISION_ACCELEROMETRE 100 // Au cm


#include "config.h"
typedef unsigned long long int U64;

typedef struct
{
    U64 objectifs;   // 0 : present, 1 : absent
    char nbObjectifs;
    int nbPoints;
    unsigned int
        x : 16,
        y : 16;
    int tempsAcces;
    int tempsAccesAdverse;

    int i_tempsRestant;
    int i_adversaire;
    int i_tempsRemplissage;
    int i_nbPoints;
    int poids;
} Cluster;

struct _LesClusters
{
    unsigned char nbClusters;
    Cluster v[NB_OBJECTIFS];
};
typedef struct _LesClusters LesClusters;

typedef struct EObjectif {
    struct
    {
        unsigned int
        type : 3,   // 1 : jeton, 2 : lingot, 3 : bouteille a nous 4 : bouteille a eux, 5 : carte a nous, 6 : carte a eux
        realise : 1,
        X : 12, // 12 bits
        Y : 12, // 11 bits
        Z : 2, // 2 bits necessaires
        grabbed : 2;   // 0 -> non, 1 -> nous, 2 -> adverse
    };
} EObjectif;
// File pour le deplacement
enum _TypeDepl
{
    DEPL_RAPIDE, DEPL_COURBE, DEPL_ALPHA, RECALC
};
typedef enum _TypeDepl TypeDepl;

struct _Depl
{
    TypeDepl typeDepl;
    union
    {
        struct
        {
            int x;
            int y;
        };
        int alpha;
    };
    unsigned int precision;
};
typedef struct _Depl Depl;

struct _FileDepl
{
    Depl file[NB_DEPL_MAX];
    unsigned char debut;
    unsigned char taille;
};
typedef struct _FileDepl FileDepl;

struct trData
{
	unsigned long
		y:11, //poids faible
		x:12,
		numero:6,
		code:3; //poids fort
};
typedef struct trData trData;

struct trDeb
{
	unsigned long
		longueur:16, //poids faible
		numero:6,
		nb:6,
		code:4; //poids fort
};
typedef struct trDeb trDeb;

struct trChar
{
	unsigned long
		oooX:8, //poids faible
		ooXo:8,
		oXoo:8,
		Xooo:8; //poids fort
};
typedef struct trChar trChar;

struct trRecep
{
	unsigned long
		y:11, //poids faible
		x:12,
		numero:6,
		l:1,
		d:1,
		r:1; //poids fort
};
typedef struct trRecep trRecep;

union Trame
{
	trChar charx4;
	trRecep recep;
	trData data;
	trDeb  first;
	unsigned long l;
};
typedef union Trame Trame;

#endif // CST_H_INCLUDED
