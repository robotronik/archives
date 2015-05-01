#include "outils.h"

#ifndef SIMU
int getTime()
{
	return Timer_85s_ReadCounter();
}
#endif

int valAbs(int a)
{
   return (a>=0) ? a : -a;
}

int max(int a, int b)
{
    return (a>=b) ? a : b;
}

int normeSup(int x1, int y1, int x2, int y2)
{
    int x = valAbs(x1 - x2);
    int y = valAbs(y1 - y2);

    return max(x,y);
}

int intLittleToBigEndian(int a)
{
    RetourneInt e,r;
    e.all = a;

    r.Xooo = e.oooX;
    r.oXoo = e.ooXo;
    r.ooXo = e.oXoo;
    r.oooX = e.Xooo;

    return r.all;
}

short shortLittleToBigEndian(short a)
{
    short r = 0;
    r = ((0xFF&a)<<8) // Partie gauche de r
      + (a >> 8);     // Partie droite de r

    return r;
}

char myRand(long int proba)
{
    const unsigned int a = 12;
    static int u = 1; // osef, l'entropie est toujours ingeree
    u = (a*u+((int)getTime()%1001))%1001;

    return u < proba;
}
