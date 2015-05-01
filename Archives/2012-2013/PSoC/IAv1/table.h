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
#ifndef TABLE_H
#define TABLE_H

#define NB_VERRES 12

#include "constantes.h"

enum StateVerre {SUR_TABLE, RECUP, ADV};
typedef enum StateVerre StateVerre;

struct Verre
{
	Pos p;//alpha non
	StateVerre state;
};
typedef struct Verre Verre;

void initVerre(void);
//long creerIndice(int i1, int i2, int i3);
void checkVerre(void);
void majToRecup(int indice);
//long ajouter(long indice, long n);

#endif
//[] END OF FILE
