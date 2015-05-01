/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H


#define BRGVAL 23

/* Functions headers */

void UART_putc(char c);

void __attribute__((__interrupt__)) _U1TXInterrupt(void);
void __attribute__((__interrupt__)) _U1ErrInterrupt(void);

void send_message(long int message);



/*---------------------------------- CODE -----------------------------------*/

#define PING
/*---------------------------------- joystick -------------------------------*/
#define SET_SPEED
#define SET_ROTATION
/*---------------------------------- game (assert) ---------------------------*/

#define START
#define STOP
#define END_OF_GAME
#define SET_DELTA
#define SET_ALPHA




#endif
