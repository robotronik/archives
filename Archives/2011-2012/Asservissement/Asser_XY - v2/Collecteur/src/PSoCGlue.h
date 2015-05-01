#ifndef _PSOC_GLUE_H
#define _PSOC_GLUE_H

#include <stdio.h>
#include "../../PSoC/CommandeAsser.cydsn/DMA.h"

#define CYGlobalIntEnable
#define LO16(x)                 ((unsigned short) (x))
#define HI16(x)                 ((unsigned short) ((unsigned long)(x) >> 16))
#define DMA_END_CHAIN_TD                0xFFu
#define STATUS_TD_ACTIVE                (1 << 1)
#define CPU_REQ                         (1 << 0)

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;

typedef signed char             int8;
typedef signed short            int16;
typedef signed long             int32;

typedef unsigned long cystatus;

enum{UART_XBee_TXDATA_PTR, UART_Asser_TXDATA_PTR, UART_XBee_RXDATA_PTR, UART_Asser_RXDATA_PTR};

void PSoC_UART_fromXBeeSetRXByte(unsigned char byte);
void PSoC_UART_fromAsserSetRXByte(unsigned char byte);

void UART_XBee_PutChar(unsigned char txDataByte);
void UART_XBee_PutString(char* string);
void UART_Asser_PutChar(unsigned char txDataByte);
void CyDelay(unsigned long milliseconds);
void UART_Asser_RX_isr_Disable();
void UART_Asser_RX_isr_Enable();
void UART_XBee_Start();
void UART_XBee_RX_isr_Start();
void UART_Asser_Start();
void UART_Asser_RX_isr_Start();
unsigned char UART_Asser_RX_isr_GetState(void);
void UART_Asser_RX_isr_ClearPending(void);
void UART_XBee_RX_isr_Disable(void);
unsigned char UART_XBee_RX_isr_GetState(void);
void UART_XBee_RX_isr_ClearPending(void);
void UART_XBee_RX_isr_Enable(void);
unsigned char Couleur_Read(void);
unsigned char UART_XBee_ReadRxData(void);
unsigned char DMA_UART_XBee_DmaInitialize(unsigned char BurstCount, unsigned char ReqestPerBurst, unsigned short UpperSrcAddress, unsigned short UpperDestAddress);
unsigned char DMA_UART_Asser_DmaInitialize(unsigned char BurstCount, unsigned char ReqestPerBurst, unsigned short UpperSrcAddress, unsigned short UpperDestAddress);
cystatus CyDmaChEnable(uint8 chHandle, uint8 preserveTds);
cystatus CyDmaChStatus(uint8 chHandle, uint8 * currentTd, uint8 * state);
uint8 CyDmaTdAllocate(void);
cystatus CyDmaTdSetAddress(uint8 tdHandle, uint16 source, uint16 destination);
cystatus CyDmaTdSetConfiguration(uint8 tdHandle, uint16 transferCount, uint8 nextTd, uint8 configuration);
cystatus CyDmaChSetInitialTd(uint8 chHandle, uint8 startTd);
cystatus CyDmaChSetRequest(uint8 chHandle, uint8 request);
unsigned char CyDmaChGetRequest(DMA_id_t id);
void CyDmaTdGetConfiguration(DMA_id_t id, unsigned short * transferCount, unsigned char * nextTd, unsigned char * configuration);

#endif
