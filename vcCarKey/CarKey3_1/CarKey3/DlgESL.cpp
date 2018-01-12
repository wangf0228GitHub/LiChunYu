// DlgESL.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgESL.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "..\..\CarKeyCommon\PassCalc.h"
#include "InputPSW.h"
#include "..\..\CarKeyCommon\MyStringSplitter.h"
#include "..\..\CarKeyCommon\VMProtectSDK.h"

void ESL_DPassResultProc(void* p,byte* pBuf)
{
	CDlgESL* dlg=(CDlgESL*)(p);
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
	dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	dlg->SetDlgItemText(IDC_BUTTON4,strTemp);
	dlg->EnableControl(TRUE);
}
// CDlgESL 对话框

IMPLEMENT_DYNAMIC(CDlgESL, CDialogEx)

CDlgESL::CDlgESL(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgESL::IDD, pParent)
	, m_bInitialised(false)
	, m_bTPremoved(false)
	, m_bPersonalised(false)
	, m_bActivated(false)
	, m_bLoadData(false)
	, m_bReaded(false)
{
	m_bRunning=false;
	m_bThreadExit=true;
	for(int i=0;i<4;i++)
		SSID[i]=i;
}

CDlgESL::~CDlgESL()
{
}

void CDlgESL::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ModelType);
	DDX_Control(pDX, IDC_STATIC1, m_Locked);
	DDX_Control(pDX, IDC_STATIC8, m_Key0Status);
	DDX_Control(pDX, IDC_STATIC9, m_Key1Status);
	DDX_Control(pDX, IDC_STATIC10, m_Key2Status);
	DDX_Control(pDX, IDC_STATIC11, m_Key3Status);
	DDX_Control(pDX, IDC_STATIC12, m_Key4Status);
	DDX_Control(pDX, IDC_STATIC13, m_Key5Status);
	DDX_Control(pDX, IDC_STATIC14, m_Key6Status);
	DDX_Control(pDX, IDC_STATIC15, m_Key7Status);
	DDX_Control(pDX, IDC_EDIT8, m_Key0);
	DDX_Control(pDX, IDC_EDIT9, m_Key1);
	DDX_Control(pDX, IDC_EDIT10, m_Key2);
	DDX_Control(pDX, IDC_EDIT11, m_Key3);
	DDX_Control(pDX, IDC_EDIT12, m_Key4);
	DDX_Control(pDX, IDC_EDIT13, m_Key5);
	DDX_Control(pDX, IDC_EDIT14, m_Key6);
	DDX_Control(pDX, IDC_EDIT15, m_Key7);
	DDX_Control(pDX, IDC_EDIT19, m_Dealer);
	DDX_Control(pDX, IDC_EDIT6, m_VIN);
	DDX_Control(pDX, IDC_EDIT16, m_MBLJH);
	DDX_Control(pDX, IDC_EDIT21, m_Version);
	DDX_Check(pDX, IDC_CHECK14, m_bInitialised);
	DDX_Check(pDX, IDC_CHECK11, m_bTPremoved);
	DDX_Check(pDX, IDC_CHECK12, m_bPersonalised);
	DDX_Check(pDX, IDC_CHECK13, m_bActivated);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}


BEGIN_MESSAGE_MAP(CDlgESL, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgESL::OnBnReadInfo)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgESL::OnBnReadData)
	ON_COMMAND(ID_FILE_OPEN, &CDlgESL::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CDlgESL::OnFileSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgESL::OnBnWriteESL)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgESL::OnBnRenewESL)
	ON_EN_CHANGE(IDC_EDIT19, &CDlgESL::OnEnDealerEdit)
END_MESSAGE_MAP()


// CDlgESL 消息处理程序


BOOL CDlgESL::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_VIN.m_bIsHex=false;
	// TODO:  在此添加额外的初始化
	if(!m_ToolBar.CreateEx( this,TBSTYLE_FLAT , WS_CHILD | WS_VISIBLE |CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS,
		CRect(4,4,0,0))||!m_ToolBar.LoadToolBar(IDR_TOOLBAR3) )
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

// 	m_hUsbEventHandle = CCarKey3App::TheHIDDevice.RegisterForUsbEvents(this->GetSafeHwnd());
// 	m_Progress.SetStep(1);
	CString strTemp;
	strTemp.LoadString(IDS_Key);
	SetDlgItemText(IDC_STATIC8,strTemp+_T("0")); 
	SetDlgItemText(IDC_STATIC9,strTemp+_T("1")); 
	SetDlgItemText(IDC_STATIC10,strTemp+_T("2")); 
	SetDlgItemText(IDC_STATIC11,strTemp+_T("3")); 
	SetDlgItemText(IDC_STATIC12,strTemp+_T("4")); 
	SetDlgItemText(IDC_STATIC13,strTemp+_T("5")); 
	SetDlgItemText(IDC_STATIC14,strTemp+_T("6")); 
	SetDlgItemText(IDC_STATIC15,strTemp+_T("7"));

	strTemp.LoadString(IDS_Dealer);
	SetDlgItemText(IDC_STATIC5,strTemp); 

	strTemp.LoadString(IDS_VIN);
	SetDlgItemText(IDC_STATIC7,strTemp);

	strTemp.LoadString(IDS_DumpDlg_StatusGroup);
	SetDlgItemText(IDC_STATIC18,_T("ESL ")+strTemp); 

	strTemp.LoadString(IDS_Initialised);
	SetDlgItemText(IDC_CHECK14,strTemp); 

	strTemp.LoadString(IDS_TPRemoved);
	SetDlgItemText(IDC_CHECK11,strTemp);

	strTemp.LoadString(IDS_Personalised);
	SetDlgItemText(IDC_CHECK12,strTemp);

	strTemp.LoadString(IDS_Activated);
	SetDlgItemText(IDC_CHECK13,strTemp);

	strTemp.LoadString(IDS_ModleType);
	SetDlgItemText(IDC_STATIC24,strTemp); 

	strTemp.LoadString(IDS_MBLJH);
	SetDlgItemText(IDC_STATIC16,strTemp); 

	strTemp.LoadString(IDS_BanBenHao);
	SetDlgItemText(IDC_STATIC17,strTemp); 

	strTemp.LoadString(IDS_Locked);
	SetDlgItemText(IDC_STATIC1,strTemp); 

	strTemp.LoadString(IDS_PortDlg_ReadInfo);
	SetDlgItemText(IDC_BUTTON1,strTemp); 

	strTemp.LoadString(IDS_PortDlg_ReadData);
	SetDlgItemText(IDC_BUTTON2,strTemp);

	strTemp.LoadString(IDS_Write);
	SetDlgItemText(IDC_BUTTON3,strTemp+_T(" ESL"));

	strTemp.LoadString(IDS_RenewPassCalc);
	SetDlgItemText(IDC_BUTTON4,strTemp); 

	strTemp.LoadString(IDS_ModleType);
	SetDlgItemText(IDC_STATIC25,_T("ESL")+strTemp); 

// 	m_ModelType.AddString(_T("W164-NEC-old"));
// 	m_ModelType.AddString(_T("W164-NEC-new"));	
// 	m_ModelType.AddString(_T("W166-NEC"));
 	m_ModelType.AddString(_T("W204-NEC"));
// 	m_ModelType.AddString(_T("W207-NEC"));
// 	m_ModelType.AddString(_T("W212-NEC-old"));
// 	m_ModelType.AddString(_T("W212-NEC-new"));	
// 	m_ModelType.AddString(_T("W221-NEC-old"));
// 	m_ModelType.AddString(_T("W221-NEC-new"));	
// 	m_ModelType.AddString(_T("W246-NEC"));
	m_ModelType.SetCurSel(0);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgESL::OnBnReadInfo()
{
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
	byte* pBuf=CCarKey3App::TheHIDDevice.ESLReadInfo(&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		return;
	}
	byte status=pBuf[0]&0x0f;
	switch(status)
	{
	case 0x0c:
		m_bInitialised = true;
		m_bTPremoved = true;
		m_bPersonalised = true;
		m_bActivated = true;
		break;
	case 0x04:
		m_bInitialised = true;
		m_bTPremoved = true;
		m_bPersonalised = true;
		m_bActivated = false;
		break;
	case 0x00:
		m_bInitialised = true;
		m_bTPremoved = true;
		m_bPersonalised = false;
		m_bActivated = false;
		break;
	case 0x02:
		m_bInitialised = true;
		m_bTPremoved = false;
		m_bPersonalised = false;
		m_bActivated = false;
		break;
	}		
	UpdateData(FALSE);
	byte xx=m_EZSData.AllFile[4];
	m_EZSData.AllFile[4]=pBuf[1];
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
	m_EZSData.AllFile[4]=xx;
	m_Version.SetContent(CCommFunc::byteToHexStr(pBuf, 4, 3, _T(" ")));
	m_MBLJH.SetContent(CCommFunc::byteToHexStr(pBuf, 0x10, 5, _T(" ")));
	if(pBuf[0x26]==0xaa)
	{
		m_Locked.ShowWindow(TRUE);
		m_Locked.SetBackgroundColor(RGB(0xdc,0x14,0x3c));
	}
	else
	{
		m_Locked.ShowWindow(FALSE);
	}
	delete[] pBuf;
	MessageBeep(MB_ICONEXCLAMATION);
	strTemp.LoadString(IDS_FinishCANInfo);
	ShowLog(strTemp);
	//MessageBox(strTemp);
}

void CDlgESL::ShowLog( CString str )
{
	m_StatusBar.SetText(str, 0, 0);
}


void CDlgESL::OnBnReadData()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	OnBnReadInfo();
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
	strTemp.LoadString(IDS_ReadDating);
	ShowLog(strTemp);
	CInteractionData Result;
	byte* pBuf=CCarKey3App::TheHIDDevice.ESLReadData(&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		return;
	}
	m_bReaded=true;
	for(int i=0;i<0x200;i++)
		ESLData[i]=pBuf[i];
	delete[] pBuf;
	ShowData();
	MessageBeep(MB_ICONEXCLAMATION);
	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	MessageBox(strTemp);	
}

int CDlgESL::GetKey(int index )
{
	int x=index*32;
	bool bSame=true;
	for(int i=0;i<8;i++)
	{
		if(ESLData[x+i]!=ESLData[x+i+8])
		{
			bSame=false;
			break;
		}
		if(ESLData[x+i]!=ESLData[x+i+16])
		{
			bSame=false;
			break;
		}
		if(ESLData[x+i]!=ESLData[x+i+24])
		{
			bSame=false;
			break;
		}
	}
	if(bSame)
		return x;
	int ret=x;
	CPassCalc cpCalc;
	bool bNext=true;

	for(int i=0;i<8;i++)
		cpCalc.HashIn[i]=ESLData[x+i];
	cpCalc.sub_HashOnce();
	if(!bIsSame(cpCalc.HashOut,&ESLData[x+8]))
	{
		bNext=false;
	}
	if(!bNext)
		ret=x+8;
	
	bNext=true;
	for(int i=0;i<8;i++)
		cpCalc.HashIn[i]=ESLData[x+i+8];
	cpCalc.sub_HashOnce();
	if(!bIsSame(cpCalc.HashOut,&ESLData[x+16]))
	{
		bNext=false;
	}
	if(!bNext)
		ret=x+16;

	bNext=true;
	for(int i=0;i<8;i++)
		cpCalc.HashIn[i]=ESLData[x+i+16];
	cpCalc.sub_HashOnce();
	if(!bIsSame(cpCalc.HashOut,&ESLData[x+24]))
	{
		bNext=false;
	}
	if(!bNext)
		ret=x+24;
	return ret;
}
bool CDlgESL::bIsSame( byte* p1,byte* p2 )
{
	bool bok=true;
	for(int i=0;i<8;i++)
	{
		if(p1[i]!=p2[i])
		{
			bok=false;
			break;
		}
	}
	return bok;
}
void CDlgESL::ShowData()
{
	int key;
	key=GetKey(0);
	m_Key0.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(1);
	m_Key1.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(2);
	m_Key2.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(3);
	m_Key3.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(4);
	m_Key4.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(5);
	m_Key5.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(6);
	m_Key6.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));
	key=GetKey(7);
	m_Key7.SetContent(CCommFunc::byteToHexStr(&(ESLData[key]), 0, 8, _T(" ")));

	m_Dealer.SetContent(CCommFunc::byteToHexStr(&(ESLData[0x100]), 0, 8, _T(" ")));

	CString strVIN;
	char VIN[15];
	for(int i=0;i<7;i++)
		VIN[i]=ESLData[0x138+i];
	for(int i=0;i<7;i++)
		VIN[i+7]=ESLData[0x140+i];
	VIN[14]='\0';
	bool bVIN=true;
	for(int i=0;i<14;i++)
	{
		if(isgraph(VIN[i])==0)
		{
			bVIN=false;
			break;
		}
	}

	if(bVIN)
	{		
		strVIN=VIN;
	}
	else
		strVIN= _T("");
	m_VIN.SetContent(strVIN);
}


void CDlgESL::OnFileOpen()
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
// 	if (dwFileLen == 0x200)//红外数据文件
// 	{
// 		for(int i=0;i<0x200;i++)
// 			ESLData[i]=pLoadBuf[i];
// 	}
	if (dwFileLen == 0x400)//红外数据文件
	{
		m_bReaded=false;
	 	m_EZSData.SetAllFile(pLoadBuf);	
		m_Dealer.SetContent(m_EZSData.GetDealer());
		m_Key0.SetContent(m_EZSData.GetKey0());
		m_Key1.SetContent(m_EZSData.GetKey1());
		m_Key2.SetContent(m_EZSData.GetKey2());
		m_Key3.SetContent(m_EZSData.GetKey3());
		m_Key4.SetContent(m_EZSData.GetKey4());
		m_Key5.SetContent(m_EZSData.GetKey5());
		m_Key6.SetContent(m_EZSData.GetKey6());
		m_Key7.SetContent(m_EZSData.GetKey7());

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

		m_VIN.SetContent(m_EZSData.GetVIN());
		strTemp.LoadString(IDS_OpenFileOK);
		ShowLog(strTemp+_T(":")+StrName);
		m_bLoadData=true;
	}
	else
	{
		strTemp.LoadString(IDS_ServerTip14);
		ShowLog(strTemp);
		MessageBox(strTemp);
	}
	delete[] pLoadBuf;
}


void CDlgESL::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	if(CheckData()!=true)
		return;
	CString str,strTemp,strTemp1;
	CFileDialog fileDlgS(FALSE);
	fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
	strTemp.LoadString(IDS_SaveFileTitle);
	fileDlgS.m_ofn.lpstrTitle=strTemp;
// 	m_SSID.GetWindowText(str);
// 	str.Replace(_T(" "),_T(""));
	CString strFileName=_T("ESL Data");//str;//+_T("_SKC");//+CCommFunc::byteToHexStr(CCarKey2App::TheHIDDevice.HexID,4,_T(""))+_T("日志_");
	// 	CTime tm=CTime::GetCurrentTime();
	// 	CString str=tm.Format("%Y-%m-%d");
	// 	strFileName+=str;
	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;
	if (IDOK==fileDlgS.DoModal())
	{		
		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
		file.Write(ESLData,0x200);
		file.Close();
		strTemp.LoadString(IDS_SaveFileOK);
		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
	}	
	strFileName.ReleaseBuffer();
}

bool CDlgESL::CheckData()
{
	CString str,strTemp,strTemp1,strTemp2;
	strTemp.LoadString(IDS_ErrDataArea);
	m_Key0.GetWindowText(str);
	strTemp1.LoadString(IDS_Key);
	if(str==_T(""))
	{
		MessageBox(strTemp1+strTemp);
		return false;
	}	
// 	m_Dealer.GetWindowText(str);
// 	if(!m_EZSData.SetDealer(str))
// 	{
// 		strTemp2.LoadString(IDS_Dealer);
// 		MessageBox(strTemp2+strTemp);
// 		return false;
// 	}
// 	for(int i=0;i<8;i++)
// 	{
// 		ESLData[0x100+i]=m_EZSData.AllFile[0x10+i];
// 		ESLData[0x100+i+8]=m_EZSData.AllFile[0x10+i];
// 		ESLData[0x100+i+16]=m_EZSData.AllFile[0x10+i];
// 		ESLData[0x100+i+24]=m_EZSData.AllFile[0x10+i];
// 	}
	return true;
}
void ESLPassThread(void* p)
{
	VMProtectBegin("ESLDPassThread");
	CDlgESL* dlgESL=(CDlgESL*)(p);
	
	CString str,strTemp,strTemp1,strInfo;

	CString sTime=_T("-1");
	//CString serverRet;
	int retry=5;
	byte* wPass;
	bool bFind=false;
	while(retry!=0)
	{
		if(!dlgESL->m_bRunning)
		{
			strTemp.LoadString(NULL,IDS_ServerTip16,CCarKey3App::LANGID);
			dlgESL->ShowLog(strTemp);
			break;
		}
		CWebRetData webRet;
		if(CCarKey3App::withServer.GetESLPass(CCarKey3App::TheHIDDevice.HexID,dlgESL->ESLPass,sTime,webRet))
		{
			if(webRet.retStatus==web_OK)
			{
				int len;
				wPass=CCommFunc::strToHexByte(webRet.strResult,_T(""),&len);
				bFind=true;
				break;
			}
			else if(webRet.retStatus==2)//刚开始排队
			{
				CStringArray strArray;
				CCarKey3App::withServer.GetRetList(webRet.strResult,_T("|"),strArray);
				int queue=_ttoi(strArray.GetAt(0));
				if(queue==1)
				{
					strTemp.LoadString(NULL,IDS_ServerTip31,CCarKey3App::LANGID);
					dlgESL->ShowLog(strTemp);
				}
				else
				{
					strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,queue-1);
					dlgESL->ShowLog(strTemp);
				}
				sTime=strArray.GetAt(1);
			}
			else if(webRet.retStatus==3)//排队中
			{
				CStringArray strArray;
				CCarKey3App::withServer.GetRetList(webRet.strResult,_T("|"),strArray);
				int queue=_ttoi(strArray.GetAt(0));
				if(queue==1)
				{
					strTemp.LoadString(NULL,IDS_ServerTip9,CCarKey3App::LANGID);
					dlgESL->ShowLog(strTemp);
				}
				else
				{
					strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,queue-1);
					dlgESL->ShowLog(strTemp);
				}
			}
			else if(webRet.retStatus==web_NoRegDevice)
			{
				strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
				MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgESL->ShowLog(strTemp);
				break;
			}
			else if(webRet.retStatus==-4)
			{
				strTemp.LoadString(NULL,IDS_ServerTip27,CCarKey3App::LANGID);				
				MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgESL->ShowLog(strTemp);
				break;
			}
			else
			{
				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				strTemp1.Format(strTemp,webRet.retStatus);
				MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgESL->ShowLog(strTemp1);
				break;
			}
			dlgESL->m_Progress.SetRange(0,10);
			dlgESL->m_Progress.SetPos(0);
			dlgESL->m_Progress.UpdateWindow();
			for(int i=1;i<11;i++)
			{
				Sleep(1000);
				dlgESL->m_Progress.SetPos(i);
				dlgESL->m_Progress.UpdateWindow();
			}
		}
		else
			retry--;
	}
	if(bFind)
	{
		strTemp.LoadString(NULL,IDS_StartCANWrite,CCarKey3App::LANGID);
		dlgESL->ShowLog(strTemp);
		CInteractionData Result;
		CCarKey3App::TheHIDDevice.ESLWrite(&dlgESL->m_Progress,&Result,dlgESL->m_EZSData.AllFile,wPass);
		if(Result.GetHIDResult()!=_T(""))
		{
			dlgESL->ShowLog(Result.GetHIDResult());			
		}
		else
		{
			MessageBeep(MB_ICONEXCLAMATION);
			strTemp.LoadString(NULL,IDS_FinishCANWrite,CCarKey3App::LANGID);
			dlgESL->ShowLog(strTemp);
			MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,0);
		}
	}
	strTemp.LoadString(NULL,IDS_Write,CCarKey3App::LANGID);
	dlgESL->SetDlgItemText(IDC_BUTTON3,strTemp+_T(" ESL"));
	dlgESL->EnableControl(TRUE);
	dlgESL->m_bThreadExit=true;
	dlgESL->m_bRunning=false;
	VMProtectEnd();
}

void CDlgESL::OnBnWriteESL()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_bLoadData)
	{
		OnFileOpen();
	}
	if(!m_bLoadData)
		return;
	CString strTemp,strTemp1,strInfo,str;
	/*if(!m_EZSData.bHavePSW())
	{
		strTemp.LoadString(IDS_ErrPSWLog);
		MessageBox(strTemp);
		strTemp.LoadString(IDS_ErrPSWLog);
		ShowLog(strTemp);
		return;
	}*/	
	strTemp.LoadString(IDS_PSWStop);
	GetDlgItem(IDC_BUTTON3)->GetWindowText(str);
	if(str==strTemp)
	{
		strTemp.LoadString(IDS_PSWTip12);
		strTemp1.LoadString(IDS_Warring);
		if(MessageBox(strTemp, strTemp1,MB_YESNO) == IDYES)
		{
			GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
			strTemp.LoadString(IDS_ServerTip8);
			ShowLog(strTemp);
			m_bRunning=false;
		}
	}
	else
	{
		CInteractionData rx;
		rx=CCarKey3App::TheHIDDevice.Check_mima();
		if(rx.mimaok!=true)
		{
			CString strx=rx.GetHIDResult();
			ShowLog(strx);
			MessageBox(strx);
			return;
		}
		bool bPass=false;
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
				for(int i=0;i<8;i++)
					ESLPass[i]=CCarKey3App::HPassword[i];
				bPass=true;
			}		
		}
		if(!bPass)
		{
			//unsigned char ={0x7C,0x8F,0xB3,0xD8,0xC7,0x3E,0xA5,0xCE};//此密码需要服务器计算获得，此处用固定值测试
			CInputPSW ipsw;
			ipsw.DoModal();
			CString strTemp;
			if(!ipsw.bPSW)
			{
				strTemp.LoadString(IDS_ServerTip11);
				MessageBox(strTemp);
				return;
			}
			for(int xx=0;xx<8;xx++)
				ESLPass[xx]=ipsw.PSW[xx];
		}
		m_bRunning=true;
		m_bThreadExit=false;
		strTemp.LoadString(IDS_PSWStop);
		GetDlgItem(IDC_BUTTON3)->SetWindowText(strTemp);
		EnableControl(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		_beginthread(ESLPassThread,0,(void *)(this));
	}	
}


void CDlgESL::OnBnRenewESL()
{
	CString strTemp,str,strTemp1;
	strTemp.LoadString(IDS_EZSRenew);
	GetDlgItem(IDC_BUTTON4)->GetWindowText(str);
	if(str==strTemp)//renew
	{		
		strTemp.LoadString(IDS_StartRenew);
		ShowLog(strTemp);
		CInteractionData Result;
		CCarKey3App::TheHIDDevice.ESLRenew(m_EZSData.DPSW,&Result);
		if(Result.GetHIDResult()==_T(""))
		{
			MessageBeep(MB_ICONEXCLAMATION);
			strTemp.LoadString(IDS_FinishEZSRenew);
			ShowLog(strTemp);
			MessageBox(strTemp);
		}
		else
		{
			ShowLog(Result.GetHIDResult());
		}
	}
	else//获得密码
	{
		strTemp.LoadString(IDS_PSWStop);
		GetDlgItem(IDC_BUTTON4)->GetWindowText(str);
		if(str==strTemp)
		{
			strTemp.LoadString(IDS_PSWTip12);
			strTemp1.LoadString(IDS_Warring);
			if(MessageBox(strTemp, strTemp1,MB_YESNO) == IDYES)
			{
				GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
				strTemp.LoadString(IDS_ServerTip8);
				ShowLog(strTemp);
				m_bRunning=false;
			}
		}
		else
		{
			m_Progress.SetPos(0);
			strTemp.LoadString(IDS_PSWStop);
			GetDlgItem(IDC_BUTTON4)->SetWindowText(strTemp);
			g_bDPassThreadRunning=true;
			g_bDPassThreadExit=false;
			EnableControl(FALSE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			strTemp.LoadString(IDS_ServerTip9);
			ShowLog(strTemp);
			CDPassThreadParam* tParam=new CDPassThreadParam();
			tParam->hWnd=this->m_hWnd;
			tParam->pDealer=&(ESLData[0x100]);
			tParam->pDPassResultProc=ESL_DPassResultProc;
			tParam->pProgress=&m_Progress;
			tParam->pStatusBar=&m_StatusBar;
			tParam->pSSID=SSID;
			tParam->dlg=this;
			_beginthread(DPassThread,0,(void *)(tParam));
		}
	}
}


void CDlgESL::OnEnDealerEdit()
{
	CString str;
	bool bRenew=false;
	m_Dealer.GetWindowText(str);
	int len;
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
			if(p[i]!=ESLData[0x100+i])
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
				if(cpc.HashOut[i]!=ESLData[0x100+i])
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
	delete[] p;
	CString strTemp;
	if (m_bReaded)
	{
		if(bRenew)
		{
			strTemp.LoadString(IDS_EZSRenew);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		}
		else
		{
			strTemp.LoadString(IDS_RenewPassCalc);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		}
	}
	else
	{
		strTemp.LoadString(IDS_RenewPassCalc);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	}
	SetDlgItemText(IDC_BUTTON4,strTemp);
}

void CDlgESL::EnableControl( BOOL bEn )
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bEn);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEn);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(bEn);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(bEn);
	GetDlgItem(IDC_EDIT19)->EnableWindow(bEn);
	GetDlgItem(IDC_COMBO1)->EnableWindow(bEn);
	m_ToolBar.EnableWindow(bEn);
}
