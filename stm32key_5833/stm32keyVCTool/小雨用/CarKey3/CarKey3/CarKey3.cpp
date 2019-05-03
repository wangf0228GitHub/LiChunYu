
// CarKey3.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgNEC_CHIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCarKey3App

BEGIN_MESSAGE_MAP(CCarKey3App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCarKey3App 构造

CCarKey3App::CCarKey3App()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCarKey3App 对象

CCarKey3App theApp;


// CCarKey3App 初始化

BOOL CCarKey3App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	HANDLE hObject = ::CreateMutex(NULL,FALSE, _T("wflcystm32keyTool20190425")); 
	if(GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		CloseHandle(hObject); 
		CString str,str1;
		str.LoadString(IDS_OneOpen);
		str1.LoadString(IDS_Warring);
		MessageBox(NULL,str, str1, MB_ICONERROR|MB_OK); 
		return FALSE; 
	}
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	CDlgNEC_CHIP dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
void CCarKey3App::WaitMS(int ms)
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

float CCarKey3App::m_fHexVer=0;

WORD CCarKey3App::LANGID;

CString CCarKey3App::strLogFormat=_T("%-30s%-20s%s\r\n");

byte CCarKey3App::HPassword[8];

byte CCarKey3App::PassSSID[4];

bool CCarKey3App::m_bPass=false;

float CCarKey3App::m_fSoftVer=30.02f;

CString CCarKey3App::RunPath;
CWithServer CCarKey3App::withServer;
CHIDWork CCarKey3App::TheHIDDevice;
