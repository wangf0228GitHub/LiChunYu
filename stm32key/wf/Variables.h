#ifndef   __Variables_h__
#define   __Variables_h__

#include <stdint.h>
#include "main.h"

typedef union
{	
	struct
	{
		unsigned bCarIR:1;
		unsigned bTxFinish:1;
		unsigned bIRRxFrame:1;
		unsigned bFirstIC:1;
		unsigned bOldBAT:1;
		unsigned bFuncRet:1;
		unsigned bIRRxH:1;

		unsigned ATA5824_bFirstIC:1;
		unsigned ATA5824_bRxFrame:1;

		unsigned b3933Wake:1;
	};
	uint32_t all;
}_gFlags;

typedef union
{	
	struct
	{
		unsigned bRomWrited:1;
		unsigned bStudy:1;

		unsigned b55:1;
		unsigned b5f:1;
		unsigned b41:1;
		unsigned b4b:1;
		unsigned b2d:1;
		unsigned b37:1;	

		unsigned bH0:1;
		unsigned bM0:1;
		unsigned bL0:1;
	};
	uint32_t all;
}_RomStateFlags;

typedef union
{	
	struct
	{
		unsigned bFindCar:1;
		unsigned bLock:1;
		unsigned bUnLock:1;
		unsigned bTailGate:1;
	};
	uint32_t keyValue;
}_KeyStateFlags;

typedef enum
{
	CarIRRx = 0U,
	CarIRTx,
	RFIRTx,
	RFTx
}_TimWorkType;

extern _TimWorkType TimWorkType;
extern _gFlags gFlags;
extern _RomStateFlags RomStateFlags;
extern uint8_t WorkValueDatas[8];
extern uint8_t DoorDatas[8];
extern uint8_t ButtonTimes;
extern uint8_t RomDatas[16];
extern uint8_t LeftTimesAddr[3];//使用的段地址
extern uint8_t OtherLeftTimesAddr[3];//未使用的段地址
extern uint8_t LeftTimes[3];//c2,c3,c4
extern uint8_t LeftTimes69Addr;
extern uint8_t LeftTimes69;
extern uint8_t curHash[8];
extern uint8_t EISHash[8];//curHash滚一步
extern uint8_t SSID[4];
extern uint8_t PSW[8];
extern uint8_t EE00;
extern uint8_t EE9e;
extern uint32_t IRRxTick;

extern uint8_t IRTxList[50];
extern uint8_t IRTxDataList[100];
extern uint32_t IRTxCount;
extern uint32_t IRTxIndex;
extern uint32_t RFTxBitIndex;
extern uint32_t RFIRPulseTimes;

extern uint32_t IRRxByteL;
extern uint32_t IRRxByteH;
extern uint8_t IRRxList[100];
extern uint32_t IRRxCount;
extern uint32_t IRRxNeedCount;

extern _KeyStateFlags curKeyStateFlags;
extern _KeyStateFlags oldKeyStateFlags;

extern const uint8_t eeprom_8E[8];
void RomData_WriteByte(uint32_t WriteAddr,uint8_t x);
void RomData_WriteBytes(uint32_t WriteAddr,uint8_t *pBuffer,uint32_t NumToWrite);
#endif
