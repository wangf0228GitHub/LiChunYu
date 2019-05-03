#include "StdAfx.h"
#ifdef CarKey2
#include ".\..\CarKey2\CarKey2\resource.h"
#else
#include ".\..\CarKey3\CarKey3\resource.h"
#endif
#include "EZSData.h"
#include "CommFunc.h"

CPortalOP::CPortalOP()
{
	bKLine=false;
	bIR=true;
	bCAN=true;

	bOBD=false;
	bDirect=true;

	strWork[0]=_T("EZS");
	strWork[1]=_T("");
	strWork[2]=_T("");

	bELV=false;
	IROp=IROptions_NEC;
}

CPortalOP::~CPortalOP()
{

}

void CPortalOP::Reset()
{
	bKLine=false;
	bIR=true;
	bCAN=true;

	bOBD=false;
	bDirect=true;

	strWork[0]=_T("EZS");
	strWork[1]=_T("");
	strWork[2]=_T("");

	bELV=false;
	IROp=IROptions_NEC;
}


CEZSData::CEZSData(void)
{
	for(int i=0;i<0x400;i++)
		AllFile[i]=0x00;
	for(int i=0;i<8;i++)
		PSW[i]=i+1;
}


CEZSData::~CEZSData(void)
{
}

CString CEZSData::GetSSID()
{
	return CCommFunc::byteToHexStr(AllFile, 0x00, 4, _T(" "));
}

// CString CEZSData::GetPSW()
// {
// 	return CCommFunc::byteToHexStr(AllFile, 0x8e, 8, _T(" "));
// }

CString CEZSData::GetDealer()
{
	return CCommFunc::byteToHexStr(AllFile, 0x10, 8, _T(" "));
}

CString CEZSData::GetKey0()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0270, 8, _T(" "));
}
CString CEZSData::GetKey1()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0280, 8, _T(" "));
}

CString CEZSData::GetKey2()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0290, 8, _T(" "));
}

CString CEZSData::GetKey3()
{
	return CCommFunc::byteToHexStr(AllFile, 0x02a0, 8, _T(" "));
}

CString CEZSData::GetKey4()
{
	return CCommFunc::byteToHexStr(AllFile, 0x02b0, 8, _T(" "));
}

CString CEZSData::GetKey5()
{
	return CCommFunc::byteToHexStr(AllFile, 0x02c0, 8, _T(" "));
}

CString CEZSData::GetKey6()
{
	return CCommFunc::byteToHexStr(AllFile, 0x02d0, 8, _T(" "));
}

CString CEZSData::GetKey7()
{
	return CCommFunc::byteToHexStr(AllFile, 0x02e0, 8, _T(" "));
}

CString CEZSData::GetTransport()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0260, 8, _T(" "));
}

CString CEZSData::GetSerialNum()
{
	return CCommFunc::byteToHexStr(AllFile, 0x0230, 8, _T(" "));
}

CString CEZSData::GetKeyStatus(int index,COLORREF* txtColor)
{
//  	byte KeyStatus1, KeyStatus2;
//  	KeyStatus1 = AllFile[4];//pBuf[4];
//  	KeyStatus2 = AllFile[6];//pBuf[6];
// 	byte b1=KeyStatus2&0x0f;
// 	byte b2=KeyStatus2>>4;
// 	b2=b2&0x0f;
// 
// 	byte b=b1+b2;
// 	if(b>0x0f)
// 	{
// 		b1=0xff;
// 		b2=0xff;
// 	}
// 
//  	enum KeyStatus ret;
//  	switch(index)
//  	{
//  	case 0:
//  		if (CCommFunc::GetBit(KeyStatus1, 7))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 7))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 1:
//  		if (CCommFunc::GetBit(KeyStatus1, 6))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 6))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 2:
//  		if (CCommFunc::GetBit(KeyStatus1, 5))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 5))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 3:
//  		if (CCommFunc::GetBit(KeyStatus1, 4))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 4))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 4:
//  		if (CCommFunc::GetBit(KeyStatus1, 3))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 3))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 5:
//  		if (CCommFunc::GetBit(KeyStatus1, 2))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 2))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 6:
//  		if (CCommFunc::GetBit(KeyStatus1, 1))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 1))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	case 7:
//  		if (CCommFunc::GetBit(KeyStatus1, 0))
//  			ret = KeyStatus_Lock;
//  		else
//  		{
// 			if(b1==index || b2==index)
// 				ret = KeyStatus_Used;
// 			else
// 				ret = KeyStatus_NoUsed;
// //  			if (CCommFunc::GetBit(KeyStatus2, 0))
// //  				ret = KeyStatus_NoUsed;
// //  			else
// //  				ret = KeyStatus_Used;
//  		}
//  		break;
//  	}	
	enum KeyStatus ret;
	switch(index)
	{
	case 0:
		if (CCommFunc::GetBit(AllFile[4], 7))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;			
		break;
	case 1:
		if (CCommFunc::GetBit(AllFile[4], 6))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
		break;
	case 2:
		if (CCommFunc::GetBit(AllFile[4], 5))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
		break;
	case 3:
		if (CCommFunc::GetBit(AllFile[4], 4))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
		break;
	case 4:
		if (CCommFunc::GetBit(AllFile[4], 3))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
		break;
	case 5:
		if (CCommFunc::GetBit(AllFile[4], 2))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
		break;
	case 6:
		if (CCommFunc::GetBit(AllFile[4], 1))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
		break;
	case 7:
		if (CCommFunc::GetBit(AllFile[4], 0))
			ret = KeyStatus_Lock;
		else
			ret = KeyStatus_Used;
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
		*txtColor=RGB(0,255,0);//*txtColor=RGB(0xff,0xd7,0);
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
	return AllFile[5];
}

CString CEZSData::GetLastKey()
{
	CString str;
	byte b=AllFile[7]&0x0f;
	if(b>7)
		return _T("");
	else
		str.Format(_T("%d"),AllFile[7]&0x07);
	return str;
}
CString CEZSData::GetPreviousKey()
{
	CString str;
	byte b=AllFile[7]&0xf0;
	b=b>>4;
	if(b>7)
		return _T("");
	else
		str.Format(_T("%d"),b);
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
		AllFile[0x00+i]=p[i];
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
	{
		PSW[i]=p[i];
		AllFile[0x0320]=p[i];
	}
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
		AllFile[0x10+i]=p[i];
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
		AllFile[0x0270+i]=p[i];
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
		AllFile[0x0280+i]=p[i];
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
		AllFile[0x0290+i]=p[i];
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
		AllFile[0x02a0+i]=p[i];
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
		AllFile[0x02b0+i]=p[i];
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
		AllFile[0x02c0+i]=p[i];
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
		AllFile[0x02d0+i]=p[i];
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
		AllFile[0x02e0+i]=p[i];
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
		AllFile[0x0260 + 0]=0xC2;
		AllFile[0x0260 + 1]=0xE9;
		AllFile[0x0260 + 2]=0x01;
		AllFile[0x0260 + 3]=0xF0;
		AllFile[0x0260 + 4]=0xED;
		AllFile[0x0260 + 5]=0xBA;
		AllFile[0x0260 + 6]=0x7C;
		AllFile[0x0260 + 7]=0x5C;
	}
	else
	{
		for(int i=0;i<8;i++)
			AllFile[0x0260+i]=p[i];
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
		AllFile[0x0230+i]=p[i];
	delete[] p;
	return true;
}

// bool CEZSData::SetKeyStatus(byte s1,byte s2)
// {
// 	KeyStatus1=s1;//pBuf[4];
// 	KeyStatus2=s2;//pBuf[6];
// 	return true;
// }
// 
// bool CEZSData::SetStatus(CString str)
// {
// 	int len;
// 	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
// 	if(p==NULL)
// 		return false;
// 	if(len!=1)
// 		return false;
// 	for(int i=0;i<1;i++)
// 		AllFile[0x80+i]=p[i];
// 	delete[] p;
// 	return true;
// }

// void CEZSData::SetStatus(byte b)
// {
// 	AllFile[0x80]=b;
// }

// bool CEZSData::SetLastKey(CString str)
// {
// 	int len;
// 	len=CCommFunc::GetCStringLength(str);
// 	if(len==1)
// 	{
// 		//ЮЊПе
// 		AllFile[0xd8]=0x0e;
// 		return true;
// 	}
// 	if(len==2)
// 	{
// 		char* pString=CCommFunc::CStringToChar(str,&len);
// 		byte c=CCommFunc::ASCII2Byte(pString[0]);
// 		delete[] pString;
// 		if(c!=0xff)
// 		{
// 			AllFile[0xd8]=c;
// 			return true;
// 		}
// 		else
// 			return false;
// 	}
// 	return false;
// }
// bool CEZSData::SetPreviousKey(CString str)
// {
// 	int len;
// 	len=CCommFunc::GetCStringLength(str);
// 	if(len==1)
// 	{
// 		//ЮЊПе
// 		AllFile[0xd8]=0x0e;
// 		return true;
// 	}
// 	if(len==2)
// 	{
// 		char* pString=CCommFunc::CStringToChar(str,&len);
// 		byte c=CCommFunc::ASCII2Byte(pString[0]);
// 		delete[] pString;
// 		if(c!=0xff)
// 		{
// 			AllFile[0xda]=c;
// 			return true;
// 		}
// 		else
// 			return false;
// 	}
// 	return false;
// }

void CEZSData::SetAllFile(byte* pBuf)
{
	for(int i=0;i<0x400;i++)
		AllFile[i]=pBuf[i];
}

// void CEZSData::SetEISWriteBuf()
// {
// 	////key0
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[i]=AllFile[0x01ac+i];
// 	////key1
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x08+i]=AllFile[0x01be+i];
// 	////key2
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x10+i]=AllFile[0x01d0+i];
// 	////key3
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x18+i]=AllFile[0x01e2+i];
// 	////key4
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x20+i]=AllFile[0x01f4+i];
// 	////key5
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x28+i]=AllFile[0x0206+i];
// 	////key6
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x30+i]=AllFile[0x0218+i];
// 	////key7
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x38+i]=AllFile[0x022a+i];
// 	////Dealer
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x40+i]=AllFile[0x019a+i];
// 	////PSW
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x48+i]=AllFile[0x008e+i];
// 	////SSID
// 	//for(int i=0;i<4;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x58+i]=AllFile[0x0be+i];
// 	////00-07
// 	//for(int i=0;i<8;i++)
// 	//	CCarKey2App::TheHIDDevice.DTwr[0x50+i]=i;
// 	//CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x04] = 0x00;
// 	////	pBuf2[0x58+0x04]=DTwr[0x58+0x04];
// 	//CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x05] = 0x00;
// 	////	pBuf2[0x58+0x05]=DTwr[0x58+0x05];
// 	//CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x06] = 0x00;
// 	////	pBuf2[0x58+0x06]=DTwr[0x58+0x06];
// 	//CCarKey2App::TheHIDDevice.DTwr[0x58 + 0x07] = 0x1c;
// 	////	pBuf2[0x58+0x07]=DTwr[0x58+0x07];  
// }

byte* CEZSData::GetSKCData()
{
	byte pBuf[0x400];
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_SKCBIN1), TEXT("SKCBIN"));
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	LPVOID pBuffer = LockResource(hGlobal);
	byte* bin=(byte*)(pBuffer);
	for(int i=0;i<0x400;i++)
		pBuf[i]=bin[i];//AllFile[i];
	
	for(int i=0;i<17;i++)
		pBuf[0x2e+i]=AllFile[0x0300+i];	

	for(int i=0;i<8;i++)
		pBuf[0x8e+i]=PSW[i];

	for(int i=0;i<8;i++)
		pBuf[0xa6+i]=AllFile[0x0260+i];
	for(int i=0;i<8;i++)
		pBuf[0xb6+i]=AllFile[0x0230+i];
	for(int i=0;i<4;i++)
		pBuf[0xbe+i]=AllFile[0x0000+i];
	pBuf[0x00d8]=AllFile[7]&0x0f;
	pBuf[0x00d8]=(AllFile[7]&0xf0)>>4;
	int offset=0;
	//key0~7
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x0270+i];
		pBuf[0x1ac+i+offset]=AllFile[0x0270+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0270,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0270,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x0280+i];
		pBuf[0x1ac+i+offset]=AllFile[0x0280+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0280,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0280,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x0290+i];
		pBuf[0x1ac+i+offset]=AllFile[0x0290+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0290,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x0290,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x02a0+i];
		pBuf[0x1ac+i+offset]=AllFile[0x02a0+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02a0,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02a0,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x02b0+i];
		pBuf[0x1ac+i+offset]=AllFile[0x02b0+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02b0,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02b0,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x02c0+i];
		pBuf[0x1ac+i+offset]=AllFile[0x02c0+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02c0,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02c0,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x02d0+i];
		pBuf[0x1ac+i+offset]=AllFile[0x02d0+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02d0,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02d0,8);
	offset=offset+0x12;
	for(int i=0;i<8;i++)
	{
		pBuf[0x10a+i+offset]=AllFile[0x02e0+i];
		pBuf[0x1ac+i+offset]=AllFile[0x02e0+i];
	}
	pBuf[0x10a+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02e0,8);
	pBuf[0x1ac+offset - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x02e0,8);
	//dealer
	for(int i=0;i<8;i++)
	{
		pBuf[0x19a+i]=AllFile[0x10+i];
		pBuf[0x23c+i]=AllFile[0x10+i];
	}
	pBuf[0x19a - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x10,8);
	pBuf[0x23c - 0x008] = CCommFunc::GetVerify_byteSum(AllFile,0x10,8);

	byte* save=new byte[0x800];
	for (int i = 0; i < 0x800; i++)
		save[i] = 0xff;
	for (int i = 0, k = 0; i < 0x400; )
	{
		save[k++] = pBuf[i++];
		save[k++] = pBuf[i++];
		k += 2;
	}
	return save;
}

void CEZSData::Clear()
{
	//byte s=AllFile[0x80];
	for(int i=0;i<0x400;i++)
		AllFile[i]=0x00;
	//AllFile[0x80]=s;
}

CString CEZSData::GetVIN()
{
	char VIN[18];
	for(int i=0;i<17;i++)
		VIN[i]=AllFile[0x0300+i];
	VIN[17]='\0';
	bool bVIN=true;
	for(int i=0;i<17;i++)
	{
		if(isgraph(VIN[i])==0)
		{
			bVIN=false;
			break;
		}
	}

	if(bVIN)
	{
		CString str;
		str=VIN;
		return str;
	}
	return _T("");
}

bool CEZSData::bHavePSW()
{
	for(int i=0;i<8;i++)
	{
		if(AllFile[0x0320+i]!=0 && AllFile[0x0320+i]!=0xff)
			return true;
	}
	return false;
}

