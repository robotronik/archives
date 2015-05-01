/* 
 * File:   interrupt.h
 * Author: julien
 *
 * Created on 1 mai 2013, 17:29
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H

#ifdef	__cplusplus
extern "C" {
#endif

//void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
void __attribute__((interrupt, auto_psv)) _DMA0Interrupt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPT_H */

