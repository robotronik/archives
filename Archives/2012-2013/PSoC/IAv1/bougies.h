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

#include "constantes.h"

#define RAYON_GATEAU 500.0
#define RAYON_ROBOT 135.0//90//75
#define DISTANCE_2_GATEAU_ROUGE 22.0//110.0
#define DISTANCE_2_GATEAU_BLEU 70.0
#define RAYON_ROUGE (RAYON_GATEAU+DISTANCE_2_GATEAU_ROUGE+RAYON_ROBOT)
#define RAYON_BLEU (RAYON_GATEAU+DISTANCE_2_GATEAU_BLEU+RAYON_ROBOT)

#define INCREMENT_ANGLE_BOUGIES_BAS 15.
#define ANGLE_ORIGINE_BAS 7.5

#define INCREMENT_ANGLE_BOUGIES_HAUT 22.5
#define ANGLE_ORIGINE_HAUT 11.25

#define NB_BOUGIES_BAS 12
#define NB_BOUGIES_HAUT 8

#define X_CENTRE_GATEAU 1480.

enum StateBougie {ALLUME, ETEINTE};
typedef enum StateBougie StateBougie;

struct Bougie
{
	Pos p;
	Couleur c;
	StateBougie state;
};
typedef struct Bougie Bougie;

void initGateau(void);
void majToEteinteBas(int indice);
void majToEteinteHaut(int indice);


//[] END OF FILE
