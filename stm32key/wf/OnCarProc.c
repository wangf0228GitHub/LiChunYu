#include "OnCarProc.h"
#include "Function.h"
#include "Variables.h"
#include "wfEEPROM.h"
#include "IRProc.h"
#include "Verify.h"
#include "wfDefine.h"
#include "lcyHash.h"

void OnCarProc(void)
{
	uint8_t i;
	while (1)
	{
		GetKeyParam();
		UsedDEC();
	}
	GetKeyParam();
	if(!RomStateFlags.bRomWrited)//ROM未被烧写过
	{
		while(1)
		{
			IRTx2425Frame();
			IRRxProc(100);
			if(gFlags.bFuncRet)//收到数据帧
			{
				IRRxDataProc();
				if(gFlags.bIRRxErr)//发生接收错误，重来
					continue;
				switch(IRRxList[1])
				{
				case 0x26:
					ProcCommand_26();
					if(gFlags.bFuncRet)
					{
						if(!RomStateFlags.bStudy)
						{
							IRRxProc(100);
							if(IRRxList[1]==0x39)
							{
								ProcCommand_39();
								if(!gFlags.bFuncRet)
									continue;
							}
						}
						//开掉电中断
						for(i=0;i<4;i++)
							RomDatas[i]=0x01;
						RomData_WriteBytes(0x90,RomDatas,4);						
						//关掉电中断
						if(bBATON())
						{
							BAT_OFF();
							HAL_Delay(100);
							if(bOnCarPower()==GPIO_PIN_SET)
								NVIC_SystemReset();//复位							
						}
						else
						{							
							NVIC_SystemReset();//复位
						}
					}
					break;
				case 0x7a:
					break;
				}
			}
		}
	}
}
void ProcCommand_26(void)//回应0x27指令
{
	uint8_t i;
	gFlags.bFuncRet=0;	
	if(IRRxCount!=10)
		return;
	if(IRRxList[2]==0x00)
		return;
	for(i=0;i<8;i++)//密码与接收到的数据逐个异或
	{
		RomDatas[i]=PSW[i]^IRRxList[2+i];
	}
	//后4字节与2425指令发送数据异或
	RomDatas[4]^=LeftTimes69;
	RomDatas[5]^=LeftTimes[LeftTimesM];
	RomDatas[6]^=LeftTimes[LeftTimesH];
	if(RomStateFlags.bStudy)
		RomDatas[7]^=0x24;
	else
		RomDatas[7]^=0x25;	
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=RomDatas[i];
	}
	lcyHashOnce();	
	/************************************************************************/
	/* 使用次数减一                                                         */
	/************************************************************************/
	UsedDEC();	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	for(i=0;i<8;i++)
	{
		IRTxList[2+i]=lcyHashOut[i]^curHash[i];
	}
	IRTxList[0]=0x10;
	IRTxList[1]=0x27;
	IRTxCount=9;
	IRTxProc();
	gFlags.bFuncRet=1;
}
void ProcCommand_39(void)//钥匙学习过程
{
	uint8_t i;
	gFlags.bFuncRet=0;	
	if(IRRxCount!=10)
		return;
	if(IRRxList[2]==0x00)
		return;
	BAT_ON();
	//密码移动一位后记载:psw8,psw1,psw2,....,psw7
	RomData_ReadBytes(0x01,&RomDatas[1],8);
	RomDatas[0]=RomDatas[8];
	//加载SSID后异或
	for(i=0;i<4;i++)
	{
		RomDatas[i]=RomDatas[i]^SSID[i];
	}
	RomDatas[4]=RomDatas[4]^EE00;

	RomDatas[5] ^= curHash[7];
	RomDatas[6] ^= curHash[6];
	RomDatas[7] ^= curHash[5];

	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=RomDatas[i];
	}
	lcyHashOnce();
	//若与0x39指令所发送数据不一致，则学习失败
	for(i=0;i<8;i++)
	{
		if(lcyHashOut[i]!=IRRxList[2+i])
		{
			BAT_OFF();
			return;
		}
	}
	//更改存储区为学习过状态
	//密码移动2位后记载:psw7,psw8,psw1,psw2,....,psw6
	RomData_ReadBytes(0x01,&RomDatas[2],8);
	RomDatas[0]=RomDatas[6];
	RomDatas[1]=RomDatas[7];
	//加载SSID后异或
	for(i=0;i<4;i++)
	{
		RomDatas[i]=RomDatas[i]^SSID[i];
	}
	RomDatas[4]=RomDatas[4]^EE00;
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=RomDatas[i];
	}
	lcyHashOnce();
	for(i=0;i<8;i++)
	{
		RomDatas[i]=lcyHashOut[i];
	}
	for(i=0;i<4;i++)
	{
		RomDatas[4+i]=SSID[i];
	}
	RomStateFlags.bStudy=1;
	RomData_WriteBytes(0x88,RomDatas,8);
	RomDatas[0]=RomData_ReadByte(0x9e);
	SetBit_uint8(RomDatas[0],4);
	RomDatas[1]=0x00-RomDatas[0];
	RomData_WriteBytes(0x9e,RomDatas,2);
	BAT_OFF();
	gFlags.bFuncRet=1;
}
void IRTx2425Frame(void)
{
	IRTxList[0]=0x10;
	if(RomStateFlags.bStudy)
		IRTxList[1]=0x24;//仅写过
	else
		IRTxList[1]=0x25;//学习过

	IRTxList[2]=LeftTimes[LeftTimes69];
	IRTxList[3]=LeftTimes[LeftTimesM];
	IRTxList[4]=LeftTimes[LeftTimesH];

	RomData_ReadBytes(0x09,&IRTxList[5],4);
	IRTxCount=9;
	IRTxProc();
}

