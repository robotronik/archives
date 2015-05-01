/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/




#include "main.h"
#include "uart.h"

extern long consigne_alpha,    // Consigne (alpha, delta)
            consigne_delta,
            alpha,            // Position (alpha, delta)
			delta;
extern PID pid_alpha; // Structure contenant l'état, à un instant t, du correcteur PID
extern PID pid_delta;
extern int stop_asser;


int message_processing(unsigned short inMessage)
{
	switch(inMessage & 0xF000)
	{
		case RESET: reset(); break;
		case TEST: test_uart(); break;
		case START: start(); break;
		case STOP: stop(); break;
		case SET_ALPHA:	set_alpha(inMessage & 0x0FFF); break;
		case SET_DELTA:	set_delta(inMessage & 0x0FFF); break;
		//case PAUSE: pause(); break;
		case GET_DELTA: get_delta(); break;
		case SET_P_ALPHA: set_p_alpha(inMessage & 0x0FFF); break;
		case SET_I_ALPHA: set_i_alpha(inMessage & 0x0FFF); break;
		case SET_D_ALPHA: set_d_alpha(inMessage & 0x0FFF); break;
		case SET_P_DELTA: set_p_delta(inMessage & 0x0FFF); break;
		case SET_I_DELTA: set_i_delta(inMessage & 0x0FFF); break;
		case SET_D_DELTA: set_d_delta(inMessage & 0x0FFF); break;
		case GET_PARAMETERS: get_parameters(); break;

		default: return -1; // Erreur de commande
	}

	return 0;
}

int reset(){
	init();
	return 0;
}

int test_uart(void)
{
	allumer_del();
	send_message(TEST);
	pause_ms(500);
	eteindre_del();

	return 0;
}

int start(void)
{
	stop_asser=0;
	return 0;
}

int stop(void)
{
	printf("Assert : STOP\n");
	stop_asser=1;
	return 0;
}

int set_alpha(unsigned short inMessage)
{
	printf("Assert : set alpha %d\n", inMessage);
	consigne_alpha = conversion_alpha_inMessage_vers_pt(inMessage);
	stop_asser = 0;
    motors_reset_qei(LEFT);
    motors_reset_qei(RIGHT);
	return 0;
}

int set_delta(unsigned short inMessage)
{
	printf("Assert : set delta %d\n", inMessage);
	consigne_delta = conversion_delta_inMessage_vers_pt(inMessage);  // conversion en points de inMessage
	printf("Assert : set delta after conversion %ld\n", consigne_delta);
	stop_asser = 0;
    motors_reset_qei(LEFT);
    motors_reset_qei(RIGHT);
	return 0;
}

int get_alpha()
{
	send_message((unsigned short)(ALPHA | conversion_alpha_pt_vers_outMessage(alpha))); // a decouper en deux char
	return 0;
}

int get_delta()
{
	send_message((unsigned short)(DELTA | conversion_delta_pt_vers_outMessage(delta))); // a decouper en deux char
	return 0;
}

// fonction si dessous sans conversion, théoriquement non fonctionnelles.


int set_p_alpha(unsigned short inMessage)
{
	pid_alpha.Kp=inMessage;

	return 0;
}

int set_i_alpha(unsigned short inMessage)
{
	pid_alpha.Ki=inMessage;

	return 0;
}

int set_d_alpha(unsigned short inMessage)
{
	pid_alpha.Kd=inMessage;

	return 0;
}

int set_p_delta(unsigned short inMessage)
{
	pid_delta.Kp=inMessage;

	return 0;
}

int set_i_delta(unsigned short inMessage)
{
	pid_delta.Ki=inMessage;

	return 0;
}

int set_d_delta(unsigned short inMessage)
{
	pid_delta.Kd=inMessage;

	return 0;
}

int get_parameters(void)
{


	return 0;
}
