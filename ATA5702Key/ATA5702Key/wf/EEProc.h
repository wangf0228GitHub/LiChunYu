#ifndef EEProc_H
#define EEProc_H
#include "HardwareProfile.h"
#include "eep.h"

typedef union
{
	struct
	{
		unsigned bRomWrited:1;
		unsigned bStudy:1;
		unsigned bRFStudy:1;

		unsigned b55:1;
		unsigned b5f:1;
		unsigned b41:1;
		unsigned b4b:1;
		unsigned b2d:1;
		unsigned b37:1;

		unsigned bH0:1;
		unsigned bM0:1;
		unsigned bL0:1;
	}Bits;
	uint16_t all;
}_RomStateFlags;

extern _RomStateFlags RomStateFlags;


#define EEDataOriginAddr 0x0200 
#define LeftTimesL 0
#define LeftTimesM 1
#define LeftTimesH 2


#define BreakMyBlock() RomData_FillBytes(EEDataOriginAddr+0xa0,0,95)//破坏我们自己定义的数据存储区
#define InitButtonTimes() RomData_FillBytes(EEDataOriginAddr+0x90,0x01,4)

void GetKeyParam(void);
void GetDoorProc(uint8_t keyValue);
void GetKeyState(void);

void ReverseRom(uint8_t addr);
void GetLeftTimeBlock(uint8_t nBlock);
void ChangeRFState(uint8_t state);
void ChangeKeyState(uint8_t state);
void GetKeyWorkValue(uint8_t* rxList,uint8_t responseCommander);
void VerifyEEDatas(uint8_t maxNum,uint8_t lastAddr);
void GetCalcTimes69(void);
void CheckDataBlockVerify(uint8_t Addr);
void UsedDEC(void);
#endif /* EEP_H */
