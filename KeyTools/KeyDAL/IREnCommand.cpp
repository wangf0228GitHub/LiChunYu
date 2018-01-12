#include "IREnCommand.h"
#include "GlobalFunc.h"
#include "Serial.h"
#include "ConstDefine.h"
#include "VMProtectSDK.h"

bool __stdcall IREn( int bOpen )
{
	VMProtectBegin("IREn");
	byte bOPEN;
	if(bOpen==0)
		bOPEN=false;
	else
		bOPEN=true;
	nErrorCode=ErrorCode_NoErr;	
	if(gSerialProcCallback==NULL)
	{
		nErrorCode=ErrorCode_NoSerialProcCallback;
		return false;
	}
	MakeTxPacket(Command_IREn,&bOPEN,1);
	if(gSerialProcCallback(Command_IREn,stm32f4_TxData,0,stm32f4_TxDataLen,500,3)==false)
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


