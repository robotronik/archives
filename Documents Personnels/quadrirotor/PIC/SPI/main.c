
#include <xc.h>
#include "spi.h"
#include "Mirf.h"

/* Interruption from nrf24l01*/
void __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
{
    IFS1bits.INT1IF = 0;
    LATAbits.LATA4 = ~ LATAbits.LATA4;
    /*uint8_t data[mirf.payload]; // Tableau de byte qui va stocker le message recu

    if (!MIRFisSending() && MIRFdataReady())
    { // Si un message a été recu et qu'un autre n'est pas en cours d'emission);
        MIRFgetData(data); // on récupére le méssage
        //Nop();
        MIRFsend(data); // Et on le renvoi tel quelle (comme pour un ping réseau)

    }*/
    //Nop();
}

void initPPS(void)
{
    RPINR0bits.INT1R = 4; //INT -> RB4

    //Input
    RPINR20bits.SDI1R = 2; //Data input ?> RP2

    //Output to complete ?
    RPOR6bits.RP12R = 0b00111; //Data output -> RP12
    RPOR1bits.RP3R = 0b01000; //Clock output -> RP3


}

int main()
{
    /*TRISA = 0b00001;
    LATA = 0;*/
    TRISA = 0x0;
    LATAbits.LATA2 = 0;
    LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
    TRISB = 0;

    AD1PCFGLbits.PCFG4 = 1; // RB2/AN4 to digital mode
    TRISBbits.TRISB2 = 1; // SPI MI inpout

    initPPS();

    TRISBbits.TRISB4 = 1; //IRQ NRF
   
    INTCON2bits.INT1EP = 1;//front descendant
    IEC1bits.INT1IE = 1;
    IFS1bits.INT1IF = 0;

    initSPI();

    //*/
    MIRFinit();
    mirf.channel = 42; // On va utiliser le canal 0 pour communiquer (128 canaux disponible, de 0 à 127)!
    mirf.payload = sizeof (unsigned char);
    MIRFconfig();
    MIRFsetTADDR((uint8_t *) "telec"); // Le 2eme module va envoyer ses info au 1er module
    MIRFsetRADDR((uint8_t *) "quad1"); // On définit ici l'adresse du 2eme module

    while (1)
    {

        uint8_t data[mirf.payload]; // Tableau de byte qui va stocker le message recu

        if (!MIRFisSending() && MIRFdataReady())
        { // Si un message a été recu et qu'un autre n'est pas en cours d'emission);
            MIRFgetData(data); // on récupére le méssage
            Nop();
            MIRFsend(data); // Et on le renvoi tel quelle (comme pour un ping réseau)
            
        }
        Nop();
    }

    /*/
    uint8_t r = SPItransfer(0xAB);
    //*/



    return 0;
}

