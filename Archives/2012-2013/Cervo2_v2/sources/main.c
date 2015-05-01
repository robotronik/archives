/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/

#include "main.h"
#include "comAsser.h"
#include "init.h"
#include "servo.h"
#include "constantes.h"
#include "uart_xbee.h"
#include "machine.h"

//#define abs(valeur) valeur < 0 ? -valeur : valeur

#define PRECISION 5

volatile Pos robotNous;
volatile Pos robot1;
extern volatile int doneAsser;
extern volatile int stopSharp;
extern volatile int requestGetXY;

enum EtatMachine etatCourant;
volatile enum EtatMachine etatSuivant;
Pos objectif;
Pos cadeaux[4];
char cadeauxFait[4];
int cadeauSuivant;
Action tableau[4];
char nombreAction;
int actionSuivante;
char px;
char py;
char ignoreSharp;
volatile char forceIgnoreSharp=1;//au début, premier cadeau



Couleur couleur = BLEU;
volatile char tirette = 0;
volatile char fin = 0;

int main() {

   // int i=0;
    //couleur=BLEU;
    etatCourant = INIT;
    etatSuivant = X;
    tirette = 0;
    doneAsser = 1;
    init();

    
LATBbits.LATB5=0;


    


    //etat 0 : attente tirette

    while(1)
    {
        send_short(0x1122);
    }
    //pause_s(3);

    //baisserBras();
    //monterBras();
         if(couleur==BLEU) {
        cadeaux[0].x = 550;
        cadeaux[0].y = 27;
        cadeaux[0].alpha = 180;
        cadeaux[1].x = 1150;
        cadeaux[1].y = 27;
        cadeaux[1].alpha = 180;
        cadeaux[2].x = 1750;
        cadeaux[2].y = 27;
        cadeaux[2].alpha = 180;
        cadeaux[3].x = 2350;
        cadeaux[3].y = 27;
        cadeaux[3].alpha = 180;
    }
    else
    {
        cadeaux[3].x = 650;
        cadeaux[3].y = 27;
        cadeaux[3].alpha = 0;
        cadeaux[2].x = 1250;
        cadeaux[2].y = 27;
        cadeaux[2].alpha = 0;
        cadeaux[1].x = 1850;
        cadeaux[1].y = 27;
        cadeaux[1].alpha = 0;
        cadeaux[0].x = 2450;
        cadeaux[0].y = 27;
        cadeaux[0].alpha = 0;
    }

    cadeauxFait[0] = 0;
    cadeauxFait[1] = 0;
    cadeauxFait[2] = 0;
    cadeauxFait[3] = 0;

    cadeauSuivant = 0;

    objectif = cadeaux[0];

    stopSharp = 0;
    // while(1)
    pause_s(1);

    T4CONbits.TON = 1; //Début du timer

    if(couleur==BLEU) {
        sendSetXY(68,217); //position depart
        //  pause_ms(500);
        //}
        pause_ms(5);
        sendSetAlpha(180); //0
        pause_ms(5);
    }
    else
    {
        sendSetXY(2932, 217); //position depart
        //  pause_ms(500);
        //}
        pause_ms(5);
        sendSetAlpha(0); //0
        pause_ms(5);
    }
    getXY();
    while (requestGetXY);


    while (1) {
        while (fin)
        {
            sendStop();
            pause_ms(500);

        }
        machine();
        verifierSharp();
        /*if(i==10000)
        {
            baisserBras();
            i=0;
        }
        i++;*/

    }
}

void machine() {
    int dx, dy,cadeauEvite,YImpossible;
    YImpossible =0;
    if (doneAsser || etatSuivant == STOP_SHARP) {

        switch (etatSuivant) {
            case INIT : break;
            case X:
                //sendStop();
                if (etatCourant != etatSuivant)//initialisation de l'état si nouvel état
                {
                    dx = objectif.x - robotNous.x;
                    if (dx > 0) {
                        nombreAction = 2;
                        tableau[0].type = 0; //alpha
                        tableau[0].valeur = 180;
                        tableau[1].type = 1; //delta
                        tableau[1].valeur = -dx; //+
                    } else {
                        nombreAction = 2;
                        tableau[0].type = 0; //alpha
                        tableau[0].valeur = 0;
                        tableau[1].type = 1; //delta
                        tableau[1].valeur = dx; //-
                    }
                    actionSuivante = 0;
                    etatCourant = X;
                    etatSuivant = X;
                } else {
                    ignoreSharp = 0;
                    actionSuivante++;
                    if (actionSuivante >= nombreAction) {
                        verifierPosition();
                        if (py && px) {
                            etatCourant = X;
                            etatSuivant = CADEAU;
                            break;
                        } else {
                            etatCourant = X;
                            etatSuivant = Y;
                            break;
                        }
                    } else {
                        etatCourant = X;
                        etatSuivant = X;
                    }

                }
                faire(tableau[actionSuivante]);
                break;
            case Y:
                //sendStop();
                YImpossible=dansTable(robotNous.x,objectif.y);
                    if(YImpossible)
                    {
                        etatCourant=Y;
                        etatSuivant=X;
                        break;
                    }

                if (etatCourant != etatSuivant)//initialisation de l'état si nouvel état
                {

                    dy = objectif.y - robotNous.y;
                    if (dy > 0) {
                        nombreAction = 2;
                        tableau[0].type = 0; //alpha
                        tableau[0].valeur = -90;
                        tableau[1].type = 1; //delta
                        tableau[1].valeur = -dy;
                    } else {
                        nombreAction = 2;
                        tableau[0].type = 0; //alpha
                        tableau[0].valeur = 90;
                        tableau[1].type = 1; //delta
                        tableau[1].valeur = dy;
                    }
                    actionSuivante = 0;
                    etatCourant = Y;
                    etatSuivant = Y;
                } else {
                    ignoreSharp = 0;
                    actionSuivante++;
                    if (actionSuivante >= nombreAction) {
                        verifierPosition(); //A CODER
                        if (px && py) {
                            etatCourant = Y;
                            etatSuivant = CADEAU;
                            break;
                        } else if(py) {
                            etatCourant = Y;
                            etatSuivant = X;
                            break;
                            }
                        else
                        {
                            etatCourant=X;
                            etatSuivant=Y;
                        }
                    }
                    else {
                        etatCourant = Y;
                        etatSuivant = Y;
                    }

                }
                faire(tableau[actionSuivante]);
                break;
            case CADEAU:
                forceIgnoreSharp=0;
                verifierPosition();
                if(!px)
                {
                    etatCourant=CADEAU;
                    etatSuivant=X;
                }
                if(!py)
                {
                    etatCourant=CADEAU;
                    etatSuivant=Y;
                }
                pause_ms(50);
                sendAlphaAbs(objectif.alpha);
                waitDoneAsser();
                monterBras();
                //sendAlphaAbs(-90);
                //waitDoneAsser();
                baisserBras();
                lacherBras();//pwm à 0, pour limiter la consommation (ou pas)
                //sendDelta(-40);
                //waitDoneAsser();
                cadeauxFait[cadeauSuivant] = 1;
                while (cadeauxFait[cadeauSuivant]) {
                    cadeauSuivant = (cadeauSuivant + 1) % 4; //recherche d'un cadeau pas fait
                }
                objectif = cadeaux[cadeauSuivant];
                //cadeauxFait[cadeauSuivant-1]=0;
                etatCourant = CADEAU;
                etatSuivant = X;
                break;
            case STOP_SHARP:
                if (etatCourant != etatSuivant) {
                    doneAsser = 1;
                    sendStop();
                    waitDoneAsser();
                    pause_ms(500);
                }
                verifierPosition();
                if (!px && etatCourant == Y) {
                    etatCourant = STOP_SHARP;
                    etatSuivant = X;
                    ignoreSharp = 1;
                    break;
                }
                if (!py && (etatCourant == X || etatCourant == CADEAU)) {
                    etatCourant = STOP_SHARP;
                    etatSuivant = Y;
                    ignoreSharp = 1;
                    break;
                }
                if (!px) {
                    etatCourant = STOP_SHARP;
                    etatSuivant = X;
                    break;
                }
                if (!py) {
                    etatCourant = STOP_SHARP;
                    etatSuivant = Y;
                    break;
                }
                etatCourant = STOP_SHARP;
                etatSuivant = X;
                break;
            case EVITER:
                if (etatCourant != etatSuivant) {
                    ignoreSharp = 1;
                    cadeauEvite = cadeauSuivant;
                    do{
                        cadeauSuivant = (cadeauSuivant + 1) % 4; //recherche d'un cadeau pas fait
                    }while (cadeauxFait[cadeauSuivant]);
                    if (cadeauEvite == cadeauSuivant) {
                        verifierPosition();
                        if (!px && etatCourant == Y) {
                            etatCourant = STOP_SHARP;
                            etatSuivant = X;
                            ignoreSharp = 1;
                            break;
                        }
                        if (!py && (etatCourant == X || etatCourant == CADEAU)) {
                            etatCourant = STOP_SHARP;
                            etatSuivant = Y;
                            ignoreSharp = 1;
                            break;
                        }
                        if (!px) {
                            etatCourant = STOP_SHARP;
                            etatSuivant = X;
                            break;
                        }
                        if (!py) {
                            etatCourant = STOP_SHARP;
                            etatSuivant = Y;
                            break;
                        }
                        etatCourant = STOP_SHARP;
                        etatSuivant = X;
                        break;
                    }
                    objectif=cadeaux[cadeauSuivant];

                    tableau[0].type=0;
                    tableau[0].valeur=-90;
                    tableau[1].type=1;
                    tableau[1].valeur=-300;
                    actionSuivante=0;
                    etatCourant=EVITER;
                    etatSuivant=EVITER;
                    nombreAction=2;
                }
                else  if (actionSuivante >= nombreAction) {
                    etatCourant=EVITER;
                    etatSuivant=X;
                    break;
                }
                else
                {
                    faire(tableau[actionSuivante]);
                    actionSuivante++;
                    etatCourant=EVITER;
                    etatSuivant=EVITER;
                }

                break;
        }

    }
}

void verifierPosition() {
    getXY();
    while (requestGetXY);
    if (abs(robotNous.x - objectif.x) < PRECISION) px = 1;
    else px = 0;
    if (abs(robotNous.y - objectif.y) < 3*PRECISION) py = 1;
    else py = 0;
}

void verifierSharp() {
    if (stopSharp && (!(ignoreSharp||forceIgnoreSharp))) {
        etatSuivant = STOP_SHARP;
        LATBbits.LATB5 = 1;
    }
}

void faire(Action mouvement) {
    switch (mouvement.type) {
        case 0:
            ignoreSharp=1;
            pause_ms(50);
            sendAlphaAbs(mouvement.valeur);
            break;
        case 1:
            ignoreSharp=0;
            pause_ms(50);
            sendDelta(mouvement.valeur);
            break;
    }
}

int abs(int n)
{
    if(n<0) return(-n);
    else return(n);
}

int dansTable(int x, int y)
{
    if((x<500 || x>3500) && y<150) return 1;
    else return 0;
}