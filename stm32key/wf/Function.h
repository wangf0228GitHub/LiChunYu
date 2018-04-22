#ifndef   __Function_h__
#define   __Function_h__

#include <stdint.h>
#include "main.h"

void GetKeyParam(void);
void Adjust2Dand37Data(void);
void Adjust41and4BData(void);
void Adjust55and5FData(void);
void UsedDEC(void);
void UpdateStepDatas(void);

void Check55and5fdata(void);
void Fix41and4Bdata(void);
void Fix2Dand37data(void);
void CheckDataBlockVerify(uint8_t Addr);
void HashCalc_N(uint32_t nCount);
void GetCalcTimesL(void);
void ReverseRom(uint8_t addr);
#endif
