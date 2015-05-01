/*-----------------------------------------------------*
 * Protocole de ommunication avec l'uart               *
 *-----------------------------------------------------*/






#ifndef _MESSAGE_H
#define _MESSAGE_H


/* Fonctions */
// externe
int message_processing(unsigned short inMessage);
// interne
int reset();
int test_uart(void);
int start(void);
int stop(void);
int set_alpha(unsigned short inMessage);
int set_delta(unsigned short inMessage);
int get_alpha();
int get_delta();
int set_p_alpha(unsigned short inMessage);
int set_i_alpha(unsigned short inMessage);
int set_d_alpha(unsigned short inMessage);
int set_p_delta(unsigned short inMessage);
int set_i_delta(unsigned short inMessage);
int set_d_delta(unsigned short inMessage);
int get_parameters(void);

/* Definition des commandes (id 4 bits + valeur 12 bits) */

#define RESET			0x0000	// Initialiser le programme
#define TEST			0x1000	// Tester la connexion
#define START			0x2000	// Démarrer le déplacement
#define STOP			0x3000	// Arrêter le déplacement
#define SET_ALPHA		0x4000	// Modifier la consigne ALPHA
#define SET_DELTA		0x5000	// Modifier la consigne DELTA
#define GET_ALPHA		0x6000	// 
#define GET_DELTA		0x7000	// 
#define SET_P_ALPHA		0x9000	// Modifier le coefficient P du PID sur ALPHA
#define SET_I_ALPHA		0xA000	// Modifier le coefficient I du PID sur ALPHA
#define SET_D_ALPHA		0xB000	// Modifier le coefficient D du PID sur ALPHA
#define SET_P_DELTA		0xC000	// Modifier le coefficient P du PID sur DELTA
#define SET_I_DELTA		0xD000	// Modifier le coefficient I du PID sur DELTA
#define SET_D_DELTA		0xE000	// Modifier le coefficient D du PID sur DELTA
#define GET_PARAMETERS	0xF000	// Demander les paramètres actuels

/* Définition des réponses (id 4 bits + valeur 12 bits) */

#define READY			0x0000	// Programme prêt
#define TEST			0x1000	// Connexion bonne
#define DONE			0x2000	// Déplacement terminé
#define	ERROR			0x3000	// Erreur d'éxecution
#define ALPHA			0x4000	// ALPHA actuel
#define DELTA			0x5000	// DELTA actuel
#define CONSIGNE_ALPHA	0x6000	// Consigne ALPHA
#define CONSIGNE_DELTA	0x7000	// Consigne DELTA
#define P_ALPHA			0x8000	// Coefficient P du PID sur ALPHA
#define I_ALPHA			0x9000	// Coefficient I du PID sur ALPHA
#define D_ALPHA			0xA000	// Coefficient D du PID sur ALPHA
#define P_DELTA			0xB000	// Coefficient P du PID sur DELTA
#define I_DELTA			0xC000	// Coefficient I du PID sur DELTA
#define D_DELTA			0xD000	// Coefficient D du PID sur DELTA

/* Autres */



#endif
