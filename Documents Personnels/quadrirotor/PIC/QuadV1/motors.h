/* 
 * File:   motors.h
 * Author: jeremy
 *
 * Created on 2 janvier 2013, 16:49
 */

#ifndef MOTORS_H
#define	MOTORS_H

#define MAX_SPEED 3999

void initPWM(void);
void motorsSetSpeedM1(unsigned short m);
void motorsSetSpeedM2(unsigned short m);
void motorsSetSpeedM3(unsigned short m);
void motorsSetSpeedM4(unsigned short m);
void motorsSetSpeed(unsigned short m1,unsigned short m2, unsigned short m3,unsigned short m4);
void motorsStop(void);

#define motorsSetSpeedLeft(v) motorsSetSpeedM4(v); //back
#define motorsSetSpeedFront(v) motorsSetSpeedM1(v);
#define motorsSetSpeedBack(v) motorsSetSpeedM2(v); //right
#define motorsSetSpeedRight(v) motorsSetSpeedM3(v); //left

#endif	/* MOTORS_H */

