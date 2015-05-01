#include "../../PSoC/CommandeAsser.cydsn/test_asser.h"
#include "../../PSoC/CommandeAsser.cydsn/DMA.h"
#include "../../PSoC/CommandeAsser.cydsn/asser.h"
#include "../../PSoC/CommandeAsser.cydsn/init.h"
#include "../../PSoC/CommandeAsser.cydsn/main.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "PSoCGlue.h"
#include "Canaux.h"

static boolean isAsserRXIsrEnabled = FALSE;
static boolean isXBeeRXIsrEnabled = FALSE;
static boolean isDMA_Asser_To_XBeeEnabled = FALSE;
static boolean isDMA_XBee_To_AsserEnabled = FALSE;
static unsigned char UART_fromXBeeRX_byte;

void *PSoCThread(void *state) {
	//time_t time_var = time(NULL);
	/*if (time(NULL) > time_var) {
		printf("PSoCThread\n");
		time_var = time(NULL);
	}*/
	
	PSoC_main();
	return NULL;
}

void UART_Asser_PutChar(unsigned char txDataByte) {
	UART_put(PSoC_to_asser, txDataByte);
	pthread_kill(asser_pthread, SIGUSR1);
	usleep(10000);
}

void PSoC_UART_from_asser_handler(unsigned char byte) {
	if (isDMA_Asser_To_XBeeEnabled)
		UART_XBee_PutChar(byte);
	if (isAsserRXIsrEnabled)
		UART_Asser_RX_Handler(byte);
}

void PSoC_UART_from_PC_handler(unsigned char byte) {
	UART_fromXBeeRX_byte = byte;
	if (isDMA_XBee_To_AsserEnabled)
		UART_Asser_PutChar(byte);
	if (isXBeeRXIsrEnabled) {
		if (gamePhase == JEU) return;
		if (gamePhase == ATTENTE) {
			gamePhase = TEST_PHASE;
			isRed = Couleur_Read();
#if SEND_COLOR
			if (isRed) sendRED();
			else sendPURPLE();
#endif // #if SEND_COLOR
		}
		test_message_processing();
	}
}

unsigned char UART_XBee_ReadRxData(void) {
	return UART_fromXBeeRX_byte;
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

void UART_XBee_PutChar(unsigned char txDataByte) {
	printf("\E[31m");
	printf("%c", txDataByte);
	printf("\E[m");
}

void UART_XBee_PutString(char* string) {
	printf("\E[31m");
	printf(string);
	printf("\E[m");
}

boolean Depart_Var(boolean invert) {
	static boolean status = TRUE;
	if (invert) status = !status;
	return status;
}

uint8 Depart_Read(void) {
	return Depart_Var(FALSE);
}

void UART_Asser_RX_isr_Disable() {
	isAsserRXIsrEnabled = FALSE;
}

void UART_Asser_RX_isr_Enable() {
	isAsserRXIsrEnabled = TRUE;
}

void UART_XBee_RX_isr_Start() {
	isXBeeRXIsrEnabled = TRUE;
}

void UART_XBee_RX_isr_Stop(void) {
	isXBeeRXIsrEnabled = FALSE;
}

void UART_Asser_RX_isr_Start() {
	isAsserRXIsrEnabled = TRUE;
}

void UART_Asser_RX_isr_Stop(void) {
	isAsserRXIsrEnabled = FALSE;
}

unsigned char UART_Asser_RX_isr_GetState(void) {
	return isAsserRXIsrEnabled == TRUE ? 1 : 0;
}

void UART_XBee_RX_isr_Disable(void) {
	isXBeeRXIsrEnabled = FALSE;
}

unsigned char UART_XBee_RX_isr_GetState(void) {
	return isXBeeRXIsrEnabled == TRUE ? 1 : 0;
}

void UART_XBee_RX_isr_Enable(void) {
	isXBeeRXIsrEnabled = TRUE;
}

cystatus CyDmaChSetInitialTd(uint8 chHandle, uint8 startTd) {
	return 0;
}

cystatus CyDmaChStatus(uint8 chHandle, uint8 * currentTd, uint8 * state) {
	return 0;
}

cystatus CyDmaChSetRequest(uint8 chHandle, uint8 request) {
	return 0;
}

cystatus CyDmaChGetRequest(uint8 chHandle) {
	return 0;
}

cystatus CyDmaTdGetConfiguration(uint8 tdHandle, uint16 * transferCount, uint8 *
	nextTd, uint8 * configuration) {
	return 0;
}

cystatus CyDmaChEnable(uint8 chHandle, uint8 preserveTds) {
	return 0;
}

void CyDelay(unsigned long milliseconds) {
	usleep(milliseconds * 1000);
}

unsigned char DMA_UART_XBee_DmaInitialize(unsigned char BurstCount, unsigned char ReqestPerBurst, unsigned short UpperSrcAddress, unsigned short UpperDestAddress) {
	return 1;
}

unsigned char DMA_UART_Asser_DmaInitialize(unsigned char BurstCount, unsigned char ReqestPerBurst, unsigned short UpperSrcAddress, unsigned short UpperDestAddress) {
	return 1;
}

void UART_Asser_Start() {
}

void UART_Asser_Stop(void) {
}

void UART_XBee_Start() {
}

void UART_XBee_Stop(void) {
}

void UART_Asser_RX_isr_ClearPending(void) {
}

void UART_XBee_RX_isr_ClearPending(void) {
}

void Couleur_Set(char couleur) {
	if (couleur == 'r') isRed = 1;
	else isRed = 0;
}

unsigned char Couleur_Read(void) {
	return isRed;
}

void    Timer_Fin_Start(void) {
	// Timer_Fin non implémenté
}

void isr_Fin_Start(void) {
	// Timer_Fin non implémenté
}

uint8 UART_XBee_ReadTxStatus(void) {
	return UART_XBee_TX_STS_COMPLETE | UART_XBee_TX_STS_FIFO_EMPTY | UART_XBee_TX_STS_FIFO_NOT_FULL;
}

void    PWM_ServoGauche_WriteCompare(uint16 compare) {
}

void    PWM_ServoDroit_WriteCompare(uint16 compare) {
}
