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

    $Id$
 */
/* Modified by Jérémy F */

#ifndef MIRF_H
#define	MIRF_H

#include <stdint.h>
#include "constants.h"
#include "nRF24L01.h"

#define CSN_PIN LATBbits.LATB10
#define CE_PIN  LATBbits.LATB7

/*#define ADD_LATB(n) LATBbits.LATB##n

#define CSN_LAT ADD_LATB(CSN_PIN)
#define CE_LAT  ADD_LATB(CE_PIN)*/

// Nrf24l settings

#define mirf_ADDR_LEN	5
#define mirf_CONFIG ((1<<EN_CRC) | (0<<CRCO) )


void MIRFinit();
void MIRFconfig();
void MIRFsend(uint8_t *value);
void MIRFsetRADDR(uint8_t * adr);
void MIRFsetTADDR(uint8_t * adr);
Bool MIRFdataReady();
Bool MIRFisSending();
Bool MIRFrxFifoEmpty();
Bool MIRFtxFifoEmpty();
void MIRFgetData(uint8_t * data);
uint8_t MIRFgetStatus();

void MIRFtransmitSync(uint8_t *dataout, uint8_t len);
void MIRFtransferSync(uint8_t *dataout, uint8_t *datain, uint8_t len);
void MIRFconfigRegister(uint8_t reg, uint8_t value);
void MIRFreadRegister(uint8_t reg, uint8_t * value, uint8_t len);
void MIRFwriteRegister(uint8_t reg, uint8_t * value, uint8_t len);
void MIRFpowerUpRx();
void MIRFpowerUpTx();
void MIRFpowerDown();

void MIRFcsnHi();
void MIRFcsnLow();

void MIRFceHi();
void MIRFceLow();
void MIRFflushRx();

/*
 * In sending mode.
 */
struct Nrf24l
{
    uint8_t PTX;

    /*
     * CE Pin controls RX / TX, default 8.
     */

    //uint8_t cePin;

    /*
     * CSN Pin Chip Select Not, default 7.
     */

    //uint8_t csnPin;

    /*
     * Channel 0 - 127 or 0 - 84 in the US.
     */
    uint8_t channel;

    /*
     * Payload width in bytes default 16 max 32.
     */

    uint8_t payload;
};
typedef struct Nrf24l Nrf24l;
/*
 * Spi interface (must extend spi).
 */

//MirfSpiDriver *spi;


extern Nrf24l mirf;


#endif	/* MIRF_H */

