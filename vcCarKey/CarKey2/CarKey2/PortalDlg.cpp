// PortalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "PortalDlg.h"
#include "afxdialogex.h"
#include ".\..\..\CarKeyCommon\CommFunc.h"

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

// CPortalDlg 对话框

IMPLEMENT_DYNAMIC(CPortalDlg, CDialogEx)

CPortalDlg::CPortalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPortalDlg::IDD, pParent)
	, m_strLog(_T(""))
{

}

CPortalDlg::~CPortalDlg()
{
	m_bkBrush.DeleteObject();
}

void CPortalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_EDIT1, m_Log);
	DDX_Text(pDX, IDC_EDIT1, m_strLog);
	DDX_Control(pDX, IDC_RADIO1, m_raKLine);
	DDX_Control(pDX, IDC_RADIO2, m_raIR);
	DDX_Control(pDX, IDC_RADIO3, m_raCAN);
	DDX_Control(pDX, IDC_RADIO4, m_raOBD);
	DDX_Control(pDX, IDC_RADIO5, m_raDirect);
	DDX_Control(pDX, IDC_RADIO6, m_raWork1);
	DDX_Control(pDX, IDC_RADIO7, m_raWork2);
	DDX_Control(pDX, IDC_RADIO8, m_raWork3);
	DDX_Control(pDX, IDC_STATIC13, m_CANConnect);
	DDX_Control(pDX, IDC_BUTTON11, m_btReadInfo);
	DDX_Control(pDX, IDC_BUTTON13, m_btCANWrite);
	DDX_Control(pDX, IDC_BUTTON23, m_btELV);
}


BEGIN_MESSAGE_MAP(CPortalDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON6, &CPortalDlg::OnBnReadData)
	ON_BN_CLICKED(IDC_BUTTON11, &CPortalDlg::OnBnReadCANInfo)
	ON_BN_CLICKED(IDC_BUTTON12, &CPortalDlg::OnBnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON13, &CPortalDlg::OnBnCANWrite)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO2, &CPortalDlg::OnBnIRRadio)
	ON_BN_CLICKED(IDC_RADIO3, &CPortalDlg::OnBnCANRadio)
	ON_BN_CLICKED(IDC_BUTTON16, &CPortalDlg::OnBnSaveData)
	ON_BN_CLICKED(IDC_BUTTON17, &CPortalDlg::OnBnLoadPSW)
	ON_BN_CLICKED(IDC_BUTTON14, &CPortalDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CPortalDlg::OnBnClickedButton15)
END_MESSAGE_MAP()


// CPortalDlg 消息处理程序
void CPortalDlg::ShowLog(CString str)
{
	/*m_Log.SetSel(-1, -1);
	m_Log.ReplaceSel(str+"\r\n");
	m_Log.UpdateWindow();*/
	m_strLog.Append(str+_T("\r\n"));
	UpdateData(FALSE);
	m_Log.UpdateWindow();
	m_Log.LineScroll(m_Log.GetLineCount());
}

void CPortalDlg::OnBnReadData()
{
	if(m_raIR.GetCheck()==TRUE)
	{
		IRRead();
	}
	else
	{
		CANRead();
	}
}

void CPortalDlg::OnBnReadCANInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp,strTemp1;
	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}
	ShowLog(_T("-----------------------------------------------"));
	strTemp.LoadString(IDS_StartCANInfo);
	ShowLog(strTemp);
	byte pBuf[0x40];
	//CInteractionData rx;
	m_Progress.SetRange(0,9);   //设置进度条的范围为0-1000	
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();
	CString str;
	int pBufIndex=0;
	for (int i=0;i<9;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANInfoDataStep(i);
		if(rx.GetHIDResult()==_T(""))
		{
			m_Progress.StepIt();
			m_Progress.UpdateWindow();
			if (CCarKey2App::TheHIDDevice.CANInfo_AT[i][15] != 0)
			{
				int ix = 8 * CCarKey2App::TheHIDDevice.CANInfo_AT[i][15];
				for (int i = 0; i < ix; i++)
				{
					pBuf[pBufIndex++] = rx.buff[i + 0x10];
				}
			}
		}
		else
		{
			ShowLog(rx.GetHIDResult());
			return;
		}
	}
	strTemp.LoadString(IDS_FinishCANInfo);
	ShowLog(strTemp);
	char ljh[11];
	ljh[0] = pBuf[0x0f];
	for (int i = 0; i < 7; i++)
		ljh[1 + i] = pBuf[0x11 + i];
	ljh[8] = pBuf[0x19];
	ljh[9] = pBuf[0x1a];
	ljh[10] = 0;
	str=ljh;
	strTemp.LoadString(IDS_MBLJH);
	ShowLog(strTemp+_T(":")+str);
	CString str1, str2, str3, str4;
	strTemp.LoadString(IDS_Initialised);
	if (CCommFunc::GetBit(pBuf[0x2c], 0))
	{		
		strTemp1.LoadString(IDS_Yes);
		str1 = strTemp+_T(":")+strTemp1;
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		str1 = strTemp+_T(":")+strTemp1;
	}

	strTemp.LoadString(IDS_TPRemoved);
	if (CCommFunc::GetBit(pBuf[0x2c], 1))
	{		
		strTemp1.LoadString(IDS_Yes);
		str2 = strTemp+_T(":")+strTemp1;
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		str2 = strTemp+_T(":")+strTemp1;
	}

	strTemp.LoadString(IDS_Personalised);
	if (CCommFunc::GetBit(pBuf[0x2c], 2))
	{		
		strTemp1.LoadString(IDS_Yes);
		str3 = strTemp+_T(":")+strTemp1;
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		str3 = strTemp+_T(":")+strTemp1;
	}

	strTemp.LoadString(IDS_Activated);
	if (CCommFunc::GetBit(pBuf[0x2c], 3))
	{		
		strTemp1.LoadString(IDS_Yes);
		str4 = strTemp+_T(":")+strTemp1;
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		str4 = strTemp+_T(":")+strTemp1;
	}	
	ShowLog(str1);
	ShowLog(str2);
	ShowLog(str3);
	ShowLog(str4);
	//MessageBox(_T("CAN信息数据读取完成!!"));
}


void CPortalDlg::OnBnOpenFile()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString StrName,strTemp;
	CString str;
	unsigned int dwFileLen;
	CFileDialog fileDlg(TRUE);
	strTemp.LoadString(IDS_OpenFileTitle);
	fileDlg.m_ofn.lpstrTitle=strTemp;
	fileDlg.m_ofn.lpstrFilter=_T("EZS Files(*.EZS)\0*.EZS\0BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	if (IDOK!=fileDlg.DoModal())
		return;
	CFile file(fileDlg.GetPathName(),CFile::modeRead);
	StrName=fileDlg.GetPathName();			//file path
	dwFileLen=(unsigned int)file.GetLength();	
	byte* pLoadBuf=new byte[dwFileLen+1];
	pLoadBuf[dwFileLen]=0;
	file.Read(pLoadBuf,dwFileLen);
	file.Close();
	if (dwFileLen == 0x800)//红外数据文件
	{
		byte temp[0x400];
		for (int i = 0, k = 0; k < 0x400; )
		{
			temp[k++] = pLoadBuf[i++];
			temp[k++] = pLoadBuf[i++];
			i += 2;
		}
		delete[] pLoadBuf;
		int BaseAddress = 0x10A;
		for (int j = 0; j < 9; j++)
		{
			byte tmpSUM;
			tmpSUM = CCommFunc::GetVerify_byteSum(temp, BaseAddress, 8);
			if (temp[BaseAddress - 0x008] != tmpSUM)
			{
				strTemp.LoadString(IDS_ErrorVerify);
				ShowLog(strTemp);
				MessageBox(strTemp);
				return;
			}
			BaseAddress += 0x12;
		}
		for (int j = 0; j < 8; j++)
		{
			byte tmpSUM;
			tmpSUM = CCommFunc::GetVerify_byteSum(temp, BaseAddress, 8);
			if (temp[BaseAddress - 0x008] != tmpSUM)
			{
				strTemp.LoadString(IDS_ErrorVerify);
				ShowLog(strTemp);
				MessageBox(strTemp);
				return;
			}
			BaseAddress += 0x12;
		}
		for(int i=0;i<0x400;i++)
			m_pDumpCalcDlg->m_EZSData.AllFile[i]=temp[i];		
		m_pDumpCalcDlg->m_SSID.SetContent(m_pDumpCalcDlg->m_EZSData.GetSSID());
		m_pDumpCalcDlg->m_Dealer.SetContent(m_pDumpCalcDlg->m_EZSData.GetDealer());
		m_pDumpCalcDlg->m_Key0.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey0());
		m_pDumpCalcDlg->m_Key1.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey1());
		m_pDumpCalcDlg->m_Key2.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey2());
		m_pDumpCalcDlg->m_Key3.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey3());
		m_pDumpCalcDlg->m_Key4.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey4());
		m_pDumpCalcDlg->m_Key5.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey5());
		m_pDumpCalcDlg->m_Key6.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey6());
		m_pDumpCalcDlg->m_Key7.SetContent(m_pDumpCalcDlg->m_EZSData.GetKey7());
		m_pDumpCalcDlg->m_Transport.SetContent(m_pDumpCalcDlg->m_EZSData.GetTransport());
		m_pDumpCalcDlg->m_SerialNum.SetContent(m_pDumpCalcDlg->m_EZSData.GetSerialNum());
//		str=m_pDumpCalcDlg->m_EZSData.GetPSW();
// 		if(str!=_T("00 00 00 00 00 00 00 00"))
// 			m_pDumpCalcDlg->m_PSW.SetContent(m_pDumpCalcDlg->m_EZSData.GetPSW());
// 		else
// 			m_pDumpCalcDlg->m_PSW.Clear();
//		m_pDumpCalcDlg->m_EZSData.SetKeyStatus(0,0xff);
		COLORREF clr;
		m_pDumpCalcDlg->m_Key0Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(0,&clr));
		m_pDumpCalcDlg->m_Key0Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key1Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(1,&clr));
		m_pDumpCalcDlg->m_Key1Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key2Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(2,&clr));
		m_pDumpCalcDlg->m_Key2Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key3Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(3,&clr));
		m_pDumpCalcDlg->m_Key3Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key4Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(4,&clr));
		m_pDumpCalcDlg->m_Key4Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key5Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(5,&clr));
		m_pDumpCalcDlg->m_Key5Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key6Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(6,&clr));
		m_pDumpCalcDlg->m_Key6Status.SetBackgroundColor(clr);
		m_pDumpCalcDlg->m_Key7Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(7,&clr));
		m_pDumpCalcDlg->m_Key7Status.SetBackgroundColor(clr);
		byte status=m_pDumpCalcDlg->m_EZSData.GetStatus();
		if (CCommFunc::GetBit(status, 0))
			m_pDumpCalcDlg->m_bInitialised = true;
		else
			m_pDumpCalcDlg->m_bInitialised = false;
		if (CCommFunc::GetBit(status, 1))
			m_pDumpCalcDlg->m_bTPremoved = true;
		else
			m_pDumpCalcDlg->m_bTPremoved = false;
		if (CCommFunc::GetBit(status, 2))
			m_pDumpCalcDlg->m_bPersonalised = true;
		else
			m_pDumpCalcDlg->m_bPersonalised = false;
		if (CCommFunc::GetBit(status, 3))
			m_pDumpCalcDlg->m_bActivated = true;
		else
			m_pDumpCalcDlg->m_bActivated = false;
		m_pDumpCalcDlg->UpdateData(FALSE);
		m_pDumpCalcDlg->m_LastKey.SetContent(m_pDumpCalcDlg->m_EZSData.GetLastKey());
		m_pDumpCalcDlg->m_PreviousKey.SetContent(m_pDumpCalcDlg->m_EZSData.GetPreviousKey());
		ShowLog(_T("-----------------------------------------------"));
		strTemp.LoadString(IDS_OpenFileOK);
		ShowLog(strTemp);
		ShowLog(StrName);
		strTemp.LoadString(IDS_ImportData);
		ShowLog(strTemp);
		ShowLog(m_pDumpCalcDlg->GetDataLog());
	}  
}


BOOL CPortalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Progress.SetStep(1);
	m_bkBrush.CreateSolidBrush(RGB(166,202,240));	
	CString strTemp;
	strTemp.LoadString(IDS_PortDlg_Portal);
	SetDlgItemText(IDC_STATIC12,strTemp); 

	strTemp.LoadString(IDS_PortDlg_CanConnect);
	SetDlgItemText(IDC_STATIC13,strTemp); 

	strTemp.LoadString(IDS_PortDlg_WorkType);
	SetDlgItemText(IDC_STATIC14,strTemp); 

	strTemp.LoadString(IDS_PortDlg_Work);
	SetDlgItemText(IDC_STATIC15,strTemp); 

	strTemp.LoadString(IDS_PortDlg_Log);
	SetDlgItemText(IDC_STATIC16,strTemp); 


	strTemp.LoadString(IDS_PortDlg_ReadInfo);
	SetDlgItemText(IDC_BUTTON11,strTemp); 

	strTemp.LoadString(IDS_PortDlg_ReadData);
	SetDlgItemText(IDC_BUTTON6,strTemp);

	strTemp.LoadString(IDS_PortDlg_LoadPSW);
	SetDlgItemText(IDC_BUTTON17,strTemp);

	strTemp.LoadString(IDS_PortDlg_WriteEZS);
	SetDlgItemText(IDC_BUTTON13,strTemp);

	strTemp.LoadString(IDS_PortDlg_ReadCode);
	SetDlgItemText(IDC_BUTTON22,strTemp);

	strTemp.LoadString(IDS_PortDlg_WriteCode);
	SetDlgItemText(IDC_BUTTON21,strTemp);

	strTemp.LoadString(IDS_PortDlg_BGA);
	SetDlgItemText(IDC_BUTTON3,strTemp);

	strTemp.LoadString(IDS_PortDlg_WriteVIN);
	SetDlgItemText(IDC_BUTTON18,strTemp);

	strTemp.LoadString(IDS_PortDlg_WriteELV);
	SetDlgItemText(IDC_BUTTON23,strTemp);

	strTemp.LoadString(IDS_PortDlg_ReadOdo);
	SetDlgItemText(IDC_BUTTON24,strTemp);

	strTemp.LoadString(IDS_PortDlg_WriteOdo);
	SetDlgItemText(IDC_BUTTON25,strTemp);

	strTemp.LoadString(IDS_PortDlg_OpenDataFile);
	SetDlgItemText(IDC_BUTTON12,strTemp);

	strTemp.LoadString(IDS_PortDlg_SaveData);
	SetDlgItemText(IDC_BUTTON16,strTemp);

	strTemp.LoadString(IDS_PortDlg_SaveLog);
	SetDlgItemText(IDC_BUTTON15,strTemp);

	strTemp.LoadString(IDS_PortDlg_ClearLog);
	SetDlgItemText(IDC_BUTTON14,strTemp);

	strTemp.LoadString(IDS_PortDlg_IR);
	SetDlgItemText(IDC_RADIO2,strTemp);

	strTemp.LoadString(IDS_PortDlg_OBD);
	SetDlgItemText(IDC_RADIO4,strTemp);

	strTemp.LoadString(IDS_PortDlg_DirectEZS);
	SetDlgItemText(IDC_RADIO5,strTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPortalDlg::SetCANShow( byte* outBuf )
{
	m_pDumpCalcDlg->m_SSID.SetContent(CCommFunc::byteToHexStr(outBuf, 0, 4, _T(" ")));//(ezs.SSID, " ");
	//textBox2.Text = "";//CCommFunc::byteToHexStr(outBuf, 20, 8, _T(" "));//(ezs.PSW, " ");
	m_pDumpCalcDlg->m_Key0.SetContent(CCommFunc::byteToHexStr(outBuf, 0xA0, 8, _T(" ")));//(ezs.Key0, " ");
	m_pDumpCalcDlg->m_Key1.SetContent(CCommFunc::byteToHexStr(outBuf, 0x0130, 8, _T(" ")));//(ezs.Key1, " ");
	m_pDumpCalcDlg->m_Key2.SetContent(CCommFunc::byteToHexStr(outBuf, 0x01C0, 8, _T(" ")));//(ezs.Key2, " ");
	m_pDumpCalcDlg->m_Key3.SetContent(CCommFunc::byteToHexStr(outBuf, 0x01D0, 8, _T(" ")));//(ezs.Key3, " ");
	m_pDumpCalcDlg->m_Key4.SetContent(CCommFunc::byteToHexStr(outBuf, 0xE0, 8, _T(" ")));//(ezs.Key4, " ");
	m_pDumpCalcDlg->m_Key5.SetContent(CCommFunc::byteToHexStr(outBuf, 0xF0, 8, _T(" ")));//(ezs.Key5, " ");
	m_pDumpCalcDlg->m_Key6.SetContent(CCommFunc::byteToHexStr(outBuf, 0x80, 8, _T(" ")));//(ezs.Key6, " ");
	m_pDumpCalcDlg->m_Key7.SetContent(CCommFunc::byteToHexStr(outBuf, 0x90, 8, _T(" ")));//(ezs.Key7, " ");
	m_pDumpCalcDlg->m_SerialNum.SetContent(CCommFunc::byteToHexStr(outBuf, 0x230, 8, _T(" ")));//(ezs.SerialNum, " ");
	m_pDumpCalcDlg->m_Dealer.SetContent(CCommFunc::byteToHexStr(outBuf, 0x10, 8, _T(" ")));//(ezs.Dealer, " ");
	m_pDumpCalcDlg->m_Transport.SetContent(CCommFunc::byteToHexStr(outBuf, 0x260, 8, _T(" ")));//(ezs.Transport, " ");

	CString str;

	m_pDumpCalcDlg->m_EZSData.SetKeyStatus(outBuf[4],outBuf[6]);
	COLORREF clr;
	m_pDumpCalcDlg->m_Key0Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(0,&clr));
	m_pDumpCalcDlg->m_Key0Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key1Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(1,&clr));
	m_pDumpCalcDlg->m_Key1Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key2Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(2,&clr));
	m_pDumpCalcDlg->m_Key2Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key3Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(3,&clr));
	m_pDumpCalcDlg->m_Key3Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key4Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(4,&clr));
	m_pDumpCalcDlg->m_Key4Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key5Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(5,&clr));
	m_pDumpCalcDlg->m_Key5Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key6Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(6,&clr));
	m_pDumpCalcDlg->m_Key6Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_Key7Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(7,&clr));
	m_pDumpCalcDlg->m_Key7Status.SetBackgroundColor(clr);
	m_pDumpCalcDlg->m_EZSData.SetStatus(outBuf[5]);
	byte status=m_pDumpCalcDlg->m_EZSData.GetStatus();
	if (CCommFunc::GetBit(status, 0))
		m_pDumpCalcDlg->m_bInitialised = true;
	else
		m_pDumpCalcDlg->m_bInitialised = false;
	if (CCommFunc::GetBit(status, 1))
		m_pDumpCalcDlg->m_bTPremoved = true;
	else
		m_pDumpCalcDlg->m_bTPremoved = false;
	if (CCommFunc::GetBit(status, 2))
		m_pDumpCalcDlg->m_bPersonalised = true;
	else
		m_pDumpCalcDlg->m_bPersonalised = false;
	if (CCommFunc::GetBit(status, 3))
		m_pDumpCalcDlg->m_bActivated = true;
	else
		m_pDumpCalcDlg->m_bActivated = false;
	m_pDumpCalcDlg->UpdateData(FALSE);


	byte b=outBuf[7]&0x0f;
	if(b>7)
		m_pDumpCalcDlg->m_LastKey.Clear();
	else
	{
		str.Format(_T("%d"),b);
		m_pDumpCalcDlg->m_LastKey.SetContent(str);
	}
	b=outBuf[7]>>4;
	b=b&0x0f;
	if(b>7)
		m_pDumpCalcDlg->m_PreviousKey.Clear();
	else
	{
		str.Format(_T("%d"),b);
		m_pDumpCalcDlg->m_PreviousKey.SetContent(str);	
	}
}


void CPortalDlg::OnBnCANWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckData()!=true)
		return;
	CString str,strTemp,strTemp1;
	m_pDumpCalcDlg->m_PSW.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetPSW(str))
	{
		strTemp.LoadString(IDS_PSWAreaErr);
		ShowLog(strTemp);
		MessageBox(strTemp);
		return;
	}
	else if(m_pDumpCalcDlg->m_EZSData.AllFile[0x8e] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+1] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+2] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+3] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+4] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+5] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+6] == 0 && m_pDumpCalcDlg->m_EZSData.AllFile[0x8e+7] == 0)
	{
		strTemp.LoadString(IDS_ErrPSWWarring);
		strTemp1.LoadString(IDS_Warring);
		if (MessageBox(strTemp,strTemp1, MB_YESNO) == IDNO)
			return;
		strTemp.LoadString(IDS_ErrPSWLog);
		ShowLog(strTemp);
	}
	


	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}
	CCarKey2App::TheHIDDevice.SetEISWriteBuf(m_pDumpCalcDlg->m_EZSData.AllFile);
	//m_pDumpCalcDlg->m_EZSData.SetEISWriteBuf();
	ShowLog(_T("-----------------------------------------------"));
	
	strTemp.LoadString(IDS_StartCANWrite);
	ShowLog(strTemp);
	
	int pBufIndex = 0;
	//CInteractionData rx;
	m_Progress.SetRange(0,6);   //设置进度条的范围为0-1000	
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();
	for (int i=0;i<6;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANWrite1Step(i);
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

	strTemp.LoadString(IDS_WaitInsertKey);
	ShowLog(strTemp);

	int waitstep = 0;
	bool Shk = false;
	m_Progress.SetRange(0,200);
	m_Progress.SetPos(200);
	int readCount = 0;
	while (true != Shk)
	{
		rx = CCarKey2App::TheHIDDevice.CANWrite2Step(waitstep);
		if(rx.GetHIDResult()==_T(""))
		{

		}
		else if(rx.hidResult == HIDResult_CANWriteWait && waitstep==0)
		{

		}
		else
		{
			ShowLog(rx.GetHIDResult());
			return;
		}
		
		readCount++;
		if (readCount > 200)
		{
			strTemp.LoadString(IDS_InsertKeyTimeOut);
			ShowLog(strTemp);
			return;
		}
		else
		{
			m_Progress.SetPos(200-readCount);
		}
		for (int i = 0; i < CCarKey2App::TheHIDDevice.CANwr_bChk[waitstep][0]; i++)
		{
			if (CCarKey2App::TheHIDDevice.CANwr_bChk[waitstep][i + 0x01] != rx.buff[i + 0x10])
			{
				Shk = true;
			}
		}
		if (Shk == true)
		{
			for (int i = 0; i < CCarKey2App::TheHIDDevice.CANwr_bChk[3][0x00]; i++)
			{
				if (CCarKey2App::TheHIDDevice.CANwr_bChk[3][i + 0x01] != rx.buff[i + 0x10])
				{
					Shk = false;
					strTemp.LoadString(IDS_InsertKeyTimeOut);
					ShowLog(strTemp);
					return;
				}
			}
		}
		//
		waitstep++;
		if (waitstep >= 0x03)
		{
			waitstep = 0;
		}
	}
	strTemp.LoadString(IDS_CANWritting);
	ShowLog(strTemp);
	m_Progress.SetRange(0,30);
	m_Progress.SetPos(0);
	CInteractionData rx1;
	for (int i=0;i<2;i++)
	{
		rx1 = CCarKey2App::TheHIDDevice.CANWrite3Step(i);
		if(rx1.GetHIDResult()==_T(""))
		{
			m_Progress.StepIt();
			m_Progress.UpdateWindow();
		}
		else
		{
			ShowLog(rx1.GetHIDResult());
			return;
		}
	}
	for (int i = 0; i < CCarKey2App::TheHIDDevice.CANwr_bChk[4][0x00]; i++)
	{
		if (CCarKey2App::TheHIDDevice.CANwr_bChk[4][i + 0x01] != rx1.buff[i + 0x10])
		{
			strTemp.LoadString(IDS_haveWriteErr);
			ShowLog(strTemp);
			return;
		}
	}
	CCarKey2App::TheHIDDevice.DTwrIndex = 0;
	for (int i=0;i<24;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANWrite4Step(i);
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
	CCarKey2App::TheHIDDevice.DTwrIndex = 0;
	for (int i=0;i<4;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANWrite5Step(i);
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
	strTemp.LoadString(IDS_FinishCANWrite);
	ShowLog(strTemp);
	MessageBox(strTemp);
}


HBRUSH CPortalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor ==CTLCOLOR_DLG)
		return m_bkBrush; //返加绿色刷子
	if (nCtlColor = CTLCOLOR_STATIC)
	{
		switch(pWnd->GetDlgCtrlID()) 
		{
		case IDC_STATIC12:
		case IDC_STATIC13:
		case IDC_STATIC14:
		case IDC_STATIC15:
		case IDC_STATIC16:
			//case IDC_CHECK10:
			pDC->SetBkColor(RGB(166,202,240));
			pDC->SetBkMode(TRANSPARENT);      //不显示文字背景
			hbr = (HBRUSH)::GetStockObject(NULL_BRUSH); //编辑框背景。注意：和文字背景不是一个意思。
			break;
		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
		case IDC_RADIO4:
		case IDC_RADIO5:
		case IDC_RADIO6:
		case IDC_RADIO7:
		case IDC_RADIO8:
			// 			pDC->SetBkColor(RGB(166,202,240));
			// 			pDC->SetBkMode(TRANSPARENT);      //不显示文字背景
			// 			hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
			pDC->SetBkMode(TRANSPARENT);
			CRect rc;
			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);
			CDC* dc = GetDC();
			pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);
			//把父窗口背景图片先画到按钮上
			ReleaseDC(dc);
			hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH); 
			break;
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CPortalDlg::OnBnIRRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_raIR.GetCheck())
	{
		m_raOBD.EnableWindow(FALSE);
		m_raDirect.EnableWindow(FALSE);
		m_btReadInfo.EnableWindow(FALSE);
		m_btCANWrite.EnableWindow(FALSE);
	}
}


void CPortalDlg::OnBnCANRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_raCAN.GetCheck())
	{
		if(m_pPortalOP->bOBD)
			m_raOBD.EnableWindow(TRUE);
		else
			m_raOBD.EnableWindow(FALSE);
		if(m_pPortalOP->bDirect)
			m_raDirect.EnableWindow(TRUE);
		else
			m_raDirect.EnableWindow(FALSE);
		m_btReadInfo.EnableWindow(TRUE);
		m_btCANWrite.EnableWindow(TRUE);
	}
}

void CPortalDlg::CANRead()
{
	CString strTemp;
	CInteractionData rx;
	rx=CCarKey2App::TheHIDDevice.Check_mima();
	if(rx.mimaok!=true)
	{
		CString strx=rx.GetHIDResult();
		ShowLog(strx);
		MessageBox(strx);
		return;
	}
	ShowLog(_T("-----------------------------------------------"));

	strTemp.LoadString(IDS_StartReadData);
	ShowLog(strTemp);

	byte pBuf[0x400];
	for (int i = 0; i < 0x400; i++)
	{
		pBuf[i] = 0xff;
	}
	int pBufIndex = 0;
	//CInteractionData rx;
	m_Progress.SetRange(0,114);   //设置进度条的范围为0-1000	
	m_Progress.SetPos(0);
	m_Progress.UpdateWindow();
	CString str;
	for (int i=0;i<2;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANEeprom1Step(i);
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
	for (int i=0;i<108;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANEeprom2Step(i);
		if(rx.GetHIDResult()==_T(""))
		{
			m_Progress.StepIt();
			m_Progress.UpdateWindow();
			if (rx.rxCount != 0)
			{
				int ix = 8 * rx.rxCount;
				for (int j = 0; j < ix; j++)
				{
					pBuf[pBufIndex++] = rx.buff[j + 0x10];
				}
			}
		}
		else
		{
			ShowLog(rx.GetHIDResult());
			return;
		}
	}
	for (int i=0;i<4;i++)
	{
		rx = CCarKey2App::TheHIDDevice.CANEeprom3Step(i);
		if(rx.GetHIDResult()==_T(""))
		{
			m_Progress.StepIt();
			m_Progress.UpdateWindow();
			if (rx.rxCount != 0)
			{
				int ix = 8 * rx.rxCount;
				for (int j = 0; j < ix; j++)
				{
					pBuf[pBufIndex++] = rx.buff[j + 0x10];
				}
			}
		}
		else
		{
			ShowLog(rx.GetHIDResult());
			return;
		}
	}
	byte outBuf[0x400];
	for (int i = 0; i < 0x400; i++)
	{
		outBuf[i] = 0x00;
	}
	for (int i = 0; i < 0x24; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			outBuf[i * 0x10 + j] = pBuf[i * 0x10 + 0x06 + j];
		}
		for (int j = 0; j < 6; j++)
		{
			outBuf[i * 0x10 + 0x02 + j] = pBuf[i * 0x10 + 0x09 + j];
		}
	}
	//
	for (int j = 0; j < 4; j++)
	{
		outBuf[0x30 * 0x10 + j] = pBuf[0x24 * 0x10 + 0x04 + j];
	}
	for (int j = 0; j < 7; j++)
	{
		outBuf[0x30 * 0x10 + 0x04 + j] = pBuf[0x24 * 0x10 + 0x09 + j];
	}
	for (int j = 0; j < 6; j++)
	{
		outBuf[0x30 * 0x10 + 0x0B + j] = pBuf[0x24 * 0x10 + 0x11 + j];
	}
	//
	for (int i = 0; i < 0x80; i++)
	{
		outBuf[0x27 * 0x10 + i] = outBuf[0x02 * 0x10 + i];
	}
	for (int i = 0; i < 0x10; i++)
	{
		outBuf[0x26 * 0x10 + i] = outBuf[0x23 * 0x10 + i];
	}
	for (int i = 0; i < 0x10; i++)
	{
		outBuf[0x23 * 0x10 + i] = outBuf[0x22 * 0x10 + i];
		outBuf[0x22 * 0x10 + i] = 0x00;
	}
	SetCANShow(outBuf);

	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);

	ShowLog(m_pDumpCalcDlg->GetDataLog());
	//MessageBox(_T("EEPROM数据读取完成!!"));
}

void CPortalDlg::IRRead()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	CInteractionData rx;
 	rx=CCarKey2App::TheHIDDevice.Check_mima();
 	if(rx.mimaok!=true)
 	{
 		CString strx=rx.GetHIDResult();
 		ShowLog(strx);
 		MessageBox(strx);
 		return;
 	}
	ShowLog(_T("-----------------------------------------------"));
	m_pDumpCalcDlg->ClearCtrl();

	strTemp.LoadString(IDS_StartReadData);
	ShowLog(strTemp);

	strTemp.LoadString(IDS_WaitInsertKey);
	ShowLog(strTemp);

	//CInteractionData rx;
	m_Progress.SetRange(0,40);   //设置进度条的范围为0-1000	
	m_Progress.SetPos(40);
	m_Progress.UpdateWindow();
	CCarKey2App::TheHIDDevice.m_pProgress=&m_Progress;
 	rx = CCarKey2App::TheHIDDevice.EZSReadDataStep(0);
 	CString str;
 	if(rx.GetHIDResult()==_T(""))
 	{		
 		str=CCommFunc::byteToHexStr(rx.buff,0x10,4,_T(" "));
 		m_pDumpCalcDlg->m_SSID.SetContent(str);
 		m_pDumpCalcDlg->m_EZSData.SetKeyStatus(rx.buff[0x10 + 4],rx.buff[0x10 + 6]);
 		COLORREF clr;
 		m_pDumpCalcDlg->m_Key0Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(0,&clr));
 		m_pDumpCalcDlg->m_Key0Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key1Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(1,&clr));
 		m_pDumpCalcDlg->m_Key1Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key2Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(2,&clr));
 		m_pDumpCalcDlg->m_Key2Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key3Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(3,&clr));
 		m_pDumpCalcDlg->m_Key3Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key4Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(4,&clr));
 		m_pDumpCalcDlg->m_Key4Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key5Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(5,&clr));
 		m_pDumpCalcDlg->m_Key5Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key6Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(6,&clr));
 		m_pDumpCalcDlg->m_Key6Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_Key7Status.SetWindowText(m_pDumpCalcDlg->m_EZSData.GetKeyStatus(7,&clr));
 		m_pDumpCalcDlg->m_Key7Status.SetBackgroundColor(clr);
 		m_pDumpCalcDlg->m_EZSData.SetStatus(rx.buff[0x10 + 5]);
 		byte status=m_pDumpCalcDlg->m_EZSData.GetStatus();
 		if (CCommFunc::GetBit(status, 0))
 			m_pDumpCalcDlg->m_bInitialised = true;
 		else
 			m_pDumpCalcDlg->m_bInitialised = false;
 		if (CCommFunc::GetBit(status, 1))
 			m_pDumpCalcDlg->m_bTPremoved = true;
 		else
 			m_pDumpCalcDlg->m_bTPremoved = false;
 		if (CCommFunc::GetBit(status, 2))
 			m_pDumpCalcDlg->m_bPersonalised = true;
 		else
 			m_pDumpCalcDlg->m_bPersonalised = false;
 		if (CCommFunc::GetBit(status, 3))
 			m_pDumpCalcDlg->m_bActivated = true;
 		else
 			m_pDumpCalcDlg->m_bActivated = false;
 		m_pDumpCalcDlg->UpdateData(FALSE);
 		byte b=rx.buff[0x10 + 7]&0x0f;
 		if(b>7)
 			m_pDumpCalcDlg->m_LastKey.Clear();
 		else
 		{
 			str.Format(_T("%d"),b);
 			m_pDumpCalcDlg->m_LastKey.SetContent(str);
 		}
 		b=rx.buff[0x10 + 7]>>4;
 		b=b&0x0f;
 		if(b>7)
 			m_pDumpCalcDlg->m_PreviousKey.Clear();
 		else
 		{
 			str.Format(_T("%d"),b);
 			m_pDumpCalcDlg->m_PreviousKey.SetContent(str);	
 		}
 	}
 	else
 	{
 		ShowLog(rx.GetHIDResult());
 		return;
 	}
	CCarKey2App::TheHIDDevice.m_pProgress=NULL;
	strTemp.LoadString(IDS_ReadDating);
	ShowLog(strTemp);
	m_Progress.SetRange(0,12);   //设置进度条的范围为0-1000	
	m_Progress.SetPos(1);
	m_Progress.UpdateWindow();
	//WaitMS(1);
	for (int i=1;i<12;i++)
	{
		int step;
		if(m_pPortalOP->IROp==IROptions_NEC)
			step=CCarKey2App::TheHIDDevice.DTnec[i];
		else 
			step=CCarKey2App::TheHIDDevice.DTmot[i];
		rx = CCarKey2App::TheHIDDevice.EZSReadDataStep(step);
		if(rx.GetHIDResult()==_T(""))
		{
			m_Progress.StepIt();
			m_Progress.UpdateWindow();
			switch (i)
			{                        
			case 1:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key0.SetContent(str);
				break;
			case 2:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key1.SetContent(str);
				break;
			case 3:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key2.SetContent(str);
				break;
			case 4:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key3.SetContent(str);
				break;
			case 5:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key4.SetContent(str);
				break;
			case 6:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key5.SetContent(str);
				break;
			case 7:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key6.SetContent(str);
				break;
			case 8:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Key7.SetContent(str);
				break;
			case 9:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_SerialNum.SetContent(str);
				break;
			case 10:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Dealer.SetContent(str);
				break;
			case 11:
				str=CCommFunc::byteToHexStr(rx.buff, 0x10, 8, _T(" "));
				m_pDumpCalcDlg->m_Transport.SetContent(str);
				break;
			default:
				break;
			}
		}
		else
		{
			ShowLog(rx.GetHIDResult());
			return;
		}
	}
	m_Progress.SetPos(12);
	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	ShowLog(m_pDumpCalcDlg->GetDataLog());
	//MessageBox(_T("数据读取完成!!可以在数据页查看"));
}
bool CPortalDlg::CheckData()
{
	CString str,strTemp,strTemp1,strTemp2;
	//m_pDumpCalcDlg->m_EZSData.Clear();
	m_pDumpCalcDlg->m_SSID.GetWindowText(str);
	strTemp.LoadString(IDS_ErrDataArea);
	if(!m_pDumpCalcDlg->m_EZSData.SetSSID(str))
	{
		strTemp2.LoadString(IDS_SSID);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_SerialNum.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetSerialNum(str))
	{
		strTemp2.LoadString(IDS_SerialNum);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Dealer.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetDealer(str))
	{
		strTemp2.LoadString(IDS_Dealer);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Transport.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetTransport(str))
	{
		strTemp2.LoadString(IDS_Transport);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key0.GetWindowText(str);
	strTemp1.LoadString(IDS_Key);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey0(str))
	{
		MessageBox(strTemp1+_T("0")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key1.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey1(str))
	{
		MessageBox(strTemp1+_T("1")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key2.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey2(str))
	{
		MessageBox(strTemp1+_T("2")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key3.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey3(str))
	{
		MessageBox(strTemp1+_T("3")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key4.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey4(str))
	{
		MessageBox(strTemp1+_T("4")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key5.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey5(str))
	{
		MessageBox(strTemp1+_T("5")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key6.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey6(str))
	{
		MessageBox(strTemp1+_T("6")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_Key7.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetKey7(str))
	{
		MessageBox(strTemp1+_T("7")+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_LastKey.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetLastKey(str))
	{
		strTemp2.LoadString(IDS_LastKey);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	m_pDumpCalcDlg->m_PreviousKey.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetPreviousKey(str))
	{
		strTemp2.LoadString(IDS_PreviousKey);
		MessageBox(strTemp2+strTemp);
		return false;
	}
	return true;
}

void CPortalDlg::OnBnSaveData()
{
	// TODO: 在此添加控件通知处理程序代码
	if(CheckData()!=true)
		return;
	m_pDumpCalcDlg->m_EZSData.GetSKCData();
	CString str,strTemp,strTemp1;
	CFileDialog fileDlgS(FALSE);
	m_pDumpCalcDlg->m_PSW.GetWindowText(str);
	if(!m_pDumpCalcDlg->m_EZSData.SetPSW(str))
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
	m_pDumpCalcDlg->m_SSID.GetWindowText(str);
	str.Replace(_T(" "),_T(""));
	CString strFileName=str+_T("_SKC");//+CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
	// 	CTime tm=CTime::GetCurrentTime();
	// 	CString str=tm.Format("%Y-%m-%d");
	// 	strFileName+=str;
	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
	if (IDOK==fileDlgS.DoModal())
	{
		byte save[0x800];
		for (int i = 0; i < 0x800; i++)
			save[i] = 0xff;
		for (int i = 0, k = 0; i < 0x400; )
		{
			save[k++] = m_pDumpCalcDlg->m_EZSData.AllFile[i++];
			save[k++] = m_pDumpCalcDlg->m_EZSData.AllFile[i++];
			k += 2;
		}
		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		file.Write(save,0x800);
		file.Close();
		ShowLog(_T("-----------------------------------------------"));
		strTemp.LoadString(IDS_SaveFileOK);
		ShowLog(strTemp);
		ShowLog(fileDlgS.GetPathName());
	}	
	strFileName.ReleaseBuffer();
}


void CPortalDlg::OnBnLoadPSW()
{
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
		str = CCommFunc::byteToHexStr(pLoadBuf, 1, 8, _T(" "));
		m_pDumpCalcDlg->m_PSW.SetContent(str);
		ShowLog(_T("-----------------------------------------------"));
		strTemp.LoadString(IDS_PSWLoadOK);
		ShowLog(strTemp);
		ShowLog(str);
	}                       
	else
	{
		delete[] pLoadBuf;
		pLoadBuf=NULL;
		strTemp.LoadString(IDS_PSWLoadErr);
		MessageBox(strTemp);  
	}
}


void CPortalDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Log.SetWindowText(_T(""));
}


void CPortalDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlgS(FALSE);
	CString strTemp;
	strTemp.LoadString(IDS_SaveLogTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;
	fileDlgS.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
	fileDlgS.m_ofn.lpstrDefExt=_T("txt");
	CString strFileName=m_strModelName+_T("_");//+CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
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
