
#ifndef main_a_moi
#define main_a_moi


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>



// inclusion des headers pour compréhension des fonctions externes








// valeurs a modifiées au besoin

#define TAILLE_TABLE_LARGEUR     300
#define TAILLE_TABLE_HAUTEUR     210
#define degre                   180/3.14159265358979323846
#define rad                     3.14159265358979323846/180
#define COULEUR_BASE_PION 13158600
#define NBR_CONFIG 99
#define NBR_STRAT 2




// enumeration des types d'instruction
enum {AVANCER, TOURNER, LIBERER_AVANT, FERMER_AVANT, LIBERER_ARRIERE, FERMER_ARRIERE};

// enumeration de la couleur de depart
enum {ROUGE, BLEU};

enum {HAUT, BAS, GAUCHE, DROITE};

enum {TRUE, FALSE};




// strucutre pour utilisation de coordonnées

typedef struct Coord
{
float x;
float y;
} Coord ;


// structure de memorisation des cases de la grille pour la stratégie

typedef struct Case
{
    Coord position;
    int couleur;
    int occupe;
} Case;

// structure définissant les différent informations sur le robot

typedef struct Robot
{
    float direction_robot;
    SDL_Surface *robot;
    Uint32 couleur;
    Coord position_robot;
    Coord angle_1;
    Coord angle_2;
    Coord angle_3;
    Coord angle_4;
    Coord angle_inter_1;
    Coord angle_inter_2;
    Coord angle_inter_3;
    Coord angle_inter_4;
    Coord arc_cercle_1_1;
    Coord arc_cercle_1_2;
    Coord arc_cercle_2_1;
    Coord arc_cercle_2_2;
    Coord direction_1;
    Coord direction_2;
    Coord centre_arc_1;
    Coord centre_arc_2;
    Coord pince_avant_1;
    Coord pince_avant_2;
    Coord pince_arriere_1;
    Coord pince_arriere_2;
    float rayon_arc;
    int detection_avant_inter;
    int detection_arriere_inter;
    int saisie_avant_ok;
    int saisie_arriere_ok;
    int temp_detection_avant;
    int temp_detection_arriere;
} Robot ;

// structure définissant les différent informations sur les pions

typedef struct Elements
{
    Coord position;
    Uint32 couleur;
} Elements ;

// structure codant les instructions données aux robots

typedef struct Inst
{
double valeur;
int signe;
double vitesse;
int type;
} inst;

// structure pour la liste

typedef struct element element;
struct element
{
    inst val;
    struct element *nxt;
};
typedef element* llist;

void finAppli();
void machine();

# include "affichage_dynamique.hpp"
# include "debug.hpp"
# include "deplacement_robot.hpp"
# include "elements.hpp"
//# include "fichier_liste_inst.hpp"
# include "generation_table.hpp"
//# include "liste_instruction.hpp"
# include "physique.hpp"
# include "saisies_utilisateur.hpp"
//# include "strategie.hpp"
//# include "outils_strategie.hpp"
# include "machine.hpp"
# include "controlRobot2.hpp"

#endif

