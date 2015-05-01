/* 
 * File:   main.c
 * Author: julien
 *
 * Created on 28 avril 2013, 13:27
 *
 * projet compilé avec XC16 option large array
 */

#include <stdio.h>
#include <stdlib.h>
#include "fir.h"
#include "init.h"
#include "adc.h"
#include "position.h"
#include <p33EP512MU810.h>


// DSPIC33EP512MU810 Configuration Bit Settings

#include <xc.h>

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code-Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF               // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC              // Initial Oscillator Source Selection Bits (Internal Fast RC (FRC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

 //FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler Bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // Power-on Reset Timer Value Select bits (128ms)
#pragma config BOREN = ON               // Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = OFF            // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = OFF            // Alternate I2C pins for I2C2 (SDA2/SCK2 pins are selected as the I/O pins for I2C2)

// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGEC2 and PGED2)
#pragma config RSTPRI = PF              // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = ON             // JTAG Enable bit (JTAG is disabled)

// FAS
#pragma config AWRP = OFF               // Auxiliary Segment Write-protect bit (Auxiliary program memory is not write-protected)
#pragma config APL = OFF                // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF               // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)



/*
 * 
 */

   // Select Internal FRC at POR
//_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure Primary Oscillator in XT mode
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
//pas de watchdog
//_FWDT(FWDTEN_OFF);
//ymemory et xmemory pour eventuelle amélioration asm (ex : MAC 1 cycle)
//le tableau des coefs du filtre
int coef_fir[3][NB_COEF_FIR]   __attribute__((space(ymemory),eds))  //fech=28.19kHz  //700,1600,2500
={
    {-16,-66,-101,-110,-86,-27,57,144,205,209,138,-2,-178,-335,-410,-357,-170,113,411,621,656,477,114,-336,-732,-935,-857,-494,62,646,1069,1183,930,368,-338,-971,-1326,-1279,-830,-108,667,1256,1475,1256,667,-108,-830,-1279,-1326,-971,-338,368,930,1183,1069,646,62,-494,-857,-935,-732,-336,114,477,656,621,411,113,-170,-357,-410,-335,-178,-2,138,209,205,144,57,-27,-86,-110,-101,-66,-16},
    {94,99,105,112,118,123,125,123,115,99,73,37,-10,-69,-138,-217,-303,-394,-486,-576,-659,-730,-787,-824,-837,-825,-786,-717,-620,-495,-346,-177,9,205,406,604,793,967,1118,1242,1334,1390,1409,1390,1334,1242,1118,967,793,604,406,205,9,-177,-346,-495,-620,-717,-786,-825,-837,-824,-787,-730,-659,-576,-486,-394,-303,-217,-138,-69,-10,37,73,99,115,123,125,123,118,112,105,99,94},
    {-73,-47,-13,26,70,114,154,180,186,163,107,18,-97,-225,-348,-444,-492,-475,-384,-219,5,262,519,737,878,912,824,614,300,-82,-482,-846,-1121,-1263,-1245,-1063,-735,-300,185,655,1046,1305,1395,1305,1046,655,185,-300,-735,-1063,-1245,-1263,-1121,-846,-482,-82,300,614,824,912,878,737,519,262,5,-219,-384,-475,-492,-444,-348,-225,-97,18,107,163,186,180,154,114,70,26,-13,-47,-73}
};
//le tableau des lectures ADC //reverse laisse des 0 à la fin de l'adresse de tab pour que le ADC puisse construire l'adresse
int  __attribute__((space(xmemory),eds,reverse(16384))) tab_adc_brut[NB_RECEIVER][NB_COEF_FIR + NB_COEF_PERIODE];


long min[32][3];
long max[32][3];
unsigned long amplitude[3][32];

long out;
unsigned int x, y;

int i, j;
short led_on=0;

int main(int argc, char** argv)
{

    init_osc();
    init_port();
    CORCONbits.IF=1; //on travaille avec des entiers pas des fractions pour le DSP
    init_adc();
    //init_timer3();
    init_dma0(); //va générer la première aquisition

//    for(i=0;i<NB_RECEIVER;i++)
//        for(j=0;j<NB_COEF_FIR+NB_COEF_PERIODE;j++)
//        {
//            tab_adc_brut[i][j]=0;
//        }

    while(1)
    {
        PORTGbits.RG8 = led_on;  //LED debug
        led_on^=1;
    

        
        
        int num_chanel,num_diode,start_coef,num_filtre;
        for(num_diode=0; num_diode < NB_RECEIVER ; num_diode ++)
        {
            for(num_filtre = 0 ; num_filtre<3 ; num_filtre++)
            {
                min[num_diode][num_filtre] = 0;
                max[num_diode][num_filtre] = 0;
            }
        }

        //calcul de l'amplitude de la reception sur chaque filtre et chaque recepteur
        for(start_coef = 0 ;start_coef < NB_COEF_PERIODE ; start_coef++)
        {
            for(num_diode=0 ; num_diode < NB_RECEIVER ; num_diode ++)
            {
                num_chanel = diode2chanel(num_diode);
                for(num_filtre = 0 ; num_filtre<3 ; num_filtre++) 
                {
                    out = asmFIR(coef_fir[num_filtre] , tab_adc_brut[num_chanel] , start_coef,NB_COEF_FIR - 2); //à changer
                    if(out > max[num_diode][num_filtre])
                    {
                        max[num_diode][num_filtre] = out;
                    }
                    else if(out < min[num_diode][num_filtre])
                    {
                        min[num_diode][num_filtre] = out;
                    }
                }
            }
        }
        for(num_diode=0; num_diode < NB_RECEIVER ; num_diode ++)
        {
            for(num_filtre = 0 ; num_filtre<3 ; num_filtre++)
            {
                amplitude[num_filtre][num_diode] = max[num_diode][num_filtre] - min[num_diode][num_filtre];
            }
        }

        int a = rcpt2angle(amplitude[0]);
        int b = rcpt2angle(amplitude[1]);
        int c = rcpt2angle(amplitude[2]);


        angle2pos(a,b,c,&x,&y);

        get_adc(); //redémare une série de sample
    }


    return (EXIT_SUCCESS);
}

