// DlgNEC_CHIP.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgNEC_CHIP.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"


// CDlgNEC_CHIP 对话框

IMPLEMENT_DYNAMIC(CDlgNEC_CHIP, CDialogEx)

CDlgNEC_CHIP::CDlgNEC_CHIP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgNEC_CHIP::IDD, pParent)
{

}

CDlgNEC_CHIP::~CDlgNEC_CHIP()
{
}

void CDlgNEC_CHIP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_EDIT12, m_wPWD);
	DDX_Control(pDX, IDC_EDIT18, m_wKeyNum);
	DDX_Control(pDX, IDC_EDIT17, m_wKeyID);
}


BEGIN_MESSAGE_MAP(CDlgNEC_CHIP, CDialogEx)
	ON_WM_TIMER()
	ON_COMMAND(ID_BUTTON_Clear, &CDlgNEC_CHIP::OnButtonClear)
	ON_COMMAND(ID_BUTTON_Bin, &CDlgNEC_CHIP::OnButtonBin)

	ON_BN_CLICKED(IDC_BUTTON50, &CDlgNEC_CHIP::OnBnReadKeyData)
	ON_BN_CLICKED(IDC_BUTTON51, &CDlgNEC_CHIP::OnBnWrite)
	//ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT12, &CDlgNEC_CHIP::OnEnChangeEdit12)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgNEC_CHIP::OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgNEC_CHIP::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgNEC_CHIP 消息处理程序


BOOL CDlgNEC_CHIP::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Progress.SetStep(1);
	CString str;
	
	//m_ModelType.AddString(_T("W221-NEC-new"));	
	//m_ModelType.AddString(_T("W246-NEC"));
	// TODO:  在此添加额外的初始化
 	if(!m_ToolBar.CreateEx( this,TBSTYLE_FLAT , WS_CHILD | WS_VISIBLE |CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS,
 		CRect(4,4,0,0))||!m_ToolBar.LoadToolBar(IDR_TOOLBAR1) )
 	{
 		TRACE0("failedto create toolbar\n");
 		return FALSE;
 	}
 	m_ToolBar.ShowWindow(SW_SHOW);
 	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);	

	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);

	int strPartDim[2]= {200,-1}; //分割数量
	m_StatusBar.SetParts(2, strPartDim);
	//设置状态栏文本
	m_StatusBar.SetText(_T("123213213123"), 0, 0);
	m_StatusBar.SetText(_T("12321312321321"), 1, 0);
	//下面是在状态栏中加入图标
	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标

	m_hUsbEventHandle = CCarKey3App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
	m_Progress.SetStep(1);



	CString strTemp;
// 	strTemp.LoadString(IDS_ModleType);
// 	SetDlgItemText(IDC_STATIC24,_T("Key")+strTemp); 
	strTemp.LoadString(IDS_KeyModule);
	SetWindowText(strTemp);
	strTemp.LoadString(IDS_Options_IR);
	SetDlgItemText(IDC_RADIO1,strTemp); 
	strTemp.LoadString(IDS_Options_OnPCB);
	SetDlgItemText(IDC_RADIO2,strTemp); 
	/************************************************************************/
	/* 按键                                                                 */
	/************************************************************************/
	strTemp.LoadString(IDS_KeyDlg_ReadKey);
	SetDlgItemText(IDC_BUTTON50,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_Write);
	SetDlgItemText(IDC_BUTTON51,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_Renew);
	SetDlgItemText(IDC_BUTTON1,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_ReadPSW);
	SetDlgItemText(IDC_BUTTON52,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_SavePSW);
	SetDlgItemText(IDC_BUTTON53,strTemp); 

	/************************************************************************/
	/* 静态控件                                                             */
	/************************************************************************/
	strTemp.LoadString(IDS_KeyDlg_Log);
	SetDlgItemText(IDC_STATIC11,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_ReadData);
	SetDlgItemText(IDC_STATIC10,strTemp);

	strTemp.LoadString(IDS_KeyDlg_WriteData);
	SetDlgItemText(IDC_STATIC8,strTemp);

	strTemp.LoadString(IDS_KeyDlg_Option);
	SetDlgItemText(IDC_STATIC30,strTemp);

	strTemp.LoadString(IDS_KeyDlg_KeyID);
	SetDlgItemText(IDC_STATIC1,strTemp);

	strTemp.LoadString(IDS_KeyDlg_KeyNum);
	SetDlgItemText(IDC_STATIC2,strTemp);

	strTemp.LoadString(IDS_KeyDlg_Life);
	SetDlgItemText(IDC_STATIC3,strTemp);

	strTemp.LoadString(IDS_KeyDlg_KeyStatus);
	SetDlgItemText(IDC_STATIC4,strTemp);

	strTemp.LoadString(IDS_KeyDlg_Version);
	SetDlgItemText(IDC_STATIC5,strTemp);

	strTemp.LoadString(IDS_KeyDlg_Remark);
	SetDlgItemText(IDC_STATIC6,strTemp);

	strTemp.LoadString(IDS_KeyDlg_KeyID);
	SetDlgItemText(IDC_STATIC9,strTemp);

	strTemp.LoadString(IDS_KeyDlg_KeyNum);
	SetDlgItemText(IDC_STATIC12,strTemp);

	strTemp.LoadString(IDS_KeyDlg_PSW);
	SetDlgItemText(IDC_STATIC13,strTemp);

	strTemp.LoadString(IDS_LifeValue);
	SetDlgItemText(IDC_STATIC7,strTemp);

	SetTimer(1,1000,NULL);//启动定时器1,定时时间是1秒
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgNEC_CHIP::OnButtonClear()
{
	// TODO: 在此添加命令处理程序代码
	
	m_Progress.SetRange(0,1);
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();

	m_wKeyID.Clear();
	m_wKeyNum.Clear();
	m_wPWD.Clear();
	CCarKey3App::TheHIDDevice.bLoadKeyData=false;
	this->UpdateWindow();
	CString strTemp;
	strTemp.LoadString(IDS_ClearOK);
	ShowLog(strTemp);
}


void CDlgNEC_CHIP::OnButtonBin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CString StrName;
	unsigned int dwFileLen;
	CFileDialog fileDlg(TRUE);

	strTemp.LoadString(IDS_OpenKeyFileTitle);
	fileDlg.m_ofn.lpstrTitle=strTemp;

	fileDlg.m_ofn.lpstrFilter=_T("KeyFiles(*.BIN)\0*.BIN\0All Files(*.*)\0*.*\0\0");
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
		str = CCommFunc::byteToHexStr(pLoadBuf, 1, 8, _T(" "));
		m_wPWD.SetContent(str);

		str.Format(_T("%d"),(pLoadBuf[9] & 0x07)+1);
		m_wKeyNum.SetContent(str);

		str=CCommFunc::byteToHexStr(pLoadBuf, 9, 4, _T(" "));
		m_wKeyID.SetContent(str);

		strTemp.LoadString(IDS_Key);
		SetWindowText(strTemp+_T("--")+StrName);

		strTemp.LoadString(IDS_LoadKeyFileOK);
		ShowLog(strTemp);
		ShowLog(StrName);
		//-->逆序预处理
// 		byte Tmp;
// 		for (int i = 0, j = 0x08; i < 4; i++, j--)
// 		{
// 			Tmp = pLoadBuf[i + 0x01];
// 			pLoadBuf[i + 0x01] = pLoadBuf[j];
// 			pLoadBuf[j] = Tmp;
// 		}
// 		for (int i = 0, j = 0x0C; i < 2; i++, j--)
// 		{
// 			Tmp = pLoadBuf[i + 0x09];
// 			pLoadBuf[i + 0x09] = pLoadBuf[j];
// 			pLoadBuf[j] = Tmp;
// 		}
		for(int i=0;i<0xa0;i++)
		{
			CCarKey3App::TheHIDDevice.KeyDataBuf[i]=pLoadBuf[i];			
		}
		CCarKey3App::TheHIDDevice.bLoadKeyData=true;		
		//<--逆序预处理
	}                       
	else
	{
		strTemp.LoadString(IDS_LoadKeyErr);
		ShowLog(strTemp);
		KeyMessageDlg(strTemp); 

		m_wKeyID.Clear();
		m_wKeyNum.Clear();
		m_wPWD.Clear();
		CCarKey3App::TheHIDDevice.bLoadKeyData=false;
	}
	delete[] pLoadBuf;
}


BOOL CDlgNEC_CHIP::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == 0x0219)
	{
		switch (wParam)
		{
		case 0x8000:
			if (!CCarKey3App::TheHIDDevice.MyDeviceDetected)
			{
				CCarKey3App::TheHIDDevice.FindDevice(); 
			}
			HIDUI();
			break;
		case 0x8004:
			// 			try
			// 			{
			CCarKey3App::TheHIDDevice.Close();
			// 			}
			// 			catch (CException* e)
			// 			{
			// 				throw _T("USB 移除出错!!");
			// 			}
			//::SendMessage(this->GetSafeHwnd(),WM_CLOSE,0,0);
			HIDUI();
			break;
		default:
			break;
		}
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CDlgNEC_CHIP::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCarKey3App::TheHIDDevice.UnregisterForUsbEvents(m_hUsbEventHandle);
	CDialogEx::OnClose();
}

void CDlgNEC_CHIP::OnBnReadKeyData()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,str;
	CInteractionData rx;	
	
// 	strTemp.LoadString(IDS_StartReadData);
// 	ShowLog(strTemp);

	CInteractionData Result;
	byte* pBuf=CCarKey3App::TheHIDDevice.KeyData_UartReadIR(&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
		return;
	}
	str = CCommFunc::byteToHexStr(pBuf, 1, 8, _T(" "));
	m_wPWD.SetContent(str);

	str.Format(_T("%d"),(pBuf[9] & 0x07)+1);
	m_wKeyNum.SetContent(str);

	str=CCommFunc::byteToHexStr(pBuf, 9, 4, _T(" "));
	m_wKeyID.SetContent(str);
// 	if(0x55==DTrd[0][0])
// 	{
// 		CString str;
// 		str.Format(_T("%d"),DTrd[0][5] & 0x07);
// 		m_rKeyNum.SetContent(str);
// 		//rx.buff[0x15] = (byte)(rx.buff[0x15] & 0xF8);
// 		DTrd[0][5]=DTrd[0][5]&0xF8;
// 		str=CCommFunc::byteToHexStr(DTrd[0], 5, 4, _T(" "));
// 		m_rKeyID.SetContent(str);
// 		
// 		byte CvtIn[3];
// 		CvtIn[0] = DTrd[0][4];//0x2F;
// 		CvtIn[1] = DTrd[0][3];//0x1F;
// 		CvtIn[2] = DTrd[0][2];//0x7F;
// 		str=CCommFunc::byteToHexStr(CvtIn,3, _T(" "));;
// 		m_rLifeHex.SetContent(str);  
// 		int lift = CvtIn[0] * 32 * 128 +CvtIn[1] * 128 + CvtIn[2];
// 		lift=196607-lift;
// 		str.Format(_T("%d"),lift);
// 		m_rLife.SetContent(str);
// 	}
// 	else
// 	{
// 
// 	}
// 	//----
// 	CString str;
// 	str=CCommFunc::byteToHexStr(DTrd[1], 5, 2, _T(" "));
// 	m_rStatus.SetContent(str);
// 	str.Format(_T("%02X"),DTrd[1][3]);
// 	m_rVersion.SetContent(str);
// 	if(DTrd[1][3]==0xbe || DTrd[1][3]==0x57)
// 	{
// 		m_btReadPSW.EnableWindow(TRUE);
// 		m_btSavePSW.EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_btReadPSW.EnableWindow(FALSE);
// 		m_btSavePSW.EnableWindow(FALSE);
// 		m_bGetPSW=false;
// 	}
// 	str=CCommFunc::byteToHexStr(DTrd[2], 3, 6, _T(" "));
// 	m_rMark.SetContent(str);	
	delete[] pBuf;
	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	MessageBeep(MB_ICONEXCLAMATION);
	MessageBox(strTemp);
}

void CDlgNEC_CHIP::ShowLog(CString str)
{
	m_StatusBar.SetText(str, 1, 0);
}

void CDlgNEC_CHIP::OnBnWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	UpdateData();
	if (!CCarKey3App::TheHIDDevice.bLoadKeyData)
	{
		OnButtonBin();
		return;
	}	
	CInteractionData rx;	

	strTemp.LoadString(IDS_StartWriteKey);
	ShowLog(strTemp);
	CInteractionData Result;
	this->UpdateData();	
	byte pBuf[0xa0];
	for(int i=0;i<0xa0;i++)
	{
		pBuf[i]=CCarKey3App::TheHIDDevice.KeyDataBuf[i];			
	}
	CCarKey3App::TheHIDDevice.KeyData_UartProgram(&m_Progress,&Result,pBuf);
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


void CDlgNEC_CHIP::OnBnRenew()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,strTemp1;
// 	strTemp.LoadString(IDS_57Warring1);
// 	
// 	strTemp1.LoadString(IDS_Warring);
// 	if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
// 		return;
	CInteractionData rx;	
	strTemp.LoadString(IDS_STRING183);
	ShowLog(strTemp);
	CInteractionData Result;
	this->UpdateData();
	
	CCarKey3App::TheHIDDevice.KeyData_UartRenew(&m_Progress,&Result);
	if(Result.GetHIDResult()==_T(""))
	{
		strTemp.LoadString(IDS_RenewOK);
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






void CDlgNEC_CHIP::KeyMessageDlg(CString str)
{
	MessageBox(str);
}

void CDlgNEC_CHIP::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgNEC_CHIP::OnCbnSelchangeCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CDlgNEC_CHIP::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	OnBnRenew();
}

void CDlgNEC_CHIP::ReHIDUI()
{
	if (!CCarKey3App::TheHIDDevice.MyDeviceDetected)
	{
		CCarKey3App::TheHIDDevice.FindDevice(); 
	}
	HIDUI();
}

void CDlgNEC_CHIP::HIDUI()
{
	CString strTemp,strTemp1;
	if (CCarKey3App::TheHIDDevice.MyDeviceDetected)
	{		
		USHORT hexver= CCommFunc::MakeShort(CCarKey3App::TheHIDDevice.HexVer[0], CCarKey3App::TheHIDDevice.HexVer[1]);
		//m_ListBox.EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		CString strID=CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexID,4,_T(""));
		strTemp.LoadString(IDS_DeviceID);
		strTemp1=CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexVer,2,_T("."));
		float hexVer=_ttof(strTemp1);
		strTemp1.Format(strTemp,strID,hexVer);
		m_StatusBar.SetText(strTemp1, 0, 0);
		KillTimer(1);		
	}
	else
	{
		//m_ListBox.EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		strTemp.LoadString(IDS_NoDevice);
		m_StatusBar.SetText(strTemp, 0, 0);
		SetTimer(1,1000,NULL);//启动定时器1,定时时间是1秒
	}
}

void CDlgNEC_CHIP::OnTimer( UINT_PTR nIDEvent )
{
	if(nIDEvent==1)
	{
		KillTimer(1);  
		if (!CCarKey3App::TheHIDDevice.MyDeviceDetected)
		{
			CCarKey3App::TheHIDDevice.FindDevice(); 
		}
		ReHIDUI();
	}
}



