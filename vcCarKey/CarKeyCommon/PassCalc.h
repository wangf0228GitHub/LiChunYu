#pragma once

#include "InteractionData.h"
#include <afxcmn.h>

class CPassCalc 
{
public:
	CPassCalc();
	virtual ~CPassCalc();	
	unsigned char HashIn[0x08];
	unsigned char HashOut[0x08];
	unsigned char Ht[0x10];
	unsigned char InTP[0x08];
	bool F_calsel;//false is fisrt,true is second;	
	void sub_CALrenew(unsigned char *InP);
	void sub_HashOnce();
	void sub_HashCalc(int n);
	void sub_half();
	void sub_add_4B(unsigned char *p);
	void sub_Shift_L(unsigned char N);
	void sub_Shift_R(unsigned char N);
	void sub_Mixed_A();
	void sub_Mixed_B();
	void loadTab();
	void sub_cal0();
	void sub_cal1(unsigned char s);
	void sub_cal2();
	void sub_cal3();
	void sub_cal_L_A(unsigned char x);
	void sub_cal_L_B(unsigned char x);
	void sub_cal_R_A(unsigned char x);
	void sub_cal_R_B(unsigned char x);
	void sub_add(unsigned char *p,unsigned char*s);
};


