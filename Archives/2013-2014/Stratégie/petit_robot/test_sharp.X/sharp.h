/* 
 * File:   adc.h
 * Author: Adrien
 *
 * Created on 14 avril 2013, 11:16
 */

#ifndef SHARP_H
#define SHARP_H

#define DISTANCE_PROXIMITE 	  	500
#define DISTANCE_ARRET_URGENCE 	1500

void initSharp(void);
void initTimerSharp();
void __attribute__((__interrupt__)) _ADC1Interrupt(void);

#endif /* SHARP_H */

