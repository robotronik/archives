#ifndef _ASSER_GLUE_H
#define _ASSER_GLUE_H

void *asserThread(void *state);
void Asser_UART_handler(unsigned char byte);

extern struct {
	int PWM1IE;
} IEC3bits;

#endif
