/* 
 * File:   dmp.h
 * Author: jeremy
 *
 * Created on 30 décembre 2012, 19:52
 */

#ifndef DMP_H
#define	DMP_H

#include <stdint.h>
#include "constants.h"
#include "mathHelper.h"

#ifdef DEBUG // import arduino
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTF(x, y) Serial.print(x, y)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTLNF(x, y) Serial.println(x, y)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTF(x, y)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTLNF(x, y)
#endif

// special methods for MotionApps 2.0 implementation

uint8_t MPU_dmpInitialize();
Bool MPU_dmpPacketAvailable();

uint8_t MPU_dmpSetFIFORate(uint8_t fifoRate);
uint8_t MPU_dmpGetFIFORate();
uint8_t MPU_dmpGetSampleStepSizeMS();
uint8_t MPU_dmpGetSampleFrequency();
int32_t MPU_dmpDecodeTemperature(int8_t tempReg);

// Register callbacks after a packet of FIFO data is processed
//uint8_t dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
//uint8_t dmpUnregisterFIFORateProcess(inv_obj_func func);
uint8_t MPU_dmpRunFIFORateProcesses();

// Setup FIFO for various output
uint8_t MPU_dmpSendQuaternion(uint_fast16_t accuracy);
uint8_t MPU_dmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendPacketNumber(uint_fast16_t accuracy);
uint8_t MPU_dmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
uint8_t MPU_dmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);

// Get Fixed Point data from FIFO
// defaut parametre uint8_t* packet
uint8_t MPU_dmpGetAccel32(int32_t *data, const uint8_t* packet);
uint8_t MPU_dmpGetAccel16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetAccelV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpGetQuaternion32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetQuaternion16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetQuaternion(Quaternion *q, const uint8_t* packet );
uint8_t MPU_dmpGet6AxisQuaternion32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGet6AxisQuaternion16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGet6AxisQuaternionQ(Quaternion *q, const uint8_t* packet );
uint8_t MPU_dmpGetRelativeQuaternion32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetRelativeQuaternion16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetRelativeQuaternionQ(Quaternion *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyro32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyro16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyroV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpSetLinearAccelFilterCoefficient(float coef);
uint8_t MPU_dmpGetLinearAccel32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetLinearAccel16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetLinearAccelV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpGetLinearAccelVV16(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity);
uint8_t MPU_dmpGetLinearAccelInWorld32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetLinearAccelInWorld16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetLinearAccelInWorldV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpGetLinearAccelInWorldVV16(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q);
uint8_t MPU_dmpGetGyroAndAccelSensor32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyroAndAccelSensor16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyroAndAccelSensorV16(VectorInt16 *g, VectorInt16 *a, const uint8_t* packet );
uint8_t MPU_dmpGetGyroSensor32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyroSensor16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetGyroSensorV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpGetControlData(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetTemperature(int32_t *data, const uint8_t* packet );
//uint8_t MPU_dmpGetGravity(int32_t *data, const uint8_t* packet = 0);
//uint8_t MPU_dmpGetGravity(int16_t *data, const uint8_t* packet = 0);
//uint8_t MPU_dmpGetGravity(VectorInt16 *v, const uint8_t* packet = 0);
uint8_t MPU_dmpGetGravity(VectorFloat *v, Quaternion *q);
uint8_t MPU_dmpGetUnquantizedAccel32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetUnquantizedAccel16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetUnquantizedAccelV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpGetQuantizedAccel32(int32_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetQuantizedAccel16(int16_t *data, const uint8_t* packet );
uint8_t MPU_dmpGetQuantizedAccelV16(VectorInt16 *v, const uint8_t* packet );
uint8_t MPU_dmpGetExternalSensorData(int32_t *data, uint16_t size, const uint8_t* packet );
uint8_t MPU_dmpGetEIS(int32_t *data, const uint8_t* packet );

uint8_t MPU_dmpGetEuler(float *data, Quaternion *q);
uint8_t MPU_dmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity);

// Get Floating Point data from FIFO
uint8_t MPU_dmpGetAccelFloat(float *data, const uint8_t* packet );
uint8_t MPU_dmpGetQuaternionFloat(float *data, const uint8_t* packet );

uint8_t MPU_dmpProcessFIFOPacket(const unsigned char *dmpData);
//uint8_t MPU_dmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed = NULL);
uint8_t MPU_dmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed);

uint8_t MPU_dmpSetFIFOProcessedCallback(void (*func) (void));

uint8_t MPU_dmpInitFIFOParam();
uint8_t MPU_dmpCloseFIFO();
uint8_t MPU_dmpSetGyroDataSource(uint8_t source);
uint8_t MPU_dmpDecodeQuantizedAccel();
uint32_t MPU_dmpGetGyroSumOfSquare();
uint32_t MPU_dmpGetAccelSumOfSquare();
void MPU_dmpOverrideQuaternion(long *q);
uint16_t MPU_dmpGetFIFOPacketSize();


#endif	/* DMP_H */

