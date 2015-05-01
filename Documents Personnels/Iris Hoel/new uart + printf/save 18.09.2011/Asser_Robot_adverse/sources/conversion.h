/*-----------------------------------------------------*
 * Conversion grandeurs                                *
 *-----------------------------------------------------*/




#ifndef _CONVERSION_H
#define _CONVERSION_H


// interne
long conversion_alpha_pt_vers_mdeg(long a);
long conversion_alpha_mdeg_vers_pt(long a);
long conversion_delta_pt_vers_diziememm(long a);
long conversion_delta_diziememm_vers_pt(long a);
// externe

long conversion_alpha_inMessage_vers_pt(unsigned short inMessage);
unsigned short conversion_alpha_pt_vers_outMessage(long a);
long conversion_delta_inMessage_vers_pt(unsigned short inMessage);
unsigned short conversion_delta_pt_vers_outMessage(long a);


/*
long passage_da_dd_vers_dx(float a, float d);
long passage_da_dd_vers_dy(float a, float d);
long passage_dx_dy_vers_dd(float a, float d);
long passage_dx_dy_vers_da(float a, float d);
//*/


#endif
