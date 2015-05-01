/* 
 * File:   adc.h
 * Author: Adrien
 *
 * Created on 14 avril 2013, 11:16
 */

#ifndef ADC_H
#define	ADC_H

extern void initSharp(void);
extern void initTimerSharp();
extern void __attribute__((__interrupt__)) _ADC1Interrupt(void);

#endif	/* ADC_H */

