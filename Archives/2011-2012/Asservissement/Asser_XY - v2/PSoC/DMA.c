/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifdef COLLECTEUR
	#include "../../Collecteur/src/PSoCGlue.h"
#elif defined(SIMU)	// #ifndef COLLECTEUR
	#include "../../SimuAsserPSoC/src/PSoCGlue.h"
#else
	#include <device.h>
#endif	// #ifndef COLLECTEUR
#include "DMA.h"

static unsigned char (*const dmaInitialize[DMA_NB_ID])(unsigned char, unsigned char, unsigned short, unsigned short)
	= {DMA_UART_Asser_DmaInitialize, DMA_UART_XBee_DmaInitialize, DMA_UART_XBee_DmaInitialize};
static const unsigned long srcAddr [DMA_NB_ID]
	= {(unsigned long) UART_Asser_RXDATA_PTR, (unsigned long) UART_XBee_RXDATA_PTR, (unsigned long) UART_XBee_RXDATA_PTR};
static const unsigned long destAddr[DMA_NB_ID]
	= {(unsigned long) UART_XBee_TXDATA_PTR, (unsigned long) UART_Asser_TXDATA_PTR, (unsigned long) UART_XBee_TXDATA_PTR};

static unsigned char get_DMA_Chan(DMA_id_t id) {
	static unsigned char DMA_Chan[DMA_NB_ID] = {0};
	if (!DMA_Chan[id])
		DMA_Chan[id] = dmaInitialize[id](1, 0, HI16(srcAddr[id]), HI16(destAddr[id]));
	return DMA_Chan[id];
}

static unsigned char get_DMA_TD(DMA_id_t id) {
	static unsigned char DMA_TD[DMA_NB_ID] = {0};
	if (!DMA_TD[id]) {
		DMA_TD[id] = CyDmaTdAllocate();
		CyDmaTdSetAddress(DMA_TD[id], LO16(srcAddr[id]), LO16(destAddr[id]));
	}
	return DMA_TD[id];
}

void DMA_Enable(DMA_id_t id) {
	unsigned char DMA_Chan = get_DMA_Chan(id);
	unsigned char DMA_TD = get_DMA_TD(id);
	CyDmaTdSetConfiguration(DMA_TD, 1, DMA_TD, 0);
	CyDmaChSetInitialTd(DMA_Chan, DMA_TD);
	uint8 state;
	CyDmaChStatus(DMA_Chan, NULL, &state);
	if (!(state & STATUS_TD_ACTIVE)) CyDmaChEnable(DMA_Chan, 1);
}

void DMA_Disable(DMA_id_t id) {
	CyDmaTdSetConfiguration(get_DMA_TD(id), 1, DMA_END_CHAIN_TD, 0);
}

void DMA_ChSetRequest(DMA_id_t id) {
	CyDmaChSetRequest(get_DMA_Chan(id), CPU_REQ);
}

unsigned char DMA_ChGetRequest(DMA_id_t id) {
	return CyDmaChGetRequest(get_DMA_Chan(id));
}

void DMA_ChStatus(DMA_id_t id, uint8 * currentTd) {
	CyDmaChStatus(get_DMA_Chan(id), currentTd, NULL);
}

void DMA_TdGetConfiguration(DMA_id_t id, uint16 * transferCount, uint8 * nextTd, uint8 * configuration) {
	CyDmaTdGetConfiguration(get_DMA_TD(id), transferCount, nextTd, configuration);
}

/* [] END OF FILE */
