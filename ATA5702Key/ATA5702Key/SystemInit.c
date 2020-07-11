#include "HardwareProfile.h"
#include "firmware/globals.h"
#include "firmware/LF_RX.h"
#include "RF_TX.h"
void SystemInit(void)
{
	/************************************************************************/
	/* PORTD
	0:power_hold出	1
	1:IR RX1	入	上拉？	INT0
	2:LED_Red	出	1	led灭
	3:S1_LOCK	入	上拉
	4:S2_UNLOCK	入	上拉
	5:IR_RX2	入	上拉？	TICP
	6:S3_Tail	入	上拉
	7:S4_Find	入	上拉                                                    */
	/************************************************************************/
	PORTD=0xff;
	DDRD =0x05;
	/************************************************************************/
	/* PORTC
	0:CAR_IR_TX	出	1
	1:RF_IR_TX	出	1
	2:OnCar_INT	入	不上拉			                                         */
	/************************************************************************/
	PORTC=0x03;
	DDRC =0x03;
	/************************************************************************/
	/* PORTB
	7:bootloader	入	上拉                                                */
	/************************************************************************/
	PORTB=0xFF;
	DDRB =0x00;
	MCUCR |= _BM(ENPS);//使能端口设定
	MCUCR &= ~_BM(PUD);	
	ATA_globalsClkStartXTO_C();//使用frc作为系统频率,6.07625Mhz
	
	//events_reset=MCUSR;//记录复位原因
	MCUSR=0x00;//清除复位源标志
	ATA_globalsWdtDisable_C();//初始化阶段禁用看门狗
	//如果低频关闭则初始化低频模块
 	if ( (LFCPR & _BM(LFCALRY)) == 0x00U ) //(1)
     {
         ATA_lfRxInit_C();
         LFCPR = _BM(LFCPCE);
         LFCPR = _BM(LFCALRY);    
     }
 	else
 	{
 		ATA_POWERON_C(PRR1, PRLFR);//初始化
 		ATA_POWERON_C(PRR1, PRLFPH);//低频协议处理模块供电
 	}	
	ATA_calibInit_C();	
	//ATA_globalsWdtEnable_C(0x1B);//32K cycles 8*32K = 256K cycles 2.1s 1.75s
	/* Enable global interrupts */
	_SEI;	

	g_sRfTx.bFlags      = 0x00U;
	g_sRfTx.bTuneFlags  = 0x00U;
	g_sRfTx.bStatus     = 0x00U;
	g_sRfTx.bConfig     = 0x00U;
	g_sRfTx.pAddress   = 0x0000U;
}


