#ifndef   __Variables_h__
#define   __Variables_h__

#include <stdint.h>
#include "main.h"

typedef union
{	
	struct
	{
		unsigned bIRTx:1;
		unsigned bCarIR:1;
		unsigned bTxFinish:1;
		unsigned bIRRxFrame:1;
		unsigned bFirstIC:1;
		unsigned bIRRxErr:1;

		unsigned bFuncRet:1;
	};
	uint32_t all;
}_gFlags;

typedef union
{	
	struct
	{
		unsigned bRomWrited:1;

		unsigned b55:1;
		unsigned b5f:1;
		unsigned b41:1;
		unsigned b4b:1;
		unsigned b2d:1;
		unsigned b37:1;	
	};
	uint32_t all;
}_RomStateFlags;



extern _gFlags gFlags;
extern _RomStateFlags RomStateFlags;
extern uint8_t RomDatas[16];
extern uint8_t StepTimesAddr[3];//使用次数对应的地址
extern uint8_t CalcTimes;
extern uint8_t CalcTimesAddr;

#endif
