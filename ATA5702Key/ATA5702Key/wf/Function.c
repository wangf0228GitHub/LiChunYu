#include "Function.h"
#include "Variables.h"
#include "LF_RX.h"
#include "globals.h"
#include <util\delay.h>
#include "RF_TX.h"

uint8_t LfRxData[10];
uint8_t LfNmbrByts;

void GetLFData(void);
void KeyWork_LFRF(void)
{
	while(1)//等待低频数据接收完成
	{
		if(LF_ID0Wake==0x03)
			break;
	}
	if (LF_ID0Wake == 0x03)
	{
		LF_ID0Wake=0;
		GetLFData();
	}
}
void GetLFData(void)
{
	uint8_t i;
	LDFCKSW |= (1<<LDFSCSW);
	while ((LDFCKSW & (1<<LDFSCKS)) ==0);// wait until clock source is switched
	LfNmbrByts = LDFFL;
	for (i=0; i < LfNmbrByts; i++)
	{
		LfRxData[i] = LDFD;
	}
	LDFCKSW &= ~(1<<LDFSCSW);
}
void Led_WaitCarPowerOff(void)
{
	uint8_t i=0;
	while(1)
	{
		if(!bOnCarPower())
		return;
		i++;
		_delay_ms(1);
		if(i>250)
		{
			i=0;
			LED_Toggle();
		}
	}
}
void rfTxShutdown(void)
{
	g_sRfTxFlowCtrl_bIndex=9;
	do
	{
		ATA_rfTxProcessing_C();		
	}while (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_ACTIVE);
}
void SystemSleep(void)
{
	ATA_lfRxEnableWakeup_flash_C();
// 	PCICR=0x02;//使能 PCINT[15:8]电平变化中断
// 	PCIFR=0x03;//清除中断标志
// 	PCMSK1=0xd8;//4个按键可以唤醒单片机
	PRR0=0xff;
	PRR1=0x5f;	
	PRR2=0xff;
	ATA_globalsClkSwitchMrc_C();	
	ATA_globalsWdtDisable_C();	
	MCUCR &= ~(1<<ENPS);//取消端口设定
	ATA_globalsSleep_C(LF_SLEEP_MODE);
}
void SystemReset(void)
{
	ATA_globalsWdtEnable_C(0x18);
	while(1)
	{
		
	}
}