/*-----------------------------------------------------*
 * Main :                                              *
 * - inclusion des headers et librairies               *
 * - declaration structure pid                         *
 * - definission des constantes                        *
 *-----------------------------------------------------*/





#ifndef _MAIN_H
#define _MAIN_H

#include <p33FJ128MC802.h>  // Nouvelle carte

#define MAX_SPEED 400   // Consige max de vitesse dû au rapport cyclique du pwm
#define V_MAX_DEMARRAGE 0
#define VITESSE 250 // Defini la vitesse maximale souhaitée du robot ( max = MAX_SPEED)
#define RAPPORT_CONVERSION 348//3.275      // angle[mDeg]/ticks = diamètre roue*360000/(N*distance entre roue(= 264mm))
#define D 2230           // Distance parcourue pour un tour de roue en 10^-1 mm 710*3.14 ( diametre roue * pi)
#define N 28500        // Nombre de point pour un tour de roue 500*6 (Nombre de point des codeurs * rapport du reducteur)
//#define max_alpha 3125		// Consigne maximale donnée au pid sans saturation (10 Deg)
//#define max_delta 13453		// Consigne maximale donnée au pid sans saturation (10 cm)
//#define MAX_PID 1657800 // Decrit la valeur maximale de sortie des pid ((13453+3125)*100)
#define LIMITE_A 300
#define LIMITE_A_SUR_ALPHA 4000

// parametre pour designer le moteur gauche/droit
typedef enum {LEFT, RIGHT} Side;

// Structure contenant l'état, à un instant t, du correcteur PID :
typedef struct PID PID;
struct PID
{
  /* Erreur */
  long err_1;           /* err(t - dt) */
  long err_0;           /* err(t)      */

  /* Derivée, Intégrale */
  long derr;            /* derr(t)/dt  */
  long ierr;            /* integrale de err de 0 à t */

  /* Sortie */
  long out;             /* sortie du correcteur */

  /* Coefficients */
  long Kp, Kd, Ki;      /* coefficients Kp, Kd et Ki */
}; /* End of struct PID */

 #include "asser.h"
 #include "debug.h"
 #include "conversion.h"
 #include "init.h"
 #include "message.h"
 #include "motors.h"
 #include "uart.h" // couche basse, autonome pour ne pas surcharger le reste



 #endif
