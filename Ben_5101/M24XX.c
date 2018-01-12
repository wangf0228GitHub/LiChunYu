#include "HardwareProfile.h"
#include "M24XX.h"
#ifdef SimI2C
#include "SimI2C.h"
#endif

//uchar  Reeprom[5];

#ifdef SimI2C
void M24XX_Init(void)
{
	SimI2C_Init();
#ifdef M24XX_WP_W
	M24XX_WP_IO=0;
	M24XX_WP_W=0;
#endif
}

unsigned char M24XX_Read( unsigned char *pBuff,unsigned int Count,unsigned char Address )
{
	unsigned char ErrTimes=M24XX_RetryCount;	
	unsigned char bRight=0;
	while(ErrTimes--)
	{
		SimI2C_Start();

		SimI2C_SendByte(M24XX_ADDR_W);//写

		if(SimI2C_RecAck())
			continue;//无应答返回到循环,表示出错

// 		AddrH=Address>>8;
// 		SimI2C_SendByte(AddrH);//写读写地址高字节
// 		if(SimI2C_RecAck())
// 			continue;
		SimI2C_SendByte(Address);//写读写地址低字节
		if(SimI2C_RecAck())
			continue;
		SimI2C_Start();

		SimI2C_SendByte(M24XX_ADDR_R);//读

		if(SimI2C_RecAck())
			continue;
		while(--Count)
		{
			*pBuff=SimI2C_ReadByte();
			pBuff++;
			SimI2C_Ack();
		}
		*pBuff=SimI2C_ReadByte();//read last byte data
		SimI2C_NoAck();
		bRight=1;
		break;
	}
	SimI2C_Stop();
	return bRight;
}

unsigned char M24XX_Write( unsigned char *pBuff,unsigned char Count,unsigned char Address )
{

	unsigned char N;
	unsigned char* pB;
	unsigned char ErrTimes=M24XX_RetryCount;
	unsigned char bRight=0;	
	while(ErrTimes--)
	{
		SimI2C_Start();		

		SimI2C_SendByte(M24XX_ADDR_W);//写

		if(SimI2C_RecAck())
			continue;//无应答返回到循环

// 		AddrH=Address>>8;
// 		SimI2C_SendByte(AddrH);           //写读写地址高字节
// 		if(SimI2C_RecAck())
// 			continue;
		SimI2C_SendByte(Address);              //写读写地址低字节
		if(SimI2C_RecAck())
			continue;
#ifdef M24XX_WP_W
//		__delay_us(500);
 		M24XX_WP_W=0;
 		__delay_us(5);
#endif		
		N=Count;
		bRight=1;//********clr bErr
		pB=pBuff;
		while(N--)
		{
			SimI2C_SendByte(*pB);
			pB++;
			if(!SimI2C_RecAck())
				continue;
			bRight=0;
			break;
		}
		if(bRight==0)
			continue;
		break;		
	}
	SimI2C_Stop();
	__delay_ms(30);
#ifdef M24XX_WP_W
// 	__delay_ms(20);
 	M24XX_WP_W=1;
 	__delay_us(5);
#endif
	return bRight;
}
#endif

