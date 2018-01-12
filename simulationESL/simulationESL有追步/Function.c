#include "HardwareProfile.h"
const unsigned char rxHeader[2][10]=
{
	{0x90,0x91,0x92,0x93,0x94,0xa0,0xb0,0xc0,0xc5,0xc7},
	{0x90,0x98,0xa0,0xa8,0xb0,0xb8,0xc0,0xc8,0xe0,0xe8}
};
void UpdateEEPROM(void)
{
	unsigned char addr,eep,i,psw;
	eeprom_write(0x90,0x2c);
	addr=uartRxList[1]<<4;//找到地址
	eep=eeprom_read(0xf0+uartRxList[1]);
	if(eep==0xff)//第一次使用此钥匙
	{
		eeprom_write(0xf0+uartRxList[1],0x01);//前8为高位
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i,uartRxList[2+i]);
			lcyHashIn[i]=uartRxList[2+i];
		}
		lcyHashOnce();
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i+8,lcyHashOut[i]);
		}
	}
	else if(eep==0)//后8为高位hash
	{
		//替换原前8低位数据
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i,uartRxList[2+i]);
		}
		//并更改标志位，改到前8为高位地址
		eeprom_write(0xf0+uartRxList[1],0x01);		
	}
	else//前8为高位hash
	{
		addr+=8;
		//替换原后8低位数据
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i,uartRxList[2+i]);
		}
		//并更改标志位，改到后8为高位地址
		eeprom_write(0xf0+uartRxList[1],0x00);		
	}
	//与密码异或,并滚步
	for(i=0;i<8;i++)
	{
		psw=eeprom_read(0xd0+i);
		lcyHashIn[i]=uartRxList[2+i]^psw;
	}
	lcyHashOnce();
	for(i=0;i<8;i++)
	{
		uartTxList[2+i]=lcyHashOut[i];
	}
	uartTxList[0]=0x97;
	uartTxList[1]=uartRxList[1];
}
void ProcCommand_work_0x3x(void)
{
	unsigned char addr,eep,i;
	unsigned int step;
	eep=eeprom_read(0x90);
	if(eep==0x22)//刚刚renew 
	{
		uartTxList[0]=0x88;
		for(i=0;i<9;i++)
		{
			uartTxList[i]=0x00;
		}
	}
// 	eep=eeprom_read(0xff);
// 	if(eep==0xff)
// 		return;
	addr=uartRxList[1]<<4;
	eep=eeprom_read(0xf0+uartRxList[1]);
	if(eep==0)//后8为高位hash
		addr+=8;
	bSameHash_eep(&uartRxList[2],addr);
	if(gFlags.bSame)//如果相同,则发生错误，送回86 00 00 00 ---
	{
		uartTxList[0]=0x86;
		for(i=0;i<9;i++)
		{
			uartTxList[i]=0x00;
		}
		return;
	}
	//不相同，则滚步再检查
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=uartRxList[i+2];
	}	
	lcyHashOnce();
	bSameHash_eep(lcyHashOut,addr);
	if(gFlags.bSame)//如果相同，则成功
	{
		UpdateEEPROM();
		return;
	}
	/************************************************************************/
	/* 追步                                                                 */
	/************************************************************************/
	addr=uartRxList[1]<<4;
	eep=eeprom_read(0xf0+uartRxList[1]);
	if(eep==0)//后8为高位hash		
	{
		for(i=0;i<8;i++)
		{
			keyH[i]=eeprom_read(addr+8+i);
			keyL[i]=eeprom_read(addr+i);
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			keyH[i]=eeprom_read(addr+i);
			keyL[i]=eeprom_read(addr+8+i);
		}
	}
	bSameHash(keyL,keyH);
	if(gFlags.bSame)//相同只追高位
		gFlags.bHL=0;
	else
	{
		for(i=0;i<8;i++)
		{
			lcyHashIn[i]=keyH[i];
		}
		lcyHashOnce();
		bSameHash(lcyHashOut,keyL);
		if(gFlags.bSame)//H+1=L,也只追高位
			gFlags.bHL=0;
		else
			gFlags.bHL=1;//否则高地位同时追步
	}
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=uartRxList[i+2];
	}	
	lcyHashOnce();
	for(step=0;step<10000;step++)
	{
		CLRWDT();
		for(i=0;i<8;i++)
		{
			lcyHashIn[i]=lcyHashOut[i];
		}	
		lcyHashOnce();
		bSameHash(lcyHashOut,keyH);//先追高
		if(gFlags.bSame)//如果相同，则成功
		{
			addr=uartRxList[1]<<4;
			eep=eeprom_read(0xf0+uartRxList[1]);
			if(eep!=0xff)//追步后刷新高地位
			{
				for(i=0;i<8;i++)
				{
					lcyHashIn[i]=uartRxList[i+2];
				}	
				lcyHashOnce();
				if(eep==0)//后8为高位hash
				{				
					eeprom_write(addr+i+8,lcyHashOut[i]);				
				}
				else
				{
					eeprom_write(addr+i,lcyHashOut[i]);
				}
				for(i=0;i<8;i++)
				{
					lcyHashIn[i]=lcyHashOut[i];
				}
				lcyHashOnce();
				if(eep==0)//后8为高位hash
				{				
					eeprom_write(addr+i,lcyHashOut[i]);				
				}
				else
				{
					eeprom_write(addr+i+8,lcyHashOut[i]);
				}
			}
			UpdateEEPROM();
			return;
		}
		if(gFlags.bHL)//同时追步,则再追低位
		{
			bSameHash(lcyHashOut,keyL);
			if(gFlags.bSame)//如果相同，则成功
			{
				addr=uartRxList[1]<<4;
				eep=eeprom_read(0xf0+uartRxList[1]);
				if(eep!=0xff)//追步后刷新高地位
				{
					for(i=0;i<8;i++)
					{
						lcyHashIn[i]=uartRxList[i+2];
					}	
					lcyHashOnce();
					if(eep==0)//后8为高位hash
					{				
						eeprom_write(addr+i+8,lcyHashOut[i]);				
					}
					else
					{
						eeprom_write(addr+i,lcyHashOut[i]);
					}
					for(i=0;i<8;i++)
					{
						lcyHashIn[i]=lcyHashOut[i];
					}
					lcyHashOnce();
					if(eep==0)//后8为高位hash
					{				
						eeprom_write(addr+i,lcyHashOut[i]);				
					}
					else
					{
						eeprom_write(addr+i+8,lcyHashOut[i]);
					}
				}
				UpdateEEPROM();
				return;
			}
		}
	}
	uartTxList[0]=0x93;
	for(i=0;i<9;i++)
	{
		uartTxList[i]=0x00;
	}
}
//read
void ProcCommand_read_0x76(void)
{
	unsigned char i,addr,h,l,eep;
	uartTxList[0]=0x78;
	uartTxList[1]=uartRxList[2];	
	addr=0xff;
	for(i=0;i<10;i++)
	{
		if(rxHeader[0][i]==uartRxList[2])
		{
			addr=rxHeader[1][i];
			break;
		}
	}
	if(addr!=0xff)
	{
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=eeprom_read(addr+i);
		}
	}
	else if(uartRxList[2]<0x90)//钥匙需要特殊处理
	{
		l=LOW_NIBBLE(uartRxList[2]);
		h=HIGH_NIBBLE(uartRxList[2]);
		if(h==0x08)//dealer
		{
			addr=uartRxList[2]&0xf0;
			for(i=0;i<8;i++)
			{
				uartTxList[i+2]=eeprom_read(addr+i);
			}
		}
		else//key
		{
			eep=eeprom_read(0xf0+h);
			if(eep==0xff)//未使用过
			{
				addr=uartRxList[2]&0xf0;
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=eeprom_read(addr+i);
				}
			}
			else if(l==0)
			{
				addr=uartRxList[2];
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=eeprom_read(addr+i);
				}
			}
			else if(l==1)
			{
				addr=h<<4;
				addr=addr+0x08;
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=eeprom_read(addr+i);
				}
			}
			else if(l==2)
			{			
				addr=h<<4;
				if(eep==0x01)//后8位为低位
					addr=addr+0x08;
				for(i=0;i<8;i++)
				{
					lcyHashIn[i]=eeprom_read(addr+i);
				}	
				lcyHashOnce();
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=lcyHashOut[i];
				}
			}
			else if(l==3)
			{
				addr=h<<4;
				if(eep==0x01)//后8位为低位
					addr=addr+0x08;
				for(i=0;i<8;i++)
				{
					lcyHashIn[i]=eeprom_read(addr+i);
				}	
				lcyHashCalc(2);
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=lcyHashOut[i];
				}
			}
			else
			{
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=0;
				}
			}
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=0;
		}
	}	
}
//renew
void ProcCommand_renew_0x52(void)
{
	unsigned char i,eep;
	uartTxList[0]=0x77;
	uartTxList[1]=0x03;
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=uartRxList[i+2];
	}	
	lcyHashCalc(101);
	bSameHash_eep(lcyHashOut,0x80);	
	if(gFlags.bSame)//成功
	{
		uartTxList[2]=0x79;
		uartTxList[3]=0x52;
		eep=eeprom_read(0x90);
		if(LOW_NIBBLE(eep)==0x02)//数据为空
			uartTxList[4]=0xff;
		else
			uartTxList[4]=0x00;
		uartTxList[5]=0x00;
		uartTxList[6]=0x00;
		uartTxList[7]=0x00;
		uartTxList[8]=0x00;
		uartTxList[9]=0x00;
		eeprom_write(0x90,0x22);
		eeprom_write(0x91,0xff);	
		for(i=0;i<8;i++)//前8为高位
		{
			eeprom_write(0xf0+i,0xff);	
		}
		eeprom_write(0xff,0xff);
	}
	else//失败
	{

	}
}
void ProcCommand_lock_0x42(void)
{
	unsigned char i,eep,addr;
	eep=eeprom_read(0x90);
	if(eep==0x22)//刚刚renew 
	{
		uartTxList[0]=0x88;
		for(i=0;i<9;i++)
		{
			uartTxList[i]=0x00;
		}
	}
	// 	eep=eeprom_read(0xff);
	// 	if(eep==0xff)
	// 		return;
	addr=uartRxList[1]<<4;
	eep=eeprom_read(0xf0+uartRxList[1]);
	if(eep==0)//后8为高位hash
		addr+=8;
	bSameHash_eep(&uartRxList[2],addr);
	if(gFlags.bSame)
	{
		for(i=0;i<8;i++)
		{
			uartTxList[2+i]=0x00;
		}
		uartTxList[0]=0x80;
		uartTxList[1]=uartRxList[1];
	}
	eep=eeprom_read(0x90);
	eep=eep&0x0f;
	eep=eep|0x20;
	eeprom_write(0x90,eep);
}
//暂不做判断
void ProcCommand_write_0x15(void)
{
	unsigned char i;
	uartTxList[0]=0x8a;
	uartTxList[1]=0x00;
	for(i=0;i<8;i++)
	{
		uartTxList[i+2]=0;
	}
}
//写key & dealer
void ProcCommand_write_0x96(void)
{
	unsigned char i,addr;
	uartTxList[0]=0x8b;
	uartTxList[1]=uartRxList[1];
	if(uartRxList[1]<0x09)
	{
		addr=uartRxList[1]<<4;
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i,uartRxList[2+i]);
			eeprom_write(addr+i+8,uartRxList[2+i]);
		}
		
	}
	else if(uartRxList[1]==0x09)//psw
	{
		addr=0xd0;
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i,uartRxList[2+i]);
			eeprom_write(addr+i+8,uartRxList[2+i]);
		}
	}
	else if(uartRxList[1]==0x0a)//校验和
	{
		eeprom_write(0x90,0x24);//强行更改状态，忽略a0 01 指令
		eeprom_write(0x91,0x00);
	}
// 	else
// 	{
// 
// 	}
	//滚步返回
	for(i=0;i<8;i++)
	{
		lcyHashIn[i]=uartRxList[2+i];
	}
	lcyHashOnce();
	for(i=0;i<8;i++)
	{
		uartTxList[i+2]=lcyHashOut[i];
	}
}
//此指令暂且忽略
void ProcCommand_write_0xa0(void)
{
	unsigned char i;
	uartTxList[0]=0xa1;
	uartTxList[1]=0xaa;		
	for(i=0;i<8;i++)
	{
		uartTxList[i+2]=uartRxList[i+2];
	}
}
void ProcCommand_write_0x9a(void)
{
	unsigned char i,addr;
	uartTxList[0]=0x98;
	uartTxList[1]=0x00;
	addr=0xa8;
	if(uartRxList[1]!=0)
		addr+=8;
	//强制字符串结尾
	uartRxList[9]=0x00;
	for(i=0;i<8;i++)
	{
		eeprom_write(addr+i,uartRxList[2+i]);
	}
	for(i=0;i<8;i++)
	{
		uartTxList[i+2]=0;
	}
}
// void ProcCommand52(void)
// {
// 
// }
// void ProcCommand52(void)
// {
// 
// }
// void ProcCommand52(void)
// {
// 
// }
/*
void ProcCommand( void )
{
	unsigned char sum,addr,h,l;
	unsigned char i;
	sum=GetVerify_Sum(uartRxList,10);
	if(sum!=uartRxList[10])
	{
		return;
	}
	if(uartRxList[0]==0x76)//读数据	
	{
		uartTxList[0]=0x78;
		uartTxList[1]=uartRxList[2];	
		addr=0xff;
		for(i=0;i<10;i++)
		{
			if(rxHeader[0][i]==uartRxList[2])
			{
				addr=rxHeader[1][i];
				break;
			}
		}
		if(addr!=0xff)
		{
			for(i=0;i<8;i++)
			{
				uartTxList[i+2]=eeprom_read(addr+i);
			}
		}
		else if(uartRxList[2]<0x90)//钥匙需要特殊处理
		{
			l=LOW_NIBBLE(uartRxList[2]);
			h=HIGH_NIBBLE(uartRxList[2]);
			if(l==0)
			{
				addr=uartRxList[2];
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=eeprom_read(addr+i);
				}
			}
			else if(l==1)
			{
				addr=h<<4;
				addr=addr+0x08;
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=eeprom_read(addr+i);
				}
			}
			else if(l==2)
			{
				addr=h<<4;
				addr=addr+0x08;
				for(i=0;i<8;i++)
				{
					lcyHashIn[i]=eeprom_read(addr+i);
				}	
				lcyHashOnce();
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=lcyHashOut[i];
				}
			}
			else if(l==3)
			{
				addr=h<<4;
				addr=addr+0x08;
				for(i=0;i<8;i++)
				{
					lcyHashIn[i]=eeprom_read(addr+i);
				}	
				lcyHashCalc(2);
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=lcyHashOut[i];
				}
			}
			else
			{
				for(i=0;i<8;i++)
				{
					uartTxList[i+2]=0;
				}
			}
		}
		else
		{
			for(i=0;i<8;i++)
			{
				uartTxList[i+2]=0;
			}
		}		
	}
	else if(uartRxList[0]==0x52)//renew
	{
		uartTxList[0]=0x77;
		uartTxList[1]=0x03;
		for(i=0;i<8;i++)
		{
			lcyHashIn[i]=uartRxList[i+2];
		}	
		lcyHashCalc(101);
		sum=0;
		for(i=0;i<8;i++)
		{
			addr=eeprom_read(0x80+i);
			if(lcyHashOut[i]!=addr)
			{
				sum=1;
				break;
			}
		}
		if(sum==0)//成功
		{

		}
		else//失败
		{

		}
	}
	else if(uartRxList[0]==0x15)//写启动
	{
		uartTxList[0]=0x8a;
		uartTxList[1]=0x00;
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=0;
		}
	}
	else if(uartRxList[0]==0x96)
	{
		uartTxList[0]=0x8b;
		uartTxList[1]=uartRxList[1];
		if(uartRxList[1]<0x09)
		{
			addr=uartRxList[1]<<4;
			for(i=0;i<8;i++)
			{
				eeprom_write(addr+i,uartRxList[2+i]);
			}
		}
		else if(uartRxList[1]==0x09)//psw
		{
			addr=0xd0;
			for(i=0;i<8;i++)
			{
				eeprom_write(addr+i,uartRxList[2+i]);
			}
		}
		else if(uartRxList[1]==0x0a)//校验和
		{

		}
		else
		{

		}
		for(i=0;i<8;i++)
		{
			lcyHashIn[i]=uartRxList[2+i];
		}
		lcyHashOnce();
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=lcyHashOut[i];
		}
	}
	else if(uartRxList[0]==0xa0)
	{
		uartTxList[0]=0xa1;
		uartTxList[1]=0xaa;		
// 		for(i=0;i<8;i++)
// 		{
// 			lcyHashIn[i]=uartRxList[2+i];
// 		}
// 		lcyHashOnce();
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=uartRxList[i+2];
		}
	}
	else if(uartRxList[0]==0x9a)
	{
		uartTxList[0]=0x98;
		uartTxList[1]=0x00;
		addr=0xa8;
		if(uartRxList[1]!=0)
			addr+=8;
		for(i=0;i<8;i++)
		{
			eeprom_write(addr+i,uartRxList[2+i]);
		}
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=0;
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			uartTxList[i+2]=0;
		}
	}
	uartTxList[10]=GetVerify_Sum(uartTxList,10);
	Uart1_PutChars(uartTxList,11);
}
*/