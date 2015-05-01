#include "UART.h"

UART_canal_t *PSoC_to_asser, *asser_to_PSoC, *PC_to_PSoC;
pthread_t main_pthread, asser_pthread, PSoC_pthread;
