#include "OnCarProc.h"
#include "Variables.h"
#include <util/delay.h>
#include "EEProc.h"
#include "lib/lcyIRDecode.h"
#include "lib/lcyHash.h"
#include "lib/Verify.h"
#include "Function.h"
void ProgramWork(uint8_t keyType,uint8_t maxNum);
void IRTx2425Frame(void);
void ProcCommand_0F(void);
void ProcCommand_7A(void);
void ProcCommand_26(void);
void ProcCommand_39(void);//钥匙学习过程
void CarIRTx_10_28(void);
void CarIRTxProc(void)
{
	//uint16_t txConst=pgm_read_word(&CarIRTxConstList[LOW_NIBBLE(IRTxList[0])]);
	_delay_ms(20);
	T3WorkType=T3_CarIRTx;
	IRTxCount=IRTxCount<<1;
	IRTxIndex=0xff;
	bIRFinish_Reset();
	ATA_POWERON_C(PRR1, PRT3)
	T3CR=0x00;
	T3CR=0x20;
	T3COR=60763;//pgm_read_word(&CarIRTxConstList[LOW_NIBBLE(IRTxList[0])]);
	T3IFR =0x07;
	T3IMR=0x02;
	T3MRA = 0x02;//xto/4
	T3MRB=0x00;
	T3CR=0x84;
	while(!bIRFinish_IsSet());
}
void CarIRRxProc(void)
{
	bFuncRet_Reset();
	T3WorkType=T3_CarIRRx;
	ATA_POWERON_C(PRR1, PRT3)
	while(1)
	{
		IRRxCount=0xff;
		T3CR=0x00;
		T3CR |= _BM(T3RES);
		T3IFR =0x07;
		T3IMR=0x06;//捕捉中断,比较中断接收失败
		T3MRA=0x02;//xto/4
		T3MRB=0x74;
		T3COR=60763;//10ms超时，接收失败计数
		T3CR=0x8c;
		bIRFinish_Reset();
		CarIRRxTimeOut_10ms=0;
		bIRRxH_Reset();
		while(!bIRFinish_IsSet())
		{
			if(!bOnCarPower())
			{
				bFuncRet_Reset();
				IRRxCount=0;
				return;
			}
		}
		if(IRRxCount==0)
		{
			bFuncRet_Reset();
			return;
		}
		else
		{
			if(CarIRCommand==0x7a)//查询命令0x7a，0x0f直接回复，无需分支判断
			{
				_delay_ms(20);
				ProcCommand_7A();
				continue;
			}
			else if (CarIRCommand==0x0f)
			{
				_delay_ms(20);
				ProcCommand_0F();
				continue;
			}
			else
			{
				bFuncRet_Set();
				return;
			}
		}
	}
	
}
void OnCarProc(void)
{
	//uint8_t i;
	if(RomStateFlags.Bits.bRomWrited)//ROM烧写过
	{
		//wfDelay_ms(13);
		while(1)
		{
			if(!bOnCarPower())
			{
				return;
			}
			//学习过：t24-r26-t27
			//未学习：t25-r26-t27-r39
			//_delay_ms(5);
			IRTx2425Frame();
			CarIRRxTimeOut_N_10ms=12;
			CarIRRxProc();
			if(!bFuncRet_IsSet())//未收到回复数据,则重新发送
				continue;
			if(CarIRCommand==0x26 && CarIRCommandParam!=0x00)//处理26指令
			{
				ProcCommand_26();//返回27指令
				/************************************************************************/
				/* 锁电,写入遥控计数值                                                  */
				/************************************************************************/
				BAT_ON();
				UsedDEC();//次数减一
				InitButtonTimes();
				ChangeRFState(ROM_9E);
				RomStateFlags.Bits.bRFStudy = 0;
				BAT_OFF();
				if(!RomStateFlags.Bits.bStudy)//未学习，则启动等待学习状态
				{
					CarIRRxTimeOut_N_10ms=100;
					CarIRRxProc();
					if(CarIRCommand==0x39 && CarIRCommandParam!=0x00)
					{
						ProcCommand_39();
						if(!bFuncRet_IsSet())
							continue;
						GetKeyState();
					}
					// 					else
					// 					{
					// 						NVIC_SystemReset();
					// 					}
				}
				/************************************************************************/
				/*修正hash区		                                                      */
				/************************************************************************/
				BAT_ON();
				//LED_ON();//调整成定时器闪烁				
				GetKeyParam();//再执行一次为下次工作准备好数据
				LED_OFF();
				BAT_OFF();
				while(1)
				{
					_delay_ms(50);
					CarIRTx_10_28();
					_delay_ms(50);
					CarIRTx_10_33_SSID();
					if(!bOnCarPower())
					{
						break;
					}
				}
			}
		}
	}
	else//没写入，等待红外指令
	{
		if(EE9e==0x0c)
		{
			ChangeKeyState(0x4c);
		}
		if(EE9e==0x4c)
		{
			RomData_ReadBytes(EEDataOriginAddr+0x09,SSID,4);
			while(1)
			{
				CarIRTx_10_28();
				_delay_ms(100);
				CarIRTx_10_33_SSID();
				_delay_ms(100);
				if(!bOnCarPower())
				{
					return;
				}
			}
		}
		else if(EE9e==0x21)
		{
			//sub_17B4
			while(1)
			{
				if(!bOnCarPower())
				{
					return;
				}
				CarIRRxTimeOut_N_10ms=100;
				CarIRRxProc();//调试阶段，永不超时
				if(bFuncRet_IsSet())
				{
					if(CarIRCommand==0x04 || CarIRCommand==0x037)
					{
						ProgramWork(0x37,0x10);
					}
				}
			}
		}
		else
		{
			while(1)
			{
				if(!bOnCarPower())
				{
					return;
				}
				CarIRRxTimeOut_N_10ms=100;
				CarIRRxProc();//调试阶段，永不超时
				if(bFuncRet_IsSet())
				{
					switch(CarIRCommand)
					{
					case 0x0e:
						ProgramWork(0x0e,0x02);
						break;
					case 0x00:
						ProgramWork(0x00,0x03);
						break;
					case 0x02:
						ProgramWork(0x02,0x10);
						break;
					case 0x35:
						ProgramWork(0x35,0x10);
						break;
					case 0x03:
						ProgramWork(0x03,0x14);
						break;
						// 					default:
						// 						LEDFlash();
						// 						break;
					}
				}
			}
		}
	}
}
void ProgramWork(uint8_t keyType,uint8_t maxNum)
{
	uint8_t i,addr,lastAddr=0,x;
	uint8_t rxData[8];
	if(CarIRCommandParam!=0xff)//非写入命令
	{
		if((keyType==0x37))// || (keyType==0x04))//需要解码后再使用
		{
			for(i=0;i<8;i++)
				lcyIRDecodeIn[i]=IRRxList[3+i];
			lcyIRDecode();
			for(i=0;i<8;i++)
				rxData[i]=lcyIRDecodeOut[i];
		}
		else
		{
			for(i=0;i<8;i++)
				rxData[i]=IRRxList[3+i];
		}
		//35指令全是0的处理在检查后直接进行
		if(keyType==0x35)
		{
			x=0;
			for(i=0;i<8;i++)
			{
				x^=IRRxList[3+i];
			}
			if(x==0)
			{
				for(i=0;i<8;i++)
					lcyHashIn[i]=rxData[i];
				lcyHashOnce();
				for(i=0;i<8;i++)
					IRTxList[3+i]=lcyHashOut[i];
				IRTxList[0]=0x10;
				IRTxList[1]=0x05;
				IRTxList[2]=CarIRCommandParam;
				IRTxCount=11;
				CarIRTxProc();
				return;
			}
		}
		if(CarIRCommandParam<maxNum)
		{
			addr=CarIRCommandParam*0x08;
			addr=addr+0x88;
			switch(keyType)
			{
			case 0x0e:
				//0x88+i*8
				break;
			case 0x00:
				addr=addr-0x08;
				//0x80+i*8
				break;
			case 0x68:
				addr=addr-0x86;//;-0x08-0x7e;
				//0x02+i*8
				break;
			case 0x03:
				addr=addr-0x90;//-0x08-0x7e-0x02-0x02;
				//i*8-0x02
				break;
			default:
				addr=addr-0x88;//-0x08-0x7e-0x02;
				//0x00+i*8
				break;
			}
			RomData_WriteBytes(EEDataOriginAddr+addr,rxData,8);
			lastAddr=addr;
		}
		//对收到数据进行hashcal后返回
		for(i=0;i<8;i++)
			lcyHashIn[i]=IRRxList[3+i];
		lcyHashOnce();
		for(i=0;i<8;i++)
			IRTxList[3+i]=lcyHashOut[i];
		IRTxList[0]=0x10;
		IRTxList[1]=0x05;
		IRTxList[2]=CarIRCommandParam;
		IRTxCount=11;
		CarIRTxProc();
	}
	else//写入存储设备中
	{
		IRTxList[0]=0x10;
		IRTxList[1]=0x05;
		IRTxList[2]=CarIRCommandParam;
		for(i=0;i<8;i++)
			IRTxList[3+i]=0xff;
		IRTxCount=11;
		/************************************************************************/
		/* 非06、68、35、37写入指令，需要先进行一次校验                         */
		/************************************************************************/
		if((CarIRCommand!=0x06) &&
		(CarIRCommand!=0x68) &&
		(CarIRCommand!=0x35) &&
		(CarIRCommand!=0x37) )
		{
			i=RomData_ReadByte(EEDataOriginAddr+lastAddr+7);
			if(CarIRCommand==i)//7F单元的02，也就是校验是02命令字时进行数据检测
			{
				VerifyEEDatas(maxNum,lastAddr);
				if(!bFuncRet_IsSet())
				{
					CarIRTxProc();
					BAT_OFF();
					LED_ON();					
					while(1)
					{
						if(!bOnCarPower())
							return;
					}				
				}
			}
		}
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		for(i=0;i<8;i++)
			IRTxList[3+i]=0x00;
		if((CarIRCommand==0x0e) || (CarIRCommand==0x68))
		{
			CarIRTxProc();
			BAT_OFF();
			LED_ON();
			while(1)
			{
				if(!bOnCarPower())
					return;
			}
		}
		while(1)
		{
			if(CarIRCommand==0x00)
			{
				ChangeKeyState(0x21);
			}
			else if((CarIRCommand==0x06) || (CarIRCommand==0x07))
			{
				ChangeKeyState(0x27);
			}
			else if (CarIRCommand==0x03)
			{
				ChangeKeyState(0x0c);
			}
			else
			{
				break;
			}
			CarIRTxProc();
			BAT_OFF();
			LED_ON();
			while(1)
			{
				if(!bOnCarPower())
					return;
			}
		}
		CheckDataBlockVerify(0x55);
		x=RomData_ReadByte(EEDataOriginAddr+0x55+8);
		if((!bFuncRet_IsSet()) || (x>0x7f))
		{
			Led_WaitCarPowerOff();
			return;
		}
		LED_OFF();
		BAT_ON();
		x=x|0x03;//低位次数
		//生成0x73段
		RomData_ReadBytes(EEDataOriginAddr+0x55, RomDatas, 8);
		for (i = 0; i < 8; i++)
		{
			lcyHashIn[i] = RomDatas[7 - i];
		}
		lcyHashCalc(3);
		for (i = 0; i < 8; i++)
		{
			RomDatas[7 - i] = lcyHashOut[i];
		}
		RomDatas[8]=x;
		RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
		RomData_WriteBytes(EEDataOriginAddr+0x73, RomDatas, 10);//写入当前次数段，且使其匹配
		//生成使用次数
		for(i=0;i<8;i++)
		{
			RomDatas[i]=0xff;
		}
		addr=0;
		i=0x7f;
		while(1)
		{
			if(i==x)
			{
				break;
			}
			i--;
			if(addr==0)
				addr=7;
			else
				addr--;
		}
		RomDatas[addr]=i;
		RomData_WriteBytes(EEDataOriginAddr+0x69, RomDatas, 8);

		RomData_ReadBytes(EEDataOriginAddr+0x5f, RomDatas, 9);
		RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
		RomData_WriteBytes(EEDataOriginAddr+0x5f, RomDatas, 10);
		ReverseRom(0x5f+9);

		// 	GetCalcTimes69();
		// 	GetLeftTimeBlock(LeftTimesH);
		// 	GetLeftTimeBlock(LeftTimesM);
		// 	GetLeftTimeBlock(LeftTimesL);

		x=RomData_ReadByte(EEDataOriginAddr+0x98);
		if(x==0xff)//写入电池电压
		{
			RomDatas[0]=0x73;
			RomDatas[1]=0x0c;
			RomData_WriteBytes(EEDataOriginAddr+0x98,RomDatas,2);
		}
		RomDatas[0]=0x01;
		RomDatas[1]=0x01;
		RomDatas[2]=0x01;
		RomDatas[3]=0x01;
		x=RomData_ReadByte(EEDataOriginAddr+0x00);
		if(GetBit_uint8(x,7)==0)
			RomDatas[4]=0xff;
		else
			RomDatas[4]=0x00;
		RomDatas[5]=0xff;
		RomData_WriteBytes(EEDataOriginAddr+0x90,RomDatas,6);
		RomData_ReadBytes(EEDataOriginAddr+0x01, RomDatas, 12);
		for(i=0;i<4;i++)
		{
			x=RomDatas[i];
			RomDatas[i]=RomDatas[7-i];
			RomDatas[7-i]=x;
		}
		x=RomDatas[8];
		RomDatas[8]=RomDatas[11];
		RomDatas[11]=x;

		x=RomDatas[9];
		RomDatas[9]=RomDatas[10];
		RomDatas[10]=x;
		RomData_WriteBytes(EEDataOriginAddr+0x01,RomDatas,12);

		if((keyType==0x37))// || (keyType==0x04))//需要解码后再使用
		{
			ChangeKeyState(0x05);
		}
		else
		{
			ChangeKeyState(0x04);
		}
		BreakMyBlock();
		GetKeyState();
		GetKeyParam();//获得钥匙当前相关数据
		LED_ON();
		CarIRTxProc();
		BAT_OFF();
		Led_WaitCarPowerOff();
		return;
	}
}
void ProcCommand_26(void)//回应0x27指令
{
	uint8_t i;
	GetKeyWorkValue(&IRRxList[2],0x26);
	for(i=0;i<8;i++)
	{
		IRTxList[2+i]=WorkValueDatas[i];
	}
	IRTxList[0]=0x10;
	IRTxList[1]=0x27;
	IRTxCount=10;
	CarIRTxProc();
}
void ProcCommand_0F(void)
{
	//有些地方未清楚
	uint8_t i;
	IRTxList[0]=0x10;
	IRTxList[1]=0x0d;
	IRTxList[2]=0x00;
	for(i=0;i<8;i++)
	{
		IRTxList[3+i]=0x01;//eeprom_8E[i];
	}
	IRTxCount=11;
	CarIRTxProc();
}
void ProcCommand_7A(void)//查询测试
{
	IRTxCount=0;
	uint32_t i;
	for(i=0;i<10;i++)
		IRTxList[i]=0x00;
	switch(CarIRCommandParam)
	{
	case 0:
		//车载一直发送00
		while(1)
		{
			IRTxCount=10;
			CarIRTxProc();
			while(bOnCarPower());
			return;
		}
		break;
	case 1:
		//电池红外一直发送00
		while(1)
		{
			IRTxCount=10;
			//RFIRTxProc();
			while(bOnCarPower());
			return;
		}
		break;
	case 2:
		//RF一直发送00
		while(1)
		{
			IRTxCount=10;
			while(bOnCarPower());
			return;
		}
		break;
	case 3:
		//等待掉电重启
		while(bOnCarPower());
		break;
	case 4:
		//有不懂得IO操作
		//等待掉电重启
		while(bOnCarPower());
		break;
	case 5:
		//开LED
		LED_ON();
		while(bOnCarPower());
		break;
	case 6:
		//收到数据写入98-9D
		RomData_WriteBytes(EEDataOriginAddr+0x98,&IRRxList[3],6);
	case 7:
		RomData_ReadBytes(EEDataOriginAddr+0x98,&IRTxList[3],8);
		IRTxList[0]=0x10;
		IRTxList[1]=0x7b;
		IRTxList[2]=CarIRCommandParam;
		IRTxCount=9;
		break;
	case 8:
		RomData_ReadBytes(EEDataOriginAddr+0x9c,&IRTxList[3],4);
		IRTxList[0]=0x10;
		IRTxList[1]=0x7b;
		IRTxList[2]=CarIRCommandParam;
		IRTxList[3]=ROMVer;
		IRTxList[4]=0x45;//电池电量
		IRTxList[7]=RomData_ReadByte(EEDataOriginAddr+0x00);
		IRTxList[8]=0x01;
		IRTxCount=9;
		break;
	}
	if(IRTxCount!=0)
	{
		//HAL_Delay(50);
		CarIRTxProc();
	}
	while(bOnCarPower());
}
void ProcCommand_39(void)//钥匙学习过程
{
	uint8_t i;
	bFuncRet_Reset();
	//密码移动一位后记载:psw8,psw1,psw2,....,psw7
	RomData_ReadBytes(EEDataOriginAddr+0x01,&RomDatas[1],8);
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
			return;
		}
	}
	/************************************************************************/
	/* 锁电，更改存储区为学习过状态                                         */
	/************************************************************************/
	BAT_ON();
	//密码移动2位后记载:psw7,psw8,psw1,psw2,....,psw6
	RomData_ReadBytes(EEDataOriginAddr+0x01,&RomDatas[2],8);
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
	RomStateFlags.Bits.bStudy=1;
	RomData_WriteBytes(EEDataOriginAddr+0x88,RomDatas,8);
	SetBit_uint8(EE9e,4);
	ChangeKeyState(EE9e);
	BAT_OFF();
	bFuncRet_Set();
}
void IRTx2425Frame(void)
{
	IRTxList[0]=0x10;
	if(RomStateFlags.Bits.bStudy)
		IRTxList[1]=0x24;//仅写过
	else
		IRTxList[1]=0x25;//学习过

	IRTxList[2]=LeftTimes69;
	IRTxList[3]=LeftTimes[LeftTimesM];
	IRTxList[4]=LeftTimes[LeftTimesH];

	RomData_ReadBytes(EEDataOriginAddr+0x09,&IRTxList[5],4);
	IRTxCount=9;
	CarIRTxProc();
}
void CarIRTx_10_33_SSID(void)
{
	uint8_t i;
	IRTxList[0]=0x10;
	IRTxList[1]=0x33;
	for(i=0;i<4;i++)
		IRTxList[2+i]=SSID[i];
	IRTxCount=6;
	CarIRTxProc();
}
void CarIRTx_10_28(void)
{
	IRTxList[0]=0x10;
	IRTxList[1]=0x28;
	IRTxCount=2;
	CarIRTxProc();
}