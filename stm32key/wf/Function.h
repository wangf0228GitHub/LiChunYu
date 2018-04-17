#ifndef   __Function_h__
#define   __Function_h__

#include <stdint.h>
#include "main.h"

void GetROMState(void);
void Check55and5fdata(void);
void Check41and4bdata(void);
void Check2Dand37data(void);
void CheckDataBlockVerify(uint8_t Addr);
#endif
