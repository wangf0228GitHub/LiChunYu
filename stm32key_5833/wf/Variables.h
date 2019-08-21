#ifndef   __Variables_h__
#define   __Variables_h__

#include <stdint.h>
#include "main.h"


#define         PSW_ID1          (UID_BASE)
#define         PSW_ID2          (UID_BASE + 0x4)
#define         PSW_ID3          (UID_BASE + 0x8)
#define         PSW_ID4          0x62795746

#define FlashPage 31
#define wfFlash_BASE_ADDR 0x0800f800 //2048*31
#define MAXRENEW 50
typedef union 
{	
	struct
	{
		uint32_t psw1;
		uint32_t psw2;
		uint32_t psw3;
		uint32_t psw4;
	}u32;
	uint8_t psw[16];
}_EEPSWOriginal;

typedef union 
{	
	uint64_t data;
	uint8_t dataList[8];
}_RenewTimes;

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

		unsigned ATA583X_bFirstIC:1;
		unsigned ATA583X_bRxFrame:1;

		unsigned b3933Wake:1;
	}Bits;
	uint32_t all;
}_gFlags;

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
	}Bits;
	uint32_t keyValue;
}_KeyStateFlags;

typedef union
{	
	struct
	{
		unsigned bZuoHou:1;
		unsigned bYouHou:1;
		unsigned bJiaShiShi:1;
		unsigned bCheNei:1;
	}Bits;
	uint32_t antValue;
}_ANTFlags;
extern _ANTFlags ANTFlags;
extern uint32_t ANTCount;
typedef enum
{
	CarIRRx = 0U,
	CarIRTx,
	RFIRTx,
	RFTx
}_TimWorkType;
extern volatile uint32_t gTempIndex;
extern volatile _TimWorkType TimWorkType;
extern volatile _gFlags gFlags;
extern volatile _RomStateFlags RomStateFlags;
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
extern uint8_t RFKeyValue;

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
uint8_t RomData_ReadByte(uint32_t addr);
void RomData_ReadBytes(uint32_t Addr,uint8_t *Buffer,uint32_t Length);
void RomData_WriteByte(uint32_t WriteAddr,uint8_t x);
void RomData_WriteBytes(uint32_t WriteAddr,uint8_t *pBuffer,uint32_t NumToWrite);
void GenerateEEPSW(void);
void ledFlash(uint32_t ms);
void CheckEEPSW(void);
void ReadEEPSW(void)/*48ms */;
void wfFlash_ReadDoubleWords(uint32_t Addr,uint64_t *Buffer,uint32_t Length);
void STMFLASH_Write(uint32_t WriteAddr,uint64_t *pBuffer,uint32_t NumToWrite);
extern _EEPSWOriginal EEPSWOriginal;
extern uint8_t EEPSW[16];
extern uint32_t EEPSWAddr;
#endif
