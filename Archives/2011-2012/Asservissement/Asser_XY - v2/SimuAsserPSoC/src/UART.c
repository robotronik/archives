#include <stdlib.h>
#include "UART.h"

typedef struct {
	struct list_head link;
	unsigned char b;
} linked_byte;

UART_canal_t *UART_canal_init(void) {
	UART_canal_t *canal = (UART_canal_t *) malloc(sizeof(*canal));
	INIT_LIST_HEAD(&canal->byte_fifo);
	pthread_mutex_init(&canal->mutex, NULL); 
	return canal;
}

void UART_canal_destroy(UART_canal_t *canal) {
	pthread_mutex_destroy(&canal->mutex);
	free(canal);
}

#include "Canaux.h"
void UART_put(UART_canal_t *canal, unsigned char b) {
	linked_byte *byte = (linked_byte *) malloc(sizeof(*byte));
	byte->b = b;
	pthread_mutex_lock(&canal->mutex);
	list_add_tail(&byte->link, &canal->byte_fifo);
	pthread_mutex_unlock(&canal->mutex);
}

int UART_get(UART_canal_t *canal, unsigned char *b) {
	pthread_mutex_lock(&canal->mutex);
	if (list_empty(&canal->byte_fifo)) {
		pthread_mutex_unlock(&canal->mutex);
		return 0;
	}
	linked_byte *byte = list_first_entry(&canal->byte_fifo, linked_byte, link);
	list_del(&byte->link);
	pthread_mutex_unlock(&canal->mutex);
	*b = byte->b;
	free(byte);
	return 1;
}
