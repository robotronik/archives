/* 
 * File:   asser.h
 * Author: jeremy
 *
 * Created on 2 janvier 2013, 16:51
 */

#ifndef ASSER_H
#define	ASSER_H

#define YAW_P 1.0//5.0
#define YAW_I 0.0
#define YAW_D 0.1
#define YAW_F 1.0 //si 0 plus de D
// 1 0 100
#define PITCH_P 0.//20.0//265.0//534.0//403.8             //20.0//15.0
#define PITCH_I 0.//3.//14.0//28.0//28.4 // pas oublier *0.01 ou /100             //0.0//5.0
#define PITCH_D 300.//4.//398.0//796.0//1265.7// pas oublier /0.01 ou *100                //350.0
#define PITCH_F 0.//10.//7.5//27.9

#define ROLL_P 20.0//265.0//534.0//403.8                 //20.0//15.0
#define ROLL_I 3.//14.0//28.0//28.4                    //0.0//5.0
#define ROLL_D 4.//398.0//796.0//1265.7                   //350.0
#define ROLL_F 10.//7.5//27.9

#define YAW_RATE_P 0.0
#define YAW_RATE_I 0.0
#define YAW_RATE_D 0.0

#define PITCH_RATE_P 0.0 //70.0
#define PITCH_RATE_I 0.0
#define PITCH_RATE_D 0.0

#define ROLL_RATE_P 0.0 //70.0
#define ROLL_RATE_I 0.0
#define ROLL_RATE_D 0.0

#define SATURATION_UPPER 2000.0
#define SATURATION_LOWER -2000.0
/* global var from main ?*/
/*extern int accOffset;
#define OFFSET_ACC_Z(v) (v - accOffset) // pour avoir des valeurs pos et neg de l'acc
*/

/* PID struct */
struct PIDObj {
    float cons; //desired value
    float error; // error
    float prevError; // previous error
    float integ; // integral
    float deriv; // derivative
    float iLimit;
    float kp; // proportional gain
    float ki; // integral gain
    float kd; // derivative gain
    float kf; //filter
};
typedef struct PIDObj PIDObj;

/* global var from MPU */
extern float yrp[3];
//extern int accZworldMPU;

/* global var from com */
extern float yawConsCom;
extern float pitchConsCom;
extern float rollConsCom;
extern float throttleConsCom;

/* PID fcts */
//short computePIDpitch();
//short computePIDroll();
//short computePIDyaw();
float computePID(PIDObj * pid, float measured);
void asserInit();
inline void pidInit(PIDObj* pid, float cons, float kp, float ki, float kd, float kf);
void updateVarAsser(void);
void pidReset(PIDObj* pid);
void pidSetCoeff(PIDObj* pid, float P, float I, float D);

void asser();

#endif	/* ASSER_H */

