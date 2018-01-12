#include "Serial.h"
#include "GlobalFunc.h"
#include "Verify.h"
#include "ConstDefine.h"
#include "VMProtectSDK.h"
#include "CANReadEEPROM.h"

int nErrorCode;

byte stm32f4_TxData[1024];
int stm32f4_TxDataLen;
stm32f4_RxPacket RxPacket;

#define stm32f4_TxPacket_HeaderSize 8
void MakeTxPacket( unsigned short com,byte* data,unsigned short len )
{
	//VMProtectBegin("MakeTxPacket");
	RxPacket.Reset();
	if(data!=NULL)
	{
		stm32f4_TxData[0] = 0x16;
		stm32f4_TxData[1] = 0x16;
		stm32f4_TxData[2] = GetLowByte(com);
		stm32f4_TxData[3] = GetHighByte(com);                
		stm32f4_TxData[4] = GetLowByte(len);
		stm32f4_TxData[5] = GetHighByte(len);
		stm32f4_TxData[6] = 0x00;
		stm32f4_TxData[7] = 0x00;
		//                 stm32f4_TxData[6] = BytesOP.GetLowByte(retry);
		//                 stm32f4_TxData[7] = BytesOP.GetHighByte(retry);
		//                 stm32f4_TxData[8] = BytesOP.GetLowByte(timeout);
		//                 stm32f4_TxData[9] = BytesOP.GetHighByte(timeout); 
		for (int i = 0; i < len; i++)
		{
			stm32f4_TxData[i + stm32f4_TxPacket_HeaderSize] = data[i];
		}
		stm32f4_TxData[stm32f4_TxPacket_HeaderSize + len] = GetVerify_byteSum(stm32f4_TxData,0, stm32f4_TxPacket_HeaderSize + len);
		stm32f4_TxData[stm32f4_TxPacket_HeaderSize + len + 1] = 0x0d;
		stm32f4_TxDataLen=stm32f4_TxPacket_HeaderSize + len + 2;
	}
	else
	{
		stm32f4_TxData[0] = 0x16;
		stm32f4_TxData[1] = 0x16;
		stm32f4_TxData[2] = GetLowByte(com);
		stm32f4_TxData[3] = GetHighByte(com);
		stm32f4_TxData[4] = 0;
		stm32f4_TxData[5] = 0;
		stm32f4_TxData[6] = 0;
		stm32f4_TxData[7] = 0;
		//                 stm32f4_TxData[6] = BytesOP.GetLowByte(retry);
		//                 stm32f4_TxData[7] = BytesOP.GetHighByte(retry);
		//                 stm32f4_TxData[8] = BytesOP.GetLowByte(timeout);
		//                 stm32f4_TxData[9] = BytesOP.GetHighByte(timeout); 
		stm32f4_TxData[stm32f4_TxPacket_HeaderSize] = GetVerify_byteSum(stm32f4_TxData, 0,stm32f4_TxPacket_HeaderSize);
		stm32f4_TxData[stm32f4_TxPacket_HeaderSize + 1] = 0x0d;
		stm32f4_TxDataLen=stm32f4_TxPacket_HeaderSize + 2;
	}        
	//VMProtectEnd();
}

void __stdcall InitDllWork(SerialProcCallback _SerialProcCallback,SetProgressCallback _SetProgressCallback)
{
	//VMProtectBegin("InitDllWork");
	gSerialProcCallback=_SerialProcCallback;
	gSetProgressCallback=_SetProgressCallback;
	initCANReadEEPROM();
	//VMProtectEnd();
}

bool __stdcall ProcRxByte( byte rx )
{
	VMProtectBegin("ProcRxByte");
	return RxPacket.DataPaketed(rx);
	VMProtectEnd();
}

void __stdcall GetRxData( byte* pBuf,int& len )
{
	VMProtectBegin("GetRxData");
	for(int i=0;i<RxPacket.RxCount;i++)
		pBuf[i]=RxPacket.Data[i];
	len=RxPacket.RxCount;
	VMProtectEnd();
}

int __stdcall GetErrorCode()
{
	VMProtectBegin("GetErrorCode");
	return nErrorCode;
	VMProtectEnd();
}

bool __stdcall CheckDevice()
{	
	VMProtectBegin("CheckDevice");
	if(gSerialProcCallback==NULL)
	{
		nErrorCode=ErrorCode_NoSerialProcCallback;
		return false;
	}
	MakeTxPacket(Command_CheckDevice,NULL,0);
	if(gSerialProcCallback(Command_CheckDevice,stm32f4_TxData,0,stm32f4_TxDataLen,300,3)==false)
	{
		nErrorCode=ErrorCode_SerialProcError;
		return false;
	}	
	if(RxPacket.bError==0)
		return true;
	else
	{
		nErrorCode=RxPacket.ErrCode;
		return false;
	}
	VMProtectEnd();
}
