#ifndef   __OnCarProc_h__
#define   __OnCarProc_h__

#include <stdint.h>
#include "main.h"

#define CarIRCommand IRRxList[1]
#define CarIRCommandParam IRRxList[2]

void OnCarProc(void);
void IRTx2425Frame(void);
void ProcCommand_0F(void);
void ProcCommand_7A(void);
void ProcCommand_26(void);
void ProgramWork(uint8_t keyType,uint8_t maxNum);
void ProcCommand_39(void);//Ô¿³×Ñ§Ï°¹ý³Ì
void WaitCarPowerOff(void);
void LEDFlash(void);
void CarIRTx_10_33_SSID(void);
uint8_t ThranslateCarIRRx(uint32_t time);
#endif
