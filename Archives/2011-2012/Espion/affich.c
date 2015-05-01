#include "affich.h"
#include <SDL/SDL.h>
#define WIDTH  600
#define HEIGHT 400
#define TAILLE_POINT 5

static Affich* affich = NULL;

static void quitSDL()
{
    SDL_FreeSurface(affich->point);

    SDL_Quit();

    free(affich);
}

void openSDL()
{
    affich = malloc(sizeof(Affich));
    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL

    // On cree les surfaces
    affich->screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE); // Ouverture de la fenêtre
    SDL_WM_SetCaption("Cartographie", NULL);

    affich->point = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_POINT, TAILLE_POINT, 32, 0, 0, 0, 0);
    SDL_FillRect(affich->point, NULL, SDL_MapRGB(affich->screen->format, 20, 20, 220));

    // On affiche le vide
    effaceEcran();

    atexit(quitSDL);
}



void effaceEcran()
{
    SDL_FillRect(affich->screen, NULL, SDL_MapRGB(affich->screen->format, 255, 255, 255));
}

void pause()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
        update();
    }
}

void pause1()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN: /* Si appui d'une touche */
            continuer = 0;
            break;
        }
        update();
    }
}


void update()
{
    SDL_Flip(affich->screen);
}

Uint32 getPixel(int red, int green, int blue)
{
    return SDL_MapRGB(affich->screen->format, red, green, blue);
}

void drawPoint(int x, int y)
{
    x = mmToPix(x);
    y = mmToPix(y);
    SDL_Rect position;
        position.x = x - TAILLE_POINT/2 - (TAILLE_POINT%2);
        position.y = HEIGHT -(y - TAILLE_POINT/2 - (TAILLE_POINT%2));

    SDL_BlitSurface(affich->point, NULL, affich->screen, &position);
}






void ligneHorizontale(int x, int y, int w, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;

  SDL_FillRect(affich->screen, &r, coul);
}

void drawDisque(int cx, int cy, int rayon, int R,int V, int B)
{
    cx = mmToPix(cx);
    cy = mmToPix(cy);
    rayon = mmToPix(rayon);
    cy= HEIGHT -cy;
   Uint32 coul = getPixel(R,V,B);
  int d, y, x;

  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;

  while (y >= x) {
    ligneHorizontale(cx - x, cy - y, 2 * x + 1, coul);
    ligneHorizontale(cx - x, cy + y, 2 * x + 1, coul);
    ligneHorizontale(cx - y, cy - x, 2 * y + 1, coul);
    ligneHorizontale(cx - y, cy + x, 2 * y + 1, coul);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }
}
void drawRect(int x, int y, int lx, int ly, int R,int V, int B)
{
    SDL_Rect r;
    Uint32 coul = getPixel(R,V,B);

    x = mmToPix(x);
    y = mmToPix(y);
    y = HEIGHT -y;
    lx = mmToPix(lx);
    ly = mmToPix(ly);

    r.x = x -lx/2;
    r.y = y -ly/2;
    r.w = lx;
    r.h = ly;

    SDL_FillRect(affich->screen, &r, coul);
}



void PutPixel(SDL_Surface *surface, int x, int y, unsigned long pixel)
{
	int bpp = surface->format->BytesPerPixel;
	unsigned char *p = (unsigned char *)surface->pixels + y * surface->pitch + x * bpp;
	if (bpp==4)
		*(unsigned long*)p = pixel;
}

void drawLine(int x1,int y1, int x2,int y2, int R, int G, int B)  // Bresenham
{
    x1 = mmToPix(x1);
    y1 = mmToPix(y1);
    x2 = mmToPix(x2);
    y2 = mmToPix(y2);

	//unsigned long couleur = Color(R, G, B);
	Uint32 couleur = getPixel(R,G,B);
	y1 = HEIGHT -y1;
	y2 = HEIGHT -y2;
	int x,y;
	int Dx,Dy;
	int xincr,yincr;
	int erreur;
	int i;

	Dx = abs(x2-x1);
	Dy = abs(y2-y1);
	if(x1<x2)
		xincr = 1;
	else
		xincr = -1;
	if(y1<y2)
		yincr = 1;
	else
		yincr = -1;

	x = x1;
	y = y1;
	if(Dx>Dy)
	{
		erreur = Dx/2;
		for(i=0;i<Dx;i++)
		{
			x += xincr;
			erreur += Dy;
			if(erreur>Dx)
			{
				erreur -= Dx;
				y += yincr;
			}
			PutPixel(affich->screen,x, y,couleur);
		}
	}
	else
	{
		erreur = Dy/2;
		for(i=0;i<Dy;i++)
		{
			y += yincr;
			erreur += Dx;

			if(erreur>Dy)
			{
				erreur -= Dy;
				x += xincr;
			}
			PutPixel(affich->screen,x, y,couleur);
		}
	}
}

int mmToPix(int lMm)
{
    return lMm*0.2;
}


/*
void drawLine(int xa, int ya, int xb, int yb)
{
    double xu = (double)(xb - xa);
    double yu = (double)(yb - ya);
    double xp = (double) xa;
    double yp = (double) ya;

    while(((xu >= 0 && (int)xp <= xb)||(xu <= 0 && (int)xp >= xb)) && ((yu >= 0 && (int)yp <= yb)||(yu <= 0 && (int)yp >= yb)))
    {
        // On dessine le point actuel
        definirPixel((int)xp, (int)yp, SDL_MapRGB(affich->screen->format, 0, 0, 0));

        // On incremente
        xp += xu;
        yp += yu;
    }
}*/



/*
void definirPixel(int x, int y, Uint32 pixel)
{
    y = HEIGHT - y;
    //nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un pixel.
    //En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    //de l'image : 8, 16, 24 ou 32 bits.
    int nbOctetsParPixel = affich->screen->format->BytesPerPixel;
    //Ici p est l'adresse du pixel que l'on veut modifier
    //surface->pixels contient l'adresse du premier pixel de l'image
    Uint8 *p = (Uint8 *)affich->screen->pixels + y * affich->screen->pitch + x * nbOctetsParPixel;

    //Gestion différente suivant le nombre d'octets par pixel de l'image
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            //Suivant l'architecture de la machine
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}*/

