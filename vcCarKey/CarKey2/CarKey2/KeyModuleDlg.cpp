// KeyModuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "KeyModuleDlg.h"
#include "afxdialogex.h"
#include ".\..\..\CarKeyCommon\CommFunc.h"

// CKeyModuleDlg 对话框

IMPLEMENT_DYNAMIC(CKeyModuleDlg, CDialogEx)

CKeyModuleDlg::CKeyModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKeyModuleDlg::IDD, pParent)
	, m_strLog(_T(""))
{

}

CKeyModuleDlg::~CKeyModuleDlg()
{
	m_bkBrush.DeleteObject();
}

void CKeyModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_rKeyID);
	DDX_Control(pDX, IDC_EDIT5, m_rKeyNum);
	DDX_Control(pDX, IDC_EDIT6, m_rLife);
	DDX_Control(pDX, IDC_EDIT7, m_rLifeHex);
	DDX_Control(pDX, IDC_EDIT8, m_rStatus);
	DDX_Control(pDX, IDC_EDIT9, m_rVersion);
	DDX_Control(pDX, IDC_EDIT10, m_rMark);
	DDX_Control(pDX, IDC_COMBO1, m_OptType);
	DDX_Control(pDX, IDC_EDIT1, m_Log);
	DDX_Text(pDX, IDC_EDIT1, m_strLog);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_EDIT12, m_wPWD);
	DDX_Control(pDX, IDC_EDIT18, m_wKeyNum);
	DDX_Control(pDX, IDC_EDIT17, m_wKeyID);
	DDX_Control(pDX, IDC_BUTTON19, m_btReadPSW);
	DDX_Control(pDX, IDC_BUTTON20, m_btSavePSW);
}


BEGIN_MESSAGE_MAP(CKeyModuleDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON5, &CKeyModuleDlg::OnBnReadKeyData)
	ON_BN_CLICKED(IDC_BUTTON3, &CKeyModuleDlg::OnBnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON4, &CKeyModuleDlg::OnBnWrite)
	ON_BN_CLICKED(IDC_BUTTON9, &CKeyModuleDlg::OnBnRenew)
	ON_BN_CLICKED(IDC_BUTTON1, &CKeyModuleDlg::OnBnClearLog)
	ON_BN_CLICKED(IDC_BUTTON2, &CKeyModuleDlg::OnBnSaveLog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON19, &CKeyModuleDlg::OnBnReadPSW)
	ON_BN_CLICKED(IDC_BUTTON20, &CKeyModuleDlg::OnBnSavePSW)
END_MESSAGE_MAP()


// CKeyModuleDlg 消息处理程序


BOOL CKeyModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hUsbEventHandle = CCarKey2App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
	m_OptType.SetCurSel(0);
	m_bkBrush.CreateSolidBrush(RGB(166,202,240));
	m_Progress.SetStep(1);
	m_btReadPSW.EnableWindow(FALSE);
	m_btSavePSW.EnableWindow(FALSE);

	CString strTemp;
	strTemp.LoadString(IDS_KeyModule);
	SetWindowText(strTemp);
	m_OptType.Clear();
	strTemp.LoadString(IDS_Options_IR);
	m_OptType.AddString(strTemp);
	strTemp.LoadString(IDS_Options_OnPCB);
	m_OptType.AddString(strTemp);
	m_OptType.SetCurSel(0);
	/************************************************************************/
	/* 按键                                                                 */
	/************************************************************************/
	strTemp.LoadString(IDS_KeyDlg_ClearLog);
	SetDlgItemText(IDC_BUTTON1,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_SaveLog);
	SetDlgItemText(IDC_BUTTON2,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_OpenFile);
	SetDlgItemText(IDC_BUTTON3,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_Write);
	SetDlgItemText(IDC_BUTTON4,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_ReadKey);
	SetDlgItemText(IDC_BUTTON5,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_Renew);
	SetDlgItemText(IDC_BUTTON9,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_ReadPSW);
	SetDlgItemText(IDC_BUTTON19,strTemp); 

	strTemp.LoadString(IDS_KeyDlg_SavePSW);
	SetDlgItemText(IDC_BUTTON20,strTemp); 

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
	SetDlgItemText(IDC_STATIC7,strTemp);

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
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CKeyModuleDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
			//::SendMessage(this->GetSafeHwnd(),WM_CLOSE,0,0);
			break;
		default:
			break;
		}
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CKeyModuleDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CCarKey2App::TheHIDDevice.UnregisterForUsbEvents(m_hUsbEventHandle);
	CDialogEx::OnClose();
}

void CKeyModuleDlg::OnBnReadKeyData()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		KeyMessageDlg(strx);
		return;
	}
	ShowLog(_T("-----------------------------------------------"));
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
	this->UpdateWindow();	
	//ShowLog(_T("开始读取钥匙信息"));
	byte type;
	if (m_OptType.GetCurSel()==0)//红外适配器
		type = 0x08;
	else//芯片在适配器上
		type = 0x10;
	rx = CCarKey2App::TheHIDDevice.ReadKeyData(type);
	if(rx.GetHIDResult()==_T(""))
	{
		strTemp.LoadString(IDS_ReadKeyOK);
		ShowLog(strTemp);
		if (0x55 == rx.buff[0x10])
		{
			CString str;
			str.Format(_T("%d"),rx.buff[0x15] & 0x07);
			m_rKeyNum.SetContent(str);
			//rx.buff[0x15] = (byte)(rx.buff[0x15] & 0xF8);
			str=CCommFunc::byteToHexStr(rx.buff, 0x15, 4, _T(" "));
			m_rKeyID.SetContent(str);

			byte CvtIn[3];
			CvtIn[0] = rx.buff[0x14];//0x2F;
			CvtIn[1] = rx.buff[0x13];//0x1F;
			CvtIn[2] = rx.buff[0x12];//0x7F;
			str=CCommFunc::byteToHexStr(CvtIn,3, _T(" "));;
			m_rLifeHex.SetContent(str);  
			int lift = rx.buff[0x14] * 32 * 128 + rx.buff[0x13] * 128 + rx.buff[0x12];
			str.Format(_T("%d"),lift);
			m_rLife.SetContent(str);
		}
		if (0x55 == rx.buff[0x20])
		{
			CString str;
			str=CCommFunc::byteToHexStr(rx.buff, 0x25, 2, _T(" "));
			m_rStatus.SetContent(str);
			str.Format(_T("%02X"),rx.buff[0x23]);
			m_rVersion.SetContent(str);
			if(rx.buff[0x23]==0xbe || rx.buff[0x23]==0x57)
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
			str=CCommFunc::byteToHexStr(rx.buff, 0x33, 6, _T(" "));
			m_rMark.SetContent(str);
		}
	}
	else
	{
		ShowLog(rx.GetHIDResult());
	}
	m_Progress.StepIt();
}

void CKeyModuleDlg::ShowLog(CString str)
{
	m_strLog.Append(str+_T("\r\n"));
	UpdateData(FALSE);
	m_Log.UpdateWindow();
	m_Log.LineScroll(m_Log.GetLineCount());
}


void CKeyModuleDlg::OnBnOpenFile()
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
			CCarKey2App::TheHIDDevice.KeyDataBuf[i]=pLoadBuf[i];			
		}
		CCarKey2App::TheHIDDevice.bLoadKeyData=true;
		delete[] pLoadBuf;
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
		CCarKey2App::TheHIDDevice.bLoadKeyData=false;
	}
}


void CKeyModuleDlg::OnBnWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	if (!CCarKey2App::TheHIDDevice.bLoadKeyData)
	{
		OnBnOpenFile();
		return;
	}	
	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		KeyMessageDlg(strx);
		return;
	}
	ShowLog(_T("-----------------------------------------------"));

	strTemp.LoadString(IDS_StartWriteKey);
	ShowLog(strTemp);

	m_Progress.SetRange(0,5);   //设置进度条的范围为0-1000	
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();
	//CInteractionData rx;
	for (int i=0;i<4;i++)
	{
		rx = CCarKey2App::TheHIDDevice.WriteKeyData1(i);
		if(rx.GetHIDResult()==_T(""))
		{
			m_Progress.StepIt();
			m_Progress.UpdateWindow();
		}
		else
		{
			ShowLog(rx.GetHIDResult());
			return;
		}
	}
	CCarKey2App::WaitMS(1000);
	byte type;
	if (m_OptType.GetCurSel()==0)//红外适配器
		type = 0x06;
	else//芯片在适配器上
		type = 0x11;
	rx = CCarKey2App::TheHIDDevice.WriteKeyData2(type);
	if(rx.GetHIDResult()==_T(""))
	{
		m_Progress.StepIt();
		m_Progress.UpdateWindow();
	}
	else
	{
		ShowLog(rx.GetHIDResult());
		return;
	}
	if(rx.GetHIDResult()==_T(""))
	{
		strTemp.LoadString(IDS_WriteKeyOK);
		ShowLog(strTemp);
		KeyMessageDlg(strTemp);
	}
	m_Progress.SetPos(0);
}


void CKeyModuleDlg::OnBnRenew()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,strTemp1;
	strTemp.LoadString(IDS_RenewWarring);
	strTemp1.LoadString(IDS_Warring);
	if (MessageBox(strTemp, strTemp1, MB_YESNO) == IDNO)
		return;
	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		KeyMessageDlg(strx);
		return;
	}
	ShowLog(_T("-----------------------------------------------"));
	//ShowLog(_T("开始擦除"));
	m_Progress.SetRange(0,1);
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();
	CInteractionData tx;
	if (m_OptType.GetCurSel()==0)//红外适配器
		tx.buff[0] = 0x0A;
	else//芯片在适配器上
		tx.buff[0] = 0x12;
	tx.buff[1] = 0x55;	//主机-->设备
	rx = CCarKey2App::TheHIDDevice.DeviceInteraction(tx);
	if (!rx.bOK)
		rx.hidResult= HIDResult_USBError;
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
	}
	else if (rx.buff[1] == 0x00)
	{
		rx.hidResult= HIDResult_OK;
	}
	else
	{
		rx.hidResult= HIDResult_RenewError;
	}
	m_Progress.StepIt();
	if(rx.GetHIDResult()==_T(""))
	{
		strTemp.LoadString(IDS_RenewOK);
		ShowLog(strTemp);
		KeyMessageDlg(strTemp);
	}
	else
	{
		ShowLog(rx.GetHIDResult());
		KeyMessageDlg(rx.GetHIDResult());
	}		
}


void CKeyModuleDlg::OnBnClearLog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Log.SetWindowText(_T(""));
}


void CKeyModuleDlg::OnBnSaveLog()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlgS(FALSE);
	CString strTemp;

	strTemp.LoadString(IDS_SaveLogTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;

	fileDlgS.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
	fileDlgS.m_ofn.lpstrDefExt=_T("txt");

	strTemp.LoadString(IDS_KeyLogName);
	CString strFileName=strTemp;//+CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
	CTime tm=CTime::GetCurrentTime();
	CString str=tm.Format("%Y-%m-%d");
	strFileName+=str;
	
	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
	if (IDOK==fileDlgS.DoModal())
	{
		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		int len;
		m_Log.GetWindowText(str);
		char* pLog=CCommFunc::CStringToChar(str,&len);
		file.Write(pLog,len*sizeof(CHAR));
		file.Close();
		delete[] pLog;
	}
	strFileName.ReleaseBuffer();
}


HBRUSH CKeyModuleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor ==CTLCOLOR_DLG)
		return m_bkBrush; //返加绿色刷子
	if (nCtlColor = CTLCOLOR_STATIC)
	{
		switch(pWnd->GetDlgCtrlID()) 
		{
		case IDC_STATIC1:
		case IDC_STATIC2:
		case IDC_STATIC3:
		case IDC_STATIC4:
		case IDC_STATIC5:
		case IDC_STATIC6:
		case IDC_STATIC7:
		case IDC_STATIC8:
		case IDC_STATIC9:
		case IDC_STATIC10:
		case IDC_STATIC11:
		case IDC_STATIC12:
		case IDC_STATIC13:
			pDC->SetBkColor(RGB(166,202,240));
			pDC->SetBkMode(TRANSPARENT);      //不显示文字背景
			hbr = (HBRUSH)::GetStockObject(NULL_BRUSH); //编辑框背景。注意：和文字背景不是一个意思。
			break;
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CKeyModuleDlg::OnBnReadPSW()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,strTemp1;
	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
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
	
	m_Progress.SetRange(0,2);
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();	
	byte type;
	if (strVer==_T("57"))
		type = 0x0c;
	else//芯片在适配器上
		type = 0x13;
	//CInteractionData rx;
	rx = CCarKey2App::TheHIDDevice.ReadPSW1(type);
	if(rx.GetHIDResult()==_T(""))
	{
		m_Progress.StepIt();
		m_Progress.UpdateWindow();				
	}
	else
	{
		ShowLog(rx.GetHIDResult());
		return;
	}
	rx = CCarKey2App::TheHIDDevice.ReadPSW2();
	if(rx.GetHIDResult()==_T(""))
	{
		m_Progress.StepIt();
		m_Progress.UpdateWindow();	
		byte BufPassWord[0x18];
		for (int i = 0; i < 0x18; i++)
		{
			BufPassWord[i] = rx.buff[i + 0x10];
		}
		for (int i = 0; i < 0x18; i++)
		{
			m_PSWBuf[i] = BufPassWord[i];
		}
		m_bGetPSW=true;
		m_btSavePSW.EnableWindow(TRUE);
		if (strVer==_T("57"))
		{			
			//
			//PSW
			m_PSWBuf[0x00] = BufPassWord[0x07];
			m_PSWBuf[0x01] = BufPassWord[0x0F];
			m_PSWBuf[0x02] = BufPassWord[0x00];
			m_PSWBuf[0x03] = BufPassWord[0x01];
			m_PSWBuf[0x04] = BufPassWord[0x02];
			m_PSWBuf[0x05] = BufPassWord[0x03];
			m_PSWBuf[0x06] = BufPassWord[0x04];
			m_PSWBuf[0x07] = BufPassWord[0x05];
			m_PSWBuf[0x08] = BufPassWord[0x06];
			//SSID
			m_PSWBuf[0x09] = BufPassWord[0x0B];
			m_PSWBuf[0x0A] = BufPassWord[0x0C];
			m_PSWBuf[0x0B] = BufPassWord[0x0D];
			m_PSWBuf[0x0C] = BufPassWord[0x0E];
			//HASH
			m_PSWBuf[0x0D] = BufPassWord[0x0A];
			m_PSWBuf[0x0E] = BufPassWord[0x09];
			m_PSWBuf[0x0F] = BufPassWord[0x08];
			m_PSWBuf[0x10] = BufPassWord[0x17];
			m_PSWBuf[0x11] = BufPassWord[0x16];
			m_PSWBuf[0x12] = BufPassWord[0x15];
			m_PSWBuf[0x13] = BufPassWord[0x14];
			m_PSWBuf[0x14] = BufPassWord[0x13];
			//OTHERS
			m_PSWBuf[0x15] = BufPassWord[0x12];
			m_PSWBuf[0x16] = BufPassWord[0x11];
			m_PSWBuf[0x17] = BufPassWord[0x10];
		}
// 		textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x0d, 8, " ");//HASH
// 		textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x01, 8, " ");//PSD
// 		textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x09, 4, " ");//serial
// 		textBox2.Text = (BufPassWord[0x0B] & 0x07).ToString();//key
		CString str;
		str=CCommFunc::byteToHexStr(m_PSWBuf, 0x09, 4, _T(" "));//KeyID
		
		strTemp.LoadString(IDS_KeyDlg_KeyID);
		ShowLog(strTemp+_T(":")+str);
		
		str.Format(_T("%d"),m_PSWBuf[0x09] & 0x07);
		strTemp.LoadString(IDS_KeyDlg_KeyNum);
		ShowLog(strTemp+_T(":")+str);
		
		str=CCommFunc::byteToHexStr(m_PSWBuf, 0x01, 8, _T(" "));
		strTemp.LoadString(IDS_KeyDlg_PSW);
		ShowLog(strTemp+_T(":")+str);
		
		m_pMainDlg->m_strPSW=str;
		str=CCommFunc::byteToHexStr(m_PSWBuf, 0x0d, 8, _T(" "));
		ShowLog(_T("HASH:")+str);
	}
	else
	{
		ShowLog(rx.GetHIDResult());
		return;
	}
	strTemp.LoadString(IDS_ReadPSWOK);
	ShowLog(strTemp);
	KeyMessageDlg(strTemp);
}


void CKeyModuleDlg::OnBnSavePSW()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CFileDialog fileDlgS(FALSE);
	strTemp.LoadString(IDS_SavePSWTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;
	fileDlgS.m_ofn.lpstrFilter=_T("PSW Files(*.PSW)\0*.PSW\0All Files(*.*)\0*.*\0");
	fileDlgS.m_ofn.lpstrDefExt=_T("PSW");
	//KeyID
	CString strFileName=CCommFunc::byteToHexStr(m_PSWBuf, 0x09, 4, _T(""));//+CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
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
		ShowLog(strTemp);
		ShowLog(fileDlgS.GetPathName());
	}
	strFileName.ReleaseBuffer();	
}

void CKeyModuleDlg::KeyMessageDlg(CString str)
{
	MessageBox(str);
	m_rKeyID.SetFocus();
}
