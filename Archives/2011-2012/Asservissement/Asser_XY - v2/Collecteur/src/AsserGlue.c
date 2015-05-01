#include "../../sources/Boolean.h"
#include "../../sources/messageAsser.h"
#include "../../sources/init.h"
#include "../../sources/asser.h"
#include "../../sources/XYAngle.h"
#include "PSoCGlue.h"

volatile boolean doitAttendre = TRUE;

inline void AsserInit() {
	isPSoCReady = TRUE;
	init();
	purpleStart();
}

void UART_putc(unsigned char c)
{
	//printf("P%02x", c);
	PSoC_UART_fromAsserSetRXByte(c);
}

static boolean UART_RX_hasData = FALSE;
static unsigned char UART_RX_byte;

void Asser_UART_setRXByte(unsigned char byte) {
	UART_RX_byte = byte;
	UART_RX_hasData = TRUE;
	messages_processing();
}

int UART_getc(unsigned char *byte) {
	if (UART_RX_hasData) {
		UART_RX_hasData = FALSE;
		*byte = UART_RX_byte;
		return 1;
	}
	return 0;
}

static long QEI_input[2] = {0};

void execAsser(char vitL, char vitR) {
	QEI_input[0] += vitL;
	QEI_input[1] += vitR;
	asser();
}

void getQEI(long *pPosL, long *pPosR) {
	*pPosL = QEI_input[0];
	*pPosR = QEI_input[1];
}

inline void motors_set_speed(short speedL, short speedR) {
}

inline void motors_stop(void) {
}
