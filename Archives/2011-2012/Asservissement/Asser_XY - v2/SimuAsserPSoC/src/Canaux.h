#ifndef _CANEAUX_H
#define _CANEAUX_H

#include "UART.h"

extern UART_canal_t *PSoC_to_asser, *asser_to_PSoC, *PC_to_PSoC;
extern pthread_t main_pthread, asser_pthread, PSoC_pthread;

#endif
