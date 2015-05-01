

# include "main.hpp"
extern int BORDURES_BLANCHES;
extern int LARGEUR_FENETRE;
extern int HAUTEUR_FENETRE;
extern float RAPPORT_LARGEUR;
extern float RAPPORT_HAUTEUR;
extern int EPAISSEUR_PIXEL;
extern int DELAY;
extern float VITESSE_TRANS_ROBOT_DEFAUT;
extern float VITESSE_ROT_ROBOT_DEFAUT;
extern float VITESSE_PINCE_DEFAUT;
extern int ANGLE_PINCE_DEFAUT;
extern SDL_Surface *screen;
extern int tab[3];

extern SDL_Color couleurBlanc;
extern TTF_Font* police5;
extern TTF_Font* police6;
extern TTF_Font* police7;
extern TTF_Font* police8;
extern TTF_Font* police10;
extern SDL_Surface *robotronik, *fond;
extern SDL_Surface* screen_save_menu;


void menu()
{
tab = {-1,-1,-1};
screen_save_menu = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_FENETRE+BORDURES_BLANCHES,HAUTEUR_FENETRE+BORDURES_BLANCHES, 32, 0, 0, 0, 0);
SDL_Event event;
SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
generation_menu();
SDL_FillRect(screen_save_menu, NULL, SDL_MapRGB(screen_save_menu->format, 255, 255, 255));
SDL_Rect position;
position.x = 0;
position.y = 0;
SDL_BlitSurface(screen, NULL, screen_save_menu, &position);

int continuer =1, val = -1;
while(continuer)
{
    val = -1;
    SDL_WaitEvent(&event);
    if( event.type == SDL_QUIT | (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        finAppli();
    else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE)
    {
        if(tab[1] != -1)
        {
            tab[1] = tab[1]/10;
            if(tab[1] == 0){
                tab[1] = -1;
            }
            actualiserAffichage();
        }
    }
    else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
    {
        tab[0] = 1;
        return ;
    }
    else if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN))
    {
        saisieFinie();
        continuer = affichage_saisie_ok();
    }
    else if((event.type == SDL_KEYDOWN))
    {
        val = recup_para(event);
        if(val != -1)
        {
            if(tab[1] == -1){
                tab[1] = val;
                actualiserAffichage();
            }
            else if(tab[1]<10){
                tab[1] = tab[1]*10 + val;
                actualiserAffichage();
            }
        }
        else{
            val = recup_para_couleur(event);
            if(val != -1)
            {
                tab[2] = val;
                actualiserAffichage();
            }
        }
    }
}
}

void saisieFinie(){
int bol = 0;
if(tab[1] == -1){
    bol = 1;
    tab[1] =1;
}
if(tab[2]== -1){
    bol =1;
    tab[2] =0;
}
if(bol)
   actualiserAffichage();
}


void actualiserAffichage(){
    SDL_Rect position;
    position.x = 0;
    position.y = 0;
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_BlitSurface(screen_save_menu, NULL, screen, &position);
    SDL_Surface* texte;
    if(tab[1]!=-1){
        char string[2] = {(char)(6+48),'\0'};
        texte = TTF_RenderText_Blended(police8,(char[3]){(tab[1]<10)?(char)(tab[1]+48):(char)((tab[1]/10)+48),(tab[1]<10)?' ':(char)((tab[1]-(tab[1]/10)*10)+48),'\0'}, couleurBlanc);
        position.x = (200+3*8)*RAPPORT_LARGEUR;
        position.y = (85+12*1)*RAPPORT_HAUTEUR;
        SDL_BlitSurface(texte, NULL, screen, &position);
    }
    if(tab[2] != -1){
        if(tab[2] == 0)
            texte = TTF_RenderText_Blended(police8, "rouge" , couleurBlanc);
        else
            texte = TTF_RenderText_Blended(police8, "bleu" , couleurBlanc);
        position.x = (200+3*8)*RAPPORT_LARGEUR;
        position.y = (85+12*2)*RAPPORT_HAUTEUR;
        SDL_BlitSurface(texte, NULL, screen, &position);
    }
    SDL_Flip(screen);
    SDL_FreeSurface(texte);
}

int affichage_saisie_ok()
{
    SDL_Rect position;
    SDL_Surface* texte1 = TTF_RenderText_Blended(police7, "Saisie ok" , couleurBlanc); //(char*)((tab_temp[n]%10)+48)
    SDL_Surface* texte2 = TTF_RenderText_Blended(police7, "Appuyer sur \" entrée \" pour demarer la simulation" , couleurBlanc);
    SDL_Surface* texte3 = TTF_RenderText_Blended(police7, "Ou sur \" retour \" pour changer la configuration." , couleurBlanc);

    position.x = 10*RAPPORT_LARGEUR;
    position.y = 140*RAPPORT_HAUTEUR;
    SDL_BlitSurface(texte1, NULL, screen, &position);
    SDL_BlitSurface(texte1, NULL, screen, &position);
    position.y = 155*RAPPORT_HAUTEUR;
    SDL_BlitSurface(texte2, NULL, screen, &position);
    SDL_BlitSurface(texte2, NULL, screen, &position);
    position.y = 170*RAPPORT_HAUTEUR;
    SDL_BlitSurface(texte3, NULL, screen, &position);
    SDL_BlitSurface(texte3, NULL, screen, &position);

    SDL_Flip(screen);
    SDL_Event event;

    SDL_Delay(500);
    SDL_FreeSurface(texte1);
    SDL_FreeSurface(texte2);
    SDL_FreeSurface(texte3);

while(1)
{
    SDL_WaitEvent(&event);
    if( (event.type == SDL_QUIT) | (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        finAppli();
    else if((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN))
        return 0;
    else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE){
        actualiserAffichage();
        return 1;
    }
}
}

int recup_para(SDL_Event event)
{
switch(event.key.keysym.sym)
{
    case SDLK_0:
    return 0;
    case SDLK_1:
    return 1;
    case SDLK_2:
    return 2;
    case SDLK_3:
    return 3;
    case SDLK_4:
    return 4;
    case SDLK_5:
    return 5;
    case SDLK_6:
    return 6;
    case SDLK_7:
    return 7;
    case SDLK_8:
    return 8;
    case SDLK_9:
    return 9;
    case SDLK_KP0:
    return 0;
    case SDLK_KP1:
    return 1;
    case SDLK_KP2:
    return 2;
    case SDLK_KP3:
    return 3;
    case SDLK_KP4:
    return 4;
    case SDLK_KP5:
    return 5;
    case SDLK_KP6:
    return 6;
    case SDLK_KP7:
    return 7;
    case SDLK_KP8:
    return 8;
    case SDLK_KP9:
    return 9;
    default :
    return -1;
}
}

int recup_para_couleur(SDL_Event event)
{
switch(event.key.keysym.sym)
{
    case SDLK_LEFT:
    return 0;
    case SDLK_RIGHT:
    return 1;
    default :
    return -1;
}
}

void generation_menu()
{
int i;
SDL_Rect position;

SDL_Surface* texte[10] = {NULL};

position.x = 0;
position.y = 0;
SDL_BlitSurface(fond, NULL, screen, &position);
position.x = 240*RAPPORT_LARGEUR;
position.y = 10*RAPPORT_HAUTEUR;
SDL_BlitSurface(robotronik, NULL, screen, &position);



texte[0] = TTF_RenderText_Blended(police10, "Bienvenue dans la simulation ", couleurBlanc);
texte[1] = TTF_RenderText_Blended(police10, "robotronik pour la coupe 2011 ", couleurBlanc);
texte[2] = TTF_RenderText_Blended(police8, "Entrez les parametres de la simulation :  ", couleurBlanc);
texte[3] = TTF_RenderText_Blended(police8, "- Numero de config des pions (0 à 100) :   ", couleurBlanc);
texte[4] = TTF_RenderText_Blended(police8, "-      depart du robot (gauche/droite) : ", couleurBlanc);

texte[8] = TTF_RenderText_Blended(police6, "Barre espace pour menu debug ou configuration de table cheloue", couleurBlanc);
texte[9] = TTF_RenderText_Blended(police5, "version 1.1 du 14/05/2011 par Hoel   ", couleurBlanc);

position.x = 10*RAPPORT_LARGEUR;
position.y = 20*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[0], NULL, screen, &position);
position.y = 32*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[1], NULL, screen, &position);
position.y = 80*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[2], NULL, screen, &position);
position.y += 5*RAPPORT_HAUTEUR;

for(i=3; i<5; i++)
{
    position.y += 12*RAPPORT_HAUTEUR;
    SDL_BlitSurface(texte[i], NULL, screen, &position);
}

position.y = 60*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[8], NULL, screen, &position);

position.x = 180*RAPPORT_LARGEUR;
position.y = 210*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[9], NULL, screen, &position);



SDL_Flip(screen);

for(i=0; i<9; i++)
{
    SDL_FreeSurface(texte[i]);
}

}


int menu_debug()
{
SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
int i;
SDL_Rect position;
SDL_Surface* texte[10] = {NULL};
position.x = 0;
position.y = 0;
SDL_BlitSurface(fond, NULL, screen, &position);
position.x = 240*RAPPORT_LARGEUR;
position.y = 10*RAPPORT_HAUTEUR;
SDL_BlitSurface(robotronik, NULL, screen, &position);

texte[0] = TTF_RenderText_Blended(police10, "Bienvenue dans la simulation ", couleurBlanc);
texte[1] = TTF_RenderText_Blended(police10, "robotronik pour la coupe 2011 ", couleurBlanc);
texte[2] = TTF_RenderText_Blended(police8, "Menu debug, menu des warriors !  ", couleurBlanc);
texte[3] = TTF_RenderText_Blended(police8, "Pas encore implémenté ..   ", couleurBlanc);
texte[4] = TTF_RenderText_Blended(police8, "Bientôt un joystick ?! ", couleurBlanc);
texte[5] = TTF_RenderText_Blended(police8, "Alors comme rien est fait ici, et bien ..", couleurBlanc);
texte[6] = TTF_RenderText_Blended(police8, "vous ne pouvez que quitter l'application   :-P ", couleurBlanc);
texte[7] = TTF_RenderText_Blended(police6, "      .. bien fait pour vous ..", couleurBlanc);

texte[9] = TTF_RenderText_Blended(police5, "version 1.1 du 14/05/2011 par Hoel   ", couleurBlanc);

position.x = 10*RAPPORT_LARGEUR;
position.y = 20*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[0], NULL, screen, &position);
position.y = 32*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[1], NULL, screen, &position);
position.y = 80*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[2], NULL, screen, &position);
position.y += 5*RAPPORT_HAUTEUR;

for(i=3; i<8; i++)
{
    position.y += 12*RAPPORT_HAUTEUR;
    SDL_BlitSurface(texte[i], NULL, screen, &position);
}


position.x = 180*RAPPORT_LARGEUR;
position.y = 210*RAPPORT_HAUTEUR;
SDL_BlitSurface(texte[9], NULL, screen, &position);



SDL_Flip(screen);

for(i=0; i<9; i++)
{
    SDL_FreeSurface(texte[i]);
}
SDL_Event event;
while(1)
{
    SDL_WaitEvent(&event);
    if( (event.type == SDL_QUIT) | (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        finAppli();
}
}


void interuption_simu()
{
SDL_Event event;
SDL_PollEvent(&event);
if( (event.type == SDL_QUIT) | (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)){
    finAppli();
}
}




