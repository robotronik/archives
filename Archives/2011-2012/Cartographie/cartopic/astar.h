#ifndef ASTAR_H_INCLUDED
#define ASTAR_H_INCLUDED

/* NOTE : les fonctions suppXxx() sont à supprimer
car elle ne supprime rien, my_free() libère entierement le
tas 2 (heap2)*/

#include "malloc.h"

#define TAILLE_ROBOT 300
#define MAILLE 100

struct ListeFils
{
    int id ;
    struct ListeFils* queue;
};
typedef struct ListeFils* ListeFils;

struct Point
{
    int x ;
    int y ;
    int id ;
    ListeFils lPt;
    int enable;
    struct Point* queue;
};

typedef struct Point* Point;



struct Set
{
    int id ;
    struct Set* queue;
};

typedef struct Set* Set;


struct Data
{
    int id ;
    int g;
    int h;
    int f;
    struct Data* queue;
};

typedef struct Data* Data;

struct Came
{
    int id ;
    int valIdCame;
    struct Came* queue;
};

typedef struct Came* Came;

struct Coor
{
    int id;
    int x;
    int y;
    struct Coor* queue;
};

typedef struct Coor* Coor;


void addListeFils(ListeFils* l, int id);
void suppListeFils(ListeFils list);
void addPoint(Point* l, int x,int y, int id, ListeFils lPt);
void suppPoint(Point list);
void addSet(Set* l, int id);
void suppSet(Set list);
Set removeIdSet(Set l, int id);
int isInSet(Set list , int id);
void suppData(Data list);
Data setGscore(int id , int val , Data l);
int getGscore(int id , Data l);
Data setHscore(int id , int val , Data l);
int getHscore(int id , Data l);
Data setFscore(int id , int val , Data l);
int getFscore(int id , Data l);
Came setCameFrom( int id , int val , Came l);
int getCameFrom(int id , Came l);
void suppCame(Came list);
void addCoor(Coor* l, int id ,int x,int y);
void suppCoor(Coor list);
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
Coor aStar(int startId , int goalId, Point list);

#endif // ASTAR_H_INCLUDED

