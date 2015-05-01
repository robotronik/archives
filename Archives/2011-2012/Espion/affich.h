#ifndef AFFICH_H_INCLUDED
#define AFFICH_H_INCLUDED

#include <SDL/SDL.h>

#define WIDTH  600
#define HEIGHT 400
#define TAILLE_POINT 5



struct Affich
{
    SDL_Surface* screen;
    SDL_Surface* point;
};
typedef struct Affich Affich;

void openSDL();
void effaceEcran();

void pause();
void update();


Uint32 getPixel(int red, int green, int blue);

void drawPoint(int x, int y);
void ligneHorizontale(int x, int y, int w, Uint32 coul);
void drawDisque(int cx, int cy, int rayon, int R,int V, int B);
void drawRect(int x, int y, int lx, int ly, int R,int V, int B);


void drawLine(int x1,int y1, int x2,int y2, int R, int G, int B);


int mmToPix(int lMm);
int mmToPix(int lMm);


//void drawLine(int xa, int ya, int xb, int yb);
//void drawListeSegment(ListeSegment l);
//void definirPixel(int x, int y, Uint32 pixel);
#endif // AFFICH_H_INCLUDED
