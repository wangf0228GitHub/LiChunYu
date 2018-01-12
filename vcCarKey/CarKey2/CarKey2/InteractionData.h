#pragma once


// CInteractionData

class CInteractionData
{
	DECLARE_DYNAMIC(CInteractionData)

public:
	CInteractionData();
	CInteractionData(byte* pBuf);
	virtual ~CInteractionData();
	byte* GetDataList();
	bool CheckDataList();
	void SetData(byte* pBuf);
	CString GetHIDResult();
	void Init();
	byte first;
	bool bOK;
	enum HIDResult hidResult;
	byte buff[0x40];
	int rxCount;
	bool mimaok;
};
enum HIDResult
{
	HIDResult_OK,
	HIDResult_MimaErr,
	HIDResult_USBError,
	HIDResult_RxError,
	HIDResult_ReadError,
	HIDResult_WriteError,
	HIDResult_RenewError,
	HIDResult_ProgramError,
	HIDResult_GetPSWError1,
	HIDResult_GetPSWError2,
	HIDResult_GetPSWError3,
	HIDResult_ReadEZSError1,
	HIDResult_ReadEZSError2,
	HIDResult_ReadEZSError3,
	HIDResult_ReadEZSError4,
	HIDResult_ReadEZSError5,
	HIDResult_CANErr,
	HIDResult_CANWriteWait
};
enum KeyOptions
{
	IRPreparedKey,
	ChipOnThePCB,
	NewPCB,
	ReNew
};


