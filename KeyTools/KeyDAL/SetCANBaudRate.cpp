#include "SetCANBaudRate.h"
#include "GlobalFunc.h"
#include "Serial.h"
#include "ConstDefine.h"
#include "VMProtectSDK.h"

SetCANBaudRateCommand src;
bool __stdcall SetCANBaudRate( int brType )
{
	VMProtectBegin("SetCANBaudRate");
	nErrorCode=ErrorCode_NoErr;
 	if(brType==0)//83.3k
 	{
 		src.Prescaler = 63;
 		src.BS1 = 6;
 		src.BS2 = 1;
 		src.SJW = 1;
		src.RNum=0;
 	}
 	else//500k
 	{
 		src.Prescaler = 6;
 		src.BS1 = 11;
 		src.BS2 = 2;
 		src.SJW = 1;
		src.RNum=3;
 	}
 	src.MakePacket();
	if(gSerialProcCallback==NULL)
	{
		nErrorCode=ErrorCode_NoSerialProcCallback;
		return false;
	}
	MakeTxPacket(Command_SetCANBaudRate,src.Packet,SetCANBaudRateCommand_PacketLen);
	if(gSerialProcCallback(Command_SetCANBaudRate,stm32f4_TxData,0,stm32f4_TxDataLen,500,3)==false)
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
	//if()
}

SetCANBaudRateCommand::SetCANBaudRateCommand(void)
{

}


SetCANBaudRateCommand::~SetCANBaudRateCommand(void)
{
}

void SetCANBaudRateCommand::MakePacket()
{
	Packet[0] = GetLowByte(Prescaler);
	Packet[1] = GetHighByte(Prescaler);
	Packet[2] = BS1;
	Packet[3] = BS2;
	Packet[4] = SJW;
	Packet[5] = Param;      
	Packet[6]=RNum;
}
