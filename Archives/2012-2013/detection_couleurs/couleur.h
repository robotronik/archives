#include <stdio.h>
#include <math.h>
#include "highgui.h"
#include "cv.h"

#ifndef __couleur_h__
#define __couleur_h__ 

#define LAPTOP 0
#define CAMERA 1

#define NB_PICKS 10

#define max( a, b ) ((a) > (b) ? (a) : (b))


int vision(char* M1, char* M2, char *M3, char *M4, char *M5, char *M6, int couleurEquipe);

#endif //__couleur_h___
