#ifndef   __OnCarProc_h__
#define   __OnCarProc_h__

#include "HardwareProfile.h"
#define CarIRCommand IRRxList[1]
#define CarIRCommandParam IRRxList[2]

void OnCarProc(void);
void CarIRRxProc(void);
void CarIRTxProc(void);
void CarIRTx_10_33_SSID(void);
#endif
