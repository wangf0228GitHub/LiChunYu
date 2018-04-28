#ifndef __IRProc_h__
#define __IRProc_h__

#include "stdint.h"
#include "tim.h"


#define IRCommand IRRxList[1]
#define IRCommandParam IRRxList[2]

extern uint8_t IRTxList[50];
extern uint32_t IRTxCount;
extern uint32_t IRTxIndex;

extern uint32_t IRRxByteL;
extern uint32_t IRRxByteH;
extern uint8_t IRRxList[50];
extern uint32_t IRRxCount;
extern uint32_t IRRxNeedCount;

void IRTxProc(void);
void IRRxProc(uint32_t timeOut_ms);
uint8_t ThranslateIRRx(uint32_t time);
void IRTx_10_33_SSID(void);
void IRTx_10_28(void);
#endif


