#include "asser.h"
#include "constants.h"
#include "motors.h"
#include "nRF24L01.h"
#include <xc.h>

#include "DEE Emulation 16-bit.h"
#include "debug.h"

// TODO : pid.c/h
//unsigned int angleD[10] __attribute__((space(auto_psv))) = {0x0000,0x1111,0x2222,0x3333,0x4444,0x5555,
//0x6666,0x7777,0x8888,0x9999};
//int cptAngle = 0;

PIDObj yaw;
PIDObj pitch;
PIDObj roll;
PIDObj yawRate;
PIDObj pitchRate;
PIDObj rollRate;

float yawMeasured;
float pitchMeasured;
float rollMeasured;
float yawRateMeasured;
float pitchRateMeasured;
float rollRateMeasured;

/*float yaw;
float yawPre = 0;
float pitch;
float pitchPre = 0;
float roll;
float rollPre = 0;

float yawError = 0;
float pitchError = 0;
float rollError = 0;
float yawErrorPre = 0;
float pitchErrorPre = 0;
float rollErrorPre = 0;*/

//int accZworld; // recentré en ~5000
//int accZworldPre; //ulss

//int vitZworld = 0;
//int vitZworldPre = 0;

/* Consigne */
/*float yawCons = 0;
float pitchCons = 0;
float rollCons = 0;*/


unsigned short throttleCons = 0; //100;
//int vitZWorldCons = 0;


/* Coeff */
/*
float Ppitch = 8;
float Ipitch = 0.1;
float Dpitch = 0;

float Proll = 8;
float Iroll = 0.1;
float Droll = 0;

float Pyaw = 5;
float Iyaw = 0.1;
float Dyaw = 0; //possibly useless according to Internet
 */
//unsigned int Pthrottle = 100;
//unsigned int Ithrottle = 10;
//unsigned int Dthrottle = 20;

/* Cmd */

short cmdPitch;
short cmdRoll;
short cmdYaw;
short cmdThrottle;

//short cmdEq = 0; // commande des moteurs pour maintenir l'équilibre, >> 0 ??

/* Vit moteurs */
short motorRight;
short motorLeft;
short motorFront;
short motorBack;

/*float computePID(PIDObj * pid, float measured)
{
    float output;

    pid->error = pid->cons - measured;
    pid->integ = (pid->error + pid->prevError) / 2 * T_ECH;
    if (pid->integ > pid->iLimit)
    {
        pid->integ = pid->iLimit;
    }
    else if (pid->integ < -pid->iLimit)
    {
        pid->integ = -pid->iLimit;
    }
    pid->deriv = (pid->error - pid->prevError);

    output = (pid->kp * pid->error) +
            (pid->ki * pid->integ) +
            (pid->kd * pid->deriv);

    pid->prevError = pid->error;

    if (output < -2000)
    {
        output = -2000;
    }
    else if (output > 2000)
    {
        output = 2000;
    }
    return output;
}*/

float computePID(PIDObj * pid, float measured)
{
    float output;

    pid->error = pid->cons - measured;
    //pid->integ = (pid->error + pid->prevError) / 2 * T_ECH * pid->ki;
    pid->integ += pid->error * T_ECH;
    if (pid->integ > pid->iLimit)
    {
        pid->integ = pid->iLimit;
    }
    else if (pid->integ < -pid->iLimit)
    {
        pid->integ = -pid->iLimit;
    }
//    pid->deriv = pid->kd * pid->error * pid->kf /
//          ( 1 + pid->kf*(pid->error+pid->prevError)/2*T_ECH);// TODO à vérifier
    pid->deriv = (pid->error- pid->prevError);


    output = (pid->kp * pid->error) + pid->ki *(pid->integ)
                     + pid->kd *(pid->deriv);

    pid->prevError = pid->error;

    if (output < -2000)
    {
        output = -2000;
    }
    else if (output > 2000)
    {
        output = 2000;
    }
    return output;
}


//float computePID(PIDObj * pid, float measured)
//{
///* local block i/o variables */
//     float Deriv;
//    float Integrator = 0;
//    static float filter_state = 0;
//    float output;
//    static float integState= 0;
//    pid->error = pid->cons - measured;
//
//    /* DiscreteIntegrator: '<S1>/Integrator' */
//    Integrator = 0.005 * pid->ki * pid->error + integState;
//
//    /* Gain: '<S1>/Filter Coefficient' incorporates:
//     *  DiscreteIntegrator: '<S1>/Filter'
//     *  Gain: '<S1>/Derivative Gain'
//     *  Inport: '<Root>/u'
//     *  Sum: '<S1>/SumD'
//     */
//    Deriv = (pid->kd * pid->error - filter_state) * pid->kf;
//
//    /* Sum: '<S1>/Sum' incorporates:
//     *  Gain: '<S1>/Proportional Gain'
//     *  Inport: '<Root>/u'
//     */
//    output = pid->kp * pid->error + Integrator + Deriv;
//
//    /* Saturate: '<S1>/Saturation' */
//    if (output >= SATURATION_UPPER)
//    {
//        /* Outport: '<Root>/y' */
//        output = SATURATION_UPPER;
//    }
//    else if (output <= SATURATION_LOWER)
//    {
//        /* Outport: '<Root>/y' */
//        output = SATURATION_LOWER;
//    }
//
//    /* End of Saturate: '<S1>/Saturation' */
//
//    /* Update for DiscreteIntegrator: '<S1>/Integrator' */
//    integState = 0.005 * pid->ki * pid->error + Integrator;
//
//    /* Update for DiscreteIntegrator: '<S1>/Filter' */
//    filter_state += 0.01 * Deriv;
//    return output;
//}


void updateVarAsser(void)
{
    //    vitZworldPre = vitZworld;
    //    accZworld = OFFSET_ACC_Z(accZworldMPU);
    //    vitZworld += accZworld; // integration acc -> vit (Z)
    static short throttleConsPrev = 0;
    static float yawMeasuredPrev = 0;
    static float pitchMeasuredPrev = 0;
    static float rollMeasuredPrev = 0;

    /* YPR and Throttle values from comm */
    yaw.cons = yawConsCom;
    pitch.cons = pitchConsCom;
    roll.cons = rollConsCom;

    throttleCons = (short)throttleConsCom;
    /* throttle ramp */ //TODO : use a #define for throttle acceleration
    if (throttleCons > throttleConsPrev)
    {
        if ((throttleCons - throttleConsPrev) > 10)
            throttleCons = throttleConsPrev + 10;
    }
    throttleConsPrev = throttleCons;

    yawMeasured = yrp[0] * TO_DEG;
    rollMeasured = yrp[1] * TO_DEG;
    pitchMeasured = yrp[2] * TO_DEG;



    yawRateMeasured = yawMeasured - yawMeasuredPrev;
    pitchRateMeasured = pitchMeasured - pitchMeasuredPrev;
    rollRateMeasured = rollMeasured - rollMeasuredPrev;
    yawMeasuredPrev = yawMeasured;
    pitchMeasuredPrev = pitchMeasured;
    rollMeasuredPrev = rollMeasured;


    /*
        yawError = yawCons - yaw;
        pitchError = pitchCons - pitch;
        rollError = rollCons - roll;

        yawPre = yaw;
        pitchPre = pitch;
        rollPre = roll;

        yawErrorPre = yawError;
        pitchErrorPre = pitchError;
        rollErrorPre = rollError;*/

}

void asserInit()
{
    pidInit(&yaw, yawConsCom, YAW_P, YAW_I, YAW_D, YAW_F);
    pidInit(&pitch, pitchConsCom, PITCH_P, PITCH_I, PITCH_D, PITCH_F);
    pidInit(&roll, rollConsCom, ROLL_P, ROLL_I, ROLL_D, ROLL_F);
    //pidInit(&yawRate, 0.0, YAW_RATE_P, YAW_RATE_I, YAW_RATE_D, 0.0);
   // pidInit(&pitchRate, 0.0, PITCH_RATE_P, PITCH_RATE_I, PITCH_RATE_D);
    //pidInit(&rollRate, 0.0, ROLL_RATE_P, ROLL_RATE_I, ROLL_RATE_D);

}

inline void pidInit(PIDObj* pid, float cons, float kp, float ki, float kd, float kf)
{
    pid->cons = cons;
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->kf = kf;
    pid->deriv = 0;
    pid->error = 0;
    pid->integ = 0;
    pid->prevError = 0;
    pid->iLimit = MAX_SPEED / 2; // TODO : au pif a test, & set function if needed
}

void asser() // a chaque coup d'asser, updateVarAsser() should be call before
{
    
    /* PID test with rate, TODO */
    //    pitchRate.cons= computePID(&pitch, pitchMeasured);
    //    cmdPitch = (short)computePID(&pitchRate, pitchRateMeasured);
    //    rollRate.cons= computePID(&roll, rollMeasured);
    //    cmdRoll = (short)computePID(&rollRate, rollRateMeasured);

    cmdPitch = (short) computePID(&pitch, pitchMeasured);
    cmdRoll = (short) computePID(&roll, rollMeasured);
    //cmdPitch = 0;
    //cmdRoll = 0;
    //cmdYaw = computePID(&yaw,yawMeasured);
    /*cmdPitch = computePIDpitch();
    cmdRoll = computePIDroll();
    //cmdYaw = computePIDyaw();*/

    cmdYaw = 0;

    //cmdYaw = (short) computePID(&yaw, yawMeasured);

    cmdThrottle = throttleCons;
    //cmdThrottle = 1500;

    if (cmdThrottle)
    {
        motorRight = (cmdYaw + cmdRoll) + cmdThrottle; // cmd real = + angle corrections
        motorLeft = (cmdYaw - cmdRoll) + cmdThrottle;
        motorFront = (cmdPitch - cmdYaw) + cmdThrottle;
        motorBack = (-cmdPitch - cmdYaw) + cmdThrottle;
    }
    else
    {
        motorRight = 0;
        motorLeft = 0;
        motorFront = 0;
        motorBack = 0;
    }
        /*
         static int DEE_cnt=0;
         DEE_cnt ++;
        if (DEE_cnt == 259)
            DataEEWrite(0x8004,0);
        if (DEE_cnt >= 260)
        {
            motorRight = 1000;
            motorLeft = 2000;
            motorFront = 1500;
            motorBack = 1500;
            unsigned int rollData;
            if (rollMeasured>=0)
                rollData = (unsigned int)(rollMeasured*100);
            else
                rollData = (unsigned int)(-rollMeasured*100) | 0x8000;

            if (DataEEWrite(rollData,DEE_cnt-259)) //error (tables pleines)
                debugLed3(OFF);
        }
        if (DEE_cnt>300)
        {
            motorRight = 0;
            motorLeft = 0;
            motorFront = 0;
            motorBack = 0;
            DEE_cnt = 1000;
        }*/

    /* security, should rarely happen (en fait si, à faible throttle)*/
    motorRight = U(motorRight);
    motorLeft = U(motorLeft);
    motorFront = U(motorFront);
    motorBack = U(motorBack);
    /*motorRight = 0;
    motorLeft = 0;
    motorFront = 0;
    motorBack = 0;*/


    motorsSetSpeedRight(motorRight);
    motorsSetSpeedLeft(motorLeft);
    motorsSetSpeedFront(motorFront);
    motorsSetSpeedBack(motorBack);

    //unsigned int page;
    //unsigned int offset;
    //unsigned long addr;
//    if (cptAngle < 10)
//    {
//        //angleD[cptAngle] = rollMeasured;
//        asm("DISI #0x3FFF");
//        page = __builtin_tblpage((const void *)(angleD));
//        offset = __builtin_tbloffset((const void *)(angleD));
//        TBLPAG = page;
//        __builtin_tblwtl(offset,0x4242);
//        __builtin_write_NVM();
//        while(NVMCONbits.WR);
//        asm("DISI #0x0000");
//        cptAngle++;
//    }

}

void pidReset(PIDObj* pid)
{
    pid->error = 0;
    pid->prevError = 0;
    pid->integ = 0;
    pid->deriv = 0;
}

void pidSetCoeff(PIDObj* pid, float P, float I, float D)
{
    pid->kp = P;
    pid->ki = I;
    pid->kd = D;
}
