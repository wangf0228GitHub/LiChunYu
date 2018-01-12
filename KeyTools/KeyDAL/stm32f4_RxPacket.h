#pragma once
#include "windows.h"

#define stm32f4_RxPacket_pDataIndex 8
#define stm32f4_RxPacket_MaxPacket 1024

class stm32f4_RxPacket
{
public:
	stm32f4_RxPacket(void);
	~stm32f4_RxPacket(void);

public:	
	unsigned short Command;
	unsigned short DataLen;
	byte bError;
	byte ErrCode;
	byte Data[stm32f4_RxPacket_MaxPacket];

	unsigned short NeedCommand;
	int RxCount;

	void Reset();
	bool DataPaketed(byte rx);
};

