#pragma once

#include "InteractionData.h"
#include <afxcmn.h>
#include "EZSData.h"


// CHIDDevice

class CHIDDevice : public CWnd
{
	DECLARE_DYNAMIC(CHIDDevice)

public:
	CHIDDevice();
	virtual ~CHIDDevice();
	CProgressCtrl* m_pProgress;
	bool MyDeviceDetected;
	bool FindDevice();	
	unsigned short VendorID;
	unsigned short ProductID;
	CString GetCANInfoStr(byte* pBuf);
	CInteractionData DeviceInteraction(CInteractionData tx,int n);
	CInteractionData DeviceInteraction(CInteractionData tx);
	HANDLE RegisterForUsbEvents(HANDLE hWnd);
	BOOL UnregisterForUsbEvents(HANDLE hHandle);
	byte HexVer[2];
	byte HexID[4];	
	HANDLE DeviceHandle;
	USHORT InputReportByteLength;
	USHORT OutputReportByteLength;
	HANDLE hOwner;	
	CInteractionData Check_mima();
	void Close();
	CInteractionData ReadKeyData(byte type);
	CInteractionData ReadPSW1(byte type);
	CInteractionData ReadPSW2();
	bool bLoadKeyData;
	byte KeyDataBuf[0xa0];
	CInteractionData WriteKeyData1(int step);
	CInteractionData WriteKeyData2(byte type);
	CInteractionData EZSReadDataStep(int step);
	byte KEY[8];
	byte HOP[4];
	byte DTwr[0x0c*0x08];
	int DTwrIndex;
	static const byte DTnec[12];
	static const byte DTmot[12];


	static const byte CANInfo_AT[0x09][0x10];
	CInteractionData CANInfoDataStep(int step);

	static const byte CANEeprom_Cmd00[0x24];
	static const byte CANEeprom_a[0x02][0x10];
	static const byte CANEeprom_b[0x03][0x10];
	static const byte CANEeprom_c[0x04][0x10];	
	
	CInteractionData CANEeprom1Step(int step);
	CInteractionData CANEeprom2Step(int step);
	CInteractionData CANEeprom3Step(int step);
	static const byte CANwr_a[0x06][0x10];
	static const byte CANwr_b[0x03][0x10];
	static const byte CANwr_bChk[0x05][0x10];
	static const byte CANwr_c[0x03][0x10];
	static const byte CANwr_d[0x04][0x10];
	static const byte CANwr_e[0x04][0x10];
	CInteractionData CANWrite1Step(int step);
	CInteractionData CANWrite2Step(int step);
	CInteractionData CANWrite3Step(int step);
	CInteractionData CANWrite4Step(int step);
	CInteractionData CANWrite5Step(int step);
	void SetEISWriteBuf(byte* pbuf);	

	
	byte* CANReadInfoData(int mode,CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* CANReadEEPROM(int mode,CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* CANReadEEPROM1(int mode,CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* IRReadData(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar);
	void CANWriteEZS(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,CEZSData* pEZSData);
	void RenewEZS(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,byte* pSSID,byte* pDPass,byte* pPass);
	
	byte* SuanMiMa(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,CEdit* m_StepLog);
	byte* QuanDiu03(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar);



	byte* ESLReadInfo(CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* ESLReadData(CProgressCtrl* pProgress,CInteractionData* pResult);
	void ESLWrite(CProgressCtrl* pProgress,CInteractionData* pResult,byte* pBuf);

	void ESLRenew(byte* pPSW,CInteractionData* pResult);
private:
	CString hidPath;	
	HANDLE WriteHandle;
	HANDLE ReadHandle;	
	OVERLAPPED HIDOverlapped;
	HANDLE hEventObject;	
	bool CheckDevice();
	void CloseHandles();
	void DECRYPT();
	void ENCODE();	
protected:
	DECLARE_MESSAGE_MAP()
};


