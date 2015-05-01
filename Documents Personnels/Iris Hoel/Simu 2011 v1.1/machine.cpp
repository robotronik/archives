
# include "function.hpp"
# include "debug.hpp"
# include "affichage_dynamique.hpp"

int next_state = SORTIR_ZONE;
int nSaisie;
int continuer;



///////////////////////////////////////////// etats de la machine a etats avec transitions :


void sortirZone()
{
    recalageDepart();
    avancer(42.5);
    tourner(45);
    avancer(24.74873734152916);
    if(pieceAvant() == 1)
    {
        nSaisie = 1;
        next_state = SAISIR_1;
    }
    else
    {
        next_state = ALIGEMENT_LIGNE_1;
    }
}
void saisir1()
{
    fermerPinceAvant();
    tourner(180);
    next_state = RETOUR_ZONE_DEP;
}
void retourZoneDep()
{
    avancer(20);
    tourner(-45);
    avancer(35);
    libererPinceAvant();
    avancer(-35);
    tourner(-135);
    avancer(20);
    next_state = ALIGEMENT_LIGNE_1;
}
void alignementLigne1(){
    tourner(45);
    next_state = AVANCER_LIGNE_1;
}
void avancerLigne1()
{
    avancerNoB(140);
    int varPiece = pieceAvant();
    if(varPiece && nSaisie == 0)
    {
        nSaisie = 1;
        stopAvancer();
        next_state = SAISIR_2;
    }
    else if(varPiece && nSaisie)
    {
        nSaisie = 0;
        stopAvancer();
        next_state = SAISIR_3;
    }
    else if(doneAvancer())
    {
        next_state = GO_LIGNE_2;
        nSaisie = 0;
    }
	else if(erreurAvancer())
	{
	ret = erreurExecutionInstructionAvancerNoB;
	}
}
void saisir2()
{
    fermerPinceAvant();
    next_state = REMONTER_LIGNE_1;

}
void remonterLigne1()
{
    tourner(180);
    avancer(positionY()-35);
    tourner(-45);
	next_state = RETOUR_ZONE_DEP;
}
void saisir3()
{
    fermerPinceAvant();
    next_state = DEPOT_ZONE_SURE;
}
void depotZoneSure()
{
    avancer(175-positionY());
    tourner(72);
    avancer(15);
    libererPinceAvant();
    avancer(-15);
    tourner(-72);
    next_state = GO_LIGNE_2;
    nSaisie = 0;
}
void goLigne2()
{
    tourner(-90);
    avancer(35);
    if(pieceAvant() == 1)
    {
        nSaisie = 1;
        next_state = SAISIR_4;
    }
    else
    {
        next_state = ALIGNEMENT_LIGNE_2;
    }
}
void saisir4()
{
    fermerPinceAvant();
    next_state = DEPOT_BONUS;
}
void depotBonus()
{
    tourner(45);
    avancer(15);
    libererPinceAvant();
    avancer(-15);
    tourner(-45);
    recalageBonus();
    next_state = ALIGNEMENT_LIGNE_2;
}
void alignementLigne2(){
    tourner(-90);
    next_state = AVANCER_LIGNE_2;
}
void avancerLigne2()
{
    avancerNoB(140);
    int varPiece = pieceAvant();
    if(varPiece && nSaisie == 0)
    {
        nSaisie = 1;
        stopAvancer();
        next_state = SAISIR_5;
    }
    else if(varPiece && nSaisie)
    {
        nSaisie = 0;
        stopAvancer();
        next_state = SAISIR_6;
    }
    else if(doneAvancer())
    {
        next_state = FIN;
    }
}
void saisir5()
{
    fermerPinceAvant();
    next_state = REDESCENDRE_LIGNE_2;
 }
void redescendreLigne2()
{
    avancer(-(175 - positionY()));
    tourner(90);
    next_state = DEPOT_BONUS;
}
void saisir6()
{
    fermerPinceAvant();
    next_state = DEPOT_4;
}
void depot4()
{
    avancer(-(140 - positionY()));
    tourner(-90-45);
    avancer(15);
    libererPinceAvant();
    avancer(-15);
    tourner(-45);
    next_state = FINIR_MONTER;
}
void finirMonter()
{
    avancer(-(positionY()-35));
	next_state = FIN;
}
int fin()
{
tourner(-45);
avancer(-20);
tourner(-45);
avancer(-35-42);

if(pieceArriere())
{
    fermerPinceArriere();
    monterPiece();
}
functionFin();
return 0;
}

///////////////////////////////////////////// machine a ??tats



void machine()
{
//    log("restart");
    continuer = 1;
    nSaisie = 0;
    next_state = SORTIR_ZONE;
    int current_state = next_state;
    while(!bientotFinMatch && continuer)
    {
        if(ret != ok)
        {
            continuer = aieAie();
        }
		else
		{
        current_state = next_state;
        afficher_state(current_state);
//        log_state(current_state);
        switch( current_state)
        {
            case SORTIR_ZONE:
            sortirZone();
            break;
            case SAISIR_1:
            saisir1();
            break;
            case RETOUR_ZONE_DEP:
            retourZoneDep();
            break;
            case ALIGEMENT_LIGNE_1:
            alignementLigne1();
            break;
            case AVANCER_LIGNE_1:
            avancerLigne1();
            break;
            case SAISIR_2:
            saisir2();
            break;
            case REMONTER_LIGNE_1:
            remonterLigne1();
            break;
            case SAISIR_3:
            saisir3();
            break;
            case DEPOT_ZONE_SURE:
            depotZoneSure();
            break;
            case GO_LIGNE_2:
            goLigne2();
            break;
            case SAISIR_4:
            saisir4();
            break;
            case DEPOT_BONUS:
            depotBonus();
            break;
            case ALIGNEMENT_LIGNE_2:
            alignementLigne2();
            break;
            case AVANCER_LIGNE_2:
            avancerLigne2();
            break;
            case SAISIR_5:
            saisir5();
            break;
            case REDESCENDRE_LIGNE_2:
            redescendreLigne2();
            break;
            case SAISIR_6:
            saisir6();
            break;
            case DEPOT_4:
            depot4();
            break;
            case FINIR_MONTER:
            finirMonter();
            break;
            case FIN:
            continuer = fin();
            break;
            default:
            ret = bugEtat;
            break;
        }
		}
    }
    if(bientotFinMatch)
    {
        instructionFinMatch();
    }
}


