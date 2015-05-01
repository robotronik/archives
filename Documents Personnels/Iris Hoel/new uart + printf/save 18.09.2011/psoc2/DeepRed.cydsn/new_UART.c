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



#include <device.h>

//#define DEBUG



static void error_handler(void);
static void send_preambule(void);
static void send_break(void);
static void send_code(int code);
static void send_data(int value);
static void RX_end_of_packet_handler(void);
static assert_send(int data);


static unsigned int time_out = 0x0005FFFF;
//static unsigned int time_out = 0xFFFFFFFF;

static int assert_uart_last_value = 0;


static int preambule_ack = 0;
static int code_ack = 0;
static int data_ack = 0;
static int value_ready = 0;
static int break_ack = 0;
static int wait_value = 0;

/* --------------------------------- Hight level com function ----------------*/
/* Com protocol between assert and psoc chip, time out error implementation */



void write_assert(unsigned int code, int data)
{
    unsigned int time = 0;

    send_preambule();
    while(!preambule_ack)
    {
        time ++;
        if(time == time_out)
        {
            error_handler();
            write_assert(code, data);
            return;
        }
    }
    preambule_ack = 0;

    send_code(code);
    time = 0;
    while(!code_ack)
    {
        time ++;
        if(time == time_out)
        {
            error_handler();
            write_assert(code, data);
            return;
        }
    }
    code_ack = 0;

    send_data(data);
    time = 0;
    while(!data_ack)
    {
        time ++;
        if(time == time_out)
        {
            error_handler();
            write_assert(code, data);
            return;
        }
    }
    data_ack = 0;
}

void read_assert(unsigned int code, int* data)
{
    unsigned int time = 0;

    send_preambule();
    while(!preambule_ack)
    {
        time ++;
        if(time == time_out)
        {
            error_handler();
            read_assert(code, data);
            return;
        }
    }
    preambule_ack = 0;

    send_code(-code);
    time = 0;
    while(!code_ack)
    {
        time ++;
        if(time == time_out)
        {
            error_handler();
            read_assert(code, data);
            return;
        }
    }
    code_ack = 0;

    wait_value = 1;
    time = 0;
    while(!value_ready)
    {
        time ++;
        if(time == time_out)
        {
            error_handler();
            read_assert(code, data);
            return;
        }
    }
    value_ready = 0;

	*data = assert_uart_last_value;
    wait_value = 0;
}

static void error_handler(void)
{
    UART_Asser_ClearRxBuffer();
    UART_Asser_ClearTxBuffer();
    preambule_ack = 0;
    code_ack = 0;
    data_ack = 0;
    value_ready = 0;
    break_ack = 0;

    send_break();
    unsigned int time = 0;
    while(!break_ack)
    {
        time ++;
        if(time == time_out)
        {
            printf("assert didn't respond, execution abording\n");
            break_ack = 1;
            while(1);
        }
    }
    break_ack = 0;

    UART_Asser_ClearRxBuffer();
    UART_Asser_ClearTxBuffer();
    preambule_ack = 0;
    code_ack = 0;
    data_ack = 0;
    value_ready = 0;
}



static void send_preambule(void)
{
    assert_send(0x7FFFFFFF);
}
static void send_break(void)
{
    assert_send(0x0FFFFFFF);
}
static void send_code(int code)
{
    assert_send(code);
}
static void send_data(int value)
{
    assert_send(value);
}


static void RX_end_of_packet_handler(void)
{
	#ifdef DEBUG
	printf("PSOC receive : %x\n", assert_uart_last_value);
    #endif
	if(wait_value)
    {
        value_ready = 1;
        return;
    }
    switch(assert_uart_last_value)
    {
        case 0x7FFFFFFF:
            preambule_ack = 1;
            break;
        case 0x3FFFFFFF:
            code_ack = 1;
            break;
        case 0x1FFFFFFF:
            data_ack = 1;
            break;
        case 0x0FFFFFFF:
            break_ack = 1;
            break;
		case 0x11111111:
			// retour d'erreur de l'assert
			break;
        default:
            //error_handler();
            break;
    }
}


/* --------------------------------- Medium level com function ----------------*/

/* unsigned value to fix */
static assert_send(int data)
{
	#ifdef DEBUG
    printf("PSOC send : %x\n", data);
	#endif
	UART_Asser_PutChar((char) (data >> 24));
	UART_Asser_PutChar((char) (data >> 16));
	UART_Asser_PutChar((char) (data >> 8));
	UART_Asser_PutChar((char) (data));
}

int let_printf_mode = 0;
void RX_IT_handler(void)
{
    static int let_printf_mode = 0;
    static int end_of_printf = 0;
    if(let_printf_mode)
    {
        char data = UART_Asser_GetChar();
        if(data == 0x0D)
            end_of_printf = 1;
        else if(end_of_printf)
        {
            if(data == 0x0A)
            {
                let_printf_mode = 0;
            }
            end_of_printf == 0;
        }
        UART_XBee_PutChar(data);
    }
    else if(UART_Asser_GetRxBufferSize() >= 4)
    {
        char data[] = {0,};
		data[0] = UART_Asser_GetChar();
		data[1] = UART_Asser_GetChar();
		data[2] = UART_Asser_GetChar();
		data[3] = UART_Asser_GetChar();

        if(data[0] == 0x41 && data[1] == 0x73 && data[2] == 0x73 && data[3] == 0x65)
        {
            /* it's a printf from assert */
            let_printf_mode = 1;
			UART_XBee_PutArray(data, 4);
        }
        else
        {
            assert_uart_last_value = (((unsigned int)data[0]) << 24)
									| (((unsigned int)data[1]) << 16)
									| (((unsigned int)data[2]) << 8)
									| ((unsigned int)data[3]); 
            RX_end_of_packet_handler();
        }
    }
}



