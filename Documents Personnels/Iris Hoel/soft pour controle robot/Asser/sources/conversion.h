/*-----------------------------------------------------*
 * Conversion grandeurs                                *
 *-----------------------------------------------------*/




#ifndef _CONVERSION_H
#define _CONVERSION_H

#define N (int)28500        		// Nombre de point pour un tour de roue 500*6 (Nombre de point des codeurs * rapport du reducteur)
#define D (int)2182         		// Distance parcourue pour un tour de roue en 10^-1 mm 695*3.14 ( diametre roue * pi)
#define RAPPORT_CONVERSION (int)301 // [ticks/Deg/roue] = N*distance entre roue(=264mm)/(diamètre roue(=70mm)*360)

// interne
long conversion_alpha_pt_vers_deg(long a);
long conversion_alpha_deg_vers_pt(long a);
long conversion_delta_pt_vers_diziememm(long a);
long conversion_delta_diziememm_vers_pt(long a);

#endif
