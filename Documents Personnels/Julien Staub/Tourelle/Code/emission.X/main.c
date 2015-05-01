/* 
 * File:   main.c
 * Author: Matthieu
 *
 * Created on 1 mai 2013, 13:55
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic12f675.h>
#include "init.h"

#define LED_debug GPIObits.GP1


// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

volatile unsigned int compteur_IT =0;

char LED_ON;

/*Led orange sur GP1
 * 
 */
int main(void) {
    TRISIO=0;
   // init_timer0();
   // init_timer1();
   // init_interrupt();
   // GPIObits.GP1=1;
    while(1)
    {
        
       /* if(compteur_IT >= 1200)
        {
            if(LED_ON)
            {
                LED_debug = 1;
                LED_ON=0;
            }
            else
            {
                LED_debug = 0;
                LED_ON = 1;
            }
            compteur_IT = 0;
        }*/
    }
    return (EXIT_SUCCESS);
}

