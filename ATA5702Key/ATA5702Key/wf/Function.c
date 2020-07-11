#include "Function.h"
#include "Variables.h"
#include "LF_RX.h"
#include "globals.h"
#include <util\delay.h>
#include "RF_TX.h"
#include "EEProc.h"
#include <avr/wdt.h>
#include <avr/boot.h>
#include "avr\pgmspace.h"
#include "lfrssi.h"

#define RWWSRE 4
void Led_WaitCarPowerOff(void)
{
	uint8_t i=0;
	while(1)
	{
		if(!bOnCarPower())
			return;
		i++;
		_delay_ms(1);
		if(i>250)
		{
			i=0;
			LED_Toggle();
		}
	}
}
void GetRSSI(void)
{
	uint8_t srcVal =0;	
	ATA_lfRssiSetEepromConfig_C( srcVal );
	ATA_lfRssiOpen_C();
	LFCR1 |= _BM(LFRE);
	//g_sLfRssiFlowCtrl_flash.bIndex++;
	g_sLfRssiRegConfig_flash.bRscr = 0x09;  // enable sign detection
	g_sLfRssiRegConfig_flash.bRsdlyr = 24; //select the RSSI Tracking time
	g_sLfRssiRegConfig_flash.bRsms1r = 0x07; // select all channels
	g_sLfRssiRegConfig_flash.bRsms2r = 0x70; // select number of samples for averaging 128 samples
	g_sLfRssiRegConfig_flash.bRssrcr = 0x05; // SRC cal on LF
	do
	{
		//__no_operation();
	} while ((RSSR & 0x01) ==0);
	TPCR2 |= _BM(TPD);
	ATA_lfRssiMeasStart_C( &g_sLfRssiRegConfig_flash, 0U, 1U );  // external RSSI and sign detection
	_delay_us(100);
	do
	{
		
	}
	while ((g_sLfRssi.bStatus & LFRSSI_STATUS_BM_MEAS_DATA_AVAILABLE_FLAG)==0);
	// read RSSI values from first HFM
	RSMS1R &= ~(1<<RSSSV);        // for output the average value
	gRssiFlag = RSFR;
	if((gRssiFlag & 0x20)==0)
	{
		gExtRssiAnt[0] = RSRES1L;
		gExtRssiAnt[0]|= (RSRES1H<<8);
	}
	else gExtRssiAnt[0]=0;
	if((gRssiFlag & 0x40)==0)
	{
		gExtRssiAnt[1] = RSRES2L;
		gExtRssiAnt[1]|= (RSRES2H<<8);
	}
	else gExtRssiAnt[1]=0;
	if((gRssiFlag & 0x80)==0)
	{
		gExtRssiAnt[2] = RSRES3L;
		gExtRssiAnt[2]|= (RSRES3H<<8);
	}
	else gExtRssiAnt[2]=0;
	/* LLR-Ref: 050 */
	RSMS1R &= ~_BM(RSSCAL);
	RSCR   &= ~(_BM(RSMODE0) | _BM(RSOFM) | _BM(RSEOR));
	RSCR   |= _BM(RSMODE1);
	 /* LLR-Ref: 060 */
	RSFR = _BM(RSOFF);
	 _CLI;
	 g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG;

	 RSCR |= _BM(RSOS);    // start 3D vector length calculation
	 _SEI;
	 /* LLR-Ref: 090 */
	 while( !(RSFR & _BM(RSOFF)) ){}

	 gRssiVector = RSRES4L;
	 gRssiVector|= (RSRES4H<<8);
	 /* LLR-Ref: 110 */
	 RSFR = _BM(RSOFF);

	 /* LLR-Ref: 120 */
	 g_sLfRssi.bStatus &= ~(LFRSSI_STATUS_BM_MEAS_DATA_AVAILABLE_FLAG | LFRSSI_STATUS_BM_CHANCORR_DATA_AVAILABLE_FLAG | LFRSSI_STATUS_BM_LINEAR_DATA_AVAILABLE_FLAG | LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG);
	 g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_3DVEC_DATA_AVAILABLE_FLAG;
	ATA_lfRssiClose_C();
// 	gExtRssiAnt=rssi[0];
// 	if(gExtRssiAntMax<rssi[1])
// 		gExtRssiAntMax=rssi[1];
// 	if(gExtRssiAntMax<rssi[2])
// 		gExtRssiAntMax=rssi[2];
	//
	// 	g_sLfRssiCtrl_flash.bFlags     = LFRSSICONFIG_FLAGS_RESET;
	// 	g_sLfRssiCtrl_flash.bStatus    = LFRSSICONFIG_STATUS_RESET;
	// 	g_sLfRssiFlowCtrl_flash.bIndex = 0;
	// 	g_sLfRssiFlowCtrl_flash.bLastRfTxStateIndex = 0;
}
// void rfTxShutdown(void)
// {
// 	g_sRfTxFlowCtrl_bIndex=9;
// 	do
// 	{
// 		ATA_rfTxProcessing_C();		
// 	}while (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_ACTIVE);
// }
void SystemSleep(void)
{	
	PRR0=0xff;	
	PRR2=0xff;
	PRR1=0xff;
	
	//else
	//{
		//PRR1=0xff;
	//}	
	bLFID0Wake_Reset();
	bLFRxFinish_Reset();
//	LEDTestMonitor();
// 	PCICR=0x02;//使能 PCINT[15:8]电平变化中断
// 	PCIFR=0x03;//清除中断标志
// 	PCMSK1=0xd8;//4个按键可以唤醒单片机	
	PORTC=0x00;
	DDRC =0x00;
	LED_OFF();
	BAT_OFF();
	if (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_ACTIVE)
	{
		ATA_rfTxShutdown_C();
	}
	while(1)
	{
		if(((PIND&0xfe)==0xfe) && !(PINC&0x04) )
		{
			break;
		}
	}
	if(RomStateFlags.Bits.bRomWrited)
	{
		PRR1=0x5f;
		ATA_lfRxInit_C();
		LFCPR = _BM(LFCPCE);
		LFCPR = _BM(LFCALRY);
		RomData_ReadBytes(EEDataOriginAddr+0x09, SSID, 4);
		ATA_lfRxEnableWakeup_flash_C();
	}
	ATA_globalsClkSwitchMrc_C();	
	ATA_globalsWdtDisable_C();	
	MCUCR &= ~(1<<ENPS);//取消端口设定
	ATA_globalsSleep_C(LF_SLEEP_MODE);
}
void SystemReset(void)
{
	ATA_globalsWdtEnable_C(0x1a);
	while(1)	
	{
		
	}
}

__attribute__((section(".exitData"))) uint8_t exitFlashData[320]=
{
	0x0C,0x94,0x66,0x40,0x0C,0x94,0x70,0x40,   //8
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //16
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //24
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //32
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //40
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //48
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //56
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //64
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //72
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //80
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //88
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //96
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //104
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //112
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //120
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //128
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //136
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //144
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //152
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //160
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //168
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //176
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //184
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //192
	0x0C,0x94,0x70,0x40,0x0C,0x94,0x70,0x40,   //200
	0x0C,0x94,0x70,0x40,0x11,0x24,0x1F,0xBE,   //208
	0xCF,0xEF,0xD5,0xE0,0xDE,0xBF,0xCD,0xBF,   //216
	0x0E,0x94,0x72,0x40,0x0C,0x94,0x87,0x40,   //224
	0x0C,0x94,0x00,0x40,0x81,0xE8,0x8E,0xB9,   //232
	0x73,0x9A,0x8F,0xEF,0x8B,0xB9,0x85,0xE0,   //240
	0x8A,0xB9,0x94,0xE0,0x2F,0xE7,0x3A,0xE1,   //248
	0x86,0xE0,0x21,0x50,0x30,0x40,0x80,0x40,   //256
	0xE1,0xF7,0x00,0xC0,0x00,0x00,0x8B,0xB1,   //264
	0x89,0x27,0x8B,0xB9,0xF3,0xCF,0xF8,0x94,   //272
	0xFF,0xCF,								//280
	          0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//288
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//296
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//304
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//312
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//320
};
BOOTLOADER_SECTION void exitProc(void)
{
	uint8_t i;//,sreg;	
	uint16_t K,exitDataIndex;
	uint16_t address;
	//sreg = SREG;
	exitDataIndex=0;
	address=0;
	for(address=0x8000;address<0xe000;address+=SPM_PAGESIZE)
	{
		boot_page_erase_safe(address); //擦除一个Flash页
		boot_spm_busy_wait(); //等待擦除完成
		if(address<0x8140)
		{
			for(i=0;i<SPM_PAGESIZE;i+=2,exitDataIndex+=2) //将数据填入Flash缓冲页中
			{			//pgm_read_byte
				K=MAKE_SHORT(pgm_read_byte(&exitFlashData[exitDataIndex+1]),pgm_read_byte(&exitFlashData[exitDataIndex]));
				boot_page_fill_safe(address+i,K);
			}
			boot_page_write_safe(address); //将缓冲页数据写入一个Flash页
			boot_spm_busy_wait (); //等待写入完成
		}
		_delay_ms(20);
	}
	while (1)
	{
// 		for(K=0;K<12500;K++)
// 		{
// 			for(i=0;i<243;i++)
// 			_NOP;
// 		}
		_delay_ms(500);
		PORTD ^= _BM(2);
	}	
// 	boot_rww_enable_safe();
// 	SREG = sreg;	
// 	asm("jmp 0x4000\n");
};
#define uartRx() (PINB & _BM(PORTB6))
#define uartRxLen 2+2+2+64+2
uint8_t uartRxData[uartRxLen];//16 16 com len addrH addrL data*64 sum 0d
BOOTLOADER_SECTION void boot_RxData(void)
{
	uint8_t i,j;	
	for(j=0;j<uartRxLen;j++)
	{
		uartRxData[j]=0;
		while(uartRx());
		_delay_us(52+104);
		for(i=0;i<8;i++)
		{
			LED_Toggle();
			if(uartRx())
			{	
				uartRxData[j] |= _BM(i);
				_delay_us(100);
			}
			else
			{
				_delay_us(102);
			}
		}
	}
}
#define uartTx_ON() PORTB |= _BM(5)
#define uartTx_OFF() PORTB &= ~_BM(5)
uint8_t uartTxData[10];
uint8_t uartTxDataCount;
BOOTLOADER_SECTION void boot_TxData(void)
{
	uint8_t i,j;
	for(j=0;j<uartTxDataCount;j++)
	{
		uartTx_OFF();
		_delay_us(100);
		for(i=0;i<8;i++)
		{
			if(uartTxData[j] & _BM(i))
				uartTx_ON();
			else
				uartTx_OFF();
			_delay_us(98);
		}
		uartTx_ON();
		_delay_us(110);
	}
}
BOOTLOADER_SECTION void BootloaderProc(void)
{
	uint8_t i,sum;//,sreg;
	uint16_t K;
	uint16_t address;
	//sreg = SREG;
	address=0;
	LED_ON();
	PORTB |= _BM(PORTB5);
	DDRB |= _BM(PORTB5);
	PORTB |= _BM(PORTB6);
	DDRB &= ~_BM(PORTB6);
	while(1)
	{
		boot_RxData();
		if(uartRxData[0]!=0x16 || uartRxData[0]!=0x16 || uartRxData[uartRxLen-1]!=0x0d)
			continue;
		sum=0;
		for(i=0;i<(uartRxLen-2);i++)
		{
			sum=(uint8_t)(sum+uartRxData[i]);
		}
		if(sum!=uartRxData[uartRxLen-2])
			continue;
		address=MAKE_SHORT(uartRxData[4],uartRxData[5]);
 		boot_page_erase(address); //擦除一个Flash页
 		boot_spm_busy_wait (); //等待擦除完成
		//boot_rww_enable_safe();
 		for(i=0;i<SPM_PAGESIZE;i+=2) //将数据填入Flash缓冲页中
 		{			//pgm_read_byte
 			K=MAKE_SHORT(uartRxData[7+i],uartRxData[6+i]);
 			boot_page_fill(address+i,K);
 		}
 		boot_page_write(address); //将缓冲页数据写入一个Flash页
 		boot_spm_busy_wait (); //等待写入完成
		//boot_rww_enable_safe();
		_delay_ms(20);
		uartTxData[0]=0x16;
		uartTxData[1]=0x16;
		uartTxData[2]=uartRxData[2];
		uartTxData[3]=2;
		uartTxData[4]='O';
		uartTxData[5]='K';
		uartTxData[6]='O';
		uartTxData[7]=0x0d;
		uartTxDataCount=8;
		boot_TxData();
		if(uartRxData[2]==0x02)//0x02 指令表示最后一帧
			break;
	}	
	while (1)
	{
		_delay_ms(500);
		PORTD ^= _BM(2);
	}
}