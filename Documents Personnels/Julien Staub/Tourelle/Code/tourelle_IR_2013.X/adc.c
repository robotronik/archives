#include "adc.h"
#include "fir.h"
#include <p33EP512MU810.h>


volatile int tableau_plein;

void get_adc(void)  //4.54ms pour 4096 mesures  -> 28.19 kHz/rcpt
{
    tableau_plein = 0;
    DMA0CONbits.CHEN = 1; //redémarer l'acq de l'adc par le DMA
    while(!tableau_plein) //mis a 1 par interrupt sur DMA1
    {
    }

}

int diode2chanel(int num_diode)
{
    int res;
    switch (num_diode)
    {
    case 0 :
        res= 6;
        break;
    case 1 :
        res=0;
        break;
    case 2 :
        res=1;
        break;
    case 3 :
        res=2;
        break;
    case 4 :
        res=3;
        break;
    case 5 :
        res=4;
        break;
    case 6 :
        res=5;
        break;
    case 7 :
        res=21;
        break;
    case 8 :
        res=20;
        break;
    case 9 :
        res=29;
        break;
    case 10 :
        res=18;
        break;
    case 11 :
        res=17;
        break;
    case 12 :
        res=16;
        break;
    case 13 :
        res=31;
        break;
    case 14 :
        res=30;
        break;
    case 15 :
        res=29;
        break;
    case 16 :
        res=28;
        break;
    case 17 :
        res=27;
        break;
    case 18 :
        res=26;
        break;
    case 19 :
        res=25;
        break;
    case 20 :
        res=24;
        break;
    case 21 :
        res=23;
        break;
    case 22 :
        res=22;
        break;
    case 23 :
        res=15;
        break;
    case 24 :
        res=14;
        break;
    case 25 :
        res=13;
        break;
    case 26 :
        res=12;
        break;
    case 27 :
        res=11;
        break;
    case 28 :
        res=10;
        break;
    case 29 :
        res=9;
        break;
    case 30 :
        res=8;
        break;
    case 31 :
        res=7;
        break;
    }
    return res;
}
