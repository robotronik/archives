# include "main.hpp"


extern Inst instructionRobot1;
extern Inst instructionRobot2;
extern float RAPPORT_HAUTEUR;
extern Robot *R1, *R2;
int nbInstNecessaireLacherPiece;
int numInstMachine = 0;
int numQuesPieceAvant = -1;
int numQuesPieceArriere = -1;
extern int done1;
extern int continuer;
extern int orientation;

int varStopAvancer = 0;
int resDetectionPieceAvant[15] ={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int resDetectionPieceArriere[2] = {-1,-1};
int varDoneAvancer = 0;


int pieceAvant()
{
if(continuer){
    numQuesPieceAvant ++;
//    log("question piece");
//    log_arg(numQuesPiece);
    if(resDetectionPieceAvant[numQuesPieceAvant] == -1){
        if(R1->detection_avant_inter != R1->temp_detection_avant)
            resDetectionPieceAvant[numQuesPieceAvant] = 1;
        else
            resDetectionPieceAvant[numQuesPieceAvant] = 0;
    }
//    log("resultat");
//    log_arg(resDetectionPiece[numQuesPiece]);
return resDetectionPieceAvant[numQuesPieceAvant];
}
return 0;
}
int pieceArriere(){
if(continuer){
    numQuesPieceArriere ++;
//    log("question piece");
//    log_arg(numQuesPiece);
    if(resDetectionPieceArriere[numQuesPieceArriere] == -1){
        if(R1->detection_arriere_inter != R1->temp_detection_arriere)
            resDetectionPieceArriere[numQuesPieceArriere] = 1;
        else
            resDetectionPieceArriere[numQuesPieceArriere] = 0;
    }
//    log("resultat");
//    log_arg(resDetectionPiece[numQuesPiece]);
return resDetectionPieceArriere[numQuesPieceArriere];
}
return 0;
}

void functionFin(){
continuer = 0;
numInstMachine = 0;
numQuesPieceAvant = -1;
numQuesPieceArriere = -1;
}
/////////////////////////////////////////////////////////////////////////////// pour machine a etats :

///////////////////////////////////////////// cmd (?? coder)
//  bloquantes ( nb : sortie des instructions si bientot fin de match d??tect??e)
void fermerPinceAvant()
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1)
    {
        R1->temp_detection_avant = R1->detection_avant_inter;
        instructionRobot1.type = FERMER_AVANT;
        instructionRobot1.valeur = 0;
        instructionRobot1.signe = 0;
        instructionRobot1.vitesse = 0;
        par_defaut(&instructionRobot1);
        functionFin();
    }
    }
}
void libererPinceAvant()
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1)
    {
        instructionRobot1.type = LIBERER_AVANT;
        instructionRobot1.valeur = 0;
        instructionRobot1.signe = 0;
        instructionRobot1.vitesse = 0;
        par_defaut(&instructionRobot1);
        functionFin();
    }
    }
}
void fermerPinceArriere()
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1)
    {
        R1->temp_detection_arriere = R1->detection_arriere_inter;
        instructionRobot1.type = FERMER_ARRIERE;
        instructionRobot1.valeur = 0;
        instructionRobot1.signe = 0;
        instructionRobot1.vitesse = 0;
        par_defaut(&instructionRobot1);
        functionFin();
    }
    }
}
void libererPinceArriere()
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1)
    {
        instructionRobot1.type = LIBERER_ARRIERE;
        instructionRobot1.valeur = 0;
        instructionRobot1.signe = 0;
        instructionRobot1.vitesse = 0;
        par_defaut(&instructionRobot1);
        functionFin();
    }
    }
}
void avancer(float valeur)
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1)
    {
        instructionRobot1.type = AVANCER;
        if(valeur != 0){
            if(valeur<0){
                instructionRobot1.signe = -1;
                valeur = -valeur;
            }
             else
                instructionRobot1.signe = 1;
            instructionRobot1.valeur = valeur;
            instructionRobot1.vitesse = 0;
            par_defaut(&instructionRobot1);
            functionFin();
        }
        else
            done1 ++;
    }
    }
}

void tourner(float valeur )
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1)
    {
        instructionRobot1.type = TOURNER;
        if(valeur<0){
            instructionRobot1.signe = -1*orientation;
            valeur = -valeur;
        }
        else
            instructionRobot1.signe = 1*orientation;

        instructionRobot1.valeur = valeur;
        instructionRobot1.vitesse = 0;
        par_defaut(&instructionRobot1);
        functionFin();
    }
    }
}
void monterPiece()
{
	// verin, a faire
}
void sousAvancerNoB(float valeur){
instructionRobot1.type = AVANCER;
if(valeur<0){
    instructionRobot1.signe = -1;
    valeur = -valeur;
}
else
    instructionRobot1.signe = 1;
instructionRobot1.valeur = valeur;
instructionRobot1.vitesse = 0;
par_defaut(&instructionRobot1);
functionFin();
}
void avancerNoB(float valeur)
{
    if(continuer){
    numInstMachine ++;
    if(numInstMachine == done1 +1){
        sousAvancerNoB(35);
        varStopAvancer = 0;
        varDoneAvancer = 0;
    }
    else if((numInstMachine == done1 +2 |
            numInstMachine == done1 +3) && !varStopAvancer)
    {
        sousAvancerNoB(35);
    }
    else if(numInstMachine == done1 +4 && !varStopAvancer)
    {
        sousAvancerNoB(35);
        varDoneAvancer = 1;
    }
    }
}

void stopAvancer()
{
varStopAvancer = 1;
}
int aieAie()
{
    log("aie");
return 0;
}
void instructionFinMatch()
{

}
void victoire()
{

}
void paume()
{

}
void recalageDepart()
{

}
void recalageBonus()
{

}

///////////////////////////////////////////// test de condition
int erreurAvancer()
{
return 0;
}
int doneAvancer()
{
return varDoneAvancer;
}

///////////////////////////////////////////// localisation
int positionY()
{
return R1->position_robot.y/RAPPORT_HAUTEUR;
}

