#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "prog2.h"
#include "message.h"
#include "conversion.h"
#include "motors.h"

int Vg = 0;
int Vd = 0;
long long posL = 0; // posL/4000[tck]
long long posR = 0; // posR/4000[tck]
char aAfficher1[512];


void allumer_del() {
}

void eteindre_del() {
}

void pause_ms(unsigned int n) {
	usleep(1000*n);
}

void pause_s(unsigned int n) {
	usleep(1000000*n);
}

void UART_putc(unsigned char c)
{
    // Envoi du message dans l'emulation de l'UART !
    sendMessageAsserToPsoc(c, 0);
}

void send_message(long m) {
	/*
	if (m < 0) strcat(aAfficher1, "- ");
	else  strcat(aAfficher1, "+ "); //*/
	//*
	char strtmp[64];
	sprintf(strtmp, "%ld ", m);
	strcat(aAfficher1, strtmp); //*/
}

void motors_get_qei(long *pPosL, long *pPosR) {
	*pPosL = posL / 4000;
	*pPosR = posR / 4000;
}

void motors_reset_qei() {
	posL = 0; // tck/4000
	posR = 0; // tck/4000
}

void motors_set_speed (short speedL, short speedR) {
	Vg = speedL;
	Vd = speedR;

	posL += (long long) Vg * 5000 * N / D / FREQ_ASSER;	// 1 => 500 mm/s
	posR += (long long) Vd * 5000 * N / D / FREQ_ASSER;

}

void motors_stop () {
}

void CyDelay(unsigned int n) {
	usleep(1000*n);
}
