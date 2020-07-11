/*
 * ATA5702Key.c
 *
 * Created: 2020/3/22 11:56:02
 * Author : Administrator
 */ 

#include "HardwareProfile.h"
#include "firmware/globals.h"
#include "Variables.h"
#include "firmware/LF_RX.h"
#include "firmware/RF_TX.h"
#include "Function.h"
#include "OnCarProc.h"
#include "EEProc.h"
#include "lib/lcyHash.h"
#include "lib/lcyIRDecode.h"
#include <util/delay.h>
#include "ButtonProc.h"
#include "LFRFProc.h"
#include "avr/boot.h"

//lcyHashOnce();13.2ms
//lcyIRDecode();74.6ms

extern void SystemInit(void);
int main(void)
{
	//uint8_t i=0;
    /* Replace with your application code */
	MCUCR = 0b10000001;//程序从flash区运行，中断向量指向flash区	
 	bLFID0Wake_Reset();
 	bLFRxFinish_Reset();
	bSleep_Reset();
	SystemInit();
	LED_OFF();
// 	IRRxList[0]=boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
// 	IRRxList[1]=boot_lock_fuse_bits_get(GET_LOCK_BITS);
// 	IRRxList[2]=boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
// 	IRRxList[3]=boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	//BootloaderProc();
	//LFQC1=0xf1;
	//LFQC2=0xf1;
	//LFQC3=0xf1;		
// 	while(1)
// 	{
// 		ReadButton();
// 		if(curKeyStateFlags!=NoKey)
// 		{
// 			ButtionProc();
// 			g_sRfTx.bConfig=RfTx_Config_KeyRF;
// 			rfTxShutdown();
// 		}
// 	}
	bIR1033_Set();
	GetKeyState();	
	GetKeyParam();
//	LEDTestMonitor();
	//while(1)
	//{
		if(bLFID0Wake_IsSet())//低频唤醒
		{			
			//PHIMR = 0x00;
			//bLFID0Wake_Reset();
			//bLFRxFinish_Reset();
			if(RomStateFlags.Bits.bRomWrited ==0 ||	RomStateFlags.Bits.bStudy==0)
			{
					//break;
			}
			else
			{
				LFRFProc();
			}
 			//LEDTestMonitor();
 			//break;
			
			//rfTxShutdown();
			//break;
		}
		else
		{
			bSleep_Set();
			if(bOnCarPower())
			{
				BAT_ON();
				// 			GetKeyState();
				// 			GetKeyParam();//获得钥匙当前相关数据
				OnCarProc();
				BAT_OFF();
				//continue;
			}
			else
			{
				ReadButton();
				if(curKeyStateFlags!=NoKey)
				{
					BAT_ON();
					ButtionProc();
					while(1)
					{
						ReadButton();
						if(curKeyStateFlags==NoKey)
						break;
					}
					//g_sRfTx.bConfig=RfTx_Config_KeyRF;
					//rfTxShutdown();
					BAT_OFF();
					//continue;
				}
			}
		}		
// 		if(!(PIND & 0x02))
// 		{
// 			_delay_ms(20);
// 			if(!(PIND & 0x02))
// 			{
// 				ChangeKeyState(ROM_9E);
// 				for(i=0;i<10;i++)
// 				{
// 					_delay_ms(100);
// 					LED_Toggle();
// 				}
// 			}
// 		}
		//LED_OFF();
		//while(1)
		//{
			//if(((PIND&0xfe)==0xfe) && !(PINC&0x04) )
			//{
				//break;
			//}
		//}
	//}	
	//LEDTestMonitor();
	if(!(PINB & _BM(PORTB7)))
		BootloaderProc();
	SystemSleep();
	SystemReset();
	if(g_temp)
	{
		g_temp=0;
	}
// 	if(LF_ID0Wake==0x01)//低频id唤醒
// 	{
// 		KeyWork_LFRF();
// 	}
    while (1) 
    {
		_WDR;
    }
}

