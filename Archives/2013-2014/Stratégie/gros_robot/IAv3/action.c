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
#include "servo.h"
#include "comAsser.h"
#include "comXBee.h"
#include "action.h"
#include "comXBee.h"
#include <math.h>

#define DEG_TO_RAD (3.14159265359/180.0)
#define RAD_TO_DEG (180.0/3.14159265359)
#define TOLERANCE 20

extern int actionTerminee;
extern Action *tabAction;
extern int actionCourante;
extern Objectif tabObjectif[20];
extern int objectifCourant;
extern volatile Position posRobot;

extern volatile int requestGetAlpha;
extern volatile int requestGetXY;

int abs(int c){
    if(c<0){
        return(-c);
    }
    return(c);
}

void forgerAction()
{
    int alpha, dAlpha;
    int dX, dY;
    miseAJourPos();
    switch(tabAction[actionCourante].type)
    {
        case MOUVEMENT_DELTA :
            dX = tabObjectif[objectifCourant].pos.x - posRobot.x;
            dY = tabObjectif[objectifCourant].pos.y - posRobot.y;
            alpha = RAD_TO_DEG*atan2( dY, dX );
            dAlpha = alpha - posRobot.alpha;
            if(abs(dX)< 100 && abs(dY) <100)
            {
                dAlpha = 0;
            }            
            /*printXBee("Alpha : ",2); 
            printIntXBee(alpha);*/
            if( - TOLERANCE < dAlpha && TOLERANCE > dAlpha )
            {
                tabAction[actionCourante].parametre = sqrt( dX*dX + dY*dY );
            }
            else if( ( - TOLERANCE < (180 - dAlpha) && TOLERANCE > (180 - dAlpha) ) || ( - TOLERANCE < (- 180 - dAlpha) && TOLERANCE > (- 180 - dAlpha) ) )
            {
                tabAction[actionCourante].parametre = - sqrt( dX*dX + dY*dY );
            }
            else
            {
                //printXBee("INTOLERANCE",11);
                //printXBee("DX : ",2);
                //printIntXBee(dX);
                //printXBee("DY : ",2);
                //printIntXBee(dY);
                //printXBee("DA : ",2);
                //printIntXBee(dAlpha);
                actionCourante--;
				tabAction[actionCourante].parametre = tabObjectif[objectifCourant].pos.alpha;
            }
            break;
        case MOUVEMENT_ALPHA :
            tabAction[actionCourante].parametre = tabObjectif[objectifCourant].pos.alpha;
            break;
        default :
            break;
    }
}

void realiserAction()
{
    switch(tabAction[actionCourante].type)
    {
        case SERVO_LANCEUR_DROIT :
            //printXBee("LANCEUR DROIT",13);
			lancerDroit( tabAction[actionCourante].parametre );
            break;
        case SERVO_LANCEUR_GAUCHE :
            //printXBee("LANCEUR GAUCHE",14);
            lancerGauche( tabAction[actionCourante].parametre );
            break;
        case QUEUE :
            //printXBee("QUEUE",5);
            if( tabAction[actionCourante].parametre )
            {
                descendreQueue();
            }
            else
            {
                monterQueue();
            }
            break;
        case MOUVEMENT_DELTA :
            //printXBee("DELTA",5);
            sendDelta( tabAction[actionCourante].parametre );
            break;
        case MOUVEMENT_ALPHA :
            //printXBee("ALPHA",5);
            sendAlphaAbs( tabAction[actionCourante].parametre );
            break;
        case STOP :
            sendStop();
            break;
        default :
        /* IMPOSSIBLE */
            break;
    }
}

void miseAJourPos()
{
    getXY();
    waitRequestGetXYSafe();
    getAlpha();
    waitRequestGetAlphaSafe();
}    
/* [] END OF FILE */
