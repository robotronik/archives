/*-----------------------------------------------------*
 * Initialisation registre et variables                *
 *-----------------------------------------------------*/





#include "main.h"
#include "motors.h"

void init_osc() {
	// Init de la PLL : 7,37 -> 79,23 MHz !!!
	CLKDIVbits.PLLPRE = 0;		// (FRC) /2
	PLLFBD = 41;				// (FRC/2) *43
	CLKDIVbits.PLLPOST = 0;		// (FRC/2*43) /2
	
	__builtin_write_OSCCONH((OSCCONH | 1 )& 0xF9);	// Choix de l'horloge FRC avec PLL
	__builtin_write_OSCCONL(OSCCONL | 1);			// Changement d'horloge
	//while (!OSCCONbits.LOCK);						// Attend le bloquage de la PLL (debug)
	while (OSCCONbits.OSWEN); 						// Attend le changement
}

void io_init(void)
{
	// "All port pins are defined as inputs after a Reset"

	// Reglage du PORTA
	//_TRISA0 = 1;	// MOTOR_LEFT_CURRENT
	//_TRISA1 = 1;
	//_TRISA2 = 1;	// MOTOR_SF

	// Reglage du PORTB
	//U1MODEbits.UARTEN = 0b0; // UART déconnecté
	//_TRISB2 = 0;
	_TRISB3 = 1;

	_TRISB4 = 0;	// Motor_enable
	_RB4 = 1;
	pause_ms(1);
	_TRISB5 = 0;	// DEL en mode output
	_RB5 = 1;	 	// On eteint la DEL par defaut
	//_TRISB6 = 1;	// Coder right B
	//_TRISB7 = 1;	// Coder right A
	//_TRISB10 = 1;	// Coder left B
	//_TRISB11 = 1;	// Coder left A

	_TRISB12 = 0;	// MOTEUR IN1 right
	_RB12 = 0;		// PWM en IO général si non-utilisé (au moins 2 broches sur 4)
	_TRISB13 = 0;	// MOTEUR IN2 right
	_RB13 = 0;
	_TRISB14 = 0;	// MOTEUR IN1 left
	_RB14 = 0;
	_TRISB15 = 0;	// MOTEUR IN2 left
	_RB15 = 0;
}

/** Initialise le module PWM PWM1 **/
void PWM1_init () {
	//P1TCONbits.PTSIDL = 1;	// Arret en pause

    P1TPER = MAX_SPEED / 2;		// Période du timer
    P1DC1 = 0;                 	// Duty-cycle PWM1H1 = 0%
    P1DC2 = 0;                 	// Duty-cycle PWM1H2 = 0%

    PWM1CON1bits.PMOD1 = 1;     // Sorties PWM1H1 et PWM1L1 indépendantes
    PWM1CON1bits.PMOD2 = 1;     // Sorties PWM1H2 et PWM1L2 indépendantes
    motors_stop();

    P1TCONbits.PTEN = 1;        // Active le Timer des PWMs
    P1TCONbits.PTOPS = ASSER_PERIOD - 1;	// Mis à 0 si modif de P1TCON
	IEC3bits.PWM1IE = 1;		// Autorisation de l'interruption
}

void QEI_init ()
{
    /* Initialise les décodeurs en quadrature QEI1 et QEI2 */

    /* Désactivation QEI avant configuration */
    QEI1CONbits.QEIM   = 0b000;
    QEI2CONbits.QEIM   = 0b000;

    /* Connection des modules QEI1 : LEFT et QEI2 : RIGHT aux pins RPx du PIC */
    RPINR14bits.QEA1R  = 11;             // QEA1 sur RP11
    RPINR14bits.QEB1R  = 10;             // QEB1 sur RP10
    RPINR16bits.QEA2R  = 6;              // QEA2 sur RP6
    RPINR16bits.QEB2R  = 7;              // QEB2 sur RP7

    /* Initialisation QEI1 (roue gauche) */
	QEI1CONbits.QEISIDL = 1;			// Lors de la mise en pause le compteur s'arrete
    QEI1CONbits.SWPAB  = 1;            	// A et B inversés
    DFLT1CONbits.CEID  = 1;             // Pas d'interruption sur erreur
	MAX1CNT = 65535;					// Valeur maximale du compteur

    /* Initialisation QEI2 (roue droite) */
    /* Sensiblement la même chose */
	QEI2CONbits.QEISIDL = 1;
    QEI2CONbits.SWPAB  = 1;
    DFLT2CONbits.CEID  = 1;
	MAX2CNT = 65535;

    /* Reset compteurs */
    POS1CNT = 0;
    POS2CNT = 0;
	
	/* Activation des interruptions sur débordement des compteurs */
	IEC3bits.QEI1IE = 1;
	IEC4bits.QEI2IE = 1;

    /* Activation QEI1 et QEI2 */
    QEI1CONbits.QEIM   = 0b111;          // Mode 4x, reset et interruption sur MAXxCNT
    QEI2CONbits.QEIM   = 0b111;
}

void UART_init()
{
	AD1PCFGLbits.PCFG5 = 1;	// Désactivation de l'entrée analogique !!!
	RPOR1bits.RP2R = 3;							// Tx1 -> RP2
	RPINR18bits.U1RXR = 3;						// RP3 -> Rx1

	//U1MODEbits.LPBACK = 1;	// Debug : Tx1 -> Rx1

	// Low speed : BRG = 79,23 MHz / 32 / Baudrate - 1
	U1MODEbits.BRGH = 1;	// High speed : BRG = 79,23 MHz / 8 / Baudrate - 1
	U1BRG = 85; 			// BAUD Rate Setting for 115200 gives 115160 bauds
	
	//U1STAbits.UTXISEL1 = 1;	// Interrupt on empty FIFO, last byte is being sent
	//U1STAbits.UTXISEL0 = 0;	//                      "

	IFS0bits.U1RXIF = 0; 	// On evite des interruptions à l'activation
	IEC0bits.U1RXIE = 1;	// Activation de l'interruption sur réceptions
	//IFS0bits.U1TXIF = 0;
	//IEC0bits.U1TXIE = 1;	// Activation de l'interruption sur l'envoie
	//IFS4bits.U1EIF = 0;
	//IEC4bits.U1EIE = 1;	// Activation de l'interruption sur erreurs

	U1MODEbits.UARTEN = 1; 	// Enable UART
	U1STAbits.UTXEN = 1; 	// Enable UART TX
}

void init(){
	init_osc();
	__builtin_write_OSCCONL(OSCCON & ~(0x40));	// Débloquage des RPIN et RPOR
    io_init();
    PWM1_init ();
    QEI_init ();
    UART_init();
	__builtin_write_OSCCONL(OSCCON | 0x40);		// Rebloquage des RPIN et RPOR
}
