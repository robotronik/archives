#include "main.h"
#include "coordonees.h"

Coordonne_mm calculX(Couleur c, Coordonne_mm relativeX) {
    if (c == JAUNE)
        return EST - relativeX;
    else
        return OUEST + relativeX;
}

Couleur cOppose (Couleur c) {
    if (c == JAUNE)
        return ROUGE;
    else
        return JAUNE;

}
