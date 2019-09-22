#ifndef   __ATA5824_h__
#define   __ATA5824_h__

#include "main.h"

extern uint32_t ATA583X_bFirstIC;
typedef struct
{
	uint16_t time;
	uint8_t pinState;
}_ATA583X_ICDataStruct;
#define ATA583X_ICData_BufLen 1000
extern _ATA583X_ICDataStruct ATA5824_ICData[ATA583X_ICData_BufLen];
extern uint32_t ATA583X_ICDataIndex;
extern uint32_t ATA583X_RxTick;

extern uint8_t ATA583X_SPITxList[20];
extern uint8_t ATA583X_SPIRxList[20];

extern uint8_t ATA583X_TxList[20];
extern uint32_t ATA583X_TxCount;
extern uint32_t ATA583X_RxTick;
extern uint8_t ATA583X_RxList[20];
extern uint32_t ATA583X_RxCount;

void ATA583X_Init(void);
void CheckEEPSW(void);
uint8_t ATA583X_ReadEEPROM(uint32_t addr);
void ATA583X_WirteEEPROM(uint32_t addr,uint8_t data);
void ATA583X_IDLEMode(void);
void ATA583X_RxMode(void);
void ATA583X_TxMode(void);
void ATA583X_Check(void);
void ATA583X_TxFrameProc(void);
void ATA583X_ReadSRAM(uint32_t addr,uint8_t len,uint8_t* pdata);
void ATA583X_WriteSRAM(uint32_t addr,uint8_t len,uint8_t* pdata);
void ATA583X_ICProc(void);
void ATA583X_FrameProc(void);
void ATA583X_WaitRx(uint32_t timeOut);
void ATA583X_RFTxFrameProc(void);
void ATA583X_Reset(void);
#endif
