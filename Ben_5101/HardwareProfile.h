#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <htc.h>
#define _XTAL_FREQ 8000000
#include "FlagDefine.h"
#include "TypeDefine.h"
#include "afx.h"


#define		TST_RA6		LATA6

#define		SW_4		PORTCbits.RB7		//;S4-寻车
#define		SW_3		PORTCbits.RB6		//;S3-尾箱
#define		SW_2		PORTCbits.RB5		//;S2-开锁
#define		SW_1		PORTCbits.RB4		//;S1-锁车

#define		LED			LATB2	
// #define		IIC_SDA		PORTB,1		;SDA
// #define		IIC_SCL		PORTB,0		;SCL

#define		IR_A		LATC0
#define		IR_B		LATC1
#define		IR_IN		PORTCbits.RC2
#define		RF_EN		LATC3		//RF_EN
#define		RFOUT		LATC4		//RF数据发射
#define		RFOUT_OP	LATC5		
#define		BAT_CL		LATC6	
#define		PW_IN		PORTCbits.RC7		

#define SimI2C
#define SimI2C_CLK_IO TRISB0
#define SimI2C_DATA_IO TRISB1
#define SimI2C_CLK_R PORTBbits.RB0
#define SimI2C_DATA_R PORTBbits.RB1
#define SimI2C_CLK_W LATB0
#define SimI2C_DATA_W LATB1
#define SimI2C_Delay() __delay_us(10);
#include "SimI2C.h"
#include "M24XX.h"

_RAM RAM @ 0x022;
_f20 f20;
_f21 f21;

// #defin// AM_81 CCPRx[0]
// #defin// AM_82 CCPRx[1]
unsigned char CCPRx[16];

unsigned char wfTempList[16];

#define KEY_VALU RAM.RAM_22
#define CNT0 RAM.RAM_23
#define CNT1 RAM.RAM_24
#define CNT2 RAM.RAM_25


#define BUF_A RAM.RAM_26
#define BUF_B RAM.RAM_27
#define BUF_0 RAM.RAM_28
#define BUF_1 RAM.RAM_29
#define FLAG0 RAM.RAM_2A
#define YK_STA RAM.RAM_2B
#define ADDRESS_H RAM.RAM_30
#define ADDRESS_L RAM.RAM_31
#define ADDRESS_WR_H RAM.RAM_32
#define ADDRESS_RD_H RAM.RAM_33
#define ADDRESS RAM.RAM_34
#define TEMP_EE RAM.RAM_35
#define TEMP_tx RAM.RAM_36
#define CNT_e0 RAM.RAM_37
#define TEMP_TT RAM.RAM_39
#define TEMP_bb RAM.RAM_3B
#define flag00 RAM.RAM_3C

#define f_cmd35 RAM.RAM_3C.b0
#define f_zero RAM.RAM_3C.b1 //'0' is not ZERO >0,'1' is equal to ZERO.


#define tmp_PT RAM.RAM_50
#define tmp_cL RAM.RAM_51

#define A_P RAM.RAM_52
#define X_P RAM.RAM_53

#define TEMP_0 RAM.RAM_56
#define TEMP_1 RAM.RAM_57
#define TEMP_2 RAM.RAM_58
#define TEMP_3 RAM.RAM_59
#define TEMP_4 RAM.RAM_5A
#define TEMP_5 RAM.RAM_5B


#define TEMP0 RAM.RAM_40
#define TEMP1 RAM.RAM_41
#define TEMP2 RAM.RAM_42

unsigned char* pFSR0;
unsigned char* pFSR1;
unsigned char* pFSR2;


uint wfTemp16;

void POW_CAR(void);
void POW_BAT(void);
#endif
