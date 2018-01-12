// DlgNEC_CHIP.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgNEC_CHIP.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "DlgShowMessage.h"


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
	DDX_Control(pDX, IDC_EDIT4, m_rKeyID);
	DDX_Control(pDX, IDC_EDIT5, m_rKeyNum);
	DDX_Control(pDX, IDC_EDIT6, m_rLife);
	DDX_Control(pDX, IDC_EDIT7, m_rLifeHex);
	DDX_Control(pDX, IDC_EDIT8, m_rStatus);
	DDX_Control(pDX, IDC_EDIT9, m_rVersion);
	DDX_Control(pDX, IDC_EDIT10, m_rMark);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_EDIT12, m_wPWD);
	DDX_Control(pDX, IDC_EDIT18, m_wKeyNum);
	DDX_Control(pDX, IDC_EDIT17, m_wKeyID);
	DDX_Control(pDX, IDC_BUTTON52, m_btReadPSW);
	DDX_Control(pDX, IDC_BUTTON53, m_btSavePSW);

	DDX_Control(pDX, IDC_RADIO1, m_raIR);
	DDX_Control(pDX, IDC_RADIO2, m_raOnPCB);
	DDX_Control(pDX, IDC_COMBO4, m_ModelType);
}


BEGIN_MESSAGE_MAP(CDlgNEC_CHIP, CDialogEx)
	ON_COMMAND(ID_BUTTON_Clear, &CDlgNEC_CHIP::OnButtonClear)
	ON_COMMAND(ID_BUTTON_Bin, &CDlgNEC_CHIP::OnButtonBin)

	ON_BN_CLICKED(IDC_BUTTON50, &CDlgNEC_CHIP::OnBnReadKeyData)
	ON_BN_CLICKED(IDC_BUTTON51, &CDlgNEC_CHIP::OnBnWrite)
	//ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON52, &CDlgNEC_CHIP::OnBnReadPSW)
	ON_BN_CLICKED(IDC_BUTTON53, &CDlgNEC_CHIP::OnBnSavePSW)
	ON_EN_CHANGE(IDC_EDIT12, &CDlgNEC_CHIP::OnEnChangeEdit12)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgNEC_CHIP::OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgNEC_CHIP::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgNEC_CHIP 消息处理程序


BOOL CDlgNEC_CHIP::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Progress.SetStep(1);
	m_ModelType.AddString(_T("ROM Ver 57"));//_T("W164-NEC-old"));
	//m_ModelType.AddString(_T(""));//(_T("W164-NEC-new"));	
	//m_ModelType.AddString(_T("W166-NEC"));
	m_ModelType.AddString(_T("ROM Ver 03"));//(_T("W204-NEC"));
	//m_ModelType.AddString(_T("W207-NEC"));
	m_ModelType.AddString(_T("ROM Ver 06"));//(_T("W212-NEC-old"));
	//m_ModelType.AddString(_T("W212-NEC-new"));	
	m_ModelType.AddString(_T("ROM Ver 08"));//(_T("W221-NEC-old"));
	//m_ModelType.AddString(_T("W221-NEC-new"));	
	//m_ModelType.AddString(_T("W246-NEC"));
	m_ModelType.SetCurSel(0);
	m_nMode=0;
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

	int strPartDim[1]= {-1}; //分割数量
	m_StatusBar.SetParts(1, strPartDim);
	//设置状态栏文本
	m_StatusBar.SetText(_T(""), 0, 0);
	//下面是在状态栏中加入图标
	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标

	m_hUsbEventHandle = CCarKey3App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
	m_Progress.SetStep(1);
	m_btReadPSW.EnableWindow(FALSE);
	m_btSavePSW.EnableWindow(FALSE);

	m_raIR.SetCheck(TRUE);


	CString strTemp;
	strTemp.LoadString(IDS_ModleType);
	SetDlgItemText(IDC_STATIC24,_T("Key")+strTemp); 
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
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgNEC_CHIP::OnButtonClear()
{
	// TODO: 在此添加命令处理程序代码
	m_rKeyID.Clear();
	m_rKeyNum.Clear();
	m_rLife.Clear();
	m_rLifeHex.Clear();
	m_rVersion.Clear();
	m_rMark.Clear();
	m_rStatus.Clear();	
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

		str.Format(_T("%d"),pLoadBuf[9] & 0x07);
		m_wKeyNum.SetContent(str);

		str=CCommFunc::byteToHexStr(pLoadBuf, 9, 4, _T(" "));
		m_wKeyID.SetContent(str);

		strTemp.LoadString(IDS_Key);
		SetWindowText(strTemp+_T("--")+StrName);

		strTemp.LoadString(IDS_LoadKeyFileOK);
		ShowLog(strTemp);
		ShowLog(StrName);
		//-->逆序预处理
		byte Tmp;
		for (int i = 0, j = 0x08; i < 4; i++, j--)
		{
			Tmp = pLoadBuf[i + 0x01];
			pLoadBuf[i + 0x01] = pLoadBuf[j];
			pLoadBuf[j] = Tmp;
		}
		for (int i = 0, j = 0x0C; i < 2; i++, j--)
		{
			Tmp = pLoadBuf[i + 0x09];
			pLoadBuf[i + 0x09] = pLoadBuf[j];
			pLoadBuf[j] = Tmp;
		}
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
	m_rKeyID.Clear();
	m_rKeyNum.Clear();
	m_rLife.Clear();
	m_rLifeHex.Clear();
	m_rVersion.Clear();
	m_rMark.Clear();
	m_rStatus.Clear();	
// 	strTemp.LoadString(IDS_StartReadData);
// 	ShowLog(strTemp);
	byte type;
	if(m_raIR.GetCheck()==TRUE)
	 	type = 0;
	else//芯片在适配器上
	 	type = 1;
	CInteractionData Result;
	byte* pBuf=CCarKey3App::TheHIDDevice.KeyData_UartReadIR(type,m_nMode,&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
		return;
	}
	byte DTrd[0x03][0x10];
	for(int i=0;i<0x03;i++)
	{
		for(int j=0;j<0x10;j++)
		{
			DTrd[i][j]=pBuf[i*0x10+j];
		}
	}
	delete[] pBuf;
	if(0x55==DTrd[0][0])
	{
		CString str;
		str.Format(_T("%d"),DTrd[0][5] & 0x07);
		m_rKeyNum.SetContent(str);
		//rx.buff[0x15] = (byte)(rx.buff[0x15] & 0xF8);
		DTrd[0][5]=DTrd[0][5]&0xF8;
		str=CCommFunc::byteToHexStr(DTrd[0], 5, 4, _T(" "));
		m_rKeyID.SetContent(str);
		
		byte CvtIn[3];
		CvtIn[0] = DTrd[0][4];//0x2F;
		CvtIn[1] = DTrd[0][3];//0x1F;
		CvtIn[2] = DTrd[0][2];//0x7F;
		str=CCommFunc::byteToHexStr(CvtIn,3, _T(" "));;
		m_rLifeHex.SetContent(str);  
		int lift = CvtIn[0] * 32 * 128 +CvtIn[1] * 128 + CvtIn[2];
		lift=196607-lift;
		str.Format(_T("%d"),lift);
		m_rLife.SetContent(str);
	}
	else
	{
// 		m_rKeyNum.SetContent(str);
// 		m_rKeyID.SetContent(str);
// 		m_rLifeHex.SetContent(str);
// 		m_rLife.SetContent(str);
	}
	//----
	CString str;
	str=CCommFunc::byteToHexStr(DTrd[1], 5, 2, _T(" "));
	m_rStatus.SetContent(str);
	str.Format(_T("%02X"),DTrd[1][3]);
	m_rVersion.SetContent(str);
	if(DTrd[1][3]==0xbe || DTrd[1][3]==0x57)
	{
		m_btReadPSW.EnableWindow(TRUE);
		m_btSavePSW.EnableWindow(FALSE);
	}
	else
	{
		m_btReadPSW.EnableWindow(FALSE);
		m_btSavePSW.EnableWindow(FALSE);
		m_bGetPSW=false;
	}
	str=CCommFunc::byteToHexStr(DTrd[2], 3, 6, _T(" "));
	m_rMark.SetContent(str);	
	
	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	MessageBeep(MB_ICONEXCLAMATION);
	MessageBox(strTemp);

// 	CString strTemp;
// 	CInteractionData rx;
// 	rx=CCarKey3App::TheHIDDevice.Check_mima();
// 	if(rx.mimaok!=true)
// 	{
// 		CString strx=rx.GetHIDResult();
// 		ShowLog(strx);
// 		KeyMessageDlg(strx);
// 		return;
// 	}
// 	m_rKeyID.Clear();
// 	m_rKeyNum.Clear();
// 	m_rLife.Clear();
// 	m_rLifeHex.Clear();
// 	m_rVersion.Clear();
// 	m_rMark.Clear();
// 	m_rStatus.Clear();	
// 	m_Progress.SetRange(0,1);
// 	m_Progress.SetPos(0);
// 	m_Progress.UpdateWindow();
// 	this->UpdateWindow();	
// 	//ShowLog(_T("开始读取钥匙信息"));
// 	byte type;
// 	if(m_raIR.GetCheck()==TRUE)
// 		type = 0x08;
// 	else//芯片在适配器上
// 		type = 0x10;
// 	rx = CCarKey3App::TheHIDDevice.ReadKeyData(type);
// 	if(rx.GetHIDResult()==_T(""))
// 	{
// 		strTemp.LoadString(IDS_ReadKeyOK);
// 		ShowLog(strTemp);
// 		if (0x55 == rx.buff[0x10])
// 		{
// 			CString str;
// 			str.Format(_T("%d"),rx.buff[0x15] & 0x07);
// 			m_rKeyNum.SetContent(str);
// 			//rx.buff[0x15] = (byte)(rx.buff[0x15] & 0xF8);
// 			str=CCommFunc::byteToHexStr(rx.buff, 0x15, 4, _T(" "));
// 			m_rKeyID.SetContent(str);
// 
// 			byte CvtIn[3];
// 			CvtIn[0] = rx.buff[0x14];//0x2F;
// 			CvtIn[1] = rx.buff[0x13];//0x1F;
// 			CvtIn[2] = rx.buff[0x12];//0x7F;
// 			str=CCommFunc::byteToHexStr(CvtIn,3, _T(" "));;
// 			m_rLifeHex.SetContent(str);  
// 			int lift = rx.buff[0x14] * 32 * 128 + rx.buff[0x13] * 128 + rx.buff[0x12];
// 			lift=196607-lift;
// 			str.Format(_T("%d"),lift);
// 			m_rLife.SetContent(str);
// 		}
// 		if (0x55 == rx.buff[0x20])
// 		{
// 			CString str;
// 			str=CCommFunc::byteToHexStr(rx.buff, 0x25, 2, _T(" "));
// 			m_rStatus.SetContent(str);
// 			str.Format(_T("%02X"),rx.buff[0x23]);
// 			m_rVersion.SetContent(str);
// 			if(rx.buff[0x23]==0xbe || rx.buff[0x23]==0x57)
// 			{
// 				m_btReadPSW.EnableWindow(TRUE);
// 				m_btSavePSW.EnableWindow(FALSE);
// 			}
// 			else
// 			{
// 				m_btReadPSW.EnableWindow(FALSE);
// 				m_btSavePSW.EnableWindow(FALSE);
// 				m_bGetPSW=false;
// 			}
// 			str=CCommFunc::byteToHexStr(rx.buff, 0x33, 6, _T(" "));
// 			m_rMark.SetContent(str);
// 		}
// 	}
// 	else
// 	{
// 		ShowLog(rx.GetHIDResult());
// 	}
// 	m_Progress.StepIt();
}

void CDlgNEC_CHIP::ShowLog(CString str)
{
	m_StatusBar.SetText(str, 0, 0);
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
	rx=CCarKey3App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		KeyMessageDlg(strx);
		return;
	}

	strTemp.LoadString(IDS_StartWriteKey);
	ShowLog(strTemp);

	CInteractionData Result;
	byte type;
	this->UpdateData();
	if (m_raIR.GetCheck()==TRUE)//红外适配器
	 	type = 0;
	else//芯片在适配器上
	 	type = 1;
	byte pBuf[0xa0];
	for(int i=0;i<0xa0;i++)
	{
		pBuf[i]=CCarKey3App::TheHIDDevice.KeyDataBuf[i];			
	}
	CCarKey3App::TheHIDDevice.KeyData_UartProgram(type,m_nMode,&m_Progress,&Result,pBuf);
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

// 	m_Progress.SetRange(0,5);   //设置进度条的范围为0-1000	
// 	m_Progress.SetPos(0);
// 	m_Progress.UpdateWindow();
// 	//CInteractionData rx;
// 	for (int i=0;i<4;i++)
// 	{
// 		rx = CCarKey3App::TheHIDDevice.WriteKeyData1(i);
// 		if(rx.GetHIDResult()==_T(""))
// 		{
// 			m_Progress.StepIt();
// 			m_Progress.UpdateWindow();
// 		}
// 		else
// 		{
// 			ShowLog(rx.GetHIDResult());
// 			return;
// 		}
// 	}
// 	CCarKey3App::WaitMS(1000);
// 	byte type;
// 	this->UpdateData();
// 	if (m_raIR.GetCheck()==TRUE)//红外适配器
// 		type = 0x06;
// 	else//芯片在适配器上
// 		type = 0x11;
// 	rx = CCarKey3App::TheHIDDevice.WriteKeyData2(type);
// 	if(rx.GetHIDResult()==_T(""))
// 	{
// 		m_Progress.StepIt();
// 		m_Progress.UpdateWindow();
// 	}
// 	else
// 	{
// 		ShowLog(rx.GetHIDResult());
// 		return;
// 	}
// 	if(rx.GetHIDResult()==_T(""))
// 	{
// 		strTemp.LoadString(IDS_WriteKeyOK);
// 		ShowLog(strTemp);
// 		KeyMessageDlg(strTemp);
// 	}
// 	m_Progress.SetPos(0);
}


void CDlgNEC_CHIP::OnBnRenew()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,strTemp1;
	if (m_raIR.GetCheck()==TRUE)//红外适配器
	{
		strTemp.LoadString(IDS_BEWarring);
	}
	else
	{
		strTemp.LoadString(IDS_57Warring1);
	}
	
	strTemp1.LoadString(IDS_Warring);
	if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
		return;
	CInteractionData rx;
	rx=CCarKey3App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		KeyMessageDlg(strx);
		return;
	}
	strTemp.LoadString(IDS_STRING183);
	ShowLog(strTemp);
	CInteractionData Result;
	byte type;
	this->UpdateData();
	if (m_raIR.GetCheck()==TRUE)//红外适配器
		type = 0;
	else//芯片在适配器上
		type = 1;
	CCarKey3App::TheHIDDevice.KeyData_UartRenew(type,m_nMode,&m_Progress,&Result);
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




void CDlgNEC_CHIP::OnBnReadPSW()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,strTemp1;
	CInteractionData rx;
	rx=CCarKey3App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		KeyMessageDlg(strx);
		return;
	}
	CString strVer;
	m_rVersion.GetWindowText(strVer);
	strTemp1.LoadString(IDS_Warring);
	if (strVer==_T("57"))
	{
		strTemp.LoadString(IDS_57Warring1);
		if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
			return;
		strTemp.LoadString(IDS_57Warring2);
		if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
			return;
	}
	else
	{
		strTemp.LoadString(IDS_BEWarring);
		if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
			return;
	}
	ShowLog(_T("-----------------------------------------------"));

	strTemp.LoadString(IDS_StartReadPSW);
	ShowLog(strTemp);

	byte type;
	if(m_raIR.GetCheck()==TRUE)
		type = 0;
	else//芯片在适配器上
		type = 1;
	CInteractionData Result;
	byte* pBuf=CCarKey3App::TheHIDDevice.KeyData_UartReadPSW(type,m_nMode,&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		MessageBox(Result.GetHIDResult());
		return;
	}
	byte DTrd[0x03][0x10];
	for(int i=0;i<0x03;i++)
	{
		for(int j=0;j<0x10;j++)
		{
			DTrd[i][j]=pBuf[i*0x10+j];
		}
	}
	delete[] pBuf;
	if(m_raIR.GetCheck()!=TRUE)
	{//pcb
		//PSW
		m_PSWBuf[0x00]=DTrd[1][0x0A];//BufPassWord[0x00];
		m_PSWBuf[0x01]=DTrd[0][0x03];//BufPassWord[0x01];
		m_PSWBuf[0x02]=DTrd[0][0x04];//BufPassWord[0x02];
		m_PSWBuf[0x03]=DTrd[0][0x05];//BufPassWord[0x03];
		m_PSWBuf[0x04]=DTrd[0][0x06];//BufPassWord[0x04];
		m_PSWBuf[0x05]=DTrd[0][0x07];//BufPassWord[0x05];
		m_PSWBuf[0x06]=DTrd[0][0x08];//BufPassWord[0x06];
		m_PSWBuf[0x07]=DTrd[0][0x09];//BufPassWord[0x07];

		//SSID
		m_PSWBuf[0x08]=DTrd[1][0x06];//BufPassWord[0x08];
		m_PSWBuf[0x09]=DTrd[1][0x07];//BufPassWord[0x09];
		m_PSWBuf[0x0A]=DTrd[1][0x08];//BufPassWord[0x0A];
		m_PSWBuf[0x0B]=DTrd[1][0x09];//BufPassWord[0x0B];
		//HASH
		m_PSWBuf[0x0C]=DTrd[1][0x05];//BufPassWord[0x0C];
		m_PSWBuf[0x0D]=DTrd[1][0x04];//BufPassWord[0x0D];
		m_PSWBuf[0x0E]=DTrd[1][0x03];//BufPassWord[0x0E];
		m_PSWBuf[0x0F]=DTrd[2][0x0A];//BufPassWord[0x0F];
		m_PSWBuf[0x10]=DTrd[2][0x09];//BufPassWord[0x10];
		m_PSWBuf[0x11]=DTrd[2][0x08];//BufPassWord[0x11];
		m_PSWBuf[0x12]=DTrd[2][0x07];//BufPassWord[0x12];
		m_PSWBuf[0x13]=DTrd[2][0x06];//BufPassWord[0x13];

		//OTHERS
		m_PSWBuf[0x14]=DTrd[2][0x05];//BufPassWord[0x14];
		m_PSWBuf[0x15]=DTrd[2][0x04];//BufPassWord[0x15];
		m_PSWBuf[0x16]=DTrd[2][0x03];//BufPassWord[0x16];
	//	m_PSWBuf[0x17]=DTrd[2][0x0A];//BufPassWord[0x17];
		//
	}
	else
	{//IR
		//PSW
		m_PSWBuf[0x00]=DTrd[0][0x04];//BufPassWord[0x00];
		m_PSWBuf[0x01]=DTrd[0][0x05];//BufPassWord[0x01];
		m_PSWBuf[0x02]=DTrd[0][0x06];//BufPassWord[0x02];
		m_PSWBuf[0x03]=DTrd[0][0x07];//BufPassWord[0x03];
		m_PSWBuf[0x04]=DTrd[0][0x08];//BufPassWord[0x04];
		m_PSWBuf[0x05]=DTrd[0][0x09];//BufPassWord[0x05];
		m_PSWBuf[0x06]=DTrd[0][0x0A];//BufPassWord[0x06];
		m_PSWBuf[0x07]=DTrd[1][0x03];//BufPassWord[0x07];

		//SSID
		m_PSWBuf[0x08]=DTrd[1][0x04];//BufPassWord[0x08];
		m_PSWBuf[0x09]=DTrd[1][0x05];//BufPassWord[0x09];
		m_PSWBuf[0x0A]=DTrd[1][0x06];//BufPassWord[0x0A];
		m_PSWBuf[0x0B]=DTrd[1][0x07];//BufPassWord[0x0B];
		//HASH
		m_PSWBuf[0x0C]=DTrd[2][0x07];//BufPassWord[0x0C];
		m_PSWBuf[0x0D]=DTrd[2][0x06];//BufPassWord[0x0D];
		m_PSWBuf[0x0E]=DTrd[2][0x05];//BufPassWord[0x0E];
		m_PSWBuf[0x0F]=DTrd[2][0x04];//BufPassWord[0x0F];
		m_PSWBuf[0x10]=DTrd[2][0x03];//BufPassWord[0x10];
		m_PSWBuf[0x11]=DTrd[1][0x0A];//BufPassWord[0x11];
		m_PSWBuf[0x12]=DTrd[1][0x09];//BufPassWord[0x12];
		m_PSWBuf[0x13]=DTrd[1][0x08];//BufPassWord[0x13];

		//OTHERS
		m_PSWBuf[0x14]=DTrd[2][0x05];//BufPassWord[0x14];
		m_PSWBuf[0x15]=DTrd[2][0x04];//BufPassWord[0x15];
		m_PSWBuf[0x16]=DTrd[2][0x03];//BufPassWord[0x16];
	}
	CString strMessage=_T("");
	CString str;
	str=CCommFunc::byteToHexStr(m_PSWBuf, 0x08, 4, _T(" "));//KeyID
	 
	strTemp.LoadString(IDS_KeyDlg_KeyID);
	strMessage+=(strTemp+_T(":")+str+_T("\r\n"));
	//ShowLog(strTemp+_T(":")+str);
	 
	str.Format(_T("%d"),m_PSWBuf[0x08] & 0x07);
	strTemp.LoadString(IDS_KeyDlg_KeyNum);
	strMessage+=(strTemp+_T(":")+str+_T("\r\n"));
	//ShowLog(strTemp+_T(":")+str);
	 
	str=CCommFunc::byteToHexStr(m_PSWBuf, 0x00, 8, _T(" "));
	strTemp.LoadString(IDS_KeyDlg_PSW);
	strMessage+=(strTemp+_T(":")+str+_T("\r\n"));
	//ShowLog(strTemp+_T(":")+str);
	 
	m_pMainDlg->m_strPSW=str;
	str=CCommFunc::byteToHexStr(m_PSWBuf, 0x0c, 8, _T(" "));
	strMessage+=(_T("HASH:")+str+_T("\r\n"));
	 
	strTemp.LoadString(IDS_ReadPSWOK);
	ShowLog(strTemp);
	//KeyMessageDlg(strTemp);
	CDlgShowMessage sm;
	sm.m_strMessage=strMessage;
	sm.DoModal();
	//ShowLog(_T("HASH:")+str);
// 	if(rx.GetHIDResult()==_T(""))
// 	{
// 		m_Progress.StepIt();
// 		m_Progress.UpdateWindow();	
// 		byte BufPassWord[0x18];
// 		for (int i = 0; i < 0x18; i++)
// 		{
// 			BufPassWord[i] = rx.buff[i + 0x10];
// 		}
// 		for (int i = 0; i < 0x18; i++)
// 		{
// 			m_PSWBuf[i] = BufPassWord[i];
// 		}
// 		m_bGetPSW=true;
// 		m_btSavePSW.EnableWindow(TRUE);
// 		if (strVer==_T("57"))
// 		{			
// 			//
// 			//PSW
// 			m_PSWBuf[0x00] = BufPassWord[0x07];
// 			m_PSWBuf[0x01] = BufPassWord[0x0F];
// 			m_PSWBuf[0x02] = BufPassWord[0x00];
// 			m_PSWBuf[0x03] = BufPassWord[0x01];
// 			m_PSWBuf[0x04] = BufPassWord[0x02];
// 			m_PSWBuf[0x05] = BufPassWord[0x03];
// 			m_PSWBuf[0x06] = BufPassWord[0x04];
// 			m_PSWBuf[0x07] = BufPassWord[0x05];
// 			m_PSWBuf[0x08] = BufPassWord[0x06];
// 			//SSID
// 			m_PSWBuf[0x09] = BufPassWord[0x0B];
// 			m_PSWBuf[0x0A] = BufPassWord[0x0C];
// 			m_PSWBuf[0x0B] = BufPassWord[0x0D];
// 			m_PSWBuf[0x0C] = BufPassWord[0x0E];
// 			//HASH
// 			m_PSWBuf[0x0D] = BufPassWord[0x0A];
// 			m_PSWBuf[0x0E] = BufPassWord[0x09];
// 			m_PSWBuf[0x0F] = BufPassWord[0x08];
// 			m_PSWBuf[0x10] = BufPassWord[0x17];
// 			m_PSWBuf[0x11] = BufPassWord[0x16];
// 			m_PSWBuf[0x12] = BufPassWord[0x15];
// 			m_PSWBuf[0x13] = BufPassWord[0x14];
// 			m_PSWBuf[0x14] = BufPassWord[0x13];
// 			//OTHERS
// 			m_PSWBuf[0x15] = BufPassWord[0x12];
// 			m_PSWBuf[0x16] = BufPassWord[0x11];
// 			m_PSWBuf[0x17] = BufPassWord[0x10];
// 		}
// 		// 		textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x0d, 8, " ");//HASH
// 		// 		textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x01, 8, " ");//PSD
// 		// 		textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x09, 4, " ");//serial
// 		// 		textBox2.Text = (BufPassWord[0x0B] & 0x07).ToString();//key
// 		CString strMessage=_T("");
// 		CString str;
// 		str=CCommFunc::byteToHexStr(m_PSWBuf, 0x09, 4, _T(" "));//KeyID
// 
// 		strTemp.LoadString(IDS_KeyDlg_KeyID);
// 		strMessage+=(strTemp+_T(":")+str+_T("\r\n"));
// 		//ShowLog(strTemp+_T(":")+str);
// 
// 		str.Format(_T("%d"),m_PSWBuf[0x09] & 0x07);
// 		strTemp.LoadString(IDS_KeyDlg_KeyNum);
// 		strMessage+=(strTemp+_T(":")+str+_T("\r\n"));
// 		//ShowLog(strTemp+_T(":")+str);
// 
// 		str=CCommFunc::byteToHexStr(m_PSWBuf, 0x01, 8, _T(" "));
// 		strTemp.LoadString(IDS_KeyDlg_PSW);
// 		strMessage+=(strTemp+_T(":")+str+_T("\r\n"));
// 		//ShowLog(strTemp+_T(":")+str);
// 
// 		m_pMainDlg->m_strPSW=str;
// 		str=CCommFunc::byteToHexStr(m_PSWBuf, 0x0d, 8, _T(" "));
// 		strMessage+=(_T("HASH:")+str+_T("\r\n"));
// 
// 		strTemp.LoadString(IDS_ReadPSWOK);
// 		ShowLog(strTemp);
// 		//KeyMessageDlg(strTemp);
// 		CDlgShowMessage sm;
// 		sm.m_strMessage=strMessage;
// 		sm.DoModal();
// 		//ShowLog(_T("HASH:")+str);
// 	}
// 	else
// 	{
// 		ShowLog(rx.GetHIDResult());
// 		return;
// 	}	
}


void CDlgNEC_CHIP::OnBnSavePSW()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CFileDialog fileDlgS(FALSE);
	strTemp.LoadString(IDS_SavePSWTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;
	fileDlgS.m_ofn.lpstrFilter=_T("PSW Files(*.PSW)\0*.PSW\0All Files(*.*)\0*.*\0");
	fileDlgS.m_ofn.lpstrDefExt=_T("PSW");
	//KeyID
	CString strFileName=CCommFunc::byteToHexStr(m_PSWBuf, 0x09, 4, _T(""));//+CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
	// 	CTime tm=CTime::GetCurrentTime();
	// 	CString str=tm.Format("%Y-%m-%d");
	// 	strFileName+=str;
	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
	if (IDOK==fileDlgS.DoModal())
	{
		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		byte outBuf[0xA0];	//最大64M内存
		for (int i = 0; i < 0xa0; i++)
			outBuf[i] = 0xff;
		for (int i = 0; i < 0x18; i++)
		{
			outBuf[i] = m_PSWBuf[i];
		}
		file.Write(outBuf,0xa0);
		file.Close();
		strTemp.LoadString(IDS_SavePSWOK);
		ShowLog(strTemp+fileDlgS.GetPathName());
	}
	strFileName.ReleaseBuffer();	
}

void CDlgNEC_CHIP::KeyMessageDlg(CString str)
{
	MessageBox(str);
	m_rKeyID.SetFocus();
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
	int sel=m_ModelType.GetCurSel();
	switch(sel)
	{	
	case 0://ROM V57
		{
			m_nMode=0;
		}
		break;
	case 1://ROM V03
		{
			m_nMode=1;
		}
		break;
	case 2://ROM V06
		{
			m_nMode=2;
		}
		break;
	case 3://ROM V08
		{
			m_nMode=3;//02款==204
		}
		break;
// 	case 4://W207-NEC
// 		{
// 			m_nMode=1;//02款==204
// 		}
// 		break;
// 	case 5://W212-NEC-old
// 		{
// 			m_nMode=2;//03款==212old
// 		}
// 		break;
// 	case 6://W212-NEC-new
// 		{
// 			m_nMode=5;//06款==212new
// 		}
// 		break;
// 	case 7://W221-NEC-old
// 		{
// 			m_nMode=3;//04款==221old
// 		}
// 		break;
// 	case 8://W221-NEC-new
// 		{
// 			m_nMode=6;//07款==221new
// 		}
// 		break;
// 	case 9://W246-NEC
// 		{
// 			m_nMode=8;//09款==246
// 		}
// 		break;
	default:
		m_nMode=0xff;
		break;
	}
}


void CDlgNEC_CHIP::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	OnBnRenew();
}
