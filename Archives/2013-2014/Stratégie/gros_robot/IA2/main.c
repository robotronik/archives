/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <device.h>

#include "global.h"
#include "machine.h"
#include "init.h"
#include "servo.h"
#include "comAsser.h"
#include "comXBee.h"

extern Etat etatCourant;
extern int actionTerminee;
extern Action *tabAction;
extern int actionCourante;
extern Objectif tabObjectif[20];
extern int objectifCourant;
extern int presenceSharp;
extern int sharpActif;
extern volatile int tirette;
extern volatile int finMatch;
extern int g_strategie;
extern Couleur g_couleur;


void main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    etatCourant = INIT;
    presenceSharp = 0;
    sharpActif = 0;
    tirette = 0;
    finMatch = 0;
    g_strategie = 0;
    
    //while( Depart_Read() == 1 );
    init();
    CyDelay(2000);
    //descendreQueue();
    /*
    CyDelay(5000);
    lancerGauche(1);
    CyDelay(5000);
    lancerGauche(2);
    CyDelay(5000);
    lancerDroit(1);
    CyDelay(5000);
    lancerDroit(2);*/
    CyGlobalIntEnable; /* Uncomment this line to enable global interrupts. */
   /* getXY();
    waitRequestGetXYSafe();
    int i;
    
    for(i = 0; i<5; i++)
    {
        sendDelta(1000);
        lancerGauche(1);
        waitDoneAsser();
        lancerGauche(2);
        sendAlphaAbs(90);
        waitDoneAsser();
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(180);
        waitDoneAsser();
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(270);
        waitDoneAsser();
        sendDelta(1000);
        waitDoneAsser();
        sendAlphaAbs(0);
        waitDoneAsser();
    }*/
    for(;;)
    {
        sendXBee(66);
        CyDelay(1000);
        //machine();
    }
}

/* [] END OF FILE */
