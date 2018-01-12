
// CarKey2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CarKey2.h"
#include "CarKey2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarKey2App

BEGIN_MESSAGE_MAP(CCarKey2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarKey2App construction

CCarKey2App::CCarKey2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCarKey2App object

CCarKey2App theApp;


// CCarKey2App initialization

BOOL CCarKey2App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CCarKey2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// delete[] the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CCarKey2App::WaitMS(int ms)
{
	MSG msg; //定义一个MSG类型的变量
	while (ms != 0)
	{
		while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE)) //获取消息并把该消息从消息队列中移除(防止重复响应)。
		{
			::DispatchMessage(&msg); //将消息移交给过程函数
			::TranslateMessage(&msg);//翻译消息 在合适的机会产生char消息
		}
		Sleep(1);
		ms--;
	}
}

CString CCarKey2App::RunPath;
//#ifdef _UseCpoy
CHIDDevice CCarKey2App::TheHIDDevice;
//#endif


