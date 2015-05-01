#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define SIMU
//#define MAP_ENABLE   // A supprimer si on n'a pas le bras

//#define DEBUG_ETAT  // Envoi a XBee de l etat courant
#define XBEE_ACTIF

#ifdef XBEE_ACTIF
#define UART_XBee_PutChar(c) printf("XBee : %2X\n", c)
#define UART_XBee_WriteTxData(c) printf("XBee : %2X\n", c)
#else
#define UART_XBee_PutChar(c) 0
#define UART_XBee_WriteTxData(c) 0
#endif

#include "../Simu/Logger.h"

#endif // CONFIG_H_INCLUDED
