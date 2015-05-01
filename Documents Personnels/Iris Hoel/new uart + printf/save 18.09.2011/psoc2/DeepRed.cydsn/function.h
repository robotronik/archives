

#if !defined(FUNCTION_H)
    #define FUNCTION_H




void setAlarmFinMatch();
int bientotFinMatch();

/////////////////////////////////////////////////////////////////////////////// tirer de la machine a etats :
///////////////////////////////////////////// cmd (à coder)
//  bloquantes ( nb : sortie des instructions si bientot fin de match détectée)
void fermerPinceAvant();
void libererPinceAvant();
void fermerPinceArriere();
void libererPinceArriere();
void avancer(float valeur, char is_hazard);
void tourner(float a);
void monterPiece();
void functionFin();
//  non bloquantes
void avancerNoB(float a);
void stopAvancer();
///////////////////////////////////////////// debug ou strategie a voir
int aieAie();
void instructionFinMatch();
void victoire();
void paume();
void recalageDepart();
void recalageBonus();
///////////////////////////////////////////// test de condition
int erreurAvancer();
int doneAvancer();
///////////////////////////////////////////// localisation
int positionY();

#endif

