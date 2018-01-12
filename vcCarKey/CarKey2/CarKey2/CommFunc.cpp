#include "StdAfx.h"
#include "CommFunc.h"


CCommFunc::CCommFunc(void)
{
}


CCommFunc::~CCommFunc(void)
{
}

char* CCommFunc::CStringToChar(CString str,int* pLen)
{
	char* pchStr;
	int nLen=GetCStringLength(str);
#ifdef  UNICODE
	pchStr = new char[nLen];
	WideCharToMultiByte(CP_ACP, NULL, str.GetBuffer(str.GetLength()), -1, pchStr, nLen,NULL, FALSE);
#else
	pchStr = new char[nLen];	
	for(int i=0;i<nLen-1;i++)
		pchStr[i]=str.GetAt(i);
	pchStr[nLen-1]='\0';
#endif
	if(pLen!=NULL)
	{
		*pLen=nLen;
	}
	return pchStr;
}

int CCommFunc::GetCStringLength(CString str)
{
	int nLen;
#ifdef  UNICODE
	nLen = WideCharToMultiByte(CP_ACP, NULL, str.GetBuffer(str.GetLength()), -1,	NULL,  0,  NULL,  FALSE);
#else
	nLen=str.GetLength()+1;
#endif
	return nLen;
}

byte CCommFunc::GetVerify_byteSum(byte* pBuff, int start, int count)
{
	byte sum = 0;
	for (int i = 0; i < count; i++)
		sum = (byte)(sum + pBuff[i + start]);
	return sum;
}

byte* CCommFunc::strToHexByte(CString hexString,CString spilt,int* pLen)
{
	if(spilt!=_T(""))
		hexString.Replace(spilt,_T(""));
	int len=GetCStringLength(hexString)-1;
	if ((len % 2) != 0)
	{
		hexString += _T(" ");
		len++;
	}
	char* pString=CStringToChar(hexString,&len);
	len--;
	*pLen=len / 2;
	byte* ret=new byte[*pLen];
	for (int i = 0; i < *pLen; i++)
	{
		byte c1=ASCII2Byte(pString[i*2]);
		byte c2=ASCII2Byte(pString[i*2+1]);
		if(c1==0xff || c2==0xff)
			return NULL;
		ret[i]=MakeByte(c1,c2);
	}
	delete[] pString;
	return ret;
}

byte CCommFunc::ASCII2Byte(char c)
{
	switch(c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return (c-'0');

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return (c-'A' + 10);
		break;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		return (c-'a' + 10);
		break;	

	default:
		return 0xff;
	}
}

USHORT CCommFunc::MakeShort(byte h,byte l)
{
	return (USHORT)((((USHORT)h)<<8)+l);
}

bool CCommFunc::GetBit(byte b, int index)
{
	if (index > 7)
		return false;
	if ((byte)((b & (1 << index)) >> index) == 0x01)
		return true;
	else
		return false;
}

CString CCommFunc::byteToHexStr(byte* bytes, int count,CString spilt)
{
	return byteToHexStr(bytes,0,count,spilt);
}

CString CCommFunc::byteToHexStr(byte* bytes, int offset,int count,CString spilt)
{
	CString str=_T("");
	CString str1;
	if (bytes != NULL)
	{
		for (int i = 0; i < (count-1);i++ )
		{
			str1.Format(_T("%02X"),bytes[offset+i]);
			str+=str1;
			str+=spilt;
		}
		str1.Format(_T("%02X"),bytes[offset + count-1]);
		str+=str1;
	}
	return str;
}

byte CCommFunc::MakeByte(byte h,byte l)
{
	return (byte)((h<<4)+(l&0x0f));
}
