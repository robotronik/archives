/*
 * File:   i2cIMU.c
 * Author: jeremy
 *
 * Created on 29 décembre 2012, 19:05
 */

#include <stdint.h>
#include "i2c.h"
#include "i2cIMU.h"

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 *
 * TODO : Pourrait avantageusement remplacer par
 * return I2C_writeBytes(devAddr, regAddr, 1, &data);
 *
 */
Bool I2C_writeByte(unsigned char devAddr, unsigned char regAddr, unsigned char data)
{
    IdleI2C(); //wait for bus Idle
    StartI2C(); //Generate Start Condition

    WriteI2C((devAddr << 1) | 0x0); //Write Control Byte Addr(8:1) + W/R(0)
    IdleI2C(); //wait for bus Idle

    WriteI2C(regAddr); //Write register address
    IdleI2C(); //wait for bus Idle

    WriteI2C(data); //Write Data
    IdleI2C();

    //Bool ret = ACKStatus();

    StopI2C(); //Generate Stop

    //return ret;
    return ACKStatus();
}

/** Write multiple bytes to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */
Bool I2C_writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data)
{
    unsigned int i;
    IdleI2C(); //wait for bus Idle
    StartI2C(); //Generate Start Condition

    WriteI2C((devAddr << 1) | 0x0); //Write Control Byte Addr(8:1) + W/R(0)
    IdleI2C(); //wait for bus Idle

    WriteI2C(regAddr); //Write register address
    IdleI2C(); //wait for bus Idle

    for (i = 0; i < length; i++)
    {
        WriteI2C(data[i]); //Write Data
        IdleI2C();
    }

    StopI2C(); //Generate Stop

    return ACKStatus();
}

/** Write multiple words to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register address to write to
 * @param length Number of words to write
 * @param data Buffer to copy new data from
 * @return Status of operation (true = success)
 */
Bool I2C_writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t* data)
{
    unsigned int i;
    IdleI2C(); //wait for bus Idle
    StartI2C(); //Generate Start Condition

    WriteI2C((devAddr << 1) | 0x0); //Write Control Byte Addr(8:1) + W/R(0)
    IdleI2C(); //wait for bus Idle

    WriteI2C(regAddr); //Write register address
    IdleI2C(); //wait for bus Idle

    for (i = 0; i < length; i++)
    {
        WriteI2C((unsigned char)(data[i]>>8)); //Write Data
        IdleI2C();
        WriteI2C((unsigned char)(data[i])); //Write Data
        IdleI2C();
    }

    StopI2C(); //Generate Stop

    return ACKStatus();
}

/** Write single word to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New word value to write
 * @return Status of operation (true = success)
 */
Bool I2C_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data)
{
    return I2C_writeWords(devAddr, regAddr, 1, &data);
}


/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
Bool I2C_writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    I2C_readByte(devAddr, regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return I2C_writeByte(devAddr, regAddr, b);
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
Bool I2C_writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (I2C_readByte(devAddr, regAddr, &b) != 0)
    {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return I2C_writeByte(devAddr, regAddr, b);
    }
    else
    {
        return FALSE;
    }
}

/** write a single bit in a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-15)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
Bool I2C_writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data)
{
    uint16_t w;
    I2C_readWord(devAddr, regAddr, &w);
    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
    return I2C_writeWord(devAddr, regAddr, w);
}

/** Write multiple bits in a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-15)
 * @param length Number of bits to write (not more than 16)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
Bool I2C_writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data)
{
    //              010 value to write
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    // 0001110000000000 mask byte
    // 1010111110010110 original value (sample)
    // 1010001110010110 original & ~mask
    // 1010101110010110 masked | value
    uint16_t w;
    if (I2C_readWord(devAddr, regAddr, &w) != 0)
    {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        return I2C_writeWord(devAddr, regAddr, w);
    }
    else
    {
        return FALSE;
    }
}

/** Read single byte from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for byte value read from device
 * @return Status of read operation (true = success)
 *
 * TODO : Pourrait avantageusement remplacer par
 * return I2C_readBytes(devAddr, regAddr, 1, &data);
 */
int8_t I2C_readByte(unsigned char devAddr, unsigned char regAddr, unsigned char *data)
{
    IdleI2C(); //wait for bus Idle
    StartI2C(); //Generate Start Condition

    WriteI2C((devAddr << 1) | 0x0); //Write Control Byte
    IdleI2C(); //wait for bus Idle

    WriteI2C(regAddr); //Write start address
    IdleI2C(); //wait for bus Idle

    RestartI2C(); //Generate restart condition
    WriteI2C((devAddr << 1) | 0x01); //Write control byte for read
    IdleI2C(); //wait for bus Idle

    *data = getI2C(); //read one byte
    NotAckI2C(); //Send Not Ack

    StopI2C(); //Generate Stop
    return 1; // 1 byte read
}

/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Number of bytes read (-1 indicates failure)
 */
int8_t I2C_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data)
{
    unsigned int i;
    IdleI2C(); //wait for bus Idle
    StartI2C(); //Generate Start Condition

    WriteI2C((devAddr << 1) | 0x0); //Write Control Byte for write
    IdleI2C(); //wait for bus Idle

    WriteI2C(regAddr); //Write start address
    IdleI2C(); //wait for bus Idle

    RestartI2C(); //Generate restart condition
    WriteI2C((devAddr << 1) | 0x01); //Write control byte for read
    IdleI2C(); //wait for bus Idle

    for (i = 0; i < length; i++)
    {
        data[i] = getI2C(); //read one byte
        if (i < (length - 1))
            AckI2C();
    }
    NotAckI2C(); //Send Not Ack

    StopI2C(); //Generate Stop
    
    return i+1;
}

/** Read multiple words from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of words to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Number of words read (0 indicates failure)
 */
int8_t I2C_readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data)
{
    unsigned int i;
    IdleI2C(); //wait for bus Idle
    StartI2C(); //Generate Start Condition

    WriteI2C((devAddr << 1) | 0x0); //Write Control Byte for write
    IdleI2C(); //wait for bus Idle

    WriteI2C(regAddr); //Write start address
    IdleI2C(); //wait for bus Idle

    RestartI2C(); //Generate restart condition
    WriteI2C((devAddr << 1) | 0x01); //Write control byte for read
    IdleI2C(); //wait for bus Idle

    for (i = 0; i < length; i++)
    {
        data[i] = getI2C() << 8; //MSB
        AckI2C();
        data[i] |= getI2C();  //LSB
        if (i < (length - 1))
            AckI2C();
    }
    NotAckI2C(); //Send Not Ack

    StopI2C(); //Generate Stop

    return i+1;
}

/** Read single word from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for word value read from device
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2C_readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data)
{
    return I2C_readWords(devAddr, regAddr, 1, data);
}

/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @return Status of read operation (true = success)
 */
int8_t I2C_readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data)
{
    uint8_t b;
    uint8_t count = I2C_readByte(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return count;
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @return Status of read operation (true = success)
 */
int8_t I2C_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = I2C_readByte(devAddr, regAddr, &b)) != 0)
    {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
    return count;
}

/** Read a single bit from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-15)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (true = success)
 */
int8_t I2C_readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data)
{
    uint16_t b;
    uint8_t count = I2C_readWord(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return count;
}

/** Read multiple bits from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-15)
 * @param length Number of bits to read (not more than 16)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in I2Cdev::readTimeout)
 * @return Status of read operation (1 = success, 0 = failure, -1 = timeout)
 */
int8_t I2C_readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data)
{
    // 1101011001101001 read byte
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    //    010           masked
    //           -> 010 shifted
    uint8_t count;
    uint16_t w;
    if ((count = I2C_readWord(devAddr, regAddr, &w)) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        *data = w;
    }
    return count;
}

