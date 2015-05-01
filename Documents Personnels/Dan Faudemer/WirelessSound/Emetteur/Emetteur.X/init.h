/* 
 * File:   init.h
 * Author: Dan
 *
 * Created on 17 octobre 2013, 17:25
 */

#ifndef INIT_H
#define	INIT_H

#define CSN LATBbits.LATB3
#define CE LATBbits.LATB4
#define LED_FULL LATDbits.LATD5
#define LED_DEBUG2  LATDbits.LATD6
#define LED_DEBUG3  LATDbits.LATD7

// Oscillator frequency (For __delay_ms)
#define _XTAL_FREQ 8000

void init_clock();
void init_ADC();
void init_timer ();
void init_led();
void init_spi();
#endif	/* INIT_H */

