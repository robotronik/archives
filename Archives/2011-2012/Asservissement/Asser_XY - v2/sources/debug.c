/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#include "Boolean.h"
#include "main.h"
#include "messageAsser.h"
#include "QEI.h"
#include "asser.h"
#include "debug.h"

boolean isDebugEnabled = FALSE;
unsigned short temps_pause_1, temps_pause_2, temps_pause_3;
unsigned short debug_setting = 0;	// Peut servir : réglable par PSoC
#ifdef NB_GET_TIME
short cpt_boucle = 0;
unsigned short time_tmp, time_max[NB_GET_TIME] = {0}, time_index = 0;
#endif	// #ifdef NB_GET_TIME

inline void allumer_del(void)
{
#if !defined(COLLECTEUR) && !defined(SIMU)
	_RB5 = 0; // nouvelle carte
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)
}

inline void eteindre_del(void)
{
#if !defined(COLLECTEUR) && !defined(SIMU)
	_RB5 = 1; // nouvelle carte
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)
}

inline void pause_s(unsigned short n)
{
	for(temps_pause_3=0; temps_pause_3<n; temps_pause_3++)
		PAUSE_MS(1000);
}

#include "XYAngle.h"
inline void debugProcess() {
	if (isDebugEnabled && commandeCourante != FREINAGE) {
		UART_putc(vitL);
		UART_putc(vitR);
	}
#ifdef NB_GET_TIME
	if (!cpt_boucle && commandeCourante != FREINAGE) SEND_TIME;
	
	cpt_boucle ++;
	cpt_boucle %= NB_GET_TIME;
#endif	// #ifdef NB_GET_TIME
}
