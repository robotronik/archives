/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#ifndef _ASSER_H
#define _ASSER_H



// externe
void asservir();
void asser_avancer(long consigne);
void asser_tourner(long consigne);
// interne
void PID_run (PID *pid, long err);

#endif
