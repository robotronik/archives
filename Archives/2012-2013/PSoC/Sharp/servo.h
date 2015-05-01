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
#ifndef SERVO_H
#define SERVO_H
//Position du sharp par rapport au 0 de l'asser 1=>Avant 2=>Arrière
#define SEUIL 8533

#define ANGLESERVO 90

#include "debug.h"

static unsigned int stop = 0;


struct Calibre
{
	int volt;
	int milli;
};
typedef struct Calibre Calibre;

void moveServo(void);
int conversion(int entree);
void initTableau(Calibre *calibre);
void afficheLED(unsigned int result);
int detectionSharp(int Numero_Sharp); //fonction principale de détection sharp => sharp 1 = avant, sharp 2 =arrière


#endif

//[] END OF FILE
