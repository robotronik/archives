/*-----------------------------------------------------*
 * Calcul des pid et asservissement                    *
 *-----------------------------------------------------*/



#include "main.h"


extern long alpha,
            delta,
            consigne_alpha,    // Consigne (alpha, delta)
            consigne_delta,
            commande_alpha,    // Commande (alpha, delta)
            commande_alpha_old,
            commande_delta,
            commande_delta_old,
            commande_l,        // Commande converties sur chaque roue
            commande_r,
            l,                 // Vitesse de chaque roue
            r;

extern PID pid_alpha; // Structure contenant l'état, à un instant t, du correcteur PID
extern PID pid_delta;
extern int stop_asser;

//  static long consigne_delta_temp = ((long)max_delta + max_alpha) * V_MAX_DEMARRAGE/VITESSE;

// RAMPE
/*  if (consigne_delta > 0) {
        if (consigne_delta_temp <= ((long)max_delta + max_alpha) * V_MAX_DEMARRAGE/VITESSE) consigne_delta_temp = ((long)max_delta + max_alpha) * V_MAX_DEMARRAGE/VITESSE;
        if (consigne_delta_temp >= consigne_delta) consigne_delta_temp = consigne_delta;
        else consigne_delta_temp += 10;
    } else {
        if (consigne_delta_temp > -((long)max_delta + max_alpha) * V_MAX_DEMARRAGE/VITESSE) consigne_delta_temp = -((long)max_delta + max_alpha) * V_MAX_DEMARRAGE/VITESSE;
        if (consigne_delta_temp < consigne_delta) consigne_delta_temp = consigne_delta;
        else consigne_delta_temp -= 10;
    }
*/
// consigne_delta_temp = consigne_delta;

void asservir()
{
	if(consigne_alpha != 0)
	{
		asser_tourner(consigne_alpha);
		pause_ms(200);	// peut etre a changer	
		return;
	}
	else if(consigne_delta != 0)
	{
		asser_avancer(consigne_delta);
		pause_ms(200);	// peut etre a changer
		return;
	}
	else
	{
		motors_stop();
	}
}

void asser_tourner(long consigne){
long delta = 0;
long alpha = 0;
PID_init(&pid_alpha,100,0,0); // nb ki divisé par 10 pour système stable
PID_init(&pid_delta,2000,0,0);
long max_consigne = 400;        // Consigne maximale donnée au pid sans saturation (10 cm)
long max_pid = 40000*2;
while(((alpha < consigne && consigne > 0) || ( alpha > consigne && consigne < 0)) && !stop_asser){ 
    /** Actualisation position actuelle (l, r) **/
    l = -motors_get_qei(LEFT);//*4;  //recupere les données des codeurs, repesentant dd
    motors_reset_qei(LEFT);    //remise a zero des codeurs
    r = -motors_get_qei(RIGHT);//*4+1;
    motors_reset_qei(RIGHT);

    /** Changement variable (l, r) -> (delta, alpha) **/
    delta += (r + l)/2; // diviser par 2 ?
    alpha += (r - l)/2; // valeur négatives ?

    /** PID asservissement de position **/
    if( consigne - alpha>max_consigne ){
                PID_run(&pid_alpha, (max_consigne));
    }
   else{
       if(consigne - alpha<-max_consigne){
            PID_run(&pid_alpha, -(max_consigne));
        }
        else{ PID_run(&pid_alpha, (consigne - alpha));}
    }
    PID_run(&pid_delta, -delta);

    commande_alpha = pid_alpha.out;
    commande_delta = pid_delta.out;

    /** Limitation A sur alpha **/
    //*/
	if ((commande_alpha-commande_alpha_old)>LIMITE_A_SUR_ALPHA) commande_alpha = commande_alpha_old+LIMITE_A;
    if ((commande_alpha-commande_alpha_old)<(-LIMITE_A_SUR_ALPHA)) commande_alpha = commande_alpha_old-LIMITE_A;
    commande_alpha_old = commande_alpha; 
	//*/
    
	/** Changement de variable (commande_delta, commande_alpha) -> (commande_r, commande_l) **/
    commande_r = ( commande_delta + commande_alpha );
    commande_l = ( commande_delta - commande_alpha );

    /** envoi au moteur **/
    //motors_set_speed envoi commande_r sous forme de rapport cyclique (en divisant par 200)
    motors_set_speed(RIGHT, commande_r*(long)VITESSE/max_pid);  // avec
    motors_set_speed(LEFT, commande_l*(long)VITESSE/max_pid);
    }
	if(consigne != 0){
    consigne_alpha = 0;
    motors_stop();
    init_parametre();
	send_message(DONE);
	}
}
void asser_avancer(long consigne) {
long delta = 0;
long alpha = 0;
// kp = 36000 pour limite oscillation
//PID_init(&pid_alpha,16800,170,0); // nb ki divisé par 10 pour système stable
PID_init(&pid_alpha,1000,10,0); // nb ki divisé par 10 pour système stable
//PID_init(&pid_alpha,100,0,0);
PID_init(&pid_delta,100,0,0);
long max_consigne = 13453;        // Consigne maximale donnée au pid sans saturation (10 cm)
long max_pid = 1345300;
consigne = consigne*8;
long consigne_temp =  0;//max_pid* V_MAX_DEMARRAGE/VITESSE; pour partir plus vite, ne pas commencer la rampe à vitesse 0
while(((delta < consigne && consigne > 0) || ( delta> consigne && consigne < 0)) && !stop_asser){


// RAMPE
 /* 
	if (consigne > 0) {
        if (consigne_temp <= 0) consigne_temp = 0;
        if (consigne_temp >= consigne) consigne_temp = consigne;
        else consigne_temp += 13453 /250;
    } else {
        if (consigne_temp > -max_pid * V_MAX_DEMARRAGE/VITESSE) consigne_temp = -max_pid * V_MAX_DEMARRAGE/VITESSE;
        if (consigne_temp < consigne_delta) consigne_temp = consigne_delta;
        else consigne_temp -= 10;
    }
//*/

consigne_temp = consigne;
    /** Actualisation position actuelle (l, r) **/


    l = -motors_get_qei(LEFT)*8;  //recupere les données des codeurs, repesentant dd
    r = -motors_get_qei(RIGHT)*8+3;
	motors_reset_qei(LEFT);
    motors_reset_qei(RIGHT);

    /** Changement variable (l, r) -> (delta, alpha) **/
    delta += (r + l)/2; // diviser par 2 ?
    alpha += (r - l)/2; // valeur négatives ?

    /** PID asservissement de position **/
    PID_run(&pid_alpha, -alpha);

     if( consigne_temp - delta>max_consigne ){
		    	PID_run(&pid_delta, (max_consigne));
		}
    else{
	       if(consigne_temp - delta<-max_consigne){
  	  			PID_run(&pid_delta, -(max_consigne));
  			}
			else{ PID_run(&pid_delta, (consigne_temp - delta));} // s'arrete brutalement 

		}//*/

	commande_alpha = pid_alpha.out;
    commande_delta = pid_delta.out;


    /** Limitation A sur delta **/
    if ((commande_delta-commande_delta_old ) > LIMITE_A ) commande_delta = commande_delta_old+LIMITE_A;
    if ((commande_delta-commande_delta_old )<(-LIMITE_A)) commande_delta = commande_delta_old-LIMITE_A;
    commande_delta_old = commande_delta;

    /** Changement de variable (commande_delta, commande_alpha) -> (commande_r, commande_l) **/
    commande_r = ( commande_delta + commande_alpha );
    commande_l = ( commande_delta - commande_alpha );

    /** envoi au moteur **/
	//motors_set_speed envoi commande_r sous forme de rapport cyclique (en divisant par 200)
    motors_set_speed(RIGHT, commande_r*(long)VITESSE/max_pid);  // avec
    motors_set_speed(LEFT, commande_l*(long)VITESSE/max_pid);
    }
	if(consigne != 0){
    consigne_delta = 0;
    motors_stop();
    init_parametre();
	send_message(DONE);
	}
}

/*
    Met à jour l'état du correcteur PID avec les nouvelles valeurs,
    et recalcule la sortie.

    Paramètres :
        - pid : pointeur vers la structure contenant l'état du correcteur PID
        - err : valeur courante du signal d'erreur
*/
void PID_run (PID *pid, long err)
{
    // Erreur
    pid->err_1 = pid->err_0;
    pid->err_0 = err;

    // Dérivée
    pid->derr = pid->err_0 - pid->err_1;

    // Intégrale et protection anti-débordement
    pid->ierr += pid->err_0;

    if (pid->ierr > 0xFFFFF)
    pid->ierr = 0xFFFFF;         // troncature à 2^20 - 1

    if (pid->ierr < (-0xFFFFF))
    pid->ierr = -0xFFFFF;        // troncature à -(2^20 - 1)

    // Calcul sortie PID
    pid->out = pid->Kp * pid->err_0 + (pid->Ki * pid->ierr) + pid->Kd * pid->derr;
}

