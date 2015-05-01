
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
extern int CONFIG;
extern int DEBUG;

Elements tableau_p[19];




void initialiser_pion()
{

// change la config
    int i;
    tableau_p[0].position.x = 80*RAPPORT_LARGEUR;
    tableau_p[1].position.x = 80*RAPPORT_LARGEUR;
    tableau_p[2].position.x = 115*RAPPORT_LARGEUR;
    tableau_p[3].position.x = 115*RAPPORT_LARGEUR;
    int pre_col = CONFIG % 10;
    int deu_col = CONFIG / 10;
    if(pre_col == 1 | pre_col == 2 | pre_col == 3 | pre_col == 4)
    {

        tableau_p[0].position.y = 35*RAPPORT_HAUTEUR;
    }
    else if(pre_col == 5 | pre_col == 6 | pre_col == 7)
    {

        tableau_p[0].position.y = 70*RAPPORT_HAUTEUR;
    }
    else if(pre_col == 8 | pre_col == 9 )
    {

        tableau_p[0].position.y = 105*RAPPORT_HAUTEUR;
    }
    else if(pre_col == 0)
    {
        tableau_p[0].position.y = 140*RAPPORT_HAUTEUR;
    }
    if(pre_col == 1)
    {
        tableau_p[1].position.y = 70*RAPPORT_HAUTEUR;
    }
    else if(pre_col == 2 | pre_col == 5)
    {
        tableau_p[1].position.y = 105*RAPPORT_HAUTEUR;
    }
    else if(pre_col == 3 | pre_col == 6 | pre_col == 8)
    {
        tableau_p[1].position.y = 140*RAPPORT_HAUTEUR;
    }
    else if(pre_col == 4 | pre_col == 7| pre_col == 9| pre_col == 0)
    {
        tableau_p[1].position.y = 175*RAPPORT_HAUTEUR;
    }

/////////////////////
    if(deu_col == 1 | deu_col == 2 | deu_col == 3 | deu_col == 4)
    {

        tableau_p[2].position.y = 35*RAPPORT_HAUTEUR;
    }
    else if(deu_col == 5 | deu_col == 6 | deu_col == 7)
    {

        tableau_p[2].position.y = 70*RAPPORT_HAUTEUR;
    }
    else if(deu_col == 8 | deu_col == 9 )
    {

        tableau_p[2].position.y = 105*RAPPORT_HAUTEUR;
    }
    else if(deu_col == 0)
    {
        tableau_p[2].position.y = 140*RAPPORT_HAUTEUR;
    }
    if(deu_col == 1)
    {
        tableau_p[3].position.y = 70*RAPPORT_HAUTEUR;
    }
    else if(deu_col == 2 | deu_col == 5)
    {
        tableau_p[3].position.y = 105*RAPPORT_HAUTEUR;
    }
    else if(deu_col == 3 | deu_col == 6 | deu_col == 8)
    {
        tableau_p[3].position.y = 140*RAPPORT_HAUTEUR;
    }
    else if(deu_col == 4 | deu_col == 7| deu_col == 9| deu_col == 0)
    {
        tableau_p[3].position.y = 175*RAPPORT_HAUTEUR;
    }
/////////////////////////

/*
    else if(CONFIG == 2)
    {
        tableau_p[0].position.x = 115*RAPPORT_LARGEUR;
        tableau_p[0].position.y = 35*RAPPORT_HAUTEUR;
        tableau_p[1].position.x = 115*RAPPORT_LARGEUR;
        tableau_p[1].position.y = 70*RAPPORT_HAUTEUR;
        tableau_p[2].position.x = 150*RAPPORT_LARGEUR;
        tableau_p[2].position.y = 35*RAPPORT_HAUTEUR;
        tableau_p[3].position.x = 115*RAPPORT_LARGEUR;
        tableau_p[3].position.y = 105*RAPPORT_HAUTEUR;

    }*/
// ne change pas selon la config
    for(i=4; i<=7; i++)
    {
        tableau_p[i].position.x = (150*RAPPORT_LARGEUR-tableau_p[i-4].position.x)+150*RAPPORT_LARGEUR;
        tableau_p[i].position.y = tableau_p[i-4].position.y;
    }
    tableau_p[8].position.y = 70*RAPPORT_HAUTEUR;
    tableau_p[8].position.x = 20*RAPPORT_LARGEUR;
    for(i=9; i<=12; i++)
    {
        tableau_p[i].position.x = 20*RAPPORT_LARGEUR;
        tableau_p[i].position.y = tableau_p[i-1].position.y + 28*RAPPORT_HAUTEUR;
    }
    tableau_p[13].position.y = 70*RAPPORT_HAUTEUR;
    tableau_p[13].position.x = 280*RAPPORT_LARGEUR;
    for(i=14; i<=17; i++)
    {
        tableau_p[i].position.x = 280*RAPPORT_LARGEUR;
        tableau_p[i].position.y = tableau_p[i-1].position.y + 28*RAPPORT_HAUTEUR;
    }

    tableau_p[18].position.x = 150*RAPPORT_HAUTEUR;
    tableau_p[18].position.y = 105*RAPPORT_LARGEUR;




    for(i=0; i<19; i++)
    {
        tableau_p[i].couleur = COULEUR_BASE_PION;

    }

}



void interaction(inst instruction,Robot *R)
{
    SDL_Surface *pixel = SDL_CreateRGBSurface(SDL_HWSURFACE, EPAISSEUR_PIXEL, EPAISSEUR_PIXEL, 32, 0, 0, 0, 0);
    int i;
    float d_rad = R->direction_robot;
    float d = d_rad*degre;
    for(i=0; i<19; i ++)
    {
       if (R->saisie_avant_ok == i | R->saisie_arriere_ok == i )           // tu es déjà saisie par le robot
        {
            if(instruction.type == AVANCER)
            {
                tableau_p[i].position.x += instruction.signe*instruction.vitesse*cos(d_rad)*RAPPORT_LARGEUR;// alors tu avance en même temp que le robot
                tableau_p[i].position.y += instruction.signe*instruction.vitesse*sin(d_rad)*RAPPORT_HAUTEUR;
                interaction_pion_pion(i);
            }
            else if(instruction.type == TOURNER)
            {
                sous_tourner(R->position_robot.x,R->position_robot.y,cos(instruction.signe*instruction.vitesse*rad),
                             sin(instruction.signe*instruction.vitesse*rad), &tableau_p[i].position);
                interaction_pion_pion(i);
            }


        }
        else if(distance_carre(R->position_robot.x,R->position_robot.y,tableau_p[i].position.x,tableau_p[i].position.y)<(10+16.8)*(10+16.8))
        {
            Coord position_haut_robot;
            position_haut_robot.x = R->position_robot.x-16.808*RAPPORT_LARGEUR-15;
            position_haut_robot.y = R->position_robot.y- 16.808*RAPPORT_HAUTEUR-15;
            float d1 = direction(position_haut_robot.x + R->arc_cercle_2_2.x, position_haut_robot.y + R->arc_cercle_2_2.y,
                                 tableau_p[i].position.x,tableau_p[i].position.y);
            float d2 = direction(position_haut_robot.x + R->arc_cercle_2_1.x, position_haut_robot.y + R->arc_cercle_2_1.y,
                                 tableau_p[i].position.x,tableau_p[i].position.y);

            if((d1<=d_rad && d_rad<=d2) | (d2<=(d_rad - 3.14159265358979323846) && (d_rad - 3.14159265358979323846)<=d1)
                    | (d2<=(d_rad + 3.14159265358979323846) && (d_rad + 3.14159265358979323846)<=d1) | (d2<=d_rad && d_rad<=d1) )
            {                                                                   // tu es dans les directions de capture du robot
                tableau_p[i].couleur = SDL_MapRGB(pixel->format, 255, 255, 255);                 // couleur -> blanc
                if(contact_arc_pion( d-88,d+88,tableau_p[i],position_haut_robot.x+R->centre_arc_1.x,
                                     position_haut_robot.y+R->centre_arc_1.y , R->rayon_arc))            // tu touche le demi cercle 1, arrière du robot
                {
                    if( abso(d_rad - modulo_2pi(direction(R->position_robot.x,R->position_robot.y,
                                                          tableau_p[i].position.x,tableau_p[i].position.y)- 3.14159265358979323846)) < 0.015)
                    {
                        // tu es en position de capture : confondu avec l'arc (0.013 = precision par rapport au centre de l'arc du robot)
                        tableau_p[i].position.x += instruction.signe*instruction.vitesse*cos(d_rad)*RAPPORT_LARGEUR;
                        tableau_p[i].position.y += instruction.signe*instruction.vitesse*sin(d_rad)*RAPPORT_HAUTEUR;
                        R->detection_arriere_inter = i;// alors detection d'un pion en position pour saisie -> stratégie
                    }
                    else if(instruction.type == AVANCER && instruction.signe == -1)// instruction provoquant la capture (robot recule)
                    {
                        tableau_p[i].position.x += instruction.signe*instruction.vitesse*cos(d_rad)*RAPPORT_LARGEUR;
                        tableau_p[i].position.y += instruction.signe*instruction.vitesse*sin(d_rad)*RAPPORT_HAUTEUR;
                        Coord position_centre_arc;
                        position_centre_arc.x = position_haut_robot.x+R->centre_arc_1.x;    // alors tu avance en même temp que le robot et
                        position_centre_arc.y = position_haut_robot.y+R->centre_arc_1.y;
                        tableau_p[i].position.x -= instruction.vitesse*cos(direction(position_centre_arc.x,position_centre_arc.y,tableau_p[i].position.x,tableau_p[i].position.y));
                        tableau_p[i].position.y -= instruction.vitesse*sin(direction(position_centre_arc.x,position_centre_arc.y,tableau_p[i].position.x,tableau_p[i].position.y));
                    }


                }
                else if(contact_arc_pion(d+93,d+267,tableau_p[i],position_haut_robot.x+R->centre_arc_2.x,
                                         position_haut_robot.y+R->centre_arc_2.y , R->rayon_arc))       // tu touche le demi cercle 2, avant du robot
                {

                    if( abso(d_rad - direction(R->position_robot.x,R->position_robot.y, tableau_p[i].position.x,tableau_p[i].position.y )) < 0.015) // dif a mettre en valeur abs
                    {
                        // tu es en position de capture : confondu avec l'arc (0.013 = precision par rapport au centre de l'arc du robot)
                        tableau_p[i].position.x += instruction.signe*instruction.vitesse*cos(d_rad)*RAPPORT_LARGEUR;
                        tableau_p[i].position.y += instruction.signe*instruction.vitesse*sin(d_rad)*RAPPORT_HAUTEUR;
                        R->detection_avant_inter = i; // alors detection d'un pion en position pour saisie -> stratégie
                    }
                    else if(instruction.type == AVANCER && instruction.signe == +1)   // instruction provoquant la capture (robot avance)
                    {
                        tableau_p[i].position.x += instruction.signe*instruction.vitesse*cos(d_rad)*RAPPORT_LARGEUR;
                        tableau_p[i].position.y += instruction.signe*instruction.vitesse*sin(d_rad)*RAPPORT_HAUTEUR;
                        Coord position_centre_arc_2;
                        position_centre_arc_2.x = position_haut_robot.x+R->centre_arc_2.x;    // alors tu avance en même temp que le robot et
                        position_centre_arc_2.y = position_haut_robot.y+R->centre_arc_2.y;
                        tableau_p[i].position.x -= instruction.vitesse*cos(direction(position_centre_arc_2.x,position_centre_arc_2.y,tableau_p[i].position.x,tableau_p[i].position.y));
                        tableau_p[i].position.y -= instruction.vitesse*sin(direction(position_centre_arc_2.x,position_centre_arc_2.y,tableau_p[i].position.x,tableau_p[i].position.y));
                    }
                }
            }
            else   // ejecte du robot
            {
                tableau_p[i].position.x += cos(direction(R->position_robot.x,R->position_robot.y,tableau_p[i].position.x,tableau_p[i].position.y));
                tableau_p[i].position.y += sin(direction(R->position_robot.x,R->position_robot.y,tableau_p[i].position.x,tableau_p[i].position.y));
                tableau_p[i].couleur = SDL_MapRGB(pixel->format, 255, 255, 0);
            }
        interaction_pion_pion(i);
        }
        else;      // pas ou plus d'interaction -> piece en gris
           // tableau_p[i].couleur = SDL_MapRGB(pixel->format, 200, 200, 200);

    }
    SDL_FreeSurface(pixel);
};


int contact_arc_pion(float direction1, float direction2 , Elements pion, float xc, float yc, float r)
{

    int i;
    for(i=direction1; i<=direction2; i++)
    {
        if(distance_carre(xc + r*cos(i*rad)*RAPPORT_LARGEUR, yc + r*sin(i*rad)*RAPPORT_HAUTEUR, pion.position.x, pion.position.y)
                < 10.3*10.3)    // vitesse de capture d'une pièce
            return 1;

    }
    return 0;

}


void interaction_pion_pion(int n)
{
int i;

for(i=0; i<19; i ++)
{

    if((distance_carre(tableau_p[n].position.x,tableau_p[n].position.y, tableau_p[i].position.x, tableau_p[i].position.y)<(20*20)) && i != n)
    {
        tableau_p[i].position.x += cos(direction(tableau_p[n].position.x,tableau_p[n].position.y,tableau_p[i].position.x,tableau_p[i].position.y));
        tableau_p[i].position.y += sin(direction(tableau_p[n].position.x,tableau_p[n].position.y,tableau_p[i].position.x,tableau_p[i].position.y));
        interaction_pion_pion(i);
    }

}

}
