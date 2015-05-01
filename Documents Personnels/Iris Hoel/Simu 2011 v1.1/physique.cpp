
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

float distance_carre(float x1, float y1, float x2, float y2)
{
return ((x2-x1)*(x2-x1))/(RAPPORT_LARGEUR*RAPPORT_LARGEUR)+((y2-y1)*(y2-y1))/(RAPPORT_HAUTEUR*RAPPORT_HAUTEUR);
}


float direction2(float x1, float y1, float x2, float y2)
{
if(x2>=x1)
    return atan((y2-y1)/(x2-x1));
else
    return 3.14159265358979323846 - atan((y2-y1)/(x1-x2));
}

//*
float direction(float x1, float y1, float x2, float y2)
{
float a = atan(abso(y2-y1)/abso(x2-x1));
if(x2>=x1 && y2>=y1)
    return a;
else if(x2<x1 && y2>y1)
    return 3.14159265358979323846 - a;
else if(x2<=x1 && y2<=y1)
    return 3.14159265358979323846 + a;
else if(x2>x1 && y2<y1)
    return  3.14159265358979323846*2 - a;
else
    return 0;
}
//*/

float milieu(float x1, float x2)
{
return ((x2-x1)/2)+x1;
}


float abso(float a)
{
    return (a<0)? -a : a;
}


float modulo_2pi(float a)
{
if(a<0)
    return 2*3.14159265358979323846 +a;
else if(a>2*3.14159265358979323846)
    return a-2*3.14159265358979323846;
else
    return a;

}
