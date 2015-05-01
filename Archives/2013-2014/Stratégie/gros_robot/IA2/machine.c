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
#include "global.h"
#include "machine.h"
#include "comAsser.h"
#include "sharp.h"
#include "init.h"
#include <device.h>
#include "action.h"
#include "fin.h"

extern Etat etatCourant;
extern int actionTerminee;
extern Action *tabAction;
extern int actionCourante;
extern Objectif tabObjectif[20];
extern int objectifCourant;
extern volatile int presenceSharp;
extern int sharpActif;
extern volatile int tirette;
extern volatile int finMatch;


void machine()
{
    verifierSharp();
    if(presenceSharp && sharpActif)
    {
        etatCourant = STOP_SHARP;
    }
    if(finMatch)
    {
        etatCourant = FIN_MATCH;
    }
    
    switch(etatCourant)
    {
        case INIT :
            init();
            etatCourant = ATTENTE_TIRETTE;
            break;
        case ATTENTE_TIRETTE :
            if(Depart_Read() == 1)
            {
                etatCourant = ATTENTE_PETIT;
                Timer_Fin_Start();
            }
            break;
        case ATTENTE_PETIT :
            if(! presenceSharp)
            {
                CyDelay(1000); // Attente d'etre sur que le petit est parti (1s)
                sharpActif = 1;
                etatCourant = DEBUT_OBJECTIF;
            }
            break;
        case DEBUT_OBJECTIF :
            tabAction = tabObjectif[objectifCourant].actions;
            actionCourante = 0;
            if(tabAction[actionCourante].type == FIN)
            {
                etatCourant = FIN_MATCH;
            }
            else
            {
                etatCourant = DEBUT_ACTION;
            }
            break;
        case DEBUT_ACTION :
            if(tabAction[actionCourante].type==FIN)
            {
                etatCourant = FIN_OBJECTIF;
            }
            else
            {
                forgerAction();
                actionTerminee = 0;
                realiserAction();
                etatCourant = EN_COURS;
            }
            break;
        case EN_COURS :
            if(actionTerminee)
            {
                etatCourant = FIN_ACTION;
            }
            break;
        case FIN_ACTION : 
            actionCourante++;
            etatCourant = DEBUT_ACTION;
            break;
        case FIN_OBJECTIF :
            objectifCourant++;
            etatCourant = DEBUT_OBJECTIF;
            break;
        case STOP_SHARP :
            sendStop();
            verifierSharp();
            if(!presenceSharp)
            {
                etatCourant = DEBUT_ACTION;
            }
            break;
        case FIN_MATCH :
            fin(); //fonction dont on ne sort jamais !
            break;
    }
}

/* [] END OF FILE */
