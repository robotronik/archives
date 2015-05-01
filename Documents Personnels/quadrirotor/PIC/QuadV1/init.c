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
    //TODO : mettre les range et bandwidth comme sur BC
}

inline void initPort(void)
{
    /* all port B = output*/
    TRISB = 0x0;
    LATB = 0x0;
    /* execpt RB5 & RB6 for I2C */
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;
    /* And RB7(INT0) for MPU */
    TRISBbits.TRISB7 = 1;
    PORTBbits.RB7 = 0;
    /* And RB8 for Vusb */
    //TRISBbits.TRISB8 = 1;


    /* MISO input SPI */
    //AD1PCFGLbits.PCFG4 = 1; // RB2/AN4 to digital mode
    TRISBbits.TRISB12 = 1; // SPI MI inpout
    TRISBbits.TRISB3 = 0; //CSN_PIN
    TRISBbits.TRISB2 = 0; //CE_PIN

    /* Debug LED */
    TRISA = 0x0;
    //LATAbits.LATA2 = 0;
    //LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
}

inline void initINT(void)
{
    IEC0bits.INT0IE = 1;
    IFS0bits.INT0IF = 0;
}

void initPPS(void)
{
    //Input
    RPINR20bits.SDI1R = 12; //Data input ?> RP12

    //Output to complete ?
    RPOR5bits.RP10R = 0b00111; //Data output -> RP10
    RPOR2bits.RP4R = 0b01000; //Clock output -> RP3
}
