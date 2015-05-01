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

#ifndef _DMA_H
#define _DMA_H

typedef enum{DMA_ASSER_TO_XBEE, DMA_XBEE_TO_ASSER, DMA_XBEE_TO_XBEE, DMA_NB_ID} DMA_id_t;

void DMA_Enable(DMA_id_t id);
void DMA_Disable(DMA_id_t id);

// Debug
void DMA_ChSetRequest(DMA_id_t id);
unsigned char DMA_ChGetRequest(DMA_id_t id);
void DMA_ChStatus(DMA_id_t id, unsigned char * currentTd);
void DMA_TdGetConfiguration(DMA_id_t id, unsigned short * transferCount, unsigned char * nextTd, unsigned char * configuration);

#endif

//[] END OF FILE
