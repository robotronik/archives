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
#include "comXBee.h"

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
        //printXBee("STOP_SHARP", 10);
        etatCourant = STOP_SHARP;
    }
    if(finMatch)
    {
        //printXBee("FIN_MATCH", 9);
        etatCourant = FIN_MATCH;
    }
    
    switch(etatCourant)
    {
        case ATTENTE_TIRETTE :
            if(Depart_Read() == 0)
            {
                //printXBee("ATTENTE_PETIT", 13);
                etatCourant = ATTENTE_PETIT;
                sendTiretteRobot2();
                tirette = 1;
                //Timer_Fin_Start();
            }
            break;
        case ATTENTE_PETIT :
            if(! presenceSharp)
            {
                //printXBee("DEBUT_OBJECTIF", 14);
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
                //printXBee("FIN_MATCH", 9);
                etatCourant = FIN_MATCH;
            }
            else
            {
                //printXBee("DEBUT_ACTION", 12);
                etatCourant = DEBUT_ACTION;
            }
            break;
        case DEBUT_ACTION :
            if(tabAction[actionCourante].type==FIN)
            {
                //printXBee("FIN_OBJECTIF", 12);
                etatCourant = FIN_OBJECTIF;
            }
            else
            {
                //printXBee("EN_COURS", 8);
                forgerAction();
                actionTerminee = 0;
                realiserAction();
                etatCourant = EN_COURS;
            }
            break;
        case EN_COURS :
            if(actionTerminee)
            {
                //printXBee("FIN_ACTION", 10);
                etatCourant = FIN_ACTION;
                actionTerminee = 0;
            }
            break;
        case FIN_ACTION :
            //printXBee("DEBUT_ACTION", 12);
            actionCourante++;
            etatCourant = DEBUT_ACTION;
            break;
        case FIN_OBJECTIF :
            //printXBee("DEBUT_OBJECTIF", 14);
            objectifCourant++;
            etatCourant = DEBUT_OBJECTIF;
            break;
        case STOP_SHARP :
            sendStop();
            verifierSharp();
            if(!presenceSharp)
            {
                //printXBee("DEBUT_ACTION", 12);
                etatCourant = DEBUT_ACTION;
            }
            break;
        case FIN_MATCH :
            fin(); //fonction dont on ne sort jamais !
            break;
    }
}

/* [] END OF FILE */
