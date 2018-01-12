// DlgEIS.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgEIS.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "CANInfo.h"
#include "..\..\CarKeyCommon\InteractionData.h"
#include "..\..\CarKeyCommon\PassCalc.h"
#include "EISTest.h"
#include "..\..\CarKeyCommon\MyStringSplitter.h"
#include "DlgKeyData.h"
#include "..\..\CarKeyCommon\VMProtectSDK.h"
void EIS_DPassResultProc(void* p,byte* pBuf)
{
	CDlgEIS* dlg=(CDlgEIS*)(p);
	CString strTemp;
	if(pBuf!=NULL)
	{
		for(int i=0;i<8;i++)
			dlg->m_EZSData.DPSW[i]=pBuf[i];
		dlg->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));		
		strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
		dlg->ShowLog(strTemp);
		MessageBoxEx(dlg->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);		
		strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
	}
	else
	{
		strTemp.LoadString(NULL,IDS_RenewPassCalc,CCarKey3App::LANGID);
	}	
	dlg->GetDlgItem(IDC_BUTTON18)->EnableWindow(TRUE);
	dlg->SetDlgItemText(IDC_BUTTON18,strTemp);
	dlg->EnableBotton(TRUE);
}
void WaitMS(int ms)
{
	MSG msg; //定义一个MSG类型的变量
	while (ms != 0)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)) //获取消息并把该消息从消息队列中移除(防止重复响应)。
		{
			DispatchMessage(&msg); //将消息移交给过程函数
			TranslateMessage(&msg);//翻译消息 在合适的机会产生char消息
		}
		Sleep(1);
		ms--;
	}	
}

// CDlgEIS 对话框

IMPLEMENT_DYNAMIC(CDlgEIS, CDialogEx)

CDlgEIS::CDlgEIS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEIS::IDD, pParent)
	, m_bInitialised(FALSE)
	, m_bTPremoved(FALSE)
	, m_bPersonalised(FALSE)
	, m_bActivated(FALSE)
	, m_bReaded(false)
{
// 	m_bRunning=false;
// 	m_bThreadExit=true;
}

CDlgEIS::~CDlgEIS()
{
}

void CDlgEIS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO4, m_ModelType);
	DDX_Control(pDX, IDC_COMBO1, m_Portal);
	DDX_Control(pDX, IDC_COMBO2, m_CANformat);
	DDX_Control(pDX, IDC_COMBO3, m_WorkArea);
	DDX_Control(pDX, IDC_EDIT8, m_Key0);
	DDX_Control(pDX, IDC_STATIC8, m_Key0Status);
	DDX_Control(pDX, IDC_STATIC9, m_Key1Status);
	DDX_Control(pDX, IDC_STATIC10, m_Key2Status);
	DDX_Control(pDX, IDC_STATIC11, m_Key3Status);
	DDX_Control(pDX, IDC_STATIC12, m_Key4Status);
	DDX_Control(pDX, IDC_STATIC13, m_Key5Status);
	DDX_Control(pDX, IDC_STATIC14, m_Key6Status);
	DDX_Control(pDX, IDC_STATIC15, m_Key7Status);
	DDX_Control(pDX, IDC_EDIT1, m_SSID);
	DDX_Control(pDX, IDC_EDIT2, m_Odo);
	DDX_Control(pDX, IDC_EDIT3, m_PSW);
	DDX_Control(pDX, IDC_EDIT16, m_SerialNum);
	DDX_Control(pDX, IDC_EDIT20, m_Transport);
	DDX_Control(pDX, IDC_EDIT19, m_Dealer);
	DDX_Control(pDX, IDC_EDIT6, m_VIN);
	DDX_Check(pDX, IDC_CHECK10, m_bInitialised);
	DDX_Check(pDX, IDC_CHECK11, m_bTPremoved);
	DDX_Check(pDX, IDC_CHECK12, m_bPersonalised);
	DDX_Check(pDX, IDC_CHECK13, m_bActivated);
	DDX_Control(pDX, IDC_EDIT4, m_LastKey);
	DDX_Control(pDX, IDC_EDIT5, m_PreviousKey);
	DDX_Control(pDX, IDC_EDIT9, m_Key1);
	DDX_Control(pDX, IDC_EDIT10, m_Key2);
	DDX_Control(pDX, IDC_EDIT11, m_Key3);
	DDX_Control(pDX, IDC_EDIT12, m_Key4);
	DDX_Control(pDX, IDC_EDIT13, m_Key5);
	DDX_Control(pDX, IDC_EDIT14, m_Key6);
	DDX_Control(pDX, IDC_EDIT15, m_Key7);
	DDX_Control(pDX, IDC_PROGRESS2, m_Progress);
}


BEGIN_MESSAGE_MAP(CDlgEIS, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgEIS::OnSelchangeModel)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgEIS::OnBnReadData)
	ON_BN_CLICKED(IDC_BUTTON11, &CDlgEIS::OnBnReadInfo)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgEIS::OnCbnSelchangePortal)
	ON_BN_CLICKED(IDC_BUTTON13, &CDlgEIS::OnBnCANWrite)
	ON_BN_CLICKED(IDC_BUTTON17, &CDlgEIS::OnBnLoadPSW)
	ON_BN_CLICKED(IDC_BUTTON12, &CDlgEIS::OnBnSaveSKC)
	ON_BN_CLICKED(IDC_BUTTON16, &CDlgEIS::OnBnEISTest)
	ON_EN_CHANGE(IDC_EDIT19, &CDlgEIS::OnDealerEdit)
	ON_BN_CLICKED(IDC_BUTTON24, &CDlgEIS::OnBnReadMillsClicked)
	ON_BN_CLICKED(IDC_BUTTON18, &CDlgEIS::OnBnEZSRenewClicked)
	ON_COMMAND(ID_FILE_OPEN, &CDlgEIS::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDlgEIS::OnFileSave)
	ON_COMMAND(ID_CANEdit, &CDlgEIS::OnCanedit)
	ON_COMMAND(ID_DisEdit, &CDlgEIS::OnDisedit)
	ON_BN_CLICKED(IDC_BUTTON19, &CDlgEIS::OnBnWriteVIN)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgEIS 消息处理程序

BOOL CDlgEIS::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	m_Progress.SetStep(1);
	m_ModelType.AddString(_T("W164-NEC-old"));
	m_ModelType.AddString(_T("W164-NEC-new"));	
	m_ModelType.AddString(_T("W166-NEC"));
	m_ModelType.AddString(_T("W204-NEC"));
	m_ModelType.AddString(_T("W207-NEC"));
	m_ModelType.AddString(_T("W212-NEC-old"));
	m_ModelType.AddString(_T("W212-NEC-new"));	
	m_ModelType.AddString(_T("W221-NEC-old"));
	m_ModelType.AddString(_T("W221-NEC-new"));	
	m_ModelType.AddString(_T("W246-NEC"));
	m_ModelType.SetCurSel(0);

	m_VIN.m_bIsHex=false;
	m_Key0.m_bSysColor=false;
	m_Key1.m_bSysColor=false;
	m_Key2.m_bSysColor=false;
	m_Key3.m_bSysColor=false;
	m_Key4.m_bSysColor=false;
	m_Key5.m_bSysColor=false;
	m_Key6.m_bSysColor=false;
	m_Key7.m_bSysColor=false;
	m_SSID.m_bSysColor=false;
	m_Odo.m_bSysColor=false;
	m_PSW.m_bSysColor=false;
	m_SerialNum.m_bSysColor=false;
	m_Transport.m_bSysColor=false;
	m_Dealer.m_bSysColor=false;
	m_VIN.m_bSysColor=false;
	setOption();	
	OnCbnSelchangePortal();
	OnSelchangeModel();
	if(!m_ToolBar.CreateEx( this,TBSTYLE_FLAT , WS_CHILD | WS_VISIBLE |CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS,
		CRect(4,4,0,0))||!m_ToolBar.LoadToolBar(IDR_TOOLBAR2) )
	{
		TRACE0("failedto create toolbar\n");
		return FALSE;
	}
	m_ToolBar.ShowWindow(SW_SHOW);
	//m_ToolBar.SetButtonStyle(3,TBBS_CHECKBOX);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);
	CToolBarCtrl &tbc = m_ToolBar.GetToolBarCtrl();
	tbc.SetState(ID_DisEdit,TBSTATE_HIDDEN);
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	int strPartDim[1]= {-1}; //分割数量
	m_StatusBar.SetParts(1, strPartDim);
	//设置状态栏文本
	m_StatusBar.SetText(_T(""), 0, 0);
	//下面是在状态栏中加入图标
	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标
	/************************************************************************/
	/* 静态控件                                                             */
	/************************************************************************/
	CString strTemp;
	strTemp.LoadString(IDS_SSID);
	SetDlgItemText(IDC_STATIC1,strTemp); 

	strTemp.LoadString(IDS_Odo);
	SetDlgItemText(IDC_STATIC2,strTemp); 

	strTemp.LoadString(IDS_PSW);
	SetDlgItemText(IDC_STATIC3,strTemp); 

	strTemp.LoadString(IDS_SerialNum);
	SetDlgItemText(IDC_STATIC4,strTemp); 

	strTemp.LoadString(IDS_Dealer);
	SetDlgItemText(IDC_STATIC5,strTemp); 

	strTemp.LoadString(IDS_Transport);
	SetDlgItemText(IDC_STATIC6,strTemp); 

	strTemp.LoadString(IDS_VIN);
	SetDlgItemText(IDC_STATIC7,strTemp); 

	strTemp.LoadString(IDS_Key);
	SetDlgItemText(IDC_STATIC8,strTemp+_T("0")); 
	SetDlgItemText(IDC_STATIC9,strTemp+_T("1")); 
	SetDlgItemText(IDC_STATIC10,strTemp+_T("2")); 
	SetDlgItemText(IDC_STATIC11,strTemp+_T("3")); 
	SetDlgItemText(IDC_STATIC12,strTemp+_T("4")); 
	SetDlgItemText(IDC_STATIC13,strTemp+_T("5")); 
	SetDlgItemText(IDC_STATIC14,strTemp+_T("6")); 
	SetDlgItemText(IDC_STATIC15,strTemp+_T("7"));

	strTemp.LoadString(IDS_DumpDlg_StatusGroup);
	SetDlgItemText(IDC_STATIC16,_T("ESL ")+strTemp); 

	strTemp.LoadString(IDS_DumpDlg_UesdKeyGroup);
	SetDlgItemText(IDC_STATIC17,strTemp); 

	strTemp.LoadString(IDS_LastKey);
	SetDlgItemText(IDC_STATIC18,strTemp); 

	strTemp.LoadString(IDS_PreviousKey);
	SetDlgItemText(IDC_STATIC19,strTemp); 

	strTemp.LoadString(IDS_DumpDlg_DataGroup);
	SetDlgItemText(IDC_STATIC20,strTemp); 

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	strTemp.LoadString(IDS_Initialised);
	SetDlgItemText(IDC_CHECK10,strTemp); 

	strTemp.LoadString(IDS_TPRemoved);
	SetDlgItemText(IDC_CHECK11,strTemp);

	strTemp.LoadString(IDS_Personalised);
	SetDlgItemText(IDC_CHECK12,strTemp);

	strTemp.LoadString(IDS_Activated);
	SetDlgItemText(IDC_CHECK13,strTemp);

	strTemp.LoadString(IDS_ModleType);
	SetDlgItemText(IDC_STATIC24,_T("EIS")+strTemp); 

	strTemp.LoadString(IDS_PortDlg_Portal);
	SetDlgItemText(IDC_STATIC21,strTemp); 

	strTemp.LoadString(IDS_PortDlg_CanConnect);
	SetDlgItemText(IDC_STATIC22,strTemp); 

	strTemp.LoadString(IDS_PortDlg_WorkType);
	SetDlgItemText(IDC_STATIC23,strTemp); 


	strTemp.LoadString(IDS_PortDlg_ReadInfo);
	SetDlgItemText(IDC_BUTTON11,strTemp); 

	strTemp.LoadString(IDS_PortDlg_ReadData);
	SetDlgItemText(IDC_BUTTON9,strTemp);

	strTemp.LoadString(IDS_PortDlg_LoadPSW);
	SetDlgItemText(IDC_BUTTON17,strTemp);

	strTemp.LoadString(IDS_Write);
	SetDlgItemText(IDC_BUTTON13,strTemp+_T(" EIS"));

	strTemp.LoadString(IDS_PortDlg_WriteVIN);
	SetDlgItemText(IDC_BUTTON19,strTemp);

	strTemp.LoadString(IDS_PortDlg_ReadOdo);
	SetDlgItemText(IDC_BUTTON24,strTemp);

	strTemp.LoadString(IDS_PortDlg_WriteOdo);
	SetDlgItemText(IDC_BUTTON25,strTemp);

	//strTemp.LoadString(IDS_SaveSKC);
	strTemp.LoadString(IDS_KeyData17);
	SetDlgItemText(IDC_BUTTON12,strTemp);

	strTemp.LoadString(IDS_EISTest);
	SetDlgItemText(IDC_BUTTON16,strTemp);
	
	strTemp.LoadString(IDS_RenewPassCalc);
	SetDlgItemText(IDC_BUTTON18,strTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgEIS::setOption()
{
	CString strTemp;
	if(m_PortalOP.bCAN)
		m_Portal.AddString(_T("CAN"));
	if(m_PortalOP.bKLine)
		m_Portal.AddString(_T("KLine"));
	if(m_PortalOP.bIR)
	{
		strTemp.LoadString(IDS_PortDlg_IR);
		m_Portal.AddString(strTemp);
	}	
	m_Portal.SetCurSel(0);

	if(m_PortalOP.bOBD)
	{
		strTemp.LoadString(IDS_PortDlg_OBD);
		m_CANformat.AddString(strTemp);
	}
	if(m_PortalOP.bDirect)
	{
		strTemp.LoadString(IDS_PortDlg_DirectEZS);
		m_CANformat.AddString(strTemp);
	}
	m_CANformat.SetCurSel(0);

	m_WorkArea.AddString(m_PortalOP.strWork[0]);
	if(m_PortalOP.strWork[1]!=_T(""))
		m_WorkArea.AddString(m_PortalOP.strWork[1]);
	if(m_PortalOP.strWork[2]!=_T(""))
		m_WorkArea.AddString(m_PortalOP.strWork[2]);
	m_WorkArea.SetCurSel(0);
}


void CDlgEIS::OnSelchangeModel()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel=m_ModelType.GetCurSel();
	CWnd *pWnd;
	pWnd = GetDlgItem( IDC_BUTTON24 );     
	pWnd->ShowWindow(FALSE); 
	pWnd = GetDlgItem( IDC_BUTTON25 );     
	pWnd->ShowWindow(FALSE); 
// 	m_ModelType.AddString(_T("W164-NEC-old"));
// 	m_ModelType.AddString(_T("W164-NEC-new"));	
// 	m_ModelType.AddString(_T("W166-NEC"));
// 	m_ModelType.AddString(_T("W204-NEC"));
// 	m_ModelType.AddString(_T("W207-NEC"));
// 	m_ModelType.AddString(_T("W212-NEC-old"));
// 	m_ModelType.AddString(_T("W212-NEC-new"));	
// 	m_ModelType.AddString(_T("W221-NEC-old"));
// 	m_ModelType.AddString(_T("W221-NEC-new"));	
// 	m_ModelType.AddString(_T("W246-NEC"));
	m_Portal.ResetContent();
	m_CANformat.ResetContent();
	m_WorkArea.ResetContent();
	m_PortalOP.Reset();
	switch(sel)
	{	
	case 0://W164-NEC-old
		{
			m_nMode=0;//01款==164old
		}
		break;
	case 1://W164-NEC-new
		{
			m_nMode=4;//05款==164new
			m_PortalOP.IROp=IROptions_MOT;
			pWnd = GetDlgItem( IDC_BUTTON24 );     
			pWnd->ShowWindow(TRUE); 
			pWnd = GetDlgItem( IDC_BUTTON25 );     
			pWnd->ShowWindow(TRUE); 
		}
		break;
	case 2://W166-NEC
		{
			m_nMode=7;//08款==166 09款==246 166和246这两个是一样的
		}
		break;
	case 3://W204-NEC
		{
			m_nMode=1;//02款==204
			m_PortalOP.bELV=true;
			m_PortalOP.strWork[1]=_T("ESL");
			m_PortalOP.strWork[2]=_T("ALL");
		}
		break;
	case 4://W207-NEC
		{
			m_nMode=1;//02款==204
			m_PortalOP.bELV=true;
			m_PortalOP.strWork[1]=_T("ESL");
			m_PortalOP.strWork[2]=_T("ALL");
		}
		break;
	case 5://W212-NEC-old
		{
			m_nMode=2;//03款==212old
			m_PortalOP.bELV=true;
			m_PortalOP.strWork[1]=_T("ESL");
			m_PortalOP.strWork[2]=_T("ALL");
		}
		break;
	case 6://W212-NEC-new
		{
			m_nMode=5;//06款==212new
			m_PortalOP.IROp=IROptions_MOT;
			pWnd = GetDlgItem( IDC_BUTTON24 );     
			pWnd->ShowWindow(TRUE); 
			pWnd = GetDlgItem( IDC_BUTTON25 );     
			pWnd->ShowWindow(TRUE); 
		}
		break;
	case 7://W221-NEC-old
		{
			m_nMode=3;//04款==221old
		}
		break;
	case 8://W221-NEC-new
		{
			m_nMode=6;//07款==221new
			m_PortalOP.IROp=IROptions_MOT;
			pWnd = GetDlgItem( IDC_BUTTON24 );     
			pWnd->ShowWindow(TRUE); 
			pWnd = GetDlgItem( IDC_BUTTON25 );     
			pWnd->ShowWindow(TRUE); 
		}
		break;
	case 9://W246-NEC
		{
			m_nMode=8;//09款==246
		}
		break;
	}
	setOption();
	OnCbnSelchangePortal();
}


void CDlgEIS::OnBnReadData()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,strTemp;
	m_Portal.GetWindowText(str);
	strTemp.LoadString(IDS_PortDlg_IR);
	if(str==strTemp)
	{
		IRRead();
	}
	else
	{
		CANRead();
	}
}

void CDlgEIS::IRRead()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CInteractionData rx;
	rx=CCarKey3App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}
	ClearCtrl();
	strTemp.LoadString(IDS_StartReadData);
	ShowLog(strTemp);
	CInteractionData Result;
	byte* pBuf=CCarKey3App::TheHIDDevice.IRReadData(m_nMode,&m_Progress,&Result,&m_StatusBar);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
		return;
	}		
	m_EZSData.SetAllFile(pBuf);
	SetShowData();

	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	MessageBeep(MB_ICONEXCLAMATION);
	MessageBox(strTemp);
	EnableBotton(TRUE);
	m_bCANRead=false;
	delete[] pBuf;
// 
// 	CString strTemp;
// 	CInteractionData rx;
// 	rx=CCarKey3App::TheHIDDevice.Check_mima();
// 	if(rx.mimaok!=true)
// 	{
// 		CString strx=rx.GetHIDResult();
// 		ShowLog(strx);
// 		MessageBox(strx);
// 		return;
// 	}
// 	ClearCtrl();
// 	strTemp.LoadString(IDS_WaitInsertKey);
// 	ShowLog(strTemp);
// 
// 	//CInteractionData rx;
// 	m_Progress.SetRange(0,40);   //设置进度条的范围为0-1000	
// 	m_Progress.SetPos(40);
// 	m_Progress.UpdateWindow();
// 	CCarKey3App::TheHIDDevice.m_pProgress=&m_Progress;
// 	rx = CCarKey3App::TheHIDDevice.EZSReadDataStep(0);
// 	CString str;
// 	EnableBotton(FALSE);
// 	if(rx.GetHIDResult()==_T(""))
// 	{		
// 		str=CCommFunc::byteToHexStr(rx.buff,0x10,4,_T(" "));
// 		m_SSID.SetContent(str);
// 		m_EZSData.SetKeyStatus(rx.buff[0x10 + 4],rx.buff[0x10 + 6]);
// 		COLORREF clr;
// 		m_EZSData.GetKeyStatus(0,&clr);
// 		m_Key0Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(1,&clr);
// 		m_Key1Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(2,&clr);
// 		m_Key2Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(3,&clr);
// 		m_Key3Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(4,&clr);
// 		m_Key4Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(5,&clr);
// 		m_Key5Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(6,&clr);
// 		m_Key6Status.SetBackgroundColor(clr);
// 		m_EZSData.GetKeyStatus(7,&clr);
// 		m_Key7Status.SetBackgroundColor(clr);
// 		m_EZSData.SetStatus(rx.buff[0x10 + 5]);
// 		byte status=m_EZSData.GetStatus();
// 		if (CCommFunc::GetBit(status, 0))
// 			m_bInitialised = true;
// 		else
// 			m_bInitialised = false;
// 		if (CCommFunc::GetBit(status, 1))
// 			m_bTPremoved = true;
// 		else
// 			m_bTPremoved = false;
// 		if (CCommFunc::GetBit(status, 2))
// 			m_bPersonalised = true;
// 		else
// 			m_bPersonalised = false;
// 		if (CCommFunc::GetBit(status, 3))
// 			m_bActivated = true;
// 		else
// 			m_bActivated = false;
// 		UpdateData(FALSE);
// 		byte b=rx.buff[0x10 + 7]&0x0f;
// 		if(b>7)
// 			m_LastKey.Clear();
// 		else
// 		{
// 			str.Format(_T("%d"),b);
// 			m_LastKey.SetContent(str);
// 		}
// 		b=rx.buff[0x10 + 7]>>4;
// 		b=b&0x0f;
// 		if(b>7)
// 			m_PreviousKey.Clear();
// 		else
// 		{
// 			str.Format(_T("%d"),b);
// 			m_PreviousKey.SetContent(str);	
// 		}
// 	}
// 	else
// 	{
// 		ShowLog(rx.GetHIDResult());
// 		EnableBotton(TRUE);
// 		return;
// 	}
// 	WaitMS(1);
// 	CCarKey3App::TheHIDDevice.m_pProgress=NULL;
// 	strTemp.LoadString(IDS_ReadDating);
// 	ShowLog(strTemp);
// 	m_Progress.SetRange(0,12);   //设置进度条的范围为0-1000	
// 	m_Progress.SetPos(1);
// 	m_Progress.UpdateWindow();
// 	//WaitMS(1);
// 	for (int i=1;i<12;i++)
// 	{
// 		int step;
// 		if(m_PortalOP.IROp==IROptions_NEC)
// 			step=CCarKey3App::TheHIDDevice.DTnec[i];
// 		else 
// 			step=CCarKey3App::TheHIDDevice.DTmot[i];
// 		rx = CCarKey3App::TheHIDDevice.EZSReadDataStep(step);
// 		if(rx.GetHIDResult()==_T(""))
// 		{
// 			m_Progress.StepIt();
// 			m_Progress.UpdateWindow();
// 			switch (i)
// 			{                        
// 			case 1:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key0.SetContent(str);
// 				break;
// 			case 2:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key1.SetContent(str);
// 				break;
// 			case 3:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key2.SetContent(str);
// 				break;
// 			case 4:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key3.SetContent(str);
// 				break;
// 			case 5:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key4.SetContent(str);
// 				break;
// 			case 6:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key5.SetContent(str);
// 				break;
// 			case 7:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key6.SetContent(str);
// 				break;
// 			case 8:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Key7.SetContent(str);
// 				break;
// 			case 9:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_SerialNum.SetContent(str);
// 				break;
// 			case 10:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Dealer.SetContent(str);
// 				break;
// 			case 11:
// 				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
// 				m_Transport.SetContent(str);
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			ShowLog(rx.GetHIDResult());
// 			EnableBotton(TRUE);
// 			return;
// 		}
// 		WaitMS(1);
// 	}
// 	m_Progress.SetPos(12);
// 	strTemp.LoadString(IDS_FinishReadData);
// 	ShowLog(strTemp);
// 	EnableBotton(TRUE);
}

void CDlgEIS::CANRead()
{
	CString strTemp;
	CInteractionData rx;
	rx=CCarKey3App::TheHIDDevice.Check_mima();	
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}	
	ClearCtrl();	
	strTemp.LoadString(IDS_StartReadData);
	ShowLog(strTemp);
	CInteractionData Result;	
	byte* pBuf=CCarKey3App::TheHIDDevice.CANReadEEPROM(m_nMode,&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
		return;
	}
	
	m_bReaded=true;
	m_EZSData.SetAllFile(pBuf);
	SetShowData();

	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	MessageBeep(MB_ICONEXCLAMATION);
	MessageBox(strTemp);
	EnableBotton(TRUE);	
	delete[] pBuf;
	m_bCANRead=true;
}

void CDlgEIS::ShowLog(CString str)
{
	m_StatusBar.SetText(str, 0, 0);
}

void CDlgEIS::ClearCtrl()
{
 	m_bInitialised=FALSE;
 	m_bPersonalised=FALSE;
 	m_bActivated=FALSE;
 	m_bTPremoved=FALSE;
 	UpdateData(FALSE);
 	this->UpdateWindow();
	m_SSID.Clear();
	m_Odo.Clear();
	m_PSW.Clear();
	m_SerialNum.Clear();
	m_Dealer.Clear();
	m_Transport.Clear();
 	m_VIN.Clear();
 	m_Key0.Clear();
 	m_Key1.Clear();
 	m_Key2.Clear();
 	m_Key3.Clear();
 	m_Key4.Clear();
 	m_Key5.Clear();
 	m_Key6.Clear();
 	m_Key7.Clear();
 	m_PreviousKey.Clear();
 	m_LastKey.Clear();
	COLORREF clr=RGB(240,240,240);
	m_Key0Status.SetBackgroundColor(clr);
	m_Key1Status.SetBackgroundColor(clr);
	m_Key2Status.SetBackgroundColor(clr);
	m_Key3Status.SetBackgroundColor(clr);
	m_Key4Status.SetBackgroundColor(clr);
	m_Key5Status.SetBackgroundColor(clr);
	m_Key6Status.SetBackgroundColor(clr);
	m_Key7Status.SetBackgroundColor(clr);
	this->UpdateWindow();
	WaitMS(100);
}

void CDlgEIS::SetShowData()
{
	m_SSID.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0, 4, _T(" ")));//(ezs.SSID, " ");
	//textBox2.Text = "";//CCommFunc::byteToHexStr(outBuf, 20, 8, _T(" "));//(ezs.PSW, " ");
	m_Key0.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x0270, 8, _T(" ")));//(ezs.Key0, " ");
	m_Key1.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x0280, 8, _T(" ")));//(ezs.Key1, " ");
	m_Key2.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x0290, 8, _T(" ")));//(ezs.Key2, " ");
	m_Key3.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x02a0, 8, _T(" ")));//(ezs.Key3, " ");
	m_Key4.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x02b0, 8, _T(" ")));//(ezs.Key4, " ");
	m_Key5.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x02c0, 8, _T(" ")));//(ezs.Key5, " ");
	m_Key6.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x02d0, 8, _T(" ")));//(ezs.Key6, " ");
	m_Key7.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x02e0, 8, _T(" ")));//(ezs.Key7, " ");
	m_SerialNum.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x230, 8, _T(" ")));//(ezs.SerialNum, " ");
	m_Dealer.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x10, 8, _T(" ")));//(ezs.Dealer, " ");
	m_Transport.SetContent(CCommFunc::byteToHexStr(m_EZSData.AllFile, 0x260, 8, _T(" ")));//(ezs.Transport, " ");
	if(CCarKey3App::m_bPass)
	{
		bool bSame=true;
		for(int i=0;i<4;i++)
		{
			if(CCarKey3App::PassSSID[i]!=m_EZSData.AllFile[i])
			{
				bSame=false;
				break;
			}
		}		
		if(bSame)
		{
			m_PSW.SetContent(CCommFunc::byteToHexStr(CCarKey3App::HPassword,8,_T(" ")));
		}		
	}
	CString str;
	COLORREF clr;
	m_EZSData.GetKeyStatus(0,&clr);
	m_Key0Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(1,&clr);
	m_Key1Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(2,&clr);
	m_Key2Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(3,&clr);
	m_Key3Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(4,&clr);
	m_Key4Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(5,&clr);
	m_Key5Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(6,&clr);
	m_Key6Status.SetBackgroundColor(clr);
	m_EZSData.GetKeyStatus(7,&clr);
	m_Key7Status.SetBackgroundColor(clr);

	byte status=m_EZSData.GetStatus();
	if (CCommFunc::GetBit(status, 0))
		m_bInitialised = true;
	else
		m_bInitialised = false;
	if (CCommFunc::GetBit(status, 1))
		m_bTPremoved = true;
	else
		m_bTPremoved = false;
	if (CCommFunc::GetBit(status, 2))
		m_bPersonalised = true;
	else
		m_bPersonalised = false;
	if (CCommFunc::GetBit(status, 3))
		m_bActivated = true;
	else
		m_bActivated = false;
	UpdateData(FALSE);

	m_LastKey.SetContent(m_EZSData.GetLastKey());
	m_PreviousKey.SetContent(m_EZSData.GetPreviousKey());	
	str=m_EZSData.GetVIN();	
	m_VIN.SetContent(str);
	CWnd *pWnd = GetDlgItem( IDC_BUTTON19 ); 
	if(str!=_T(""))
	{    
		pWnd->EnableWindow(TRUE); 
	}
	else
	{
		pWnd->EnableWindow(FALSE); 
	}
}

void CDlgEIS::EnableBotton( BOOL bEn )
{
	CWnd *pWnd;
	pWnd = GetDlgItem( IDC_BUTTON9 );     
	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON11 );     
	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON13 );     
	pWnd->EnableWindow(bEn); 
// 	pWnd = GetDlgItem( IDC_BUTTON18 );     
// 	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON24 );     
	pWnd->EnableWindow(bEn); 
// 	pWnd = GetDlgItem( IDC_BUTTON19 );     
// 	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON17 );     
	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON12 );     
	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON16 );     
	pWnd->EnableWindow(bEn); 
	pWnd = GetDlgItem( IDC_BUTTON25 );     
	pWnd->EnableWindow(bEn); 

	pWnd = GetDlgItem( IDC_COMBO4 );     
	pWnd->EnableWindow(bEn); 

	pWnd = GetDlgItem( IDC_COMBO1 );     
	pWnd->EnableWindow(bEn);

	pWnd = GetDlgItem( IDC_COMBO2 );     
	pWnd->EnableWindow(bEn);

	pWnd = GetDlgItem( IDC_COMBO3);     
	pWnd->EnableWindow(bEn);
	if(bEn)
		OnCbnSelchangePortal();
}


void CDlgEIS::OnBnReadInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码	
	CString strTemp,strTemp1,strInfo;
	CInteractionData rx;
	rx=CCarKey3App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}
	strTemp.LoadString(IDS_StartCANInfo);
	ShowLog(strTemp);
	CInteractionData Result;
	byte* pBuf=CCarKey3App::TheHIDDevice.CANReadInfoData(m_nMode,&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
		return;
	}
	MessageBeep(MB_ICONEXCLAMATION);
	CCANInfo dlgCanInfo;
	dlgCanInfo.pBuf=pBuf;
	dlgCanInfo.DoModal();
	delete[] pBuf;
}


void CDlgEIS::OnCbnSelchangePortal()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,strTemp;
	m_Portal.GetWindowText(str);
	strTemp.LoadString(IDS_PortDlg_IR);
	CWnd *pWnd;	
	if(str==strTemp)
	{
		pWnd = GetDlgItem( IDC_BUTTON11 );     
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		pWnd = GetDlgItem( IDC_BUTTON11 );     
		pWnd->EnableWindow(TRUE);
	}
}


void CDlgEIS::OnBnCANWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckData()!=true)
		return;
	CString str,strTemp,strTemp1;
	strTemp.LoadString(IDS_CANWriteWarring);
	strTemp1.LoadString(IDS_Warring);
	if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
		return;
	m_PSW.GetWindowText(str);
	if(!m_EZSData.SetPSW(str))
	{
		strTemp1.LoadString(IDS_Warring);
		strTemp.LoadString(IDS_PSWAreaErr);
		if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
			return;
		m_EZSData.SetPSW(_T("01 23 45 67 89 ab cd ef"));
	}
	else if(m_EZSData.AllFile[0x8e] == 0 && m_EZSData.AllFile[0x8e+1] == 0 && m_EZSData.AllFile[0x8e+2] == 0 && m_EZSData.AllFile[0x8e+3] == 0 && m_EZSData.AllFile[0x8e+4] == 0 && m_EZSData.AllFile[0x8e+5] == 0 && m_EZSData.AllFile[0x8e+6] == 0 && m_EZSData.AllFile[0x8e+7] == 0)
	{
		strTemp.LoadString(IDS_ErrPSWWarring);
		strTemp1.LoadString(IDS_Warring);
		if (MessageBox(strTemp,strTemp1, MB_YESNO) == IDNO)
			return;
		strTemp.LoadString(IDS_ErrPSWLog);
		ShowLog(strTemp);
	}
	strTemp.LoadString(IDS_StartCANWrite);
	ShowLog(strTemp);
	CInteractionData Result;
	CCarKey3App::TheHIDDevice.CANWriteEZS(m_nMode,&m_Progress,&Result,&m_StatusBar,&m_EZSData);
	if(Result.GetHIDResult()==_T(""))
	{
		strTemp.LoadString(IDS_FinishCANWrite);
		ShowLog(strTemp);
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(strTemp);
	}
	else
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
	}
}

bool CDlgEIS::CheckData()
{
	CString str,strTemp,strTemp1,strTemp2;
	//m_EZSData.Clear();
	m_SSID.GetWindowText(str);
	strTemp.LoadString(IDS_ErrDataArea);
	if(!m_EZSData.SetSSID(str))
	{
		strTemp2.LoadString(IDS_SSID);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_SerialNum.GetWindowText(str);
	if(!m_EZSData.SetSerialNum(str))
	{
		strTemp2.LoadString(IDS_SerialNum);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	CPassCalc cpc;
	for(int i=0;i<8;i++)
		cpc.HashIn[i]=m_EZSData.DPSW[i];
	cpc.sub_HashCalc(101);
	bool bRight=true;
	for(int i=0;i<8;i++)
	{
		if(cpc.HashOut[i]!=m_EZSData.AllFile[0x10+i])
		{
			bRight=false;
			break;
		}
	}
	if(!bRight)
	{
		m_Dealer.GetWindowText(str);
		if(!m_EZSData.SetDealer(str))
		{
			strTemp2.LoadString(IDS_Dealer);
			MessageBox(strTemp2+strTemp);
			return false;
		}
	}
	m_Transport.GetWindowText(str);
	if(!m_EZSData.SetTransport(str))
	{
		strTemp2.LoadString(IDS_Transport);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_Key0.GetWindowText(str);
	strTemp1.LoadString(IDS_Key);
	if(!m_EZSData.SetKey0(str))
	{
		MessageBox(strTemp1+_T("0")+strTemp);
		return false;
	}
	m_Key1.GetWindowText(str);
	if(!m_EZSData.SetKey1(str))
	{
		MessageBox(strTemp1+_T("1")+strTemp);
		return false;
	}
	m_Key2.GetWindowText(str);
	if(!m_EZSData.SetKey2(str))
	{
		MessageBox(strTemp1+_T("2")+strTemp);
		return false;
	}
	m_Key3.GetWindowText(str);
	if(!m_EZSData.SetKey3(str))
	{
		MessageBox(strTemp1+_T("3")+strTemp);
		return false;
	}
	m_Key4.GetWindowText(str);
	if(!m_EZSData.SetKey4(str))
	{
		MessageBox(strTemp1+_T("4")+strTemp);
		return false;
	}
	m_Key5.GetWindowText(str);
	if(!m_EZSData.SetKey5(str))
	{
		MessageBox(strTemp1+_T("5")+strTemp);
		return false;
	}
	m_Key6.GetWindowText(str);
	if(!m_EZSData.SetKey6(str))
	{
		MessageBox(strTemp1+_T("6")+strTemp);
		return false;
	}
	m_Key7.GetWindowText(str);
	if(!m_EZSData.SetKey7(str))
	{
		MessageBox(strTemp1+_T("7")+strTemp);
		return false;
	}
// 	m_LastKey.GetWindowText(str);
// 	if(!m_EZSData.SetLastKey(str))
// 	{
// 		strTemp2.LoadString(IDS_LastKey);
// 		MessageBox(strTemp2+strTemp);
// 		return false;
// 	}
// 	m_PreviousKey.GetWindowText(str);
// 	if(!m_EZSData.SetPreviousKey(str))
// 	{
// 		strTemp2.LoadString(IDS_PreviousKey);
// 		MessageBox(strTemp2+strTemp);
// 		return false;
// 	}
	return true;
}


void CDlgEIS::OnBnLoadPSW()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString StrName,strTemp;
	unsigned int dwFileLen;
	CFileDialog fileDlg(TRUE);
	strTemp.LoadString(IDS_PSWLoadTitle);
	fileDlg.m_ofn.lpstrTitle=strTemp;
	fileDlg.m_ofn.lpstrFilter=_T("PSWFiles(*.PSW)\0*.PSW\0All Files(*.*)\0*.*\0\0");
	if (IDOK!=fileDlg.DoModal())
		return;
	CFile file(fileDlg.GetPathName(),CFile::modeRead);
	StrName=fileDlg.GetPathName();			//file path
	dwFileLen=(unsigned int)file.GetLength();	
	byte* pLoadBuf=new byte[dwFileLen+1];
	pLoadBuf[dwFileLen]=0;
	file.Read(pLoadBuf,dwFileLen);
	file.Close();
	if (dwFileLen == 0xa0)
	{
		CString str;
		str = CCommFunc::byteToHexStr(pLoadBuf, 0, 8, _T(" "));
		m_PSW.SetContent(str);
		strTemp.LoadString(IDS_PSWLoadOK);
		ShowLog(strTemp);
		delete[] pLoadBuf;
		pLoadBuf=NULL;
	}                       
	else
	{
		delete[] pLoadBuf;
		pLoadBuf=NULL;
		strTemp.LoadString(IDS_PSWLoadErr);
		MessageBox(strTemp);  
	}
}


void CDlgEIS::OnBnSaveSKC()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str,strTemp,strTemp1;
	if(CheckData()!=true)
		return;
	m_PSW.GetWindowText(str);
	if(!m_EZSData.SetPSW(str))
	{
		strTemp.LoadString(IDS_KeyData12);
		MessageBox(strTemp);
		return;
	}
	if(m_EZSData.AllFile[4]==0xff)
	{
		strTemp.LoadString(IDS_KeyData13);
		MessageBox(strTemp);
		return;
	}
	CDlgKeyData dlgKeyData;
	for(int i=0;i<0x400;i++)
		dlgKeyData.pBIN[7+i]=m_EZSData.AllFile[i];
	for(int i=0;i<8;i++)
		dlgKeyData.pBIN[0x3f8+i+7]=m_EZSData.PSW[i];
	dlgKeyData.DoModal();
	return;
	if(CheckData()!=true)
		return;
	//m_EZSData.SetVerify();
	//CString str,strTemp,strTemp1;
	CFileDialog fileDlgS(FALSE);
	m_PSW.GetWindowText(str);
	if(!m_EZSData.SetPSW(str))
	{
		strTemp.LoadString(IDS_ErrPSWWarring);
		strTemp1.LoadString(IDS_Warring);
		if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
			return;
		fileDlgS.m_ofn.lpstrFilter=_T("EZS Files(*.EZS)\0*.EZS\0BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
		fileDlgS.m_ofn.lpstrDefExt=_T("EZS");
	}
	else
	{
		fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0EZS Files(*.EZS)\0*.EZS\0All Files(*.*)\0*.*");
		fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
	}
	strTemp.LoadString(IDS_SaveFileTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;
	m_SSID.GetWindowText(str);
	str.Replace(_T(" "),_T(""));
	CString strFileName=str+_T("_ST12");
	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
	if (IDOK==fileDlgS.DoModal())
	{		
		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		byte* save=m_EZSData.GetSKCData();
		file.Write(save,0x800);		
		file.Close();
		strTemp.LoadString(IDS_SaveFileOK);
		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
		delete[] save;
	}	
	strFileName.ReleaseBuffer();	
}


void CDlgEIS::OnBnEISTest()
{
	CEISTest eisTest;
	eisTest.m_nMode=m_nMode;
	eisTest.DoModal();
}


void CDlgEIS::OnDealerEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	bool bRenew=false;
	m_Dealer.GetWindowText(str);
	int len;
// 	byte* p=NULL;
// 	if(str!=_T("") && str.GetLength()==23)
// 	{
// 		
// 	}
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		bRenew=false;
	else if(len!=8)
		bRenew=false;
	else
		bRenew=true;
	if(bRenew)
	{		
		bool bSame=true;
		for(int i=0;i<8;i++)
		{
			if(p[i]!=m_EZSData.AllFile[0x10+i])
			{
				bSame=false;
				break;
			}
		}
		if(bSame)
			bRenew=false;
		else
		{
			CPassCalc cpc;
			for(int i=0;i<8;i++)
				cpc.HashIn[i]=p[i];
			cpc.sub_HashCalc(101);
			bool bRight=true;
			for(int i=0;i<8;i++)
			{
				if(cpc.HashOut[i]!=m_EZSData.AllFile[0x10+i])
				{
					bRight=false;
					break;
				}
			}
			if(!bRight)
				bRenew=false;
			else
			{
				for(int i=0;i<8;i++)
					m_EZSData.DPSW[i]=p[i];
			}
		}
	}
	//if(p!=NULL)
	delete[] p;
	CString strTemp;
	if (m_bReaded)
	{
		if(bRenew)
		{
			strTemp.LoadString(IDS_EZSRenew);
			GetDlgItem(IDC_BUTTON18)->EnableWindow(TRUE);
		}
		else
		{
			strTemp.LoadString(IDS_RenewPassCalc);
			GetDlgItem(IDC_BUTTON18)->EnableWindow(TRUE);
		}
	}
	else
	{
		strTemp.LoadString(IDS_RenewPassCalc);
		GetDlgItem(IDC_BUTTON18)->EnableWindow(FALSE);
	}
	SetDlgItemText(IDC_BUTTON18,strTemp);
}


void CDlgEIS::OnBnReadMillsClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Dealer.SetWindowText(_T("00 01 02 03 04 05 06 07"));
}


void CDlgEIS::OnBnEZSRenewClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,str,strTemp1;
	strTemp.LoadString(IDS_EZSRenew);
	GetDlgItem(IDC_BUTTON18)->GetWindowText(str);
	if(str==strTemp)//renew
	{
		if(CheckData()!=true)
			return;
		CString str,strTemp,strTemp1;
		strTemp.LoadString(IDS_RenewWarring);
		strTemp1.LoadString(IDS_Warring);
		if (MessageBoxEx(this->m_hWnd,strTemp, strTemp1, MB_YESNO,CCarKey3App::LANGID) == IDNO)
			return;
		m_PSW.GetWindowText(str);
		if(!m_EZSData.SetPSW(str))
		{
			strTemp1.LoadString(IDS_Warring);
			strTemp.LoadString(IDS_PSWAreaErr);
			if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
				return;
			m_EZSData.SetPSW(_T("01 23 45 67 89 ab cd ef"));
			//m_PSW.SetContent(_T("01"));
			// 		ShowLog(strTemp);
			// 		MessageBox(strTemp);
			// 		return;
		}
		else if(m_EZSData.PSW[0] == 0 && m_EZSData.PSW[0+1] == 0 && m_EZSData.PSW[0+2] == 0 && m_EZSData.PSW[0+3] == 0 && m_EZSData.PSW[0+4] == 0 && m_EZSData.PSW[0+5] == 0 && m_EZSData.PSW[0+6] == 0 && m_EZSData.PSW[0+7] == 0)
		{
			strTemp.LoadString(IDS_ErrPSWWarring);
			strTemp1.LoadString(IDS_Warring);
			if (MessageBox(strTemp,strTemp1, MB_YESNO) == IDNO)
				return;
			strTemp.LoadString(IDS_ErrPSWLog);
			ShowLog(strTemp);
		}
		strTemp.LoadString(IDS_StartRenew);
		ShowLog(strTemp);
		CInteractionData Result;
		CCarKey3App::TheHIDDevice.RenewEZS(m_nMode,&m_Progress,&Result,&m_StatusBar,m_EZSData.AllFile,m_EZSData.DPSW,m_EZSData.PSW);
		if(Result.GetHIDResult()==_T(""))
		{
			strTemp.LoadString(IDS_FinishEZSRenew);
			ShowLog(strTemp);
			MessageBeep(MB_ICONEXCLAMATION);
			MessageBox(strTemp);
		}
		else
		{
			ShowLog(Result.GetHIDResult());
			MessageBox(Result.GetHIDResult());
		}
	}
	else//获得密码
	{
// 		if(CCarKey3App::withServer.m_bOnline)
// 		{
			strTemp.LoadString(IDS_PSWStop);
			GetDlgItem(IDC_BUTTON18)->GetWindowText(str);
			if(str==strTemp)
			{
				strTemp.LoadString(IDS_PSWTip12);
				strTemp1.LoadString(IDS_Warring);
				if(MessageBox(strTemp, strTemp1,MB_YESNO) == IDYES)
				{
					GetDlgItem(IDC_BUTTON18)->EnableWindow(FALSE);
					strTemp.LoadString(IDS_ServerTip8);
					ShowLog(strTemp);
				}
			}
			else
			{
				m_Progress.SetPos(0);
				strTemp.LoadString(IDS_PSWStop);
				GetDlgItem(IDC_BUTTON18)->SetWindowText(strTemp);
				g_bDPassThreadRunning=true;
				g_bDPassThreadExit=false;
				EnableBotton(FALSE);
				strTemp.LoadString(IDS_ServerTip9);
				ShowLog(strTemp);
				CDPassThreadParam* tParam=new CDPassThreadParam();
				tParam->hWnd=this->m_hWnd;
				tParam->pDealer=&(m_EZSData.AllFile[0x10]);
				tParam->pDPassResultProc=EIS_DPassResultProc;
				tParam->pProgress=&m_Progress;
				tParam->pStatusBar=&m_StatusBar;
				tParam->pSSID=m_EZSData.AllFile;
				tParam->dlg=this;
				_beginthread(DPassThread,0,(void *)(tParam));
			}
// 		}
// 		else
// 			MessageBox(_T("没法获得擦除密码"));
	}
}


void CDlgEIS::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString StrName,strTemp,strTemp1;
	CString str;
	unsigned int dwFileLen;
	CFileDialog fileDlg(TRUE);
	strTemp.LoadString(IDS_OpenFileTitle);
	strTemp1.Format(strTemp,_T("EIS"));
	fileDlg.m_ofn.lpstrTitle=strTemp1;
	fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	if (IDOK!=fileDlg.DoModal())
		return;
	CFile file(fileDlg.GetPathName(),CFile::modeRead);
	StrName=fileDlg.GetPathName();			//file path
	dwFileLen=(unsigned int)file.GetLength();	
	byte* pLoadBuf=new byte[dwFileLen+1];
	pLoadBuf[dwFileLen]=0;
	file.Read(pLoadBuf,dwFileLen);
	file.Close();
	if (dwFileLen == 0x400)//红外数据文件
	{
		m_bReaded=false;
		m_EZSData.SetAllFile(pLoadBuf);		
		m_SSID.SetContent(m_EZSData.GetSSID());
		// 		str=m_EZSData.GetPSW();
		// 		if(str!=_T("00 00 00 00 00 00 00 00"))
		// 			m_PSW.SetContent(m_EZSData.GetPSW());
		// 		else
		m_PSW.Clear();
		if(CCarKey3App::m_bPass)
		{
			bool bSame=true;
			for(int i=0;i<4;i++)
			{
				if(CCarKey3App::PassSSID[i]!=m_EZSData.AllFile[i])
				{
					bSame=false;
					break;
				}
			}		
			if(bSame)
			{
				m_PSW.SetContent(CCommFunc::byteToHexStr(CCarKey3App::HPassword,8,_T(" ")));
			}		
		}
		m_Dealer.SetContent(m_EZSData.GetDealer());
		m_Key0.SetContent(m_EZSData.GetKey0());
		m_Key1.SetContent(m_EZSData.GetKey1());
		m_Key2.SetContent(m_EZSData.GetKey2());
		m_Key3.SetContent(m_EZSData.GetKey3());
		m_Key4.SetContent(m_EZSData.GetKey4());
		m_Key5.SetContent(m_EZSData.GetKey5());
		m_Key6.SetContent(m_EZSData.GetKey6());
		m_Key7.SetContent(m_EZSData.GetKey7());
		m_Transport.SetContent(m_EZSData.GetTransport());
		m_SerialNum.SetContent(m_EZSData.GetSerialNum());		
		//m_EZSData.SetKeyStatus(0,0xff);
		COLORREF clr;
		m_EZSData.GetKeyStatus(0,&clr);
		m_Key0Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(1,&clr);
		m_Key1Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(2,&clr);
		m_Key2Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(3,&clr);
		m_Key3Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(4,&clr);
		m_Key4Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(5,&clr);
		m_Key5Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(6,&clr);
		m_Key6Status.SetBackgroundColor(clr);
		m_EZSData.GetKeyStatus(7,&clr);
		m_Key7Status.SetBackgroundColor(clr);
		byte status=m_EZSData.GetStatus();
		if (CCommFunc::GetBit(status, 0))
			m_bInitialised = true;
		else
			m_bInitialised = false;
		if (CCommFunc::GetBit(status, 1))
			m_bTPremoved = true;
		else
			m_bTPremoved = false;
		if (CCommFunc::GetBit(status, 2))
			m_bPersonalised = true;
		else
			m_bPersonalised = false;
		if (CCommFunc::GetBit(status, 3))
			m_bActivated = true;
		else
			m_bActivated = false;
		UpdateData(FALSE);
		m_LastKey.SetContent(m_EZSData.GetLastKey());
		m_PreviousKey.SetContent(m_EZSData.GetPreviousKey());

		strTemp.LoadString(IDS_OpenFileOK);
		ShowLog(strTemp+_T(":")+StrName);
		str=m_EZSData.GetVIN();	
		m_VIN.SetContent(str);
		CWnd *pWnd = GetDlgItem( IDC_BUTTON19 ); 
		if(str!=_T(""))
		{    
			pWnd->EnableWindow(TRUE); 
		}
		else
		{
			pWnd->EnableWindow(FALSE); 
		}
	}  
}


void CDlgEIS::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if(CheckData()!=true)
		return;
	//m_EZSData.SetVerify();
	CString str,strTemp,strTemp1;
	CFileDialog fileDlgS(FALSE);
	// 	m_PSW.GetWindowText(str);
	// 	if(!m_EZSData.SetPSW(str))
	// 	{
	// 		strTemp.LoadString(IDS_ErrPSWWarring);
	// 		strTemp1.LoadString(IDS_Warring);
	// 		if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
	// 			return;
	// 		fileDlgS.m_ofn.lpstrFilter=_T("EZS Files(*.EZS)\0*.EZS\0BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	// 		fileDlgS.m_ofn.lpstrDefExt=_T("EZS");
	// 	}
	// 	else
	// 	{
	fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0EZS Files(*.EZS)\0*.EZS\0All Files(*.*)\0*.*");
	fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
	// 	}
	strTemp.LoadString(IDS_SaveFileTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;
	m_SSID.GetWindowText(str);
	str.Replace(_T(" "),_T(""));
	CString strFileName;//=str+_T("_EIS");
	if(m_bCANRead)
	{
		strFileName=str+_T("_CAN_EIS");
	}
	else
	{
		strFileName=str+_T("_IR_EIS");
	}
	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
	if (IDOK==fileDlgS.DoModal())
	{		
		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		file.Write(m_EZSData.AllFile,0x400);
		file.Close();
		strTemp.LoadString(IDS_SaveFileOK);
		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
	}	
	strFileName.ReleaseBuffer();
}


void CDlgEIS::OnCanedit()
{
	CToolBarCtrl &tbc = m_ToolBar.GetToolBarCtrl();
	tbc.SetState(ID_CANEdit,TBSTATE_HIDDEN);
	tbc.SetState(ID_DisEdit,TBSTATE_ENABLED);
	m_Key0.SetReadOnly(FALSE);
	m_Key1.SetReadOnly(FALSE);
	m_Key2.SetReadOnly(FALSE);
	m_Key3.SetReadOnly(FALSE);
	m_Key4.SetReadOnly(FALSE);
	m_Key5.SetReadOnly(FALSE);
	m_Key6.SetReadOnly(FALSE);
	m_Key7.SetReadOnly(FALSE);
	m_SSID.SetReadOnly(FALSE);
	m_Odo.SetReadOnly(FALSE);
	m_PSW.SetReadOnly(FALSE);
	m_SerialNum.SetReadOnly(FALSE);
	m_Transport.SetReadOnly(FALSE);
	m_Dealer.SetReadOnly(FALSE);
	m_VIN.SetReadOnly(FALSE);
	// TODO: 在此添加命令处理程序代码
}


void CDlgEIS::OnDisedit()
{
	CToolBarCtrl &tbc = m_ToolBar.GetToolBarCtrl();
	tbc.SetState(ID_DisEdit,TBSTATE_HIDDEN);
	tbc.SetState(ID_CANEdit,TBSTATE_ENABLED);
	m_Key0.SetReadOnly(TRUE);
	m_Key1.SetReadOnly(TRUE);
	m_Key2.SetReadOnly(TRUE);
	m_Key3.SetReadOnly(TRUE);
	m_Key4.SetReadOnly(TRUE);
	m_Key5.SetReadOnly(TRUE);
	m_Key6.SetReadOnly(TRUE);
	m_Key7.SetReadOnly(TRUE);
	m_SSID.SetReadOnly(TRUE);
	m_Odo.SetReadOnly(TRUE);
	m_PSW.SetReadOnly(TRUE);
	m_SerialNum.SetReadOnly(TRUE);
	m_Transport.SetReadOnly(TRUE);
	m_Dealer.SetReadOnly(TRUE);
	m_VIN.SetReadOnly(TRUE);

	// TODO: 在此添加命令处理程序代码
}


void CDlgEIS::OnBnWriteVIN()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CInteractionData rx;
	rx=CCarKey3App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}
	strTemp.LoadString(IDS_StartCANWrite);
	ShowLog(strTemp);
	CInteractionData Result;
	CCarKey3App::TheHIDDevice.WriteVIN(m_nMode,m_EZSData.AllFile,&Result,&m_Progress);
	if(Result.GetHIDResult()==_T(""))
	{
		strTemp.LoadString(IDS_FinishCANWrite);
		ShowLog(strTemp);
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(strTemp);
	}
	else
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
	}
}


void CDlgEIS::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(g_bDPassThreadRunning && !g_bDPassThreadExit)
		return;
	CDialogEx::OnClose();
}
