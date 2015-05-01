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
float ypr[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

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

    /* Init */
    initOsc();
    initPort();
    InitI2C();
    devStatus = initMPU6050();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        debugLed1(OFF);
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
        debugLed1(ON);
        while (1);
    }

    while (1)
    {
        if (PORTBbits.RB7 == 1)
            mpuInterrupt = TRUE;

        while (!mpuInterrupt && fifoCount < packetSize)
        {
            if (PORTBbits.RB7 == 1)
                mpuInterrupt = TRUE;
        }

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
            MPU_dmpGetYawPitchRoll(ypr, &q, &gravity);
            ypr[0] = ypr[0] * TO_DEG;//180/M_PI;
            ypr[1] = ypr[1] * TO_DEG;//180/M_PI;
            ypr[2] = ypr[2] * TO_DEG;//180/M_PI;

            debugLed1Toggle();
        }

    }


    while (1)
    {
        LATBbits.LATB15 = ~LATBbits.LATB15;
        //buf = MPU_getAccelerationZ();

        //MPU_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        for (i = 0; i < 60000; i++)
            for (j = 0; j < 100; j++);
    }

    while (1);

    return 0;
}

