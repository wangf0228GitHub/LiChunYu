
// CarKey2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include ".\..\..\CarKeyCommon\HIDDevice.h"


// CCarKey2App:
// See CarKey2.cpp for the implementation of this class
//

class CCarKey2App : public CWinApp
{
public:
	CCarKey2App();
	static void WaitMS(int ms);
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	static CHIDDevice TheHIDDevice;
	static CString RunPath;
	DECLARE_MESSAGE_MAP()
};

extern CCarKey2App theApp;