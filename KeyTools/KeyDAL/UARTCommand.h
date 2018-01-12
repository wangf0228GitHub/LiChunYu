#pragma once
#include "windows.h"
class UARTCommand
{
public:
	UARTCommand(void);
	~UARTCommand(void);
public:
	int ParamLen;
	unsigned short BaudRate;
	unsigned short Retry;
	unsigned short RxTimeOut;
	byte Data[1024];//[UART_WithPC_BUFMAX-6];
	byte Packet[1024];
};

