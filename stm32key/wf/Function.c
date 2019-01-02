#include "Function.h"
#include "wfEEPROM.h"
#include "Variables.h"
#include "..\..\..\WF_Device\wfDefine.h"
#include "..\..\..\WF_Device\Verify.h"
#include "lcyHash.h"
#include "OnCarProc.h"

void GetDoorProc(uint8_t keyValue)
{
	uint8_t i,x,addr;
	uint8_t ram93[8];
// 	if(EE00==0)
// 		RomStateFlags.bStudy=1;
// 	if(!RomStateFlags.bStudy)
// 		keyValue=0x43;
	/************************************************************************/
	/* 生成要送的钥匙数据                                                   */
	/************************************************************************/
	//1.获得计数值
//  	addr=LeftTimes69&0x03;
//  	addr=addr+0x90;
//  	ButtonTimes=RomData_ReadByte(addr);
// 	//2,加载ssid
 	//3,当前hash滚一步
   	for(i=0;i<8;i++)
   		ram93[i]=EISHash[i];

	//4,与密码异或
	for(i=0;i<8;i++)
		DoorDatas[i]=ram93[i]^PSW[i];
	//5
	if(!RomStateFlags.bStudy)
	{	
		for(i=0;i<8;i++)
			DoorDatas[i]=PSW[i];
	}
	//6
	DoorDatas[0]^=ram93[0];
	DoorDatas[6]^=ButtonTimes;
	DoorDatas[7]^=keyValue;
	for(i=0;i<8;i++)
		lcyHashIn[i]=DoorDatas[i];
	lcyHashOnce();
	//7
//	x=ButtonTimes+1;
//	RomData_WriteByte(addr,x);
	//8
	if(RomStateFlags.bStudy)
	{
		for(i=0;i<8;i++)
			DoorDatas[i]=ram93[i]^lcyHashOut[i];
	}
	else
	{
		for(i=0;i<8;i++)
			DoorDatas[i]=lcyHashOut[i];
	}
}
//生成回应车载端的数据，响应指令为红外0x26，或无线0x50
//responseCommander为响应命令：0x26或0x50
void GetKeyWorkValue(uint8_t* rxList,uint8_t responseCommander)
{
	uint8_t i;
	for(i=0;i<8;i++)//密码与接收到的数据逐个异或
	{
		WorkValueDatas[i]=PSW[i]^rxList[i];
	}
	//后4字节与2425指令发送数据异或
	WorkValueDatas[4]^=LeftTimes69;
	WorkValueDatas[5]^=LeftTimes[LeftTimesM];
	WorkValueDatas[6]^=LeftTimes[LeftTimesH];
	if(responseCommander==0x26)
	{
		if(RomStateFlags.bStudy)
			WorkValueDatas[7]^=0x24;
		else
			WorkValueDatas[7]^=0x25;
	}	
	else//0x50
	{
		WorkValueDatas[7]^=0xca;
	}
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=WorkValueDatas[i];
	}
	lcyHashOnce();		
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	for(i=0;i<8;i++)
	{
		WorkValueDatas[i]=lcyHashOut[i]^curHash[i];
	}	
}
void GetKeyParam(void)
{
	uint8_t i, x,addr,t1;	
	if(!RomStateFlags.bRomWrited)
		return;
	//写过了，获取系统参数
	EE00 = RomData_ReadByte(0x00);
	EE00 >>= 6;
	RomData_ReadBytes(0x01, PSW, 8);
	RomData_ReadBytes(0x09, SSID, 4);
	//使用次数低位
	//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
	//70-6f-6e-6d-6c-6b-6a-69-70
	GetCalcTimes69();
	//使用次数高位
	GetLeftTimeBlock(LeftTimesH);
	//使用次数中位
	GetLeftTimeBlock(LeftTimesM);
	//使用次数低位所用段，同时找到当前hash
	GetLeftTimeBlock(LeftTimesL);	
	x = LeftTimes69 & 0x03;
	if (x == 0)
	{
		RomData_ReadBytes(LeftTimesAddr[LeftTimesL], RomDatas, 8);
		for (i = 0; i < 8; i++)
		{
			curHash[i] = RomDatas[7 - i];
		}
	}
	else
	{
		addr=0xd0+x*10;
		CheckDataBlockVerify(addr);
		t1 = RomDatas[8];
		if(!gFlags.bFuncRet || RomDatas[9]==0 || t1!=LeftTimes69)//当前存储器中没有当前哈希值，则生成
		{
			RomData_ReadBytes(LeftTimesAddr[LeftTimesL], RomDatas, 8);
			for (i = 0; i < 8; i++)
			{
				lcyHashIn[i] = RomDatas[7 - i];
			}
			lcyHashCalc(x);
			for (i = 0; i < 8; i++)
			{
				RomDatas[7 - i] = lcyHashOut[i];
			}
			RomDatas[8] = LeftTimes69;
			RomDatas[9] = GetVerify_byteXOR(RomDatas,9);	
			RomData_WriteBytes(addr, RomDatas, 10);//写入当前次数段，且使其匹配
			for (i = 0; i < 8; i++)
			{
				curHash[i] = RomDatas[7 - i];
			}
		}
		else//正确，取出hash
		{
			for (i = 0; i < 8; i++)
			{
				curHash[i] = RomDatas[7 - i];
			}
		}						
	}
	for(i=0;i<8;i++)
		lcyHashIn[i]=curHash[i];
	lcyHashOnce();
	for(i=0;i<8;i++)
		EISHash[i]=lcyHashOut[i];
	addr=LeftTimes69&0x03;
	addr=addr+0x90;
	ButtonTimes=RomData_ReadByte(addr);
}

//获得使用次数
void GetLeftTimeBlock(uint8_t nBlock)
{
	uint8_t t1, t2, addr1, addr2, tTop;
	switch (nBlock)
	{
	case LeftTimesH:
		addr1 = 0x2d;
		addr2 = 0x37;
		tTop = 0x2f;
		break;
	case LeftTimesM:
		addr1 = 0x41;
		addr2 = 0x4b;
		tTop = 0x1f;
		break;
	case LeftTimesL:
		addr1 = 0x55;
		addr2 = 0x5f;
		tTop = 0x00;
		break;
	default:
		return;
	}
	CheckDataBlockVerify(addr1);
	t1 = RomDatas[8];
	if(!gFlags.bFuncRet)
	{
		t1 = 0xff;
	}
	CheckDataBlockVerify(addr2);
	t2 = RomDatas[8];
	if(!gFlags.bFuncRet)
	{
		t2 = 0xff;
	}
	LeftTimesAddr[nBlock] = 0xff;//异常值
	if ((t1 != 0xff) && (t2 != 0xff))
	{
		if (nBlock != LeftTimesL)
		{
			if (t1 > t2)//用大的
			{
				if ((t2 == 0) && (t1 == tTop))//例如：00 1f则使用00
				{
					LeftTimesAddr[nBlock] = addr2;
					OtherLeftTimesAddr[nBlock] = addr1;
					LeftTimes[nBlock] = t2;
				}
				else//否则正常使用大的
				{
					LeftTimesAddr[nBlock] = addr1;
					OtherLeftTimesAddr[nBlock] = addr2;
					LeftTimes[nBlock] = t1;
				}
			}
			else//t2>t1
			{
				if ((t1 == 0) && (t2 == tTop))//例如：00 1f则使用00
				{
					LeftTimesAddr[nBlock] = addr1;
					OtherLeftTimesAddr[nBlock] = addr2;
					LeftTimes[nBlock] = 0;
				}
				else//否则正常使用大的
				{
					LeftTimesAddr[nBlock] = addr2;
					OtherLeftTimesAddr[nBlock] = addr1;
					LeftTimes[nBlock] = t2;
				}
			}
			return;//返回
		}
		else
		{
			tTop = LeftTimes69 & 0xfc;
			if (t1 == tTop)
			{
				LeftTimesAddr[nBlock] = addr1;
				OtherLeftTimesAddr[nBlock] = addr2;
				LeftTimes[nBlock] = t1;
				return;
			}
			else if (t2 == tTop)
			{
				LeftTimesAddr[nBlock] = addr2;
				OtherLeftTimesAddr[nBlock] = addr1;
				LeftTimes[nBlock] = t2;
				return;
			}
			else//两段都不匹配，需要重新生成低位hash
			{
				t1 = 0xff;
				t2 = 0xff;
			}
		}
	}
	else if (t1 != 0xff)//第一段匹配，则重新生成第二段
	{
		LeftTimesAddr[nBlock] = addr1;
		OtherLeftTimesAddr[nBlock] = addr2;
		LeftTimes[nBlock] = t1;
	}
	else if (t2 != 0xff)
	{
		LeftTimesAddr[nBlock] = addr2;
		OtherLeftTimesAddr[nBlock] = addr1;
		LeftTimes[nBlock] = t2;
	}
	//IRTx_10_33_SSID();
	if (t1 != 0xff || t2 != 0xff)//需要重新生成第二段
	{

		switch (nBlock)
		{
		case LeftTimesH:
			if (LeftTimes[LeftTimesH] == 0)//高位为0则不再生成第二段
				break;
			else
			{							
				t2 = LeftTimes[LeftTimesH] - 1;
				if (t2 >= 0x24)
				{
					t1 = 0x24;
					addr1 = 0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152								
				}
				else if (t2 >= 0x18)
				{
					t1 = 0x18;
					addr1 = 0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
				}
				else if (t2 >= 0xc)
				{
					t1 = 0x0c;
					addr1 = 0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
				}
				else
				{
					t1 = 0x00;
					addr1 = 0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
				}
				// 							if ((LeftTimes[LeftTimesH] == 0x2e) ||
				// 								(LeftTimes[LeftTimesH] == 0x24) ||
				// 								(LeftTimes[LeftTimesH] == 0x18) ||
				// 								(LeftTimes[LeftTimesH] == 0x0c)
				// 								)//此时生成高位段4096hash
				// 							{
				// 								RomData_ReadBytes(addr1, ref RomDatas, 8);
				// 								addr2 = 0xa0;
				// 								RomData_WriteBytes(addr2, RomDatas, 8);
				// 								for (i = 0; i < 8; i++)
				// 								{
				// 									lcyHashIn[i] = RomDatas[7 - i];
				// 								}
				// 								for (j = 1; j < 12; j++)
				// 								{
				// 									//HashCalc_N(4096);
				// 									// 				for (i = 0; i < 8; i++)
				// 									// 				{
				// 									// 					RomDatas[7 - i] = lcyHashOut[i];
				// 									// 				}
				// 									lcyHashIn = LCYHashCal.HashCal(lcyHashIn, 4096);
				// 									for (i = 0; i < 8; i++)
				// 									{
				// 										RomDatas[7 - i] = lcyHashIn[i];
				// 									}
				// 									addr2 += 8;
				// 									RomData_WriteBytes(addr2, RomDatas, 8);//写入当前次数段，且使其匹配
				// 								}
				// 							}
				t1 = t2 - t1;
				// 							addr2=(uint8_t)(0xa0+(t1<<3));
				// 							RomData_ReadBytes(addr2, ref RomDatas, 8);
				// 							RomDatas[8] = (uint8_t)(LeftTimes[LeftTimesH] - 1);
				// 							//RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				// 							RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				// 							RomData_WriteBytes(OtherLeftTimesAddr[LeftTimesH], RomDatas, 10);//写入当前次数段，且使其匹配
				FixDataBlock(OtherLeftTimesAddr[LeftTimesH], addr1, 4096, t1, LeftTimes[LeftTimesH] - 1);
			}
			break;
		case LeftTimesM:
			if (LeftTimes[LeftTimesM] == 0)//中位为0，则需要向高位借位
			{
				if (LeftTimes[LeftTimesH] == 0)//高位为0，则无法借位，不再生成中位第二段
					break;
				else//借位生成中位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesH], 128, 0x1f, 0x1f);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesM], LeftTimesAddr[LeftTimesH], 128, LeftTimes[LeftTimesM] - 1, LeftTimes[LeftTimesM] - 1);
			}
			break;
		case LeftTimesL:
			if (LeftTimes[LeftTimesL] == 0)//低位为0，则需要向中位借位
			{
				if ((LeftTimes[LeftTimesM] == 0) && (LeftTimes[LeftTimesH] == 0))//中位、高位都为0，则无法借位，不再生成低位第二段
					break;
				else//借位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesM], 0x7c, 1, 0x7c);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesL], LeftTimesAddr[LeftTimesM], LeftTimes[LeftTimesL] - 4, 1, LeftTimes[LeftTimesL] - 4);
			}
			break;
		}
	}
	else//两段都不匹配，需要用小的次数从新生成
	{
		t1 = RomData_ReadByte(addr1+8);
		t2 = RomData_ReadByte(addr2 + 8);
		if (t1 < t2)
		{
			LeftTimesAddr[nBlock] = addr1;
			OtherLeftTimesAddr[nBlock] = addr2;
			LeftTimes[nBlock] = t1;
		}
		else
		{
			LeftTimesAddr[nBlock] = addr2;
			OtherLeftTimesAddr[nBlock] = addr1;
			LeftTimes[nBlock] = t2;
		}
		switch (nBlock)
		{
		case LeftTimesH:
			if (LeftTimes[LeftTimesH] == 0)//高位为0则仅生成第一段
			{
				RomData_ReadBytes(0x0d, RomDatas, 8);
				RomDatas[8] = 0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				//RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				RomData_WriteBytes(LeftTimesAddr[LeftTimesH], RomDatas, 10);//写入当前次数段，且使其匹配
			}
			else
			{
				t2 = LeftTimes[LeftTimesH] - 1;
				if (t2 >= 0x24)
				{
					t1 = 0x24;
					addr1 = 0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
				}
				else if (t2 >= 0x18)
				{
					t1 = 0x18;
					addr1 = 0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
				}
				else if (t2 >= 0xc)
				{
					t1 = 0x0c;
					addr1 = 0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
				}
				else
				{
					t1 = 0x00;
					addr1 = 0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
				}
				t1 = t2 - t1;
				FixDataBlock(OtherLeftTimesAddr[LeftTimesH], addr1, 4096, t1, LeftTimes[LeftTimesH] - 1);//修复-1段
				FixDataBlock(LeftTimesAddr[LeftTimesH], OtherLeftTimesAddr[LeftTimesH], 4096, 1, LeftTimes[LeftTimesH]);//修复当前
			}
			break;
		case LeftTimesM:
			if (LeftTimes[LeftTimesM] == 0)//中位为0，则需要向高位借位
			{
				RomData_ReadBytes(LeftTimesAddr[LeftTimesH], RomDatas, 8);
				RomDatas[8] = 0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				//RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				RomData_WriteBytes(LeftTimesAddr[LeftTimesM], RomDatas, 10);
				if (LeftTimes[LeftTimesH] == 0)//高位为0，则仅生成当前段			
					break;
				else//借位生成中位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesH], 128, 0x1f, 0x1f);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesM], LeftTimesAddr[LeftTimesH], 128, LeftTimes[LeftTimesM] - 1, LeftTimes[LeftTimesM] - 1);
				FixDataBlock(LeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesM], 128, 1, LeftTimes[LeftTimesM]);
			}
			break;
		case LeftTimesL:
			LeftTimes[LeftTimesL] = LeftTimes69 & 0xfc;
			if (LeftTimes[LeftTimesL] == 0)//低位为0，则需要向中位借位
			{
				RomData_ReadBytes(LeftTimes[LeftTimesM], RomDatas, 8);
				RomDatas[8] = 0;
				RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
				//RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
				RomData_WriteBytes(LeftTimesAddr[LeftTimesL], RomDatas, 10);
				if ((LeftTimes[LeftTimesM] == 0) && (LeftTimes[LeftTimesH] == 0))//中位、高位都为0，则无法借位，不再生成低位第二段
					break;
				else//借位
				{
					FixDataBlock(OtherLeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesM], 0x7c, 1, 0x7c);
				}
			}
			else
			{
				FixDataBlock(OtherLeftTimesAddr[LeftTimesL], LeftTimesAddr[LeftTimesM], LeftTimes[LeftTimesL] - 4, 1, LeftTimes[LeftTimesL] - 4);
				FixDataBlock(LeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesL], 4, 1, LeftTimes[LeftTimesL]);
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
	RomData_ReadBytes(hashAddr, RomDatas, 8);
	if ((loopTimes != 0) && (stepLen!=0))
	{
		for (i = 0; i < 8; i++)
		{
			lcyHashIn[i] = RomDatas[7 - i];
		}
		for (i = 0; i < loopTimes; i++)
		{
		 	HashCalc_N(stepLen);
		}
		for (i = 0; i < 8; i++)
		{
		 	RomDatas[7 - i] = lcyHashOut[i];
		}
// 		for (i = 0; i < loopTimes; i++)
// 		{
// 			lcyHashIn=LCYHashCal.HashCal(lcyHashIn, stepLen);					
// 		}
// 		for (i = 0; i < 8; i++)
// 		{
// 			RomDatas[7 - i] = lcyHashIn[i];
// 		}
	}
	RomDatas[8] = leftTiems;
	RomDatas[9] = GetVerify_byteXOR(RomDatas,9);	
	RomData_WriteBytes(fixAddr, RomDatas, 10);//写入当前次数段，且使其匹配
}
//sub_1442_USEDEC:
//使用次数减一
void UsedDEC(void)
{
	uint8_t x;
	if (LeftTimes69 == 0)//0-1需要借位，写入0x7f
	{
		if (LeftTimes[LeftTimesM] == 0 && LeftTimes[LeftTimesH] == 0)//所有次数已经用尽
		{
			NVIC_SystemReset();//复位
		}
		else
		{
			ReverseRom(LeftTimesAddr[LeftTimesM] + 9);//破坏中位当前使用段					
			if (LeftTimes[LeftTimesM] == 0)//中段使用次数为零，则需要向高位借位
			{
				ReverseRom(LeftTimesAddr[LeftTimesH] + 9);
			}
			ReverseRom(LeftTimesAddr[LeftTimesL] + 9);//破坏低位当前使用段

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
// 			ReverseRom(OtherLeftTimesAddr[LeftTimesM] + 9);//破坏中位当前使用段					
// 			if (LeftTimes[LeftTimesM] == 0)//中段使用次数为零，则需要向高位借位
// 			{
// 				ReverseRom(OtherLeftTimesAddr[LeftTimesH] + 9);
// 			}
// 			ReverseRom(OtherLeftTimesAddr[LeftTimesL] + 9);//破坏低位当前使用段
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			LeftTimes69 = 0x7f;
		}
	}
	else
	{
		x = LeftTimes69 & 0x03;
		if (x == 0x00)//需要切换低位段
		{
			ReverseRom(LeftTimesAddr[LeftTimesL] + 9);//破坏低位当前使用段
		}
		LeftTimes69--;//使用次数减一
	}
	RomData_WriteByte(LeftTimes69Addr, 0xff);//原地址写0xff，写一个写入减一的
	if (LeftTimes69Addr == 0x69)
	{
		RomData_WriteByte(0x70, LeftTimes69);
	}
	else
	{
		RomData_WriteByte(LeftTimes69Addr - 1, LeftTimes69);
	}	
}
//反转指定地址的数据，若为0xff则写为0，反之若不是0xff则写为0xff
void ReverseRom(uint8_t addr)
{
	if (RomData_ReadByte(addr) != 0xff)
	{
		RomData_WriteByte(addr, 0xff);
	}
	else
	{
		RomData_WriteByte(addr, 0);
	}
}
//sub_13EB_Get69to70_FirstNFF
//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
//70-6f-6e-6d-6c-6b-6a-69-70
void GetCalcTimes69(void)
{
	uint8_t i, x;
	RomData_ReadBytes(0x69, RomDatas, 8);
	for (i = 8; i != 0; i--)
	{
		x = i - 1;
		if (RomDatas[x] != 0xff)
			break;
	}
	if (i == 0)//没有非ff单元,出错，将0x69写入00，最少的低位次数
	{
		LeftTimes69 = 0;
		RomData_WriteByte(0x69, 0);
		// 		CalcTimes_D2=0;
		// 		CalcTimes_BF=0;
		LeftTimes69Addr = 0x69;
		return;
	}
	else
	{
		//容错：再判断下一个是否也为非ff
		if (i == 1)//无需再判断
		{
			LeftTimes69 = RomDatas[0];
			// 			CalcTimes_D2=CalcTimes[0]&0xfc;
			// 			CalcTimes_BF=CalcTimes[0]&0x03;
			LeftTimes69Addr = 0x69;
			return;
		}
		else
		{
			x = i - 2;
			if (RomDatas[x] != 0xff)//出现了两个，使用下一个
			{
				LeftTimes69 = RomDatas[x];
				// 				CalcTimes_D2=CalcTimes[0]&0xfc;
				// 				CalcTimes_BF=CalcTimes[0]&0x03;
				LeftTimes69Addr = 0x69 + x;
				ReverseRom(0x68 + i);//修正rom
			}
			else//没错，则使用此值
			{
				x = i - 1;
				LeftTimes69 = RomDatas[x];
				// 				CalcTimes_D2=CalcTimes[0]&0xfc;
				// 				CalcTimes_BF=CalcTimes[0]&0x03;
				LeftTimes69Addr =0x69 + x;
			}
		}
		//rom容错:再判断前一个是否也为非ff,只有70单元时需要判断
		if (i == 8)//70单元
		{
			if (RomDatas[0] != 0xff)
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
		if(bOnCarPower())
			CarIRTx_10_33_SSID();
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
void GetKeyState(void)
{
	uint8_t i, x;
	RomData_ReadBytes(0x9e,RomDatas, 2);
	x = RomDatas[0];
	x += RomDatas[1];
	if((RomDatas[0]==0x00) || (x!=0x00))//内存状态校验失败
	{
		ChangeKeyState(ROM_9E);
	}	
	RomData_ReadBytes(0x9e, RomDatas, 2);
	x = RomDatas[0];
	x += RomDatas[1];
	RomStateFlags.bRomWrited = 0;
	RomStateFlags.bStudy = 0;
	if (x != 0)
		NVIC_SystemReset();//存储区无法初始化，系统复位
	EE9e=RomDatas[0]&0x7f;
	x=EE9e&0xee;
	if (x==0x04)//写过了
	{
		RomStateFlags.bRomWrited = 1;
	}
	if (GetBit(RomDatas[0], 4) != 0)//学习过
	{
		RomStateFlags.bStudy = 1;
	}
}
void ChangeKeyState(uint8_t state)
{
	uint8_t x[2];
	x[0]=state;
	x[1]=0x00-state;
	RomData_WriteBytes(0x9e,x,2);
}
void VerifyEEDatas(uint8_t maxNum,uint8_t lastAddr)
{
	uint8_t i,j,addr;
	uint8_t ee[8];
	gFlags.bFuncRet=0;
	switch(CarIRCommand)
	{
	case 0x0e:
		addr=0x88;
		break;
	case 0x00:
		addr=0x80;
		break;
	case 0x03:
		addr=0xfe;
		break;
	default:
		addr=0x00;
		break;
	}
	for(i=0;i<8;i++)
		ee[i]=0x00;
	for(i=0;i<maxNum;i++)
	{
		RomData_ReadBytes(addr,RomDatas,8);
		if(addr==0xfe)
		{
			RomDatas[0]=0;
			RomDatas[1]=0;
		}
		for(j=0;j<8;j++)
		{
			lcyHashIn[i]=RomDatas[i]^ee[i];
		}
		lcyHashOnce();
		for(j=0;j<8;j++)
		{
			ee[i]=lcyHashOut[i];
		}
		addr+=8;
	}
	RomData_ReadBytes(lastAddr,RomDatas,8);
	for(i=0;i<8;i++)
	{
		if(ee[i]!=RomDatas[i])
			return;
	}
	gFlags.bFuncRet=1;
}
