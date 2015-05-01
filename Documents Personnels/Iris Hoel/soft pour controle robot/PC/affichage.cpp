/*=============================================================================
 *
 *
 *          Control du robot par stick et visualisation
 *               de la cmd et du retour des moteurs
 *
 *
 *===========================================================================*/



/*------------------------------- include -----------------------------------*/


#include "affichage.hpp"



/*------------------------------- variables ---------------------------------*/

extern float vitesse_deplacement;
extern float vitesse_rotation;
extern Robot *R;
extern Robot *R_Real;
extern SDL_Surface *screen;
extern int Vg;
extern int Vd;
extern int leftReel;
extern int rightReel;


/*------------------------------- declaration fonctions ---------------------*/


/*-------------------- deplacement robot simu -----------*/

//Uint32 oneStepSimu(Uint32 intervalle, void*);

static float avancer(float deplacement, Robot *R);
static float tourner(float rotation, Robot* R);


/*--------------------- affichage robot simu ------------*/


static void generation_robot(Robot *R);
static void generation_droite(Coord position_1,Coord position_2, Robot* R, SDL_Surface* pixel);
static void generation_cercle(float direction1, float direction2 , Coord centre, Robot * R, SDL_Surface* pixel);


/*--------------------- geometrie ----------------------*/

static float modulo_2pi(float a);










/*---------------------------------------------------------------------------*/









/*------------------------------ deplacement robot simu ------------------------*/

void oneStepSimu()
{
    avancer(((float)(Vg+Vd)/2)/RAPPORT_VITESSE_SIMU,R);
    tourner(((float)(Vg-Vd)/2)/RAPPORT_VITESSE_SIMU,R);
    //tourner(atan((float)(Vg-Vd)/RAPPORT_VITESSE_SIMU)/2, R);

    //avancer(vitesse_deplacement_mesuree/10000,R_Real);
    //tourner(vitesse_rotation_mesuree/10000,R_Real);

    // printf("vitesse sim = %lf , vitesse reel = %lf\n",vitesse_deplacement/RAPPORT_VITESSE_SIMU,vitesse_deplacement_mesuree/10000 );

    static float old_vitesse_deplacement_mesuree = 0;
    static float old_vitesse_rotation_mesuree = 0;

    avancer(((float)(leftReel+rightReel)/20000) - old_vitesse_deplacement_mesuree, R_Real);
    tourner(((float)(leftReel-rightReel)/20000) - old_vitesse_rotation_mesuree, R_Real);

    old_vitesse_deplacement_mesuree = (float)(leftReel+rightReel)/20000;
    old_vitesse_rotation_mesuree = (float)(leftReel-rightReel)/20000;



    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));


    afficher_robot(R);
    afficher_robot(R_Real);

    SDL_Flip(screen);
 //   return intervalle;
}


// Uint32 oneStepSimu(Uint32 intervalle, void*)
// {
//     avancer(vitesse_deplacement/(RAPPORT_VITESSE_SIMU*2),R);
//     tourner(vitesse_rotation/RAPPORT_VITESSE_SIMU,R);
//     afficher_robot(R);
//     return intervalle;
// }

static void sous_tourner(float xc, float yc, float coss, float sinn, Coord* surface)
{
   float save = coss*((*surface).x - xc) - sinn*((*surface).y - yc) + xc;
    (*surface).y = sinn*((*surface).x - xc) + coss*((*surface).y - yc) + yc;
    (*surface).x = save;

}


float tourner_a_90(Robot* R)
{
    float xc = 16.808*RAPPORT_LARGEUR+15;
    float yc = 16.808*RAPPORT_HAUTEUR+15;
    float coss = cos(-M_PI/2);
    float sinn = sin(-M_PI/2);
    sous_tourner(xc,yc,coss,sinn, &R->angle_1);
    sous_tourner(xc,yc,coss,sinn, &R->angle_2);
    sous_tourner(xc,yc,coss,sinn, &R->angle_3);
    sous_tourner(xc,yc,coss,sinn, &R->angle_4);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_1);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_2);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_3);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_4);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_1_1);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_1_2);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_2_1);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_2_2);
    sous_tourner(xc,yc,coss,sinn, &R->direction_1);
    sous_tourner(xc,yc,coss,sinn, &R->direction_2);
    sous_tourner(xc,yc,coss,sinn, &R->centre_arc_1);
    sous_tourner(xc,yc,coss,sinn, &R->centre_arc_2);
    sous_tourner(xc,yc,coss,sinn, &R->pince_arriere_1);
    sous_tourner(xc,yc,coss,sinn, &R->pince_arriere_2);
    sous_tourner(xc,yc,coss,sinn, &R->pince_avant_1);
    sous_tourner(xc,yc,coss,sinn, &R->pince_avant_2);
    R->direction_robot = modulo_2pi(R->direction_robot-(M_PI/2));
}

/* deplacement en cm */
static float avancer(float deplacement, Robot *R)
{
    R->position_robot.x += deplacement*cos(R->direction_robot)*RAPPORT_LARGEUR;
    R->position_robot.y += deplacement*sin(R->direction_robot)*RAPPORT_HAUTEUR;
    //R->position_robot = modulo_fenetre(R->position_robot);
    return deplacement;
}

/* rotation en degre */
static float tourner(float rotation, Robot* R)
{
    float xc = 16.808*RAPPORT_LARGEUR+15;
    float yc = 16.808*RAPPORT_HAUTEUR+15;
    float coss = cos(rotation*rad);
    float sinn = sin(rotation*rad);
    sous_tourner(xc,yc,coss,sinn, &R->angle_1);
    sous_tourner(xc,yc,coss,sinn, &R->angle_2);
    sous_tourner(xc,yc,coss,sinn, &R->angle_3);
    sous_tourner(xc,yc,coss,sinn, &R->angle_4);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_1);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_2);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_3);
    sous_tourner(xc,yc,coss,sinn, &R->angle_inter_4);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_1_1);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_1_2);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_2_1);
    sous_tourner(xc,yc,coss,sinn, &R->arc_cercle_2_2);
    sous_tourner(xc,yc,coss,sinn, &R->direction_1);
    sous_tourner(xc,yc,coss,sinn, &R->direction_2);
    sous_tourner(xc,yc,coss,sinn, &R->centre_arc_1);
    sous_tourner(xc,yc,coss,sinn, &R->centre_arc_2);
    sous_tourner(xc,yc,coss,sinn, &R->pince_arriere_1);
    sous_tourner(xc,yc,coss,sinn, &R->pince_arriere_2);
    sous_tourner(xc,yc,coss,sinn, &R->pince_avant_1);
    sous_tourner(xc,yc,coss,sinn, &R->pince_avant_2);
    R->direction_robot = modulo_2pi(R->direction_robot + rotation*rad);
    //log(" direction");
    //log_arg(d);
    //log_arg(R->direction_robot);
    return rotation;
}







/*------------------------------ affichage simu ------------------------------*/




Robot *configuration_initiale_robot(Uint32 couleur, Coord position)
{
    Robot* R = NULL;
    R = (Robot*) malloc(sizeof(Robot));
    R->direction_robot = 0;  // angle en degré
    R->position_robot.x = position.x;  // position du robot sur la table
    R->position_robot.y = position.y;
    /* +15 pour ouverture des pinces arrières */
    R->angle_1.x = 8.308*RAPPORT_LARGEUR + 15; // position des différenets points du robot à l'intérieur du carré de celui-ci
    R->angle_1.y = 2.308*RAPPORT_HAUTEUR + 15;
    R->angle_2.x = 25.308*RAPPORT_LARGEUR+ 15;
    R->angle_2.y = 2.308*RAPPORT_HAUTEUR+ 15;
    R->angle_3.x = 25.308*RAPPORT_LARGEUR+ 15;
    R->angle_3.y = 31.308*RAPPORT_HAUTEUR+ 15;
    R->angle_4.x = 8.308*RAPPORT_LARGEUR+ 15;
    R->angle_4.y = 31.308*RAPPORT_HAUTEUR+ 15;
    R->angle_inter_1.x = 4.796*RAPPORT_LARGEUR+ 15;
    R->angle_inter_1.y = 5.819*RAPPORT_HAUTEUR+ 15;
    R->angle_inter_2.x = 28.819*RAPPORT_LARGEUR+ 15;
    R->angle_inter_2.y = 5.819*RAPPORT_HAUTEUR+ 15;
    R->angle_inter_3.x = 28.819*RAPPORT_LARGEUR+ 15;
    R->angle_inter_3.y = 27.796*RAPPORT_HAUTEUR+ 15;
    R->angle_inter_4.x = 4.796*RAPPORT_LARGEUR+ 15;
    R->angle_inter_4.y = 27.796*RAPPORT_HAUTEUR+ 15;
    R->arc_cercle_1_1.x = 4.796*RAPPORT_LARGEUR+ 15;
    R->arc_cercle_1_1.y = 6.319*RAPPORT_HAUTEUR+ 15;
    R->arc_cercle_1_2.x = 4.796*RAPPORT_LARGEUR+ 15;
    R->arc_cercle_1_2.y = 27.296*RAPPORT_HAUTEUR+ 15;
    R->arc_cercle_2_1.x = 28.819*RAPPORT_LARGEUR+ 15;
    R->arc_cercle_2_1.y = 6.319*RAPPORT_HAUTEUR+ 15;
    R->arc_cercle_2_2.x = 28.819*RAPPORT_LARGEUR+ 15;
    R->arc_cercle_2_2.y = 27.296*RAPPORT_HAUTEUR+ 15;
    R->direction_1.x = (16.808 +16.808)*RAPPORT_LARGEUR+ 15;
    R->direction_1.y = 16.808*RAPPORT_HAUTEUR+ 15;
    R->direction_2.x = (12 +16.808)*RAPPORT_LARGEUR+ 15;
    R->direction_2.y = 16.808*RAPPORT_HAUTEUR+ 15;
    R->rayon_arc = 10.5;
    R->centre_arc_1.x = 4.308*RAPPORT_LARGEUR+ 15;
    R->centre_arc_1.y = 16.808*RAPPORT_HAUTEUR+ 15;
    R->centre_arc_2.x = 29.308*RAPPORT_LARGEUR+ 15;
    R->centre_arc_2.y = 16.808*RAPPORT_HAUTEUR+ 15;
    R->pince_avant_1.x = R->angle_2.x;
    R->pince_avant_1.y = R->angle_2.y;
    R->pince_avant_2.x = R->angle_3.x;
    R->pince_avant_2.y = R->angle_3.y;
    R->pince_arriere_1.x = R->angle_1.x;
    R->pince_arriere_1.y = R->angle_1.y;
    R->pince_arriere_2.x = R->angle_4.x;
    R->pince_arriere_2.y = R->angle_4.y;
    R->robot = SDL_CreateRGBSurface(SDL_HWSURFACE, (33.6+1)*RAPPORT_LARGEUR+30, (33.6+1)*RAPPORT_HAUTEUR+30, 32, 0, 0, 0, 0);
    R->couleur = couleur;

    return R;
}





typedef struct Inst
{
int vitesse;
int direction;
} inst;


void afficher_robot(Robot* R)
{
    generation_robot(R);
    SDL_Rect position;
    //save_table();
    position.x = (R->position_robot.x - 16.808*RAPPORT_LARGEUR - 15);
    position.y = (R->position_robot.y - 16.808*RAPPORT_HAUTEUR - 15);
    SDL_BlitSurface(R->robot, NULL, screen, &position);
}


static void generation_robot(Robot *R)
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
static void generation_droite(Coord position_1,Coord position_2, Robot* R, SDL_Surface* pixel)
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



/*------------------------------ geometrie -----------------*/


static float modulo_2pi(float a)
{
if(a<0)
    return 2*3.14159265358979323846 +a;
else if(a>2*3.14159265358979323846)
    return a-2*3.14159265358979323846;
else
    return a;

}



