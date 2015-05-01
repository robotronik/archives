#include "machine.h"

static StateMouv etatMouv = IDLE;
static StateMouv next_etatMouv = IDLE;

static StateMind etatMind = SPY1;
static StateMind next_etatMind = SPY1;

void initPssoc()
{
    initSignaux();
    initEspion();
    initRemplirObj();
    initAsser();
    startRefreshTourelle();

    initComCarto(); // a faire en derniere il faut que la carto soit initialise

 #ifdef TEST_CONNEXION
    // Test de toutes les connexions
    beginTest();
    // Un signal test redirige les IT
    testCarto();
    testAsser(); // TODO .. avec mouvement
    // autres trucs a tester
    #ifdef SIMU
    unsigned int i;
    for(int j=0; j<10; j++)
    {
        for(i=0; i<100000; i++)
            forcerAsser();
    }
    forcerCarto();
	/*
    for(i = 0; i<10; i++)
        forcerTourelle();
	*/
    #else
    CyDelay(5000);
    #endif

    endTest();
#endif

    /// ATTENDRE LA FIN DE TIRETTE

    startRefreshCarto();
}

void runMachine()
{
    #ifdef SIMU
    afficherAutomate();
    #endif
    #ifdef DEBUG_ETAT
	sendEtat();
	#endif

    switch(etatMouv)
    {
        case IDLE :  stateIdle(); break;
        case DEPL_GO : stateDeplGo(); break;
        case WAIT_END_DEPL_GO : stateWaitEndDeplGo(); break;
        case DEPL_RETURN : stateDeplReturn(); break;
        case WAIT_END_DEPL_RETURN : stateWaitEndDeplReturn(); break;
        case BLOCKED : stateBlocked(); break;
        default : break;
    }
    switch(etatMind)
    {
        case SPY1 : stateSpy1(); break;
        case SPY2 : stateSpy2(); break;
        case CLUSTER : stateCluster(); break;
        case CHOIX_OBJ : stateChoixObj(); break;
        case REMPLIR_OBJ : stateRemplirObj(); break;
        default: break;
    }

    etatMouv = next_etatMouv;
    etatMind = next_etatMind;

	#ifndef SIMU
	Control_Reg_1_Write(1);
	#endif
}

// Mouv
void stateIdle()
{
    if(signaux.blocked)
    {
        next_etatMouv = BLOCKED;
        return;
    }
    if(isEmptyFileDepl(fileDeplGo))
    {
        signaux.besoinObj = 1;
        next_etatMouv = IDLE;
    }
    else
    {
        next_etatMouv = DEPL_GO;
    }
}

void stateDeplGo()
{
    if(signaux.blocked)
    {
        next_etatMouv = BLOCKED;
        return;
    }
    if(isEmptyFileDepl(fileDeplGo) && isEmptyFileMacro())
    {
        next_etatMouv = DEPL_RETURN;
        return;
    }

    // On envoie a l asser
    sendDeplGo();

    next_etatMouv = WAIT_END_DEPL_GO;
}

void stateWaitEndDeplGo()
{
    if(signaux.blocked)
    {
        next_etatMouv = BLOCKED;
        return;
    }
    if(signaux.finMouvement)
        next_etatMouv = DEPL_GO;
}

void stateDeplReturn()
{
    if(signaux.blocked)
    {
        next_etatMouv = BLOCKED;
        return;
    }
    if(isEmptyFileDepl(fileDeplReturn) && isEmptyFileMacro()) // && getNbCmdInAsser() == 0)
    {
        next_etatMouv = IDLE;
        return;
    }

    // On envoie a l asser
    sendDeplReturn();

    next_etatMouv = WAIT_END_DEPL_RETURN;
}

void stateWaitEndDeplReturn()
{
    if(signaux.blocked)
    {
        next_etatMouv = BLOCKED;
        return;
    }
    if(signaux.finMouvement)
        next_etatMouv = DEPL_RETURN;
}

// VERY DANGEROUS
void stateBlocked()
{
    stopAsserByBrake();
}

// Mind
void stateSpy1()
{
    if(signaux.besoinObj && signaux.objectifDispo)
    {
        next_etatMind = REMPLIR_OBJ;
        return;
    }

	/// TODO : Pb avec l'UART
    espion(); // TODO
    if(signaux.blocked)
    {
        next_etatMind = CALCUL_REPOS;
    }
    else if(signaux.calculRetour)
    {
        next_etatMind = CALCUL_RETOUR;
    }
    else
    {
        next_etatMind = CLUSTER;
    }
}

void stateSpy2()
{
    espion(); // TODO
    next_etatMind = CALCUL_REPOS;
}

void stateCluster()
{
    if(signaux.besoinObj && signaux.objectifDispo)
    {
        next_etatMind = REMPLIR_OBJ;
        return;
    }
    if(signaux.blocked)
    {
        next_etatMind = CALCUL_REPOS;
        return;
    }
    // On clusterise
    clusterize(eObjectifs, NB_OBJECTIFS);

    next_etatMind = CHOIX_OBJ;
}

void stateChoixObj()
{
    if(signaux.blocked)
    {
        next_etatMind = CALCUL_REPOS;
        return;
    }
    choisirObjectif();

    if(signaux.besoinObj)
    {
        next_etatMind = REMPLIR_OBJ;
    }
    else
    {
        next_etatMind = SPY1;
    }
}

void stateRemplirObj()
{
    if(signaux.blocked)
    {
        next_etatMind = CALCUL_REPOS;
        return;
    }

    remplirCluster();
    // Plus de besoinObj car on l'a envoye
    signaux.besoinObj = 0;

    next_etatMind = SPY1;
}


#ifdef SIMU
void afficherAutomate()
{
    wchar_t txt[71] = {0};
    switch(etatMouv)
    {
        case IDLE :  swprintf(txt, L"Etat Mouv : IDLE"); break;
        case DEPL_GO :  swprintf(txt, L"Etat Mouv : DEPL_GO"); break;
        case DEPL_RETURN :  swprintf(txt, L"Etat Mouv : DEPL_RETURN"); break;
        case WAIT_END_DEPL_GO :  swprintf(txt, L"Etat Mouv : WAIT_END_DEPL_GO"); break;
        case WAIT_END_DEPL_RETURN :  swprintf(txt, L"Etat Mouv : WAIT_END_DEPL_RETURN"); break;
        case BLOCKED :  swprintf(txt, L"Etat Mouv : BLOCKED"); break;
        case DEPL_REPOS :  swprintf(txt, L"Etat Mouv : DEPL_REPOS"); break;
        case WAIT_END_DEPL_REPOS :  swprintf(txt, L"Etat Mouv : WAIT_END_DEPL_REPOS"); break;
        case RECALCUL_RETURN :  swprintf(txt, L"Etat Mouv : RECALCUL_RETURN"); break;
        case FIN_MATCH :  swprintf(txt, L"Etat Mouv : FIN_MATCH"); break;
    }
   setLog(11, txt);

    switch(etatMind)
    {
        case SPY1 : swprintf(txt, L"Etat Mind : SPY1"); break;
        case SPY2 : swprintf(txt, L"Etat Mind : SPY2"); break;
        case CLUSTER : swprintf(txt, L"Etat Mind : CLUSTER"); break;
        case CHOIX_OBJ : swprintf(txt, L"Etat Mind : CHOIX_OBJ"); break;
        case REMPLIR_OBJ : swprintf(txt, L"Etat Mind : REMPLIR_OBJ"); break;
        case CALCUL_REPOS : swprintf(txt, L"Etat Mind : CALCUL_REPOS"); break;
        case CALCUL_RETOUR : swprintf(txt, L"Etat Mind : CALCUL_RETOUR"); break;
    }
   setLog(12, txt);
}

#endif
void sendEtat()
{
    switch(etatMouv)
    {
        case IDLE :  UART_XBee_PutChar(0x00); break;
        case DEPL_GO :  UART_XBee_PutChar(0x01); break;
        case DEPL_RETURN :  UART_XBee_PutChar(0x02); break;
        case WAIT_END_DEPL_GO :  UART_XBee_PutChar(0x03); break;
        case WAIT_END_DEPL_RETURN :  UART_XBee_PutChar(0x04); break;
        case BLOCKED :  UART_XBee_PutChar(0x05); break;
        case DEPL_REPOS : UART_XBee_PutChar(0x06); break;
        case WAIT_END_DEPL_REPOS :  UART_XBee_PutChar(0x07); break;
        case RECALCUL_RETURN :  UART_XBee_PutChar(0x08); break;
        case FIN_MATCH :  UART_XBee_PutChar(0x09); break;
    }

    switch(etatMind)
    {
        case SPY1 : UART_XBee_PutChar(0x00); break;
        case SPY2 :UART_XBee_PutChar(0x01); break;
        case CLUSTER : UART_XBee_PutChar(0x02); break;
        case CHOIX_OBJ : UART_XBee_PutChar(0x03); break;
        case REMPLIR_OBJ : UART_XBee_PutChar(0x04); break;
        case CALCUL_REPOS : UART_XBee_PutChar(0x05); break;
        case CALCUL_RETOUR : UART_XBee_PutChar(0x06); break;
    }
}
