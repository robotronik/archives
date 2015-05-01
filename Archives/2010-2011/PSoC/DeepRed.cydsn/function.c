

// prevoir pour retenir la position
// prevoir l'évitement dans les fonctions avancer
// 


#include <device.h>
#include "UART.h"
#define TMPPIECE 1000 // 1s
#include "function.h"
//#include "divers.h"

int ret = ok;
robot r;
extern int8 couleur_depart;

int bientotFinMatch(){
	return 0;
}

void functionFin(){
	
}


/////////////////////////////////////////////////////////////////////////////// pour machine a etats :

///////////////////////////////////////////// cmd (?? coder)
//  bloquantes ( nb : sortie des instructions si bientot fin de match d??tect??e)
void fermerPinceAvant()
{
	PWM_ServoAv_WriteCompare1(52800);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	PWM_ServoAv_WriteCompare2(19200);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	CyDelay(TMPPIECE);
}
void libererPinceAvant()
	{
	PWM_ServoAv_WriteCompare1(60000);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	PWM_ServoAv_WriteCompare2(12000);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	CyDelay(TMPPIECE);
}
void fermerPinceArriere()
{
	PWM_ServoAr_WriteCompare1(19200);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	PWM_ServoAr_WriteCompare2(52800);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	CyDelay(TMPPIECE);
}
void libererPinceArriere()
{
	PWM_ServoAr_WriteCompare1(12000);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	PWM_ServoAr_WriteCompare2(60000);  // valeur a d??termin??e, suposition : elle d??termine l'angle de rotation
	CyDelay(TMPPIECE);
}
void avancer(float valeur, char is_hazard)
{
	Trame trame = conversion_delta(valeur);
	if (ret != ok) {
	    UART_XBee_PutString("Pas commande delta\n");
	    return;
	}
	if (is_hazard) {
		isr_Robot_Adverse1_Start();
		isr_Robot_Adverse2_Start();
	}
	UART_XBee_PutString("Commande delta\n");
	emission(trame);
	//startMouvement();  // inutile ?
	while(!done())
	{
		/*if(erreur())
		{
			ret = erreurExecutionInstructionAvancer;
			return;
		}*/
		if(bientotFinMatch())
		{
			stopAvancer();
			return;
		}
	}
	isr_Robot_Adverse1_Stop();
	isr_Robot_Adverse2_Stop();
}
void tourner(float valeur)
{
	Trame trame = conversion_alpha(valeur*couleur_depart);
	if (ret != ok) {
		UART_XBee_PutString("Pas commande alpha\n");
		return;
	}
	UART_XBee_PutString("Commande alpha\n");
	emission(trame);
	//startMouvement();  // inutile ?
	while(!done())
	{
		/*if(erreur())
		{
			ret = erreurExecutionInstructionAvancer;
			return;
		}*/
	}
}
void monterPiece()
{
	// verin, a faire
}
//  non bloquantes
int enCour = 0;
void stopAvancer()
{
	enCour = 0;
	stopMouvement(); // vérifier l'ordre : stopMouvement réinitiale delta ?	
}
///////////////////////////////////////////// debug ou strategie a voir
int aieAie()
{
	// s'il y a eu un probl??me avec l'assert ou la com c'est ici (ret != -2)
	// corriger le prob si possible et return 1, return 0 sinon
	return 0; // arr??te le robot
}
void instructionFinMatch()
{
	// plus le temps : retour a la zone depart en speed ou shutdown si 1min30
}
void victoire()
{
	// rien a faire, juste la danse de la victoire ...
}
void paume()
{
	// drole de configuration impossible
}
void recalageDepart()
{

}
void recalageBonus()
{

}
///////////////////////////////////////////// test de condition
int erreurAvancer()
{
	return erreur();
}
int doneAvancer()
{
	return done();
}
///////////////////////////////////////////// localisation
int positionY()
{
	return r.positionY;

}


/*
Trame trame;
trame.part_1 = GET_POSITION >> 8;
trame.part_2 = 0;
UART_XBee_PutString("GET_POSITION\n");
emission(trame);
return reponsePosition().y;
*/



/* [] END OF FILE */
