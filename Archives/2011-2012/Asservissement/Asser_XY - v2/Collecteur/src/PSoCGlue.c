#include "../../PSoC/CommandeAsser.cydsn/test_asser.h"
#include "../../PSoC/CommandeAsser.cydsn/Boolean.h"
#include <stdio.h>
#include "AsserGlue.h"
#include "PSoCGlue.h"
#include "../../PSoC/CommandeAsser.cydsn/asser.h"
#include "../../PSoC/CommandeAsser.cydsn/init.h"

inline void PSoCstep() {
	test();
}

void UART_XBee_PutChar(unsigned char txDataByte) {
	putchar(txDataByte);
}

void UART_XBee_PutString(char* string) {
	printf("%s", string);
}

void UART_Asser_PutChar(unsigned char txDataByte) {
	//printf("A%02x", txDataByte);
	Asser_UART_setRXByte(txDataByte);
}

void CyDelay(unsigned long milliseconds) {
}

static boolean isAsserRXIsrEnabled = FALSE;

void UART_Asser_RX_isr_Disable() {
	isAsserRXIsrEnabled = FALSE;
}

void UART_Asser_RX_isr_Enable() {
	isAsserRXIsrEnabled = TRUE;
}

void UART_XBee_Start() {
}

static boolean isXBeeRXIsrEnabled = FALSE;

void UART_XBee_RX_isr_Start() {
	isXBeeRXIsrEnabled = TRUE;
}

void UART_Asser_Start() {
}

void UART_Asser_RX_isr_Start() {
	isAsserRXIsrEnabled = TRUE;
}

unsigned char UART_Asser_RX_isr_GetState(void) {
	return isAsserRXIsrEnabled == TRUE ? 1 : 0;
}

void UART_Asser_RX_isr_ClearPending(void) {
}

void UART_XBee_RX_isr_Disable(void) {
	isXBeeRXIsrEnabled = FALSE;
}

unsigned char UART_XBee_RX_isr_GetState(void) {
	return isXBeeRXIsrEnabled == TRUE ? 1 : 0;
}

void UART_XBee_RX_isr_ClearPending(void) {
}

void UART_XBee_RX_isr_Enable(void) {
	isXBeeRXIsrEnabled = TRUE;
}

unsigned char Couleur_Read(void) {
	return 0;	// 0=Violet 1=Rouge
}

static boolean isDMA_Asser_To_XBeeEnabled = FALSE;

void PSoC_UART_fromAsserSetRXByte(unsigned char byte) {
	if (isDMA_Asser_To_XBeeEnabled)
		UART_XBee_PutChar(byte);
	if (isAsserRXIsrEnabled)
		UART_Asser_RX_Handler(byte);
}

static boolean isDMA_XBee_To_AsserEnabled = FALSE;
static unsigned char UART_fromXBeeRX_byte = 0;

void PSoC_UART_fromXBeeSetRXByte(unsigned char byte) {
	if (isDMA_XBee_To_AsserEnabled)
		UART_Asser_PutChar(byte);
	if (isAsserRXIsrEnabled) {
		UART_fromXBeeRX_byte = byte;
		if (gamePhase == JEU) return;
		if (gamePhase == ATTENTE) {
			gamePhase = TEST_PHASE;
			isRed = Couleur_Read();
#if SEND_COLOR
			if (isRed)	sendRED();
			else		sendPURPLE();
#endif // #if SEND_COLOR
		}
		test_message_processing();
	}
}

unsigned char UART_XBee_ReadRxData(void) {
	return UART_fromXBeeRX_byte;
}

unsigned char DMA_UART_XBee_DmaInitialize(unsigned char BurstCount, unsigned char ReqestPerBurst, unsigned short UpperSrcAddress, unsigned short UpperDestAddress) {
	return 1;
}

unsigned char DMA_UART_Asser_DmaInitialize(unsigned char BurstCount, unsigned char ReqestPerBurst, unsigned short UpperSrcAddress, unsigned short UpperDestAddress) {
	return 1;
}

cystatus CyDmaChEnable(uint8 chHandle, uint8 preserveTds) {
	return 0;
}

cystatus CyDmaChStatus(uint8 chHandle, uint8 * currentTd, uint8 * state) {
	return 0;
}

uint8 CyDmaTdAllocate(void) {
	static uint8 ret = 0;
	ret ++;
	if (ret == DMA_END_CHAIN_TD || ret == 0) ret ++;
	return ret;
}

static uint8 td[2] = {DMA_END_CHAIN_TD, DMA_END_CHAIN_TD};

cystatus CyDmaTdSetAddress(uint8 tdHandle, uint16 source, uint16 destination) {
	if (source == LO16(UART_Asser_RXDATA_PTR)) td[0] = tdHandle;
	else td[1] = tdHandle;
	return 0;
}

cystatus CyDmaTdSetConfiguration(uint8 tdHandle, uint16 transferCount, uint8 nextTd, uint8 configuration) {
	if (nextTd == DMA_END_CHAIN_TD) {
		if (td[0] == tdHandle) isDMA_Asser_To_XBeeEnabled = FALSE;
		else isDMA_XBee_To_AsserEnabled = FALSE;
	} else {
		if (td[0] == tdHandle) isDMA_Asser_To_XBeeEnabled = TRUE;
		else isDMA_XBee_To_AsserEnabled = TRUE;
	}
	return 0;
}

cystatus CyDmaChSetInitialTd(uint8 chHandle, uint8 startTd) {
	return 0;
}

cystatus CyDmaChSetRequest(uint8 chHandle, uint8 request) {
	return 0;
}

unsigned char CyDmaChGetRequest(DMA_id_t id) {
	return 0;
}

void CyDmaTdGetConfiguration(DMA_id_t id, unsigned short * transferCount, unsigned char * nextTd, unsigned char * configuration) {
}
