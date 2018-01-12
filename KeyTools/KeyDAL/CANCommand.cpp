#include "CANCommand.h"
#include "GlobalFunc.h"


CANCommand::CANCommand(void)
{
	TxIDE = CAN_ID_STD;
	TxRTR = CAN_RTR_DATA;
	TxDLC = 8;
	RxIDE = CAN_ID_STD;
	bTxOnly = 0;
	NeedRxPocket = 1;
	RxRetry = 3;
	RxTimeOut = 500;
	TxRetry = 3;
	TxTimeOut = 5;

	SerialReadTimeout=3000;
	SerialRetry=3;

	bNeedReadReg=false;
	//bNeedSaveData=false;
}


CANCommand::~CANCommand(void)
{
}

void CANCommand::MakePacket()
{
	Packet[0] = GetLowByte(TxStdId);
	Packet[1] = GetHighByte(TxStdId);
	Packet[2] = GetLowByte(TxBak1);
	Packet[3] = GetHighByte(TxBak1);
	Packet[4] = GetLowByte(GetLowShort(TxExtId));
	Packet[5] = GetHighByte(GetLowShort(TxExtId));
	Packet[6] = GetLowByte(GetHighShort(TxExtId));
	Packet[7] = GetHighByte(GetHighShort(TxExtId));
	Packet[8] = TxIDE;
	Packet[9] = TxRTR;
	Packet[10] = TxDLC;
	Packet[11] = TxBak2;//
	for (int i = 0; i < 8; i++)
	{
		Packet[12+i] = TxData[i];
	}
	Packet[20] = GetLowByte(TxRetry);
	Packet[21] = GetHighByte(TxRetry);
	Packet[22] = GetLowByte(TxTimeOut);
	Packet[23] = GetHighByte(TxTimeOut);


	Packet[24] = bTxOnly;
	Packet[25] = RxIDE;
	Packet[26] = RxRetry;
	Packet[27] = NeedRxPocket;
	Packet[28] = GetLowByte(RxTimeOut);
	Packet[29] = GetHighByte(RxTimeOut);
	Packet[30] = GetLowByte(NeedStdId);
	Packet[31] = GetHighByte(NeedStdId);
	Packet[32] = GetLowByte(GetLowShort(NeedExtId));
	Packet[33] = GetHighByte(GetLowShort(NeedExtId));
	Packet[34] = GetLowByte(GetHighShort(NeedExtId));
	Packet[35] = GetHighByte(GetHighShort(NeedExtId)); 
}


