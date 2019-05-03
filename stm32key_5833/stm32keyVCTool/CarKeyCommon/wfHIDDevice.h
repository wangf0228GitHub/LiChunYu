#pragma once

#include "InteractionData.h"
#include <afxcmn.h>
#include "EZSData.h"


class CwfHIDDevice : public CWnd
{
	DECLARE_DYNAMIC(CwfHIDDevice)

public:
	CwfHIDDevice();
	virtual ~CwfHIDDevice();
	CProgressCtrl* m_pProgress;
	bool MyDeviceDetected;
	bool FindDevice();	
	unsigned short VendorID;
	unsigned short ProductID;
	
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


