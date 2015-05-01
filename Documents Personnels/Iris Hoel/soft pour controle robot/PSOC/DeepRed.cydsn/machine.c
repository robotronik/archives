
# include "function.h"
# include "machine.h"
# include "capteur.h"
# include "device.h"

int next_state = SORTIR_ZONE;
int nSaisie = 0;
int continuer = 1;
int nbCaseAvanceeLigne1 = 0;
int nbCaseAvanceeLigne2 = 0;
extern int ret;

///////////////////////////////////////////// etats de la machine a etats avec transitions :


void sortirZone()
{
    recalageDepart();
    avancer(8+42.5, 1);
    tourner(45);
	libererPinceAvant();
    avancer(24.74873734152916, 1);
    uint8 varPiece = pieceAvant();
	if(varPiece == 1)
    {
        nSaisie = 0;
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
    next_state = DEPOT_DIRECT;
}
void depotDirect()
{
    tourner(90);
    avancer(22, 0);
    libererPinceAvant();
    avancer(-22, 0);
    tourner(-45);
    next_state = AVANCER_LIGNE_1;
}
void alignementLigne1(){
    tourner(45);
    next_state = AVANCER_LIGNE_1;

}
void avancerLigne1()
{
    nbCaseAvanceeLigne1 ++;
    avancer(35, 1);
    uint8 varPiece = 1;//pieceAvant();
    if(varPiece && nSaisie == 0)
    {
		if(nbCaseAvanceeLigne1 == 3)
        nSaisie = 1;
		else
		nSaisie = 0;
        next_state = SAISIR_2;
		//UART_XBee_PutString("SAISIR_2");
    }
    else if(varPiece)
    {
        nSaisie = 0;
        next_state = SAISIR_3;
		//UART_XBee_PutString("SAISIR_3");
    }
    else if(nbCaseAvanceeLigne1 == 4){
        next_state = GO_LIGNE_2;
		//UART_XBee_PutString("GO_LIGNE_2");
        nSaisie = 0;
    }
	/*else if(erreurAvancer())
	{
	ret = erreurExecutionInstructionAvancerNoB;
	}*/
}
void saisir2()
{
    fermerPinceAvant();
    if(nbCaseAvanceeLigne1 ==  1 || nbCaseAvanceeLigne1 == 3){
    tourner(90+45);
    avancer(15, 0);
    libererPinceAvant();
    avancer(-15, 0);
    tourner(-90-45);
    }
    else if(nbCaseAvanceeLigne1 == 2){
    tourner(60);
    avancer(20, 0);
    libererPinceAvant();
    avancer(-20, 0);
    tourner(-60);
    }
    next_state = AVANCER_LIGNE_1;
}

void saisir3()
{
    fermerPinceAvant();
    next_state = DEPOT_ZONE_SURE;
}
void depotZoneSure()
{
	if (140-35*nbCaseAvanceeLigne1 > 100) {
    	avancer(35, 1);
    	avancer(105-35*nbCaseAvanceeLigne1, 1);
	} else avancer(140-35*nbCaseAvanceeLigne1, 1);
    tourner(72);
    avancer(15, 0);
    libererPinceAvant();
    avancer(-15, 0);
    tourner(-72);
    next_state = GO_LIGNE_2;
    nSaisie = 0;
}
void goLigne2()
{
    tourner(-90);
    avancer(35, 1);
    if(pieceAvant())
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
    tourner(45);
    next_state = DEPOT_BONUS;
}
void depotBonus()
{
    avancer(15, 1);
    libererPinceAvant();
    avancer(-15, 0);
    tourner(-45);
    recalageBonus();
    nbCaseAvanceeLigne2 = 0;
    next_state = ALIGNEMENT_LIGNE_2;
}
void alignementLigne2(){
    tourner(-90);
    next_state = AVANCER_LIGNE_2;
}
void avancerLigne2()
{
    nbCaseAvanceeLigne2 ++;
    avancer(35, 1);
    int varPiece = pieceAvant();
	if(varPiece && (nSaisie == 0))
    {
    	if(nbCaseAvanceeLigne1 == 3)
        nSaisie = 1;
		else
		nSaisie = 0;
        next_state = SAISIR_5;
    }
    else if(varPiece && nSaisie)
    {
        nSaisie = 0;
        next_state = SAISIR_6;
    }
	
    else if(nbCaseAvanceeLigne1 == 4){
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
    /*tourner(180);
	if (140-35*nbCaseAvanceeLigne2 > 100) {
    	avancer(35, 1);
    	avancer(105-35*nbCaseAvanceeLigne2, 1);
	} else avancer(140-35*nbCaseAvanceeLigne2, 1);
    tourner(-45);
    next_state = DEPOT_BONUS;*/
    if(nbCaseAvanceeLigne2 == 2){
    tourner(90+45);
    avancer(15, 1);
    libererPinceAvant();
    avancer(-15, 1);
    tourner(-90-45);
    }
    else if(nbCaseAvanceeLigne2 ==  1 || nbCaseAvanceeLigne2 == 3){
    tourner(60);
    avancer(20, 1);
    libererPinceAvant();
    avancer(-20, 1);
    tourner(-60);
    }
	next_state = AVANCER_LIGNE_2;
}
void saisir6()
{
    /*fermerPinceAvant();
    tourner(180);
    next_state = DEPOT_4;*/
	fermerPinceAvant();
	tourner(45);
	avancer(15,1);
	libererPinceAvant();
	avancer(-5,1);	
	next_state = FIN;
}
void depot4()
{
	if (175-35*nbCaseAvanceeLigne2 > 100) {
    	avancer(35, 1);
		if (140-35*nbCaseAvanceeLigne2 > 100) {
			avancer(35, 1);
			avancer(105-35*nbCaseAvanceeLigne2, 1);
		} else avancer(140-35*nbCaseAvanceeLigne2, 1);
	} else avancer(175-35*nbCaseAvanceeLigne2, 1);
    tourner(45);
    avancer(15, 1);
    libererPinceAvant();
    avancer(-15, 0);
    tourner(-45);
    next_state = FIN;
}

int fin()
{
//libererPinceArriere();
libererPinceAvant();
avancer(-15, 0);
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
    while(!bientotFinMatch() && continuer)
    {
        if(ret != ok)
        {
            continuer = aieAie();
        }
		else
		{
        current_state = next_state;
//        afficher_state(current_state);
//        log_state(current_state);
        switch( current_state)
        {
            case SORTIR_ZONE:
            sortirZone();
            break;
            case SAISIR_1:
            saisir1();
            break;
            case DEPOT_DIRECT:
            depotDirect();
            break;
            case RETOUR_ZONE_DEP:

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
    if(bientotFinMatch())
    {
        instructionFinMatch();
    }
}


