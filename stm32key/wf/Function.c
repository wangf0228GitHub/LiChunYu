#include "Function.h"
#include "wfEEPROM.h"
#include "Variables.h"
#include "..\..\..\WF_Device\wfDefine.h"
#include "..\..\..\WF_Device\Verify.h"
#include "lcyHash.h"
#include "IRProc.h"
#include "lcyIRDecode.h"

void GetKeyParam(void)
{
	uint8_t x;
	RomData_ReadBytes(0x9e,RomDatas,2);
	if(RomDatas[0]==0xff && RomDatas[1]==0xff)//尚未初始化
	{
		BAT_ON();
		RomDatas[0]=ROM_9E;
		RomDatas[1]=ROM_9F;
		RomData_WriteBytes(0x9e,RomDatas,2);//初始化ROM版本
	}
	RomData_ReadBytes(0x9e,RomDatas,2);
	x=RomDatas[0]+RomDatas[1];
	RomStateFlags.bRomWrited=0;
	RomStateFlags.bStudy=0;
	if(x!=0)
		NVIC_SystemReset();//存储区无法初始化，系统复位
	if(GetBit(RomDatas[0],2)!=0)//写过了
	{
		RomStateFlags.bRomWrited=1;
	}
	if(GetBit(RomDatas[0],4)!=0)//学习过
	{
		RomStateFlags.bStudy=1;
	}
	if(RomStateFlags.bRomWrited)//写过了，获取系统参数
	{
		//使用次数低位
		//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
		//70-6f-6e-6d-6c-6b-6a-69-70
		GetCalcTimes69();
		//使用次数高位
		GetLeftTimeBlock(LeftTimesH);
		if(LeftTimes[LeftTimesH]==0xff)//ee出现异常，容错
		{
			Fix2Dand37data();
		}
		//使用次数中位
		GetLeftTimeBlock(LeftTimesM);
		if(LeftTimes[LeftTimesM]==0xff)//ee出现异常，容错
		{
			Fix41and4Bdata();
		}
		//使用次数低位所用段，同时找到当前hash
		Check55and5fdata();
	}
}
//获得使用次数
void GetLeftTimeBlock(uint8_t nBlock)
{
	uint8_t t1,t2,addr1,addr2,tTop,x,y;
	switch(nBlock)
	{
	case LeftTimesH:
		addr1=0x2d;
		addr2=0x37;
		tTop=0x2f;
		break;
	case LeftTimesM:
		addr1=0x41;
		addr2=0x4b;
		tTop=0x1f;
		break;
	case LeftTimesL:
		addr1=0x55;
		addr2=0x5f;
		break;
	default:
		return;
	}
	CheckDataBlockVerify(addr1);
	t1=RomDatas[8];
	if(!gFlags.bFuncRet)
	{
		t1=0xff;		
	}
	CheckDataBlockVerify(addr2);
	t2=RomDatas[8];	
	if(!gFlags.bFuncRet)
	{
		t2=0xff;				
	}
	LeftTimesAddr[nBlock]=0xff;//异常值
	if((t1!=0xff) && (t2!=0xff))
	{	
		if(nBlock!=LeftTimesL)
		{
			if(t1>t2)//用大的
			{
				if((t2==0) && (t1==tTop))//例如：00 1f则使用00
				{
					LeftTimesAddr[nBlock]=addr2;
					OtherLeftTimesAddr[nBlock]=addr1;
					LeftTimes[nBlock]=t2;
				}
				else//否则正常使用大的
				{
					LeftTimesAddr[nBlock]=addr1;
					OtherLeftTimesAddr[nBlock]=addr2;
					LeftTimes[nBlock]=t1;
				}			
			}
			else//t2>t1
			{
				if((t1==0) && (t2==tTop))//例如：00 1f则使用00
				{
					LeftTimesAddr[nBlock]=addr1;
					OtherLeftTimesAddr[nBlock]=addr2;
					LeftTimes[nBlock]=0;
				}
				else//否则正常使用大的
				{
					LeftTimesAddr[nBlock]=addr2;
					OtherLeftTimesAddr[nBlock]=addr1;
					LeftTimes[nBlock]=t2;
				}
			}
			return;//返回
		}
		else
		{
			tTop=LeftTimes69&0xfc;
			if(t1==tTop)
			{
				LeftTimesAddr[nBlock]=addr1;
				OtherLeftTimesAddr[nBlock]=addr2;
				LeftTimes[nBlock]=t1;	
				return;
			}
			else if(t2==tTop)
			{
				LeftTimesAddr[nBlock]=addr2;
				OtherLeftTimesAddr[nBlock]=addr1;
				LeftTimes[nBlock]=t2;
				return;
			}
			else//两段都不匹配，需要重新生成低位hash
			{
				t1=0xff;
				t2=0xff;
			}
		}
	}
	else if(t1!=0xff)//第一段匹配，则重新生成第二段
	{
		LeftTimesAddr[nBlock]=addr1;
		OtherLeftTimesAddr[nBlock]=addr2;
		LeftTimes[nBlock]=t1;		
	}
	else if(t2!=0xff)
	{
		LeftTimesAddr[nBlock]=addr2;
		OtherLeftTimesAddr[nBlock]=addr1;
		LeftTimes[nBlock]=t2;
	}
	if(t1!=0xff || t2!=0xff)//需要重新生成第二段
	{
		switch(nBlock)
		{
		case LeftTimesH:
			if(LeftTimes[LeftTimesH]==0)//高位为0则不再生成第二段
				break;
			else
			{	
				t2=LeftTimes[LeftTimesH]-1;
				if(t2>=0x24)
				{
					t1=0x24;
					addr1=0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
				}
				else if(t2>=0x18)
				{
					t1=0x18;
					addr1=0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
				}
				else if(t2>=0xc)
				{
					t1=0x0c;
					addr1=0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
				}
				else
				{
					t1=0x00;
					addr1==0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
				}
				t1=t2-t1;
				FixDataBlock(OtherLeftTimesAddr[LeftTimesH],addr1,4096,t1,LeftTimes[LeftTimesH]-1);
			}
		case LeftTimesM:
			if(LeftTimes[LeftTimesM]==0)//中位为0，则需要向高位借位
			{
				if (LeftTimes[LeftTimesH]==0)//高位为0，则无法借位，不再生成中位第二段
					break;
				else//借位生成中位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesM],OtherLeftTimesAddr[LeftTimesH],128,0x1f,0x1f);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesM],LeftTimesAddr[LeftTimesH],128,LeftTimes[LeftTimesM]-1,LeftTimes[LeftTimesM]-1);
			}
			break;
		case LeftTimesL:
			if(LeftTimes[LeftTimesL]==0)//低位为0，则需要向中位借位
			{
				if ((LeftTimes[LeftTimesM]==0) && (LeftTimes[LeftTimesH]==0))//中位、高位都为0，则无法借位，不再生成低位第二段
					break;
				else//借位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesL],OtherLeftTimesAddr[LeftTimesM],0x7c,1,0x7c);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesL],LeftTimesAddr[LeftTimesM],LeftTimes[LeftTimesL]-4,1,LeftTimes[LeftTimesL]-4);
			}
			break;
		}
	}
	else//两段都不匹配，需要用小的次数从新生成
	{
		if(t1<t2)
		{
			LeftTimesAddr[nBlock]=addr1;
			OtherLeftTimesAddr[nBlock]=addr2;
			LeftTimes[nBlock]=t1;
		}
		else
		{
			LeftTimesAddr[nBlock]=addr2;
			OtherLeftTimesAddr[nBlock]=addr1;
			LeftTimes[nBlock]=t2;
		}
		switch(nBlock)
		{
		case LeftTimesH:
			if(LeftTimes[LeftTimesH]==0)//高位为0则仅生成第一段
			{
				RomData_ReadBytes(0x0d,RomDatas,8);
				RomDatas[8]=0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
				RomData_WriteBytes(LeftTimesAddr[LeftTimesH],RomDatas,10);//写入当前次数段，且使其匹配
			}
			else
			{	
				t2=LeftTimes[LeftTimesH]-1;
				if(t2>=0x24)
				{
					t1=0x24;
					addr1=0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
				}
				else if(t2>=0x18)
				{
					t1=0x18;
					addr1=0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
				}
				else if(t2>=0xc)
				{
					t1=0x0c;
					addr1=0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
				}
				else
				{
					t1=0x00;
					addr1==0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
				}
				t1=t2-t1;
				FixDataBlock(OtherLeftTimesAddr[LeftTimesH],addr1,4096,t1,LeftTimes[LeftTimesH]-1);//修复-1段
				FixDataBlock(LeftTimesAddr[LeftTimesH],OtherLeftTimesAddr[LeftTimesH],4096,1,LeftTimes[LeftTimesH]);//修复当前
			}
		case LeftTimesM:
			if(LeftTimes[LeftTimesM]==0)//中位为0，则需要向高位借位
			{
				RomData_ReadBytes(LeftTimes[LeftTimesH],RomDatas,8);
				RomDatas[8]=0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
				RomData_WriteBytes(LeftTimesAddr[LeftTimesM],RomDatas,10);
				if (LeftTimes[LeftTimesH]==0)//高位为0，则仅生成当前段			
					break;
				else//借位生成中位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesM],OtherLeftTimesAddr[LeftTimesH],128,0x1f,0x1f);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesM],LeftTimesAddr[LeftTimesH],128,LeftTimes[LeftTimesM]-1,LeftTimes[LeftTimesM]-1);
				FixDataBlock(LeftTimesAddr[LeftTimesM],OtherLeftTimesAddr[LeftTimesM],128,1,LeftTimes[LeftTimesM]);
			}
			break;
		case LeftTimesL:
			LeftTimes[LeftTimesL]=LeftTimes69&0xfc;			
			if(LeftTimes[LeftTimesL]==0)//低位为0，则需要向中位借位
			{
				RomData_ReadBytes(LeftTimes[LeftTimesM],RomDatas,8);
				RomDatas[8]=0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
				RomData_WriteBytes(LeftTimesAddr[LeftTimesL],RomDatas,10);
				if ((LeftTimes[LeftTimesM]==0) && (LeftTimes[LeftTimesH]==0))//中位、高位都为0，则无法借位，不再生成低位第二段
					break;
				else//借位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesL],OtherLeftTimesAddr[LeftTimesM],0x7c,1,0x7c);
				}				
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesL],LeftTimesAddr[LeftTimesM],LeftTimes[LeftTimesL]-4,1,LeftTimes[LeftTimesL]-4);
				FixDataBlock(LeftTimesAddr[LeftTimesL],OtherLeftTimesAddr[LeftTimesL],4,1,LeftTimes[LeftTimesL]);
			}
			break;
		}
	}
}
//fixAddr:要修复的地址
//hashAddr：使用的hash的地址xiuf
//stepLen:修复滚步步长
//times：目标次数
void FixDataBlock(uint8_t fixAddr,uint8_t hashAddr,uint16_t stepLen,uint8_t loopTimes,uint8_t leftTiems)
{
	uint8_t i;
	RomData_ReadBytes(hashAddr,RomDatas,8);
	if(loopTimes!=0)
	{
		for(i=0;i<8;i++)
		{
			lcyIRDecodeIn[i]=RomDatas[7-i];
		}
		for(i=0;i<loopTimes;i++)
		{
			HashCalc_N(stepLen);
		}
		for(i=0;i<8;i++)
		{
			RomDatas[7-i]=lcyIRDecodeOut[i];
		}
	}
	RomDatas[8]=leftTiems;
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
	RomData_WriteBytes(fixAddr,RomDatas,10);//写入当前次数段，且使其匹配
}
//修复2d及37段数据
//使用几个预存的49152步数进行容错
void Fix2Dand37data(void)
{
	uint8_t t2d,t37,i,x;
	t2d=RomData_ReadByte(0x35);
	t37=RomData_ReadByte(0x3f);
	//用小的,点火开关去追步
	if(t2d<t37)
	{
		LeftTimesAddr[LeftTimesH]=0x2d;
		LeftTimes[LeftTimesH]=t2d;
	}
	else
	{
		LeftTimesAddr[LeftTimesH]=0x37;
		LeftTimes[LeftTimesH]=t37;
	}
	if(LeftTimes[LeftTimesH]==0)//高位次数用尽
	{
		//使用0d段
		RomData_ReadBytes(0x0d,RomDatas,8);
		RomDatas[8]=LeftTimes[LeftTimesH];
		RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
		RomData_WriteBytes(LeftTimesLAddr[LeftTimesH],RomDatas,10);//写入当前次数段，且使其匹配
		return;
	}
	IRTx_10_33_SSID();
	/************************************************************************/
	/* 开始容错使用次数段                                                   */
	/************************************************************************/		
	if(LeftTimes[LeftTimesH]>=0x24)
	{
		x=0x24;
		RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
	}
	else if(LeftTimes[2]>=0x18)
	{
		x=0x18;
		RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
	}
	else if(LeftTimes[2]>=0xc)
	{
		x=0x0c;
		RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
	}
	else
	{
		x=0x00;
		RomData_ReadBytes(0x0d,RomDatas,8);//49152
	}
	x=LeftTimes[LeftTimesH]-x;
	for(i=0;i<8;i++)
	{
		lcyIRDecodeIn[i]=RomDatas[7-i];
	}
	for(i=0;i<x;i++)
	{
		HashCalc_N(4096);
	}
	for(i=0;i<8;i++)
	{
		RomDatas[7-i]=lcyIRDecodeOut[i];
	}
	RomDatas[8]=LeftTimes[LeftTimesH];
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
	RomData_WriteBytes(LeftTimesLAddr[LeftTimesH],RomDatas,10);//写入当前次数段，且使其匹配
	/************************************************************************/
	/* 继续容错另一段                                                       */
	/************************************************************************/
	HashCalc_N(4096);
	for(i=0;i<8;i++)
	{
		RomDatas[7-i]=lcyIRDecodeOut[i];
	}
	RomDatas[8]=LeftTimes[LeftTimesH]-1;
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
	if(RomDatas[9]==0xff)
		RomDatas[9]=0;
	else
		RomDatas[9]=0xff;
	if(LeftTimesLAddr[LeftTimesH]==0x2d)
	{
		RomData_WriteBytes(0x37,RomDatas,10);//写入当前次数段，且使其不匹配
	}
	else
	{
		RomData_WriteBytes(0x2d,RomDatas,10);//写入当前次数段，且使其不匹配
	}
}
//修复41及4b段数据
//使用高位次数进行容错
void Fix41and4Bdata(void)
{
	uint8_t t41,t4b,i;
	uint16_t C0BF;
	t41=RomData_ReadByte(0x49);
	t4b=RomData_ReadByte(0x53);
	//用小的,点火开关去追步
	if(t41<t4b)
	{
		LeftTimesAddr[LeftTimesM]=0x41;
		LeftTimes[LeftTimesM]=t41;
	}
	else
	{
		LeftTimesAddr[LeftTimesM]=0x4b;
		LeftTimes[LeftTimesM]=t4b;
	}
	/************************************************************************/
	/* 开始容错使用次数段                                                   */
	/************************************************************************/	
	C0BF=0x80;
	C0BF=C0BF*LeftTimes[LeftTimesM];
	for(i=0;i<8;i++)
	{
		lcyIRDecodeIn[i]=RomDatas[7-i];
	}
	HashCalc_N(C0BF);
	for(i=0;i<8;i++)
	{
		RomDatas[7-i]=lcyIRDecodeOut[i];
	}
	RomDatas[8]=LeftTimes[1];
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
	RomData_WriteBytes(LeftTimesLAddr[LeftTimesH],RomDatas,10);//写入当前次数段，且使其匹配
	/************************************************************************/
	/* 继续容错另一段                                                       */
	/************************************************************************/
	HashCalc_N(0x80);
	for(i=0;i<8;i++)
	{
		RomDatas[7-i]=lcyIRDecodeOut[i];
	}
	RomDatas[8]=LeftTimes[LeftTimesM]-1;
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
	if(RomDatas[9]==0xff)
		RomDatas[9]=0;
	else
		RomDatas[9]=0xff;
	if(LeftTimesLAddr[LeftTimesM]==0x41)
	{
		RomData_WriteBytes(0x4b,RomDatas,10);//写入当前次数段，且使其不匹配
	}
	else
	{
		RomData_WriteBytes(0x41,RomDatas,10);//写入当前次数段，且使其不匹配
	}
}
//修复55及5f段数据
//使用中位次数进行容错
void Fix55and5Fdata(void)
{
	uint8_t t55,t5f,i;
	uint16_t C0BF;
	t55=RomData_ReadByte(0x5d);
	t5f=RomData_ReadByte(0x67);
	//用小的,点火开关去追步
	if(t55<t5f)
	{
		LeftTimesAddr[LeftTimesM]=0x41;
		LeftTimes[LeftTimesM]=t55;
	}
	else
	{
		LeftTimesAddr[LeftTimesM]=0x4b;
		LeftTimes[LeftTimesM]=t5f;
	}
	/************************************************************************/
	/* 开始容错使用次数段                                                   */
	/************************************************************************/	
	C0BF=0x80;
	C0BF=C0BF*LeftTimes[LeftTimesM];
	for(i=0;i<8;i++)
	{
		lcyIRDecodeIn[i]=RomDatas[7-i];
	}
	HashCalc_N(C0BF);
	for(i=0;i<8;i++)
	{
		RomDatas[7-i]=lcyIRDecodeOut[i];
	}
	RomDatas[8]=LeftTimes[1];
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
	RomData_WriteBytes(LeftTimesLAddr[LeftTimesH],RomDatas,10);//写入当前次数段，且使其匹配
	/************************************************************************/
	/* 继续容错另一段                                                       */
	/************************************************************************/
	HashCalc_N(0x80);
	for(i=0;i<8;i++)
	{
		RomDatas[7-i]=lcyIRDecodeOut[i];
	}
	RomDatas[8]=LeftTimes[LeftTimesM]-1;
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);	
	if(RomDatas[9]==0xff)
		RomDatas[9]=0;
	else
		RomDatas[9]=0xff;
	if(LeftTimesLAddr[LeftTimesM]==0x41)
	{
		RomData_WriteBytes(0x4b,RomDatas,10);//写入当前次数段，且使其不匹配
	}
	else
	{
		RomData_WriteBytes(0x41,RomDatas,10);//写入当前次数段，且使其不匹配
	}
}
//检查55级5f段数据
void Check55and5fdata(void)
{
	uint8_t t55,t5f,t;
	RomStateFlags.b55=0;
	RomStateFlags.b5f=0;	
	CheckDataBlockVerify(0x55);
	if(gFlags.bFuncRet)
	{
		RomStateFlags.b55=1;		
	}
	CheckDataBlockVerify(0x5f);
	if(gFlags.bFuncRet)
	{
		RomStateFlags.b5f=1;		
	}
	t=LeftTimes[0]&0xfc;//应该存储的次数
	t55=0xff;
	t5f=0xff;
	LeftTimesAddr[0]=0xff;//初值为错误值，用于判断是否出错
	if(RomStateFlags.b55 && RomStateFlags.b5f)//全匹配，使用次数多的
	{
		t55=RomData_ReadByte(0x55+8);
		t5f=RomData_ReadByte(0x5f+8);
		if(t55==t)
		{
			LeftTimesAddr[0]=0x55;//使用55段
		}
		if(t5f==t)
		{
			LeftTimesAddr[0]=0x5f;//使用5f段
		}
	}
	else if(RomStateFlags.b55)//只有一个匹配
	{
		t55=RomData_ReadByte(0x55+8);
		if(t55==t)
		{
			LeftTimesAddr[0]=0x55;//使用55段
		}
	}
	else if(RomStateFlags.b5f)//只有一个匹配
	{
		t5f=RomData_ReadByte(0x5f+8);
		if(t5f==t)
		{
			LeftTimesAddr[0]=0x5f;//使用55段
		}
	}
	if(LeftTimesAddr[0]==0xff)//没找到匹配的，需要容错
	{

	}
}
//sub_1442_USEDEC:
//使用次数减一
void UsedDEC(void)
{
	uint8_t x;
	GetCalcTimes69();	
	if(LeftTimes[0]==0)//0-1需要借位，写入0x7f
		x=0x7f;
	else
		x=LeftTimes[0]-1;
	RomData_WriteByte(LeftTimesLAddr,0xff);//原地址写0xff，写一个写入减一的
	if(LeftTimesLAddr==0x69)
	{	
		RomData_WriteByte(0x70,x);
	}
	else
	{
		RomData_WriteByte(LeftTimesLAddr-1,x);
	}
}

//每使用4次更新数据存储区
//sub_1490
void UpdateStepDatas(void)
{
	uint8_t c4,x,ram9b,i;
	if(RomStateFlags.b55  && RomStateFlags.b5f)
	{
		return;
	}
	if(!RomStateFlags.b41 && !RomStateFlags.b4b)//41、4B全不匹配，先容错
	{
		Adjust41and4BData();
	}
	if(RomStateFlags.b55 || RomStateFlags.b5f)
	{
		CalcTimes_D2=CalcTimes_D2-4;
	}
	Adjust55and5FData();
}
//55、5F段全不匹配时调用
//sub_14B1
void Adjust55and5FData(void)
{
	uint8_t addr,i;
	if(CalcTimes_D2>0x7f)
	{
		//全匹配时不进行借位操作
		if(!(RomStateFlags.b41 && RomStateFlags.b4b))//41、4B段全不匹配时,先修正
		{
			Adjust2Dand37Data();
		}
		CalcTimes_D2=0x7c;
		if(LeftTimesAddr[1]==0x41)
		{
			addr=0x4b;
		}
		else
		{
			addr=0x41;
		}
	}
	else
	{
		addr=LeftTimesAddr[1];
	}
	RomData_ReadBytes(addr,RomDatas,8);	
	if(CalcTimes_D2!=0)
	{
		for(i=0;i<8;i++)
		{
			lcyIRDecodeIn[i]=RomDatas[7-i];
		}
		HashCalc_N(CalcTimes_D2);
		for(i=0;i<8;i++)
		{
			RomDatas[7-i]=lcyIRDecodeOut[i];
		}
	}
	RomDatas[8]=CalcTimes_D2;
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
	if(LeftTimesAddr[0]==0x55)
	{
		RomData_ReadBytes(0x5f,RomDatas,10);
	}
	else
	{
		RomData_ReadBytes(0x55,RomDatas,10);
	}
	Check55and5fdata();
}
//41、4B段全不匹配时调用
//sub_1562
void Adjust41and4BData(void)
{
	uint8_t x,i;
	uint16_t C0BF;
	if(RomStateFlags.b41 && RomStateFlags.b4b)
		return;//全匹配则返回
	if(!RomStateFlags.b2d && !RomStateFlags.b37)//2D、37段全不匹配时,先修正
	{
		Adjust2Dand37Data();
	}	
	if(LeftTimes[1]==0)//中位使用次数用尽
	{
		LeftTimes[1]=0x1f;
		if(RomStateFlags.b2d && RomStateFlags.b37)//2D、37段不是全匹配时,先修正
		{
			Adjust2Dand37Data();
		}		
		if(GetBit(LeftTimes[2],0))
		{
			RomData_ReadBytes(0x2d,RomDatas,8);
		}
		else
		{
			RomData_ReadBytes(0x37,RomDatas,8);
		}
	}
	else
	{
		LeftTimes[1]--;
		if(GetBit(LeftTimes[2],0))
		{
			RomData_ReadBytes(0x37,RomDatas,8);
		}
		else
		{
			RomData_ReadBytes(0x2d,RomDatas,8);
		}
	}
	if(LeftTimes[1]!=0)
	{		
		C0BF=0x80;
		C0BF=C0BF*LeftTimes[1];
		for(i=0;i<8;i++)
		{
			lcyIRDecodeIn[i]=RomDatas[7-i];
		}
		HashCalc_N(C0BF);
		for(i=0;i<8;i++)
		{
			RomDatas[7-i]=lcyIRDecodeOut[i];
		}
	}
	RomDatas[8]=LeftTimes[1];
	RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
	if(GetBit(LeftTimes[1],0))
	{
		RomData_ReadBytes(0x41,RomDatas,10);
	}
	else
	{
		RomData_ReadBytes(0x4b,RomDatas,10);
	}
	Fix41and4Bdata();
}
//2D、37段全不匹配时调用
//sub_15BE
void Adjust2Dand37Data(void)
{
	uint8_t x,ram9b,i;
	if(RomStateFlags.b2d && RomStateFlags.b37)
		return;//全匹配则返回
	if(LeftTimes[2]==0)//使用次数用尽
	{
		Fix2Dand37data();
		return;
	}
	if(!RomStateFlags.b2d)//若2d段不匹配，则将7e段数据加载到2d段中
	{
		IRTx_10_33_SSID();
		RomData_ReadBytes(0x7e,RomDatas,8);
		x=RomData_ReadByte(0x86);
		x=x&0x7f;
		RomDatas[8]=x;
		RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
		RomData_WriteBytes(0x2d,RomDatas,10);
	}
	if(!RomStateFlags.b37)//若37段不匹配
	{
		IRTx_10_33_SSID();
		ram9b=0;
		CheckDataBlockVerify(0x7e);
		if(gFlags.bFuncRet)
		{
			x=RomData_ReadByte(0x86);
			if(x!=0xff)
			{
				ram9b=x;
			}
		}		
		LeftTimes[2]--;
		if(GetBit(ram9b,0))//单数次使用37段数据计算
		{
			RomData_ReadBytes(0x37,RomDatas,8);
		}
		if(ram9b==0)//37、7E段均不匹配，根据当前的次数从rom中重新生成高位hash
		{
			//sub_1643
			IRTx_10_33_SSID();
			if(LeftTimes[2]>=0x24)
			{
				ram9b=0x24^0x80;
				RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
			}
			else if(LeftTimes[2]>=0x12)
			{
				ram9b=0x12^0x80;
				RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
			}
			else
			{
				ram9b=0x00^0x80;
				RomData_ReadBytes(0x15,RomDatas,8);//49152
			}
		}		
		while(1)
		{
			ram9b++;
			x=ram9b&0x7f;
			if(x==LeftTimes[2])
			{
				ram9b=LeftTimes[2];
			}
			for(i=0;i<8;i++)
			{
				lcyIRDecodeIn[i]=RomDatas[7-i];
			}
			HashCalc_N(4096);
			for(i=0;i<8;i++)
			{
				RomDatas[7-i]=lcyIRDecodeOut[i];
			}
			RomDatas[8]=ram9b;
			RomDatas[9]=GetVerify_byteXOR(RomDatas,9);				
			if(GetBit(ram9b,0))//单数写入0x37段
			{
				RomData_WriteBytes(0x37,RomDatas,10);
			}
			else//偶数写入0x37段
			{
				RomData_WriteBytes(0x7e,RomDatas,10);
			}
			if(ram9b<0x80)
				break;
		}
		Fix2Dand37data();
	}
}
//反转指定地址的数据，若为0xff则写为0，反之若不是0xff则写为0xff
void ReverseRom(uint8_t addr)
{
	if(RomData_ReadByte(addr)!=0xff)
	{
		RomData_WriteByte(addr,0xff);
	}
	else
	{
		RomData_WriteByte(addr,0);
	}
}
//sub_13EB_Get69to70_FirstNFF
//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
//70-6f-6e-6d-6c-6b-6a-69-70
void GetCalcTimes69(void)
{
	uint8_t i,x;
	RomData_ReadBytes(0x69,RomDatas,8);
	for(i=8;i!=0;i--)
	{
		x=i-1;
		if(RomDatas[x]!=0)
			break;
	}
	if(i==0)//没有非ff单元,出错，将0x69写入00，最少的低位次数
	{
		LeftTimes69=0;
		RomData_WriteByte(0x69,0);
// 		CalcTimes_D2=0;
// 		CalcTimes_BF=0;
		LeftTimesLAddr=0x69;
		return;
	}
	else
	{
		//容错：再判断下一个是否也为非ff
		if(i==1)//无需再判断
		{
			LeftTimes69=RomDatas[0];
// 			CalcTimes_D2=CalcTimes[0]&0xfc;
// 			CalcTimes_BF=CalcTimes[0]&0x03;
			LeftTimesLAddr=0x69;
			return;
		}
		else 
		{
			x=i-2;
			if(RomDatas[x]!=0xff)//出现了两个，使用下一个
			{
				LeftTimes69=RomDatas[x];
// 				CalcTimes_D2=CalcTimes[0]&0xfc;
// 				CalcTimes_BF=CalcTimes[0]&0x03;
				LeftTimesLAddr=0x69+x;				
				ReverseRom(0x68+i);//修正rom
			}
			else//没错，则使用此值
			{
				x=i-1;
				LeftTimes69=RomDatas[x];
// 				CalcTimes_D2=CalcTimes[0]&0xfc;
// 				CalcTimes_BF=CalcTimes[0]&0x03;
				LeftTimesLAddr=0x69+x;
			}
		}
		//rom容错:再判断前一个是否也为非ff,只有70单元时需要判断
		if(i==8)//70单元
		{
			if(RomDatas[0]!=0xff)
			{
				ReverseRom(0x69);
			}
		}
	}
}
void HashCalc_N(uint32_t nCount)
{
	uint32_t i;
	for(i=0;i<nCount;i+=4)
	{
		lcyHashCalc(4);
		IRTx_10_33_SSID();
	}
}
//检查数据段校验
void CheckDataBlockVerify(uint8_t Addr)
{
	uint8_t x;
	gFlags.bFuncRet=0;	
	RomData_ReadBytes(Addr,RomDatas,10);
	if(RomDatas[8]>0x7f)
	{
		gFlags.bFuncRet=0;
		return;
	}
	x=GetVerify_byteXOR(RomDatas,9);
	if(x==RomDatas[9])
		gFlags.bFuncRet=1;
}

