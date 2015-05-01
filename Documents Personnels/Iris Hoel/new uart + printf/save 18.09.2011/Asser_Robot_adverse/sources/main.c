/*-----------------------------------------------------*
 * Main                                                *
 *-----------------------------------------------------*/





#include "main.h"

// Variables globales utilisées pour l'asservissement
long alpha;             // Position (alpha, delta)
long delta;
long consigne_alpha;    // Consigne (alpha, delta)
long consigne_delta;
long commande_alpha;    // Commande (alpha, delta)
long commande_alpha_old;
long commande_delta;
long commande_delta_old;
long commande_alpha_old;
long commande_l;        // Commande converties sur chaque roue
long commande_r;
long l;                 // Vitesse de chaque roue
long r;
PID pid_alpha; // Structure contenant l'état, à un instant t, du correcteur PID
PID pid_delta;
int stop_asser;

// assert 10ms (sur interruption)
// timer pour recup tmp du système
// 32khz -> 320 dans le registre de comparaison du timer.


int main()
{
	init();

	pause_s(1);
	motors_stop();
	stop_asser=0;

//*
//	motors_set_speed(LEFT, 300);
//	motors_set_speed(RIGHT, 300);
//while(1);
//*/



/*
   /// Asservissement sur 5*35cm

	consigne_delta =  conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	pause_s(1);//*/
/*
	consigne_delta = conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	pause_s(1);
	

//*/

/*
   /// Asservissement sur 5*18 deg

	consigne_delta = 0;// conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = conversion_alpha_mdeg_vers_pt(18000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = 0;// conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = conversion_alpha_mdeg_vers_pt(18000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = 0;// conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = conversion_alpha_mdeg_vers_pt(18000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = 0;// conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = conversion_alpha_mdeg_vers_pt(18000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = 0;// conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = conversion_alpha_mdeg_vers_pt(18000);
	asser_avancer(consigne_delta);
	pause_s(1);

	consigne_delta = 0;// conversion_delta_diziememm_vers_pt(-3500); // conversion ..
	consigne_alpha = conversion_alpha_mdeg_vers_pt(18000);
	asser_avancer(consigne_delta);
	pause_s(1);
	

//*/

/*
   /// Asservissement angle 90
	consigne_alpha = conversion_alpha_mdeg_vers_pt(360000);
	consigne_delta = 0;
	asser_tourner(consigne_alpha);
//*/

/*
while(1)
{

	consigne_delta = conversion_delta_diziememm_vers_pt(5000); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	stop_asser=0;
	pause_ms(500);
	
	consigne_alpha = conversion_alpha_mdeg_vers_pt(180000);
	consigne_delta = 0;
	asser_tourner(consigne_alpha);
	stop_asser=0;
	pause_ms(500);

	consigne_delta = conversion_delta_diziememm_vers_pt(-5000); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	stop_asser=0;
	pause_ms(500);

	consigne_delta = conversion_delta_diziememm_vers_pt(10000); // conversion ..
	consigne_alpha = 0;//conversion_alpha_mdeg_vers_pt(8000);
	asser_avancer(consigne_delta);
	stop_asser=0;
	pause_ms(500);

	consigne_alpha = conversion_alpha_mdeg_vers_pt(-180000);
	consigne_delta = 0;
	asser_tourner(consigne_alpha);
	stop_asser=0;
	pause_ms(500);
}	
//*/



	while(1)
 	{
 		//UART_putc('a');
		//printf("Assert : alive\n");
		//pause_ms(10000);
		//asservir();
		//stop_asser=0;
	}

    return 0;
}
