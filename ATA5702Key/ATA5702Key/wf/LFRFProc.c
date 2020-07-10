#include "LFRFProc.h"
#include "RF_TX.h"
#include "Variables.h"
#include "EEProc.h"
#include <avr/pgmspace.h>
#include <util\delay.h>
#include "lib/Verify.h"
#include "LF_RX.h"
#include "Function.h"

void GetLFData(void);
void WaitLFRx(uint8_t n674);
void RFTxProc(void);
void LFRFProc(void)
{
	uint8_t key,i,x,cmd,RxKeyNum,usedKey,addr,y;//	unlock,
	//RomData_ReadBytes(EEDataOriginAddr+0x09, SSID, 4);
	ATA_rfTxInit_C();
	ATA_rfTxStartTx_C(RfTx_Config_LFRF);
	for(i=0;i<5;i++)
		IRTxList[i]=0x00;
	key=SSID[0]&0x07;
	//GetLFData();
	WaitLFRx(45);//40ms
	if(!bLFRxFinish_IsSet())
		return;
	if(LfRxCount==1)
	{
		RomData_WriteByte(EEDataOriginAddr+0xfb,LfRxData[0]);
		return;
	}
	x=GetVerify_byteXOR(LfRxData,LfRxCount-1);
	if(x!=LfRxData[LfRxCount-1])
	{
		return;
	}
	cmd=LOW_NIBBLE(LfRxData[0]);	
	RxKeyNum=HIGH_NIBBLE(LfRxData[0]);
// 	if(LfRxCount==4)
// 	{
// 		//g_temp=2;
// 		//LEDTestMonitor();
// 		//LEDTestMonitor();
// 	}
// 	else
// 	{
// 		g_temp=0;
// 	}
	if(LfRxCount==4)
	{		
		if(cmd==0x0a)//寻找钥匙
		{
			if(!(LfRxData[2]& _BM(key)))//判断此钥匙已使用
			{
				return;
			}
			//LEDTestMonitor();
			GetRSSI();
			//LEDTestMonitor();
			if(gRssiVector<1300)
				return;
			LfTxCount=2;
			LfTxData[0]=0x1a;
			x=key*0x24;
			x=x+0x03;
			LfTxData[1]=x;
			_delay_us(460);
			usedKey=LfRxData[2];
			y=0;
			for(i=0;i<8;i++)
			{
				if(usedKey & _BM(i))
					y++;
				if(i==key)
					break;
			}
			switch(y)
			{
				case 1:
					_delay_us(4003);
					break;
				case 2:
					_delay_us(7900);
					break;
				case 3:
					_delay_us(11939);
					break;
				case 4:
					_delay_us(16216);
					break;
				case 5:
					_delay_us(20532);
					break;
				case 6:
					_delay_us(25145);
					break;
				case 7:
					_delay_us(29874);
					break;
				case 8:
					_delay_us(34720);
					break;
			}
			RFTxProc();
		}		
	}
	else if(LfRxCount==11)
	{
		if(cmd==0x09)//点火
		{
			if(RxKeyNum<0x08)
				x=key;
			else
				x=key+0x08;
			if(RxKeyNum==x)//判断此钥匙请求数据
			{
				_delay_ms(10);
				_delay_ms(13);
				_delay_us(476);
				LfTxCount=14;
				LfTxData[0]=0x19;
				LfTxData[1]=LeftTimes69;//LeftTimes[LeftTimesL];
				LfTxData[2]=LeftTimes[LeftTimesM];
				LfTxData[3]=LeftTimes[LeftTimesH];
				GetKeyWorkValue(&LfRxData[2],0x50);
				for(i=0;i<8;i++)
				{
					LfTxData[4+i]=WorkValueDatas[i];
				}
				LfTxData[12]=GetVerify_byteXOR(&LfTxData[1],11);
				x=0x09^RxKeyNum;
				LfTxData[12] ^= x;
				LfTxData[13]=MAKE_BYTE(RxKeyNum,0x0f);
				RFTxProc();
				//rfTxShutdown();
				PowerLed();
				UsedDEC();
				for(i=0;i<4;i++)
					RomDatas[i]=0x01;
				RomData_WriteBytes(EEDataOriginAddr+0x90,RomDatas,4);
				GetKeyState();
				GetKeyParam();//获得钥匙当前相关数据
			}	
		}		
	}	
	else if(LfRxCount==5)
	{
		usedKey=LfRxData[3];
		if(!(usedKey& _BM(key)))//判断此钥匙已使用
		{
			return;
		}
		if(cmd==0x0B)//锁车
		{
			RFKeyValue=0x23;
		}
		else if(cmd==0x0D  || cmd==0x0c)//开锁
		{
			RFKeyValue=RomData_ReadByte(EEDataOriginAddr+0x94);
			if(RFKeyValue!=0)
				RFKeyValue=0x21;
			else
				RFKeyValue=0x20;
		}
		else if(cmd==0x0e)//后备箱开
		{
			//unlock=RomData_ReadByte(EEDataOriginAddr+0x94);
			RFKeyValue=0x22;
		}
		GetDoorProc(RFKeyValue);
		_delay_ms(3);
		//_delay_us(3000);
		y=RomData_ReadByte(EEDataOriginAddr+0xfb);
		if(RxKeyNum!=key && y!=0x67)
		{
			y=0;
			for(i=0;i<8;i++)
			{
				if((usedKey & _BM(i)) &&(i!=RxKeyNum))
					y++;
				if(i==key)
					break;
			}
			switch(y)
			{
				case 1:
					_delay_ms(13);
					_delay_us(350);
					break;
				case 2:
					_delay_ms(27);
					_delay_us(178);
					break;
				case 3:
					_delay_ms(41);
					_delay_us(504);
					break;
				case 4:
					_delay_ms(56);
					_delay_us(573);
					break;
				case 5:
					_delay_ms(72);
					_delay_us(350);
					break;
				case 6:
					_delay_ms(88);
					_delay_us(481);
					break;
				case 7:
					_delay_ms(105);
					_delay_us(90);
					break;
				// 					case 8:
				// 					_delay_us(34720);
				// 					break;
			}
			// 				for(i=0;i<8;i++)
			// 				{
			// 					if(i==key)
			// 						break;
			// 					if(i==RxKeyNum)
			// 						continue;
			// 					if(usedKey & _BM(i))
			// 						_delay_us(13500);
			// 				}
		}
		LfTxCount=12;
		if(cmd==0x0B)//锁车
		{
			LfTxData[0]=0x1B;
		}
		else if(cmd==0x0D  || cmd==0x0c)//开锁
		{
			if(RFKeyValue==0x21)
				LfTxData[0]=0x1c;
			else
				LfTxData[0]=0x1d;
		}
		else if(cmd==0x0e)//后备箱开
		{
			LfTxData[0]=0x1e;
		}		
		LfTxData[1]=ButtonTimes;//LeftTimes[LeftTimesL];
		for(i=0;i<8;i++)
			LfTxData[2+i]=DoorDatas[i]^LfRxData[2];
		LfTxData[10]=GetVerify_byteXOR(&LfTxData[1],9);
		if(cmd==0x0B || cmd==0x0e)//锁车 后备箱开
		{
			x=cmd;
		}
		else if(cmd==0x0D  || cmd==0x0c)//开锁
		{
			if(RFKeyValue==0x21)
				x=0x0c;
			else
				x=0x0d;
		}
		x=x^key;
		LfTxData[10] ^= x;
		x=key<<1;
		x=x+1;
		LfTxData[11]=MAKE_BYTE(x,0x0f);
		RFTxProc();
		//rfTxShutdown();
		/************************************************************************/
		/*  修改按键次数                                                        */
		/************************************************************************/
		addr=LeftTimes69&0x03;
		addr=addr+0x90;
		ButtonTimes=RomData_ReadByte(EEDataOriginAddr+addr);
		ButtonTimes++;
		RomData_WriteByte(EEDataOriginAddr+addr,ButtonTimes);
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		PowerLed();
	}
}
void GetLFData(void)
{
	uint8_t i;
	LDFCKSW |= (1<<LDFSCSW);
	while ((LDFCKSW & (1<<LDFSCKS)) ==0);// wait until clock source is switched
	LfRxCount = LDFFL;
	for (i=0; i < LfRxCount; i++)
	{
		LfRxData[i] = LDFD;
	}
	LDFCKSW &= ~(1<<LDFSCSW);
}
void WaitLFRx(uint8_t n674)
{
	ATA_POWERON_C(PRR1, PRT2)
	T2CR &= ~_BM(T2ENA);
	T2CR = _BM(T2RES);	
	T2MR=0x33;
	T2COR=n674;
	T2IMR=0x00;
	T2IFR=0xff;
	T2CR=_BM(T2ENA);
	//bLFRxFinish_Reset();
	while(1)
	{
		if(T2IFR & _BM(T2COF))
		{
			bLFRxFinish_Reset();
			return;
		}
		if(bLFRxFinish_IsSet())
		{
			T2CR &= ~_BM(T2ENA);
			T2CR = _BM(T2RES);
			GetLFData();
			if(LfRxCount==0)
			{
				bLFRxFinish_Reset();
				return;
			}
			return;
		}
	}	
}
void RFTxProc(void)
{
	ATA_SETBITMASK_C(DFL,_BM(DFCLR))
	ATA_rfTxFillDFifo_C(2, IRTxList);	
	ATA_rfTxFillDFifo_C(LfTxCount, LfTxData);
	do 
	{
		ATA_rfTxProcessing_C();		
	}while (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_ACTIVE);
	//ATA583X_RFTxFrameProc();
}