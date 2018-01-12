#pragma once
#include "windows.h"

#define SetCANBaudRateCommand_PacketLen 6
class SetCANBaudRateCommand
{
public:
	SetCANBaudRateCommand(void);
	~SetCANBaudRateCommand(void);

public:
	unsigned short Prescaler;
	byte BS1;
	byte BS2;
	byte SJW;
	byte Param;
	byte Packet[SetCANBaudRateCommand_PacketLen];
	void MakePacket();
};

