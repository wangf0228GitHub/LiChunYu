#ifndef   __ATA5824_h__
#define   __ATA5824_h__

#include "main.h"
extern uint32_t ATA5824_bFirstIC;
typedef struct
{
	uint16_t time;
	uint8_t pinState;
}_ATA5824_ICDataStruct;
#define ATA5824_ICData_BufLen 1000
extern _ATA5824_ICDataStruct ATA5824_ICData[ATA5824_ICData_BufLen];
extern uint32_t ATA5824_ICDataIndex;
void ATA5824_RxStart(void);
void ATA5824_Init(void);
void ATA5824_ICProc(void);
void ATA5824_IDLE(void);
#endif
