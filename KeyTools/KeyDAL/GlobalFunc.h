#pragma once
#include "windows.h"

byte GetLowByte(unsigned short x);
byte GetHighByte(unsigned short x );

unsigned short GetLowShort(unsigned int x);
unsigned short GetHighShort(unsigned int x );

unsigned short MakeShort(byte h,byte l);


typedef bool (__stdcall *SerialProcCallback)(unsigned short command,byte* pBuf,int offset, int count,int rxTimeout,int retry);


#define SetProgress_Value	0
#define SetProgress_Max		1
#define SetProgress_Min		2
#define SetProgress_Step	3
#define SetProgress_StepIn	4
typedef void (__stdcall *SetProgressCallback)(int mode,int value);

extern SerialProcCallback gSerialProcCallback;
extern SetProgressCallback gSetProgressCallback;
class GlobalFunc
{
public:
	GlobalFunc(void);
	~GlobalFunc(void);
};

