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

#include "bumper.h"
#include "constantes.h"
#include "BumperDroit.h"
#include "BumperGauche.h"

extern Couleur couleur;

volatile int bumperGauche;
volatile int bumperDroit;
volatile int bumperBuffetN = 1;
volatile int bumper2N = 1;


void checkBumper(void)
{
	if (couleur == BLEU)
	{
		if (bumperGauche)
			bumperBuffetN = 0;
		else
			bumperBuffetN = 1;
	}
	else
	{
		if (bumperDroit)
			bumperBuffetN = 0;
		else
			bumperBuffetN = 1;
	}
	
	if (bumperGauche && bumperDroit)
		bumper2N = 0;
	else
		bumper2N = 1;
}

void bumperGaucheHandler(void)
{
	bumperGauche = !BumperGauche_Read();
	PRINTF("Bumper Gauche %d\r", bumperGauche);
	checkBumper();
}

void bumperDroitHandler(void)
{
	bumperDroit = !BumperDroit_Read();
	PRINTF("Bumper Droit %d\r", bumperDroit);
	checkBumper();

}

void bumperGaucheFinHandler(void)
{
	bumperGauche = !BumperGauche_Read();
	PRINTF("Bumper Gauche Fin %d\r", bumperGauche);
	checkBumper();
}

void bumperDroitFinHandler(void)
{
	bumperDroit = !BumperDroit_Read();
	PRINTF("Bumper Droit Fin %d\r", bumperDroit);
	checkBumper();

}

/* [] END OF FILE */
