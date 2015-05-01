#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

/*La fonction aStar() renvoi la liste des points de passages. Elle n'eset pas tres optimisé
amélioration possible en vue d'une V2(ou 3) :
- regrouper certaines structures de façon à éviter les parcourts de liste inutiles
- faire de la liste ouverte une liste triées en f (=g+h)
- transformer certaines structures en champs de bits pour économiser la mémoire ~DONE
- transformer la structure Point en un tableau : beaucoup plus rapide et évite
beacoup de parcours de liste DONE*/

/* NOTE : les fonctions suppXxx() sont à supprimer
car elles ne suppriment rien, my_free() libère entierement le
tas 2 (heap2) : DONE*/

/* PointROM permet de placer de maniere astucieuse le tableau de données 
(cf data.c) dans la flash et non dans la RAM*/


#define NB_POINTS 104//112//410
#define NB_FILS 8

#define TAILLE_ADV2 300//300
#define MAILLE 100
#define RACINE_2 1.414

//#define abs(v) (v < 0 ? -v : v)

struct _Point
{
    unsigned long id : 9, x : 12, y : 11; //4 octets
    //ListeFils lPt; // 2 octets, 4 sur psoc (32bits)
	int lPt[NB_FILS];
    //char enable; // 1 octet
};
typedef struct _Point Point;
typedef const struct _Point PointROM;


struct _Set
{
    int id ;
    struct _Set* queue;
};

typedef struct _Set* Set;


struct _Data
{
    int id ;
    int g;
    int h;
    int f;
    struct _Data* queue;
};

typedef struct _Data* Data;

struct _Came
{
    int id ;
    int valIdCame;
    struct _Came* queue;
};

typedef struct _Came* Came;

/*struct _Coor
{
    int id;
    int x;
    int y;
    struct _Coor* queue;
};*/
struct _Coor
{
    unsigned long id:9,x:12,y:11;
    struct _Coor* queue;
};
typedef struct _Coor* Coor;


void addSet(Set* l, int id);
Set removeIdSet(Set l, int id);
int isInSet(Set list , int id);
Data setGscore(int id , int val , Data l);
int getGscore(int id , Data l);
Data setHscore(int id , int val , Data l);
int getHscore(int id , Data l);
Data setFscore(int id , int val , Data l);
int getFscore(int id , Data l);
Came setCameFrom( int id , int val , Came l);
int getCameFrom(int id , Came l);
void addCoor(Coor* l, int id ,int x,int y);
int nbCoor(Coor list);
Coor removeIdCoor(Coor liste, int valeur);
int distBetween( int id1 ,int id2);
int idToXy(PointROM *list, int id, int* x,int* y);
//int abs(int a);
int heuristicCostEstimate( int id1 , int id2 ,  PointROM * list);
int lowestFscore(Set openSet, Data data);
Point sePlacerEn(int id , PointROM * list);
Set reconstruireChemin(Came l,int startId ,int goalId);
Coor setToCoor(Set list , PointROM * points);
int testAligne(int xAv,int yAv,int xMil,int yMil,int xAp,int yAp);
Coor simplifierResultat(Coor list);
#ifdef DEUX_ROBOT_ADV
void loadAdversaire(int x, int y, int x2, int y2, PointROM * list);
#else
void loadAdversaire(int x, int y, PointROM * list);
#endif
int calculLongueur(Coor list);
void calculDepartArrivee(PointROM * list,int xDep,int yDep,int xArr,int yArr,int* startId,int* goalId);
Coor aStar(int xDep, int yDep , int xArr, int yArr, PointROM * list);

#endif // ASTAR_H_INCLUDED
