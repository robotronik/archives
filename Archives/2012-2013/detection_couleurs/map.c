#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define WIDTH 656
#define HEIGHT 416
#define OFFSETY 21
#define POSX 0

extern int posX;
extern int posY;
int map()
{

    SDL_Event event;
    SDL_Surface *ecran, *table = NULL;
    SDL_Rect posTable, posRobot1; //Position des élements
    char continuer = 1;
    Uint32 colRobot1 ;
    posTable.x = 0;
    posTable.y = 0;

    posRobot1.x = 0;
    posRobot1.y = 0;

    posRobot1.w = 30;
    posRobot1.h = 30;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL \n");
        exit(EXIT_FAILURE);
    }
     
    ecran =  SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_FULLSCREEN);//Ouverture de la SDL 
    if(ecran == NULL)
    {
	fprintf(stderr, "Erreur lors de l'ouverture de la fenetre");
        exit(EXIT_FAILURE);
    } 
    colRobot1 = SDL_MapRGB(ecran->format, 40,100,40); //Couleur du robot 1
    table = IMG_Load("/home/pi/Robotronik/robotronik/detection_couleurs/img/table.png");//Chargement de l'image de la table
    if(table == NULL)
    {
	fprintf(stderr, "Impossible de charger l'image de la table");
    }

    while (continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
	case SDL_QUIT:
	    continuer = 0;
	    break;
	case SDL_KEYDOWN :
	    switch (event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE : 
		continuer = 0;
		break;
	    default :
		continuer = 1;
		break;
	    }
	    
	    break;
	}
    

	//On affiche de nouveau nos élements
	posRobot1.w = 20; 
	 posRobot1.h=20; 
	 posRobot1.x = (posX*WIDTH)/3000.0;
	 posRobot1.y = HEIGHT-(posY*HEIGHT)/2000.0 - posRobot1.h - OFFSETY;

  	
	SDL_BlitSurface(table, NULL, ecran, &posTable);
	SDL_FillRect(ecran, &posRobot1,  colRobot1);
	SDL_Flip(ecran);
	SDL_Delay(50);
    }

    SDL_FreeSurface(table);
    SDL_FreeSurface(ecran);
    SDL_Quit();
  	fprintf(stderr, "Exit SDL clean \n");
    return 0;

}
