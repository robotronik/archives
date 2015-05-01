/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/




#include "main.h"
#include "uart.h"

extern long consigneAlpha, consigneDelta;    // Consignes (alpha, delta)

int message_processing(unsigned short inMessage)
{
	switch(inMessage & 0xF000)
	{
		case RESET: reset(); break;
		case TEST: test_uart(); break;
		//case START: start(); break;
		//case STOP: stop(); break;
		case SET_ALPHA:	set_alpha(inMessage & 0x0FFF); break;
		case SET_DELTA:	set_delta(inMessage & 0x0FFF); break;
		//case PAUSE: pause(); break;
		//case GET_DELTA: get_delta(); break;
		//case SET_P_ALPHA: set_p_alpha(inMessage & 0x0FFF); break;
		//case SET_I_ALPHA: set_i_alpha(inMessage & 0x0FFF); break;
		//case SET_D_ALPHA: set_d_alpha(inMessage & 0x0FFF); break;
		//case SET_P_DELTA: set_p_delta(inMessage & 0x0FFF); break;
		//case SET_I_DELTA: set_i_delta(inMessage & 0x0FFF); break;
		//case SET_D_DELTA: set_d_delta(inMessage & 0x0FFF); break;
		//case GET_PARAMETERS: get_parameters(); break;

		default: return -1; // Erreur de commande
	}

	return 0;
}

int reset() {
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

int set_alpha(unsigned short inMessage)
{
	//consigneAlpha = conversion_alpha_inMessage_vers_pt(inMessage);
	return 0;
}

int set_delta(unsigned short inMessage)
{
	//consigneDelta = conversion_delta_inMessage_vers_pt(inMessage);  // conversion en points de inMessage
	return 0;
}
