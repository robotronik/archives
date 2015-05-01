


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
extern SDL_Color couleurNoir;
extern TTF_Font* police6;
extern SDL_Surface* infoState;
extern SDL_Surface* infoStateBlanc;

extern Elements tableau_p[19];








// affichage et generation pour les elements de jeux (pions)

void generation_pion(SDL_Surface* pion, Uint32 couleur)
{
    SDL_Rect position_sdl;
    SDL_Surface *pixel = SDL_CreateRGBSurface(SDL_HWSURFACE, EPAISSEUR_PIXEL, EPAISSEUR_PIXEL, 32, 0, 0, 0, 0);
    SDL_FillRect(pixel, NULL, couleur);

    int i;
    for(i=0; i<360; i++)
    {
        position_sdl.x= (10 + 10*cos(i*rad))*RAPPORT_LARGEUR;
        position_sdl.y= (10 + 10*sin(i*rad))*RAPPORT_HAUTEUR;
        SDL_BlitSurface(pixel, NULL, pion, &position_sdl);
    }



    SDL_FreeSurface(pixel);
}


void sous_affichage_elements(SDL_Surface * pion, Elements p)
{
    SDL_Rect position_sdl;
    position_sdl.x = p.position.x - 10*RAPPORT_LARGEUR + BORDURES_BLANCHES/2;
    position_sdl.y = p.position.y - 10*RAPPORT_LARGEUR + BORDURES_BLANCHES/2;
    if(p.couleur != COULEUR_BASE_PION )
    {
        generation_pion(pion, p.couleur);
        SDL_BlitSurface(pion, NULL, screen, &position_sdl);
        generation_pion(pion,COULEUR_BASE_PION);
    }
    else
        SDL_BlitSurface(pion, NULL, screen, &position_sdl);

}


void affichage_elements()
{


    SDL_Surface * pion = SDL_CreateRGBSurface(SDL_HWSURFACE, 21*RAPPORT_LARGEUR, 21*RAPPORT_HAUTEUR, 32, 0, 0, 0, 0);
    SDL_SetColorKey(pion, SDL_SRCCOLORKEY, SDL_MapRGB(pion->format, 0, 0, 0));

    generation_pion(pion,COULEUR_BASE_PION);
// boucle sur tout les pions
    int i;
    for(i=0; i<19; i++)
        sous_affichage_elements(pion,tableau_p[i]);



    SDL_FreeSurface(pion);
}


void afficher_state(int i){
char* mot;
switch(i)
{
    case SORTIR_ZONE:
        mot="Sortir zone";
        break;
    case SAISIR_1:
         mot="Saisir 1";
        break;
    case RETOUR_ZONE_DEP:
         mot="Retour zone dep";
        break;
    case AVANCER_LIGNE_1:
         mot="Avancer ligne 1";
        break;
    case SAISIR_2:
         mot="Saisir 2";
        break;
    case REMONTER_LIGNE_1:
         mot="Remonter ligne 1";
        break;
    case SAISIR_3:
         mot="Saisir 3";
        break;
    case DEPOT_ZONE_SURE:
         mot="Depot zone sure";
        break;
    case GO_LIGNE_2:
         mot="Go ligne 2";
        break;
    case SAISIR_4:
         mot="Saisir 4";
        break;
    case DEPOT_BONUS:
         mot="Depot bonus";
        break;
    case AVANCER_LIGNE_2:
         mot="Avancer ligne 2";
        break;
    case SAISIR_5:
         mot="Saisir 5";
        break;
    case REDESCENDRE_LIGNE_2:
         mot="Redescendre ligne 2";
        break;
    case SAISIR_6:
         mot="Saisir 6";
        break;
    case DEPOT_4:
         mot="Depot 4";
        break;
    case FINIR_MONTER:
         mot="Finir monter";
        break;
    case FIN:
         mot = "Fin";
        break;
    default:
     mot="?";
        break;
}
creer_info(mot);
}





void creer_info(char * infof)
{
    infoState = TTF_RenderText_Blended(police6, infof , couleurNoir);
}


void afficher_info()
{
    SDL_Rect position;
    position.x = 250*RAPPORT_LARGEUR;
    position.y = 0*RAPPORT_HAUTEUR;
    SDL_BlitSurface(infoStateBlanc, NULL, screen, &position);
    SDL_BlitSurface(infoState, NULL, screen, &position);
}

// affichage et generation pour les robots


void afficher_robot(Robot* R1, Robot* R2)
{
    SDL_Rect position;
    save_table();
    position.x = R1->position_robot.x - 16.808*RAPPORT_LARGEUR -15 + BORDURES_BLANCHES/2;
    position.y = R1->position_robot.y - 16.808*RAPPORT_HAUTEUR -15 + BORDURES_BLANCHES/2;
    SDL_BlitSurface(R1->robot, NULL, screen, &position);
    position.x = R2->position_robot.x - 16.808*RAPPORT_LARGEUR -15 + BORDURES_BLANCHES/2;
    position.y = R2->position_robot.y - 16.808*RAPPORT_HAUTEUR -15 + BORDURES_BLANCHES/2;
    SDL_BlitSurface(R2->robot, NULL, screen, &position);
    afficher_info();
    affichage_elements();
    SDL_Flip(screen);
}


void generation_robot(Robot *R)
{
    float d = R->direction_robot*degre;

    SDL_FillRect(R->robot, NULL, SDL_MapRGB(R->robot->format, 0, 0, 0));
    SDL_SetColorKey(R->robot, SDL_SRCCOLORKEY, SDL_MapRGB(R->robot->format, 0, 0, 0));
    SDL_Surface *pixel = SDL_CreateRGBSurface(SDL_HWSURFACE, EPAISSEUR_PIXEL, EPAISSEUR_PIXEL, 32, 0, 0, 0, 0);
    SDL_FillRect(pixel, NULL, R->couleur);

    generation_droite( R->angle_1, R->angle_2, R, pixel);
    generation_droite( R->direction_1, R->direction_2, R, pixel);
    generation_droite( R->angle_2, R->angle_inter_2, R, pixel);
    generation_droite( R->angle_inter_2, R->arc_cercle_2_1, R, pixel);
    generation_droite( R->arc_cercle_2_2, R->angle_inter_3, R, pixel);
    generation_droite( R->angle_inter_3, R->angle_3, R, pixel);
    generation_droite( R->angle_3, R->angle_4, R, pixel);
    generation_droite( R->angle_4, R->angle_inter_4, R, pixel);
    generation_droite( R->angle_inter_4, R->arc_cercle_1_2, R, pixel);
    generation_droite( R->arc_cercle_1_1, R->angle_inter_1, R, pixel);
    generation_droite( R->angle_inter_1, R->angle_1, R, pixel);
    generation_droite( R->pince_avant_1, R->arc_cercle_2_1, R, pixel);
    generation_droite( R->pince_avant_2, R->arc_cercle_2_2, R, pixel);
    generation_droite( R->pince_arriere_1, R->arc_cercle_1_1, R, pixel);
    generation_droite( R->pince_arriere_2, R->arc_cercle_1_2, R, pixel);
    generation_cercle( d-88,d+88, R->centre_arc_1, R, pixel);
    generation_cercle( d+93,d+267, R->centre_arc_2, R, pixel);

    SDL_FreeSurface(pixel);
}

// réinterprétation de l' Algorithme de tracé de segment de Bresenham
void generation_droite(Coord position_1,Coord position_2, Robot* R, SDL_Surface* pixel)
{
    Coord position;
    position.x = position_1.x;
    position.y = position_1.y;
    SDL_Rect position_sdl;
    int j;
    if(position_1.x == position_2.x )
    {
        position.x = position_1.x;
        if( position_1.y<=position_2.y){
            for(j=position_1.y; j<=position_2.y; j ++)
                {
                    position.y = j;
                    position_sdl.x = position.x;
                    position_sdl.y = position.y;
                    SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                }
        }
        else{
            for(j=position_2.y; j<=position_1.y; j ++)
                {
                    position.y = j;
                    position_sdl.x = position.x;
                    position_sdl.y = position.y;
                    SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                }
        }
    }
    else if((position_1.y == position_2.y))
    {
        position.y = position_1.y;
        if( position_1.x<=position_2.x){
            for(j=position_1.x; j<=position_2.x; j ++)
                {
                    position.x = j;
                    position_sdl.x = position.x;
                    position_sdl.y = position.y;
                    SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                }
        }
        else{
            for(j=position_2.x; j<=position_1.x; j ++)
                {
                    position.x = j;
                    position_sdl.x = position.x;
                    position_sdl.y = position.y;
                    SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                }
        }
    }
    else if ( position_1.x < position_2.x && position_1.y < position_2.y)
    {
        if(position_2.x - position_1.x > position_2.y - position_1.y){
           // log("Bresenham classique");
            int e = position_2.x - position_1.x;
            int dx = 2*e;
            int dy = (position_2.y - position_1.y)*2;
            while(position.x < position_2.x){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.x ++;
                e -= dy;
                if(e<=0){
                    position.y ++;
                    e += dx;
                }
            }
        }
        else{
           // log("Bresenham classique 2");
            int e = (position_2.y - position_1.y);
            int dx = 2*(position_2.x - position_1.x);
            int dy = e*2;
            while(position.y < position_2.y){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.y ++;
                e -= dx;
                if(e<=0){
                    position.x ++;
                    e += dy;
                }
            }
        }
    }
    else if ( position_1.x < position_2.x && position_1.y > position_2.y)
    {
        if(position_2.x - position_1.x > position_1.y - position_2.y){
           // log("Bresenham y inversés");
            int e = position_2.x - position_1.x;
            int dx = 2*e;
            int dy = (position_1.y - position_2.y)*2;
            while(position.x < position_2.x){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.x ++;
                e -= dy;
                if(e<=0){
                    position.y --;
                    e += dx;
                }
            }
        }

        else{
            //log("Bresenham y inversés 2 ");
            position.x = position_2.x;
            position.y = position_2.y;
            int e = (position_1.y - position_2.y);
            int dx = 2*(position_2.x - position_1.x);
            int dy = e*2;
            while(position.y < position_1.y){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.y ++;
                e -= dx;
                if(e<=0){
                    position.x --;
                    e += dy;
                }
            }
        }
    }
    else if ( position_1.x > position_2.x && position_1.y < position_2.y )
    {
        if(position_1.x - position_2.x > position_2.y - position_1.y){
           // log("Bresenham x inversés");
            int e = position_1.x - position_2.x;
            int dx = 2*e;
            int dy = (position_2.y - position_1.y)*2;
            while(position.x > position_2.x){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.x --;
                e -= dy;
                if(e<=0){
                    position.y ++;
                    e += dx;
                }
            }
        }

        else{
           // log("Bresenham x inversés 2 ");
            position.x = position_2.x;
            position.y = position_2.y;
            int e = (position_2.y - position_1.y);
            int dx = 2*(position_1.x - position_2.x);
            int dy = e*2;
            while(position.y > position_1.y){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.y --;
                e -= dx;
                if(e<=0){
                    position.x ++;
                    e += dy;
                }
            }
        }
    }
    else if ( position_1.x > position_2.x && position_1.y > position_2.y )
    {
        if(position_1.x - position_2.x > position_1.y - position_2.y){
          //  log("Bresenham x et y inversés");
            int e = position_1.x - position_2.x;
            int dx = 2*e;
            int dy = (position_1.y - position_2.y)*2;
            while(position.x > position_2.x){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.x --;
                e -= dy;
                if(e<=0){
                    position.y --;
                    e += dx;
                }
            }
        }

        else{
         //   log("Bresenham x et y inversés 2 ");
            int e = (position_1.y - position_2.y);
            int dx = 2*(position_1.x - position_2.x);
            int dy = e*2;
            while(position.y > position_2.y){
                position_sdl.x = position.x;
                position_sdl.y = position.y;
                SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
                position.y --;
                e -= dx;
                if(e<=0){
                    position.x --;
                    e += dy;
                }
            }
        }
    }





}


void generation_cercle(float direction1, float direction2 , Coord centre, Robot * R, SDL_Surface* pixel)
{
SDL_Rect position_sdl;
int r = R->rayon_arc, i;
for(i=direction1; i<=direction2;i++){
position_sdl.x= centre.x + r*cos(i*rad)*RAPPORT_LARGEUR;
position_sdl.y= centre.y + r*sin(i*rad)*RAPPORT_HAUTEUR;
SDL_BlitSurface(pixel, NULL, R->robot, &position_sdl);
}
}

