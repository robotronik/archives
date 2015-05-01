#ifndef OUTILS_H_INCLUDED
#define OUTILS_H_INCLUDED

#include "cst.h"

#define NSQ(x,y) (x*x+y*y)
#define SYMX(x) 3000-x
#define SYMY(y) 2000-y
#define SYMX_ALPHA(a) ((540-a)%360)
#define SYMY_ALPHA(a) ((360-a)%360)

#ifdef SIMU
extern int getTime();
#else
int getTime();
#endif

struct _CoordPlateau
{
  unsigned int
        x : 16,
        y : 16;
};
typedef struct _CoordPlateau CoordPlateau;

struct _RetourneInt
{
    union
    {
        int
            Xooo : 8,
            oXoo : 8,
            ooXo : 8,
            oooX : 8;
        int all;
    };
};
typedef struct _RetourneInt RetourneInt;


struct _RetourneShort
{
    union
    {
        short
            Xo : 8,
            oX : 8;
        short all;
    };
};
typedef struct _RetourneShort RetourneShort;

int valAbs(int a);
int max(int a, int b);
int normeSup(int x1, int y1, int x2, int y2);

int intLittleToBigEndian(int a);
short shortLittleToBigEndian(short a);

/** proba est un %0 (un pour mille)
 *  renvoie 1 si probabilite accomplie, 0 sinon
 **/
char myRand(long int proba);

#endif // OUILS_H_INCLUDED
