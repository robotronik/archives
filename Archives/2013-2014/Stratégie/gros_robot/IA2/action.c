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
#include "action.h"
#include <math.h>

#define DEG_TO_RAD (3.14159265359/180.0)
#define RAD_TO_DEG (180.0/3.14159265359)
#define TOLERANCE 1

extern int actionTerminee;
extern Action *tabAction;
extern int actionCourante;
extern Objectif tabObjectif[20];
extern int objectifCourant;
extern volatile Position posRobot;

extern volatile int requestGetAlpha;
extern volatile int requestGetXY;


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
            if( - TOLERANCE < dAlpha && TOLERANCE > dAlpha )
            {
                tabAction[actionCourante].parametre = sqrt( dX*dX + dY*dY );
            }
            else if( ( - TOLERANCE < 180 - dAlpha && TOLERANCE > 180 - dAlpha ) || ( - TOLERANCE < - 180 - dAlpha && TOLERANCE > - 180 - dAlpha ) )
            {
                tabAction[actionCourante].parametre = - sqrt( dX*dX + dY*dY );
            }
            else
            {
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
			lancerDroit( tabAction[actionCourante].parametre );
            break;
        case SERVO_LANCEUR_GAUCHE :
            lancerGauche( tabAction[actionCourante].parametre );
            break;
        case QUEUE :
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
            sendDelta( tabAction[actionCourante].parametre );
            break;
        case MOUVEMENT_ALPHA :
            sendAlpha( tabAction[actionCourante].parametre );
            break;
        case STOP :
            sendStop();
            break;
        case FIN :
        /* IMPOSSIBLE */
            break;
    }
}

void miseAJourPos()
{
	requestGetXY = 0;
	requestGetAlpha = 0;
    getXY();
    while(requestGetXY);
    getAlpha();
    while(requestGetAlpha);
}    
/* [] END OF FILE */
