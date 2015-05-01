/* 
 * File:   init.h
 * Author: jeremy
 *
 * Created on 2 janvier 2013, 15:52
 */

#ifndef INIT_H
#define	INIT_H
                 
void initOsc();
unsigned char initMPU6050(void);
inline void initPort(void);
inline void initINT(void);
void initPPS(void);

#endif	/* INIT_H */

