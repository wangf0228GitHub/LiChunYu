#include "Variables.h"
#include "..\..\..\WF_Device\stm32\wfEEPROM.h"
_gFlags gFlags;
_RomStateFlags RomStateFlags;
uint8_t RomDatas[16];
uint8_t LeftTimesAddr[3];//使用的段地址
uint8_t OtherLeftTimesAddr[3];//未使用的段地址
uint8_t LeftTimes[3];
uint8_t LeftTimes69Addr;
uint8_t curHash[8];
uint8_t LeftTimes69;
uint8_t SSID[4];
uint8_t PSW[8];
uint8_t EE00;

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
