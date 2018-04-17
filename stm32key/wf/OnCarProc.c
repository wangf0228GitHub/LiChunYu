#include "OnCarProc.h"
#include "Function.h"
#include "Variables.h"
#include "wfEEPROM.h"
#include "IRProc.h"
#include "Verify.h"
#include "wfDefine.h"
#include "lcyIRDecode.h"

void OnCarProc(void)
{
	GetROMState();
	if(!RomStateFlags.bRomWrited)//ROM未被烧写过
	{

	}
}
//2D、37段全不匹配时调用
//sub_15BE
void Adjust2Dand37Data(void)
{
	uint8_t c4,x,ee08,ram9b,i;
	if(RomStateFlags.b2d && RomStateFlags.b37)
		return;//全匹配则返回
	c4=RomData_ReadByte(StepTimesAddr[0]+8);
	if(c4==0)//使用次数用尽
	{
		Check2Dand37data();
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
		ee08=RomData_ReadByte(0x08);
		ram9b=0;
		if(ee08!=0xff)
		{
			CheckDataBlockVerify(0x7e);
			if(gFlags.bFuncRet)
			{
				ram9b=ee08;
			}
		}
		c4--;
		if(GetBit(ram9b,0))
		{
			RomData_ReadBytes(0x37,RomDatas,8);
		}
		if(ram9b==0)
		{
			//sub_1643
			IRTx_10_33_SSID();
		}
		while(1)
		{
			ram9b++;
			x=ram9b&0x7f;
			x=c4-x;
			if(x==0)
			{
				ram9b=c4;
			}
			for(i=0;i<8;i++)
			{
				lcyIRDecodeIn[i]=RomDatas[7-i];
			}
		}
	}
}
void Get2425TxParam(void)
{
	Check55and5fdata();
	Check41and4bdata();
	Check2Dand37data();
}
