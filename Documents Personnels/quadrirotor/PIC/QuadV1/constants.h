/* 
 * File:   constants.h
 * Author: jeremy
 *
 * Created on 30 décembre 2012, 10:22
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H


enum Bool
{
    FALSE = 0, TRUE = 1
};
typedef enum Bool Bool;

#define M_PI   3.14159265358979323846
#define TO_DEG 57.29577951308232

#define T_ECH 0.01 //100HZ

#define pgm_read_byte(p) (*(const unsigned char *)(p)) // read a byte in the program memory
//#define abs(v) (v < 0 ? -v : v) //defined in stdlib.h

#define U(v) (v > 0 ? v : 0) //Heaviside :)

//#define VUSB_PIN PORTBbits.RB8


#endif	/* CONSTANTS_H */

