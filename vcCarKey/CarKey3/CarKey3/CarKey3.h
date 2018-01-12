
// CarKey3.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#ifdef _UseCopy
#include ".\..\..\CarKeyCommon\HIDDevice.h"
#else
#include "..\..\CarKeyCommon\HIDWork.h"
#endif
#include "..\..\CarKeyCommon\WithServer.h"

// CCarKey3App:
// 有关此类的实现，请参阅 CarKey3.cpp
//

class CCarKey3App : public CWinApp
{
public:
	CCarKey3App();
	static void WaitMS(int ms);
// 重写
public:
	virtual BOOL InitInstance();

// 实现
	static CString RunPath;
	static CWithServer withServer;
	static float m_fSoftVer;
	static float m_fHexVer;
#ifdef _UseCopy
	static CHIDDevice TheHIDDevice;
#else
	static CHIDWork TheHIDDevice;
	static bool m_bPass;
	static byte PassSSID[4];
	static byte HPassword[8];
	static CString strLogFormat;
	static WORD LANGID;
#endif
	DECLARE_MESSAGE_MAP()
};

extern CCarKey3App theApp;