#ifndef _UART_SIMU_H
#define _UART_SIMU_H

#include "list.h"
#include <pthread.h>

typedef struct {
	struct list_head byte_fifo;
	pthread_mutex_t mutex;
} UART_canal_t;

UART_canal_t *UART_canal_init(void);
void UART_canal_destroy(UART_canal_t *canal);
void UART_put(UART_canal_t *canal, unsigned char b);
int UART_get(UART_canal_t *canal, unsigned char *b);

#endif
