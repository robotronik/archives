/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



#ifndef _UART_H
#define _UART_H


//#define BRGVAL 23 /* for 9600 baud rate, see uart ref manual */
#define BRGVAL 1  /* for 115200 baud rate, see uart ref manual */




/*------------------------------- define ------------------------------------*/

#define ADRESSE_PSOC					((unsigned char)0x01)
#define ADRESSE_ASSER					((unsigned char)0x03)
#define ADRESSE_PC						((unsigned char)0x07)


/* code */
#define MOTOR                 			((unsigned int)19)








/*------------------------------- declaration fonctions ---------------------*/


/*-------------------- com uart high level --------------*/

int write_assert(unsigned char addr, unsigned long int code, long int data);


/*-------------------- com medium level functions -------*/

void send_message(long int message);


/*-------------------- com uart low level ---------------*/

void UART_putc(char c);
void __attribute__((__interrupt__)) _U1TXInterrupt(void);
void __attribute__((__interrupt__)) _U1ErrInterrupt(void);


/*----------- enable coresponding printf by uncomment -----------------------*/

#define UART_DEBUG
#define UART_ERROR
#define UART_READ_BRUT
#define DEBUG_HIGH_LEVEL




#endif
