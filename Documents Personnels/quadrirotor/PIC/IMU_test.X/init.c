#include "init.h"
#include <p33Fxxxx.h>
#include "IMU.h"
#include "dmp.h"

void initOsc()
{
    // Init de la PLL : 7,37 -> 79,23 MHz !!!
    CLKDIVbits.PLLPRE = 0; // (FRC) /2
    PLLFBD = 41; // (FRC/2) *43
    CLKDIVbits.PLLPOST = 0; // (FRC/2*43) /2

    __builtin_write_OSCCONH((OSCCONH | 1)& 0xF9); // Choix de l'horloge FRC avec PLL
    __builtin_write_OSCCONL(OSCCONL | 1); // Changement d'horloge
    //while (!OSCCONbits.LOCK);      // Attend le bloquage de la PLL (debug)
    while (OSCCONbits.OSWEN); // Attend le changement
}

unsigned char initMPU6050(void)
{
    // initialize device
    MPU_initialize();

    // verify connection
    while (!MPU_testConnection()); // test connection

    // load and configure the DMP
    return MPU_dmpInitialize();
}

inline void initPort(void)
{
    /* all port B = output*/
    TRISB = 0x0;
    /* execpt RB5 & RB6 for I2C */
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    /* And RB7(INT0) for MPU */
    TRISBbits.TRISB7 = 1;
    PORTBbits.RB7 = 0;

    /* Debug LED */
    LATBbits.LATB15 = 0;
}
