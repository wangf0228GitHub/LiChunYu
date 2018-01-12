// DumpCalcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "DumpCalcDlg.h"
#include "afxdialogex.h"
#include ".\..\..\CarKeyCommon\CommFunc.h"


// CDumpCalcDlg 对话框

IMPLEMENT_DYNAMIC(CDumpCalcDlg, CDialogEx)

CDumpCalcDlg::CDumpCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDumpCalcDlg::IDD, pParent)
	, m_bInitialised(FALSE)
	, m_bTPremoved(FALSE)
	, m_bPersonalised(FALSE)
	, m_bActivated(FALSE)
{

}

CDumpCalcDlg::~CDumpCalcDlg()
{
	m_bkBrush.DeleteObject();
}

void CDumpCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT8, m_Key0);
	DDX_Control(pDX, IDC_Key0Status, m_Key0Status);
	DDX_Control(pDX, IDC_Key1Status, m_Key1Status);
	DDX_Control(pDX, IDC_Key2Status, m_Key2Status);
	DDX_Control(pDX, IDC_Key3Status, m_Key3Status);
	DDX_Control(pDX, IDC_Key4Status, m_Key4Status);
	DDX_Control(pDX, IDC_Key5Status, m_Key5Status);
	DDX_Control(pDX, IDC_Key6Status, m_Key6Status);
	DDX_Control(pDX, IDC_Key7Status, m_Key7Status);
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
}


BEGIN_MESSAGE_MAP(CDumpCalcDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON6, &CDumpCalcDlg::OnBnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON7, &CDumpCalcDlg::OnBnPSWIn)
	ON_BN_CLICKED(IDC_BUTTON10, &CDumpCalcDlg::OnBnSaveData)
	ON_BN_CLICKED(IDC_BUTTON8, &CDumpCalcDlg::OnBnExportTXT)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT9, &CDumpCalcDlg::OnEnChangeEdit9)
END_MESSAGE_MAP()


// CDumpCalcDlg 消息处理程序


void CDumpCalcDlg::OnBnOpenFile()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString StrName;
	//CString str;
	//unsigned int dwFileLen;
	//CFileDialog fileDlg(TRUE);
	//fileDlg.m_ofn.lpstrTitle=_T("电子点火开关数据文件打开:*.BIN");
	//fileDlg.m_ofn.lpstrFilter=_T("EZS Files(*.EZS)\0*.EZS\0BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	//if (IDOK!=fileDlg.DoModal())
	//	return;
	//CFile file(fileDlg.GetPathName(),CFile::modeRead);
	//StrName=fileDlg.GetPathName();			//file path
	//dwFileLen=(unsigned int)file.GetLength();	
	//byte* pLoadBuf=new byte[dwFileLen+1];
	//pLoadBuf[dwFileLen]=0;
	//file.Read(pLoadBuf,dwFileLen);
	//file.Close();
	//if (dwFileLen == 0x800)//红外数据文件
	//{
	//	for (int i = 0, k = 0; k < 0x400; )
	//	{
	//		m_EZSData.AllFile[k++] = pLoadBuf[i++];
	//		m_EZSData.AllFile[k++] = pLoadBuf[i++];
	//		i += 2;
	//	}
	//	delete[] pLoadBuf;
	//	int BaseAddress = 0x10A;
	//	for (int j = 0; j < 9; j++)
	//	{
	//		byte tmpSUM;
	//		tmpSUM = CCommFunc::GetVerify_byteSum(m_EZSData.AllFile, BaseAddress, 8);
	//		if (m_EZSData.AllFile[BaseAddress - 0x008] != tmpSUM)
	//		{
	//			MessageBox(_T("数据校验错误!"));
	//			return;
	//		}
	//		BaseAddress += 0x12;
	//	}
	//	for (int j = 0; j < 8; j++)
	//	{
	//		byte tmpSUM;
	//		tmpSUM = CCommFunc::GetVerify_byteSum(m_EZSData.AllFile, BaseAddress, 8);
	//		if (m_EZSData.AllFile[BaseAddress - 0x008] != tmpSUM)
	//		{
	//			MessageBox(_T("数据校验错误!"));
	//			return;
	//		}
	//		BaseAddress += 0x12;
	//	}
	//	m_SSID.SetContent(m_EZSData.GetSSID());
	//	m_Dealer.SetContent(m_EZSData.GetDealer());
	//	m_Key0.SetContent(m_EZSData.GetKey0());
	//	m_Key1.SetContent(m_EZSData.GetKey1());
	//	m_Key2.SetContent(m_EZSData.GetKey2());
	//	m_Key3.SetContent(m_EZSData.GetKey3());
	//	m_Key4.SetContent(m_EZSData.GetKey4());
	//	m_Key5.SetContent(m_EZSData.GetKey5());
	//	m_Key6.SetContent(m_EZSData.GetKey6());
	//	m_Key7.SetContent(m_EZSData.GetKey7());
	//	m_Transport.SetContent(m_EZSData.GetTransport());
	//	m_SerialNum.SetContent(m_EZSData.GetSerialNum());
	//	str=m_EZSData.GetPSW();
	//	if(str!=_T("00 00 00 00 00 00 00 00"))
	//		m_PSW.SetContent(m_EZSData.GetPSW());
	//	else
	//		m_PSW.Clear();

	//	COLORREF clr;
	//	m_Key0Status.SetWindowText(m_EZSData.GetKeyStatus(0,&clr));
	//	m_Key0Status.SetBackgroundColor(clr);
	//	m_Key1Status.SetWindowText(m_EZSData.GetKeyStatus(1,&clr));
	//	m_Key1Status.SetBackgroundColor(clr);
	//	m_Key2Status.SetWindowText(m_EZSData.GetKeyStatus(2,&clr));
	//	m_Key2Status.SetBackgroundColor(clr);
	//	m_Key3Status.SetWindowText(m_EZSData.GetKeyStatus(3,&clr));
	//	m_Key3Status.SetBackgroundColor(clr);
	//	m_Key4Status.SetWindowText(m_EZSData.GetKeyStatus(4,&clr));
	//	m_Key4Status.SetBackgroundColor(clr);
	//	m_Key5Status.SetWindowText(m_EZSData.GetKeyStatus(5,&clr));
	//	m_Key5Status.SetBackgroundColor(clr);
	//	m_Key6Status.SetWindowText(m_EZSData.GetKeyStatus(6,&clr));
	//	m_Key6Status.SetBackgroundColor(clr);
	//	m_Key7Status.SetWindowText(m_EZSData.GetKeyStatus(7,&clr));
	//	m_Key7Status.SetBackgroundColor(clr);
	//	byte status=m_EZSData.GetStatus();
 //		if (CCommFunc::GetBit(status, 0))
 //			m_bInitialised = true;
 //		else
 //			m_bInitialised = false;
 //		if (CCommFunc::GetBit(status, 1))
 //			m_bTPremoved = true;
 //		else
 //			m_bTPremoved = false;
 //		if (CCommFunc::GetBit(status, 2))
 //			m_bPersonalised = true;
 //		else
 //			m_bPersonalised = false;
 //		if (CCommFunc::GetBit(status, 3))
 //			m_bActivated = true;
 //		else
 //			m_bActivated = false;
	//	UpdateData(FALSE);
	//	m_LastKey.SetContent(m_EZSData.GetLastKey());
	//	m_PreviousKey.SetContent(m_EZSData.GetPreviousKey());
	//}  
}


void CDumpCalcDlg::OnBnPSWIn()
{
// 	CString StrName;
// 	unsigned int dwFileLen;
// 	CFileDialog fileDlg(TRUE);
// 	fileDlg.m_ofn.lpstrTitle=_T("密码数据文件打开:*.PSW");
// 	fileDlg.m_ofn.lpstrFilter=_T("PSWFiles(*.PSW)\0*.PSW\0All Files(*.*)\0*.*\0\0");
// 	if (IDOK!=fileDlg.DoModal())
// 		return;
// 	CFile file(fileDlg.GetPathName(),CFile::modeRead);
// 	StrName=fileDlg.GetPathName();			//file path
// 	dwFileLen=(unsigned int)file.GetLength();	
// 	byte* pLoadBuf=new byte[dwFileLen+1];
// 	pLoadBuf[dwFileLen]=0;
// 	file.Read(pLoadBuf,dwFileLen);
// 	file.Close();
// 	if (dwFileLen == 0xa0)
// 	{
// 		CString str;
// 		str = CCommFunc::byteToHexStr(pLoadBuf, 1, 8, _T(" "));
// 		m_PSW.SetContent(str);
// 	}                       
// 	else
// 	{
// 		delete[] pLoadBuf;
// 		pLoadBuf=NULL;
// 		MessageBox(_T("加载密码数据文件错误!!"));  
// 	}
}


BOOL CDumpCalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ClearCtrl();
	m_bkBrush.CreateSolidBrush(RGB(166,202,240));
	CString strTemp;
	/************************************************************************/
	/* 静态控件                                                             */
	/************************************************************************/
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
	SetDlgItemText(IDC_STATIC16,strTemp); 

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
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDumpCalcDlg::ClearCtrl()
{
	m_bInitialised=FALSE;
	m_bPersonalised=FALSE;
	m_bActivated=FALSE;
	m_bTPremoved=FALSE;
	UpdateData(FALSE);
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

	m_Key0Status.SetWindowText(_T(""));
	m_Key1Status.SetWindowText(_T(""));
	m_Key2Status.SetWindowText(_T(""));
	m_Key3Status.SetWindowText(_T(""));
	m_Key4Status.SetWindowText(_T(""));
	m_Key5Status.SetWindowText(_T(""));
	m_Key6Status.SetWindowText(_T(""));
	m_Key7Status.SetWindowText(_T(""));
	COLORREF clr=RGB(166,202,240);
	m_Key0Status.SetBackgroundColor(clr);
	m_Key1Status.SetBackgroundColor(clr);
	m_Key2Status.SetBackgroundColor(clr);
	m_Key3Status.SetBackgroundColor(clr);
	m_Key4Status.SetBackgroundColor(clr);
	m_Key5Status.SetBackgroundColor(clr);
	m_Key6Status.SetBackgroundColor(clr);
	m_Key7Status.SetBackgroundColor(clr);
}


void CDumpCalcDlg::OnBnSaveData()
{
	// TODO: 在此添加控件通知处理程序代码
	//if(!CheckData())
/*		return;*/
// 	CFileDialog fileDlgS(FALSE);
// 	CString str;
// 	m_PSW.GetWindowText(str);
// 	if(!m_EZSData.SetPSW(str))
// 	{
// 		if (MessageBox(_T("密码 内容不符合要求，是否继续保存？"), _T("注意"), MB_YESNO) == IDNO)
// 			return;
// 		fileDlgS.m_ofn.lpstrFilter=_T("EZS Files(*.EZS)\0*.EZS\0BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
// 		fileDlgS.m_ofn.lpstrDefExt=_T("EZS");
// 	}
// 	else
// 	{
// 		fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0EZS Files(*.EZS)\0*.EZS\0All Files(*.*)\0*.*");
// 		fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
// 	}
// 	
// 	fileDlgS.m_ofn.lpstrTitle=_T("保存为电子点火开关数据文件:*.EZS");
// 	m_SSID.GetWindowText(str);
// 	str.Replace(_T(" "),_T(""));
// 	CString strFileName=str+_T("_SKC");//+CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
// // 	CTime tm=CTime::GetCurrentTime();
// // 	CString str=tm.Format("%Y-%m-%d");
// // 	strFileName+=str;
// 	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
// 	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
// 	if (IDOK==fileDlgS.DoModal())
// 	{
// 		byte save[0x800];
// 		for (int i = 0; i < 0x800; i++)
// 			save[i] = 0xff;
// 		for (int i = 0, k = 0; i < 0x400; )
// 		{
// 			save[k++] = m_EZSData.AllFile[i++];
// 			save[k++] = m_EZSData.AllFile[i++];
// 			k += 2;
// 		}
// 		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
// 		file.Write(save,0x800);
// 		file.Close();
// 	}	
// 	strFileName.ReleaseBuffer();
}


void CDumpCalcDlg::OnBnExportTXT()
{
	// TODO: 在此添加控件通知处理程序代码
// 	CFileDialog fileDlgS(FALSE);
// 	fileDlgS.m_ofn.lpstrTitle=_T("保存为导出数据文件:*.txt");
// 	fileDlgS.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
// 	fileDlgS.m_ofn.lpstrDefExt=_T("txt");
// 	CString str,str1;
// 	m_SSID.GetWindowText(str);
// 	str.Replace(_T(" "),_T(""));
// 	CString strFileName=str+_T("导出数据");
// // 	CTime tm=CTime::GetCurrentTime();
// // 	CString str=tm.Format("%Y-%m-%d");
// // 	strFileName+=str;
// 	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
// 	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
// 	if (IDOK==fileDlgS.DoModal())
// 	{
// 		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
// 		int len;
// 		m_SSID.GetWindowText(str);
// 		CString strEx=_T("SSID:")+str+_T("\r\n");
// 		m_Odo.GetWindowText(str);
// 		strEx.Append(_T("公里数:")+str+_T("\r\n"));
// 		m_PSW.GetWindowText(str);
// 		strEx.Append(_T("密码:")+str+_T("\r\n"));
// 		m_SerialNum.GetWindowText(str);
// 		strEx.Append(_T("序列号:")+str+_T("\r\n"));
// 		m_Dealer.GetWindowText(str);
// 		strEx.Append(_T("特殊值:")+str+_T("\r\n"));
// 		m_Transport.GetWindowText(str);
// 		strEx.Append(_T("固定值:")+str+_T("\r\n"));
// 		m_VIN.GetWindowText(str);
// 		strEx.Append(_T("VIN:")+str+_T("\r\n"));
// 		m_Key0.GetWindowText(str);
// 		m_Key0Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙0:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key1.GetWindowText(str);
// 		m_Key1Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙1:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key2.GetWindowText(str);
// 		m_Key2Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙2:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key3.GetWindowText(str);
// 		m_Key3Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙3:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key4.GetWindowText(str);
// 		m_Key4Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙4:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key5.GetWindowText(str);
// 		m_Key5Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙5:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key6.GetWindowText(str);
// 		m_Key6Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙6:")+str+_T("  ")+str1+_T("\r\n"));
// 		m_Key7.GetWindowText(str);
// 		m_Key7Status.GetWindowText(str1);
// 		strEx.Append(_T("钥匙7:")+str+_T("  ")+str1+_T("\r\n"));
// 		UpdateData(TRUE);
// 		if(m_bInitialised)
// 			strEx.Append(_T("初始化：是\r\n"));
// 		else
// 			strEx.Append(_T("初始化：否\r\n"));
// 		if(m_bTPremoved)
// 			strEx.Append(_T("运输模式已解除：是\r\n"));
// 		else
// 			strEx.Append(_T("运输模式已解除：否\r\n"));
// 		if(m_bPersonalised)
// 			strEx.Append(_T("个性化设置：是\r\n"));
// 		else
// 			strEx.Append(_T("个性化设置：否\r\n"));
// 		if(m_bActivated)
// 			strEx.Append(_T("激活：是\r\n"));
// 		else
// 			strEx.Append(_T("激活：否\r\n"));
// 		m_LastKey.GetWindowText(str);
// 		strEx.Append(_T("最后使用的钥匙:")+str+_T("\r\n"));
// 		m_PreviousKey.GetWindowText(str);
// 		strEx.Append(_T("之前使用的钥匙:")+str+_T("\r\n"));
// 		char* pLog=CCommFunc::CStringToChar(strEx,&len);
// 		file.Write(pLog,len*sizeof(CHAR));
// 		file.Close();
// 		delete[] pLog;
// 	}
// 	strFileName.ReleaseBuffer();
}


HBRUSH CDumpCalcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		case IDC_STATIC14:
		case IDC_STATIC15:
		case IDC_STATIC16:
		case IDC_STATIC17:
		case IDC_STATIC18:
		case IDC_STATIC19:
		case IDC_STATIC20:
		//case IDC_CHECK10:
			pDC->SetBkColor(RGB(166,202,240));
			pDC->SetBkMode(TRANSPARENT);      //不显示文字背景
			hbr = (HBRUSH)::GetStockObject(NULL_BRUSH); //编辑框背景。注意：和文字背景不是一个意思。
			break;
		case IDC_CHECK10:
		case IDC_CHECK11:
		case IDC_CHECK12:
		case IDC_CHECK13:
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


void CDumpCalcDlg::OnEnChangeEdit9()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

CString CDumpCalcDlg::GetDataLog()
{
	CString str,str1,strTemp,strTemp1;

	m_SSID.GetWindowText(str);
	strTemp.LoadString(IDS_SSID);
	CString strEx=strTemp+_T(":")+str+_T("\r\n");

	strTemp.LoadString(IDS_Odo);
	m_Odo.GetWindowText(str);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	m_PSW.GetWindowText(str);
	strTemp.LoadString(IDS_PSW);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	m_SerialNum.GetWindowText(str);
	strTemp.LoadString(IDS_SerialNum);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	m_Dealer.GetWindowText(str);
	strTemp.LoadString(IDS_Dealer);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	m_Transport.GetWindowText(str);
	strTemp.LoadString(IDS_Transport);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	m_VIN.GetWindowText(str);
	strTemp.LoadString(IDS_VIN);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	strTemp.LoadString(IDS_Key);

	m_Key0.GetWindowText(str);
	m_Key0Status.GetWindowText(str1);	
	strEx.Append(strTemp+_T("0:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key1.GetWindowText(str);
	m_Key1Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("1:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key2.GetWindowText(str);
	m_Key2Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("2:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key3.GetWindowText(str);
	m_Key3Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("3:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key4.GetWindowText(str);
	m_Key4Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("4:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key5.GetWindowText(str);
	m_Key5Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("5:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key6.GetWindowText(str);
	m_Key6Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("6:")+str+_T("  ")+str1+_T("\r\n"));

	m_Key7.GetWindowText(str);
	m_Key7Status.GetWindowText(str1);
	strEx.Append(strTemp+_T("7:")+str+_T("  ")+str1+_T("\r\n"));

	UpdateData(TRUE);
	strTemp.LoadString(IDS_Initialised);
	if(m_bInitialised)
	{
		strTemp1.LoadString(IDS_Yes);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}

	strTemp.LoadString(IDS_TPRemoved);
	if(m_bTPremoved)
	{
		strTemp1.LoadString(IDS_Yes);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}

	strTemp.LoadString(IDS_Personalised);
	if(m_bPersonalised)
	{
		strTemp1.LoadString(IDS_Yes);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}

	strTemp.LoadString(IDS_Activated);
	if(m_bActivated)
	{
		strTemp1.LoadString(IDS_Yes);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}
	else
	{
		strTemp1.LoadString(IDS_No);
		strEx.Append(strTemp+_T(":")+strTemp1+_T("\r\n"));
	}
	m_LastKey.GetWindowText(str);
	strTemp.LoadString(IDS_LastKey);
	strEx.Append(strTemp+_T(":")+str+_T("\r\n"));

	m_PreviousKey.GetWindowText(str);
	strTemp.LoadString(IDS_PreviousKey);
	strEx.Append(strTemp+_T(":")+str);
	return strEx;
}

