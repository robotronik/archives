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

#ifndef GESTION_BRAS_H
#define GESTION_BRAS_H

#define POS_DROIT_HAUT_REPLIER 100
#define POS_DROIT_HAUT_ARMER 205
#define POS_DROIT_HAUT_TAPER 140//150

#define POS_DROIT_BAS_REPLIER 87
#define POS_DROIT_BAS_ARMER 120 
#define POS_DROIT_BAS_TAPER 200//180

#define POS_GAUCHE_HAUT_REPLIER 200//105//200//160
#define POS_GAUCHE_HAUT_ARMER 105
#define POS_GAUCHE_HAUT_TAPER 160//150//205

#define POS_GAUCHE_BAS_REPLIER 220
#define POS_GAUCHE_BAS_ARMER 183 
#define POS_GAUCHE_BAS_TAPER 100//120//80

void armerBrasDroitHaut(void);
void replierBrasDroitHaut(void);
void taperBrasDroitHaut(void);
void starterBrasDroitHaut(void);
void stopBrasDroitHaut(void);

void armerBrasDroitBas(void);
void replierBrasDroitBas(void);
void taperBrasDroitBas(void);
void starterBrasDroitBas(void);
void stopBrasDroitBas(void);

void armerBrasGaucheHaut(void);
void replierBrasGaucheHaut(void);
void taperBrasGaucheHaut(void);
void starterBrasGaucheHaut(void);
void stopBrasGaucheHaut(void);

void armerBrasGaucheBas(void);
void replierBrasGaucheBas(void);
void taperBrasGaucheBas(void);
void starterBrasGaucheBas(void);
void stopBrasGaucheBas(void);

void armerBrasHaut(void);
void replierBrasHaut(void);
void taperBrasHaut(void);
void starterBrasHaut(void);
void stopBrasHaut(void);

void armerBrasBas(void);
void replierBrasBas(void);
void taperBrasBas(void);
void starterBrasBas(void);
void stopBrasBas(void);


#endif
//[] END OF FILE
