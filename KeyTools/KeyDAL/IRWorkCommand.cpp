#include "IRWorkCommand.h"
#include "GlobalFunc.h"
#include "ConstDefine.h"
#include "VMProtectSDK.h"
#include "Serial.h"

CIRWorkCommand irWork;
bool __stdcall IRWork( int bOpen )
{
	VMProtectBegin("IREn");
	byte IRTxDataList[11]; 
	IRTxDataList[0]=0x10;
	IRTxDataList[1]=0x37;
	IRTxDataList[2]=0x00;
	IRTxDataList[3]=0x43;
	IRTxDataList[4]=0x3c;
	IRTxDataList[5]=0x02;
	IRTxDataList[6]=0x52;
	IRTxDataList[7]=0x02;
	IRTxDataList[8]=0xda;
	IRTxDataList[9]=0xb3;
	IRTxDataList[10]=0x27;
	irWork.count=11;
	for(int i=0;i<11;i++)
		irWork.data[i]=IRTxDataList[i];
	irWork.MakePacket();
	MakeTxPacket(Command_IRWork,irWork.Packet,2+irWork.count);
	if(gSerialProcCallback(Command_IRWork,stm32f4_TxData,0,stm32f4_TxDataLen,500,3)==false)
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
CIRWorkCommand::CIRWorkCommand(void)
{
	
}


CIRWorkCommand::~CIRWorkCommand(void)
{
}

void CIRWorkCommand::MakePacket()
{
	Packet[0] = count;	
	for (int i = 0; i < count; i++)
	{
		Packet[2+i] = data[i];
	}	
}


