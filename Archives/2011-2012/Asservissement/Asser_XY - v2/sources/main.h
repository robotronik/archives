#ifndef _MAIN_H
#define _MAIN_H

#include "Boolean.h"

extern volatile boolean doitAttendre;		// Echantilloneur

// Structure associant 2 shorts dans un long
typedef union {
	long all;
	struct {
		short low;
		short high;
	} part;
} T_dividedLong;

typedef union {
	unsigned long all;
	struct {
		short low;
		unsigned short high;
	} part;
} T_dividedULong;

#if !defined(COLLECTEUR) && !defined(SIMU)

#include <p33FJ128MC802.h>

#define mulSS	__builtin_mulss					// long=short*short, s=signed u=unsigned
#define mulSU	__builtin_mulsu
#define mulUU	__builtin_muluu
#define divS	__builtin_divsd					// short=long/short
#define divU	__builtin_divud

#else	// #if !defined(COLLECTEUR) && !defined(SIMU)

#define mulSS(a, b)	((long) (a)*(b))
#define mulSU(a, b)	((long) (a)*(b))
#define mulUU(a, b)	((unsigned long) (a)*(b))
#define divS(a, b)	((a)/(b))
#define divU(a, b)	((a)/(b))

#endif	// #if !defined(COLLECTEUR) && !defined(SIMU)
 
 #endif
