/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H

// BRGVAL = 73,7 M / 32 / Baudrate - 1
//#define BRGVAL 23	// 9600 Bauds
#define BRGVAL 1	// 115200 Bauds

/* Functions headers */

void UART_putc(unsigned char c);

void send_message(long int message);



// /*---------------------------------- CODE -----------------------------------*/

// #define PING
// /*---------------------------------- joystick -------------------------------*/
// #define SET_SPEED
// #define SET_ROTATION
// /*---------------------------------- game (assert) ---------------------------*/

// #define START
// #define STOP
// #define END_OF_GAME
// #define SET_DELTA
// #define SET_ALPHA




#endif
