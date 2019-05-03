#ifndef __AS3933_H__
#define __AS3933_H__

#include "main.h"


#define AS3933_COMM_ClearWake 0xc0
#define AS3933_COMM_PResetDefault 0xc4
void AS3933_Init(void);
uint8_t AS3933_ReadReg(uint8_t addr) /*D°‰AS3933?®π®¢? */;
void AS3933_COMM(uint8_t com) /*D°‰AS3933?®π®¢? */;
#endif
