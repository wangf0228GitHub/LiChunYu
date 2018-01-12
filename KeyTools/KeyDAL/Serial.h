#pragma once
#include <windows.h>
#include "GlobalFunc.h"
#include "stm32f4_RxPacket.h"

extern int nErrorCode;

extern byte stm32f4_TxData[1024];
extern int stm32f4_TxDataLen;
extern stm32f4_RxPacket RxPacket;

void MakeTxPacket(unsigned short com,byte* data,unsigned short len);


//extern "C" __declspec(dllexport) int __stdcall add(int x,int y);
extern "C" __declspec(dllexport) void __stdcall InitDllWork(SerialProcCallback _SerialProcCallback,SetProgressCallback _SetProgressCallback);

extern "C" __declspec(dllexport) bool __stdcall ProcRxByte(byte rx);

extern "C" __declspec(dllexport) void __stdcall GetRxData(byte* pBuf,int& len);

extern "C" __declspec(dllexport) int __stdcall GetErrorCode();

extern "C" __declspec(dllexport) bool __stdcall CheckDevice();