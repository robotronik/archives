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


#ifndef _NEW_UART_
#define _NEW_UART_


#define ERROR_TIME_OUT      -1


void write_assert(unsigned int code, int data);
void read_assert(unsigned int code, int *data);
void RX_IT_handler(void);



#endif
//[] END OF FILE
