#ifndef __STDAFX_H__
#define __STDAFX_H__

//#define protues

#include <htc.h>
#define _XTAL_FREQ 32000000
#include <afx.h>
#include "TypeDefine.h"
#include "FlagDefine.h"
#include "Verify.h"
#include "lcyHash.h"
GFlags gFlags;
#define uartRxListMax 11
unsigned char uartRxList[uartRxListMax];
unsigned char uartRxCount;
unsigned char uartTxList[uartRxListMax];
unsigned char keyH[8];
unsigned char keyL[8];


#define T1_100ms 15536 

#define Uart_Uart1
#define Uart_TRMT TRMT
#define Uart_TXREG TXREG
#include "Uart.h"

void InitSystem( void );
//void ProcCommand( void );
void bSameHash(const unsigned char* d1,const unsigned char* d2);
void bSameHash_eep(const unsigned char* d1,unsigned char addr);
void ProcCommand_read_0x76(void);
void ProcCommand_renew_0x52(void);
void ProcCommand_write_0x15(void);
void ProcCommand_write_0x96(void);
void ProcCommand_write_0xa0(void);
void ProcCommand_write_0x9a(void);
void ProcCommand_work_0x3x(void);
void ProcCommand_lock_0x42(void);
//void ProcCommand_0x76(void);
#endif
