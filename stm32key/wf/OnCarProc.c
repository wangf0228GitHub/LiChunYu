#include "OnCarProc.h"
#include "Function.h"
#include "Variables.h"
#include "wfEEPROM.h"
#include "IRProc.h"
#include "Verify.h"
#include "wfDefine.h"
#include "lcyIRDecode.h"
#include "lcyHash.h"

void OnCarProc(void)
{
	uint8_t i;
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
						Check55and5fdata();
						Fix41and4Bdata();
						Fix2Dand37data();
						if(!(RomStateFlags.b55 && RomStateFlags.b5f))
						{
							if(CalcTimes_BF==0)
							{
								UpdateStepDatas();
							}
						}
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
							Check55and5fdata();
							Fix41and4Bdata();
							Fix2Dand37data();
							UpdateStepDatas();
							Adjust41and4BData();
							Adjust2Dand37Data();
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
	uint8_t i,x;
	gFlags.bFuncRet=0;	
	if(IRRxCount!=10)
		return;
	if(IRRxList[2]==0x00)
		return;
	RomData_ReadBytes(0x01,RomDatas,8);//加载密码
	for(i=0;i<8;i++)//与接收到的数据逐个异或
	{
		RomDatas[i]=RomDatas[i]^IRRxList[2+i];
	}
	for(i=0;i<4;i++)//后4个字节,与24指令发送参数异或
	{
		RomDatas[4+i]=RomDatas[4+i]^Com24DataBak[i];
	}
	/************************************************************************/
	/* 使用次数减一                                                         */
	/************************************************************************/
	if(CalcTimes_D2<0x80)// 无需借位
	{
		x=LeftTimes[0]&0x03;
		if(x==0)
		{
			ReverseRom(LeftTimesAddr[0]);//切换使用低位使用段
		}
		UsedDEC();
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	for(i=0;i<8;i++)
	{
		lcyIRDecodeIn[i]=RomDatas[i];
	}
	lcyHashOnce();
	for(i=0;i<8;i++)
	{
		IRTxList[2+i]=lcyIRDecodeOut[i]^curHash[i];
	}
	IRTxList[0]=0x10;
	IRTxList[1]=0x27;
	IRTxCount=9;
	IRTxProc();
	gFlags.bFuncRet=1;
}
void ProcCommand_39(void)//钥匙学习过程
{
	uint8_t i,x;
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
		x=RomData_ReadByte(0x09+i);
		RomDatas[i]=RomDatas[i]^x;
	}
	x=RomData_ReadByte(0x00);
	x=x>>6;
	RomDatas[4]=RomDatas[4]^x;

	RomDatas[5] ^= curHash[7];
	RomDatas[6] ^= curHash[6];
	RomDatas[7] ^= curHash[5];

	for(i=0;i<8;i++)
	{
		lcyIRDecodeIn[i]=RomDatas[i];
	}
	lcyHashOnce();
	//若与0x39指令所发送数据不一致，则学习失败
	for(i=0;i<8;i++)
	{
		lcyIRDecodeOut[i]!=IRRxList[2+i];
		BAT_OFF();
		return;
	}
	//更改存储区为学习过状态
	//密码移动2位后记载:psw7,psw8,psw1,psw2,....,psw6
	RomData_ReadBytes(0x01,&RomDatas[2],8);
	RomDatas[0]=RomDatas[6];
	RomDatas[1]=RomDatas[7];
	//加载SSID后异或
	for(i=0;i<4;i++)
	{
		x=RomData_ReadByte(0x09+i);
		RomDatas[i]=RomDatas[i]^x;
	}
	x=RomData_ReadByte(0x00);
	x=x>>6;
	RomDatas[4]=RomDatas[4]^x;
	for(i=0;i<8;i++)
	{
		lcyIRDecodeIn[i]=RomDatas[i];
	}
	lcyHashOnce();
	for(i=0;i<8;i++)
	{
		RomDatas[i]=lcyIRDecodeOut[i];
	}
	RomData_ReadBytes(0x09,&RomDatas[4],4);
	RomStateFlags.bRomWrited=1;
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
	uint8_t i,x;
	Get2425TxParam();
	IRTxList[0]=0x10;
	x=RomData_ReadByte(0x9e);
	if(GetBit(x,4))
		IRTxList[1]=0x24;//仅写过
	else
		IRTxList[1]=0x25;//学习过
	Com24DataBak[3]=IRTxList[1];
	for(i=0;i<3;i++)
	{
		IRTxList[2+i]=LeftTimes[i];
		Com24DataBak[i]=IRTxList[2+i];
	}
	RomData_ReadBytes(0x09,&IRTxList[5],4);
	IRTxCount=9;
	IRTxProc();
}
void Get2425TxParam(void)
{
	uint8_t i,x;
	Check55and5fdata();
	Fix41and4Bdata();
	Fix2Dand37data();
	if(!RomStateFlags.b55 && !RomStateFlags.b5f)//55、5F段均不匹配
	{
		if(!RomStateFlags.b41 && !RomStateFlags.b4b)//41、4B段全不匹配
		{
			//掉电中断
		}
		//sub_1490
		//容错rom中的数据
		UpdateStepDatas();
		if(bBATON()==GPIO_PIN_RESET)
		{
			Check55and5fdata();
			Fix41and4Bdata();
			Fix2Dand37data();
		}
		else
		{
			NVIC_SystemReset();//复位
		}
	}
	if(RomStateFlags.b55 || RomStateFlags.b5f)//0x55 0x5F至少有1个匹配
	{
		//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
		//70-6f-6e-6d-6c-6b-6a-69-70
		GetCalcTimes69();
		RomData_ReadBytes(LeftTimesAddr[0],RomDatas,8);
		if(LeftTimes[0]!=0)
		{
			//第4次不用滚步
			if(CalcTimes_BF!=0)
			{
				for(i=0;i<8;i++)
				{
					lcyIRDecodeIn[i]=RomDatas[7-i];
				}
				HashCalc_N(CalcTimes_BF);
				for(i=0;i<8;i++)
				{
					RomDatas[7-i]=lcyIRDecodeOut[i];
					curHash[i]=lcyIRDecodeOut[i];
				}
			}
		}
		else//计算次数为0
		{
			//掉电中断
			UsedDEC();//处理借位
			//破坏41、4B的匹配段的校验字节，目的下次不使用他了，用另一个
			ReverseRom(LeftTimesAddr[1]+0x09);
			x=RomData_ReadByte(LeftTimesAddr[1]+0x08);//中位次数
			if(x!=0)//中位次数为0，则破坏高位使用段的校验
			{
				ReverseRom(LeftTimesAddr[2]+0x09);
				if(LeftTimesAddr[2]==0x37)//37段，还要破坏7e段的校验
					ReverseRom(0x87);//(0x7e+0x09);
			}
			ReverseRom(LeftTimesAddr[0]+0x09);
			//关掉电中断
			if(bBATON()==GPIO_PIN_RESET)
			{
				//D2中的次数要置位0x80，目的？
				CalcTimes_D2=0x80;
				return;
			}
			else
			{
				NVIC_SystemReset();//复位
			}
		}
	}
}

