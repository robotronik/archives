# include "main.hpp"
extern int BORDURES_BLANCHES;
extern int LARGEUR_FENETRE;
extern int HAUTEUR_FENETRE;
extern float RAPPORT_LARGEUR;
extern float RAPPORT_HAUTEUR;
extern SDL_Surface *screen;
extern SDL_Surface * screen_save;

void creation_table ()
{
SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 200, 200, 200));

screen_save = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_FENETRE,HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
// définition de toutes les surfaces
//log("creation table");

SDL_Surface *depart_bleu = SDL_CreateRGBSurface(SDL_HWSURFACE, 40*RAPPORT_LARGEUR, 40*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
//log_arg(300*RAPPORT_LARGEUR);
//log_arg(210*RAPPORT_HAUTEUR);
SDL_Surface *depart_rouge = SDL_CreateRGBSurface(SDL_HWSURFACE, 40*RAPPORT_LARGEUR, 40*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
SDL_Surface *case_bleu = SDL_CreateRGBSurface(SDL_HWSURFACE, 35*RAPPORT_LARGEUR, 35*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
SDL_Surface *case_rouge = SDL_CreateRGBSurface(SDL_HWSURFACE, 35*RAPPORT_LARGEUR, 35*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
SDL_Surface *zones_vertes = SDL_CreateRGBSurface(SDL_HWSURFACE, 40*RAPPORT_LARGEUR, 167.8*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
SDL_Surface *mur = SDL_CreateRGBSurface(SDL_HWSURFACE, 40*RAPPORT_LARGEUR, 2.2*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
SDL_Surface *bande_noire = SDL_CreateRGBSurface(SDL_HWSURFACE, 5*RAPPORT_LARGEUR, 210*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
SDL_Surface *zone_protegees = SDL_CreateRGBSurface(SDL_HWSURFACE, 70*RAPPORT_LARGEUR, 12*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);

   SDL_FillRect(depart_bleu, NULL, SDL_MapRGB(screen_save->format, 50, 50, 255));
  SDL_FillRect(depart_rouge, NULL, SDL_MapRGB(screen_save->format, 255, 50, 50));
     SDL_FillRect(case_bleu, NULL, SDL_MapRGB(screen_save->format, 90, 90, 255));
    SDL_FillRect(case_rouge, NULL, SDL_MapRGB(screen_save->format, 255, 90, 90));
  SDL_FillRect(zones_vertes, NULL, SDL_MapRGB(screen_save->format, 50, 255, 50));
           SDL_FillRect(mur, NULL, SDL_MapRGB(screen_save->format, 50, 50, 50));
   SDL_FillRect(bande_noire, NULL, SDL_MapRGB(screen_save->format, 60, 60, 60));
SDL_FillRect(zone_protegees, NULL, SDL_MapRGB(screen_save->format, 50, 50, 50));

SDL_Rect position;
position.x = 0;
position.y = 0;
SDL_BlitSurface(depart_rouge, NULL, screen_save, &position);
position.x = 260*RAPPORT_LARGEUR;
SDL_BlitSurface(depart_bleu, NULL, screen_save, &position);
position.y = 42.2*RAPPORT_HAUTEUR;
SDL_BlitSurface(zones_vertes, NULL, screen_save, &position);
position.x = 0;
SDL_BlitSurface(zones_vertes, NULL, screen_save, &position);
position.y = 40*RAPPORT_HAUTEUR;
SDL_BlitSurface(mur, NULL, screen_save, &position);
position.x = 260*RAPPORT_LARGEUR;
SDL_BlitSurface(mur, NULL, screen_save, &position);
position.x = 40*RAPPORT_LARGEUR;
position.y = 0;
SDL_BlitSurface(bande_noire, NULL, screen_save, &position);
position.x = 255*RAPPORT_LARGEUR;
SDL_BlitSurface(bande_noire, NULL, screen_save, &position);

int i,j;
for(i=45;i<254;i += 70){
    for(j=0;j<209;j += 70){
        position.x = i*RAPPORT_LARGEUR;
        position.y = j*RAPPORT_HAUTEUR;
        SDL_BlitSurface(case_bleu, NULL, screen_save, &position);
    }
}
for(i=80;i<254;i += 70){
    for(j=35;j<209;j += 70){
        position.x = i*RAPPORT_LARGEUR;
        position.y = j*RAPPORT_HAUTEUR;
        SDL_BlitSurface(case_bleu, NULL, screen_save, &position);
    }
}
for(i=80;i<254;i += 70){
    for(j=0;j<209;j += 70){
        position.x = i*RAPPORT_LARGEUR;
        position.y = j*RAPPORT_HAUTEUR;
        SDL_BlitSurface(case_rouge, NULL, screen_save, &position);
    }
}
for(i=45;i<254;i += 70){
    for(j=35;j<209;j += 70){
        position.x = i*RAPPORT_LARGEUR;
        position.y = j*RAPPORT_HAUTEUR;
        SDL_BlitSurface(case_rouge, NULL, screen_save, &position);
    }
}











position.x = 45*RAPPORT_LARGEUR;
position.y = 198*RAPPORT_HAUTEUR;
SDL_BlitSurface(zone_protegees, NULL, screen_save, &position);
position.x = 185*RAPPORT_LARGEUR;
SDL_BlitSurface(zone_protegees, NULL, screen_save, &position);


position.x = BORDURES_BLANCHES/2;
position.y = BORDURES_BLANCHES/2;
//SDL_SetAlpha(screen_save, SDL_SRCALPHA, 180);

SDL_BlitSurface(screen_save, NULL, screen, &position);
SDL_Flip(screen);




SDL_FreeSurface(depart_bleu);
SDL_FreeSurface(depart_rouge);
SDL_FreeSurface(case_bleu);
SDL_FreeSurface(case_rouge);
SDL_FreeSurface(zones_vertes);
SDL_FreeSurface(mur);
SDL_FreeSurface(bande_noire);
SDL_FreeSurface(zone_protegees);






}


 void save_table()
 {
    //log("coucou save");
SDL_Rect position;
position.x = BORDURES_BLANCHES/2;
position.y = BORDURES_BLANCHES/2;
//SDL_Surface *blanc = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR_FENETRE,HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
//SDL_FillRect(blanc, NULL, SDL_MapRGB(screen_save->format, 255, 255, 255));
//SDL_BlitSurface(blanc, NULL, screen, &position);
SDL_BlitSurface(screen_save, NULL, screen, &position);
//SDL_FreeSurface(blanc);
}








