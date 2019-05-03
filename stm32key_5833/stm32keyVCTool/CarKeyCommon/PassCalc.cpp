// HIDDevice.cpp : 实现文件
//

#include "stdafx.h"
#include "PassCalc.h"
#include "VMProtectSDK.h"


CPassCalc::CPassCalc()
{
	F_calsel=false;
}

CPassCalc::~CPassCalc()
{
}

void CPassCalc::sub_CALrenew( unsigned char *InP )
{
	VMProtectBegin("sub_CALrenew1");
	unsigned char Tp[0x08];
	unsigned int i;
	//--
	Tp[0]=InP[0x0C];
	Tp[1]=InP[0x0D];
	Tp[2]=InP[0x0E];
	Tp[3]=InP[0x0F];
	Tp[4]=InP[0x08];
	Tp[5]=InP[0x09];
	Tp[6]=InP[0x0A];
	Tp[7]=InP[0x0B];
	//
	HashIn[0]=InP[0x10];//RN2_T[0];
	HashIn[1]=InP[0x11];//RN2_T[1];
	HashIn[2]=InP[0x17];//RN3_T[0];
	HashIn[3]=InP[0x18];//RN3_T[1];
	HashIn[4]=InP[0x19];//RN3_T[2];
	HashIn[5]=InP[0x1A];//RN3_T[3];
	HashIn[6]=InP[0x1B];//RN3_T[4];
	HashIn[7]=InP[0x1C];//RN3_T[5];
	//--------
	//-->
	for(i=0;i<8;i++)
	{
		HashIn[i]=HashIn[i]^Tp[i];
	}
	sub_HashOnce();
	for(i=0;i<8;i++)
	{
		HashOut[i]=HashOut[i]^InP[i];
	}
	VMProtectEnd();
}

void CPassCalc::sub_HashOnce()
{
	//VMProtectBegin("sub_CALrenew2");
	unsigned char i;
	unsigned char Sa[0x08];
	unsigned char Sb[0x08];
	//--
	F_calsel=false;
	sub_half();
	for(i=0;i<8;i++)
	{
		Sa[i]=Ht[i];
	}
	//--
	F_calsel=true;
	sub_half();
	for(i=0;i<8;i++)
	{
		Sb[i]=Ht[i+0x04];
	}
	//--
	sub_add(Sa,Sb);
	//VMProtectEnd();
}
void CPassCalc::sub_add(unsigned char *p,unsigned char*s)
{
	VMProtectBegin("sub_CALrenew3");
	unsigned char i,C;
	unsigned int Sum;
	const unsigned char B[0x08]={0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98};
	//--
	C=0;
	for(i=0;i<0x04;i++)
	{
		Sum=p[0x03-i]+s[0x03-i]+C;
		HashOut[0x00+i]=p[0x03-i]+s[0x03-i]+C;
		if(Sum>0xFF)
		{
			C=0x01;
		}
		else
		{
			C=0x00;
		}
	}
	//
	C=0;
	for(i=0;i<0x04;i++)
	{
		Sum=HashOut[0x00+i]+B[i]+C;
		HashOut[0x00+i]=HashOut[0x00+i]+B[i]+C;
		if(Sum>0xFF)
		{
			C=0x01;
		}
		else
		{
			C=0x00;
		}
	}
	//--
	C=0;
	for(i=0;i<0x04;i++)
	{
		Sum=p[0x07-i]+s[0x07-i]+C;
		HashOut[0x04+i]=p[0x07-i]+s[0x07-i]+C;
		if(Sum>0xFF)
		{
			C=0x01;
		}
		else
		{
			C=0x00;
		}
	}
	//
	C=0;
	for(i=0;i<0x04;i++)
	{
		Sum=HashOut[0x04+i]+B[0x04+i]+C;
		HashOut[0x04+i]=HashOut[0x04+i]+B[0x04+i]+C;
		if(Sum>0xFF)
		{
			C=0x01;
		}
		else
		{
			C=0x00;
		}
	}
	VMProtectEnd();
	//--
}
//----------------------------------------------------------------
//----------------------------------------------------------------
//以下放入加密狗
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
void CPassCalc::sub_add_4B(unsigned char *p)
{
	VMProtectBegin("sub_CALrenew4");
	unsigned char i,C;
	unsigned int Sum;
	C=0;
	for(i=0;i<0x04;i++)
	{
		Sum=Ht[0x0B-i]+p[0x03-i]+C;
		Ht[0x0B-i]=Ht[0x0B-i]+p[0x03-i]+C;
		if(Sum>0xFF)
		{
			C=0x01;
		}
		else
		{
			C=0x00;
		}
	}
	VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_Shift_L(unsigned char N)//N is cnt
{
	VMProtectBegin("sub_CALrenew5");
	unsigned char C,Ct,i,j;
	for(i=0;i<N;i++)
	{
		C=Ht[0x08]&0x80;
		for(j=0;j<0x04;j++)
		{
			Ct=Ht[0x0B-j]&0x80;
			Ht[0x0B-j]=Ht[0x0B-j]<<1;
			if(0x00==C)
			{
				Ht[0x0B-j]=Ht[0x0B-j]&0xFE;
			}
			else
			{
				Ht[0x0B-j]=Ht[0x0B-j]|0x01;
			}
			C=Ct;
		}
	}
	VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_Shift_R(unsigned char N)//N is cnt
{
	VMProtectBegin("sub_CALrenew6");
	unsigned char C,Ct,i,j;
	for(i=0;i<N;i++)
	{
		C=Ht[0x0B]&0x01;
		for(j=0;j<0x04;j++)
		{
			Ct=Ht[0x08+j]&0x01;
			Ht[0x08+j]=Ht[0x08+j]>>1;
			if(0x00==C)
			{
				Ht[0x08+j]=Ht[0x08+j]&0x7F;
			}
			else
			{
				Ht[0x08+j]=Ht[0x08+j]|0x80;
			}
			C=Ct;
		}
	}
	VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_Mixed_A()//
{
	VMProtectBegin("sub_CALrenew7");
	//                              +0   +1   +2   +3   +4   +5   +6   +7   +8   +9   +A   +B   +C   +D   +E   +F
	const unsigned char Ta[0x10]={0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0F,0x0C,0x0D,0x0E,0x00,0x01,0x02,0x03};
	unsigned char tmp[0x10];
	unsigned char i,j;

	for(i=0;i<0x10;i++)
	{
		j=Ta[i];
		tmp[j]=Ht[i];
	}
	//
	for(i=0;i<0x10;i++)
	{
		Ht[i]=tmp[i];
	}
	VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_Mixed_B()//
{
	VMProtectBegin("sub_CALrenew8");
	//                              +0   +1   +2   +3   +4   +5   +6   +7   +8   +9   +A   +B   +C   +D   +E   +F
	const unsigned char Tb[0x10]={0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0E,0x0F,0x0C,0x0D,0x00,0x01,0x02,0x03};
	unsigned char tmp[0x10];
	unsigned char i,j;

	for(i=0;i<0x10;i++)
	{
		j=Tb[i];
		tmp[j]=Ht[i];
	}
	//
	for(i=0;i<0x10;i++)
	{
		Ht[i]=tmp[i];
	}
	VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::loadTab()
{
	VMProtectBegin("sub_CALrenew9");
	const unsigned char TAB0[0x10]={
		0x45,0x03,0x11,0x4F,0x85,0x4C,0x7A,0x31,
		0xF6,0x7C,0xCB,0xB8,0x15,0xA4,0xD0,0x42};
		const unsigned char TAB1[0x10]={
			0x51,0x9B,0x49,0x14,0xB1,0x2E,0x8F,0xB5,
			0x95,0xDE,0x62,0x7A,0xD8,0x0D,0x44,0x1C};

			int i;
			for(i=0;i<0x10;i++)
			{
				if(false==F_calsel)
				{
					Ht[i]=TAB0[i];
				}
				else
				{
					Ht[i]=TAB1[i];
				}
			}
			VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal0()//0AFE
{
	VMProtectBegin("sub_CALrenew10");
	unsigned char i;
	unsigned char Biao_a[0x04]={0x50,0xA2,0x8B,0xE6};
	unsigned char TP[0x04];
	for(i=0;i<0x04;i++)
	{
		TP[i]=((Ht[0x00+i]^Ht[0x04+i])&Ht[0x0C+i])^Ht[0x04+i];
	}
	//--
	VMProtectEnd();
	sub_add_4B(TP);
	if(true==F_calsel)
	{
		sub_add_4B(Biao_a);
	}
	
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal1(unsigned char s)//0CDF
{
	VMProtectBegin("sub_CALrenew11");
	unsigned char i;
	unsigned char T[0x04];
	s=s&0x04;
	for(i=0;i<0x04;i++)
	{
		T[0x03-i]=HashIn[i+s];
	}
	VMProtectEnd();
	sub_add_4B(T);
	
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal2()//0B4B
{
	VMProtectBegin("sub_CALrenew12");
	unsigned char i;
	unsigned char Biao_b[0x04]={0x5A,0x82,0x79,0x99};
	unsigned char TP[0x04];
	unsigned char m,n;

	for(i=0;i<0x04;i++)
	{
		m=Ht[0x00+i]&Ht[0x04+i];
		n=(Ht[0x00+i]|Ht[0x04+i])&Ht[0x0C+i];
		TP[i]=m|n;
	}
	VMProtectEnd();
	//--
	sub_add_4B(TP);
	if(false==F_calsel)
	{
		sub_add_4B(Biao_b);
	}
	
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal3()//0BD9
{
	VMProtectBegin("sub_CALrenew13");
	unsigned char i;
	unsigned char Biao_c[0x04]={0x6E,0xD9,0xEB,0xA1};
	unsigned char Biao_d[0x04]={0x5C,0x4D,0xD1,0x24};
	unsigned char TP[0x04];

	for(i=0;i<0x04;i++)
	{
		TP[i]=Ht[0x00+i]^Ht[0x04+i]^Ht[0x0C+i];
	}
	VMProtectEnd();
	//--
	sub_add_4B(TP);
	if(false==F_calsel)
	{
		sub_add_4B(Biao_c);
	}
	else
	{
		sub_add_4B(Biao_d);
	}
	
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal_L_A(unsigned char x)//
{
	//VMProtectBegin("sub_CALrenew14");
	sub_Shift_L(x);
	sub_Mixed_A();
	//VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal_L_B(unsigned char x)//
{
	//VMProtectBegin("sub_CALrenew15");
	sub_Shift_L(x);
	sub_Mixed_B();
	//VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal_R_A(unsigned char x)//
{
	//VMProtectBegin("sub_CALrenew16");
	sub_Shift_R(x);
	sub_Mixed_A();
	//VMProtectEnd();
}
/*---------------------------------------------------------------------*/
void CPassCalc::sub_cal_R_B(unsigned char x)//
{
	//VMProtectBegin("sub_CALrenew17");
	sub_Shift_R(x);
	sub_Mixed_B();
	//VMProtectEnd();
}
void CPassCalc::sub_half()
{
	//VMProtectBegin("sub_CALrenew18");
	unsigned char n,m;
	loadTab();
	sub_cal0();//0AFE
	n=0x00;
	sub_cal1(n);//0CDF
	m=1;
	sub_cal_L_A(m);//0A9C
	sub_cal0();//0AFE
	n=0x04;
	sub_cal1(n);//0CDF
	sub_Mixed_A();
	//00
	sub_cal2();//0B4B
	m=1;
	sub_cal_R_A(m);//0A95
	//01
	sub_cal2();//0B4B
	m=2;
	sub_cal_R_A(m);//
	//02
	sub_cal2();//0B4B
	sub_Mixed_A();
	//03
	sub_cal2();//0B4B
	m=3;
	sub_cal_R_B(m);
	//04
	sub_cal2();//0B4B
	m=3;
	sub_cal_L_A(m);
	//05
	sub_cal2();//0B4B
	m=1;
	sub_cal_L_A(m);
	//06
	sub_cal2();//0B4B
	n=0x04;
	sub_cal1(n);//0CDF
	m=1;
	sub_cal_R_A(m);//
	//07
	sub_cal2();//0B4B
	m=1;
	sub_cal_R_B(m);
	//08
	sub_cal2();//0B4B
	m=1;
	sub_cal_R_A(m);//
	//09
	sub_cal2();//0B4B
	m=4;
	sub_cal_L_A(m);
	//0A
	sub_cal2();//0B4B
	m=1;
	sub_cal_R_B(m);
	//0B
	sub_cal2();//0B4B
	m=1;
	sub_cal_L_A(m);
	//0C
	sub_cal2();//0B4B
	n=0x00;
	sub_cal1(n);//0CDF
	m=1;
	sub_cal_R_A(m);//
	//0D
	sub_cal2();//0B4B
	m=3;
	sub_cal_L_A(m);
	//0E
	sub_cal2();//0B4B
	m=3;
	sub_cal_R_B(m);//
	//0F
	sub_cal2();//0B4B
	m=4;
	sub_cal_L_A(m);
	//10
	sub_cal3();//0BD9
	m=3;
	sub_cal_L_A(m);
	//11
	sub_cal3();//0BD9
	m=3;
	sub_cal_R_B(m);
	//12
	sub_cal3();//0BD9
	m=2;
	sub_cal_R_B(m);
	//13
	sub_cal3();//0BD9
	m=1;
	sub_cal_R_A(m);
	//14
	sub_cal3();//0BD9
	m=2;
	sub_cal_R_B(m);
	//15
	sub_cal3();//0BD9
	n=0x04;
	sub_cal1(n);//0CDF
	m=1;
	sub_cal_L_A(m);
	//16
	sub_cal3();//0BD9
	m=3;
	sub_cal_R_B(m);
	//17
	sub_cal3();//0BD9
	m=1;
	sub_cal_R_B(m);
	//18
	sub_cal3();//0BD9
	n=0x00;
	sub_cal1(n);//0CDF
	m=2;
	sub_cal_R_A(m);
	//19
	sub_cal3();//0BD9
	sub_Mixed_A();
	//1A
	sub_cal3();//0BD9
	m=3;
	sub_cal_R_B(m);
	//1B
	sub_cal3();//0BD9
	m=2;
	sub_cal_R_A(m);
	//1C
	sub_cal3();//0BD9
	m=4;
	sub_cal_L_A(m);
	//1D
	sub_cal3();//0BD9
	m=3;
	sub_cal_R_A(m);
	//1E
	sub_cal3();//0BD9
	m=1;
	sub_cal_R_A(m);
	//1F
	sub_cal3();//0BD9
	m=3;
	sub_cal_R_A(m);
	//VMProtectEnd();
}

void CPassCalc::sub_HashCalc( int n )
{
	VMProtectBegin("sub_CALrenew19");
	for(int i=0;i<n;i++)
	{
		sub_HashOnce();
		for(int j=0;j<8;j++)
			HashIn[j]=HashOut[j];
	}
	VMProtectEnd();
}

// CPassCalc 消息处理程序


