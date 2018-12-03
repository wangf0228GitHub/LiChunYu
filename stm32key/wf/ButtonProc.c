#include "ButtonProc.h"
#include "Variables.h"
#include "wfDefine.h"
#include "wfEEPROM.h"
#include "wfSys.h"
#include "..\Inc\tim.h"
#include "OnCarProc.h"
#include "Function.h"
#include "lcyHash.h"

#define NoKey 0x0f
#define FindCarKey 0x0e
#define LockKey 0x0d
#define UnLockKey 0x0b
#define TailGateKey 0x07
#define Lock_UnLock_Key 0x09

uint8_t KeyData[8];
uint8_t keyValue;
uint8_t KeyTimes;
void GetKeyData(void);
void keyRFIRTx(void);
void keyRFTx(void);
void ReadButton(void)
{
	uint32_t x;
	curKeyStateFlags.bFindCar=bFindCarKey();
	curKeyStateFlags.bLock=bLockKey();
	curKeyStateFlags.bUnLock=bUnLockKey();
	curKeyStateFlags.bTailGate=bTailGateKey();
	if(curKeyStateFlags.keyValue==NoKey)
	{
		NVIC_SystemReset();
	}
	x=curKeyStateFlags.keyValue;
	wfDelay_ms(20);
	curKeyStateFlags.bFindCar=bFindCarKey();
	curKeyStateFlags.bLock=bLockKey();
	curKeyStateFlags.bUnLock=bUnLockKey();
	curKeyStateFlags.bTailGate=bTailGateKey();
	if(x!=curKeyStateFlags.keyValue)
	{
		NVIC_SystemReset();
	}
}
void ButtionProc(void)
{
	uint8_t i;	
	uint32_t li;
// 	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
// 	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
// 	ReadButton();	
// 	if(curKeyStateFlags.keyValue==Lock_UnLock_Key)
// 	{
// 		 ReverseRom(0x94);
// 		 while(1)
// 		 {
// 			 LED_ON();
// 			 wfDelay_ms(200);
// 			 LED_OFF();
// 			 wfDelay_ms(200);
// 		 }
// 	}
// 	switch(curKeyStateFlags.keyValue)
// 	{
// 	case  FindCarKey:
// 		keyValue=0x2b;
// 		break;
// 	case  LockKey:
// 		keyValue=0x23;
// 		break;
// 	case  UnLockKey:
// 		keyValue=RomData_ReadByte(0x94);
// 		if(keyValue!=0)
// 			keyValue=0x21;
// 		else
// 			keyValue=0x20;
// 		break;
// 	case  TailGateKey:
// 		keyValue=0x22;
// 		break;
// 	default:
// 		NVIC_SystemReset();
// 		break;
// 	}
	keyValue=0x23;
	BAT_ON();
	if(RomStateFlags.bRomWrited)//则生成发送数据
	{
		GetKeyData();
	}
	keyRFTx();
	oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
	ReadButton();
	if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
	{
		while(1);
		//NVIC_SystemReset();
	}
	wfDelay_ms(70);
	keyRFIRTx();
	if(keyValue!=0x2b)//寻车
	{
		//ad检查电压
	}
	BAT_OFF();
	oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
	ReadButton();
	if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
	{
		while(1);
		//NVIC_SystemReset();
	}
	/************************************************************************/
	/* 再处理按键长按                                                       */
	/************************************************************************/
	if((curKeyStateFlags.keyValue==UnLockKey) || (curKeyStateFlags.keyValue==LockKey))
	{
		for(li=0;li<0x0270;i++)
		{
			IRTxList[0]=0x30;
			IRTxCount=1;
			RFIRTxProc();
			wfDelay_ms(20);
			oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
			ReadButton();
			if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
			{
				while(1);
				//NVIC_SystemReset();
			}
		}
	}
	else 
	{
		for(i=0;i<30;i++)
		{
			oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
			ReadButton();
			if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
			{
				while(1);
			}				//	NVIC_SystemReset();
		}
		if(curKeyStateFlags.keyValue==FindCarKey)
		{
			keyValue=0x29;
		}
		else
		{
			keyValue=0x45;
		}
		BAT_ON();
		if(RomStateFlags.bRomWrited)//则生成发送数据
		{
			GetKeyData();
		}
		keyRFTx();
		oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
		ReadButton();
		if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
		{
			while(1);
			//NVIC_SystemReset();
		}
		wfDelay_ms(70);
		keyRFIRTx();
		BAT_OFF();
		while(1);
	}
}
void GetKeyData(void)
{
	uint8_t i,x,addr;
	uint8_t ram93[8];
	if(EE00==0)
		RomStateFlags.bStudy=1;
	if(!RomStateFlags.bStudy)
		keyValue=0x43;
	/************************************************************************/
	/* 生成要送的钥匙数据                                                   */
	/************************************************************************/
	//1.获得计数值
	addr=LeftTimes69&0x03;
	addr=addr+0x90;
	KeyTimes=RomData_ReadByte(addr);
	//2,加载ssid
	//3,当前hash滚一步
	for(i=0;i<8;i++)
		lcyHashIn[i]=curHash[i];
	lcyHashOnce();
	for(i=0;i<8;i++)
		ram93[i]=lcyHashOut[i];
	//4
	for(i=0;i<8;i++)
		KeyData[i]=ram93[i]^PSW[i];
	//5
	if(!RomStateFlags.bStudy)
	{	for(i=0;i<8;i++)
	KeyData[i]=PSW[i];
	}
	//6
	KeyData[0]^=ram93[0];
	KeyData[6]^=KeyTimes;
	KeyData[7]^=keyValue;
	for(i=0;i<8;i++)
		lcyHashIn[i]=KeyData[i];
	lcyHashOnce();
	//7
	x=KeyTimes+1;
	RomData_WriteByte(addr,x);
	//8
	if(RomStateFlags.bStudy)
	{
		for(i=0;i<8;i++)
			KeyData[i]=ram93[i]^lcyHashOut[i];
	}
	else
	{
		for(i=0;i<8;i++)
			KeyData[i]=lcyHashOut[i];
	}
}
void keyRFIRTx(void)
{
	uint8_t i;
	for(i=0;i<19;i++)
		IRTxList[i]=0x00;
	for(i=0;i<4;i++)
		IRTxList[i]=0x88;
	IRTxList[4]=0x08;
	IRTxList[5]=keyValue;
	if(RomStateFlags.bRomWrited)
	{
		IRTxList[6]=KeyTimes;
		for(i=0;i<4;i++)
			IRTxList[7+i]=SSID[i];
		for(i=0;i<8;i++)
			IRTxList[11+i]=KeyData[i];
	}
	IRTxCount=19;
	RFIRTxProc();
}
void keyRFTx(void)
{
	uint8_t i;
	for(i=0;i<19;i++)
		IRTxList[i]=0x00;
	if(RomStateFlags.bRomWrited)//则生成发送数据
	{
		//写入发送区
		IRTxList[5]=KeyTimes;
		for(i=0;i<4;i++)
			IRTxList[6+i]=SSID[i];
		for(i=0;i<8;i++)
			IRTxList[10+i]=KeyData[i];
	}
	IRTxList[3]=0x80;
	IRTxList[4]=keyValue;
	IRTxCount=18;
	RFTxProc();
}
void RFIRTxProc(void)
{
	uint32_t i,x;	
	TimWorkType=RFIRTx;
	gFlags.bTxFinish=0;
	for(i=0;i<IRTxCount;i++)
	{
		x=i<<1;
		IRTxDataList[x]=LOW_NIBBLE(IRTxList[i]);
		IRTxDataList[x+1]=HIGH_NIBBLE(IRTxList[i]);
	}
	IRTxCount=IRTxCount<<1;
	IRTxIndex=0;
	RFIRPulseTimes=0;
	htim2.Instance->ARR=16;
	htim2.Instance->CNT=0;
	HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);  
	while(gFlags.bTxFinish==0);
}
void RFTxProc(void)
{
// 	uint32_t i,j,x;	
// 	uint8_t b;
// 	RFEnable();
// 	TimWorkType=RFTx;
// 	gFlags.bTxFinish=0;
// 	for(i=0;i<IRTxCount;i++)
// 	{
// 		x=i<<1;
// 		b=LOW_NIBBLE(IRTxList[i]);
// 		IRTxDataList[x]=0;
// 		for(j=0;j<4;j++)
// 		{	
// 			IRTxDataList[x]=IRTxDataList[x]>>2;
// 			if((b&0x01)!=0x00)
// 			{
// 				IRTxDataList[x] |=0xc0;
// 			}
// 			b=b>>1;			
// 		}
// 		b=HIGH_NIBBLE(IRTxList[i]);
// 		IRTxDataList[x+1]=0;
// 		for(j=0;j<4;j++)
// 		{	
// 			IRTxDataList[x+1]=IRTxDataList[x+1]>>2;
// 			if((b&0x01)!=0x00)
// 			{
// 				IRTxDataList[x] |=0xc0;
// 			}
// 			b=b>>1;			
// 		}
// 		//if(GetRFType())//0:1->0:01,1:0->1:10
// 		{
// 			IRTxDataList[x]=IRTxDataList[x]^0x55;
// 			IRTxDataList[x+1]=IRTxDataList[x+1]^0x55;
// 		}
// // 		else
// // 		{
// // 			IRTxDataList[x]=IRTxDataList[x]^0xaa;
// // 			IRTxDataList[x+1]=IRTxDataList[x+1]^0xaa;
// // 		}
// 	}
// 	IRTxCount=IRTxCount<<1;
// 	IRTxIndex=0;
// 	RFTxBitIndex=1;
// 	if(GetBit(IRTxDataList[0],0))
// 	{
// 		RFDataHigh();
// 	}
// 	else
// 	{
// 		RFDataLow();
// 	}	
// 	htim2.Instance->ARR=500;//500us反转电平
// 	htim2.Instance->CNT=0;
// 	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
// 	HAL_TIM_Base_Start_IT(&htim2);  
// 	while(gFlags.bTxFinish==0);
// 	wfDelay_ms(1);
// 	RFDisable();
}



