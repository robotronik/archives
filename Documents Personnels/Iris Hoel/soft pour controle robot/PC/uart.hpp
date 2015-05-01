
#ifndef _UART_
#define _UART_


/*------------------------------- include -----------------------------------*/


#include "main.hpp"


/*------------------------------- define ------------------------------------*/


#define DELAY_BEFORE_RETRY				((int)100)
#define NB_RETRY						((unsigned int)6)
#define TIME_CHECK_ASSERT				((unsigned int)300)
#define ACK_TIME_OUT					((int)200)
#define TMP_OUT_BETWEEN_ACK_AND_DATA	((int)300)
#define DELAY_UART_LOOP					((unsigned int)8)

#define ADRESSE_PSOC					((unsigned char)0x01)
#define ADRESSE_ASSER					((unsigned char)0x03)
#define ADRESSE_PC						((unsigned char)0x07)


#define CONF_UART_9600_BAUD				1
#define CONF_UART_115200_BAUD			2


/*------------------------------- declaration fonctions ---------------------*/


/*-------------------- com uart high level --------------*/

void write_assert(unsigned char addr, unsigned int code, int data);


/*-------------------- com uart low level ---------------*/

OVERLAPPED initCom();
HANDLE link(int conf);
void WriteABuffer(char *lpBuf, DWORD dwToWrite, HANDLE hComm, OVERLAPPED *osReader);
void readUart(HANDLE hComm, OVERLAPPED *osReader);






/*----------- enable coresponding printf by uncomment -----------------------*/

#define UART_DEBUG
#define UART_ERROR
//#define UART_READ_BRUT


#endif
