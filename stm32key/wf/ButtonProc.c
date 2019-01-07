#include "ButtonProc.h"
#include "Variables.h"
#include "wfDefine.h"
#include "wfEEPROM.h"
#include "wfSys.h"
#include "..\Inc\tim.h"
#include "OnCarProc.h"
#include "Function.h"
#include "lcyHash.h"
#include "ATA5824.h"



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
		return;
		//NVIC_SystemReset();
	}
	x=curKeyStateFlags.keyValue;
	wfDelay_ms(20);
	curKeyStateFlags.bFindCar=bFindCarKey();
	curKeyStateFlags.bLock=bLockKey();
	curKeyStateFlags.bUnLock=bUnLockKey();
	curKeyStateFlags.bTailGate=bTailGateKey();
	if(x!=curKeyStateFlags.keyValue)
	{
		return;
		//NVIC_SystemReset();
	}
}
void ButtionProc(void)
{
	uint8_t i,addr,x;	
	uint32_t li;
//  	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
//  	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
 	//ReadButton();	
 	if(curKeyStateFlags.keyValue==Lock_UnLock_Key)
 	{
 		 ReverseRom(0x94);
 		 while(1)
 		 {
 			 LED_ON();
 			 wfDelay_ms(200);
 			 LED_OFF();
 			 wfDelay_ms(200);
 		 }
 	}
 	switch(curKeyStateFlags.keyValue)
 	{
 	case  FindCarKey:
 		keyValue=0x2b;
 		break;
 	case  LockKey:
 		keyValue=0x23;
 		break;
 	case  UnLockKey:
 		keyValue=RomData_ReadByte(0x94);
 		if(keyValue!=0)
 			keyValue=0x21;
 		else
 			keyValue=0x20;
 		break;
 	case  TailGateKey:
 		keyValue=0x22;
 		break;
 	default:
 		NVIC_SystemReset();
 		break;
 	}
	BAT_ON();
	ATA5824_RFInit();
	GetKeyState();
	GetKeyParam();//获得钥匙当前相关数据		
	if(curKeyStateFlags.keyValue==TailGateKey || curKeyStateFlags.keyValue==FindCarKey)//后备箱没有短按
	{
		if(curKeyStateFlags.keyValue==FindCarKey)
		{
			keyValue=0x29;
		}
		else
		{
			keyValue=0x45;
		}
		if(RomStateFlags.bRomWrited)//则生成发送数据
		{
			GetDoorProc(keyValue);
			/************************************************************************/
			/*  修改按键次数                                                        */
			/************************************************************************/
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			x=RomData_ReadByte(addr);
			x++;
			RomData_WriteByte(addr,x);
		}
		keyRFTx();
		wfDelay_ms(150);
		keyRFTx();
		wfDelay_ms(112);
		oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
		ReadButton();
		if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
		{
			BAT_OFF();
			while(1);
		}
		if(curKeyStateFlags.keyValue==FindCarKey)
		{
			keyValue=0x2b;
		}
		else
		{
			keyValue=0x22;
		}
		if(RomStateFlags.bRomWrited)//则生成发送数据
		{
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			ButtonTimes=RomData_ReadByte(addr);
			GetDoorProc(keyValue);
			/************************************************************************/
			/*  修改按键次数                                                        */
			/************************************************************************/
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			x=RomData_ReadByte(addr);
			x++;
			RomData_WriteByte(addr,x);
		}
		keyRFTx();
		wfDelay_ms(95);
		keyRFIRTx();
		wfDelay_ms(95);	
		keyRFTx();
		BAT_OFF();
		while(1);
	}
	else
	{
		if(RomStateFlags.bRomWrited)//则生成发送数据
		{
			GetDoorProc(keyValue);
			/************************************************************************/
			/*  修改按键次数                                                        */
			/************************************************************************/
			addr=LeftTimes69&0x03;
			addr=addr+0x90;
			x=RomData_ReadByte(addr);
			x++;
			RomData_WriteByte(addr,x);
		}
		keyRFTx();
		//oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
		// 	ReadButton();
		// 	if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
		// 	{
		// 		while(1);
		// 		//NVIC_SystemReset();
		// 	}
		wfDelay_ms(95);
		keyRFIRTx();

		if(keyValue!=0x2b)//寻车
		{
			//ad检查电压
		}	
		oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
		ReadButton();	
		if(curKeyStateFlags.keyValue==NoKey)//按键变化则复位
		{
			wfDelay_ms(95);	
			keyRFTx();
			BAT_OFF();
			while(1);
		}
		else if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
		{
			BAT_OFF();
			while(1);
			//NVIC_SystemReset();
		}
		BAT_OFF();
		/************************************************************************/
		/* 再处理按键长按                                                       */
		/************************************************************************/
		if((curKeyStateFlags.keyValue==UnLockKey) || (curKeyStateFlags.keyValue==LockKey))
		{
			wfDelay_ms(16);
			for(li=0;li<0x0270;i++)
			{
				IRTxList[0]=0x30;
				IRTxCount=1;
				RFIRTxProc();
				wfDelay_ms(15);
				oldKeyStateFlags.keyValue=curKeyStateFlags.keyValue;
				ReadButton();
				if(curKeyStateFlags.keyValue!=oldKeyStateFlags.keyValue)//按键变化则复位
				{
					while(1);
					//NVIC_SystemReset();
				}
			}
		}
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
		IRTxList[6]=ButtonTimes;
		for(i=0;i<4;i++)
			IRTxList[7+i]=SSID[i];
		for(i=0;i<8;i++)
			IRTxList[11+i]=DoorDatas[i];
	}
	IRTxCount=19;
	RFIRTxProc();
}
void keyRFTx(void)
{
	uint8_t i;
	for(i=0;i<19;i++)
		IRTxList[i]=0x00;
	//IRTxList[3]=0x80;
	IRTxList[0]=keyValue;
	if(RomStateFlags.bRomWrited)//则生成发送数据
	{
		//写入发送区
		IRTxList[1]=ButtonTimes;
		for(i=0;i<4;i++)
			IRTxList[2+i]=SSID[i];
		for(i=0;i<8;i++)
			IRTxList[6+i]=DoorDatas[i];
	}	
	IRTxCount=14;
	ATA5824_RFTxFrameProc();
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

// void RFTxProc(void)
// {
//  	uint32_t i,j,x;	
//  	uint8_t b;
//  	ATA5824_RFInit();
//  	TimWorkType=RFTx;
//  	gFlags.bTxFinish=0;
//  	for(i=0;i<IRTxCount;i++)
//  	{
//  		x=i<<1;
//  		b=LOW_NIBBLE(IRTxList[i]);
//  		IRTxDataList[x]=0;
//  		for(j=0;j<4;j++)
//  		{	
//  			IRTxDataList[x]=IRTxDataList[x]>>2;
//  			if((b&0x01)!=0x00)
//  			{
//  				IRTxDataList[x] |=0xc0;
//  			}
//  			b=b>>1;			
//  		}
//  		b=HIGH_NIBBLE(IRTxList[i]);
//  		IRTxDataList[x+1]=0;
//  		for(j=0;j<4;j++)
//  		{	
//  			IRTxDataList[x+1]=IRTxDataList[x+1]>>2;
//  			if((b&0x01)!=0x00)
//  			{
//  				IRTxDataList[x] |=0xc0;
//  			}
//  			b=b>>1;			
//  		}
//  		//if(GetRFType())//0:1->0:01,1:0->1:10
//  		{
//  			IRTxDataList[x]=IRTxDataList[x]^0x55;
//  			IRTxDataList[x+1]=IRTxDataList[x+1]^0x55;
//  		}
//  // 		else
//  // 		{
//  // 			IRTxDataList[x]=IRTxDataList[x]^0xaa;
//  // 			IRTxDataList[x+1]=IRTxDataList[x+1]^0xaa;
//  // 		}
//  	}
//  	IRTxCount=IRTxCount<<1;
//  	IRTxIndex=0;
//  	RFTxBitIndex=1;
//  	if(GetBit(IRTxDataList[0],0))
//  	{
//  		//RFDataHigh();
//  	}
//  	else
//  	{
//  		//RFDataLow();
//  	}	
//  	htim2.Instance->ARR=500;//500us反转电平
//  	htim2.Instance->CNT=0;
//  	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
//  	HAL_TIM_Base_Start_IT(&htim2);  
//  	while(gFlags.bTxFinish==0);
//  	wfDelay_ms(1);
//  	//RFDisable();
// }



