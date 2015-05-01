#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "mouvement.h"
#include "comAsser.h"
#include "debug.h"

void initBumper() {
    TRIS_BUMPER_DROITE = 1;
    TRIS_BUMPER_GAUCHE = 1;
}

bool colision() {
    return BUMPER_DROITE && BUMPER_GAUCHE;
}

void caller(Coordonne_mm postCallageX, Coordonne_mm postCallageY) {
    affiche_debug(DEBUG_MSG | 0b10101010);
    while (!colision()) {    // on rentre en butée
        sendDelta(DISTANCE_CALLAGE);
    }
    affiche_debug(DEBUG_MSG | 0b10101010);

    sendSetXY(postCallageX,postCallageY);   // On définit la position sûre
    
    Angle actualAlpha = (int)(getAlphaAbs()) %360, nextAlpha;    // On teste l'angle connu
    if (actualAlpha < 45) {             // entre 0,45
        nextAlpha = 0;
    } else if (actualAlpha < 135) {     // autour de 90
        nextAlpha = 90;
    } else if (actualAlpha < 225) {     // autour de 180
        nextAlpha = 180;
    } else if (actualAlpha < 315) {     // autour de 270
        nextAlpha = 270;
    } else {                            // au-delà de 270 
        nextAlpha = 0;
    }
    sendSetAlpha(nextAlpha);
}

void bouger(Coordonne_mm x, Coordonne_mm y, TypeMvt mouvement) {
    Coordonne_mm deltaX = x-getPosX();
    Coordonne_mm deltaY = y-getPosY();
    Coordonne_mm deplacement;
    Angle nextAlpha;
    //  On définit d'abord les angles pour le mouvement "avancer", puis on gère le type de mvt.

    if (abs(deltaX) < ERR_SECUR_XY) {
        if (deltaY >  ERR_SECUR_XY) {        // Rotation pour se mettre dans le sens de la marche
            nextAlpha=90;
        }
        if (deltaY < -ERR_SECUR_XY) {
            nextAlpha=270;
        }
        deplacement = abs(deltaY);      // Distance de déplacement relatif selon Y, dans ce cas

    } else if (abs(deltaY) < ERR_SECUR_XY) {
        if (deltaX >  ERR_SECUR_XY){
            nextAlpha=0;
        }
        if (deltaY < -ERR_SECUR_XY){
            nextAlpha=180;
        }
        deplacement = abs(deltaX);

    } else {
        // Angle zarb :(
        nextAlpha = atan(deltaX/deltaY);
        deplacement=sqrtf(deltaX*deltaX+deltaY*deltaY);
    }

    if (mouvement == RECULER){
        nextAlpha = 360 - nextAlpha;    // Super simple, non ? :)
    }
    if (mouvement == BOUGER) {          
        if (abs(nextAlpha - getAlphaAbs()) > 180){
            nextAlpha = 360 - nextAlpha;// Moins simple pour un mouvement non précisé…
        }
    }

    // Fini de jouer, on se déplace.


    if (deplacement<ERR_SECUR_XY) {     // Si on n'est pas trop proche de la cible…
        if (abs(nextAlpha - getAlphaAbs())<ROTATION_MIN) {  // Si l'angle de rotation n'est pas trop faible…
            sendAlphaAbs(nextAlpha);                        // On tourne.
            while (!assertAtteint()) {
                ; }
        }

        sendDelta(deplacement);         // On se déplace.
        while (!assertAtteint()) {
            ; }
    }
}
