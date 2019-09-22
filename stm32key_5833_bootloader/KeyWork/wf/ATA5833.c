#include "ATA5833.h"
#include "SimSPI.h"
#include "wfDefine.h"
#include "Variables.h"
#include "..\Inc\tim.h"
#include "AS3933.h"
#include "Function.h"
#include "Verify.h"
_ATA583X_ICDataStruct ATA583X_ICData[ATA583X_ICData_BufLen];
uint8_t ATA583X_SPITxList[20];
uint8_t ATA583X_SPIRxList[20];

uint8_t ATA583X_TxList[20];
uint32_t ATA583X_TxCount;
uint32_t ATA583X_RxTick;
uint8_t ATA583X_RxList[20];
uint32_t ATA583X_RxCount;
uint32_t ATA583X_ICDataIndex;

#define TMDODelay() wfDelay_us(49)
#define TMDORFDelay() wfDelay_us(489)
#define TMDI_Low() HAL_GPIO_WritePin(ATA583X_TMDI_GPIO_Port,ATA583X_TMDI_Pin,GPIO_PIN_RESET)
#define TMDI_High() HAL_GPIO_WritePin(ATA583X_TMDI_GPIO_Port,ATA583X_TMDI_Pin,GPIO_PIN_SET)

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
		{
			BAT_OFF();
			ledFlash(500);
		}
	}
}
void ATA583X_SPIProc(uint8_t len);
void ATA583X_Test(void)
{
	HAL_GPIO_WritePin(ATA583X_PWR_ON_GPIO_Port,ATA583X_PWR_ON_Pin,GPIO_PIN_SET);
	ATA583X_Init();
	while(1)
	{
		gFlags.Bits.ATA583X_bRxFrame=0;
		ATA583X_RxMode();
		while(!gFlags.Bits.ATA583X_bRxFrame);
		ATA583X_IDLEMode();
		ATA583X_FrameProc();//曼码解码
		wfDelay_ms(2);
	}
}
void ATA583X_FrameProc(void)
{
	uint32_t i;	
	uint8_t rxBitList[500],rxBitList1[500];
	uint32_t rxBitLen=0,rxBitLen1=0,nBit;
	uint32_t bData=0,bOdd=0;	
	ATA583X_RxCount=0;
	for(i=0;i<ATA583X_ICDataIndex;i++)
	{
		rxBitList[rxBitLen++]=ATA583X_ICData[i].pinState;
		if(ATA583X_ICData[i].time>60)//全位脉宽保存两个半位值
		{
			rxBitList[rxBitLen++]=ATA583X_ICData[i].pinState;
			if(bData==0)//判断出奇偶数项
			{
				if(i%2==0)
				{
					bOdd=1;//奇数项
				}
				else
				{
					bOdd=0;//偶数项
				}
				bData=1;
			}
		}
	}
	for(i=0;i<rxBitLen;i+=2)
	{
		if(bOdd==0)//偶数项
		{
			rxBitList1[rxBitLen1++]=rxBitList[i];
		}
		else
		{
			rxBitList1[rxBitLen1++]=rxBitList[i+1];
		}
	}
	bData=0;
	nBit=0;
	for(i=0;i<rxBitLen1;i++)
	{
		if(bData==0)//去掉同步头
		{
			if(rxBitList1[i]==1)
			{
				bData=1;
			}
		}
		else
		{
			if(rxBitList1[i]==1)
			{
				ATA583X_RxList[ATA583X_RxCount] |= 0x80;
			}
			else
			{
				ATA583X_RxList[ATA583X_RxCount] &= 0x7f;
			}
			nBit++;
			if(nBit>=8)
			{
				ATA583X_RxCount++;
				nBit=0;
			}
			else
			{
				ATA583X_RxList[ATA583X_RxCount] =ATA583X_RxList[ATA583X_RxCount] >>1;
			}
		}
	}
	//帧接收完成，等待处理
	//ATA583X_RxMode();
}
void ATA583X_ReadSRAM(uint32_t addr,uint8_t len,uint8_t* pdata)
{
	uint32_t i;
	ATA583X_SPITxList[0]=0x08;
	ATA583X_SPITxList[1]=len;
	ATA583X_SPITxList[2]=HIGH_BYTE(addr);
	ATA583X_SPITxList[3]=LOW_BYTE(addr);
	ATA583X_SPIProc(5+len);
	for(i=0;i<len;i++)
		pdata[i]=ATA583X_SPIRxList[5+i];
}
void ATA583X_WriteSRAM(uint32_t addr,uint8_t len,uint8_t* pdata)
{
	uint32_t i;
	ATA583X_SPITxList[0]=0x07;
	ATA583X_SPITxList[1]=len;
	ATA583X_SPITxList[2]=HIGH_BYTE(addr);
	ATA583X_SPITxList[3]=LOW_BYTE(addr);
	for(i=0;i<len;i++)
		ATA583X_SPITxList[4+i]=pdata[i];
	ATA583X_SPIProc(4+len);
}
uint8_t ATA583X_ReadEEPROM(uint32_t addr)
{
	ATA583X_SPITxList[0]=0x0a;
	ATA583X_SPITxList[1]=HIGH_BYTE(addr);
	ATA583X_SPITxList[2]=LOW_BYTE(addr);
	ATA583X_SPIProc(5);
	return ATA583X_SPIRxList[4];
}
void ATA583X_WirteEEPROM(uint32_t addr,uint8_t data)
{
	ATA583X_SPITxList[0]=0x09;
	ATA583X_SPITxList[1]=HIGH_BYTE(addr);
	ATA583X_SPITxList[2]=LOW_BYTE(addr);
	ATA583X_SPITxList[3]=LOW_BYTE(data);
	ATA583X_SPIProc(4);
	wfDelay_ms(10);
}
void ATA583X_IDLEMode()
{
	ATA583X_SPITxList[0]=0x0d;
	ATA583X_SPITxList[1]=0x10;
	ATA583X_SPITxList[2]=0x00;
	ATA583X_SPIProc(3);
}
void ATA583X_Reset()
{
	ATA583X_SPITxList[0]=0x15;
	ATA583X_SPITxList[1]=0x00;
	ATA583X_SPIProc(2);
}
void ATA583X_RxMode()
{
	ATA583X_SPITxList[0]=0x0d;
	//ATA583X_SPITxList[1]=0x36;
	//ATA583X_SPITxList[2]=0x43;
	ATA583X_SPITxList[1]=0x37;
	ATA583X_SPITxList[2]=0x00;
	ATA583X_SPIProc(3);
	gFlags.Bits.ATA583X_bRxFrame=0;	
	gFlags.Bits.ATA583X_bFirstIC=1;
	ATA583X_ICDataIndex=0;	
	TMDI_Low();
	__HAL_TIM_CLEAR_IT(&htim16, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&htim16, TIM_CHANNEL_1);	
	ATA583X_RxTick=HAL_GetTick();
}
void ATA583X_TxMode()
{
	ATA583X_SPITxList[0]=0x0d;
	ATA583X_SPITxList[1]=0x35;
	ATA583X_SPITxList[2]=0x43;
	ATA583X_SPIProc(3);	
}
void ATA583X_Check()
{
	ATA583X_SPITxList[0]=0x0e;
	ATA583X_SPITxList[1]=0x08;
	ATA583X_SPITxList[2]=0x43;
	ATA583X_SPIProc(3);
}
void ATA583X_ICProc(void)
{
	uint32_t read;
	if(gFlags.Bits.ATA583X_bFirstIC!=0)//第一次捕捉
	{
		gFlags.Bits.ATA583X_bFirstIC=0;
		htim16.Instance->CNT=0;
		__HAL_TIM_CLEAR_IT(&htim16, TIM_IT_UPDATE);
		HAL_TIM_Base_Start_IT(&htim16);	
	}
	else
	{
		read=HAL_TIM_ReadCapturedValue(&htim16,TIM_CHANNEL_1);//+13;
		htim16.Instance->CNT=0;
		ATA583X_ICData[ATA583X_ICDataIndex].time=read;
		ATA583X_ICData[ATA583X_ICDataIndex].pinState=HAL_GPIO_ReadPin(ATA583X_TMDO_GPIO_Port,ATA583X_TMDO_Pin);
		ATA583X_ICDataIndex++;		
	}
}
void ATA583X_TxFrameProc(void)
{
	uint32_t i,j;
	uint8_t x;
	uint32_t Pre0=12;
	ATA583X_TxMode();
	wfDelay_us(500);
	for(i=0;i<Pre0;i++)
	{
		TMDI_High();
		TMDODelay();
		TMDI_Low();
		TMDODelay();
	}
	TMDI_Low();
	TMDODelay();
	TMDI_High();
	TMDODelay();
	for(i=0;i<ATA583X_TxCount;i++)
	{
		x=ATA583X_TxList[i];
		for (j=0;j<8;j++)//先低位
		{
			if((x&0x01)==0x01)
			{
				TMDI_Low();
				TMDODelay();
				TMDI_High();
				TMDODelay();
			}
			else
			{
				TMDI_High();
				TMDODelay();
				TMDI_Low();
				TMDODelay();
			}
			x=x>>1;
		}
	}
	TMDI_Low();
	TMDODelay();
	TMDI_High();
	TMDODelay();
	TMDI_Low();
	TMDODelay();
	ATA583X_IDLEMode();
}
void ATA583X_RFTxFrameProc(void)
{
	uint32_t i,j;
	uint8_t x;
	uint32_t Pre0=80;
	ATA583X_TxMode();
	wfDelay_us(4000);
	for(i=0;i<Pre0;i++)
	{
		TMDI_High();
		TMDORFDelay();
		TMDI_Low();
		TMDORFDelay();
	}
	TMDI_Low();
	TMDORFDelay();
	TMDI_High();
	TMDORFDelay();
	for(i=0;i<IRTxCount;i++)
	{
		x=IRTxList[i];
		for (j=0;j<8;j++)//先低位
		{
			if((x&0x01)==0x01)
			{
				TMDI_Low();
				TMDORFDelay();
				TMDI_High();
				TMDORFDelay();
			}
			else
			{
				TMDI_High();
				TMDORFDelay();
				TMDI_Low();
				TMDORFDelay();
			}
			x=x>>1;
		}
	}
	TMDI_Low();
	TMDORFDelay();
	TMDI_High();
	TMDORFDelay();
	TMDI_Low();
	TMDORFDelay();
	ATA583X_IDLEMode();
}
void ATA583X_Init()
{	
	ATA583X_IDLEMode();
	ATA583X_Check();
 	CheckEEPSW();
	ATA583X_IDLEMode();
	wfDelay_ms(1);
}
void ATA583X_SPIProc(uint8_t len)
{
	uint8_t i;
	HAL_GPIO_WritePin(ATA583X_CS_GPIO_Port,ATA583X_CS_Pin,GPIO_PIN_RESET);
	wfDelay_us(8);
	for(i=0;i<len;i++)
	{
		ATA583X_SPIRxList[i]=SimSPI_Proc(ATA583X_SPITxList[i]);
	}
	HAL_GPIO_WritePin(ATA583X_CS_GPIO_Port,ATA583X_CS_Pin,GPIO_PIN_SET);
	wfDelay_us(8);
}

