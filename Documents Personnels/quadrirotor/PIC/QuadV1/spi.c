#include <xc.h>
#include "spi.h"
//#include <spi.h>


void initSPI ()
{
    //SPI Configuration
// Clear the Interrupt Flag
    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 0;


    //Configuration du SPI
    SPI1CON1bits.DISSCK = 0; //Internal Clock
    SPI1CON1bits.DISSDO = 0; //SDO pin used
    SPI1CON1bits.MODE16 = 0; //8 bits transmision

    SPI1CON1bits.SSEN = 0; //Slave pin not used //0 si comme arduino
    //Data clock
    
    //Clock Edge 
    SPI1CON1bits.CKE =0; //Send data on clk up
    SPI1CON1bits.CKP = 0; //Idle clock = low level

    SPI1STATbits.SPIROV = 0;

    SPI1CON1bits.SMP = 0;
    SPI1CON1bits.MSTEN = 1; //Master mode


    //Clock prescaler
    //SPI1CON1bits.SPRE = 0b111; //1:1
    //SPI1CON1bits.PPRE = 0b10;//4:1



    //Activation de SPI
    SPI1STATbits.SPIEN = 1;

    //IFS0bits.SPI1IF = 0;
    //IEC0bits.SPI1IE = 1;
}

uint8_t SPItransfer(uint8_t byte)
{
    //uint8_t buf;// = SPI1BUF;
    SPI1BUF = byte;
    while (!SPI1STATbits.SPIRBF);
    //while(!DataRdySPI1());
    //buf = SPI1BUF;
    return SPI1BUF;
}

//Interrupt fonction
/*void __attribute__((__interrupt__)) _SPI1Interrupt(void)
{
    IFS0bits.SPI1IF = 0;
}*/

