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

#ifndef GLOBAL_H
#define GLOBAL_H

typedef enum { JAUNE, ROUGE } Couleur ;
typedef enum { ATTENTE_TIRETTE, ATTENTE_PETIT, DEBUT_OBJECTIF, DEBUT_ACTION, EN_COURS, FIN_ACTION, FIN_OBJECTIF, STOP_SHARP, FIN_MATCH } Etat ;
typedef enum { SERVO_LANCEUR_DROIT, SERVO_LANCEUR_GAUCHE, QUEUE, MOUVEMENT_DELTA, MOUVEMENT_ALPHA, STOP, FIN } TypeAction;

typedef struct
{
    int x, y, alpha;
} Position;

typedef struct
{
    int parametre;
    TypeAction type;
} Action;
      
typedef struct
{
    Position pos;
    Action actions[20];
} Objectif;

#endif

//[] END OF FILE
