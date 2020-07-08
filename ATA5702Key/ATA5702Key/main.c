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
//lcyHashOnce();13.2ms
//lcyIRDecode();74.6ms
uint8_t PAYLOADDATA  [ 32 ];
extern void SystemInit(void);
int main(void)
{
	uint8_t i=0;
    /* Replace with your application code */
	MCUCR = 0b10000001;//程序从flash区运行，中断向量指向flash区	
	SystemInit();
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
	while(1)
	{
		if(bOnCarPower())
		{
			GetKeyState();
			GetKeyParam();//获得钥匙当前相关数据
			OnCarProc();
			BAT_OFF();
			continue;
		}
		else
		{
			ReadButton();
			if(curKeyStateFlags!=NoKey)
			{
				ButtionProc();
				g_sRfTx.bConfig=RfTx_Config_KeyRF;
				rfTxShutdown();
				continue;
			}
		}
		break;
	}
	if(!(PIND & 0x02))
	{
		_delay_ms(20);
		if(!(PIND & 0x02))
		{
			ChangeKeyState(ROM_9E);
			for(i=0;i<10;i++)
			{
				_delay_ms(100);
				LED_Toggle();
			}
			LED_OFF();
		}
	}
	SystemSleep();
	while(1)
	{
		
	}
	if(LF_ID0Wake==0x01)//低频id唤醒
	{
		KeyWork_LFRF();
	}
    while (1) 
    {
		_WDR;
    }
}

