/*-----------------------------------------------------*
 * Initialisation registre et variables                *
 *-----------------------------------------------------*/





#include "main.h"

extern long alpha;             // Position (alpha, delta)
extern long delta;
extern long consigne_alpha;    // Consigne (alpha, delta)
extern long consigne_delta;
extern long commande_alpha;    // Commande (alpha, delta)
extern long commande_alpha_old;
extern long commande_delta;
extern long commande_delta_old;
extern long commande_l;        // Commande converties sur chaque roue
extern long commande_r;
extern long l;                 // Vitesse de chaque roue
extern long r;
extern PID pid_alpha; // Structure contenant l'état, à un instant t, du correcteur PID
extern PID pid_delta;
extern int stop_asser;



void init(){
    init_parametre();
    PWM1_init ();
    QEI_init ();
    io_init();
    UART_init();
    timer_init();
}

void init_parametre(){
    PID_init(&pid_alpha,0,0,0); // nb ki divisé par 10 pour système stable
    PID_init(&pid_delta,0,0,0);
    motors_reset_qei(LEFT);
    motors_reset_qei(RIGHT);
    alpha = 0;             // Position (alpha, delta)
    delta = 0;
    consigne_alpha = 0;    // Consigne (alpha, delta)
    consigne_delta = 0;
    commande_alpha = 0;    // Commande (alpha, delta)
    commande_alpha_old = 0;
    commande_delta = 0;
    commande_delta_old = 0;
    commande_l = 0;        // Commande converties sur chaque roue
    commande_r = 0;
    l = 0;                 // Vitesse de chaque roue
    r = 0;
    stop_asser = 1;
}

void PID_init (PID *pid, long Kp, long Ki, long Kd)
{
/*
Initialise la structure contenant l'etat d'un correcteur PID

Paramètres :
- pid        : pointeur vers la structure contenant l'état du correcteur PID
- Kp, Ki, Kd : coefficients proportionnels, intégral et dérivée du correcteur
*/
    pid->err_1 = 0;
    pid->err_0 = 0;
    pid->derr  = 0;
    pid->ierr  = 0;

    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}


void PWM1_init () {
  /* Initialise le module PWM PWM1 */
    P1TCONbits.PTEN = 0;         // Désactive PWM 1

    P1TCONbits.PTOPS  = 0b0000;  // No postscale
    P1TCONbits.PTCKPS = 0b00;    // No prescale

    P1TCONbits.PTMOD1 = 0;       // Free running mode
    P1TCONbits.PTMOD0 = 0;       // (PTMODE = 0b00)
	//P1TCONbits.PTSIDL = 1;		 // Arret en pause

    /*
    La fréquence est codée "en dur" car c'est la bonne fréquence
    à utiliser : le pont en H utilisé ne supporte pas de fréquence
    plus élevée, et un sifflement désagréable est produit pour
    une fréquence plus basse.
    */
    P1TPER = MAX_SPEED/2;           // Le sifflement a disparu 
    //P1TPER = 2000 - 1;           // 10kHz avec 80MHz clk
    P1DC1  = 0;                  // Duty-cycle PWM1H1 0%
    P1DC2  = 0;                  // Duty-cycle PWM1H2 0%

    PWM1CON1bits.PMOD1 = 1;      // Sorties PWM1H1 et PWM1L1 indépendantes
    PWM1CON1bits.PMOD2 = 1;      // Sorties PWM1H2 et PWM1L2 indépendantes
    motors_stop();

    P1TCONbits.PTEN = 1;         // Active PWM 1
}

void QEI_init ()
{
    /* Initialise les décodeurs en quadrature QEI QEI1 et QEI2 */

    /* Désactivation QEI avant configuration */
    QEI1CONbits.QEIM   = 0b000;
    QEI2CONbits.QEIM   = 0b000;

    /* Désactivation de AN2 à  AN5 (utilisent les mêmes pins que QEI) */
// Antho dit ok :
/*    AD1PCFGLbits.PCFG2 = 1;
    AD1PCFGLbits.PCFG3 = 1;
    AD1PCFGLbits.PCFG4 = 1;
    AD1PCFGLbits.PCFG5 = 1;
*/
    /* Connection des modules QEI1 : LEFT et QEI2 : RIGHT aux pins RPx du PIC */
    RPINR14bits.QEA1R  = 11;              // QEA1 sur RP11
    RPINR14bits.QEB1R  = 10;               // QEB1 sur RP2
    RPINR16bits.QEA2R  = 7;              // QEA2 sur RP3
    RPINR16bits.QEB2R  = 6;              // QEB2 sur RP4

    /* Configuration de RP1 à RP4 comme étant des entrées */
    /* C'est une exception dans le dsPIC : TRISB est prioritaire
     sur la configuration de RPx quand RPx est configuré
     en entrée
     Mis à 1 par défault, mais c'est pour être bien sûr !*/
 //   TRISB |= 0x001E;

    /* Initialisation QEI1 (roue gauche) */
    QEI1CONbits.UPDN   = 1;              // Comptage positif
    QEI1CONbits.SWPAB  = 0;              // A et B non inversés
    QEI1CONbits.PCDOUT = 0;              // Pas de sortie UPDNx
    QEI1CONbits.POSRES = 0;              // Index non utilisé

    DFLT1CONbits.CEID  = 0;              // Pas d'interruption sur erreur
    DFLT1CONbits.QEOUT = 0;              // Pas de filtrage des entrées (trop lent !)
    DFLT1CONbits.QECK  = 0;              // Inutilisé - concerne le filtrage

    MAX1CNT = 65535;                     // Valeur max pour registre 16 bits

    /* Initialisation QEI2 (roue droite) */
    /* Sensiblement la même chose */
    QEI2CONbits.UPDN   = 1;
    QEI2CONbits.SWPAB  = 0;
    QEI2CONbits.PCDOUT = 0;
    QEI2CONbits.POSRES = 0;

    DFLT2CONbits.CEID  = 0;
    DFLT2CONbits.QEOUT = 0;
    DFLT2CONbits.QECK  = 0;

    MAX2CNT = 65535;

    /* Reset compteurs */
    POS1CNT = 0;
    POS2CNT = 0;

    /* Activation QEI1 et QEI2 */
    QEI1CONbits.QEIM   = 0b111;          // Mode 4x, reset par MAXxCNT
    QEI2CONbits.QEIM   = 0b111;

	//QEI1_resetPOS();
	//QEI2_resetPOS();
}


void io_init(void)
{
	// Reglage du PORTA
	_TRISA0 = 1;	// MOTOR_LEFT_CURRENT
	_TRISA1 = 1;
	_TRISA2 = 1;	// MOTOR_SF

	// Reglage du PORTB
	
	U1MODEbits.UARTEN = 0b0; // Test UART
	_TRISB2 = 0;
	_TRISB3 = 1;

	_TRISB4 = 0;	// Motor_enable
		_RB4 = 1;
	_TRISB5 = 0;	// DEL en mode output
		_RB5 = 1; // On eteint la DEL par defaut*/
	_TRISB6 = 1;	// Coder right B
	_TRISB7 = 1;	// Coder right A
	_TRISB10 = 1;	// Coder left B
	_TRISB11 = 1;	// Coder left A

	_TRISB12 = 0;	// IN1 right
		_RB12 = 0;
	_TRISB13 = 0;	// IN2 right
		_RB13 = 0;
	_TRISB14 = 0;	// IN1 left
		_RB14 = 0;
	_TRISB15 = 0;	// IN2 left
		_RB15 = 0;
	//LATB |= 0b0000111111111111;*/
}

void UART_init(void)
{
//	TRISBbits.TRISB3 = 1;	// "All port pins are defined as inputs after a Reset"
	AD1PCFGLbits.PCFG5 = 1;	// Désactivation de l'entrée analogique !!!
	__builtin_write_OSCCONL(OSCCON & ~(0x40));	// Débloquage des RPOR
	RPOR1bits.RP2R = 3;							// Tx1 -> RP2
	RPINR18bits.U1RXR = 3;						// RP3 -> Rx1
	__builtin_write_OSCCONL(OSCCON | 0x40);		// Rebloquage des RPOR

	U1MODEbits.PDSEL = 0; // No Parity, 8 data bits
	U1MODEbits.STSEL = 0; // 1 Stop bit
	U1MODEbits.ABAUD = 0; // Autobaud Disabled
	U1MODEbits.BRGH = 0; // Low Speed mode
	//U1MODEbits.LPBACK = 1;	// Tx1 -> Rx1

	U1STAbits.URXISEL = 0b00;	// Interruption a chaque caractere recu

	U1BRG = BRGVAL; // BAUD Rate Setting for 9600

	U1MODEbits.UARTEN = 1; // Enable UART
	U1STAbits.UTXEN = 1; // Enable UART TX

	IFS0bits.U1RXIF = 0; // On sort de l'interruption
	IEC0bits.U1RXIE = 1;// Activation de l'interruption sur réceptions
	IFS4bits.U1EIF = 0;
	IEC4bits.U1EIE = 1;	// Activation de l'interruption sur erreurs
}


void timer_init(void)
{
/*
Initialisation du timer et de l'interruption générée tout les 10ms
L'interruption en question lance le calcul de l'asservissement tout les deltaT :
activer le compteur
x
lors de la mise en pause le compteur s'arrete
x
d'ou vien l'horloge 1
préscalaire
suite présca
x
x
d'ou vien l'horloge 2
x */


// T1CON = 10100000000
}

