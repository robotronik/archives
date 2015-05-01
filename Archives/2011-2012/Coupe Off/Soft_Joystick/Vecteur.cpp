#include "Vecteur.h"

Vecteur::Vecteur() : X(0), Y(0)
{
}

float Vecteur::getLengthSQ()
{
    return X*X + Y*Y;
}

void Vecteur::normalize()
{
    float norme = sqrt(getLengthSQ());
    if(norme != 0)
    {
        X /= norme;
        Y /= norme;
    }
}
