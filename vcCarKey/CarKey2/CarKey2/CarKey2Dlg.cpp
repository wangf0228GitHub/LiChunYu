
// CarKey2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CarKey2.h"
#include "CarKey2Dlg.h"
#include "afxdialogex.h"
#include "MainDlg.h"
#include "KeyModuleDlg.h"
#include ".\..\..\CarKeyCommon\CommFunc.h"
#include "ECUDlg.h"
#include "ELVDlg.h"
#include "ISM7GDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCarKey2Dlg dialog




CCarKey2Dlg::CCarKey2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCarKey2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCarKey2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_Pic, m_Pic);
}

BEGIN_MESSAGE_MAP(CCarKey2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_LIST1, &CCarKey2Dlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CCarKey2Dlg::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_LIST1, &CCarKey2Dlg::OnLbnDblclkList1)
	ON_WM_TIMER()
	ON_COMMAND(ID_LANGUAGE_32775, &CCarKey2Dlg::OnLanguage32775)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, &CCarKey2Dlg::OnLanguageEnglish)
END_MESSAGE_MAP()


// CCarKey2Dlg message handlers

BOOL CCarKey2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
 	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
 
 	int strPartDim[1]= {-1}; //分割数量
 	m_StatusBar.SetParts(1, strPartDim);
 	//设置状态栏文本
 	m_StatusBar.SetText(_T(""), 0, 0);
 	//下面是在状态栏中加入图标
 	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString   path=AfxGetApp()->m_pszHelpFilePath;   
	CString   str=AfxGetApp()->m_pszExeName;   
	path=path.Left(path.GetLength()-str.GetLength()-4); 
	CCarKey2App::RunPath=path;
	path+=_T("Language.ini");
	GetPrivateProfileString(_T("Form"),_T("Language"), _T(""), str.GetBuffer(MAX_PATH), MAX_PATH ,path);
	if(str==_T("Chinese"))
	{
		SetThreadUILanguage(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED));
		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);
	}
	else if(str==_T("English"))
	{
		SetThreadUILanguage(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US));
		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_CHECKED);
	}
	else
	{
		WritePrivateProfileString(_T("Form"),_T("Language"), _T("Chinese"),path); 
		SetThreadUILanguage(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED));
		GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);
	}
	LoadString();
	CCarKey2App::TheHIDDevice.hOwner=this->m_hWnd;
	m_hUsbEventHandle = CCarKey2App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
	SetTimer(1,1000,NULL);//启动定时器1,定时时间是1秒

	//枚举对话框中所有组件  
// 	if  (GetUserDefaultUILanguage() == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED)) 
// 	{ 
// 		SetThreadUILanguage(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US)); 
// 	} 

	////枚举对话框中所有组件
	//CWnd *pCtrl = GetWindow(GW_CHILD);
	//while(pCtrl!=NULL)
	//{
	//	UINT ctrlID = pCtrl->GetDlgCtrlID();
	//	//	setControlCaption(pCtrl,ctrlID);
	//	CString str_id,str_text;
	//	str_id.Format("%u",ctrlID);
	//	GetDlgItem(ctrlID)->GetWindowText(str_text);
	//	WritePrivateProfileString("Form", str_id, str_text,lfname);
	//	pCtrl = pCtrl->GetNextWindow();
	//}
	//ReHIDUI();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCarKey2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCarKey2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCarKey2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCarKey2Dlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码

	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   
	switch(m_ListBox.GetCurSel())
	{
	case 0://key
		bitmap.LoadBitmap(IDB_BITMAP4);
		break;	
	case 1://164 old
	case 2://164 new
		bitmap.LoadBitmap(IDB_BITMAP13);
		break;
	case 3://166
		bitmap.LoadBitmap(IDB_BITMAP6);
		break;
	case 4://204
		bitmap.LoadBitmap(IDB_BITMAP7);
		break;
	case 5://207
		bitmap.LoadBitmap(IDB_BITMAP8);
		break;
	case 6://212 old
	case 7://212 new
		bitmap.LoadBitmap(IDB_BITMAP9);
		break;
	case 8://221 old
	case 9://221 new
		bitmap.LoadBitmap(IDB_BITMAP11);
		break;
	case 10://246
		bitmap.LoadBitmap(IDB_BITMAP10);
		break;
	case 11://elv
		bitmap.LoadBitmap(IDB_BITMAP15);
		break;
	case 12://ecu
		bitmap.LoadBitmap(IDB_BITMAP14);
		break;	
	case 13://变速箱
		bitmap.LoadBitmap(IDB_BITMAP12);
		break;
	default:
		bitmap.LoadBitmap(IDB_BITMAP3);
		break;
	}
	  // 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_Pic.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  	
}


void CCarKey2Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnLbnDblclkList1();
}


void CCarKey2Dlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ListBox.GetCurSel()!=-1)
	{
		CCarKey2App::TheHIDDevice.UnregisterForUsbEvents(m_hUsbEventHandle);
		KillTimer(1);
		int sel=m_ListBox.GetCurSel();
		switch(sel)
		{
		case 0:
			{
				CKeyModuleDlg keyDlg;
				keyDlg.m_pMainDlg=this;
				this->ShowWindow(SW_HIDE);
				keyDlg.DoModal();
				delete keyDlg;
			}
			break;		
		case 1://W164-NEC-old
			{
				CMainDlg mainDlg;	
				mainDlg.m_pMainDlg=this;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 2://W164-NEC-new
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				mainDlg.m_PortalOP.IROp=IROptions_MOT;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 3://W166-NEC
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 4://W204-NEC
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				mainDlg.m_PortalOP.bELV=true;
				mainDlg.m_PortalOP.strWork[1]=_T("ESL");
				mainDlg.m_PortalOP.strWork[2]=_T("ALL");
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 5://W207-NEC
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				mainDlg.m_PortalOP.bELV=true;
				mainDlg.m_PortalOP.strWork[1]=_T("ESL");
				mainDlg.m_PortalOP.strWork[2]=_T("ALL");
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 6://W212-NEC-old
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				mainDlg.m_PortalOP.bELV=true;
				mainDlg.m_PortalOP.strWork[1]=_T("ESL");
				mainDlg.m_PortalOP.strWork[2]=_T("ALL");
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 7://W212-NEC-new
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				mainDlg.m_PortalOP.IROp=IROptions_MOT;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 8://W221-NEC-old
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 9://W221-NEC-new
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				mainDlg.m_PortalOP.IROp=IROptions_MOT;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 10://W246-NEC
			{
				CMainDlg mainDlg;
				mainDlg.m_pMainDlg=this;
				m_ListBox.GetText(sel, mainDlg.m_PortalDlg.m_strModelName);
				this->ShowWindow(SW_HIDE);
				mainDlg.DoModal();
				delete mainDlg;
			}
			break;
		case 11://elv
			{
// 				CELVDlg elvDlg;
// 				this->ShowWindow(SW_HIDE);
// 				elvDlg.DoModal();
// 				delete elvDlg;
			}
			break;
		case 12://ecu
			{
// 				CECUDlg ecuDlg;
// 				this->ShowWindow(SW_HIDE);
// 				ecuDlg.DoModal();
// 				delete ecuDlg;
			}
			break;		
		case 13://ect
			{
// 				CISM7GDlg ismDlg;
// 				this->ShowWindow(SW_HIDE);
// 				ismDlg.DoModal();
// 				delete ismDlg;
			}
			break;
		}		
		m_hUsbEventHandle = CCarKey2App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
		ReHIDUI();
		this->ShowWindow(SW_SHOW);
		OnLbnSelchangeList1();
	}
}

BOOL CCarKey2Dlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == 0x0219)
	{
		switch (wParam)
		{
		case 0x8000:
			if (!CCarKey2App::TheHIDDevice.MyDeviceDetected)
			{
				CCarKey2App::TheHIDDevice.FindDevice(); 
			}
			HIDUI();
			break;
		case 0x8004:
// 			try
// 			{
 				CCarKey2App::TheHIDDevice.Close();
// 			}
// 			catch (CException* e)
// 			{
// 				throw _T("USB 移除出错!!");
// 			}
			HIDUI();
			break;
		default:
			break;
		}
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}

void CCarKey2Dlg::HIDUI()
{
	CString strTemp;
	if (CCarKey2App::TheHIDDevice.MyDeviceDetected)
	{
		USHORT hexver= CCommFunc::MakeShort(CCarKey2App::TheHIDDevice.HexVer[0], CCarKey2App::TheHIDDevice.HexVer[1]);
		m_ListBox.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		CString strID=CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""));
		strTemp.LoadString(IDS_DeviceID);
		m_StatusBar.SetText(strTemp+_T(":")+strID, 0, 0);
		KillTimer(1);
	}
	else
	{
		//m_ListBox.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		strTemp.LoadString(IDS_NoDevice);
		m_StatusBar.SetText(strTemp, 0, 0);
		SetTimer(1,1000,NULL);//启动定时器1,定时时间是1秒
	}
}

void CCarKey2Dlg::ReHIDUI()
{
	if (!CCarKey2App::TheHIDDevice.MyDeviceDetected)
	{
		CCarKey2App::TheHIDDevice.FindDevice(); 
	}
	HIDUI();
}


void CCarKey2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);        
	ReHIDUI();
	CDialogEx::OnTimer(nIDEvent);
}


void CCarKey2Dlg::OnLanguage32775()
{
	// TODO: 在此添加命令处理程序代码
	SetThreadUILanguage(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED));
	GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_UNCHECKED);

	WritePrivateProfileString(_T("Form"),_T("Language"), _T("Chinese"),CCarKey2App::RunPath+_T("Language.ini")); 

	LoadString();
}


void CCarKey2Dlg::OnLanguageEnglish()
{
	// TODO: 在此添加命令处理程序代码
	SetThreadUILanguage(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US));
	GetMenu()->GetSubMenu(0)->CheckMenuItem(0,MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(1,MF_BYPOSITION|MF_CHECKED);

	WritePrivateProfileString(_T("Form"),_T("Language"), _T("English"),CCarKey2App::RunPath+_T("Language.ini")); 

	LoadString();
}

void CCarKey2Dlg::LoadString()
{
	CString strTemp;
	m_ListBox.ResetContent();
	strTemp.LoadString(IDS_KeyModule);
	m_ListBox.AddString(strTemp);	
	m_ListBox.AddString(_T("W164-NEC-old"));
	m_ListBox.AddString(_T("W164-NEC-new"));	
	m_ListBox.AddString(_T("W166-NEC"));
	m_ListBox.AddString(_T("W204-NEC"));
	m_ListBox.AddString(_T("W207-NEC"));
	m_ListBox.AddString(_T("W212-NEC-old"));
	m_ListBox.AddString(_T("W212-NEC-new"));	
	m_ListBox.AddString(_T("W221-NEC-old"));
	m_ListBox.AddString(_T("W221-NEC-new"));	
	m_ListBox.AddString(_T("W246-NEC"));
	strTemp.LoadString(IDS_ELVModule);
	m_ListBox.AddString(strTemp);	
	strTemp.LoadString(IDS_ECUModule);
	m_ListBox.AddString(strTemp);		
	strTemp.LoadString(IDS_ISM7GModule);
	m_ListBox.AddString(strTemp);

	strTemp.LoadString(IDS_CarKey2_btStart);    //LoadString读取一个ID号定义的字符串资源 
	SetDlgItemText(IDC_BUTTON1,strTemp); 
	if (CCarKey2App::TheHIDDevice.MyDeviceDetected)
	{
		CString strID=CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""));
		strTemp.LoadString(IDS_DeviceID);
		m_StatusBar.SetText(strTemp+_T(":")+strID, 0, 0);
	}
	else
	{
		strTemp.LoadString(IDS_NoDevice);
		m_StatusBar.SetText(strTemp, 0, 0);
	}
}
