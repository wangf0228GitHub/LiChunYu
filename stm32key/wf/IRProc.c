#include "IRProc.h"
#include "Variables.h"
#include "wfDefine.h"
#include "wfEEPROM.h"
#include "wfSys.h"
uint8_t IRTxList[50];
uint32_t IRTxCount;
uint32_t IRTxIndex;

uint32_t IRRxList[50];
uint32_t IRRxCount;

void IRTx_10_33_SSID(void)
{
	IRTxList[0]=0x10;
	IRTxList[1]=0x33;
	RomData_ReadBytes(0x09,&IRTxList[2],4);
	IRTxCount=6;
	IRTxProc();
}

void IRTxProc(void)
{
	uint32_t x;		
	gFlags.bIRTx=1;
	gFlags.bTxFinish=0;
	IRTxCount=IRTxCount<<1;
	IRTxIndex=1;
	x=LOW_BYTE(IRTxList[0]);
	x=x<<6;
	x=x+1019;
	htim2.Instance->ARR=x;
	htim2.Instance->CNT=0;
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&htim2);  
}

void IRRxProc(uint32_t timeOut_ms)
{
	uint32_t rxTick;
	gFlags.bFuncRet=0;
	gFlags.bIRTx=0;
	gFlags.bIRRxFrame=0;
	gFlags.bFirstIC=1;
	IRRxCount=0;
	gFlags.bIRRxErr=0;
	htim2.Instance->ARR=50000;
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
	rxTick=HAL_GetTick();
	while(1)
	{
		if(GetDeltaTick(rxTick)>timeOut_ms)
			return;
		if(gFlags.bIRRxFrame)
		{
			gFlags.bFuncRet=1;
			return;
		}
	}
}
void IRRxDataProc(void)
{
	uint32_t i,x,y;
	gFlags.bIRRxErr=0;
	x=IRRxList[0];
	y=IRRxList[0];
	for(i=1;i<IRRxCount;i++)
	{
		x=IRRxList[i];
		IRRxList[i]=IRRxList[i]-y;
		y=x;
	}
	for(i=0;i<IRRxCount;i++)
	{
		if(IRRxList[i]>992)
		{
			x=992+64;
			for(y=0;y<16;y++)
			{
				if(IRRxList[i]<x)
					break;
				x+=64;
			}
			if(y>=16)
			{
				gFlags.bIRRxErr=1;
				return;
			}
			else
			{
				IRRxList[i]=y;
			}
		}
		else
		{
			gFlags.bIRRxErr=1;
			return;
		}
	}
}
