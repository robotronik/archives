/* 
 * File:   i2cIMU.h
 * Author: jeremy
 *
 * Created on 29 décembre 2012, 19:05
 */

#ifndef I2CIMU_H
#define	I2CIMU_H

#include <stdint.h>
#include "constants.h"


Bool I2C_writeByte(unsigned char devAddr, unsigned char regAddr, unsigned char data);
Bool I2C_writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data);
Bool I2C_writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t* data);
Bool I2C_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
Bool I2C_writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
Bool I2C_writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
Bool I2C_writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
Bool I2C_writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);

int8_t I2C_readByte(unsigned char devAddr, unsigned char regAddr, unsigned char *data);
int8_t I2C_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
int8_t I2C_readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);
int8_t I2C_readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data);
int8_t I2C_readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data);
int8_t I2C_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
int8_t I2C_readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data);
int8_t I2C_readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data);


#endif	/* I2CIMU_H */

