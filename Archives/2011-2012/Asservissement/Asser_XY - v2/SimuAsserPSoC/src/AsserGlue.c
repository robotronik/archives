#include "../../sources/Boolean.h"
#include "../../sources/messageAsser.h"
#include "../../sources/init.h"
#include "../../sources/asser.h"
#include "../../sources/motors.h"

#include "Modelisation.h"
#include "Canaux.h"
#include "Acquirer.h"

#include <unistd.h>

void *asserThread(void *state) {
	char *state2 = state;
	/*struct timeval time_var;
	struct timeval time_prec;
	unsigned int time_diff;
	unsigned int nb_pass = 0;*/
	
	init();
	//gettimeofday(&time_prec, NULL);
	while (*state2 < 1) {
		asser();
		messages_processing();
		/*nb_pass ++;
		gettimeofday(&time_var, NULL);
		time_diff = (time_var.tv_sec - time_prec.tv_sec) * 1000 + (time_var.tv_usec - time_prec.tv_usec) / 1000;
		if (time_diff > 1000) {
			time_prec.tv_sec ++;
			printf("%u ", nb_pass);
			nb_pass = 0;
		}*/
	}
	return NULL;
}

volatile boolean doitAttendre = FALSE;
struct {
	int PWM1IE;
} IEC3bits;

void UART_putc(unsigned char c)
{
	if (isDebugEnabled) return;
	
	UART_put(asser_to_PSoC, c);
	pthread_kill(PSoC_pthread, SIGUSR1);
	usleep(87);
}

#define RX_BUFFER_SIZE 16
extern unsigned char rxBuffer[RX_BUFFER_SIZE];
extern unsigned short indexRxBuffer;
extern unsigned short rxBufferLength;

void Asser_UART_handler(unsigned char byte) {	// Voir sources de l'asser
	rxBuffer[(indexRxBuffer + rxBufferLength) % RX_BUFFER_SIZE] = byte;
	rxBufferLength ++;
}

int UART_getc(unsigned char *byte) {			// Voir sources de l'asser
	sigset_t sigset;
	
	sigaddset(&sigset, SIGUSR1);
	if (rxBufferLength) {
		*byte = rxBuffer[indexRxBuffer];
		pthread_sigmask(SIG_BLOCK, &sigset, NULL);		// Désactivation de l'interruption pour modifier les variables globales
		rxBufferLength --;
		indexRxBuffer ++; indexRxBuffer %= RX_BUFFER_SIZE;
		pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);	// Activation de l'interruption
		return 1;
	}
	return 0;
}

static double dL = 0, dR = 0; // [m]

void getQEI(long *pPosL, long *pPosR) {
	*pPosL = dL * METRE;
	*pPosR = dR * METRE;
	
	/*static int i = 0;
	if (i < 40 && isDebugEnabled && commandeCourante != FREINAGE) {
		i ++;
		printf("2 %ld\t%ld\n", *pPosL, *pPosR);
	}*/
}

#define TMAX		(10*ASSER_FREQ)		// [per] (= 10 s)

inline void motors_set_speed(short speedL, short speedR) {
	int u;							// [mV]
	double a;						// [m/s²]
	static double vL = 0, vR = 0;	// [m/s]
	static long long t = 0;
	if (t < TMAX * 2 / 10) t ++;
	
	if (speedL > MAX_SPEED) speedL = MAX_SPEED;
	else if (speedL < -MAX_SPEED) speedL = -MAX_SPEED;
	if (speedR > MAX_SPEED) speedR = MAX_SPEED;
	else if (speedR < -MAX_SPEED) speedR = -MAX_SPEED;
	
	u = (long) speedL*VBAT_MAX/MAX_SPEED;
	//u -= VBAT_MAX*t/TMAX;
	a = motors_run(u, 0, vL);
	vL += a / ASSER_FREQ;
	dL += vL / ASSER_FREQ;
	
	u = (long) speedR*VBAT_MAX/MAX_SPEED;
	//u -= VBAT_MAX*t/TMAX;
	a = motors_run(u, 1, vR);
	vR += a / ASSER_FREQ;
	dR += vR / ASSER_FREQ;
	
	if (isDebugEnabled && commandeCourante != FREINAGE) {
		acquire();
		/*static int i = 0;
		if (i >= 4000) {
			i = 0;
			//printf("0 %hd\t%hd\n", speedL, speedR);
			//printf("1 %lf\t%lf\n", vL, vR);
			printf("2 %d\n", (int) (a*10));
		}
		i ++;*/
	}
	doitAttendre = FALSE;
}

inline void motors_stop(void) {
	motors_set_speed(0, 0);
}
