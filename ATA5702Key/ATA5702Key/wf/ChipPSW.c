#include "ChipPSW.h"
#include "eep.h"
#include "lcyHash.h"
#include "Function.h"
uint8_t EEPSWOriginal[16];
uint8_t EEPSW[16];
uint16_t EEPSWAddr;
void GenerateEEPSW()//4ms
{
	uint8_t i,sum,j;
	uint8_t ee[16];
	for(j=0;j<16;j++)
			EEPSWOriginal[j]=j+5;
	for(i=0;i<16;i++)
	{
		sum=i<<4;
		RomData_ReadBytes(0x0880+sum,ee, 16);
		for(j=0;j<16;j++)
			EEPSWOriginal[j]^=ee[j];
	}
	for(i=0;i<8;i++)
		lcyHashIn[i]=EEPSWOriginal[i];
	lcyHashOnce();//2ms
	for(i=0;i<8;i++)
		EEPSW[i]=lcyHashOut[i];
	for(i=0;i<8;i++)
		lcyHashIn[i]=EEPSWOriginal[8+i];
	lcyHashOnce();
	for(i=0;i<8;i++)
		EEPSW[8+i]=lcyHashOut[i];
	for(i=0;i<16;i++)
		EEPSW[i]=EEPSW[i]^EEPSWOriginal[15-i];
	sum=0;
	for(i=0;i<16;i++)
	{
		sum=(uint8_t)(sum+EEPSWOriginal[i]);
	}
	EEPSWAddr=CHIPPSWAddr+sum;
}
void CheckEEPSW()//11.5ms-4
{	
	uint8_t i,retry;
	uint8_t ataPSW[16];
	retry=16;
	while(retry!=0)
	{
		GenerateEEPSW();
		RomData_ReadBytes(EEPSWAddr,ataPSW,16);
		for(i=0;i<16;i++)
		{
			if(ataPSW[i]!=EEPSW[i])
			{
				retry--;
				break;
			}
		}
		if(i==16)
			return;
	}
	//摧毁整个芯片的程序
	BAT_OFF();
	exitProc();
}