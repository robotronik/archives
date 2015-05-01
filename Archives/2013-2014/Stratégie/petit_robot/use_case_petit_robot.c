// C/C++ File
// AUTHOR:   robin_arch
// FILE:     use_case_petit_robot.c
// ROLE:     TODO (some explanation)
// CREATED:  2014-04-02 17:37:49
// MODIFIED: 2014-04-10 18:32:07


//////////////////////////////////////
// devrait être inclu ailleurs

#ifndef Bool
enum Bool {false, true};
#endif

#include <math.h> // pour les calculs d'angles
#ifndef PI
#define PI 3.1415
#endif

//////////////////////////////////////
// À définir ailleurs

Bool personneDerriere();
Bool personneDevant();
Angle getAlphaAbs();
void moveRelative(Distance_mm d);
void rotationRelative(Angle angle);
void pause_ms(int nb_ms);
void avancer(Coordonne_mm x, Coordonne_mm y);
void reculer(Coordonne_mm x, Coordonne_mm y);
void set_ready();
void wait_match_start();
Bool timeEnd();
Bool assertAtteint()
void sendStop();
Bool timeEnd();
void set_origin(postCallageX,postCallageY);  
Coordonne_mm getPosX();
Coordonne_mm getPosY();

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
// Fonction correspondant au bloc au dessus
// Et truc utile trouvé ailleurs


// main.h
typedef struct {
    int x;
    int y;
    int alpha;
} Pos;

// void set_ready();
// void wait_match_start();
void message_processing_xbee(unsigned char inMessage); // comXbee.h
        // case CODE_TIRETTE:
        //     tirette = 1;
        // break;

// Bool timeEnd();
void message_processing_xbee(unsigned char inMessage); // comXbee.h
        // case CODE_FIN :
        //        // fin = 1;
        // break;


// Bool personneDerriere();
// Bool personneDevant();
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void); // sharp.h
        //     ain0Buff[sampleCounter]=ADC1BUF0;
        // if(ain0Buff[sampleCounter]>1500)


// void pause_ms(int nb_ms);
void pause_ms(unsigned short n); // debug.h
void pause_s(unsigned short n); // debug.h

// Angle getAlphaAbs();
void getAlpha(void); // comAsser.h

// void sendStop();
void sendStop(void); // comAsser.h
void motors_stop(void); // Arrete les moteurs ++ motor.h


// void rotationRelative(Angle angle);
void sendAlpha( short alpha ); // comAsser.h

// void moveRelative(Distance_mm d);
// void avancer(Coordonne_mm x, Coordonne_mm y);
// void reculer(Coordonne_mm x, Coordonne_mm y);
void sendPresGros(void); // comAsser.h
void sendPresPrecis(void); // comAsser.h
void sendDelta( short delta ); // comAsser.h
void sendAlphaAbs( short alpha ); // comAsser.h
/* motors_set_speed : règle la vitesse d'un moteur, ie le rapport cyclique du PWM associé */
void motors_set_speed (short speedL, short speedR); // motor.h

// Bool assertAtteint()
volatile int doneAsser = 1;// comAsser.c
// void message_processing(unsigned char inMessage)
// {...
// case (DONE_ASSER):
//     //UART_XBee_PutString(" Fin Asser Vrai");
//     doneAsser = 1;
//     messageType = DONE_ASSER;
//     DEBUG_PRINTF("DoneAsser");
//     break;
// ...}
void waitDoneAsser(void) // comAsser.h
{
    while(!doneAsser);
}

// void set_origin(postCallageX,postCallageY);  
void sendSetAlpha(short alpha); // comAsser.h
void sendSetXY(int x, int y); // comAsser.h
/* motors_get_qei : renvoie la lecture actuelle d'un codeur incrémentiel */
void motors_get_qei(long *pPosL, long *pPosR);// motor.h
/* motors_reset_qei : réinitialise le codeur incrémental */
void motors_reset_qei();// motor.h


// Coordonne_mm getPosX();
// Coordonne_mm getPosY();
extern volatile Pos robotNous; // aloué dans main.c
volatile int requestGetXY = 0; // aloué dans comAsser.c
void getXY(void); // comAsser.h
void verifierPosition() {
    getXY();
    while (requestGetXY);
    if (abs(robotNous.x - objectif.x) < PRECISION) px = 1;
    else px = 0;
    if (abs(robotNous.y - objectif.y) < 3*PRECISION) py = 1;
    else py = 0;
}

//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////



//////////////////////////////////////
// Typedef et constante


typedef int Coordonne_mm;       /// < Coordonnées en mm
typedef int Distance_mm;        /// < Distance en mm
typedef float Angle;          /// < Angle en degré

// Distance_mm
#define DISTANCE_EVITEMENT
#define DISTANCE_CALLAGE
#define SORTIR_ZONE_DEPART

// Coordonne_mm
// uniquement en x
#define EST     3000
#define OUEST   0
#define COIN_ROUGE OUEST
#define COIN_JAUNE EST
// uniquement en y
#define NORD    0
#define SUD     2000
#define BAC     (295 + AVANT)
#define DEPART  600
// commun
#define COIN    370
#define ARBRE   275
// donnees du robot
#define COTE    88
#define AVANT   60
#define ARRIERE 95

/*! Les couleurs de départ
 */
enum Couleur {
    JAUNE,
    ROUGE
};

// /*! Type de mouvement pour le robot
//  */
// enum typeMvt {
//     AVANCER,            /// < côté plat en avant
//     RECULER,            /// < pointe en avant
//     bouger,             /// < sens pour avancer indiférent
//     CALLER             /// < pour caller le robot contre un mur et mettre à jour ses coordonées
// };

/*! Vitesse de déplacement
 */
enum TypeVitesse {
    RAPIDE,
    LENT
}

/*! Un objectif à atteindre
 */
typedef struct {
    Couleur coteRef;            /// < Couleur de référence
    // TypeMvt typeMvt;            /// < Type de mouvement (avancer, reculer, ou quelconque)
    // TypeVitesse TypeVitesse;    /// < Vittesse de déplacement
    void (*mvt) (Coordonne_mm x, Coordonne_mm y);            /// < Fonction de mouvement à appliquer (ex : avancer, reculer, bouger, caller, ...)
    Coordonne_mm nextX;         /// < Coordonnées du point à atteindre par rapport à la couleur de référence
    Coordonne_mm nextY;
} Objectif;

//////////////////////////////////////

/** Le parcourt du robot
 * \note À définir dans le main.
 */
Objectif objPetitRobot[] = {
    {          c,  bouger,     135,       DEPART},
    {          c, avancer,     135, NORD + AVANT}, // poser les fresques
    {          c,  bouger,    95,5,  SUD - ARBRE},
    {          c, avancer,    COIN,  SUD - ARBRE}, // rammasser les fruits
    {          c, avancer,   ARBRE,  SUD -  COIN},
    {          c, avancer,   ARBRE,       DEPART}, // rammasser les fruits
    {          c,  caller, ARRIERE,       DEPART}, // optionnel si manque de précision
    {c_oppose(c),  bouger,   95,5 ;       DEPART},
    {c_oppose(c), avancer,   95,5 ;          BAC}
};
    // {          c,  bouger,     135,       DEPART},
    // {          c, AVANCER,     135, NORD + AVANT}, // poser les fresques
    // {          c,  bouger,    95,5,  SUD - ARBRE},
    // {          c, AVANCER,    COIN,  SUD - ARBRE}, // rammasser les fruits
    // {          c, AVANCER,   ARBRE,  SUD -  COIN},
    // {          c, AVANCER,   ARBRE,       DEPART}, // rammasser les fruits
    // {          c,  CALLER, ARRIERE,       DEPART}, // optionnel si manque de précision
    // {c_oppose(c),  bouger,   95,5 ;       DEPART},
    // {c_oppose(c), AVANCER,   95,5 ;          BAC}


//////////////////////////////////////
// Utilitaire

/** Renvoi la position en absolu que l'ont souhaite atteindre à partir d'une position relative.
 *  \param c La couleur qui sert de côté de référence.
 *  \see cOppose
 */
Coordonnees calculX(Couleur c, Coordonnees relativeX);

/** \abstract Renvoie la couleur opposé à celle du départ.
 *  \note On pourrait tout à fait utiliser la syntaxe "!nextX" mais qui est moins lisible.
 *  \note La même chose sous forme de macro pour optimisation : #define C_OPPOSE(c) ((c) == JAUNE) ? ROUGE : JAUNE)
 */
Couleur cOppose (Couleur c);

//////////////////////////////////////

Coordonnees calculX(Couleur c, Coordonnees relativeX)
{
    if (c == JAUNE) {
        return EST - nextX;
    } else {
        return OUEST + nextX;
    }
}

Couleur cOppose (Couleur c)
{
    if (c == JAUNE) {
        return ROUGE;
    } else {
        return JAUNE;
    }
}

//////////////////////////////////////
// Stratégie

/*! Stratégie d'évitement d'obstacle.
 * \warning Cette fonction est actuellement blocante.
 */
void evitementObstacle();

/*! Cherche le prochain objectif et débute le déplacement vers lui
 * Le prochain objectif est soit l'objectif en cour si il y a eu une interruption de parcourt (tel qu'un obstactle),
 * soit l'objectif suivant.
 * \note Vérifie qu'il reste des objectifs.
 */
void chercheObjectif(Objectif obj[], int nbObj);

/*! Pirmit de réinitialiser les coordonnées du robot.
 * Avance le robot sur une courte distance jusqu'à ce qu'il rentre en butée puis enregistre les coordonnées.
 * de la nouvelle position dans l'asser.
 */
void caller(Coordonne_mm postCallageX, Coordonne_mm postCallageY);

/*! Toute la statégie du petit robot.
 */
void parcourt(Objectif objPetitRobot[], int nbObj);

///////////////////////////////////////

void evitementObstacle()
{
    if (personneDerriere()) { // On peut manœvrer
        // On recule, on se tourne de 90° et on avance
        Angle currentAlpha = getAlphaAbs();

        moveRelative(-DISTANCE_EVITEMENT);
        rotationRelative(PI/2);
        if (personneDevant()) {
            moveRelative(+DISTANCE_EVITEMENT);
        } else {
             // On est un peu coinsé !
        }
    } else {
        // On tourne sur place
        rotationRelative(PI/4);
        if (!personneDevant()) {
            pause_ms(1000);
        }
    }
}

void chercheObjectif(Objectif obj[], int nbObj)
{
    static int indiceCurrentObj = 0;
    static Bool attendFinMatch = false;

    int nextX, nextY;
    int currentX = getPosX();
    int currentY = getPosY();

    nextX = calculX(obj[indiceCurrentObj].coteRef,obj[indiceCurrentObj].nextX);
    nextY = obj[indiceCurrentObj].nextY;

    // On regarde si on a atteind le prochain objectif
    if ((nextX == currentX) && (nextY == currentY)) { // On a atteind le prochain objectif
        indiceCurrentObj++;
        if (indiceCurrentObj == nbObj) { // On vérifie qu'on a pas attend la fin de la liste des objectifs
            attendFinMatch = true;
        } else {
            nextX = calculX(obj[indiceCurrentObj].coteRef,obj[indiceCurrentObj].nextX);
            nextY = obj[indiceCurrentObj].nextY;
        }
    }

    // On calcule le prochain déplacement
    if (!attendFinMatch) {
        obj[indiceCurrentObj].mvt(nextX, nextY);
        // switch (obj[indiceCurrentObj].typeMvt) {
        // case AVANCER:
        //     avancer(nextX, nextY);
        //     break;
        // case RECULER:
        //     reculer(nextX, nextY);
        //     break;
        // case bouger:
        //     bouger(nextX, nextY);
        //     break;
        // case CALLER:
        //     caller(nextX, nextY);
        //     break;
        // }
    }
}

void caller(Coordonne_mm postCallageX, Coordonne_mm postCallageY)
{
    moveRelative(+DISTANCE_CALLAGE);
    while (!collision) { ; }    // on rentre en butée
    set_origin(postCallageX,postCallageY);
}

void bouger(Coordonne_mm x, Coordonne_mm y);
{
    Angle currentAlpha = getAlphaAbs();
    Angle nextAlpha = cos(nextX-currentX,nextY-currentY);

    // rotation
    if ((nextAlpha - currentAlpha) < (PI/2)) {
        avancer(nextX,nextY);
    } else {
        reculer(nextX,nextY);
    }
}

void parcourt(Objectif objPetitRobot[], int nbObj)
{
    Bool finMatch = false;

    // initialisation
    caller(ARRIERE, DEPART);
    set_ready();
    wait_match_start();

    // quoi qu'il arrive il faut partir
    avancer(SORTIR_ZONE_DEPART);

    // le match
    while (!finMatch) {

        if(timeEnd()) { // on vérifie que les 90s sont écoulé
            finMatch = true;
        } else if (!personneDevant()) {
            evitementObstacle();
        } else if (assertAtteint()) {
            chercheObjectif(objPetitRobot, nbObj);
        } else { // On est en train de se déplacer vers le prochain objectif
            // on attend
        }

    }
    
    // fin du match, plus rien à faire
    sendStop();
    
    while (1) {
        // j'ai dit on fait plus rien
        // non mais !!
    }
}