#include "main.h"
#include "strategie.h"
#include "mouvement.h"
#include "coordonees.h"

#include "debug.h"
#include "comAsser.h"
#include "sharp.h"
#include "timer.h"

void evitementObstacle(InfoMvt *infoMvt) {
    if (getDistance(!(infoMvt->sensMvt)) > DISTANCE_PROXIMITE) { // On peut manœvrer
        // On recule, on se tourne de 90° et on avance
        //        Angle currentAlpha = getAlphaAbs();

        sendDelta(-DISTANCE_EVITEMENT);
        sendAlpha(90);
        if (getDistance(infoMvt->sensMvt) > DISTANCE_PROXIMITE) {
            sendDelta(+DISTANCE_EVITEMENT);
        } else {
            // On est un peu coinsé !
        }
    } else {
        // On tourne sur place
        sendAlpha(45);
        if (getDistance(infoMvt->sensMvt) > DISTANCE_PROXIMITE) {
            pause_ms(1000);
        }
    }
}

void chercheObjectif(Objectif obj[], int nbObj, Couleur couleur) {
    static int indiceCurrentObj = 0;
    static bool attendFinMatch = false;

    int nextX, nextY;
    int currentX = getPosX();
    int currentY = getPosY();

    nextX = calculX(couleur,obj[indiceCurrentObj].nextX);
    nextY = obj[indiceCurrentObj].nextY;

    // On regarde si on a atteind le prochain objectif
    if ((nextX == currentX) && (nextY == currentY)) { // On a atteind le prochain objectif
        indiceCurrentObj++;
        if (indiceCurrentObj == nbObj) { // On vérifie qu'on a pas attend la fin de la liste des objectifs
            attendFinMatch = true;
        } else {
            nextX = calculX(couleur,obj[indiceCurrentObj].nextX);
            nextY = obj[indiceCurrentObj].nextY;
        }
    }

    // On calcule le prochain déplacement
    if (!attendFinMatch) {
        switch (obj[indiceCurrentObj].typeMvt) {
        case CALLER:
            caller(nextX, nextY);
            break;
        case AVANCER:
        case RECULER:
        case BOUGER:
            bouger(nextX, nextY, obj[indiceCurrentObj].typeMvt);
            break;
        case ROTATION_HORAIRE:
        case ROTATION_ANTI_HORAIRE:
        default:
            break;
        }
    }
}
