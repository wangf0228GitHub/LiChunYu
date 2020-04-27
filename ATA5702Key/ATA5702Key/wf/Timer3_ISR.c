#include "OnCarProc.h"
#include "Variables.h"
#include <avr/pgmspace.h>
#include <util/delay.h>

PROGMEM const uint16_t CarIRTxConstList[16]={5974,6362,6751,7140,7529,7918,8307,8696,9085,9474,9862,10251,11029,11029,11418,11807};
PROGMEM const uint16_t RFIRTxConstList[16]={5415,6970,8526,10082,11637,13193,14748,16304,17859,19415,20970,22526,25637,25637,27192,28748};
uint8_t ThranslateCarIRRx(uint16_t time)
{
	uint8_t ret;
	uint16_t x,i;
	if(time>6027)//992us
	{
		x=6027+389;//992+64,389=64us
		for(i=0;i<16;i++)
		{
			if(time<x)
				break;
			x+=389;
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
	return ret;
}
ISR(T3CAP_vect)
{
	uint8_t h,l;	
	switch(T3WorkType)
	{
	case T3_CarIRTx:
		break;
	case T3_CarIRRx:
		if(IRRxCount==0xff)//第一次捕捉
		{
			IRRxCount=0;
			T3IFR =0x02;
			bIRRxH_Reset();
		}
		else
		{
			if(!bIRRxH_IsSet())
			{
				IRRxByteL=T3ICR;
				bIRRxH_Set();
			}
			else //if(IRRxHL==1)
			{
				IRRxByteH=T3ICR;
				bIRRxH_Reset();
				h=ThranslateCarIRRx(IRRxByteH);
				l=ThranslateCarIRRx(IRRxByteL);
				if(h>16 || l>16)
				{
					IRRxCount=0;
					IRRxNeedCount=0;
				}
				else
				{
					IRRxList[IRRxCount++]=MAKE_BYTE(h,l);
				}
				if(IRRxCount>2)
				{
					if(IRRxCount==IRRxNeedCount)
					{
						T3CR=0x00;
						bIRFinish_Set();
					}
				}
				else if(IRRxCount==2)//用于判断接收数据的长度
				{
					switch(CarIRCommand)
					{
						case 0x08:
						case 0x0f:
						case 0x6a:
						case 0x71:
						case 0x46:
							T3CR=0x00;
							bIRFinish_Set();
							break;
						case 0x38:
							IRRxNeedCount=1+2;
							break;
						case 0x73:
						case 0x79:
							IRRxNeedCount=2+2;
							break;
						case 0x7a:
							IRRxNeedCount=7+2;
							break;
						case 0x26:
						case 0x63:
						case 0x3e:
						case 0x39:
							IRRxNeedCount=8+2;
							break;
						default:
							IRRxNeedCount=9+2;
							break;
					}
				}
				else if(IRRxCount==1)//0x10
				{
					if(IRRxList[0]!=0x10)
						IRRxCount=0;
				}
			}
		}		
		break;
	}
	T3IFR =0x04;
}
ISR(T3COMP_vect)
{
	uint8_t x,i;		
	switch(T3WorkType)
	{
	case T3_CarIRTx:
		T3CR=0x00;
		T3CR |= _BM(T3RES);//复位定时器
		if(IRTxIndex<IRTxCount || IRTxIndex==0xff)
		{
			IRTxIndex++;
			x=IRTxIndex>>1;
			if((IRTxIndex&0x01)==0x00)//双数发低位，单数发高位
			{
				T3COR=pgm_read_word(&CarIRTxConstList[LOW_NIBBLE(IRTxList[x])]);
			}
			else
			{
				T3COR=pgm_read_word(&CarIRTxConstList[HIGH_NIBBLE(IRTxList[x])]);
			}
			CAR_IR_TX_ON();
			_delay_us(28);
			CAR_IR_TX_OFF();
			T3CR=0x84;			
		}
		else
		{
			bIRFinish_Set();
		}		
		break;
	case T3_CarIRRx:
		if(IRRxCount==0xff)//第一次捕捉
		{
			if(CarIRRxTimeOut_N_10ms!=0xff)
			{
				LED_Toggle();
				CarIRRxTimeOut_10ms++;
				if(CarIRRxTimeOut_10ms>=CarIRRxTimeOut_N_10ms)
				{
					T3CR=0x00;
					IRRxCount=0;
					bIRFinish_Set();
				}
			}
		}
		else
		{
			T3CR=0x00;
			IRRxCount=0;
			bIRFinish_Set();
		}		
		break;
	case T3_RFIRTx:
		T3CR=0x00;
		T3CR |= _BM(T3RES);//复位定时器
		if(IRTxIndex<IRTxCount || IRTxIndex==0xff)
		{
			IRTxIndex++;
			x=IRTxIndex>>1;
			if((IRTxIndex&0x01)==0x00)//双数发低位，单数发高位
			{
				T3COR=pgm_read_word(&RFIRTxConstList[LOW_NIBBLE(IRTxList[x])]);
			}
			else
			{
				T3COR=pgm_read_word(&RFIRTxConstList[HIGH_NIBBLE(IRTxList[x])]);
			}
			for(i=0;i<8;i++)
			{
				CAR_IR_TX_ON();
				_delay_us(16);
				CAR_IR_TX_OFF();
				_delay_us(16);
			}			
			T3CR=0x84;
		}
		else
		{
			bIRFinish_Set();
		}
		break;
	}	
	T3IFR =0x02;
}
ISR(T3OVF_vect)
{	
	switch(T3WorkType)
	{
	case T3_CarIRTx:		
		break;
	case T3_CarIRRx:
		break;
	}
	T3IFR =0x01;
}
