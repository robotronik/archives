

# include "main.hpp"


void menu();
int recup_para(SDL_Event event);
void assignation_parametre(int n, int valeur);
void affichage_para(int valeur, int x, int y);
void affichage_couleur(int val);
int recup_para_couleur(SDL_Event event);
void automatisme_ligne(int * tab,int n);
int check_para(int * tab);
void affichage_erreur_saisie();
int affichage_saisie_ok();
void saisieFinie();
int menu_debug();
void interuption_simu();
void actualiserAffichage();
