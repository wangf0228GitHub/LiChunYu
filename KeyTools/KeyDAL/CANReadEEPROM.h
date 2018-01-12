#pragma once
#include "CANCommand.h"
extern "C" __declspec(dllexport) bool __stdcall CANReadEEPROMProc(int mode);

extern "C" __declspec(dllexport) void __stdcall GetEEPROMData(byte* pBuf,int& len);
 class CANReadEEPROM
 {
 public:
 	CANReadEEPROM(void);
 	~CANReadEEPROM(void);
 public:
 	CANCommand WakeUp;
 	CANCommand GetDataSeq[10];
 	int GetDataSeqLen;//共使用的帧数
 
	byte ReadReg[100];//需要读取的地址单元
	int ReadRegLen;//需要读取的地址单元总量

 	int PacketCount;
 };
void initCANReadEEPROM();

