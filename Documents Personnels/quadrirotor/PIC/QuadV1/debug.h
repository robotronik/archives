/* 
 * File:   debug.h
 * Author: jeremy
 *
 * Created on 2 janvier 2013, 16:13
 */

#ifndef DEBUG_H
#define	DEBUG_H

//#define DEBUG_LED_1 LATAbits.LATA2
//#define DEBUG_LED_2 LATAbits.LATA3
#define DEBUG_LED_3 LATAbits.LATA4 // seule led

enum Of { OFF = 0, ON = 1 };
typedef enum Of Of;

//inline void debugLed1(Of o);
//inline void debugLed1Toggle(void);
//inline void debugLed2(Of o);
//inline void debugLed2Toggle(void);
inline void debugLed3(Of o);
inline void debugLed3Toggle(void);

#endif	/* DEBUG_H */

