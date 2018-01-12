#pragma once
class CEZSData
{
public:
	CEZSData(void);
	~CEZSData(void);	
	void SetAllFile(byte* pBuf);
	void SetVerify();
	void Clear();
	byte AllFile[0x400];
	CString GetSSID();
	CString GetPSW();	
	CString GetKey0();
	CString GetKey1();
	CString GetKey2();
	CString GetKey3();
	CString GetKey4();
	CString GetKey5();
	CString GetKey6();
	CString GetKey7();
	CString GetTransport();
	CString GetSerialNum();
	CString GetDealer();

	void SetEISWriteBuf();

	CString GetKeyStatus(int index,COLORREF* txtColor);
	byte GetStatus();
	CString GetLastKey();
	CString GetPreviousKey();

	bool SetSSID(CString str);
	bool SetPSW(CString str);	
	bool SetKey0(CString str);
	bool SetKey1(CString str);
	bool SetKey2(CString str);
	bool SetKey3(CString str);
	bool SetKey4(CString str);
	bool SetKey5(CString str);
	bool SetKey6(CString str);
	bool SetKey7(CString str);
	bool SetTransport(CString str);
	bool SetSerialNum(CString str);
	bool SetDealer(CString str);
	bool SetKeyStatus(byte s1,byte s2);
	bool SetStatus(CString str);
	void SetStatus(byte b);
	bool SetLastKey(CString str);
	bool SetPreviousKey(CString str);

	byte KeyStatus1;
	byte KeyStatus2;
};

enum KeyStatus
{
	KeyStatus_Used,
	KeyStatus_Lock,
	KeyStatus_NoUsed
};