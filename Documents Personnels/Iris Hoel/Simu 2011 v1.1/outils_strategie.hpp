
# include "main.hpp"

// necessaire au reste du programme
llist inverser_inst_couleur(llist liste);
void ini_strategie();
void gagner(SDL_Surface *screen, Robot *R);



// necessaire a la stratégie


int position_grille_x(Robot *R);
int position_grille_y(Robot *R);
int direction_grille(Robot *R);
llist inverser_sens_avancer(llist liste);
int rotation_ok(Robot * R);
llist deposer_avant_derriere_proche(llist liste, int sens, inst *instruction_en_cour);
llist deposer_avant_derriere_loin(llist liste, int sens, inst *instruction_en_cour);
llist deposer_avant_devant_proche(llist liste, int sens, inst *instruction_en_cour);
llist deposer_avant_devant_loin(llist liste, int sens, inst *instruction_en_cour);
llist fermer_avant(llist liste, inst *instruction_en_cour);
llist fermer_arriere(llist liste, inst *instruction_en_cour);


// useless

void save_inst(llist liste, inst *instruction_en_cour);
