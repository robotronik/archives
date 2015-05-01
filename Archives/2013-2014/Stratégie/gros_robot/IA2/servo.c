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

#define PWM_DROIT_0 1100
#define PWM_DROIT_1 1500 //ok vérifié
#define PWM_DROIT_2 1700 //ok vérifié
#define PWM_GAUCHE_0 1800
#define PWM_GAUCHE_1 1500 //ok vérifié
#define PWM_GAUCHE_2 1300 //ok vérifié
#define QUEUE_HAUTE 1800
#define QUEUE_BASSE 1000

#include "global.h"
#include "servo.h"
#include <device.h>

int lanceurDroite = 0;
int lanceurGauche = 0;
extern int actionTerminee;

void initPosServo()
{
    PWM_ServoBrasDroit_WriteCompare(PWM_DROIT_0);
    CyDelay(500);
    PWM_ServoBrasDroit_WriteCompare(0);
    PWM_ServoBrasGauche_WriteCompare(PWM_GAUCHE_0);
    CyDelay(500);
    PWM_ServoBrasGauche_WriteCompare(0);
    PWM_ServoQueue_WriteCompare(QUEUE_HAUTE);
    CyDelay(500);
    PWM_ServoQueue_WriteCompare(0);
}

void lancerDroit( int lanceur )
{
    if(lanceur == 1)
    {
        lanceurDroite = 1;
        PWM_ServoBrasDroit_WriteCompare(PWM_DROIT_1);
        CyDelay(500);
        PWM_ServoBrasDroit_WriteCompare(0);
        actionTerminee = 1;
        
    }
    else if(lanceur == 2)
    {
        lanceurDroite = 2;
        PWM_ServoBrasDroit_WriteCompare(PWM_DROIT_2);
        CyDelay(500);
        PWM_ServoBrasDroit_WriteCompare(0);
        actionTerminee = 1;
    }
}

void lancerGauche( int lanceur )
{
    if(lanceur == 1)
    {
        lanceurGauche = 1;
        PWM_ServoBrasGauche_WriteCompare(PWM_GAUCHE_1);
        CyDelay(500);
        PWM_ServoBrasGauche_WriteCompare(0);
        actionTerminee = 1;
    }
    else if(lanceur == 2)
    {
        lanceurGauche = 2;
        PWM_ServoBrasGauche_WriteCompare(PWM_GAUCHE_2);
        CyDelay(500);
        PWM_ServoBrasGauche_WriteCompare(0);
        actionTerminee = 1;
    }
}

void monterQueue()
{
    PWM_ServoQueue_WriteCompare(QUEUE_HAUTE);
    CyDelay(500);
    actionTerminee = 1;
    PWM_ServoQueue_WriteCompare(0);
}

void descendreQueue()
{
    PWM_ServoQueue_WriteCompare(QUEUE_BASSE);
    CyDelay(500);
    actionTerminee = 1;
    PWM_ServoQueue_WriteCompare(0);
}

/* [] END OF FILE */
