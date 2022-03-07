#ifndef   __FlagDefine_h__
#define   __FlagDefine_h__

#define LED_ON 0
#define LED_OFF 1

#define Lock_ON 0
#define Lock_OFF 1

#define FlashAddr 0x0fe0
/************************************************************************/
/* 第一个为最低位                                                      */
/************************************************************************/
typedef union   
{
	struct
	{
		unsigned bMBusRx:1;
		unsigned bWifiSet:1;
		unsigned bWXSetWifi:1;	
		//unsigned bEnterDlgJump:1;
		unsigned bError:1;
		unsigned bPrinted:1;
	};
	unsigned char AllFlag;
} GFlags;        // general flags

GFlags gFlags;
volatile unsigned int SystemTick;
volatile unsigned int RxTick;
unsigned char MBusAddr;


#endif
