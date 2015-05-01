#include "main.h"
#include "coordonees.h"
#include "mouvement.h"
#include "strategie.h"
#include "sharp.h"
#include "comAsser.h"
#include "init.h"
#include "debug.h"

#include "comAsser.h"
#include "init.h"
#include "debug.h"
#include "init.h"
#include "uart.h" // couche basse, autonome pour ne pas surcharger le reste
#include "timer.h"
#include "uart_xbee.h"

#include "bitsConfiguration.h"


volatile Pos robotNous;
volatile Pos robot1;
volatile int doneAsser     = 1;
volatile int stopSharp       = 0;
volatile int requestGetXY    = 0;
volatile int requestGetAlpha = 0;

volatile char ignoreSharp;
volatile char forceIgnoreSharp=1;//au début, premier cadeau

volatile char tirette = 0;

volatile unsigned char txBuffer[TX_BUFFER_SIZE];
volatile short indexTxBuffer;
volatile short txBufferLength;

volatile unsigned char tx2Buffer[TX_BUFFER_SIZE];
volatile short indexTx2Buffer;
volatile short tx2BufferLength;

Couleur couleur;


int main() {
    init();
   sendSetXY(ARRIERE,DEPART);
    sendSetAlpha(0);
    start_timer_match();

    // quoi qu'il arrive il faut partir
    sendDelta(SORTIR_ZONE_DEPART);
//    sendAlphaAbs(90);
    while (!assertAtteint()) {
        affiche_debug(DEBUG_MSG | 0b10101010);
        ; }
   return 0 ;
}

//   // return 0 ;
//
////    while(1)
////    {
////
////        if(getDistance(SHARP_1) >= 300 || getDistance(SHARP_2) >= 300)
////        {
////            _RB4 = 1 ;
////        }
////        else
////        {
////            _RB4 = 0 ;
////
////        }
////
////    }
////    return 0 ;
////}
//
//
//    /*Programme le mouvement de la porte*/
//   /*
//   initPWM();
//   lever_Porte();
//  // baisser_Porte();
//   lacher_Porte();
//   baisser_Porte();
//   lacher_Porte();
//   while(1){;}
//   //lacher_Porte();
//    //    // get couleur
//    //
//    /** Le parcourt du robot
//     * \note À définir dans le main.
//     */
//
//   Objectif objPetitRobot[] = {
//        {  BOUGER,           135,       DEPART},
//        { AVANCER,           135, NORD + AVANT}, // poser les fresques
//        {  BOUGER,          95.5,  SUD - ARBRE},
//        { AVANCER,          COIN,  SUD - ARBRE}, // rammasser les fruits
//        { AVANCER,         ARBRE,  SUD -  COIN},
//        { AVANCER,         ARBRE,       DEPART}, // rammasser les fruits
//        {  CALLER,       ARRIERE,       DEPART}, // optionnel, si manque de précision
//        {  BOUGER,    EST - 95.5,       DEPART},
//        { AVANCER,    EST - 95.5,          BAC}
//    };
//    int objPetitRobot_size = sizeof(objPetitRobot);
//    bool finMatch;
//
//    init();
//
//    couleur = ROUGE; // \todo : remplacer par le bon truck
//    finMatch = false;
//
//    // initialisation
//    //caller(ARRIERE, DEPART);
//    //wait_match_start();
//    sendSetXY(ARRIERE,DEPART);
//    sendSetAlpha(0);
//    start_timer_match();
//
//    // quoi qu'il arrive il faut partir
//    sendDelta(SORTIR_ZONE_DEPART);
////    sendAlphaAbs(90);
//    while (!assertAtteint()) {
//        affiche_debug(DEBUG_MSG | 0b10101010);
//        ; }
////    sendAlphaAbs(180);
////    while (!assertAtteint()) {
////        ; }
////    affiche_debug(DEBUG_MSG | 0b11111110);
////    affiche_debug(DEBUG_MSG | 0b11111110);
////    affiche_debug(DEBUG_MSG | 0b11111110);
////
////
////
////
////    /*Programme le mouvement de la porte*/
////   /*
////   initPWM();
////   lever_Porte();
////  // baisser_Porte();
////   lacher_Porte();
////   baisser_Porte();
////   lacher_Porte();
////   while(1){;}
////   //lacher_Porte();
////    //    // get couleur
////    //
////    /** Le parcourt du robot
////     * \note À définir dans le main.
////     */
////
////   Objectif objPetitRobot[] = {
////        {  BOUGER,           135,       DEPART},
////        { AVANCER,           135, NORD + AVANT}, // poser les fresques
////        {  BOUGER,          95.5,  SUD - ARBRE},
////        { AVANCER,          COIN,  SUD - ARBRE}, // rammasser les fruits
////        { AVANCER,         ARBRE,  SUD -  COIN},
////        { AVANCER,         ARBRE,       DEPART}, // rammasser les fruits
////        {  CALLER,       ARRIERE,       DEPART}, // optionnel, si manque de précision
////        {  BOUGER,    EST - 95.5,       DEPART},
////        { AVANCER,    EST - 95.5,          BAC}
////    };
////    int objPetitRobot_size = sizeof(objPetitRobot);
////    bool finMatch;
////
////    init();
////
////    couleur = ROUGE; // \todo : remplacer par le bon truck
////    finMatch = false;
////
////    // initialisation
////    //caller(ARRIERE, DEPART);
////    //wait_match_start();
////    sendSetXY(ARRIERE,DEPART);
////    sendSetAlpha(0);
////    start_timer_match();
////
////    // quoi qu'il arrive il faut partir
////    sendDelta(SORTIR_ZONE_DEPART);
//////    sendAlphaAbs(90);
////    while (!assertAtteint()) {
////        affiche_debug(DEBUG_MSG | 0b10101010);
////        ; }
//////    sendAlphaAbs(180);
//////    while (!assertAtteint()) {
//////        ; }
//////    affiche_debug(DEBUG_MSG | 0b11111110);
//////    affiche_debug(DEBUG_MSG | 0b11111110);
//////    affiche_debug(DEBUG_MSG | 0b11111110);
//////
//////    bouger(135, DEPART, AVANCER);
//////    affiche_debug(DEBUG_MSG | 0b11111010);
//////    bouger(135, NORD + AVANT, AVANCER);
//////    affiche_debug(DEBUG_MSG | 0b11100010);
////
////    while (1) {
////        affiche_debug(DEBUG_MSG | 0b11110000);
////    }
////    // le match
//////    while (!finMatch) {
//////
//////        affiche_debug(DEBUG_MSG | 0b11111110);
//////
//////        if(timeEnd()) { // on vérifie que les 90s sont écoulé
//////            finMatch = true;
//////        /*} else if (getDistance(infoMvt->sensMvt) > DISTANCE_PROXIMITE) {*/
//////            /*evitementObstacle(infoMvt);*/
//////        } else if (assertAtteint()) {
//////            chercheObjectif(objPetitRobot, objPetitRobot_size, couleur);
//////        } else { // On est en train de se déplacer vers le prochain objectif
//////            // on attend
//////        }
//////
//////    }
//////
//////    // fin du match, plus rien à faire
//////    sendStop();
//////
//////    while (1) {
//////        sendStop();
//////        // j'ai dit on fait plus rien
//////        // non mais !!
//////    }
////
////    return 0;
////}
