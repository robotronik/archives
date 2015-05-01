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
#ifndef COM_XBEE_H
#define COM_XBEE_H

#define CODE_ROBOT2_TIRETTE 0b0000
#define CODE_ROBOT2_XY 0b0001
#define CODE_ROBOT2_COULEUR 0b0010
#define CODE_FIN 0b0011
#define CODE_ROBOT2_GET_XY 0b0100

void UART_XBee_RX_Handler(void);


struct TrCouleur
{
    unsigned char
    couleur : 1,
    stub : 3,
    codet : 4;
};
typedef struct TrCouleur TrCouleur;

struct TrDefault
{
    unsigned char
    stub : 4,
    codet : 4;
};
typedef struct TrDefault TrDefault;

union TrXbee
{
    TrCouleur couleur;
    TrDefault d;
    char c;
};
typedef union TrXbee TrXbee;

void sendRobot2XY(int x, int y);
void sendXbeePos();
void sendCouleurRobot2();
void sendTiretteRobot2();
void sendXBee( unsigned short message);
void sendRobot2Fin();

#endif

//[] END OF FILE
