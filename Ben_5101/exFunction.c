//Build F:\work_tfs\MCU\WDJ\PLC_Flow\PLC_Flow for device 16F689
//Using driver d:\Program Files\HI-TECH Software\PICC\9.71a\bin\picc.exe
#include "HardwareProfile.h"

unsigned char sub_1AF5_CheckRomData(void)//存储器中数据是否正确
{
	unsigned char x;
	M24XX_Read(wfTempList,2,0x9e);
	if(wfTempList[0]==0xff)
	{
		if(wfTempList[1]==0xff)
		{
			BAT_CL=1;
			wfTempList[0]=eeprom_read(0x9e);
			wfTempList[1]=eeprom_read(0x9f);
			M24XX_Write(wfTempList,2,0x9e);
		}
	}
	M24XX_Read(wfTempList,2,0x9e);
	x=wfTempList[0]+wfTempList[1];
	if(x==0x00)
	{
		RAM.fCA.Byte=wfTempList[0]&0x7f;//0x21
		x=RAM.fCA.Byte&0xee;//0x20
		if(x==0x04)
			return 1;
		//return x^0x04;//0x24
	}
	return 0;
}
void adr_12C8(void)
{

}
unsigned char CHG_AD(unsigned char adH)
{
	unsigned char x1,x2;
	if(adH<95)
	{
		adH=0x95;
	}
	else 
	{
		if(adH>=0xfe)
		{
			adH=0xfb;
		}
	}
	x1=0xff;
	x2=0x95;
	while(1)
	{
		if(adH<x2)
			break;
		x1++;
		x2=x2+3;
	}
	x1=x1+0x64;
	return x1;
}
void EE_WT_CHK(unsigned char addr)
{
	unsigned char x;
	f21.eeFF=0;
	M24XX_Read(&x,1,addr);
	//x=x^0xff;
	if(x==0xff)
	{
		x=0;
		M24XX_Write(&x,1,addr);
	}
	else
	{
		//f21.eeFF=1;
		x=0xff;
		M24XX_Write(&x,1,addr);
	}
}
void CHK_VOL()
{
	unsigned char A_P,X_P,W;
	LED=1;
	ADON=1;
	GO_nDONE=1;
	while(GO_nDONE==1);
	RAM.RAM_84= CHG_AD(ADRESH);
	A_P=RAM.RAM_84;
	ADON=0;
	if(f20.power)
	{
		while(1)
		{
			if(CCP1IF==1)
			{
				LED=0;
				return;
			}
		}
	}
	M24XX_Read(&X_P,1,0x95);
	if(X_P==0)
	{
		A_P=0x0a;
	}
	X_P=0xff;
	if(A_P>=0x81)
	{
		LED=1;
		X_P=0;
	}
	M24XX_Read(&W,1,0x95);
	if(W!=X_P)
	{
		EE_WT_CHK(0x95);
	}
	CCP1CON=0b00001010;
	CCPR1H=HIGH_BYTE(16000);
	CCPR1L=LOW_BYTE(16000);
	ON_CCP();
	while(1)
	{
		if(CCP1IF==1)
		{
			LED=0;
			return;
		}
	}
}
void sub_1C8F_waitRxNibbleByte_inRAM_A4(void)//接收完的数据应在RAM_A4 BANKED中
{
	f20.rxok=0;
	while(f20.rxok==0)
	{
		CLRWDT();
	}
}
void adr_1C73_exitCCP(void)//退出捕捉，
{
	f20.ONbuzh=0;
	f20.ONcal=0;
	f20.rxok=0;
	CCP1IF=0;
	CCP1IE=0;
	PEIE=0;
	GIE=0;
	CCP1CON=0;
}
void sub_1C7A_RxByte(void)//接收一个字节
{
	sub_1C8F_waitRxNibbleByte_inRAM_A4();
	if(f21.over1)
		return;
	RAM.RAM_A3=RAM.RAM_A4;
	sub_1C8F_waitRxNibbleByte_inRAM_A4();
	if(f21.over1)
		return;
	RAM.RAM_A3=MAKE_BYTE(RAM.RAM_A4,RAM.RAM_A3);	
}
void sub_1C9B_GetNeedRxLen_retCNT0(void)//返回值CNT0，为帧数据长度
{
	unsigned char x;
	CNT0=0;
	if(RAM.RAM_81==0x08)
		return;
	if(RAM.RAM_81==0x0f)
		return;
	if(RAM.RAM_81==0x6a)
		return;
	if(RAM.RAM_81==0x71)
		return;

	CNT0=2;
	if(RAM.RAM_81==0x73)
		return;
	if(RAM.RAM_81==0x79)
		return;	;

	CNT0=7;
	if(RAM.RAM_81==0x7a)
		return;
	CNT0=8;
	if(RAM.RAM_81==0x26)
		return;
	if(RAM.RAM_81==0x63)
		return;
	if(RAM.RAM_81==0x3e)
		return;
	if(RAM.RAM_81==0x39)
		return;
	CNT0=9;
}
void INI_T1_10mS()
{
	TMR1ON=0;
	TMR1L=0xEF;
	TMR1H=0xD8;
	TMR1ON=1;
	TMR1IE=0;
	TMR1IF=0;
}

void DELAY_N_10MS(unsigned char n)//延时n个10ms等待捕捉完成，完成后立即返回
{
	INI_T1_10mS();
	//捕捉打开时才会用到
	while(n--)
	{
		if(f20.rxok)//未捕捉完成
		{
			return;
		}
		while(TMR1IF==0);
		INI_T1_10mS();
	}
	
}
void sub_1C30_RxFrame_pBuf80(unsigned char n)//接收1帧数据，n为帧数据起始延时
{
	unsigned char i;
	f20.ONbuzh=1;//捕捉开始
	f20.ONcal=0;//清除计算标志
	f20.rxok=0;
	
	RAM.RAM_A4=0;

	TMR3IF=0;
	TMR3ON=0;
	TMR3L=0;
	TMR3H=0;
	TMR3ON=1;

	f21.over1=1;

	CCP1CON=0b00000100;
	CCP1IF=0;
	CCP1IE=1;
	PEIE=1;
	GIE=1;
	DELAY_N_10MS(n);
	if(f21.over1)
	{
		adr_1C73_exitCCP();
		return;
	}	
	/************************************************************************/
	/* RX_HEAD                                                              */
	/************************************************************************/
	while(1)
	{
		sub_1C8F_waitRxNibbleByte_inRAM_A4();
		if(f21.over1)
		{
			adr_1C73_exitCCP();
			return;
		}
		if(RAM.RAM_A4!=0)//如果收到的是0，则重新等待
			break;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	sub_1C7A_RxByte();
	if(f21.over1)
	{
		adr_1C73_exitCCP();
		return;
	}	
	RAM.RAM_81=RAM.RAM_A3;
	sub_1C9B_GetNeedRxLen_retCNT0();
	if(CNT0==0)
	{
		adr_1C73_exitCCP();
		return;
	}
	CCPRx[0]=RAM.RAM_81;
	for(i=0;i<CNT0;i++)
	{
		sub_1C7A_RxByte();//接收1个字节
		if(f21.over1)
		{
			adr_1C73_exitCCP();
			return;
		}
		CCPRx[i+1]=RAM.RAM_A3;
	}
	
}
void LOAD_8EE_F(unsigned char* pBuf,unsigned char addr)//addr由X_P传递
{
	unsigned char i;
	if(f20.RDee)//'0'read 24C02,'1'read EEPROM
	{
		for(i=0;i<8;i++)
		{
			*pBuf=eeprom_read(addr+i);
			pBuf--;
		}
	}
	else
	{
		M24XX_Read(pBuf-7,8,addr);
	}
}
void sub_1B48_Load8EEto93to9A_byX_P(unsigned char addr)
{
	LOAD_8EE_F(&RAM.RAM_9A,addr);
//	LFSR	FSR0,0X9A		;RAM_9A
//		CALL	LOAD_8EE_F		;
}
void sub_19D9_LoadData(void)
{
	unsigned char x;
	if(RAM.fCA.b1)
	{
		x=0x4e;
	}
	else
		x=0x05;
	sub_1B48_Load8EEto93to9A_byX_P(x);
	
	if(RAM.fCA.b1)
	{
		x=0x56;
	}
	else
		x=0x9b;
	M24XX_Read(&RAM.RAM_97,1,x);
	RAM.RAM_98=0;
	RAM.RAM_99=0;
	RAM.RAM_9A=RAM.RAM_C2;
}
void sub_19F7_Data83and8Bis93(void)
{
	unsigned char i,x;
	RAM.RAM_81=RAM.RAM_C2;
	RAM.RAM_82=RAM.RAM_C3;
	if(RAM.fCA.b1==1 && RAM.RAM_82==0x0a)
	{
		sub_19D9_LoadData();//加载存储器中的数据
	}
	for(i=0;i<8;i++)
	{
		x=(&RAM.RAM_93)[i];
		(&RAM.RAM_8B)[i]=x;
		(&RAM.RAM_83)[i]=x;
	}
}
void sub_1BE7_93to9A_FanXuPaiLie(void)
{
	unsigned char i,x,w;
	pFSR0=&RAM.RAM_93;
	pFSR1=&RAM.RAM_9A;
	for(i=0;i<4;i++)
	{
		x=*pFSR0;
		*pFSR0=*pFSR1;
		pFSR0++;
		*pFSR1=x;
		pFSR1--;
	}
}

void ON_CCP()
{
	TMR3ON=0;
	TMR3L=3;
	TMR3H=0;
	TMR3ON=1;
	CCP1IF=0;
}
void INI_BJQ()
{
	CCP1CON=0b00001010;//发生比较匹配时产生软件中断 （CCPx 引脚不受影响， CCPxIF 置 1）
	CCPR1H=0x01;//500
	CCPR1L=0xf4;
}
void sub_1D4D_RFTXByte(unsigned char D)
{
	unsigned char i,x;
	for(i=0;i<8;i++)
	{
		while(CCP1IF==0);
		x = PORTC & 0b11000111;
		if((D&0x01)==0x01)
		{
			PORTC = x | 0b00101000;
		}
		else
			PORTC = x | 0b00011000;
		D=D>>1;
		ON_CCP();

		while(CCP1IF==0);

		PORTC = PORTC ^ 0b00110000;

		ON_CCP();

	}
}
void ON_CCP_IRB()
{
	TMR3ON=0;
	TMR3L=9;
	TMR3H=0;
	TMR3ON=1;
	CCP1IF=0;
}
void ON_CCP_IRA()
{
	TMR3ON=0;
	if(RAM.fCB.b6)
		TMR3L=32;
	else
		TMR3L=12;	
	TMR3H=0;
	TMR3ON=1;
	CCP1IF=0;
}
void sub_1D91_TxNibbleByte(unsigned char halfD)
{
	unsigned int x;
	unsigned char i;
	while(CCP1IF);
	if(f20.power)
	{
		//车载供电时
		IR_B=0;
		__delay_us(30);
		IR_B=1;
		x=halfD;
		x=x*0x40;
		x=x+0x03e2;
		CCPR1H=HIGH_BYTE(x);
		CCPR1L=LOW_BYTE(x);
		CCP1CON=0b00001010;//发生比较匹配时产生软件中断 （CCPx 引脚不受影响， CCPxIF 置 1）
		ON_CCP_IRB();
	}
	else
	{
		//电池供电时
		if(RAM.fCB.b6)
		{
			for(i=0;i<6;i++)
			{
				IR_A=!IR_A;
				__delay_us(3);
				IR_A=!IR_A;
				__delay_us(18);
			}
		}
		else
		{
			for(i=0;i<7;i++)
			{
				IR_A=!IR_A;
				__delay_us(5);
				IR_A=!IR_A;
				__delay_us(10);
			}			
		}
		x=halfD;
		x=x*0x80;
		x=x<<1;
		x=x+0x0488;
		CCPR1H=HIGH_BYTE(x);
		CCPR1L=LOW_BYTE(x);
		CCP1CON=0b00001010;
		ON_CCP_IRA();
	}
}
void sub_1D80_IRTxByte(unsigned char D)
{
	sub_1D91_TxNibbleByte(LOW_NIBBLE(D));
	sub_1D91_TxNibbleByte(HIGH_NIBBLE(D));
}
void sub_1D43_TxByte(unsigned char D)
{
	if(RF_EN)
	{
		sub_1D4D_RFTXByte(D);
	}
	else
		sub_1D80_IRTxByte(D);
}
void sub_1D42_TxByte0()
{
	sub_1D43_TxByte(0);
}

void sub_1D11_TxDataBuf(unsigned char* pBuf,unsigned char N)
{
	if(RF_EN)
	{
		INI_BJQ();
		ON_CCP();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
		sub_1D42_TxByte0();
	}
	while(N--)
	{
		sub_1D43_TxByte(*pBuf);
		pBuf++;
	}
}
void adr_1D04_TxFrame_lenBYX_P_buf80(unsigned char X_P)//N由X_P传递
{
	//CNT0=N+1;
	//pFSR0=&RAM.RAM_80;

	CCP1CON=0b00001010;//发生比较匹配时产生软件中断 （CCPx 引脚不受影响， CCPxIF 置 1）
	CCPR1H=0;
	CCPR1L=100;
	ON_CCP();
	sub_1D11_TxDataBuf(&RAM.RAM_80,X_P+1);
	sub_1D91_TxNibbleByte(0);
	CCP1CON=0;
}
void TX_IR_B(unsigned char X_P)//N由X_P传递:10 ram81 ram82 数据区：ram83~ram8a
{
	RAM.RAM_80=0x10;	
	adr_1D04_TxFrame_lenBYX_P_buf80(X_P);
}
void sub_17F2_Answer_0x0F(void)
{
	RAM.RAM_D1=0x88;

	RAM.RAM_C2=0x0d;
	RAM.RAM_C3=0;

	f20.RDee=1;//读eeprom
	sub_1B48_Load8EEto93to9A_byX_P(0x88);
	if(RAM.fCB.b6)
	{
		sub_1BE7_93to9A_FanXuPaiLie(); //loaded data 反序
		//CALL	sub_1BE7		;loaded data 反序
	}
	sub_19F7_Data83and8Bis93();
	DELAY_N_10MS(10);
	BAT_CL=1;
	TX_IR_B(10);
	//else
	//{
	//	// 	BRA	adr_17F3		;
	//}
// 	BTFSS	RAM_CB,6,BANKED		;
// 	BRA	adr_17F3		;
// 	CALL	sub_1BE7		;loaded data 反序
}
unsigned char sub_17DC_waitRxFrame(void)//即adr_17DC,A_P中为接收的数据
{
	unsigned char x=0,rx;
	while (1)
	{
		sub_1C30_RxFrame_pBuf80(x);//帧数据头延时256个10ms，接收一帧数据
		if(f21.over1)
		{
			sub_1C30_RxFrame_pBuf80(100);//超时则，再延时100个10ms，接收数据
			if(f21.over1)//再超时则退出
			{
				BAT_CL=0;
				return 0;
			}
		}
		RAM.RAM_C2=RAM.RAM_81;
		rx=RAM.RAM_81;
		if(rx==0x0f)//收到的是0x0f
		{
			sub_17F2_Answer_0x0F();
			x=50;
		}
		else 
		{
			BAT_CL=0;
			return rx;
		}
	}
	return 0;
	
// 	if(A_P!=0x0f)
// 	{
// 		BAT_CL=0;
// 		return;
// 	}
	
// 	MOVLW	.50			;
// 	BRA	adr_17DD		;adr_17DC

}
void LED_FLASH(void)
{
	LED=1;
	DELAY_N_10MS(30);
	LED=0;
	DELAY_N_10MS(30);
	LED=1;
	DELAY_N_10MS(30);
	LED=0;
	DELAY_N_10MS(30);
}
void sub_0C11(unsigned char N)
{

}
void sub_184A_TX_IR_B_10_05_00_data83()
{
	RAM.RAM_81=0x05;
	TX_IR_B(0x0a);
}
void adr_183c_wirteI2C_apisAddr_data8B(unsigned char ap)
{
	BAT_CL=1;
	M24XX_Write(&RAM.RAM_8B,8,ap);
}
void sub_1804_WriteDecode_hashcal_TxAnswer(void)
{
	unsigned char i,x,index,addr;
	for(i=0;i<8;i++)
	{
		(&RAM.RAM_8B)[i]=(&RAM.RAM_83)[i];
	}
	if(RAM.fCB.b5)//调用写入解码程序
	{
		//已有写入的解码程序,结果存放在RAM_8B
	}
	if(f_cmd35)
	{
		f_cmd35=0;
		if(f_zero)
		{
			//对83地址的数据进行hashcal
			sub_184A_TX_IR_B_10_05_00_data83();
			return;
		}
	}
	if(RAM.RAM_82<RAM.RAM_C4)//RAM.RAM_C4=0x10?????
	{
		index=RAM.RAM_82;
		addr=index*0x08;
		addr=addr+0x88;
		switch(RAM.RAM_81)
		{
		case 0x0e:
			//0x88+i*8			
			break;
		case 0x00:
			addr=addr-0x08;
			//0x80+i*8
			break;
		case 0x68:
			addr=addr-0x86;//;-0x08-0x7e;
			//0x02+i*8
			break;
		case 0x03:
			addr=addr-0x90;//-0x08-0x7e-0x02-0x02;
			//i*8-0x02
			break;
		default:
			addr=addr-0x88;//-0x08-0x7e-0x02;
			//0x00+i*8
			break;
		}		
		adr_183c_wirteI2C_apisAddr_data8B(addr);
	}
	//对83地址的数据进行hashcal
	sub_184A_TX_IR_B_10_05_00_data83();
}
void sub_18C4_CheckSameFrame_82isFF()
{
	sub_1C30_RxFrame_pBuf80(100);
	if(f21.over1)
		return;
	f21.over1=1;
	if(RAM.RAM_81==RAM.RAM_C2)//若两帧命令字相同
	{
		f21.over1=0;
		if(RAM.RAM_82!=0xff)
			sub_1804_WriteDecode_hashcal_TxAnswer();
	}
}
void whileLEDFlash()
{
	while(1)
	{
		LED_FLASH();
	}
}
void sub_1B07_I2C_addr9e_dataWandnotW(unsigned char w)//写入遥控状态
{
	RAM.RAM_8B=w;
	RAM.RAM_8C=0-RAM.RAM_8B;
	M24XX_Write(&RAM.RAM_8B,2,0x9e);
}
unsigned char sub_1B9D_XOR_93to9A_reA_P()
{
	unsigned char i,ret;
	ret=RAM.RAM_93;
	for(i=1;i<8;i++)
	{
		ret =ret ^ (&RAM.RAM_93)[i];
	}
	return ret;
}
unsigned char sub_1BB6_XOR_xptoxp8_XOR_I2C0809_reW(unsigned char xp)
{
	unsigned ap,ret;
	sub_1B48_Load8EEto93to9A_byX_P(xp);
	ap=sub_1B9D_XOR_93to9A_reA_P();
	M24XX_Read(wfTempList,2,xp+8);
	ret=ap;
	ret=ret^wfTempList[0];
	ret=ret^wfTempList[1];
	return ret;
}
unsigned char sub_1BAF_CheckEEdataXOR_is0(unsigned char w)
{
	unsigned char xp;
	xp=w;
	M24XX_Read(wfTempList,1,xp+8);
	if(wfTempList[0]>0x7f)
		return 0;//+8的EE不能大于7F.
	if(sub_1BB6_XOR_xptoxp8_XOR_I2C0809_reW(xp)==0)//检查当前EE地址之后的10个是否异或结果为0，
		return 1;
	else 
		return 0;
}
unsigned char sub_16B8_CheckEE41andEE49_reC3()
{
	unsigned char xp;
	unsigned char EE_49,EE_53;
	RAM.RAM_C3=0;
	RAM.fD3.b1=1;
	RAM.fC9.b4=1;
	RAM.fC9.b5=1;
	M24XX_Read(&EE_53,1,0x53);
	M24XX_Read(&EE_49,1,0x49);
	if(sub_1BAF_CheckEEdataXOR_is0(0x41)==1)//检查当前EE地址之后的10个是否异或结果为0
	{
		RAM.fD3.b1=0;//0x41开始的数据段匹配
		RAM.fC9.b4=0;
		if(EE_49==0)//EE_49
		{
			if(sub_1BAF_CheckEEdataXOR_is0(0x4B)==1)//检查当前EE地址之后的10个是否异或结果为0
			{
				RAM.fC9.b5=0;
				if(EE_53==0x1f)
				{
					//adr_1704
					RAM.fD3.b1=0;//0x41开始的数据段匹配
					xp=EE_49; //EE_49
				}
				else
				{
					//adr_170B
					RAM.fD3.b1=1;//0x4B开始的数据段匹配
					xp=EE_53;//EE_53
				}
			}
			else
			{
				//adr_1704
				RAM.fD3.b1=0;//0x41开始的数据段匹配
				xp=EE_49; //EE_49
			}
		}
	}
	if(sub_1BAF_CheckEEdataXOR_is0(0x4B)==1)//检查当前EE地址之后的10个是否异或结果为0
	{
		RAM.fC9.b5=0;
		if(RAM.fD3.b1)//0x41开始的数据段未匹配
		{
			//adr_170B
			RAM.fD3.b1=1;//0x4B开始的数据段匹配
			xp=EE_53;//EE_53
		}
		else
		{
			if(EE_53==0)
			{
				if(EE_49==0x1f)
				{
					//adr_170B
					RAM.fD3.b1=1;//0x4B开始的数据段匹配
					xp=EE_53;//EE_53
				}
				else
				{
					//adr_1704
					RAM.fD3.b1=0;//0x41开始的数据段匹配
					xp=EE_49; //EE_49
				}
			}
			else
			{
				if(EE_53<EE_49)
				{
					//adr_1704
					RAM.fD3.b1=0;//0x41开始的数据段匹配
					xp=EE_49; //EE_49
				}
				else
				{
					//adr_170B
					RAM.fD3.b1=1;//0x4B开始的数据段匹配
					xp=EE_53;//EE_53
				}
			}
		}
	}
	else
	{
		//adr_1704
		RAM.fD3.b1=0;//0x41开始的数据段匹配
		xp=EE_49; //EE_49
	}
	//adr_1710
	if(RAM.fC9.b5==1 && RAM.fC9.b4==1)//均未匹配
	{
		RAM.RAM_C3=xp;
		return RAM.RAM_C3;
	}
	//RAM.RAM_D4=EE_49;
	if(EE_49==0)
	{
		xp=EE_53;
		if(EE_53==0x1f)
		{
			RAM.RAM_C3=xp;
			return RAM.RAM_C3;
		}
	}
	xp=EE_53;
	if(EE_53==0)
	{
		xp=EE_49;
		if(EE_49==0x1f)
		{
			RAM.RAM_C3=xp;
			return RAM.RAM_C3;
		}
		xp=EE_53;
	}
	if(xp==EE_49)
	{
		xp=EE_53;
		RAM.RAM_C3=xp;
		return RAM.RAM_C3;
	}
	else if(xp>EE_49)
	{
		xp=EE_49;
		RAM.RAM_C3=xp;
		return RAM.RAM_C3;
	}
}
unsigned char BJ_EQL(unsigned char tmp_cL)
{
	unsigned char i,TEMP2;
	TEMP2=0x7f;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			if(TEMP2==tmp_cL)// I2C_5D | 0x03;
			{
				return i;
			}
			else
			{
				TEMP2--;
			}
		}
	}
	return 0;
}
void sub_1468()
{
	unsigned char Temp_EE,tmp_cL,RAM_BF,i,TEMP2;
	sub_16B8_CheckEE41andEE49_reC3();
	if(RAM.fD3.b1==0)//0:0x41开始的数据段匹配
	{
		sub_1B48_Load8EEto93to9A_byX_P(0x41);//读出需要修正段的前8个数据
	}
	else
		sub_1B48_Load8EEto93to9A_byX_P(0x4b);
	M24XX_Read(&Temp_EE,1,0x5d);//i2c中5d地址的数据
	tmp_cL=Temp_EE|0x03;
	RAM_BF=Temp_EE-0x04;
	while(1)
	{
		RAM_BF--;
		if(RAM_BF==0xff)
			break;
		//0x93开始的内存地址进行滚步
	}
	RAM.RAM_C5=0x5f;
	sub_17B4();
	RAM_BF=Temp_EE-0x04;//D2
	RAM_BF=tmp_cL-RAM_BF;
	while(1)
	{
		RAM_BF--;
		if(RAM_BF==0xff)
			break;
		//0x93开始的内存地址进行滚步
	}


	RAM.RAM_C5=0x73;
	sub_17B4();

	for(i=0;i<8;i++)
	{
		(&RAM.RAM_93)[i]=0xff;
	}

	i=BJ_EQL(tmp_cL);// I2C_5D | 0x03;
	(&RAM.RAM_93)[i]=i;
	M24XX_Write(&RAM.RAM_93,8,0x69);
}
void sub_1854()
{
	unsigned char i;
	while(1)
	{
		sub_18C4_CheckSameFrame_82isFF();
		if(f21.over1)
			whileLEDFlash();
		if(RAM.RAM_82==0xff)
			break;
	}
	if(RAM.RAM_81==0x06 || RAM.RAM_81==0x68 || RAM.RAM_81==0x35 || RAM.RAM_81==0x37)
	{
		
	}
	else
	{
		//	CALL	sub_18E0		;检查收到数据的完整性
		//BNZ	adr_18BA		;
	}
	for(i=0;i<8;i++)
		(&RAM.RAM_83)[i]=0x00;
	/************************************************************************/
	/* ?                                                                     */
	/************************************************************************/
	if(RAM.RAM_81==0x00 || RAM.RAM_81==0x06||RAM.RAM_81==0x07 || RAM.RAM_81==0x03)
		sub_1B07_I2C_addr9e_dataWandnotW(0);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	switch(RAM.RAM_81)
	{
	case 0x00:
		sub_1B07_I2C_addr9e_dataWandnotW(0x21);
		break;
	case 0x06:
		sub_1B07_I2C_addr9e_dataWandnotW(0x27);
		break;
	case 0x07:
		sub_1B07_I2C_addr9e_dataWandnotW(0x27);
		break;
	case 0x03:
		sub_1B07_I2C_addr9e_dataWandnotW(0x0c);
		break;
	}
	if(RAM.RAM_81==0x0e || RAM.RAM_81==0x68)
	{
		sub_184A_TX_IR_B_10_05_00_data83();
		LED=1;
	}
	if(RAM.RAM_81==0x00)
	{
		//xp=0x21
		//sub_1B07		;写入遥控状态
		sub_184A_TX_IR_B_10_05_00_data83();
		LED=1;
	}
}
void sub_17B4(void)
{
	unsigned char rx,i;
	rx=sub_17DC_waitRxFrame();
	if(f21.over1)
	{
		whileLEDFlash();
	}
	RAM.fCB.b5=1;//命令字37时标志
	if(rx==0x04 || rx==0x37)
	{
		RAM.RAM_C4=0x10;//用于判断是否需要存储的最大命令号
		f20.RDee=1;
		sub_1B48_Load8EEto93to9A_byX_P(0x80);
		sub_1BE7_93to9A_FanXuPaiLie();
		sub_1804_WriteDecode_hashcal_TxAnswer();
		sub_1854();//no return -call
	}
	else if(rx==0x7a)
	{
		CCP1CON=0b00001010;
		CCPR1H=HIGH_BYTE(500);
		CCPR1L=LOW_BYTE(500);
		ON_CCP();
		//A_P=RAM.RAM_82;
		switch(RAM.RAM_82)
		{
		case 0:
			while(1)
			{
				sub_1D43_TxByte(0);
			}
			break;
		case 1:
			f20.power=0;
			while(1)
			{
				sub_1D43_TxByte(0);
			}
			break;
		case 2:
			f20.power=0;
			RF_EN=1;
			while(1)
			{
				sub_1D43_TxByte(0);
			}
			break;
		case 3:
			f20.power=0;
			RF_EN=1;
			while(1)
			{
				CLRWDT();
				if(PW_IN==0)
					RESET();
			}
			break;
		case 4:
			f20.power=0;
			RF_EN=1;
			RFOUT=0;
			RFOUT_OP=1;
			while(1)
			{
				CLRWDT();
				if(PW_IN==0)
					RESET();
			}
			break;
		case 5:
			f20.power=0;
			RF_EN=0;
			LED=1;
			while(1)
			{
				CLRWDT();
				if(PW_IN==0)
					RESET();
			}
			break;
		case 6:
			f20.power=0;
			RF_EN=0;
			LED=0;
			M24XX_Write(&RAM.RAM_83,6,0x98);
			//break;
		case 7:
			f20.power=1;
			for(i=0;i<8;i++)
			{
				(&RAM.RAM_83)[i]=eeprom_read(0x98+i);
			}
			if((RAM.RAM_82 & 0x80)!=0)
			{
				RAM.RAM_83=0x53;//ID=74
				CHK_VOL();//ADC
				M24XX_Read(&RAM.RAM_87,1,0);
			}
			RAM.RAM_81=0x7b;
			__delay_20ms(5);
			TX_IR_B(8);
			break;
		case 8:
			f20.power=1;
			for(i=0;i<8;i++)
			{
				(&RAM.RAM_83)[i]=eeprom_read(0x9C+i);
			}
			if((RAM.RAM_82 & 0x80)!=0)
			{
				RAM.RAM_83=0x53;//ID=74
				CHK_VOL();//ADC
				M24XX_Read(&RAM.RAM_87,1,0);
			}
			RAM.RAM_81=0x7b;
			__delay_20ms(5);
			TX_IR_B(8);
			break;
		default:
			while(1)
			{
				CLRWDT();
				if(PW_IN==0)
					RESET();
			}
		}
	}
	else
	{
		whileLEDFlash();
	}
}
void POW_CAR(void)
{
	//sub_1D11_TxDataBuf(wfTempList,8);
	if(sub_1AF5_CheckRomData()==0)//存储器中数据是否正确
	{
		adr_12C8();
	}
	else if(RAM.fCA.Byte==0x4c)//判断9E地址是否为4c
	{

	}
	else if(RAM.fCA.Byte==0x0c)//判断9E地址是否为0c
	{

	}
	else if(RAM.fCA.Byte==0x21)//判断9E地址是否为21
	{
		sub_17B4();
	}
	//	GOTO	sub_1784		;no return
}
void POW_BAT(void)
{

}

