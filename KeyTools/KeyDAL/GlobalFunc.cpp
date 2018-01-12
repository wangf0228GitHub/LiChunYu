#include "GlobalFunc.h"


GlobalFunc::GlobalFunc(void)
{
}


GlobalFunc::~GlobalFunc(void)
{
}

byte GetLowByte(unsigned short x )
{
	return (byte)(x&0x00ff);
}
byte GetHighByte(unsigned short x )
{
	return (byte)((x>>8)&0x00ff);
}

unsigned short GetLowShort( unsigned int x )
{
	return (unsigned short)(x&0xffff);
}

unsigned short GetHighShort( unsigned int x )
{
	return (unsigned short)((x>>16)&0xffff);
}

unsigned short MakeShort( byte h,byte l )
{
	return (unsigned short)((((unsigned short)h)<<8)+l);
}

SerialProcCallback gSerialProcCallback = NULL;
SetProgressCallback gSetProgressCallback=NULL;
