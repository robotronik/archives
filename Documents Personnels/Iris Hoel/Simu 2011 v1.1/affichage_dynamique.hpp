


# include "main.hpp"



void generation_pion(SDL_Surface* pion, Uint32 couleur);
void sous_affichage_elements(SDL_Surface * pion, Elements p);
void affichage_elements();
void afficher_robot(Robot* R1, Robot* R2);
void generation_robot(Robot *R);
void generation_droite(Coord position_1,Coord position_2, Robot* R, SDL_Surface* pixel);
void generation_cercle(float direction1, float direction2 , Coord centre, Robot * R, SDL_Surface* pixel);
void creer_info(char * infof);
void afficher_state(int i);
void afficher_info();
