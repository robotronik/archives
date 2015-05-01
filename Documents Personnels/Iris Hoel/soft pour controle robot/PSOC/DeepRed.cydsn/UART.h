/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#if !defined(UART_H)
#define UART_H

typedef struct Trame
{
uint16 inst;  // sert plus a rien ?
uint8 part_1;
uint8 part_2;
} Trame;

typedef struct Position
{
uint16 x;
uint16 y;
} Position; 

uint16 inMessage;

uint16 readAsserMessage();

uint16 getAsserMessage();

void clearAsserMessage(uint8 nb);

Trame conversion_delta(float valeur);
Trame conversion_alpha(float valeur);

void emission(Trame trame);

void startMouvement();

void stopMouvement();

int done();

int erreur();

Position reponsePosition();

int getDelta();

/* Fonctions */
/*
int message_processing(unsigned short inMessage);
int init(void);
int test(void);
int start(void);
int stop(void);
int set_alpha(unsigned short inMessage); 
int set_delta(unsigned short inMessage);
int set_x(unsigned short inMessage); 
int set_y(unsigned short inMessage); 
int get_position(void);
int set_p_alpha(unsigned short inMessage); 
int set_i_alpha(unsigned short inMessage); 
int set_d_alpha(unsigned short inMessage); 
int set_p_delta(unsigned short inMessage); 
int set_i_delta(unsigned short inMessage); 
int set_d_delta(unsigned short inMessage);
int get_parameters(void);*/

/* D????finition des commandes (id 4 bits + valeur 12 bits) */ 

#define RESET			0x0000	// Initialiser le programme
#define TEST			0x1000	// Tester la connexion
#define START			0x2000	// DÃ©marrer le dÃ©placement
#define STOP			0x3000	// ArrÃªter le dÃ©placement
#define SET_ALPHA		0x4000	// Modifier la consigne ALPHA
#define SET_DELTA		0x5000	// Modifier la consigne DELTA
#define GET_ALPHA		0x8888	// Attention Opcode bidon !!!!!!
#define GET_DELTA		0x8888	// Attention Opcode bidon !!!!!!
#define SAVE_ASSER		0x6000	// Sauvegarder l'état actuel de l'asser
#define REPRISE_ASSER	0x7000	// Reprise de l'état de
#define PAUSE_ASSER		0x8000	// Reprise de l'état de
#define SET_P_ALPHA		0x9000	// Modifier le coefficient P du PID sur ALPHA
#define SET_I_ALPHA		0xA000	// Modifier le coefficient I du PID sur ALPHA
#define SET_D_ALPHA		0xB000	// Modifier le coefficient D du PID sur ALPHA
#define SET_P_DELTA		0xC000	// Modifier le coefficient P du PID sur DELTA
#define SET_I_DELTA		0xD000	// Modifier le coefficient I du PID sur DELTA
#define SET_D_DELTA		0xE000	// Modifier le coefficient D du PID sur DELTA
#define GET_PARAMETERS	0xF000	// Demander les paramÃ¨tres actuels

/* D????finition des r????ponses (id 4 bits + valeur 12 bits) */ 

#define READY			0x0000	// Programme pr????t
#define TEST			0x1000	// Connexion bonne
#define DONE			0x2000	// D????placement termin????
#define	ERROR			0x3000	// Erreur d'????xecution
#define ALPHA			0x4000	// Consigne ALPHA
#define DELTA			0x5000	// Consigne DELTA
#define POSITION_X		0x6000	// Position sur X
#define POSITION_Y		0x7000	// Position sur Y
#define P_ALPHA			0x8000	// Coefficient P du PID sur ALPHA
#define I_ALPHA			0x9000	// Coefficient I du PID sur ALPHA
#define D_ALPHA			0xA000	// Coefficient D du PID sur ALPHA
#define P_DELTA			0xB000	// Coefficient P du PID sur DELTA
#define I_DELTA			0xC000	// Coefficient I du PID sur DELTA
#define D_DELTA			0xD000	// Coefficient D du PID sur DELTA


#endif




//[] END OF FILE
