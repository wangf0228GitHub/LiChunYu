#ifndef __CP1616_Client_h__
#define __CP1616_Client_h__

/************************************************************************/
/* 命令定义                                                             */
/************************************************************************/
#define WithPC_Command_Connect 1 //串口与设备建立连接
#define WithPC_Command_ESL 2 //方向锁透传读取数据

#define WithPC_Command_SetUartBaudRate 3 //方向锁透传读取数据


#define WithPC_Command_SetCANBaudRate 100 //CAN波特率设定
#define WithPC_Command_CANRead 101 //CAN按要求读取

#define WithPC_Command_IRWork 200 //红外读取按要求读取
#define WithPC_Command_IREn 201	//红外供电开关



#define UART_WithPC_Instance USART1
#define UART_WithPC_Handle huart1
#define UART_WithPC_BUFMAX 1024
#define UART_WithPC_RxFrameOther 2+2+2+2+1+1

#define UARTCommand_DataOffset 2
typedef union   
{
	struct
	{
		unsigned short FrameHeader;
		unsigned short CommandIndex;
		unsigned short DataLen;		
		unsigned short HeaderBak;
		union
		{
			struct
			{
				unsigned short BaudRate;
			}SetUartBaudRate;
			struct
			{
				unsigned short Retry;
				unsigned short RxTimeOut;
				unsigned char Data[UART_WithPC_BUFMAX-6];
			}UARTCommand;
			struct
			{
				//工作设定相关
				unsigned short Prescaler;
				unsigned char BS1;
				unsigned char BS2;
				unsigned char SJW;
				unsigned char Param;	
				unsigned char RNum;//总线电阻序号
			}SetCANBaudRate;
			struct
			{				
				//发送请求				
				unsigned short TxStdId;//Min_Data = 0 and Max_Data = 0x7FF
				unsigned short TxBak1;//备用
				unsigned int TxExtId;//Min_Data = 0 and Max_Data = 0x1FFFFFFF
				unsigned char TxIDE;//CAN_Identifier_Type
				unsigned char TxRTR;//CAN_remote_transmission_request
				unsigned char TxDLC;//Min_Data = 0 and Max_Data = 8
				unsigned char TxBak2;//备用
				unsigned char TxData[8];

				unsigned short TxRetry;
				unsigned short TxTimeOut;
				//接收需求配置
				unsigned char bTxOnly;
				unsigned char RxIDE;
				unsigned char RxRetry;
				unsigned char NeedRxPocket;
				unsigned short RxTimeOut;
				unsigned short NeedStdId;
				unsigned int NeedExtId;//Min_Data = 0 and Max_Data = 0x1FFFFFFF				
			}CANCommand;
			struct
			{			
				unsigned char count;//发送数据量
				unsigned char Retry;
				unsigned char TxData[100];
			}IRWork;
			struct
			{			
				unsigned char bEn;			
			}IREn;
			unsigned char AllCommandData[UART_WithPC_BUFMAX];
		}CommandData;		
	}RxStruct;
	unsigned char AllRxData[UART_WithPC_BUFMAX+UART_WithPC_RxFrameOther];
} _WithPC_RxBuf;        // general flags
#define UART_WithPC_TxFrameOther 2+2+2+2+1+1
typedef union   
{
	struct
	{
		unsigned short FrameHeader;
		unsigned short CommandIndex;
		unsigned short DataLen;
		unsigned char bError;
		unsigned char ErrCode;
		unsigned char Data[UART_WithPC_BUFMAX];
	}TxStruct;
	unsigned char AllData[UART_WithPC_BUFMAX+UART_WithPC_TxFrameOther];
} _WithPC_TxBuf;        // general flags
// 
// 
// 
// #define CP1616_Client_AddTxHeader()	CP1616_Client_TxByteWithVerify(0x16);	\
// 						CP1616_Client_TxByteWithVerify(0x16);	\
// 						CP1616_Client_TxByteWithVerify(CP1616_Client_Addr.u8H);	\
// 						CP1616_Client_TxByteWithVerify(CP1616_Client_Addr.u8L);
// #define CP1616_Client_AddTxEnder()	CP1616_Client_TxByte(sum);	\
// 						CP1616_Client_TxByte(0x0d);
// 
// #ifndef MAX_CP1616_Client_RX
// #define MAX_CP1616_Client_RX 50
// #endif
// 
// _CP1616_Client_Flags CP1616_Client_Flags;
// unsigned char CP1616_Client_RxList[MAX_CP1616_Client_RX];
// unsigned int CP1616_Client_RxCount;
// unsigned int CP1616_Client_NeedRxCount;
// unsigned int CP1616_Client_NoRxTick;
// uint CP1616_Client_Addr;
// 
extern volatile unsigned int WithPC_NoRxTick;
extern volatile unsigned short WithPC_RxBufIndex;
extern volatile _WithPC_RxBuf WithPC_RxBuf;

void CP1616_Client_Init(void);
void CP1616_Client_EndProcCommand(void);
void CP1616_Client_ProcRx(unsigned char rx);
unsigned char GetVerify_Sum(unsigned char* pBuff,unsigned int Count);
void CP1616_Client_SendData(unsigned short CommandIndex,unsigned char* pBuff,unsigned short Count);
void ProcCommandFromPC( void );
void CP1616_Client_SendOK(unsigned char CommandIndex);
void CP1616_Client_SendError(unsigned char CommandIndex,unsigned char errCode);
#endif


