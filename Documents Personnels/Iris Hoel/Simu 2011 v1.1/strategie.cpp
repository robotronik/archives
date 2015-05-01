

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





llist strategie(llist liste, inst *instruction_en_cour, Robot *R)
{
int premiere_colonne = 1;
int deuxieme_colonne = 2;
int sens = 1;
    if(R->couleur_depart == BLEU)
    {
        sens = -1;
        premiere_colonne = 5;
        deuxieme_colonne = 4;
    }
    inst new_inst;
    if (R->detection_avant_inter != R->temp_detection_avant)
    {
        liste = fermer_avant(liste, instruction_en_cour);
        R->temp_detection_avant = R->detection_avant_inter;

    }
    else if (R->detection_arriere_inter != R->temp_detection_arriere)
    {

        liste = fermer_arriere(liste, instruction_en_cour);
        R->temp_detection_arriere = R->detection_arriere_inter;
    }

    else
    {


switch(R->etape)
{
    case 2:
    if( position_grille_x(R) == premiere_colonne && position_grille_y(R) != -1 && direction_grille(R) == BAS && R->saisie_avant_ok != -1)
    {
        if(position_grille_y(R) == 1 | position_grille_y(R) == 3)
        {
            liste = deposer_avant_devant_loin(liste,sens, instruction_en_cour);
        }
        else if(position_grille_y(R) == 2 | position_grille_y(R) == 4)
        {
            liste = deposer_avant_derriere_proche(liste,sens, instruction_en_cour);
        }
        R->etape = 3;
    }
    break;
    case 3:
    if( position_grille_x(R) == premiere_colonne && position_grille_y(R) == 5 && direction_grille(R) == BAS)
    {
        liste = deposer_avant_devant_proche(liste,sens, instruction_en_cour);  // zone safe
        R->etape = 4;
    }
    break;
    case 4:
    if( position_grille_x(R) == deuxieme_colonne && position_grille_y(R) != -1 && direction_grille(R) == HAUT && R->saisie_avant_ok != -1)
    {
        if(position_grille_y(R) == 5 )
        {
            liste = deposer_avant_derriere_loin(liste,sens, instruction_en_cour);
        }
        else if(position_grille_y(R) == 4 | position_grille_y(R) == 2)
        {
            liste = deposer_avant_derriere_loin(liste,-1*sens, instruction_en_cour);
        }
        else if(position_grille_y(R) == 3)
        {
            liste = deposer_avant_devant_loin(liste,-1*sens, instruction_en_cour);
        }
        R->etape = 5;
    }
    break;
    case 5:
    if(position_grille_x(R) == deuxieme_colonne && R->saisie_avant_ok == -1)
    {
        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        new_inst.type = TOURNER;
        new_inst.valeur = 180;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = inverser_sens_avancer(liste);
        R->etape = 6;
    }
    break;
    case 6:
    if(R->saisie_arriere_ok != -1 && rotation_ok(R))
    {
        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        new_inst.type = TOURNER;
        new_inst.valeur = 180;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = inverser_sens_avancer(liste);
        R->etape = 7;
    }
    break;
    default:
    break;
}
}
//position_grille_x(R);
//position_grille_y(R);
//direction_grille(R);

    return liste;

}

/*
llist strategie(llist liste, inst *instruction_en_cour, Robot *R)
{
int premiere_colonne = 1;
int deuxieme_colonne = 2;
int sens = 1;
    if(R->couleur_depart == BLEU)
    {
        sens = -1;
        premiere_colonne = 5;
        deuxieme_colonne = 4;
    }
    inst new_inst;
    if (R->detection_avant_inter != R->temp_detection_avant)
    {

        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        new_inst.type = FERMER_AVANT;
        new_inst.valeur = ANGLE_PINCE_DEFAUT;
        new_inst.vitesse = VITESSE_PINCE_DEFAUT;
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        R->temp_detection_avant = R->detection_avant_inter;

    }
    else if (R->detection_arriere_inter != R->temp_detection_arriere)
    {

        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        new_inst.type = FERMER_ARRIERE;
        new_inst.valeur = ANGLE_PINCE_DEFAUT;
        new_inst.vitesse = VITESSE_PINCE_DEFAUT;
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        R->temp_detection_arriere = R->detection_arriere_inter;
    }

    else
    {


switch(R->etape)
{
    case 2:
    if( position_grille_x(R) == premiere_colonne && position_grille_y(R) != -1 && direction_grille(R) == BAS && R->saisie_avant_ok != -1)
    {
            liste->val.valeur = (*instruction_en_cour).valeur;
            (*instruction_en_cour).valeur = 0;
        if(position_grille_y(R) == 1 | position_grille_y(R) == 3)
        {
            liste = deposer_avant_devant(liste,sens);
        }
        else if(position_grille_y(R) == 2 | position_grille_y(R) == 4)
        {
            liste = deposer_avant_derriere(liste,sens);
        }
    R->etape = 3;
    }
    break;
    case 3:
    if( position_grille_x(R) == premiere_colonne && position_grille_y(R) == 5 && direction_grille(R) == BAS)
    {
        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        liste = deposer_avant_devant(liste,sens);
        R->etape = 4;
    }
    break;
    case 4:
    if( position_grille_x(R) == deuxieme_colonne && position_grille_y(R) != -1 && direction_grille(R) == HAUT && R->saisie_avant_ok != -1)
    {
        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        if(position_grille_y(R) == 5 )
        {
            liste = deposer_avant_derriere(liste,sens);
        }
        else if(position_grille_y(R) == 4 | position_grille_y(R) == 2)
        {
            liste = deposer_avant_derriere(liste,-1*sens);
        }
        else if(position_grille_y(R) == 3)
        {
            liste = deposer_avant_devant(liste,-1*sens);
        }


        R->etape = 5;
    }
    break;
    case 5:
    if(position_grille_x(R) == deuxieme_colonne && R->saisie_avant_ok == -1)
    {
        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        new_inst.type = TOURNER;
        new_inst.valeur = 180;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = inverser_sens_avancer(liste);
        R->etape = 6;
    }
    break;
    case 6:
    if(R->saisie_arriere_ok != -1 && rotation_ok(R))
    {
        liste->val.valeur = (*instruction_en_cour).valeur;
        (*instruction_en_cour).valeur = 0;
        new_inst.type = TOURNER;
        new_inst.valeur = 180;
        new_inst.signe = sens;
        new_inst.vitesse = 0;
        par_defaut(&new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = ajouterEnTete(liste, new_inst);
        liste = inverser_sens_avancer(liste);
        R->etape = 7;
    }
    break;
    default:
    break;
}
}
//position_grille_x(R);
//position_grille_y(R);
//direction_grille(R);

    return liste;

}

//*/
