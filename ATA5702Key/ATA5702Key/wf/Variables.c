#include "Variables.h"

//uint8_t events_reset;
uint8_t T3WorkType;
uint16_t IRRxByteH;
uint16_t IRRxByteL;
uint8_t IRRxHL;
uint8_t IRRxList[20];
uint8_t IRRxCount;
uint8_t IRRxNeedCount;
uint8_t IRTxCount;
uint8_t IRTxList[20];
uint8_t IRTxIndex;

uint8_t CarIRRxTimeOut_N_10ms;
uint8_t CarIRRxTimeOut_10ms;
uint8_t WorkValueDatas[8];
uint8_t DoorDatas[8];
uint8_t ButtonTimes;
uint8_t RomDatas[16];
uint8_t LeftTimesAddr[3];
uint8_t OtherLeftTimesAddr[3];//未使用的段地址
uint8_t LeftTimes[3];
uint8_t LeftTimes69Addr;
uint8_t LeftTimes69;
uint8_t curHash[8];
uint8_t EISHash[8];//curHash滚一步
uint8_t SSID[4];
uint8_t PSW[8];
uint8_t EE00;
uint8_t EE9e;
uint8_t RFKeyValue;
volatile uint8_t LF_ID0Wake=0;

uint8_t curKeyStateFlags;
uint8_t oldKeyStateFlags;