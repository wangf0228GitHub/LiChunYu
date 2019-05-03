#pragma once
#include "wfhiddevice.h"
class CHIDWork :public CwfHIDDevice
{
public:
	CHIDWork(void);
	~CHIDWork(void);
	
	byte* KeyData_UartReadIR(CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/);
	void KeyData_UartProgram(CProgressCtrl* pProgress,CInteractionData* pResult,byte* pBuf/*,CStatusBarCtrl* pStatusBar*/);
	void KeyData_UartRenew(CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/);
	void KeyData_SetRenewTimes(int n,CInteractionData* pResult);
	
};

