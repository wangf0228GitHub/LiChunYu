#include "IRProc.h"
#include "Variables.h"
#include "wfDefine.h"
#include "wfEEPROM.h"
#include "wfSys.h"
uint8_t IRTxList[50];
uint32_t IRTxCount;
uint32_t IRTxIndex;


uint32_t IRRxByteL;
uint32_t IRRxByteH;
uint8_t IRRxList[50];
uint32_t IRRxCount;
uint32_t IRRxNeedCount;

uint32_t tIRRxTime[50];
uint32_t tIRRxData[50];
int tIRRxDelta[50];
uint32_t tIRRxIndex;
const uint32_t tIRRxHope[16]={1024,1088,1152,1216,1280,1344,1408,1472,1536,1600,1664,1728,1792,1856,1920,1984};
void IRTx_10_33_SSID(void)
{
	uint8_t i;
	IRTxList[0]=0x10;
	IRTxList[1]=0x33;
	for(i=0;i<4;i++)
		IRTxList[2+i]=SSID[i];
	IRTxCount=6;
	IRTxProc();
}
void IRTx_10_28(void)
{
	IRTxList[0]=0x10;
	IRTxList[1]=0x28;
	IRTxCount=2;
	IRTxProc();
}
void IRTxProc(void)
{
	uint32_t x;		
	gFlags.bIRTx=1;
	//while(gFlags.bTxFinish==0);
	gFlags.bTxFinish=0;
	while(1)
	{
		if(GetDeltaTick(IRRxTick)>20)
			break;
	}
	
	IRTxCount=IRTxCount<<1;
	IRTxIndex=1;
	x=LOW_NIBBLE(IRTxList[0]);
	x=x<<6;
	x=x+1019;
	htim2.Instance->ARR=x;
	htim2.Instance->CNT=0;
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&htim2);  
	while(gFlags.bTxFinish==0);
}
uint8_t ThranslateIRRx(uint32_t time)
{
	uint8_t ret;
	uint32_t x,i;
	if(time>992)
	{
		x=992+64;
		for(i=0;i<16;i++)
		{
			if(time<x)
				break;
			x+=64;
		}
		if(i>=16)
		{
			ret=0xff;
		}
		else
		{
			ret=i;
		}
	}
	else
	{
		ret=0xfe;
	}
	if(ret<0x10)
	{
		tIRRxTime[tIRRxIndex]=time;
		tIRRxData[tIRRxIndex]=ret;
		tIRRxDelta[tIRRxIndex]=tIRRxHope[ret]-time;
		tIRRxIndex++;
	}
	else
	{
		tIRRxTime[tIRRxIndex]=time;
		tIRRxData[tIRRxIndex]=ret;
		tIRRxDelta[tIRRxIndex]=0xffffff;
		tIRRxIndex++;
	}
	return ret;
}
void IRRxProc(uint32_t timeOut_ms)
{
	uint32_t rxTick;

	tIRRxIndex=0;
	gFlags.bFuncRet=0;
	//while(gFlags.bTxFinish==0);
	gFlags.bIRTx=0;
	gFlags.bIRRxFrame=0;
	gFlags.bFirstIC=1;
	IRRxCount=0;
	IRRxNeedCount=0xffff;
	gFlags.bIRRxErr=0;
	htim2.Instance->ARR=3000;
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
	rxTick=HAL_GetTick();
	while(1)
	{
		if((timeOut_ms!=0) && (GetDeltaTick(rxTick)>timeOut_ms))
		{
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_4);		
			HAL_TIM_Base_Stop_IT(&htim2);	
			return;
		}
		if(gFlags.bIRRxFrame)
		{
			gFlags.bFuncRet=1;
			return;
		}
#ifdef KeepPower
 		if(bOnCarPower()==OnCarPowerState_OFF)
 			NVIC_SystemReset();
#endif
	}
}

