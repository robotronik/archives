

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
extern int num_strat;

extern Elements tableau_p[19];



//Case grille[6][6];
void ini_strategie()
{
/*
int i = 0, j = 0;
for(i=0; i<6;i++)
grille[i][i].occupe = 0;
for(i=0; i<6;i++)
    for(j=0; j<6; j++)
    {
        grille[i][j].position.x = i;
        grille[i][j].position.y = j;
    }
for(i=0; i<6;i +=2)
    for(j=0; j<6; j +=2)
    {
        grille[i][j].couleur = BLEU;
    }
for(i=1; i<6;i +=2)
    for(j=1; j<6; j +=2)
    {
        grille[i][j].couleur = ROUGE;
    }
*/
//creer_info_position(-1,-1);
}

void gagner(SDL_Surface *screen, Robot *R)
{
    if(R->etape == 7 && R->position_robot.x >= 15*RAPPORT_LARGEUR && R->position_robot.x <= 25*RAPPORT_LARGEUR
        && R->position_robot.y >= 15*RAPPORT_HAUTEUR && R->position_robot.y <= 25*RAPPORT_HAUTEUR && R->saisie_arriere_ok != -1 && R->couleur_depart == ROUGE )
    {
        log("Objectifs remplis !");
        creer_info("gagné !  :P");
    }
    else if(R->etape == 7 && R->position_robot.x >= 275*RAPPORT_LARGEUR && R->position_robot.x <= 285*RAPPORT_LARGEUR
        && R->position_robot.y >= 15*RAPPORT_HAUTEUR && R->position_robot.y <= 25*RAPPORT_HAUTEUR && R->saisie_arriere_ok != -1 && R->couleur_depart == BLEU )
    {
        log("Objectifs remplis !");
        creer_info("gagné !  :P");
    }

}

//----------------------

int position_grille_x(Robot *R)
{
    int i = 0;
for(i=0;i<=6;i++)
{
    if(R->position_robot.x >= (44.4+35*i)*RAPPORT_LARGEUR && R->position_robot.x <= (45.6+35*i)*RAPPORT_LARGEUR)
    {
    if(R->num ==1 )
    creer_info_position(i,-2);
    return i;
    }
}
if(R->num ==1 )
creer_info_position(-1,-2);
return -1;
}

int position_grille_y(Robot *R)
{
    int i = 0;
for(i=0;i<=5;i++)
{
    if(R->position_robot.y >= (35*i-0.6)*RAPPORT_HAUTEUR && R->position_robot.y <= (35*i+0.6)*RAPPORT_HAUTEUR)
    {
    if(R->num ==1 )
    creer_info_position(-2,i);
    return i;
    }
}
if(R->num ==1 )
creer_info_position(-2,-1);
return -1;
}

int direction_grille(Robot *R)
{
    if(R->direction_robot <= 1.58 && R->direction_robot >= 1.57 )
        return BAS;
    else if(R->direction_robot <= 3.15 && R->direction_robot >= 3.14 )
        return GAUCHE;
    else if(R->direction_robot <= 4.72 && R->direction_robot >= 4.71 )
        return HAUT;
    else if(R->direction_robot <= 0.01 | R->direction_robot >= 6.27)
        return DROITE;
return -1;
}

//----------------------

llist inverser_inst_couleur(llist liste)
{
llist liste_save = liste;
liste = rechercherElement(liste, TOURNER);
while(liste != NULL)
{
    liste->val.signe *= -1;
    liste = liste->nxt;
    liste = rechercherElement(liste, TOURNER);
}

return liste_save;
}

llist inverser_sens_avancer(llist liste)
{
llist liste_save = liste;
liste = rechercherElement(liste, AVANCER);
while(liste != NULL)
{
    liste->val.signe *= -1;
    liste = liste->nxt;
    liste = rechercherElement(liste, AVANCER);
}

return liste_save;
}

int rotation_ok(Robot * R)
{
int ret = 1, i =0;
int distance_min = (10+23)*(10+23);
for(i=0; i < 19; i++ )
if( R->saisie_avant_ok != i && R->saisie_arriere_ok != i && (distance_carre(R->position_robot.x,R->position_robot.y,tableau_p[i].position.x,tableau_p[i].position.y) < distance_min))
    ret = 0;
return ret;

}

//----------------------

void save_inst(llist liste, inst *instruction_en_cour)
{
    liste->val.valeur = (*instruction_en_cour).valeur;
    (*instruction_en_cour).valeur = 0;
}

llist deposer_avant_derriere_proche(llist liste, int sens, inst *instruction_en_cour)
{
        save_inst(liste, instruction_en_cour);
        inst new_inst;
        new_inst.type = TOURNER;
        new_inst.valeur = 108;
        new_inst.signe = -1*sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 15;
        new_inst.signe = -1;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = LIBERER_AVANT;
        new_inst.valeur = 0;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 15;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = TOURNER;
        new_inst.valeur = 108;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        return liste;

}

llist deposer_avant_derriere_loin(llist liste, int sens, inst *instruction_en_cour)
{
        save_inst(liste, instruction_en_cour);
        inst new_inst;
        new_inst.type = TOURNER;
        new_inst.valeur = 135;
        new_inst.signe = -1*sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 25;
        new_inst.signe = -1;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = LIBERER_AVANT;
        new_inst.valeur = 0;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 25;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = TOURNER;
        new_inst.valeur = 135;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        return liste;

}

llist deposer_avant_devant_proche(llist liste, int sens, inst *instruction_en_cour)
{
        save_inst(liste, instruction_en_cour);
        inst new_inst;
        new_inst.type = TOURNER;
        new_inst.valeur = 72;
        new_inst.signe = -1*sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 15;
        new_inst.signe = -1;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = LIBERER_AVANT;
        new_inst.valeur = 0;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 15;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = TOURNER;
        new_inst.valeur = 72;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        return liste;
}

llist deposer_avant_devant_loin(llist liste, int sens, inst *instruction_en_cour)
{
        save_inst(liste, instruction_en_cour);
        inst new_inst;
        new_inst.type = TOURNER;
        new_inst.valeur = 45;
        new_inst.signe = -1*sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 25;
        new_inst.signe = -1;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = LIBERER_AVANT;
        new_inst.valeur = 0;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = AVANCER;
        new_inst.valeur = 25;
        new_inst.signe = 0;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        new_inst.type = TOURNER;
        new_inst.valeur = 45;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        return liste;
}

llist fermer_avant(llist liste, inst *instruction_en_cour)
{
        save_inst(liste, instruction_en_cour);
    inst new_inst;
    new_inst.type = FERMER_AVANT;
    new_inst.valeur = ANGLE_PINCE_DEFAUT;
    new_inst.vitesse = VITESSE_PINCE_DEFAUT;
    liste = ajouterEnTete(liste, new_inst);
    liste = ajouterEnTete(liste, new_inst);

return liste;


}

llist fermer_arriere(llist liste, inst *instruction_en_cour)
{
    save_inst(liste, instruction_en_cour);
    inst new_inst;
    new_inst.type = FERMER_ARRIERE;
    new_inst.valeur = ANGLE_PINCE_DEFAUT;
    new_inst.vitesse = VITESSE_PINCE_DEFAUT;
    liste = ajouterEnTete(liste, new_inst);
    liste = ajouterEnTete(liste, new_inst);

return liste;


}

