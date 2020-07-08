#ifndef __VERIFY_H__
#define __VERIFY_H__

#include "HardwareProfile.h"


#define VERIFYSUM(x,y) ((uint8_t)(x+y))
#define VERIFYXOR(x,y) ((uint8_t)(x^y))

uint8_t GetVerify_Sum(uint8_t* pBuff,uint8_t Count);


uint8_t GetVerify_byteXOR(uint8_t* pBuff, uint8_t Count);
uint8_t GetVerify_byteXOR_WithOrigV(uint8_t origV,uint8_t* pBuff, uint8_t Count);

#endif
