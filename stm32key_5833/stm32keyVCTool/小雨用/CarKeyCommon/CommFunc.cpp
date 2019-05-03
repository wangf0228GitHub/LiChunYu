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
		{
			delete[] pString;
			delete[] ret;
			return NULL;
		}
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

byte* CCommFunc::Encrypt( byte* pBuf, int count,byte* DeviceID, byte* SSID,int offset )
{
	byte* ret = new byte[count];
	byte key[8];
	for (int i = 0; i < 4; i++)
	{
		key[i] = (byte)(DeviceID[i] + EncryptOffset+offset);
		key[i+4] = (byte)(SSID[i] + EncryptOffset+offset);
	}
	int c = count / 8;
	for (int i = 0; i < c; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ret[i * 8 + j] = (byte)(key[j] ^ pBuf[i * 8 + j]);
		}
	}
	return ret;
}

byte* CCommFunc::Encrypt( byte* pBuf,int count, byte* DeviceID, byte* SSID )
{
	return Encrypt(pBuf,count,DeviceID,SSID,0);
}

byte* CCommFunc::PSWEncrypt( byte* pBuf,bool bQuanDiu/*=false*/ )
{
	byte* ret;
	int count;
	if(bQuanDiu)
	{
		ret=new byte[2048*20*4];
		count=2048*4;
	}
	else
	{
		ret=new byte[1024*20*4];
		count=1024*4;
	}	
	srand((int)time(0));
	for(int i=0;i<count;i++)
	{		
		for(int j=0;j<10;j++)
		{			
			int r=rand()%256;
			ret[i*20+j*2]=(byte)r;
			ret[i*20+j*2+1]=pBuf[i*16+j];
		}
	}
	return ret;
}

byte* CCommFunc::PSWDecrypt( byte* pBuf,bool bQuanDiu/*=false*/ )
{
	byte* ret;
	int count;
	if(bQuanDiu)
	{
		ret=new byte[2048*16*4];
		count=2048*4;
	}
	else
	{
		ret=new byte[1024*16*4];
		count=1024*4;
	}	

	for(int i=0;i<count;i++)
	{
		for(int j=0;j<10;j++)
		{
			ret[i*16+j]=pBuf[i*20+j*2+1];
		}
		for(int j=10;j<16;j++)
		{
			ret[i*16+j]=0x00;
		}
	}
	return ret;
}

void CCommFunc::cal_asic( unsigned char C,unsigned char INb[],char *const P_Out )
{
	unsigned char i=0;
	unsigned char j=0;
	unsigned char k;
	unsigned char  AC[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x39,0x41,0x42,0x43,0x44,0x45,0x46};
	for(k=0;k<C;k++)
	{
		P_Out[j++]=AC[(INb[i]>>4) & 0x0F];
		P_Out[j++]=AC[INb[i] & 0x0F];
		i++;
	}
}

BOOL CCommFunc::ImageFromIDResource( ATL::CImage *pImage, UINT nResID,LPCTSTR lpTyp )
{
	if ( pImage == NULL) return false;


	pImage->Destroy();


	// 查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL) return false;


	// 加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}


	// 锁定内存中的指定资源
	LPVOID lpVoid    = ::LockResource(hImgData);


	LPSTREAM pStream = NULL;
	DWORD dwSize    = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew    = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte    = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);


	// 解除内存中的指定资源
	::GlobalUnlock(hNew);


	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if ( ht != S_OK )
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		pImage->Load(pStream);


		GlobalFree(hNew);
	}
	// 释放资源
	::FreeResource(hImgData);
	return true;
}


int CCommFunc::EncryptOffset=5;
