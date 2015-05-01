/**
 * Mirf
 *
 * Additional bug fixes and improvements
 *  11/03/2011:
 *   Switched spi library.
 *  07/13/2010:
 *   Added example to read a register
 *  11/12/2009:
 *   Fix dataReady() to work correctly
 *   Renamed keywords to keywords.txt ( for IDE ) and updated keyword list
 *   Fixed client example code to timeout after one second and try again
 *    when no response received from server
 * By: Nathan Isburgh <nathan@mrroot.net>
 * $Id: mirf.cpp 67 2010-07-13 13:25:53Z nisburgh $
 *
 *
 * An Ardunio port of:
 * http://www.tinkerer.eu/AVRLib/nRF24L01
 *
 * Significant changes to remove depencence on interupts and auto ack support.
 *
 * Aaron Shrimpton <aaronds@gmail.com>
 *
 *
 * port to PIC33F by Jérémy F
 *
 */

/*
    Copyright (c) 2007 Stefan Engelke <mbox@stefanengelke.de>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy,
    modify, merge, publish, distribute, sublicense, and/or sell copies
    of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

    $Id: mirf.cpp 67 2010-07-13 13:25:53Z nisburgh $
 */

#include <xc.h>
#include "Mirf.h"
#include "spi.h"
// Defines for setting the MiRF registers for transmitting or receiving mode

Nrf24l mirf = {/*.cePin = CE_PIN, .csnPin = CSN_PIN, */.channel = 1, .payload = 16};

void MIRFtransferSync(uint8_t *dataout, uint8_t *datain, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        datain[i] = SPItransfer(dataout[i]);
    }
}

void MIRFtransmitSync(uint8_t *dataout, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        SPItransfer(dataout[i]);
    }
}

void MIRFinit()
// Initializes pins to communicate with the MiRF module
// Should be called in the early initializing phase at startup.
{
    //pinMode(cePin, OUTPUT);
    //pinMode(csnPin, OUTPUT);

    MIRFceLow();
    MIRFcsnHi();

    // Initialize spi module
    //SPIbegin();

}

void MIRFconfig()
// Sets the important registers in the MiRF module and powers the module
// in receiving mode
// NB: channel and payload must be set now.
{
    // Set RF channel
    MIRFconfigRegister(RF_CH, mirf.channel);

    // Set length of incoming payload
    MIRFconfigRegister(RX_PW_P0, mirf.payload);
    MIRFconfigRegister(RX_PW_P1, mirf.payload);

    // Start receiver
    MIRFpowerUpRx();
    MIRFflushRx();
}

void MIRFsetRADDR(uint8_t * adr)
// Sets the receiving address
{
    MIRFceLow();
    MIRFwriteRegister(RX_ADDR_P1, adr, mirf_ADDR_LEN);
    MIRFceHi();
}

void MIRFsetTADDR(uint8_t * adr)
// Sets the transmitting address
{
    /*
     * RX_ADDR_P0 must be set to the sending addr for auto ack to work.
     */

    MIRFwriteRegister(RX_ADDR_P0, adr, mirf_ADDR_LEN);
    MIRFwriteRegister(TX_ADDR, adr, mirf_ADDR_LEN);
}

extern Bool MIRFdataReady()
// Checks if data is available for reading
{
    // See note in getData() function - just checking RX_DR isn't good enough
    uint8_t status = MIRFgetStatus();

    // We can short circuit on RX_DR, but if it's not set, we still need
    // to check the FIFO for any pending packets
    if (status & (1 << RX_DR)) return 1;
    return !MIRFrxFifoEmpty();
}

extern Bool MIRFrxFifoEmpty()
{
    uint8_t fifoStatus;

    MIRFreadRegister(FIFO_STATUS, &fifoStatus, sizeof (fifoStatus));
    return (fifoStatus & (1 << RX_EMPTY));
}

extern void MIRFgetData(uint8_t * data)
// Reads payload bytes into data array
{
    MIRFcsnLow(); // Pull down chip select
    SPItransfer(R_RX_PAYLOAD); // Send cmd to read rx payload
    MIRFtransferSync(data, data, mirf.payload); // Read payload
    MIRFcsnHi(); // Pull up chip select
    // NVI: per product spec, p 67, note c:
    //  "The RX_DR IRQ is asserted by a new packet arrival event. The procedure
    //  for handling this interrupt should be: 1) read payload through SPI,
    //  2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more
    //  payloads available in RX FIFO, 4) if there are more data in RX FIFO,
    //  repeat from step 1)."
    // So if we're going to clear RX_DR here, we need to check the RX FIFO
    // in the dataReady() function
    MIRFconfigRegister(STATUS, (1 << RX_DR)); // Reset status register
}

void MIRFconfigRegister(uint8_t reg, uint8_t value)
// Clocks only one byte into the given MiRF register
{
    MIRFcsnLow();
    SPItransfer(W_REGISTER | (REGISTER_MASK & reg));
    SPItransfer(value);
    MIRFcsnHi();
}

void MIRFreadRegister(uint8_t reg, uint8_t * value, uint8_t len)
// Reads an array of bytes from the given start position in the MiRF registers.
{
    MIRFcsnLow();
    SPItransfer(R_REGISTER | (REGISTER_MASK & reg));
    MIRFtransferSync(value, value, len);
    MIRFcsnHi();
}

void MIRFwriteRegister(uint8_t reg, uint8_t * value, uint8_t len)
// Writes an array of bytes into inte the MiRF registers.
{
    MIRFcsnLow();
    SPItransfer(W_REGISTER | (REGISTER_MASK & reg));
    MIRFtransmitSync(value, len);
    MIRFcsnHi();
}

void MIRFsend(uint8_t * value)
// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
{
    uint8_t status;
    status = MIRFgetStatus();

    while (mirf.PTX)
    {
        status = MIRFgetStatus();

        if ((status & ((1 << TX_DS) | (1 << MAX_RT))))
        {
            mirf.PTX = 0;
            break;
        }
    } // Wait until last paket is send

    MIRFceLow();

    MIRFpowerUpTx(); // Set to transmitter mode , Power up

    MIRFcsnLow(); // Pull down chip select
    SPItransfer(FLUSH_TX); // Write cmd to flush tx fifo
    MIRFcsnHi(); // Pull up chip select

    MIRFcsnLow(); // Pull down chip select
    SPItransfer(W_TX_PAYLOAD); // Write cmd to write mirf.payload
    MIRFtransmitSync(value, mirf.payload); // Write payload
    MIRFcsnHi(); // Pull up chip select

    MIRFceHi(); // Start transmission
}

/**
 * isSending.
 *
 * Test if chip is still sending.
 * When sending has finished return chip to listening.
 *
 */

Bool MIRFisSending()
{
    uint8_t status;
    if (mirf.PTX)
    {
        status = MIRFgetStatus();

        /*
         *  if sending successful (TX_DS) or max retries exceded (MAX_RT).
         */

        if ((status & ((1 << TX_DS) | (1 << MAX_RT))))
        {
            MIRFpowerUpRx();
            return FALSE;
        }

        return TRUE;
    }
    return FALSE;
}

uint8_t MIRFgetStatus()
{
    uint8_t rv;
    MIRFreadRegister(STATUS, &rv, 1);
    return rv;
}

void MIRFpowerUpRx()
{
    mirf.PTX = 0;
    MIRFceLow();
    MIRFconfigRegister(CONFIG, mirf_CONFIG | ((1 << PWR_UP) | (1 << PRIM_RX)));
    MIRFceHi();
    MIRFconfigRegister(STATUS, (1 << TX_DS) | (1 << MAX_RT));
}

void MIRFflushRx()
{
    MIRFcsnLow();
    SPItransfer(FLUSH_RX);
    MIRFcsnHi();
}

void MIRFpowerUpTx()
{
    mirf.PTX = 1;
    MIRFconfigRegister(CONFIG, mirf_CONFIG | ((1 << PWR_UP) | (0 << PRIM_RX)));
}

void MIRFceHi()
{
    CE_PIN = 1;
    //digitalWrite(cePin,HIGH);
}

void MIRFceLow()
{
    CE_PIN = 0;
    //digitalWrite(cePin, LOW);
}

void MIRFcsnHi()
{
    CSN_PIN = 1;
    //digitalWrite(csnPin, HIGH);
}

void MIRFcsnLow()
{
    CSN_PIN = 0;
    //digitalWrite(csnPin, LOW);
}

void MIRFpowerDown()
{
    MIRFceLow();
    MIRFconfigRegister(CONFIG, mirf_CONFIG);
}
