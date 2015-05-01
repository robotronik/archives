/*-----------------------------------------------------*
 * Conversion grandeurs                                *
 *-----------------------------------------------------*/




#ifndef _CONVERSION_H
#define _CONVERSION_H

#define N (int)28500        		// [tck] 1 tour de roue (=500*57) (=Nombre de points des codeurs * rapport du reducteur)
#define D (int)2182         		// [10^-1 mm] 1 tour de roue (=695*pi) (=diametre roue * pi)
#define RAPPORT_CONVERSION (int)602 // [tck/°/roue] = N*distance entre roue(=264mm)/(diamètre roue(=70mm)*360)

long conversion_alpha_pt_vers_deg(long a);
long conversion_alpha_deg_vers_pt(long a);
long conversion_delta_pt_vers_diziememm(long a);
long conversion_delta_diziememm_vers_pt(long a);

#endif
