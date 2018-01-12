#pragma once
#include "windows.h"


extern "C" __declspec(dllexport) bool __stdcall IRWork(int bOpen);
class CIRWorkCommand
{
public:
	CIRWorkCommand(void);
	~CIRWorkCommand(void);

public:	
	byte count;	
	byte data[100];
	byte Packet[102];
	void MakePacket();
};

