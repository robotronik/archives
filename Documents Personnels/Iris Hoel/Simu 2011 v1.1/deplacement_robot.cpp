
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
extern Robot *R1, *R2;
extern Inst instructionRobot1;
extern Inst instructionRobot2;
extern int done1;
extern int done2;

Robot *configuration_initiale_robot(Uint32 couleur, Coord position)
{
    Robot* R = NULL;
    R = (Robot*) malloc(sizeof(Robot));
    R->direction_robot = 0;  // angle en degré
    R->position_robot.x = position.x;  // position du robot sur la table
    R->position_robot.y = position.y;
    R->angle_1.x = 8.308*RAPPORT_LARGEUR+15; // position des différenets points du robot à l'intérieur du carré de celui-ci
    R->angle_1.y = 2.308*RAPPORT_HAUTEUR+15;
    R->angle_2.x = 25.308*RAPPORT_LARGEUR+15;
    R->angle_2.y = 2.308*RAPPORT_HAUTEUR+15;
    R->angle_3.x = 25.308*RAPPORT_LARGEUR+15;
    R->angle_3.y = 31.308*RAPPORT_HAUTEUR+15;
    R->angle_4.x = 8.308*RAPPORT_LARGEUR+15;
    R->angle_4.y = 31.308*RAPPORT_HAUTEUR+15;
    R->angle_inter_1.x = 4.796*RAPPORT_LARGEUR+15;
    R->angle_inter_1.y = 5.819*RAPPORT_HAUTEUR+15;
    R->angle_inter_2.x = 28.819*RAPPORT_LARGEUR+15;
    R->angle_inter_2.y = 5.819*RAPPORT_HAUTEUR+15;
    R->angle_inter_3.x = 28.819*RAPPORT_LARGEUR+15;
    R->angle_inter_3.y = 27.796*RAPPORT_HAUTEUR+15;
    R->angle_inter_4.x = 4.796*RAPPORT_LARGEUR+15;
    R->angle_inter_4.y = 27.796*RAPPORT_HAUTEUR+15;
    R->arc_cercle_1_1.x = 4.796*RAPPORT_LARGEUR+15;
    R->arc_cercle_1_1.y = 6.319*RAPPORT_HAUTEUR+15;
    R->arc_cercle_1_2.x = 4.796*RAPPORT_LARGEUR+15;
    R->arc_cercle_1_2.y = 27.296*RAPPORT_HAUTEUR+15;
    R->arc_cercle_2_1.x = 28.819*RAPPORT_LARGEUR+15;
    R->arc_cercle_2_1.y = 6.319*RAPPORT_HAUTEUR+15;
    R->arc_cercle_2_2.x = 28.819*RAPPORT_LARGEUR+15;
    R->arc_cercle_2_2.y = 27.296*RAPPORT_HAUTEUR+15;
    R->direction_1.x = (16.808 +16.808)*RAPPORT_LARGEUR+15;
    R->direction_1.y = 16.808*RAPPORT_HAUTEUR+15;
    R->direction_2.x = (12 +16.808)*RAPPORT_LARGEUR+15;
    R->direction_2.y = 16.808*RAPPORT_HAUTEUR+15;
    R->rayon_arc = 10.5;
    R->centre_arc_1.x = 4.308*RAPPORT_LARGEUR+15;
    R->centre_arc_1.y = 16.808*RAPPORT_HAUTEUR+15;
    R->centre_arc_2.x = 29.308*RAPPORT_LARGEUR+15;
    R->centre_arc_2.y = 16.808*RAPPORT_HAUTEUR+15;
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
    R->detection_avant_inter = -1;
    R->detection_arriere_inter = -1;
    R->saisie_avant_ok = -1;
    R->saisie_arriere_ok = -1;

    R->temp_detection_avant = - 1;
    R->temp_detection_arriere = -1;



    generation_robot(R);


    return R;
}

void traiter_inst(Inst* instruction_en_cour, Robot *R)
{
    if((*instruction_en_cour).type == FERMER_ARRIERE)
    {
        (*instruction_en_cour).valeur -= tourner_arriere(-1,(*instruction_en_cour).vitesse, R);
        //R->saisie_arriere_ok = R->detection_arriere_inter;
        //log("fermer arriere");
    }
    else if((*instruction_en_cour).type == LIBERER_ARRIERE)
    {
        (*instruction_en_cour).valeur -= tourner_arriere(+1,(*instruction_en_cour).vitesse, R);
        //R->saisie_arriere_ok = -1;
    }
    else if((*instruction_en_cour).type == FERMER_AVANT)
    {
        (*instruction_en_cour).valeur -= tourner_avant(-1,(*instruction_en_cour).vitesse, R);
        //R->saisie_avant_ok = R->detection_avant_inter;
    }
    else if((*instruction_en_cour).type == LIBERER_AVANT)
    {
        (*instruction_en_cour).valeur -= tourner_avant(+1,(*instruction_en_cour).vitesse, R);
        //R->saisie_avant_ok = -1;
       // log("liberer avant, val :  ");
       // log_arg((*instruction_en_cour).valeur);
    }
    else if ((*instruction_en_cour).type == AVANCER)
    {
        (*instruction_en_cour).valeur -= avancer((*instruction_en_cour).signe,(*instruction_en_cour).vitesse, R);
        interaction(*instruction_en_cour,R);
    }
    else if ((*instruction_en_cour).type == TOURNER)
    {
        (*instruction_en_cour).valeur -= tourner((*instruction_en_cour).signe,(*instruction_en_cour).vitesse, R);
        interaction(*instruction_en_cour,R);
    }
}

void fin_inst(Inst instruction_en_cour, Robot* R)
{
    if(instruction_en_cour.type == FERMER_ARRIERE)
        R->saisie_arriere_ok = R->detection_arriere_inter;
    else if(instruction_en_cour.type == LIBERER_ARRIERE)
        R->saisie_arriere_ok = -1;
    else if(instruction_en_cour.type == FERMER_AVANT)
        R->saisie_avant_ok = R->detection_avant_inter;
    else if(instruction_en_cour.type == LIBERER_AVANT)
        R->saisie_avant_ok = -1;
//save_inst_precedente(instruction_en_cour.type);
}

void deplacement()
{
instructionRobot2.type = AVANCER;
instructionRobot2.valeur = 10;
instructionRobot2.vitesse =0;
instructionRobot2.signe =1;
afficher_robot(R1,R2);
int n = 1;
machine();
while(1)
{
    if (n == 1)
    {
        if(instructionRobot1.valeur !=0 )
        {
            if(instructionRobot1.valeur > instructionRobot1.vitesse)
            {
                traiter_inst(&instructionRobot1,R1);
                n = 2;
            }
            else
            {
                if(instructionRobot1.valeur != 0)
                {
                    instructionRobot1.vitesse = instructionRobot1.valeur;
                    traiter_inst(&instructionRobot1,R1);
                    instructionRobot1.valeur = 0;
                }
                fin_inst(instructionRobot1,R1);
                done1 ++;
                machine();
                n = 2;
            }
        }
        else
            n = 2;
    }
    if (n == 2)
    {
        //instructionRobot2.valeur =10;
        //Joystick();
        //traiter_inst(&instructionRobot2,R2);
        n = 1;
    }
//log("instruction ");
//log_arg(instruction_en_cour_1.type);
//log_arg(instruction_en_cour_1.valeur);
    SDL_Delay(DELAY);
    afficher_robot(R1,R2);
    interuption_simu();
}
  /*  SDL_Delay(500);
    gagner(R1);
    afficher_robot(R1,R2);*/
}


float avancer(float signe, float vitesse, Robot *R)
{
    R->position_robot.x += signe*vitesse*cos(R->direction_robot)*RAPPORT_LARGEUR;
    R->position_robot.y += signe*vitesse*sin(R->direction_robot)*RAPPORT_HAUTEUR;
    return vitesse;
}


void sous_tourner(float xc, float yc, float coss, float sinn, Coord* surface)
{
   float save = coss*((*surface).x - xc) - sinn*((*surface).y - yc) + xc;
    (*surface).y = sinn*((*surface).x - xc) + coss*((*surface).y - yc) + yc;
    (*surface).x = save;
}


float tourner_a_180(Robot* R)
{
    float xc = 16.808*RAPPORT_LARGEUR+15;
    float yc = 16.808*RAPPORT_HAUTEUR+15;
    float coss = cos(M_PI);
    float sinn = sin(M_PI);
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
    R->direction_robot = modulo_2pi(R->direction_robot+M_PI);
    generation_robot(R);
}


float tourner(float signe, float vitesse, Robot* R)
{
    float xc = 16.808*RAPPORT_LARGEUR+15;
    float yc = 16.808*RAPPORT_HAUTEUR+15;
    float d = signe*vitesse*rad;
    float coss = cos(d);
    float sinn = sin(d);
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
    R->direction_robot = modulo_2pi(R->direction_robot + d);
    //log(" direction");
    //log_arg(d);
    //log_arg(R->direction_robot);
    generation_robot(R);
    return vitesse;

}


float tourner_avant(float signe, float vitesse, Robot* R)
{
   float coss = cos(signe*vitesse*rad);
    float sinn = sin(-1*signe*vitesse*rad);


    sous_tourner(R->arc_cercle_2_1.x,R->arc_cercle_2_1.y,coss,sinn, &R->pince_avant_1);
    sous_tourner(R->arc_cercle_2_2.x,R->arc_cercle_2_2.y,coss,-sinn, &R->pince_avant_2);
    generation_robot(R);
return vitesse;

}


float tourner_arriere(float signe, float vitesse, Robot* R)
{
    float coss = cos(-1*signe*vitesse*rad);
    float sinn = sin(signe*vitesse*rad);


    sous_tourner(R->arc_cercle_1_1.x,R->arc_cercle_1_1.y,coss,sinn, &R->pince_arriere_1);
    sous_tourner(R->arc_cercle_1_2.x,R->arc_cercle_1_2.y,coss,-sinn, &R->pince_arriere_2);
    generation_robot(R);
return vitesse;

}

void par_defaut(inst* inst_temp)
{
    if((*inst_temp).type == -1)
        exit(0);
    switch((*inst_temp).type)
    {
        case AVANCER:
            if((*inst_temp).signe == 0)
                (*inst_temp).signe = 1;
            if((*inst_temp).vitesse == 0)
                (*inst_temp).vitesse = VITESSE_TRANS_ROBOT_DEFAUT;
        break;
        case TOURNER:
            if((*inst_temp).signe == 0)
                (*inst_temp).signe = 1;
            if((*inst_temp).vitesse == 0)
                (*inst_temp).vitesse = VITESSE_ROT_ROBOT_DEFAUT;
        break;
        default:
            if((*inst_temp).valeur == 0)
                (*inst_temp).valeur = ANGLE_PINCE_DEFAUT;
            if((*inst_temp).vitesse == 0)
                (*inst_temp).vitesse = VITESSE_PINCE_DEFAUT;
        break;
    }
}

