/* 
 * File:   debug.h
 * Author: jeremy
 *
 * Created on 2 janvier 2013, 16:13
 */

#ifndef DEBUG_H
#define	DEBUG_H

#define DEBUG_LED_1 LATBbits.LATB15

enum Of { OFF = 0, ON = 1 };
typedef enum Of Of;

inline void debugLed1(Of o);
inline void debugLed1Toggle(void);


#endif	/* DEBUG_H */

