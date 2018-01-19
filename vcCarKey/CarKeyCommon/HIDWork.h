#pragma once
#include "wfhiddevice.h"
class CHIDWork :public CwfHIDDevice
{
public:
	CHIDWork(void);
	~CHIDWork(void);
	CString GetCANInfoStr(byte* pBuf);
	byte* CANReadInfoData(int mode,CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* CANReadEEPROM(int mode,CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* IRReadData(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar);
	void CANWriteEZS(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,CEZSData* pEZSData);
	void RenewEZS(int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,byte* pSSID,byte* pDPass,byte* pPass);

	byte* SuanMiMa( HWND hWnd,int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,bool* bRun);
	byte* QuanDiu03(HWND hWnd,int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,bool* bRun);



	byte* ESLReadInfo(CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* ESLReadData(CProgressCtrl* pProgress,CInteractionData* pResult);
	void ESLWrite(CProgressCtrl* pProgress,CInteractionData* pResult,byte* pIndex,byte* cPSW);

	void ESLRenew(byte* pPSW,CInteractionData* pResult);

	byte* EISTest(int mode,CInteractionData* pResult,int* P);

	void UpdateDevice(byte* pBuf,CInteractionData* pResult,CProgressCtrl* pProgress);
	void WriteVIN(int mode,byte* pBuf,CInteractionData* pResult,CProgressCtrl* pProgress);
	byte* KeyData_UartReadIR(int type, int mode,CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/);
	void KeyData_UartProgram(int type, int mode,CProgressCtrl* pProgress,CInteractionData* pResult,byte* pBuf/*,CStatusBarCtrl* pStatusBar*/);
	void KeyData_UartRenew(int type, int mode,CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/);
	byte* KeyData_UartReadPSW(int type, int mode,CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/);
	byte* ISMReadInfo(CProgressCtrl* pProgress,CInteractionData* pResult );
	byte* ISMReadData(CProgressCtrl* pProgress,CInteractionData* pResult );
	void ISMRenew(byte* pDS, CProgressCtrl* pProgress,CInteractionData* pResult );
	byte* ECUReadData(CProgressCtrl* pProgress,CInteractionData* pResult,bool* fEcuMd);
	byte* ECUReadInfo(CProgressCtrl* pProgress,CInteractionData* pResult );
	void ECURenew(byte* pDS, CProgressCtrl* pProgress,CInteractionData* pResult,bool fEcuMd );
	byte* GBoxReadData(CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* GBoxReadInfo(CProgressCtrl* pProgress,CInteractionData* pResult );
	void GBoxRenew(byte* DS, CProgressCtrl* pProgress,CInteractionData* pResult);
	byte* QuanDiu02(HWND hWnd,int mode,CProgressCtrl* pProgress,CInteractionData* pResult,CStatusBarCtrl* pStatusBar,bool* bRun);
	byte* ECUReadInfo_SIM217DE(CProgressCtrl* pProgress,CInteractionData* pResult );
};

