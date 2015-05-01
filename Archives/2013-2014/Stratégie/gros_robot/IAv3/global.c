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

Couleur g_couleur;
Etat etatCourant;
Action *tabAction;
int actionCourante;
Objectif tabObjectif[20];
int objectifCourant;
int presenceSharp;
int sharpActif;
volatile int tirette = 0;
volatile int finMatch;
volatile int temps;
int g_strategie;

volatile Position posRobot;
volatile Position posRobot2;
//Asser
volatile int requestGetAlpha;
volatile int requestGetXY;
volatile int actionTerminee;

/* [] END OF FILE */
