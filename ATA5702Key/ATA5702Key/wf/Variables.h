#ifndef   __Variables_h__
#define   __Variables_h__

#include "HardwareProfile.h"

#define T3_CarIRTx	1
#define T3_CarIRRx	2
#define T3_RFIRTx	3
extern uint8_t CarIRRxTimeOut_N_10ms;
extern uint8_t CarIRRxTimeOut_10ms;
extern uint16_t IRRxByteH;
extern uint16_t IRRxByteL;

extern uint8_t T3WorkType;
extern uint8_t IRRxList[20];
extern uint8_t IRRxCount;
extern uint8_t IRRxNeedCount;
extern uint8_t IRTxCount;
extern uint8_t IRTxList[20];
extern uint8_t IRTxIndex;


extern uint8_t WorkValueDatas[8];
extern uint8_t DoorDatas[8];
extern uint8_t ButtonTimes;
extern uint8_t RomDatas[16];
extern uint8_t LeftTimesAddr[3];
extern uint8_t OtherLeftTimesAddr[3];//未使用的段地址
extern uint8_t LeftTimes[3];//c2,c3,c4
extern uint8_t LeftTimes69Addr;
extern uint8_t LeftTimes69;
extern uint8_t curHash[8];
extern uint8_t EISHash[8];//curHash滚一步
extern uint8_t SSID[4];
extern uint8_t PSW[8];
extern uint8_t EE00;
extern uint8_t EE9e;
extern uint8_t RFKeyValue;

extern uint8_t curKeyStateFlags;
extern uint8_t oldKeyStateFlags;

//extern uint8_t events_reset;
extern volatile uint8_t LF_ID0Wake;
#endif
