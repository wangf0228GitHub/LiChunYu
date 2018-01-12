#pragma once
#include "windows.h"

#define CAN_RTR_REMOTE 0x02;

#define CAN_RTR_DATA 0x00;

#define CAN_ID_EXT 0x04;

#define CAN_ID_STD 0x00;


#define CANCommand_PacketLen 24+12
class CANCommand
{
public:
	CANCommand(void);
	~CANCommand(void);

public:
	//taken from c struct in platform sdk 
	//         //工作设定相关  2+1+1+1+1=6
	// 		public ushort Prescaler;
	// 		public byte BS1;
	// 		public byte BS2;
	// 		public byte SJW;
	// 		public byte Param;

	//发送请求	2+2+4+4+8+2+2=22			
	unsigned short TxStdId;//Min_Data = 0 and Max_Data = 0x7FF
	unsigned short TxBak1;
	unsigned int TxExtId;//Min_Data = 0 and Max_Data = 0x1FFFFFFF
	byte TxIDE;//CAN_Identifier_Type
	byte TxRTR;//CAN_remote_transmission_request
	byte TxDLC;//Min_Data = 0 and Max_Data = 8
	byte TxBak2;//备用
	byte TxData[8];

	unsigned short TxRetry;
	unsigned short TxTimeOut;
	//接收需求配置  2+2+2+2+4=12
	byte bTxOnly;
	byte RxIDE;
	byte RxRetry;
	byte NeedRxPocket;
	unsigned short RxTimeOut;
	unsigned short NeedStdId;
	unsigned int NeedExtId;//Min_Data = 0 and Max_Data = 0x1FFFFFFF

	byte Packet[CANCommand_PacketLen];

	/************************************************************************/
	/* dll处理数据用                                                        */
	/************************************************************************/
	int SerialReadTimeout;//此次指令超时时长，默认3s
	int SerialRetry;//此次指令在pc端重试次数

	//bool bNeedSaveData;//此次指令返回数据是否需要存储
	
	int ReadRegAddr;//动态字节地址
	bool bNeedReadReg;//是否有动态字节需要改变	

	void MakePacket();
};

