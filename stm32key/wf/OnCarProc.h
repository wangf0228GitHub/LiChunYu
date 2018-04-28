#ifndef   __OnCarProc_h__
#define   __OnCarProc_h__

#include <stdint.h>
#include "main.h"

void OnCarProc(void);
void IRTx2425Frame(void);
void ProcCommand_0F(void);
void ProcCommand_7A(void);
void ProcCommand_26(void);
void ProgramWork(uint8_t keyType,uint8_t maxNum);
void ProcCommand_39(void);//Ô¿³×Ñ§Ï°¹ý³Ì
void WaitCarPowerOff(void);
void LEDFlash(void);
#endif
