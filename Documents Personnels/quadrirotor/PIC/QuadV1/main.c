/* 
 * File:   main.c
 * Author: jeremy
 *
 * Created on 29 décembre 2012, 16:29
 */

#include <stdio.h>
#include <stdlib.h>
#include <p33Fxxxx.h>
#include <stdint.h>
#include <math.h> // to delete ??
#include "init.h"
#include "i2cIMU.h"
#include "i2c.h"
#include "IMU.h"
#include "dmp.h"
#include "constants.h"
#include "debug.h"
#include "asser.h"
#include "motors.h"
#include "spi.h"
#include "Mirf.h"

#include "DEE Emulation 16-bit.h"

/*
 * 
 */

// Bits de configuration
_FOSCSEL(FNOSC_FRCPLL & IESO_ON)
_FOSC(FCKSM_CSECMD & IOL1WAY_ON & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FPOR(ALTI2C_ON) /* I2C on pin ASDA1 & ASCL1 */



volatile Bool mpuInterrupt = FALSE;     // indicates whether MPU interrupt pin has gone high

Quaternion q; // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float yrp[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//int accZworldMPU;
// DEBU a spprG TODO
//float tab[300];
//int cpt = 0;

float yawConsCom = 0.0;
float pitchConsCom =0.0;
float rollConsCom = 0.0;

float throttleConsCom = 0.0;//2000.0;
//float throttleConsComPrec = 0.0;

extern PIDObj pitch;
extern PIDObj roll;

extern short cmdRoll;

//int accOffset;

/* Interruption from Mpu6050*/
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void)
{
    IFS0bits.INT0IF = 0;
    mpuInterrupt = TRUE;
}

//*/
typedef struct T2Q
{
    float gaz;
    float P;
    float I;
    float D;
} T2Q;
typedef struct Q2T
{
    float angleYaw;
    float anglePitch;
    float angleRoll;
    float motorCmd;
} Q2T;
typedef union Payload
{
    T2Q t2q;
    Q2T q2t;
} Payload;
//*/

int main()
{
    unsigned int i, j;
    //int ax,ay,az,gx,gy,gz;
    Bool dmpReady = FALSE; // set true if DMP init was successful
    unsigned char devStatus; // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
    uint8_t mpuIntStatus; // holds actual interrupt status byte from MPU
    uint16_t fifoCount; // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer
    uint8_t nrfStatus;
    
    
    /* Init */
    initOsc();
    initPort();


    //DataEEInit();
    //dataEEFlags.val = 0;


    //if (VUSB_PIN)
     //   while(1);

    InitI2C();
    //*
    devStatus = initMPU6050();
    /*/
    devStatus = 0;
    //*/
    initPWM();
    initINT();
    initPPS();
    initSPI();

    MIRFinit();
    mirf.channel = 42;
    //mirf.payload = sizeof (uint8_t);
    mirf.payload = sizeof (Payload); //TODO
    MIRFconfig();
    MIRFsetTADDR((uint8_t *) "telec");
    MIRFsetRADDR((uint8_t *) "quad1");
    //uint8_t dataNRF; //TODO
    Payload dataNRF;

    //*/
    motorsSetSpeedRight(0); //back
    motorsSetSpeedLeft(0); //right
    motorsSetSpeedFront(0);
    motorsSetSpeedBack(0); //left
    /*/  
    motorsSetSpeedRight(0); //back
    motorsSetSpeedLeft(0); //right
    motorsSetSpeedFront(MAX_SPEED*0.65);
    motorsSetSpeedBack(MAX_SPEED*0.65); //left
    //*/
    
   
#ifdef DEBUG_ASSER
    motorsSetSpeedFront(MAX_SPEED/2);
    motorsSetSpeedBack(MAX_SPEED/2);
    int time = 0;
    Payload toSend;
#endif

    //while (1)
    //    debugLed3Toggle();
    debugLed3(ON);
    //debugLed2(OFF);
    /* Prog security */
    for (i = 0; i < 60000; i++)
    {
        for( j = 0 ; j < 150 ; j++);
    } //TODO bof !
    //debugLed2(ON);
    //motorsSetSpeedM1(2000);
    //motorsSetSpeedM2(3000);
    //while(1); /// debug PWM

    asserInit();

    // make sure it worked (returns 0 if so)
    //*
    if (devStatus == 0)
    {
        //debugLed1(OFF);
        // turn on the DMP, now that it's ready
        MPU_setDMPEnabled(TRUE);

        mpuIntStatus = MPU_getIntStatus();

        // set our DMP Ready flag
        dmpReady = TRUE;

        // get expected DMP packet size for later comparison
        packetSize = MPU_dmpGetFIFOPacketSize();
    }
    else
    {
        //ERROR !!
        //debugLed1(ON);
        while (1);
    }
    /*/
    dmpReady = TRUE;
    packetSize = 0;
    //*/
    nrfStatus = MIRFgetStatus();//TODO

    i = 0;
    
    /* Main loop */
    while (1)
    {
        if (mpuInterrupt == TRUE)
        {
            // reset interrupt flag and get INT_STATUS byte
            mpuInterrupt = FALSE;
            mpuIntStatus = MPU_getIntStatus();

            // get current FIFO count
            fifoCount = MPU_getFIFOCount();

            // check for overflow (this should never happen unless our code is too inefficient)
            if ((mpuIntStatus & 0x10) || fifoCount == 1024)
            {
                // reset so we can continue cleanly
                MPU_resetFIFO();
            }// otherwise, check for DMP data ready interrupt (this should happen frequently)
            else if (mpuIntStatus & 0x02)
            {
                // wait for correct available data length, should be a VERY short wait
                while (fifoCount < packetSize) fifoCount = MPU_getFIFOCount();

                // read a packet from FIFO
                MPU_getFIFOBytes(fifoBuffer, packetSize);

                // track FIFO count here in case there is > 1 packet available
                // (this lets us immediately read more without waiting for an interrupt)
                fifoCount -= packetSize;

                MPU_dmpGetQuaternion(&q, fifoBuffer);
                MPU_dmpGetGravity(&gravity, &q);
                MPU_dmpGetYawPitchRoll(yrp, &q, &gravity);
//                ypr[0] = ypr[0] * TO_DEG; //180/M_PI;
//                ypr[1] = ypr[1] * TO_DEG; //180/M_PI;
//                ypr[2] = ypr[2] * TO_DEG; //180/M_PI;
//                i = 0; // for breakpoint
                //debugLed1Toggle();
#ifdef DEBUG_ASSER
                time++;
                if (time == 500)
                {
                    motorsSetSpeedFront(MAX_SPEED / 2 + MAX_SPEED/10);
                    motorsSetSpeedBack(MAX_SPEED / 2 - MAX_SPEED/10);

                }
                if (time >= 500 && time <=15000)
                {

                    toSend.q2t.angleInt = (int8_t) (yrp[1] * TO_DEG);
                    toSend.q2t.angleDec = (int8_t) (abs(((float) (yrp[1] * TO_DEG) - (float) (toSend.q2t.angleInt))*100));
                    MIRFsend((uint8_t*) & toSend);
                }
                else if (time > 15000)
                {
                    time = 15001;
                    motorsSetSpeedFront(0);
                    motorsSetSpeedBack(0);
                }
#else
                updateVarAsser();
                asser();
                /*
                tab[cpt] = yrp[2];
                cpt++;
                if (cpt >= 300)
                {
                    motorsSetSpeedRight(0); //back
                    motorsSetSpeedLeft(0); //right
                    motorsSetSpeedFront(0);
                    motorsSetSpeedBack(0); //left
                    for (cpt = 0 ; cpt < 300 ; cpt ++)
                    {
                        float buf = tab[cpt]*TO_DEG;
                        dataNRF.q2t.angleDec = (int)((buf*100-(int)buf)*100);
                        dataNRF.q2t.angleInt = (int)(buf);
                        MIRFsend((uint8_t *)&dataNRF);
                    }
                    while (1);
                }*/
                //float buf = yrp[2]*TO_DEG;
                dataNRF.q2t.angleYaw = yrp[0]*TO_DEG;
                dataNRF.q2t.angleRoll = yrp[1]*TO_DEG;
                dataNRF.q2t.anglePitch = yrp[2]*TO_DEG;
                dataNRF.q2t.motorCmd = cmdRoll;
                MIRFsend((uint8_t *)&dataNRF);
#endif

            }
            
        }
        /* Data from NRF */
        
        if (!MIRFisSending() && MIRFdataReady())
        {
            MIRFgetData((uint8_t*)&dataNRF);
            //throttleConsCom = (float)(((unsigned long)dataNRF*MAX_SPEED)/255);
            throttleConsCom = (float)(dataNRF.t2q.gaz);
            pidSetCoeff(&pitch, (float)(dataNRF.t2q.P), (float)(dataNRF.t2q.I), (float)(dataNRF.t2q.D));
            pidSetCoeff(&roll, (float)(dataNRF.t2q.P), (float)(dataNRF.t2q.I), (float)(dataNRF.t2q.D));
            debugLed3Toggle();

            /*float buf = yrp[2]*TO_DEG;
            dataNRF.q2t.angleDec = (int)((buf*100-(int)buf)*100);
            dataNRF.q2t.angleInt = (int)(buf);
            MIRFsend((uint8_t *)&dataNRF);*/
        }
            /*  dataNRF.q2t.angleDec = 42;
            dataNRF.q2t.angleInt = 28;
            MIRFsend((uint8_t *)&dataNRF);*/
        //if (VUSB_PIN)
        //    break;
        
    }


    /*motorsSetSpeedRight(0);
    motorsSetSpeedLeft(0);
    motorsSetSpeedFront(0);
    motorsSetSpeedBack(0);*/
    /* Do not delete, break in previous while(1) */
    motorsStop();
    while (1);

    return 0;
}

