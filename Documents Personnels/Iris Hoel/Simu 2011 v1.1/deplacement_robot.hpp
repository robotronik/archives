


# include "main.hpp"



Robot * configuration_initiale_robot(Uint32 couleur, Coord position);
void deplacement();
float avancer(float signe,float vitesse, Robot* R) ;
float tourner(float signe,float vitesse, Robot* R);
float tourner_a_180(Robot* R);
float tourner_avant(float signe, float vitesse, Robot* R);
float tourner_arriere(float signe, float vitesse, Robot* R);
void sous_tourner(float xc, float yc, float coss, float sinn, Coord* surface);
void par_defaut(inst* inst_temp);
