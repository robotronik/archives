#include "signaux.h"

Signaux signaux;
static Testeur testeur;

void initSignaux()
{
    signaux.besoinObj = 0;
    signaux.mouvRepos = 0;
    signaux.calculRetour = 0;
    signaux.objectifDispo = 0;
    signaux.blocked = 0;
    signaux.warnBlocked = 0;
	signaux.pathReceived = 0;
	signaux.posRecue = 0;
	signaux.timerOn = 0;
}

void beginTest()
{
    testeur.test = 1;
    testeur.asserOk = 0;
    testeur.cartoOk = 0;
    testeur.tourelleOk = 0;
}

char isTest()
{
    return testeur.test;
}

void endTest()
{
    if(testeur.asserOk)
        UART_XBee_PutChar(0x1A);
    else
        UART_XBee_PutChar(0x0A);

    if(testeur.cartoOk)
        UART_XBee_PutChar(0x1C);
    else
        UART_XBee_PutChar(0x0C);

    if(testeur.tourelleOk)
        UART_XBee_PutChar(0x1B);
    else
        UART_XBee_PutChar(0x0B);

    testeur.test = 0;
}

void validerAsser()
{
    testeur.asserOk = 1;
}

void validerCarto()
{
    testeur.cartoOk = 1;
}

void validerTourelle()
{
    testeur.tourelleOk = 1;
}
