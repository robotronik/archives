/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/


#include "main.h"
#include <stdio.h>

/*------------------------------------ variables and define -----------------*/

//#define DEBUG
int rx_buffer[4];
int rx_buffer_indice = 0;
extern T_dividedLong distL, distR;			// Position de chaque roue pour un trajet

#define MODE_IDLE					0x01
#define MODE_CODE		      		0x02
#define MODE_DATA_RECEIVER			0x04



/*------------------------------------ functions ----------------------------*/


static void RX_IT_handler(void);
static void uart_error(void);
static long int answer(unsigned long int code);
static void process(unsigned long int code, long int data);



/*------------------------------------ low level functions ------------------*/

void UART_putc(char c)
{
	while(U1STAbits.UTXBF) {}		// Attend si le buffer est plein
	U1TXREG = c;
}

void send_message(long int message) {
	#ifdef DEBUG
	printf("Assert send : %lx\n", message);
	#endif
	UART_putc((char) (message >> 24));
	UART_putc((char) (message >> 16));
	UART_putc((char) (message >> 8));
	UART_putc((char) message);
}


void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void)
{
	IFS0bits.U1RXIF = 0; // On s'acquitte de l'interruption

	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U1STAbits.OERR == 1)
	{
		U1STAbits.OERR = 0;
		return ;
	}

	/* get the data */
	if(U1STAbits.URXDA == 1)
	{
		rx_buffer[rx_buffer_indice] = U1RXREG;
		rx_buffer_indice = (rx_buffer_indice + 1)%4;
		if(rx_buffer_indice == 0)
		{
			RX_IT_handler();				
		}
//		rx_buffer_indice = (rx_buffer_indice + 1) %3;
//		RX_IT_handler(); 
	}
}

void __attribute__((interrupt, auto_psv)) _U1ErrInterrupt(void)
{
	IFS4bits.U1EIF = 0; // On s'acquitte de l'interruption
	if(U1STAbits.FERR == 1)	// Erreurs ?
		return ;
	/* must clear the overrun error to keep uart receiving */
	if(U1STAbits.OERR == 1)
		U1STAbits.OERR = 0;
	//error();

}

/*------------------------------------ Medium level functions ---------------*/

static void RX_IT_handler(void)
{
	long int data = 0;
	static unsigned long int code = 0;
	static unsigned char uart_mode = MODE_IDLE;

	data = (((unsigned long int)rx_buffer[0])<<24)
			| (((unsigned long int)rx_buffer[1])<<16)
			| (((unsigned long int)rx_buffer[2])<<8)
			| ((unsigned long int)rx_buffer[3]);
	#ifdef DEBUG
	printf("Assert receive : %lx\n", data);
	#endif
	if(data == 0x0FFFFFFF)
	{
		send_message(0x0FFFFFFF);
		uart_mode = MODE_IDLE;
		return;
	}
	else if (data == 0x7FFFFFFF)
	{
		uart_mode = MODE_CODE;
		send_message(0x7FFFFFFF);
		return;	
	}
	switch(uart_mode)
	{
		case MODE_CODE:
			send_message(0x3FFFFFFF);
			if(data < 0)
			{
				send_message(answer(-data));
				uart_mode = MODE_IDLE;
			}
			else
			{
				code = data;
				uart_mode = MODE_DATA_RECEIVER;
			}
			break;
		case MODE_DATA_RECEIVER:
			send_message(0x1FFFFFFF);
			process(code, data);
			uart_mode = MODE_IDLE;
			break;
		default:
			uart_error();
			break;
	}
}

static void uart_error(void)
{
	//printf("Assert uart error\n");
	/* block it by staying in until unsychronise bytes are lost */
	char trash = U1RXREG;
	rx_buffer_indice = 0;

	//pause_ms(20);
}


/*------------------------------------ Hight level functions ----------------*/



static long int answer(unsigned long int code)
{
	long int ret;
	//pause_ms(100);
	//printf("Assert answer : %lx\n", code);
	switch (code)
	{
	case 12:
		ret = -motors_get_qei(LEFT);
		motors_reset_qei(LEFT);
		return ret;
	case 13:
		ret = -motors_get_qei(RIGHT);
		motors_reset_qei(RIGHT);
		return ret;
	case 14:
		return distL.all;
	case 15:
		return distR.all;
	}

	return 0;	
}

static void process(unsigned long int code, long int data)
{
	static long int Vg = 0;
	static long int Vd = 0;

	//pause_ms(100);
	if(code == 19)
	{
		motors_set_speed(LEFT, (int)(data >>16));
		motors_set_speed(RIGHT, (int)data);
	}
	else if(code == 12)
	{
		motors_set_speed(RIGHT,data);
		motors_set_speed(LEFT, data);
	}
	else if(code == 13)
	{
		motors_set_speed(RIGHT, data);
		motors_set_speed(LEFT, -data);		
	}
	else if(code == 14)
	{
		motors_set_speed(RIGHT, -data);
		motors_set_speed(LEFT, data);		
	}
	else if(code == 15)
    	motors_stop();
    else if(code == 16)
    	Vg = data;
    else if(code == 17)
    	Vd = data;
    else if(code == 18)
    {
		motors_set_speed(RIGHT, Vd);
		motors_set_speed(LEFT, Vg);		
    }


	//printf("Assert process : %lx,%lx\n", code, data);
}
