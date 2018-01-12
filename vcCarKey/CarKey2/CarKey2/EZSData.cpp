#include "StdAfx.h"
#include "EZSData.h"
#include "CommFunc.h"
#include "CarKey2.h"


CEZSData::CEZSData(void)
{
	for(int i=0;i<0x400;i++)
		AllFile[i]=0x00;
}


CEZSData::~CEZSData(void)
{
}

CString CEZSData::GetSSID()
{
	return CCommFunc::byteToHexStr(AllFile, 0xbe, 4, _T(" "));
}

CString CEZSData::GetPSW()
{
	return CCommFunc::byteToHexStr(AllFile, 0x8e, 8, _T(" "));
}

CString CEZSData::GetDealer()
{
	return CCommFunc::byteToHexStr(AllFile, 0x019a, 8, _T(" "));
}

CString CEZSData::GetKey0()
{
	return CCommFunc::byteToHexStr(AllFile, 0x01ac, 8, _T(" "));
}
CString CEZSData::GetKey1()
{
	return CCommFunc::byteToHexStr(AllFile, 0x01be, 8, _T(" "));
}

CString CEZSData::GetKey2()
{
	return CCommFunc::byteToHexStr(AllFile, 0x01d0, 8, _T(" "));
}

CString CEZSData::GetKey3()
{
	return CCommFunc::byteToHexStr(AllFile, 0x01e2, 8, _T(" "));
}

CString CEZSData::GetKey4()
{
	return CCommFunc::byteToHexStr(AllFile, 0x01f4, 8, _T(" "));
}

CString CEZSData::GetKey5()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0206, 8, _T(" "));
}

CString CEZSData::GetKey6()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0218, 8, _T(" "));
}

CString CEZSData::GetKey7()
{
	return CCommFunc::byteToHexStr(AllFile, 0x022a, 8, _T(" "));
}

CString CEZSData::GetTransport()
{
	return CCommFunc::byteToHexStr(AllFile, 0xa6, 8, _T(" "));
}

CString CEZSData::GetSerialNum()
{
	return CCommFunc::byteToHexStr(AllFile, 0xb6, 8, _T(" "));
}

CString CEZSData::GetKeyStatus(int index,COLORREF* txtColor)
{
//  	byte KeyStatus1, KeyStatus2;
//  	KeyStatus1 = AllFile[4];//pBuf[4];
//  	KeyStatus2 = AllFile[6];//pBuf[6];
 	enum KeyStatus ret;
 	switch(index)
 	{
 	case 0:
 		if (CCommFunc::GetBit(KeyStatus1, 7))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 7))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 1:
 		if (CCommFunc::GetBit(KeyStatus1, 6))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 6))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 2:
 		if (CCommFunc::GetBit(KeyStatus1, 5))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 5))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 3:
 		if (CCommFunc::GetBit(KeyStatus1, 4))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 4))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 4:
 		if (CCommFunc::GetBit(KeyStatus1, 3))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 3))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 5:
 		if (CCommFunc::GetBit(KeyStatus1, 2))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 2))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 6:
 		if (CCommFunc::GetBit(KeyStatus1, 1))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 1))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	case 7:
 		if (CCommFunc::GetBit(KeyStatus1, 0))
 			ret = KeyStatus_Lock;
 		else
 		{
 			if (CCommFunc::GetBit(KeyStatus2, 0))
 				ret = KeyStatus_NoUsed;
 			else
 				ret = KeyStatus_Used;
 		}
 		break;
 	}	
	CString str;
 	switch(ret)
 	{
 	case KeyStatus_Lock:
 		*txtColor=RGB(0xdc,0x14,0x3c);
		str.LoadString(IDS_KeyStatus_Lock);
		break;
 	case KeyStatus_NoUsed:
		*txtColor=RGB(0,255,0);
		str.LoadString(IDS_KeyStatus_NoUsed);
		break;
 	case KeyStatus_Used:
		*txtColor=RGB(0xff,0xd7,0);
		str.LoadString(IDS_KeyStatus_Used);
		break;
	default:
		str=_T("");
		break;
 	}
	return str;
}

byte CEZSData::GetStatus()
{
	return AllFile[0x80];
}

CString CEZSData::GetLastKey()
{
	CString str;
	byte b=AllFile[0xd8]&0x0f;
	if(b>7)
		return _T("0");
	else
		str.Format(_T("%d"),AllFile[0xd8]&0x07);
	return str;
}
CString CEZSData::GetPreviousKey()
{
	CString str;
	byte b=AllFile[0xda]&0x0f;
	if(b>7)
		return _T("0");
	else
		str.Format(_T("%d"),AllFile[0xda]&0x07);
	return str;
}

bool CEZSData::SetSSID(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=4)
		return false;
	for(int i=0;i<4;i++)
		AllFile[0xbe+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetPSW(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x8e+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetDealer(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x019a+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey0(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x01ac+i]=p[i];
	delete[] p;
	return true;
}
bool CEZSData::SetKey1(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x01be+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey2(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x01d0+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey3(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x01e2+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey4(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x01f4+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey5(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x0206+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey6(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x0218+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKey7(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0x022a+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetTransport(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	bool bProtec=true;
	for(int i=0;i<8;i++)
	{
		if(p[i]!=0x00)
		{
			bProtec=false;
			break;
		}
	}
	if(bProtec)
	{
		//C2 E9 01 F0 ED BA 7C 5C
		AllFile[0x0A6 + 0]=0xC2;
		AllFile[0x0A6 + 1]=0xE9;
		AllFile[0x0A6 + 2]=0x01;
		AllFile[0x0A6 + 3]=0xF0;
		AllFile[0x0A6 + 4]=0xED;
		AllFile[0x0A6 + 5]=0xBA;
		AllFile[0x0A6 + 6]=0x7C;
		AllFile[0x0A6 + 7]=0x5C;
	}
	else
	{
		for(int i=0;i<8;i++)
			AllFile[0xa6+i]=p[i];
	}	
	delete[] p;
	return true;
}

bool CEZSData::SetSerialNum(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=8)
		return false;
	for(int i=0;i<8;i++)
		AllFile[0xb6+i]=p[i];
	delete[] p;
	return true;
}

bool CEZSData::SetKeyStatus(byte s1,byte s2)
{
	KeyStatus1=s1;//pBuf[4];
	KeyStatus2=s2;//pBuf[6];
	return true;
}

bool CEZSData::SetStatus(CString str)
{
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		return false;
	if(len!=1)
		return false;
	for(int i=0;i<1;i++)
		AllFile[0x80+i]=p[i];
	delete[] p;
	return true;
}

void CEZSData::SetStatus(byte b)
{
	AllFile[0x80]=b;
}

bool CEZSData::SetLastKey(CString str)
{
	int len;
	len=CCommFunc::GetCStringLength(str);
	if(len==1)
	{
		//ЮЊПе
		AllFile[0xd8]=0x0e;
		return true;
	}
	if(len==2)
	{
		char* pString=CCommFunc::CStringToChar(str,&len);
		byte c=CCommFunc::ASCII2Byte(pString[0]);
		delete[] pString;
		if(c!=0xff)
		{
			AllFile[0xd8]=c;
			return true;
		}
		else
			return false;
	}
	return false;
}
bool CEZSData::SetPreviousKey(CString str)
{
	int len;
	len=CCommFunc::GetCStringLength(str);
	if(len==1)
	{
		//ЮЊПе
		AllFile[0xd8]=0x0e;
		return true;
	}
	if(len==2)
	{
		char* pString=CCommFunc::CStringToChar(str,&len);
		byte c=CCommFunc::ASCII2Byte(pString[0]);
		delete[] pString;
		if(c!=0xff)
		{
			AllFile[0xda]=c;
			return true;
		}
		else
			return false;
	}
	return false;
}

void CEZSData::SetAllFile(byte* pBuf)
{

}

void CEZSData::SetEISWriteBuf()
{
	//key0
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[i]=AllFile[0x01ac+i];
	//key1
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x08+i]=AllFile[0x01be+i];
	//key2
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x10+i]=AllFile[0x01d0+i];
	//key3
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x18+i]=AllFile[0x01e2+i];
	//key4
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x20+i]=AllFile[0x01f4+i];
	//key5
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x28+i]=AllFile[0x0206+i];
	//key6
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x30+i]=AllFile[0x0218+i];
	//key7
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x38+i]=AllFile[0x022a+i];
	//Dealer
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x40+i]=AllFile[0x019a+i];
	//PSW
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x48+i]=AllFile[0x008e+i];
	//SSID
	for(int i=0;i<4;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x58+i]=AllFile[0x0be+i];
	//00-07
	for(int i=0;i<8;i++)
		CCarKey2App::TheHIDDevice.DTwr[0x50+i]=i;
	CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x04] = 0x00;
	//	pBuf2[0x58+0x04]=DTwr[0x58+0x04];
	CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x05] = 0x00;
	//	pBuf2[0x58+0x05]=DTwr[0x58+0x05];
	CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x06] = 0x00;
	//	pBuf2[0x58+0x06]=DTwr[0x58+0x06];
	CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x07] = 0x1c;
	//	pBuf2[0x58+0x07]=DTwr[0x58+0x07];  
}

void CEZSData::SetVerify()
{
	//
	int BaseAddress = 0x100;
	for (int i = 0; i < 10; i++)
	{
		AllFile[BaseAddress] = 0x07;
		BaseAddress += 0x12;
	}
	//
	BaseAddress = 0x1A2;
	for (int i = 0; i < 10; i++)
	{
		AllFile[BaseAddress] = 0x0F;
		BaseAddress += 0x12;
	}
	//
	BaseAddress = 0x244;
	for (int i = 0; i < 6; i++)
	{
		AllFile[BaseAddress] = 0x03;
		BaseAddress += 0x12;
	}
	//
	BaseAddress = 0x2B0;
	for (int i = 0; i < 11; i++)
	{
		AllFile[BaseAddress] = 0x03;
		BaseAddress += 0x12;
	}
	//
	BaseAddress = 0x104;
	for (int i = 0; i < 18; i++)
	{
		AllFile[BaseAddress] = 0x00;
		BaseAddress += 0x12;
	}
	//
	BaseAddress = 0x108;
	for (int i = 0; i < 18; i++)
	{
		AllFile[BaseAddress] = 0x3F;
		BaseAddress += 0x12;
	}
	//
	for (int i = 0; i < (0x3EF - 0x376); i++)
	{
		AllFile[0x376 + i] = 0x00;
	}
	AllFile[0x001] = 0x5A;
	AllFile[0x0C2] = 0xAA;
	AllFile[0x0C3] = 0xFF;
	AllFile[0x0C5] = 0xAA;
	AllFile[0x3EE] = 0x03;
	
	//KEY0,...7,dealer
	BaseAddress = 0x10A;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x01ac+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01ac,8);
	//key1
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x01be+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01be,8);
	//key2
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x01d0+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01d0,8);
	//key3
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x01e2+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01e2,8);
	//key4
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x01f4+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01f4,8);
	//key5
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x0206+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0206,8);
	//key6
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x0218+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0218,8);
	//key7
	BaseAddress += 0x12;
	for (int i = 0; i < 8; i++)
	{
		AllFile[BaseAddress + i] = AllFile[0x022a+i];
	}
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x022a,8);
	
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x019a,8);

	//key0
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01ac,8);
	//key1
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01be,8);
	//key2
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01d0,8);
	//key3
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01e2,8);
	//key4
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x01f4,8);
	//key5
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0206,8);
	//key6
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0218,8);
	//key7
	BaseAddress += 0x12;
	AllFile[BaseAddress - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x022a,8);
	//
	for (int i = 0; i < 0x10; i++)
	{
		AllFile[0x0C6 + i] = 0x00;
	}
}

void CEZSData::Clear()
{
	byte s=AllFile[0x80];
	for(int i=0;i<0x400;i++)
		AllFile[i]=0x00;
	AllFile[0x80]=s;
}

