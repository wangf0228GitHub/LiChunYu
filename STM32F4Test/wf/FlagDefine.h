#ifndef   __FlagDefine_h__
#define   __FlagDefine_h__
/************************************************************************/
/* 第一个为最低位                                                       */
/************************************************************************/
typedef union   
{
	struct
	{
		unsigned bWithPC_Rx:1;//是否串口收到数据帧
		unsigned bError:1;//当前是否有错误发生
		/************************************************************************/
		/* 红外用                                                               */
		/************************************************************************/
		unsigned bIRTxPulse:1;//IR当前是否为发送脉冲
		unsigned bIRTxFinish:1;
		unsigned bFirstIC:1;//是否为第一次捕捉
		unsigned bIRRxFinish:1;
	}Bits;
	unsigned int AllFlag;
} _GFlags;        // general flags


#endif
