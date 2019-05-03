#pragma once
class CCommFunc
{
public:
	CCommFunc(void);
	~CCommFunc(void);
	static char* CStringToChar(CString str,int* pLen);
	static int GetCStringLength(CString str);
	static CString byteToHexStr(byte* bytes, int count,CString spilt);
	static CString byteToHexStr(byte* bytes, int offset,int count,CString spilt);
	static byte GetVerify_byteSum(byte* pBuff, int start, int count);
	static byte* strToHexByte(CString hexString,CString spilt,int* pLen);
	static byte ASCII2Byte(char c);
	static USHORT MakeShort(byte h,byte l);
	static bool GetBit(byte b, int index);
	static byte MakeByte(byte h,byte l);
	static int EncryptOffset;
	static byte* Encrypt(byte* pBuf,int count, byte* DeviceID, byte* SSID);
	static byte* Encrypt(byte* pBuf,int count, byte* DeviceID, byte* SSID,int offset);
	static byte* PSWEncrypt(byte* pBuf,bool bQuanDiu=false);
	static byte* PSWDecrypt(byte* pBuf,bool bQuanDiu=false);

	static void cal_asic(unsigned char C,unsigned char INb[],char *const P_Out);

	static BOOL ImageFromIDResource(ATL::CImage *pImage, UINT nResID,LPCTSTR lpTyp);
};
