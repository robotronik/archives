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
int actionTerminee;
Action *tabAction;
int actionCourante;
Objectif tabObjectif[20];
int objectifCourant;
int presenceSharp;
int sharpActif;
volatile int tirette;
volatile int finMatch;
int g_strategie;
volatile Position posRobot;

volatile int requestGetAlpha;
volatile int requestGetXY;
volatile int doneAsser;

/* [] END OF FILE */
