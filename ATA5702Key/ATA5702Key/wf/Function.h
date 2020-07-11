#ifndef   __Function_h__
#define   __Function_h__
//#include "HardwareProfile.h"
extern void GetRSSI(void);
extern void KeyWork_LFRF(void);
void Led_WaitCarPowerOff(void);
//void rfTxShutdown(void);
extern void SystemSleep(void);
//extern void CarIRTxProc(uint8_t t);
void SystemReset(void);
void exitProc(void);
void BootloaderProc(void);
#endif
