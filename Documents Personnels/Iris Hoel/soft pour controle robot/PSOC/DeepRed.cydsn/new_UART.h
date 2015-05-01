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


/*------------------------------- include -----------------------------------*/


#include <device.h>


/*------------------------------- define ------------------------------------*/


#define DELAY_BEFORE_RETRY				((int)200)
#define NB_RETRY						((unsigned int)2)
#define TIME_CHECK_ASSERT				((unsigned int)500)
#define ACK_TIME_OUT					((int)100)
#define TMP_OUT_BETWEEN_ACK_AND_DATA	((int)1000)

#define ADRESSE_PSOC					((unsigned char)0x01)
#define ADRESSE_ASSER					((unsigned char)0x03)
#define ADRESSE_PC						((unsigned char)0x07)



/*------------------------------- declaration fonctions ---------------------*/


/*-------------------- com uart high level --------------*/

void write_assert(unsigned char addr, unsigned int code, int data);


/*-------------------- com uart low level ---------------*/

void init_com(void);
void close_com(void);
void RX_ASSER_IT_Handler(void);
void RX_XBEE_IT_Handler(void);
void Systick_IT_Handler(void);





/*----------- enable coresponding printf by uncomment -----------------------*/

//#define UART_DEBUG
//#define UART_ERROR
//#define UART_READ_BRUT




#endif
//[] END OF FILE
