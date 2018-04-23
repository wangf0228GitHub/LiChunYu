#ifndef   __Function_h__
#define   __Function_h__

#include <stdint.h>
#include "main.h"

void GetKeyParam(void);

void UsedDEC(void);
void GetLeftTimeBlock(uint8_t nBlock);
void FixDataBlock(uint8_t fixAddr,uint8_t hashAddr,uint16_t stepLen,uint8_t loopTimes,uint8_t leftTiems);
void CheckDataBlockVerify(uint8_t Addr);
void HashCalc_N(uint32_t nCount);
void GetCalcTimes69(void);
void ReverseRom(uint8_t addr);
#endif
