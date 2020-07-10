#ifndef RF_TX_H
#define RF_TX_H
#include "HardwareProfile.h"
#include "globals.h"
#include "RF_TX_Defs.h"

extern sRfTxCurrentServiceChannelConfiguration g_sRfTxCurrentService;
extern sRfTxConfig g_sRfTx;
extern uint8_t g_bVcoTuningResult;
extern uint8_t g_bAntennaTuningResult;
extern volatile uint8_t g_sRfTxFlowCtrl_bIndex;

extern void ATA_rfTxInit_C(void);
extern void ATA_rfTxFillSFifo_C(uint8_t bLen, uint8_t *pData);
extern void ATA_rfTxFillDFifo_C(uint8_t bLen, uint8_t *pData);
extern void ATA_rfTxStartTx_C(uint8_t config);
extern void ATA_rfTxProcessing_C(void);
extern void ATA_rfTxShutdown_C(void);
#endif /* LFRX_H */
