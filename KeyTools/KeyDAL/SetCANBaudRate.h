#pragma once
#include <windows.h>

extern "C" __declspec(dllexport) bool __stdcall SetCANBaudRate(int brType);

#define SetCANBaudRateCommand_PacketLen 7
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
	byte RNum;
	byte Packet[SetCANBaudRateCommand_PacketLen];
	void MakePacket();
};
