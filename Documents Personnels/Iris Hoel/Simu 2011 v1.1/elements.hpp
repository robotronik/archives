



# include "main.hpp"



void initialiser_pion();
int contact_arc_pion(float direction1, float direction2 , Elements pion, float xc, float yc, float r);
void capture_piece(Coord position_centre_arc, Coord centre_arc, Coord *pion);
void interaction_pion_pion(int n);
void interaction(inst instruction,Robot *R);
