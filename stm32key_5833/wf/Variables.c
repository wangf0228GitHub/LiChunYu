#include "Variables.h"
#include "wfEEPROM.h"
#include "..\..\..\WF_Device\SPIROM.h"
#include "..\..\..\WF_Device\wfDefine.h"
#include "ATA5833.h"
#include "..\..\..\WF_Device\lcyHash.h"
volatile _TimWorkType TimWorkType;
volatile _gFlags gFlags;
volatile _RomStateFlags RomStateFlags;
volatile uint32_t gTempIndex;
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
uint8_t RFKeyValue;

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
_EEPSWOriginal EEPSWOriginal;
uint8_t EEPSW[16];
uint32_t EEPSWAddr;
const uint8_t eeprom_8E[8]=
{	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01
};
// const uint32_t PSWAddr[16]=
// {
// 	0x0280,0x0281,0x0282,0x0283,0x0284,0x0285,0x0286,0x0287,
// 	0x0290,0x0291,0x0292,0x0293,0x0294,0x0295,0x0296,0x0297,
// };
void ledFlash(uint32_t ms)
{
	while(1)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		HAL_Delay(ms);
	}
}
void GenerateEEPSW()//4ms
{
	uint8_t i,sum;
	EEPSWOriginal.u32.psw1 = *(uint32_t *) PSW_ID1;
	EEPSWOriginal.u32.psw2 = *(uint32_t *) PSW_ID2;
	EEPSWOriginal.u32.psw3 = *(uint32_t *) PSW_ID3;
	EEPSWOriginal.u32.psw4=PSW_ID4;
	for(i=0;i<8;i++)
		lcyHashIn[i]=EEPSWOriginal.psw[i];
	lcyHashOnce();//2ms
	for(i=0;i<8;i++)
		EEPSW[i]=lcyHashOut[i];
	for(i=0;i<8;i++)
		lcyHashIn[i]=EEPSWOriginal.psw[8+i];
	lcyHashOnce();
	for(i=0;i<8;i++)
		EEPSW[8+i]=lcyHashOut[i];
	for(i=0;i<16;i++)
		EEPSW[i]=EEPSW[i]^EEPSWOriginal.psw[15-i];
	sum=0;
	for(i=0;i<16;i++)
	{
		sum=(uint8_t)(sum+EEPSWOriginal.psw[i]);
	}
	EEPSWAddr=0x0280+sum;
}
void ReadEEPSW()//8ms
{
	uint32_t i;
	uint8_t sum;
	EEPSWOriginal.u32.psw1 = *(uint32_t *) PSW_ID1;
	EEPSWOriginal.u32.psw2 = *(uint32_t *) PSW_ID2;
	EEPSWOriginal.u32.psw3 = *(uint32_t *) PSW_ID3;
	EEPSWOriginal.u32.psw4=PSW_ID4;
	sum=0;
	for(i=0;i<16;i++)
	{
		sum=(uint8_t)(sum+EEPSWOriginal.psw[i]);
	}
	EEPSWAddr=0x0280+sum;
	for(i=0;i<16;i++)
		EEPSW[i]=ATA583X_ReadEEPROM(EEPSWAddr+i);
}
void CheckEEPSW()//11.5ms-4
{
	uint32_t i;
	uint8_t ataPSW[16];
	//GenerateEEPSW();
	for(i=0;i<16;i++)
		ataPSW[i]=ATA583X_ReadEEPROM(EEPSWAddr+i);
	for(i=0;i<16;i++)
	{
		if(ataPSW[i]!=EEPSW[i])
			ledFlash(500);
	}
}
uint8_t RomData_ReadByte(uint32_t addr)
{
	uint8_t ret,index;
	ret= SPIROM_ReadByte(addr);
	index=LOW_NIBBLE(addr);
	return ret^EEPSW[index];
}
void RomData_ReadBytes(uint32_t Addr,uint8_t *Buffer,uint32_t Length) 
{  
	uint8_t i,index;
	SPIROM_ReadArray(Addr,Buffer,Length);
 	for(i=0;i<Length;i++)
 	{
 		index=LOW_NIBBLE(Addr+i);
 		Buffer[i]=Buffer[i]^EEPSW[index];
 	}	
}
void RomData_WriteByte( uint32_t addr,uint8_t x )
{
	uint8_t index;
 	index=LOW_NIBBLE(addr);
 	x=x^EEPSW[index];
	if(bBATON()==GPIO_PIN_SET)
	{
		gFlags.Bits.bOldBAT=1;
	}
	else
	{
		gFlags.Bits.bOldBAT=0;
		BAT_ON();
	}
	SPIROM_WriteByte(addr,x);
	if(!gFlags.Bits.bOldBAT)//之前电池没有打开，则关闭
	{	
		BAT_OFF();
	}
}

void RomData_WriteBytes( uint32_t addr,uint8_t *pBuffer,uint32_t NumToWrite )
{
	uint8_t i,index;
	uint8_t data[20];
 	for(i=0;i<NumToWrite;i++)
 	{
 		index=LOW_NIBBLE(addr+i);
 		data[i]=pBuffer[i]^EEPSW[index];
 	}
	if(bBATON()==GPIO_PIN_SET)
	{
		gFlags.Bits.bOldBAT=1;
	}
	else
	{
		gFlags.Bits.bOldBAT=0;
		BAT_ON();
	}
	//SPIROM_WriteArray(addr,pBuffer,NumToWrite);
	SPIROM_WriteArray(addr,data,NumToWrite);
 	if(!gFlags.Bits.bOldBAT)//之前电池没有打开，则关闭
 	{	
 		BAT_OFF();
 	}
}
void wfFlash_ReadDoubleWords(uint32_t Addr,uint64_t *Buffer,uint32_t Length)  
{  
	uint64_t *wAddr;  
	wAddr=(uint64_t*)(wfFlash_BASE_ADDR+Addr);  
	while(Length--)
	{  
		*Buffer++=*wAddr++;  
	}     
}
void STMFLASH_Write(uint32_t WriteAddr,uint64_t *pBuffer,uint32_t NumToWrite)	
{ 
	uint32_t i;
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus=HAL_OK;
	uint32_t SectorError=0;
	HAL_FLASH_Unlock();             //解锁	
	FlashEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;       //擦除类型，页擦除 
	FlashEraseInit.Page=63;   //要擦除的页，即写入地址
	FlashEraseInit.NbPages=1;               //只擦除一页
	if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK) 
	{
		HAL_FLASH_Lock();           //上锁
		return;//发生错误了	
	}	
	for(i=0;i<NumToWrite;i++)
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,wfFlash_BASE_ADDR+WriteAddr,*pBuffer)!=HAL_OK)//写入数据
		{ 
			break;	//写入异常
		}
		WriteAddr+=8;
		pBuffer++;
	}
	HAL_FLASH_Lock();           //上锁
} 

