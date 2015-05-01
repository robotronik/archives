#ifndef VECTEUR_H
#define VECTEUR_H

#include <cmath>

class Vecteur
{
    public:
        Vecteur();

        float getLengthSQ();
        void normalize();
        void multScal(float s) {X*=s; Y*=s;}

        float X;
        float Y;
};

#endif // VECTEUR_H
