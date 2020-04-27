#ifndef EEP_H
#define EEP_H
#include "HardwareProfile.h"

void RomData_ReadBytes(uint16_t Addr,uint8_t* pBuf,  uint8_t bCount);
uint8_t RomData_ReadByte(uint16_t Addr);
void RomData_WriteBytes(uint16_t Addr,uint8_t *pBuf, uint8_t bCount);
void RomData_FillBytes(uint16_t Addr,uint8_t data, uint8_t bCount);
void RomData_WriteByte(uint16_t Addr,uint8_t data);
#endif /* EEP_H */
