#ifndef MAPPING_H
#define MAPPING_H

// Les TRIS commentés doivent sont les tris utilisés
//#define TRIS_UNUSED_0 _TRISA0
//#define TRIS_UNUSED_1 _TRISA1
#define TRIS_UNUSED_2 _TRISA2
#define TRIS_UNUSED_3 _TRISA3
#define TRIS_UNUSED_4 _TRISA4

// Les TRIS commentés doivent sont les tris utilisés
//#define TRIS_UNUSED_0 _TRISB0
//#define TRIS_UNUSED_1 _TRISB1
//#define TRIS_UNUSED_2 _TRISB2
//#define TRIS_UNUSED_3 _TRISB3
//#define TRIS_UNUSED_4 _TRISB4
//#define TRIS_UNUSED_5 _TRISB5
//#define TRIS_UNUSED_6 _TRISB6
//#define TRIS_UNUSED_7 _TRISB7
//#define TRIS_UNUSED_8 _TRISB8
//#define TRIS_UNUSED_9 _TRISB9
//#define TRIS_UNUSED_10 _TRISB10
//#define TRIS_UNUSED_11 _TRISB11
//#define TRIS_UNUSED_12 _TRISB12
//#define TRIS_UNUSED_13 _TRISB13
#define TRIS_UNUSED_14 _TRISB14
//#define TRIS_UNUSED_15 _TRISB15

//-----------------------------------

// Pickit
#define TRIS_PGD _TRISB0
#define TRIS_PGC _TRISB1

// Sharp
#define SHARP1 _RA0
#define SHARP2 _RA1
#define TRIS_SHARP_AVANT   _TRISA0
#define TRIS_SHARP_ARRIERE _TRISA1

// Bumper
#define BUMPER_DROITE _RB10
#define BUMPER_GAUCHE _RB12
#define TRIS_BUMPER_DROITE _TRISB10
#define TRIS_BUMPER_GAUCHE _TRISB12

// DELs
#define DEL_DEBUG_1  _RB4
#define DEL_DEBUG_2  _RB5
#define DEL_DEBUG_3  _RB6
#define TRIS_DEBUG_1 _TRISB4
#define TRIS_DEBUG_2 _TRISB5
#define TRIS_DEBUG_3 _TRISB6

// Cervo
#define TRIS_SERVO_1 _TRISB9
#define TRIS_SERVO_2 _TRISB11
#define TRIS_SERVO_3 _TRISB13
#define TRIS_SERVO_4 _TRISB15

// UART assert
#define TX_ASSER _RB2
#define RX_ASSER _RB3
#define TRIS_TX_ASSER _TRISB2
#define TRIS_RX_ASSER _TRISB3

// UART xbee
#define RX_XBEE      _RB7
#define TX_XBEE      _RB8
#define TRIS_RX_XBEE _TRISB7
#define TRIS_TX_XBEE _TRISB8

// Pwd
#define SERVO1 _RB9
#define SERVO2 _RB11
#define SERVO3 _RB13
#define SERVO4 _RB15
#define SERVO                P1DC1

// Evitement
// \todo verifier qu’il sert à qqch
#define TMR_EVITEMMENT      TMR1
#define TCON_EVITEMMENT     T1CON
#define TCONbits_EVITEMMENT T1CONbits
#define PR_EVITEMMENT       PR1
#define IF_EVITEMMENT       IFS0bits.T1IF
#define IE_EVITEMMENT       IEC0bits.T1IE
#define IP_EVITEMMENT       IPC0bits.T1IP

// Timer 2 (non utilisé)
#define _2TMR_UNUSED        TMR2
#define _2TCON_UNUSED       T2CON
#define _2TCONbits_UNUSED   T2CONbits
#define _2PR_UNUSED         PR2
#define _2IF_UNUSED         IFS0bits.T2IF
#define _2IE_UNUSED         IEC0bits.T2IE
#define _2IP_UNUSED         IPC0bits.T2IP

// Timer sharp
#define TMR_SHARP           TMR3
#define TCON_SHARP          T3CON
#define TCONbits_SHARP      T3CONbits
#define PR_SHARP            PR3
#define IF_SHARP            IFS0bits.T3IF
#define IE_SHARP            IEC0bits.T3IE
#define IP_SHARP            IPC0bits.T3IP

// Timer sur 32 bits - utilise les timers 4 et 5 pour calculer les 90s de la coupe
#define TMR_MATCH_A          TMR4
#define TCON_MATCH_A         T4CON
#define TCONbits_MATCH_A     T4CONbits
#define PR_MATCH_A           PR4
#define IF_MATCH_A           IFS0bits.T4IF
#define IE_MATCH_A           IEC0bits.T4IE
#define IP_MATCH_A           IPC0bits.T4IP
// La partie B du timer 32 bits
#define TMR_MATCH_B          TMR5
#define TCON_MATCH_B         T5CON
#define TCONbits_MATCH_B     T5CONbits
#define PR_MATCH_B           PR5
#define IF_MATCH_B           IFS1bits.T5IF
#define IE_MATCH_B           IEC1bits.T5IE
#define IP_MATCH_B           IPC7bits.T5IP


#endif /* MAPPING_H */
