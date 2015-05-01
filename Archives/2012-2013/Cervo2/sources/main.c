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

//enum EtatMachine etatCourant;
volatile enum EtatMachine etatSuivant;
Pos objectif;
//Pos cadeaux[4];
//char cadeauxFait[4];
//int cadeauSuivant;
//Action tableau[4];
//char nombreAction;
int actionSuivante;
char px;
char py;
char ignoreSharp;
volatile char forceIgnoreSharp=1;//au début, premier cadeau



Couleur couleur = BLEU;
volatile char tirette = 0;
volatile char fin = 0;

int main() {
    init();
    pause_s(2);
    sendDelta(1000);
    while(1);
}