#ifndef   __CHIPPSW_h__
#define   __CHIPPSW_h__
#include "HardwareProfile.h"
#define CHIPPSWAddr 0
extern uint8_t EEPSWOriginal[16];
extern uint8_t EEPSW[16];
extern uint16_t EEPSWAddr;
void GenerateEEPSW();//4ms
void CheckEEPSW();//11.5ms-4
#endif
