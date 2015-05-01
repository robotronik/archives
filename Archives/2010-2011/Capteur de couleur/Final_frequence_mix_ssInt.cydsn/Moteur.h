/* ========================================
 *
 * Copyright Phelma, 2011
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF PHELMA.
 *
 * ========================================
*/
#ifndef H_MOTEUR
#define H_MOTEUR

#include <device.h>

#define V_RAPIDE 50	// = V_LENT = 30 ?
#define V_LENT	 35
#define MOTOR_COEFF_L	 93 // [% de la vitesse demandée] roue gauche ralentie pour égaliser les 2 moteurs

void init_moteur(void);
void commande_moteurs(T_sensors sensors);

#endif

/* [] END OF FILE */
