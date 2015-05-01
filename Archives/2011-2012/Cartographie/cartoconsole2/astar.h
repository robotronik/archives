#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

/* NOTE : les fonctions suppXxx() sont à supprimer
car elle ne supprime rien, my_free() libère entierement le
tas 2 (heap2)*/

#include "malloc.h"

#define TAILLE_ROBOT 300
#define TAILLE_ADV 300
#define MAILLE 100

struct _ListeFils
{
    int id ;
    struct _ListeFils* queue;
};
typedef struct _ListeFils* ListeFils;

struct _Point
{
    int x ;
    int y ;
    int id ;
    ListeFils lPt;
    char enable;
    struct _Point* queue;
};

typedef struct _Point* Point;



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

struct _Coor
{
    int id;
    int x;
    int y;
    struct _Coor* queue;
};

typedef struct _Coor* Coor;


void addListeFils(ListeFils* l, int id);
void suppListeFils(ListeFils list);
void addPoint(Point* l, int x,int y, int id, ListeFils lPt);
//void suppPoint(Point list);
//Point setEnable(int id , int val,Point list);
void addSet(Set* l, int id);
//void suppSet(Set list);
Set removeIdSet(Set l, int id);
int isInSet(Set list , int id);
//void suppData(Data list);
Data setGscore(int id , int val , Data l);
int getGscore(int id , Data l);
Data setHscore(int id , int val , Data l);
int getHscore(int id , Data l);
Data setFscore(int id , int val , Data l);
int getFscore(int id , Data l);
Came setCameFrom( int id , int val , Came l);
int getCameFrom(int id , Came l);
//void suppCame(Came list);
void addCoor(Coor* l, int id ,int x,int y);
//void suppCoor(Coor list);
int nbCoor(Coor list);
Coor removeIdCoor(Coor liste, int valeur);
int distBetween( int id1 ,int id2);
int idToXy(Point list, int id, int* x,int* y);
int abs(int a);
int heuristicCostEstimate( int id1 , int id2 ,  Point list);
int lowestFscore(Set openSet, Data data);
Point sePlacerEn(int id , Point list);
Set reconstruireChemin(Came l,int startId ,int goalId);
Coor setToCoor(Set list , Point points);
int testAligne(int xAv,int yAv,int xMil,int yMil,int xAp,int yAp);
Coor simplifierResultat(Coor list);
Point loadAdversaire(int x, int y, Point list);
int calculLongueur(Coor list);
void calculDepartArrivee(Point list,int xDep,int yDep,int xArr,int yArr,int* startId,int* goalId);
Coor aStar(int xDep, int yDep , int xArr, int yArr, Point list);

#endif // ASTAR_H_INCLUDED
