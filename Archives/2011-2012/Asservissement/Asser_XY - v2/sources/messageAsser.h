/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_ASSER_H
#define _MESSAGE_ASSER_H

#include "messageCommun.h"
#include "Boolean.h"
#include "uart.h"
#include "debug.h"

#define NB_MAX_OCT_COMMANDES 128
extern unsigned char commande[NB_MAX_OCT_COMMANDES];
extern short nbCommande;
extern short indexCommande;
extern short indexLibreCommande;
extern boolean isPSoCReady;
extern boolean isPaused;

inline void connectPSoC(void);
inline void messages_processing(void);

#define SEND_DONE do {						\
	if (!isDebugEnabled) UART_putc(DONE);	\
} while (0)

#endif
