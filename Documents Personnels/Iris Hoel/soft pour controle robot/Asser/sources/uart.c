/*---------------------------------------------------------------------------*
 * Couche basse uart                                                         *
 *---------------------------------------------------------------------------*/



/*------------------------------------ include ------------------------------*/


#include "main.h"
#include <stdio.h>

/*------------------------------------ variables and define -----------------*/

//#define DEBUG
unsigned char rx_buffer[5];
int rx_buffer_indice = 0;

static int codeAck = 0;
static int dataAck = 0;
static int modeData = 0;
static int writeStep = 0;
unsigned long int codeRead = 0;
long int valueRead = 0;

int Vg = 0;
int Vd = 0;


/*------------------------------------ functions ----------------------------*/


static void RX_end_of_packet_handler(void);
static void clearRX(void);
static long int answer(unsigned long int code);
static void process(unsigned long int code, long int data);
static int checkCode(unsigned int code, unsigned char *addr);


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

void send(unsigned char addr, long int data)
{
    #ifdef UART_DEBUG
    printf("Asser send     -> %2x%3x%3x%3x%3x\n",(char) addr,(char) (data >> 24),(char) (data >> 16),
                                                    (char) (data >> 8),(char) data );
    #endif
	UART_putc((char) addr);
	UART_putc((char) (data >> 24));
	UART_putc((char) (data >> 16));
	UART_putc((char) (data >> 8));
	UART_putc((char) data);
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
		rx_buffer_indice = (rx_buffer_indice + 1)%5;
		if(rx_buffer_indice == 0)
		{
			RX_end_of_packet_handler();				
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



int write_assert(unsigned char addr, unsigned long int code, long int data)
{
    if(modeData == 0)
    {
        //clearRX();	
        if(!writeStep)
        {
            clearRX();
        	send(addr, code);
        	writeStep = 1;
        	return 0;
    	}
    	else if(writeStep && codeAck)
    	{
        	send(addr, data);
        	codeAck = 0;
      		return 0;		
    	}
    	else if(dataAck)
    	{
    		dataAck = 0;
    		writeStep = 0;
    		return 1;
    	}
    	return 0;
    }
    else
    {
        #ifdef UART_DEBUG
        printf("Asser write ignored\n");
        #endif        
        return 0;
    }
}



static void RX_end_of_packet_handler(void)
{
    #ifdef UART_DEBUG
    printf("Asser received <- %2x%3x%3x%3x%3x\n", rx_buffer[0],rx_buffer[1],rx_buffer[2],rx_buffer[3],rx_buffer[4]);
    #endif

    static unsigned char addr = 0;
    unsigned long int data = 0;

	switch(rx_buffer[0])
    {
        case ADRESSE_PSOC:
            return;
        case ADRESSE_PC:
            return;
        case ADRESSE_ASSER:
            data = (((unsigned long int)rx_buffer[1]) << 24)
                    | (((unsigned long int)rx_buffer[2]) << 16)
                    | (((unsigned long int)rx_buffer[3]) << 8)
                    | ((unsigned long int)rx_buffer[4]);
            if(data == 0x7FFFFFFF)
            {
                if(codeAck == 1)
                {
                    #ifdef UART_ERROR
                    printf("Asser overflow error\n");
                    #endif
                    clearRX();
                }
                else
                    codeAck = 1;
            }
            else if(data == 0x3FFFFFFF)
            {
                if(dataAck == 1)
                {
                    #ifdef UART_ERROR
                    printf("Asser overflow error\n");
                    #endif
                    clearRX();
                }
                else
                    dataAck = 1;
            }
            else
            {
                if(!modeData)
                {
                    if(checkCode(data, &addr))
                    {
                        codeRead = data;
                        modeData = 1;                    
                        send(addr, 0x7FFFFFFF);
                        // timeOutBetweenCodeAndData = SDL_AddTimer(TMP_OUT_BETWEEN_ACK_AND_DATA,
                        //                                 timeOutBetweenCodeAndDataHandler, 0);
                    }
                    else
                    {
                        #ifdef UART_ERROR
                        printf("Asser code read error = %lx\n", data);
                        #endif
                        clearRX();
                    }
                }
                else
                {
                    valueRead = (long int)data;
                    modeData = 0;
                    send(addr, 0x3FFFFFFF);
                    process(codeRead, valueRead);
                }
            }
            break;
        default:
            #ifdef UART_ERROR
            printf("Asser addr error = %3x\n", rx_buffer[0]);
            #endif
            clearRX();
            return;
    }
}



static void clearRX(void)
{
	//printf("Assert uart error\n");
	/* block it by staying in until unsychronise bytes are lost */
    int i = 0;
    for(i = 0; i<10 ; i++)
    {
	   char trash = U1RXREG;
	}
    rx_buffer_indice = 0;
    modeData = 0;
    codeAck = 0;
    dataAck = 0;
    writeStep = 0;
	//pause_ms(20);
}



static int checkCode(unsigned int code, unsigned char *addr)
{
    if(code == MOTOR)
    {
        *addr = ADRESSE_PC;
        return 1;
    }
    return 0;
}


/*------------------------------------ Hight level functions ----------------*/



static long int answer(unsigned long int code)
{
	//pause_ms(100);
	//printf("Assert answer : %lx\n", code);
	if(code == 12)
	{
		long int ret = -motors_get_qei(LEFT);
		motors_reset_qei(LEFT);
		return ret;
	}
	else if(code == 13)
	{	
		long int ret = -motors_get_qei(RIGHT);
		motors_reset_qei(RIGHT);
		return ret;
	}

	return 0;	
}

static void process(unsigned long int code, long int data)
{
	//pause_ms(100);
	if(code == 19)
	{
        int Vg_tmp = (int)(data >> 16);
        int Vd_tmp = (int)data;
        if(Vg_tmp >4000 || Vg_tmp < -4000)
        {
            #ifdef DEBUG_HIGH_LEVEL
            printf("Asser erreur consigne moteur = %d\n", Vg_tmp);
            #endif
		}
        else if(Vd_tmp > 4000 || Vd_tmp < -4000)
        {
            #ifdef DEBUG_HIGH_LEVEL
            printf("Asser erreur consigne moteur = %d\n", Vd_tmp);
            #endif
        }
        else
        {
            Vg = Vg_tmp;
            Vd = Vd_tmp;
        }
	}
	//printf("Assert process : %lx,%lx\n", code, data);
}


