#include "dmp.h"
#include "constants.h"
#include "i2cIMU.h"
#include "IMU.h"
#include "dataDmp.h"
#include <math.h>
#include <string.h>
/**
 * Import Arduino MPU6050_6AXIS_MOTIONAPPS20
 * 
 */

/* variables de class */
static uint8_t *dmpPacketBuffer;
static uint16_t dmpPacketSize;

inline void delay(unsigned long t)
{
    //#ifndef DELAY_CALL
    unsigned long cy_delay;
    for (cy_delay = 0; cy_delay < t * 2000; cy_delay++);
}

uint8_t MPU_dmpInitialize()
{
    // reset device
    DEBUG_PRINTLN(F("\n\nResetting MPU6050..."));
    MPU_reset();
    delay(30); // wait after reset

    // enable sleep mode and wake cycle
    /*Serial.println(F("Enabling sleep mode..."));
    setSleepEnabled(true);
    Serial.println(F("Enabling wake cycle..."));
    setWakeCycleEnabled(true);*/

    // disable sleep mode
    DEBUG_PRINTLN(F("Disabling sleep mode..."));
    MPU_setSleepEnabled(FALSE);

    // get MPU hardware revision
    DEBUG_PRINTLN(F("Selecting user bank 16..."));
    MPU_setMemoryBank(0x10, TRUE, TRUE);
    DEBUG_PRINTLN(F("Selecting memory byte 6..."));
    MPU_setMemoryStartAddress(0x06);
    DEBUG_PRINTLN(F("Checking hardware revision..."));
#ifdef DEBUG
    uint8_t hwRevision = MPU_readMemoryByte();
    delay(hwRevision);
#endif
    DEBUG_PRINT(F("Revision @ user[16][6] = "));
    DEBUG_PRINTLNF(hwRevision, HEX);
    DEBUG_PRINTLN(F("Resetting memory bank selection to 0..."));
    MPU_setMemoryBank(0, FALSE, FALSE);

    // check OTP bank valid
    DEBUG_PRINTLN(F("Reading OTP bank valid flag..."));
#ifdef DEBUG
    uint8_t otpValid = MPU_getOTPBankValid();
    delay(otpValid);
#endif
    DEBUG_PRINT(F("OTP bank is "));
    DEBUG_PRINTLN(otpValid ? F("valid!") : F("invalid!"));

    // get X/Y/Z gyro offsets
    DEBUG_PRINTLN(F("Reading gyro offset values..."));
    int8_t xgOffset = MPU_getXGyroOffset();
    int8_t ygOffset = MPU_getYGyroOffset();
    int8_t zgOffset = MPU_getZGyroOffset();
    DEBUG_PRINT(F("X gyro offset = "));
    DEBUG_PRINTLN(xgOffset);
    DEBUG_PRINT(F("Y gyro offset = "));
    DEBUG_PRINTLN(ygOffset);
    DEBUG_PRINT(F("Z gyro offset = "));
    DEBUG_PRINTLN(zgOffset);

    // setup weird slave stuff (?)
    DEBUG_PRINTLN(F("Setting slave 0 address to 0x7F..."));
    MPU_setSlaveAddress(0, 0x7F);
    DEBUG_PRINTLN(F("Disabling I2C Master mode..."));
    MPU_setI2CMasterModeEnabled(FALSE);
    DEBUG_PRINTLN(F("Setting slave 0 address to 0x68 (self)..."));
    MPU_setSlaveAddress(0, 0x68);
    DEBUG_PRINTLN(F("Resetting I2C Master control..."));
    MPU_resetI2CMaster();
    delay(20);

    // load DMP code into memory banks
    DEBUG_PRINT(F("Writing DMP code to MPU memory banks ("));
    DEBUG_PRINT(MPU6050_DMP_CODE_SIZE);
    DEBUG_PRINTLN(F(" bytes)"));
    if (MPU_writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE, 0, 0, TRUE))
    {
        DEBUG_PRINTLN(F("Success! DMP code written and verified."));

        // write DMP configuration
        DEBUG_PRINT(F("Writing DMP configuration to MPU memory banks ("));
        DEBUG_PRINT(MPU6050_DMP_CONFIG_SIZE);
        DEBUG_PRINTLN(F(" bytes in config def)"));
        if (MPU_writeProgDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE))
        {
            DEBUG_PRINTLN(F("Success! DMP configuration written and verified."));

            DEBUG_PRINTLN(F("Setting clock source to Z Gyro..."));
            MPU_setClockSource(MPU6050_CLOCK_PLL_ZGYRO);

            DEBUG_PRINTLN(F("Setting DMP and FIFO_OFLOW interrupts enabled..."));
            MPU_setIntEnabled(0x12);

            DEBUG_PRINTLN(F("Setting sample rate to 200Hz..."));
            MPU_setRate(4); // 1khz / (1 + 4) = 200 Hz

            DEBUG_PRINTLN(F("Setting external frame sync to TEMP_OUT_L[0]..."));
            MPU_setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);

            DEBUG_PRINTLN(F("Setting DLPF bandwidth to 42Hz..."));
            MPU_setDLPFMode(MPU6050_DLPF_BW_42);

            DEBUG_PRINTLN(F("Setting gyro sensitivity to +/- 2000 deg/sec..."));
            MPU_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

            DEBUG_PRINTLN(F("Setting DMP configuration bytes (function unknown)..."));
            MPU_setDMPConfig1(0x03);
            MPU_setDMPConfig2(0x00);

            DEBUG_PRINTLN(F("Clearing OTP Bank flag..."));
            MPU_setOTPBankValid(FALSE);

            DEBUG_PRINTLN(F("Setting X/Y/Z gyro offsets to previous values..."));
            MPU_setXGyroOffset(xgOffset);
            MPU_setYGyroOffset(ygOffset);
            MPU_setZGyroOffset(zgOffset);

            DEBUG_PRINTLN(F("Setting X/Y/Z gyro user offsets to zero..."));
            MPU_setXGyroOffsetUser(0);
            MPU_setYGyroOffsetUser(0);
            MPU_setZGyroOffsetUser(0);

            DEBUG_PRINTLN(F("Writing final memory update 1/7 (function unknown)..."));
            uint8_t dmpUpdate[16], j;
            uint16_t pos = 0;
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], TRUE, FALSE);

            DEBUG_PRINTLN(F("Writing final memory update 2/7 (function unknown)..."));
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], TRUE, FALSE);

            DEBUG_PRINTLN(F("Resetting FIFO..."));
            MPU_resetFIFO();

            DEBUG_PRINTLN(F("Reading FIFO count..."));
            uint8_t fifoCount = MPU_getFIFOCount();
            uint8_t fifoBuffer[128];
/*
            DEBUG_PRINT(F("Current FIFO count=")); // Donn't work if I uncomment
            DEBUG_PRINTLN(fifoCount);              // these lines ????!!??WTF
            MPU_getFIFOBytes(fifoBuffer, fifoCount);
*/
            DEBUG_PRINTLN(F("Setting motion detection threshold to 2..."));
            MPU_setMotionDetectionThreshold(2);

            DEBUG_PRINTLN(F("Setting zero-motion detection threshold to 156..."));
            MPU_setZeroMotionDetectionThreshold(156);

            DEBUG_PRINTLN(F("Setting motion detection duration to 80..."));
            MPU_setMotionDetectionDuration(80);
            //delay(10);
            DEBUG_PRINTLN(F("Setting zero-motion detection duration to 0..."));
            MPU_setZeroMotionDetectionDuration(0);
            //delay(10);
            DEBUG_PRINTLN(F("Resetting FIFO..."));
            MPU_resetFIFO();
            //delay(10);
            DEBUG_PRINTLN(F("Enabling FIFO..."));
            MPU_setFIFOEnabled(TRUE);
            //delay(10);
            DEBUG_PRINTLN(F("Enabling DMP..."));
            MPU_setDMPEnabled(TRUE);

            DEBUG_PRINTLN(F("Resetting DMP..."));
            MPU_resetDMP();

            DEBUG_PRINTLN(F("Writing final memory update 3/7 (function unknown)..."));
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], TRUE, FALSE);

            DEBUG_PRINTLN(F("Writing final memory update 4/7 (function unknown)..."));
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], TRUE, FALSE);

            DEBUG_PRINTLN(F("Writing final memory update 5/7 (function unknown)..."));
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], TRUE, FALSE);

            DEBUG_PRINTLN(F("Waiting for FIFO count > 2..."));
            while ((fifoCount = MPU_getFIFOCount()) < 3);

            DEBUG_PRINT(F("Current FIFO count="));
            DEBUG_PRINTLN(fifoCount);
            DEBUG_PRINTLN(F("Reading FIFO data..."));
            MPU_getFIFOBytes(fifoBuffer, fifoCount);

            DEBUG_PRINTLN(F("Reading interrupt status..."));
            uint8_t mpuIntStatus = MPU_getIntStatus();

            DEBUG_PRINT(F("Current interrupt status="));
            DEBUG_PRINTLNF(mpuIntStatus, HEX);

            DEBUG_PRINTLN(F("Reading final memory update 6/7 (function unknown)..."));
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            DEBUG_PRINTLN(F("Waiting for FIFO count > 2..."));
            while ((fifoCount = MPU_getFIFOCount()) < 3);

            DEBUG_PRINT(F("Current FIFO count="));
            DEBUG_PRINTLN(fifoCount);

            DEBUG_PRINTLN(F("Reading FIFO data..."));
            MPU_getFIFOBytes(fifoBuffer, fifoCount);

            DEBUG_PRINTLN(F("Reading interrupt status..."));
            mpuIntStatus = MPU_getIntStatus();

            DEBUG_PRINT(F("Current interrupt status="));
            DEBUG_PRINTLNF(mpuIntStatus, HEX);

            DEBUG_PRINTLN(F("Writing final memory update 7/7 (function unknown)..."));
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            MPU_writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1], TRUE, FALSE);

            DEBUG_PRINTLN(F("DMP is good to go! Finally."));

            DEBUG_PRINTLN(F("Disabling DMP (you turn it on later)..."));
            MPU_setDMPEnabled(FALSE);
/**/
            DEBUG_PRINTLN(F("Setting up internal 42-byte (default) DMP packet buffer..."));
            dmpPacketSize = 42;
            /*if ((dmpPacketBuffer = (uint8_t *)malloc(42)) == 0) {
                return 3; // _TODO: proper error code for no memory
            }*/

            DEBUG_PRINTLN(F("Resetting FIFO and clearing INT status one last time..."));
            MPU_resetFIFO();
            MPU_getIntStatus();
        }
        else
        {
            DEBUG_PRINTLN(F("ERROR! DMP configuration verification failed."));
            return 2; // configuration block loading failed
        }
    }
    else
    {
        DEBUG_PRINTLN(F("ERROR! DMP code verification failed."));
        return 1; // main binary block loading failed
    }
    return 0; // success
}

Bool MPU_dmpPacketAvailable()
{
    return MPU_getFIFOCount() >= MPU_dmpGetFIFOPacketSize();
}

// uint8_t MPU6050::dmpSetFIFORate(uint8_t fifoRate);
// uint8_t MPU6050::dmpGetFIFORate();
// uint8_t MPU6050::dmpGetSampleStepSizeMS();
// uint8_t MPU6050::dmpGetSampleFrequency();
// int32_t MPU6050::dmpDecodeTemperature(int8_t tempReg);

//uint8_t MPU6050::dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
//uint8_t MPU6050::dmpUnregisterFIFORateProcess(inv_obj_func func);
//uint8_t MPU6050::dmpRunFIFORateProcesses();

// uint8_t MPU6050::dmpSendQuaternion(uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendPacketNumber(uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);

uint8_t MPU_dmpGetAccel32(int32_t *data, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((int32_t) packet[28] << 24) + ((int32_t) packet[29] << 16) + ((int32_t) packet[30] << 8) + (int32_t) packet[31]);
    data[1] = (((int32_t) packet[32] << 24) + ((int32_t) packet[33] << 16) + ((int32_t) packet[34] << 8) + (int32_t) packet[35]);
    data[2] = (((int32_t) packet[36] << 24) + ((int32_t) packet[37] << 16) + ((int32_t) packet[38] << 8) + (int32_t) packet[39]);
    return 0;
}

uint8_t MPU_dmpGetAccel16(int16_t *data, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((int16_t) packet[28] << 8) + (int16_t) packet[29];
    data[1] = ((int16_t) packet[32] << 8) + (int16_t) packet[33];
    data[2] = ((int16_t) packet[36] << 8) + (int16_t) packet[37];
    return 0;
}

uint8_t MPU_dmpGetAccelV16(VectorInt16 *v, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    v -> x = ((int16_t) packet[28] << 8) + (int16_t) packet[29];
    v -> y = ((int16_t) packet[32] << 8) + (int16_t) packet[33];
    v -> z = ((int16_t) packet[36] << 8) + (int16_t) packet[37];
    return 0;
}

uint8_t MPU_dmpGetQuaternion32(int32_t *data, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((int32_t) packet[0] << 24) + ((int32_t) packet[1] << 16) + ((int32_t) packet[2] << 8) + (int32_t) packet[3]);
    data[1] = (((int32_t) packet[4] << 24) + ((int32_t) packet[5] << 16) + ((int32_t) packet[6] << 8) + (int32_t) packet[7]);
    data[2] = (((int32_t) packet[8] << 24) + ((int32_t) packet[9] << 16) + ((int32_t) packet[10] << 8) + (int32_t) packet[11]);
    data[3] = (((int32_t) packet[12] << 24) + ((int32_t) packet[13] << 16) + ((int32_t) packet[14] << 8) + (int32_t) packet[15]);
    return 0;
}

uint8_t MPU_dmpGetQuaternion16(int16_t *data, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((int16_t) packet[0] << 8) + (int16_t) packet[1]);
    data[1] = (((int16_t) packet[4] << 8) + (int16_t) packet[5]);
    data[2] = (((int16_t) packet[8] << 8) + (int16_t) packet[9]);
    data[3] = (((int16_t) packet[12] << 8) + (int16_t) packet[13]);
    return 0;
}

uint8_t MPU_dmpGetQuaternion(Quaternion *q, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    int16_t qI[4];
    uint8_t status = MPU_dmpGetQuaternion16(qI, packet);
    if (status == 0)
    {
        q -> w = (float) qI[0] / 16384.0f;
        q -> x = (float) qI[1] / 16384.0f;
        q -> y = (float) qI[2] / 16384.0f;
        q -> z = (float) qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}
// uint8_t MPU_dmpGet6AxisQuaternion(long *data, const uint8_t* packet);
// uint8_t MPU_dmpGetRelativeQuaternion(long *data, const uint8_t* packet);

uint8_t MPU_dmpGetGyro32(int32_t *data, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((int32_t) packet[16] << 24) + ((int32_t) packet[17] << 16) + ((int32_t) packet[18] << 8) + (int32_t) packet[19]);
    data[1] = (((int32_t) packet[20] << 24) + ((int32_t) packet[21] << 16) + ((int32_t) packet[22] << 8) + (int32_t) packet[23]);
    data[2] = (((int32_t) packet[24] << 24) + ((int32_t) packet[25] << 16) + ((int32_t) packet[26] << 8) + (int32_t) packet[27]);
    return 0;
}

uint8_t MPU_dmpGetGyro16(int16_t *data, const uint8_t* packet)
{
    // _TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((int16_t) packet[16] << 8) + (int16_t) packet[17];
    data[1] = ((int16_t) packet[20] << 8) + (int16_t) packet[21];
    data[2] = ((int16_t) packet[24] << 8) + (int16_t) packet[25];
    return 0;
}
// uint8_t MPU6050::dmpSetLinearAccelFilterCoefficient(float coef);
// uint8_t MPU6050::dmpGetLinearAccel(long *data, const uint8_t* packet);

uint8_t MPU_dmpGetLinearAccelVV16(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity)
{
    // get rid of the gravity component (+1g = +4096 in standard DMP FIFO packet)
    v -> x = vRaw -> x - gravity -> x * 4096;
    v -> y = vRaw -> y - gravity -> y * 4096;
    v -> z = vRaw -> z - gravity -> z * 4096;
    return 0;
}
// uint8_t MPU_dmpGetLinearAccelInWorld(long *data, const uint8_t* packet);

uint8_t MPU_dmpGetLinearAccelInWorldVV16(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q)
{
    // rotate measured 3D acceleration vector into original state
    // frame of reference based on orientation quaternion
    memcpy(v, vReal, sizeof (VectorInt16));
    V16_rotate(v, q);
    return 0;
}
// uint8_t MPU6050::dmpGetGyroAndAccelSensor(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetGyroSensor(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetControlData(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetTemperature(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetGravity(long *data, const uint8_t* packet);

uint8_t MPU_dmpGetGravity(VectorFloat *v, Quaternion *q)
{
    v -> x = 2 * (q -> x * q -> z - q -> w * q -> y);
    v -> y = 2 * (q -> w * q -> x + q -> y * q -> z);
    v -> z = q -> w * q -> w - q -> x * q -> x - q -> y * q -> y + q -> z * q -> z;
    return 0;
}
// uint8_t MPU6050::dmpGetUnquantizedAccel(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetQuantizedAccel(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetExternalSensorData(long *data, int size, const uint8_t* packet);
// uint8_t MPU6050::dmpGetEIS(long *data, const uint8_t* packet);

uint8_t MPU_dmpGetEuler(float *data, Quaternion *q)
{
    data[0] = atan2(2 * q -> x * q -> y - 2 * q -> w * q -> z, 2 * q -> w * q -> w + 2 * q -> x * q -> x - 1); // psi
    data[1] = -asin(2 * q -> x * q -> z + 2 * q -> w * q -> y); // theta
    data[2] = atan2(2 * q -> y * q -> z - 2 * q -> w * q -> x, 2 * q -> w * q -> w + 2 * q -> z * q -> z - 1); // phi
    return 0;
}

uint8_t MPU_dmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity)
{
    // yaw: (about Z axis)
    data[0] = atan2(2 * q -> x * q -> y - 2 * q -> w * q -> z, 2 * q -> w * q -> w + 2 * q -> x * q -> x - 1);
    // pitch: (nose up/down, about Y axis)
    data[1] = atan(gravity -> x / sqrt(gravity -> y * gravity -> y + gravity -> z * gravity -> z));
    // roll: (tilt left/right, about X axis)
    data[2] = atan(gravity -> y / sqrt(gravity -> x * gravity -> x + gravity -> z * gravity -> z));
    return 0;
}

// uint8_t MPU6050::dmpGetAccelFloat(float *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetQuaternionFloat(float *data, const uint8_t* packet);

uint8_t MPU_dmpProcessFIFOPacket(const unsigned char *dmpData)
{
    /*for (uint8_t k = 0; k < dmpPacketSize; k++) {
        if (dmpData[k] < 0x10) Serial.print("0");
        Serial.print(dmpData[k], HEX);
        Serial.print(" ");
    }
    Serial.print("\n");*/
    //Serial.println((uint16_t)dmpPacketBuffer);
    return 0;
}

uint8_t MPU_dmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed)
{
    uint8_t status;
    uint8_t buf[dmpPacketSize];
    uint8_t i;
    for (i = 0; i < numPackets; i++)
    {
        // read packet from FIFO
        MPU_getFIFOBytes(buf, dmpPacketSize);

        // process packet
        if ((status = MPU_dmpProcessFIFOPacket(buf)) > 0) return status;

        // increment external process count variable, if supplied
        //if (processed != 0) *processed++;
    }
    return 0;
}

// uint8_t MPU6050::dmpSetFIFOProcessedCallback(void (*func) (void));

// uint8_t MPU6050::dmpInitFIFOParam();
// uint8_t MPU6050::dmpCloseFIFO();
// uint8_t MPU6050::dmpSetGyroDataSource(uint_fast8_t source);
// uint8_t MPU6050::dmpDecodeQuantizedAccel();
// uint32_t MPU6050::dmpGetGyroSumOfSquare();
// uint32_t MPU6050::dmpGetAccelSumOfSquare();
// void MPU6050::dmpOverrideQuaternion(long *q);

uint16_t MPU_dmpGetFIFOPacketSize()
{
    return dmpPacketSize;
}



