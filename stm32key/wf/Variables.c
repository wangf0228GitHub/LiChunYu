#include "Variables.h"
#include "wfEEPROM.h"
_TimWorkType TimWorkType;
_gFlags gFlags;
_RomStateFlags RomStateFlags;
uint8_t WorkValueDatas[8];
uint8_t DoorDatas[8];
uint8_t ButtonTimes;
uint8_t RomDatas[16];
uint8_t LeftTimesAddr[3];//使用的段地址
uint8_t OtherLeftTimesAddr[3];//未使用的段地址
uint8_t LeftTimes[3];
uint8_t LeftTimes69Addr;
uint8_t curHash[8];
uint8_t EISHash[8];//curHash滚一步
uint8_t LeftTimes69;
uint8_t SSID[4];
uint8_t PSW[8];
uint8_t EE00;
uint8_t EE9e;

uint8_t IRTxList[50];
uint8_t IRTxDataList[100];
uint32_t IRTxCount;
uint32_t IRTxIndex;
uint32_t RFTxBitIndex;

uint32_t RFIRPulseTimes;

uint32_t IRRxByteL;
uint32_t IRRxByteH;
uint8_t IRRxList[100];
uint32_t IRRxCount;
uint32_t IRRxNeedCount;
uint32_t IRRxTick;

_KeyStateFlags curKeyStateFlags;
_KeyStateFlags oldKeyStateFlags;

const uint8_t eeprom_8E[8]=
{	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01
};
void RomData_WriteByte( uint32_t addr,uint8_t x )
{
	if(bBATON()==GPIO_PIN_SET)
	{
		gFlags.bOldBAT=1;
	}
	else
	{
		gFlags.bOldBAT=0;
		BAT_ON();
	}
	wfEEPROM_WriteByte(addr,x);
	if(!gFlags.bOldBAT)//之前电池没有打开，则关闭
	{	
		BAT_OFF();
	}
}

void RomData_WriteBytes( uint32_t addr,uint8_t *pBuffer,uint32_t NumToWrite )
{
	if(bBATON()==GPIO_PIN_SET)
	{
		gFlags.bOldBAT=1;
	}
	else
	{
		gFlags.bOldBAT=0;
		BAT_ON();
	}
	wfEEPROM_WriteBytes(addr,pBuffer,NumToWrite);
 	if(!gFlags.bOldBAT)//之前电池没有打开，则关闭
 	{	
 		BAT_OFF();
 	}
}
