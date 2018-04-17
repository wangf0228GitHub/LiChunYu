#ifndef __IRProc_h__
#define __IRProc_h__

#include "stdint.h"
#include "tim.h"


extern uint8_t IRTxList[50];
extern uint32_t IRTxCount;
extern uint32_t IRTxIndex;

extern uint32_t IRRxList[50];
extern uint32_t IRRxCount;

void IRTxProc(void);
void IRRxProc(void);
void IRRxDataProc(void);
void IRTx_10_33_SSID(void);
#endif


