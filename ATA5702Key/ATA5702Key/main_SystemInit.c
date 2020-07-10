/*
 * ATA5702KeySystemInit.c
 *
 * Created: 2020/5/25 21:54:46
 * Author : Administrator
 */ 
#include "HardwareProfile.h"
#include "ChipPSW.h"
#include <stdlib.h>
#include "eep.h"
#include <util\delay.h>
#include "EEProc.h"
extern void SystemInit(void);
void ATA5702_InitEEPROM(void);
int main(void)
{
	MCUCR = 0b10000001;//程序从flash区运行，中断向量指向flash区
	bLFID0Wake_Reset();
	bLFRxFinish_Reset();
	SystemInit();
    /* Replace with your application code */
	GenerateEEPSW();
	ATA5702_InitEEPROM();
	ChangeKeyState(ROM_9E);
	ChangeRF433315State(ROM_9E);
    while (1) 
    {
		LED_Toggle();
		_delay_ms(500);
    }
}
void ATA5702_InitEEPROM(void)
{
	uint8_t i;
	uint16_t addr,seed;
	uint8_t dataIn[16],dataOut[16];
	seed=0x10;
	for(i=0;i<16;i++)
		seed+=EEPSWOriginal[i];
	srand(seed);
	for(addr=0;addr<0x0800;addr+=16)
	{
		for(i=0;i<16;i++)
		{
			dataIn[i]=rand();
			dataOut[i]=0;
		}
		RomData_WriteBytes(addr,dataIn,16);
		RomData_ReadBytes(addr,dataOut,16);
		for(i=0;i<16;i++)
		{
			if(dataIn[i]!=dataOut[i])
			{
				while(1)
				{
					LED_Toggle();
					_delay_ms(200);
				}
			}
		}
	}
	RomData_WriteBytes(EEPSWAddr,EEPSW,16);
	RomData_ReadBytes(EEPSWAddr,dataOut,16);
	for(i=0;i<16;i++)
	{
		if(EEPSW[i]!=dataOut[i])
		{
			while(1)
			{
				LED_Toggle();
				_delay_ms(200);
			}
		}
	}
}

