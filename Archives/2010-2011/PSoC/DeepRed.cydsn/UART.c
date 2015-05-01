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


#include <device.h>
#include "UART.h"

extern int ret;


extern uint8 nbMessageRxAsser;
extern uint16 fifoRxAsser[];
extern uint16 pFifoRxAsser;
extern int8 is_done;

uint16 readAsserMessage() {
	return fifoRxAsser[(pFifoRxAsser+nbMessageRxAsser)%16];
}

uint16 getAsserMessage() {
	nbMessageRxAsser--;
	return fifoRxAsser[(pFifoRxAsser+nbMessageRxAsser+1)%16];
}

void clearAsserMessage(uint8 nb) {
	if (nb > nbMessageRxAsser) nbMessageRxAsser = 0;
	else nbMessageRxAsser -= nb;
}

Trame conversion_alpha(float valeurF) // emission : 4bit pour le code instruction, 12 bit pour la valeur
{
int valeur = (int)(valeurF*5); // codage des trames en 0.2 Deg
Trame trame;
trame.part_1 = SET_ALPHA>>8;
trame.part_2 = 0;
if(valeur<0)
{
	trame.part_1 |= (1<<3);
	valeur = - valeur;
}
if(valeur>2047)
{
	ret = bugConversionValeurTropGrande;
	return trame;
}
trame.part_1 |= valeur >> 8;
trame.part_2 = (uint8)valeur;
return trame;
}

Trame conversion_delta(float valeurF) // emission : 4bit pour le code instruction, 12 bit pour la valeur
{
int valeur = (int)(valeurF*20); // codage des trames en 10-1 mm
Trame trame;
trame.part_1 = SET_DELTA>>8;
trame.part_2 = 0;
if(valeur<0)
{
	trame.part_1 |= (1<<3);
	valeur = - valeur;
}
while(valeur>2047)
{
	UART_XBee_PutString("depassement\n");
    avancer(100);
	valeur -= 2000;
}
trame.part_1 |= valeur >> 8;
trame.part_2 = (uint8)valeur;
return trame;
}

void emission(Trame trame)  // return erreur ?? pr??voir ?
{
UART_Asser_PutChar(trame.part_1);
UART_Asser_PutChar(trame.part_2);
// erreur emission a voir
}

void startMouvement()
{
Trame trame;
trame.part_1 = START >> 8;
trame.part_2 = 0;
UART_XBee_PutString("START\n");
emission(trame);
}

void stopMouvement()
{
Trame trame;
trame.part_1 = STOP >> 8;
trame.part_2 = 0;
UART_XBee_PutString("STOP\n");
emission(trame);
}

void pauseMouvement()
{
Trame trame;
trame.part_1 = PAUSE_ASSER >> 8;
trame.part_2 = 0;
UART_XBee_PutString("PAUSE_ASSER\n");
emission(trame);
}

void saveMouvement()
{
Trame trame;
trame.part_1 = SAVE_ASSER >> 8;
trame.part_2 = 0;
UART_XBee_PutString("SAVE_ASSER\n");
emission(trame);
}

int done()
{
	if (!is_done) return 0;
	is_done = 0;
	return 1; // return 1 si l'instruction non bloquante en cours est finie
}

int erreur()
{
	if (!nbMessageRxAsser || readAsserMessage()!=ERROR) return 0;
	clearAsserMessage(1);
	return 1; // return 1 si l'instruction non bloquante en cour est finie
}

Position reponsePosition()  
// nb : pas de conversion -> postion non negative et non flotant, valeur max 1023cm
{
Position position;
while (!nbMessageRxAsser);
if ((readAsserMessage() & 0xF000) != POSITION_X) {
	ret = erreurRxAsser;
	return position;
}
position.x = getAsserMessage() & 0x0FFF;
while (!nbMessageRxAsser);
if ((readAsserMessage() & 0xF000) != POSITION_Y) {
	ret = erreurRxAsser;
	return position;
}
position.y = inMessage & 0x0FFF;
return position;
}

int getDelta(){
return 0;
}

/* [] END OF FILE */
