/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#ifndef _DEBUG_H
#define _DEBUG_H

#include "Boolean.h"

extern boolean isDebugEnabled;
extern unsigned short temps_pause_1, temps_pause_2;
extern unsigned short debug_setting;	// Peut servir: réglable par PSoC

#if !defined(COLLECTEUR) && !defined(SIMU)
#define PAUSE_MS(nb_ms)	do {										\
	for(temps_pause_2=0; temps_pause_2<nb_ms; temps_pause_2++)		\
     	for(temps_pause_1=0; temps_pause_1<4000; temps_pause_1++)	\
			Nop();													\
} while (0)
#else	// #if !defined(COLLECTEUR) && !defined(SIMU)
#define PAUSE_MS(nb_ms)
#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)

#define FLASH_LONG	do {	\
	_RB5 = 0;				\
	PAUSE_MS(600);			\
	_RB5 = 1;				\
	PAUSE_MS(300);			\
} while (0)

#define FLASH_COURT	do {	\
	_RB5 = 0;				\
	PAUSE_MS(300);			\
	_RB5 = 1;				\
	PAUSE_MS(300);			\
} while (0)

// DEL
inline void allumer_del(void);
inline void eteindre_del(void);

// Time
inline void pause_s(unsigned short n);

// Execution time testing
//#define NB_GET_TIME 8	// [per] 1 période d'asser (2 octets à envoyer) par SAVE_TIME
#ifdef NB_GET_TIME

extern unsigned short time_tmp, time_max[NB_GET_TIME], time_index;
#define SAVE_TIME(num_save)	do {						\
		time_max[num_save] = P1TMRbits.PTMR - time_tmp;	\
		time_tmp = P1TMRbits.PTMR;						\
	} while (0)
#define SEND_TIME	do {													\
		for (time_index=0; time_index<NB_GET_TIME; time_index++)			\
			UART_send_tab((unsigned char *) (time_max + time_index), 2);	\
		time_index = 0;														\
	} while (0)
	
#else	// #ifdef NB_GET_TIME

// Execution time testing
#define SAVE_TIME(num_save)

#endif	// #ifdef NB_GET_TIME

inline void debugProcess();

#endif
