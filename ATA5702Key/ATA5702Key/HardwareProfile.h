#ifndef __HardwareProfile_H__
#define __HardwareProfile_H__

#define F_CPU 6076250
#include <avr/io.h>
#include <avr/interrupt.h>
#include "globals_defs.h"

#define ROMVer 0x76
//21DF  写过是05FB 学习过 15EB
//02FE  写过是04FC 学习过 14EC
#define ROM_9E 0x21

#define NoKey 0xd8
#define FindCarKey 0x58
#define LockKey 0xd0
#define UnLockKey 0xc8
#define TailGateKey 0x98
#define Lock_UnLock_Key 0xc0
#define UnLock_TailGate_Key 0x88
#define Lock_TailGate_Key 0x90
#define ReadButton_PD() (PIND & 0xd8)

#define LED_OFF()		PORTD |= _BM(2)
#define LED_ON()		PORTD &= ~_BM(2)
#define LED_Toggle()	PORTD ^= _BM(2)
#define LEDTestMonitor() LED_Toggle();LED_Toggle();LED_Toggle();LED_Toggle()
#define PowerLed() LED_ON();_delay_ms(35);LED_OFF()

#define BAT_OFF()		PORTD |= _BM(0)
#define BAT_ON()		PORTD &= ~_BM(0)

#define CAR_IR_TX_OFF()		PORTC |= _BM(0)
#define CAR_IR_TX_ON()		PORTC &= ~_BM(0)

#define RFIR_TX_OFF()		PORTC |= _BM(1)
#define RFIR_TX_ON()		PORTC &= ~_BM(1)

#define bOnCarPower() (PINC & 0x04)

#define RfTx_Config_KeyRF 0xd2 //都调谐，fifo，xto，完成后留在发送状态，1k
#define RfTx_Config_LFRF 0xd0 //都调谐，fifo，xto，完成后留在发送状态，1k
/************************************************************************/
/* GPIOR0位操作                                                          */
/************************************************************************/
#define bFuncRet_Set() GPIOR0 |= BITMASK(BIT_0)
#define bFuncRet_Reset() GPIOR0 &= ~BITMASK(BIT_0)
#define bFuncRet_IsSet() (GPIOR0& BITMASK(BIT_0))

#define bIRRxH_Set() GPIOR0 |= BITMASK(BIT_1)
#define bIRRxH_Reset() GPIOR0 &= ~BITMASK(BIT_1)
#define bIRRxH_IsSet() (GPIOR0& BITMASK(BIT_1))

#define bIRFinish_Set() GPIOR0 |= BITMASK(BIT_2)
#define bIRFinish_Reset() GPIOR0 &= ~BITMASK(BIT_2)
#define bIRFinish_IsSet() (GPIOR0& BITMASK(BIT_2))


#define bLFID0Wake_Set() GPIOR0 |= BITMASK(BIT_3)
#define bLFID0Wake_Reset() GPIOR0 &= ~BITMASK(BIT_3)
#define bLFID0Wake_IsSet() (GPIOR0& BITMASK(BIT_3))

#define bLFRxFinish_Set() GPIOR0 |= BITMASK(BIT_4)
#define bLFRxFinish_Reset() GPIOR0 &= ~BITMASK(BIT_4)
#define bLFRxFinish_IsSet() (GPIOR0& BITMASK(BIT_4))

#define bIR1033_Set() GPIOR0 |= BITMASK(BIT_5)
#define bIR1033_Reset() GPIOR0 &= ~BITMASK(BIT_5)
#define bIR1033_IsSet() (GPIOR0& BITMASK(BIT_5))

#define bSleep_Set() GPIOR0 |= BITMASK(BIT_6)
#define bSleep_Reset() GPIOR0 &= ~BITMASK(BIT_6)
#define bSleep_IsSet() (GPIOR0& BITMASK(BIT_6))
#endif
