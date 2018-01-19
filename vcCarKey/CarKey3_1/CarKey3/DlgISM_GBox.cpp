// DlgISM_GBox.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgISM_GBox.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "..\..\CarKeyCommon\MyStringSplitter.h"
#include "..\..\CarKeyCommon\PassCalc.h"
#include "..\..\CarKeyCommon\VMProtectSDK.h"

void IG_DPassResultProc(void* p,byte* pBuf)
{
	CDlgISM_GBox* dlg=(CDlgISM_GBox*)(p);
	CString strTemp;
	if(pBuf!=NULL)
	{
		for(int i=0;i<8;i++)
			dlg->DPSW[i]=pBuf[i];
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
// CDlgISM_GBox 对话框

IMPLEMENT_DYNAMIC(CDlgISM_GBox, CDialogEx)

CDlgISM_GBox::CDlgISM_GBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgISM_GBox::IDD, pParent)
	, m_bInitialised(false)
	, m_bTPremoved(false)
	, m_bPersonalised(false)
	, m_bActivated(false)
	, m_bLoadData(false)
	, m_bReaded(false)
{
	for(int i=0;i<4;i++)
		SSID[i]=i;
	bDealerEdit=false;
}

CDlgISM_GBox::~CDlgISM_GBox()
{
	
}

void CDlgISM_GBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ModelType);
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
	DDX_Control(pDX, IDC_EDIT6, m_SVer);
	DDX_Control(pDX, IDC_EDIT16, m_MBLJH);
	DDX_Control(pDX, IDC_EDIT21, m_HVer);
	DDX_Check(pDX, IDC_CHECK14, m_bInitialised);
	DDX_Check(pDX, IDC_CHECK11, m_bTPremoved);
	DDX_Check(pDX, IDC_CHECK12, m_bPersonalised);
	DDX_Check(pDX, IDC_CHECK13, m_bActivated);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_EDIT23, m_SerialNum);
	DDX_Control(pDX, IDC_EDIT20, m_Transport);
}


BEGIN_MESSAGE_MAP(CDlgISM_GBox, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgISM_GBox::OnBnReadInfo)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgISM_GBox::OnBnReadData)
	// 	ON_COMMAND(ID_FILE_OPEN, &CDlgESL::OnFileOpen)
	// 	ON_COMMAND(ID_FILE_SAVE, &CDlgESL::OnFileSave)
	//	ON_BN_CLICKED(IDC_BUTTON3, &CDlgECU::OnBnECUGetPass)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgISM_GBox::OnBnRenew)
	ON_EN_CHANGE(IDC_EDIT19, &CDlgISM_GBox::OnEnDealerEdit)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgISM_GBox::OnSelchangeModel)
END_MESSAGE_MAP()

BOOL CDlgISM_GBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SVer.m_bIsHex=false;
	// TODO:  在此添加额外的初始化
	// 	if(!m_ToolBar.CreateEx( this,TBSTYLE_FLAT , WS_CHILD | WS_VISIBLE |CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS,
	// 		CRect(4,4,0,0))||!m_ToolBar.LoadToolBar(IDR_TOOLBAR3) )
	// 	{
	// 		TRACE0("failedto create toolbar\n");
	// 		return FALSE;
	// 	}
	// 	m_ToolBar.ShowWindow(SW_SHOW);
	// 	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, 0);	

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


	strTemp.LoadString(IDS_SerialNum);
	SetDlgItemText(IDC_STATIC4,strTemp);

	strTemp.LoadString(IDS_Dealer);
	SetDlgItemText(IDC_STATIC5,strTemp); 

	strTemp.LoadString(IDS_Transport);
	SetDlgItemText(IDC_STATIC6,strTemp); 

	strTemp.LoadString(IDS_VIN);
	SetDlgItemText(IDC_STATIC7,strTemp);

	strTemp.LoadString(IDS_PSWUI4);
	SetDlgItemText(IDC_STATIC18,strTemp); 

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
	SetDlgItemText(IDC_BUTTON3,strTemp+_T(" ISM GBox"));

	strTemp.LoadString(IDS_RenewPassCalc);
	SetDlgItemText(IDC_BUTTON4,strTemp); 

	strTemp.LoadString(IDS_PSWUI2);
	SetDlgItemText(IDC_STATIC25,strTemp); 

	// 	m_ModelType.AddString(_T("W164-NEC-old"));
	// 	m_ModelType.AddString(_T("W164-NEC-new"));	
	// 	m_ModelType.AddString(_T("W166-NEC"));
	m_ModelType.AddString(_T("ISM"));
	m_ModelType.AddString(_T("TCU 722.9"));		
	// 	m_ModelType.AddString(_T("W207-NEC"));
	// 	m_ModelType.AddString(_T("W212-NEC-old"));
	// 	m_ModelType.AddString(_T("W212-NEC-new"));	
	// 	m_ModelType.AddString(_T("W221-NEC-old"));
	// 	m_ModelType.AddString(_T("W221-NEC-new"));	
	// 	m_ModelType.AddString(_T("W246-NEC"));
	m_ModelType.SetCurSel(0);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgISM_GBox::OnBnReadInfo()
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
	byte* pBuf=NULL;
	if(m_ModelType.GetCurSel()==0)
		pBuf=CCarKey3App::TheHIDDevice.ISMReadInfo(&m_Progress,&Result);
	else
		pBuf=CCarKey3App::TheHIDDevice.GBoxReadInfo(&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		return;
	}
	if(m_ModelType.GetCurSel()==0)//ism
	{
		if(pBuf[0x100]==1)//西门子
		{
			char xx[11];
			xx[0]=pBuf[0x67];
			for(int i=0;i<7;i++)
				xx[i+1]=pBuf[0x69+i];
			for(int i=0;i<2;i++)
				xx[i+8]=pBuf[0x71+i];
			xx[10]='\0';
			CString strs=(CString)xx;
			m_MBLJH.SetContent(strs);
			m_HVer.SetContent(CCommFunc::byteToHexStr(pBuf, 0x62, 2, _T(" ")));

			char vin[18];
			for(int i=0;i<4;i++)
				vin[i+0]=pBuf[0x04+i];
			for(int i=0;i<7;i++)
				vin[i+4]=pBuf[0x11+i];
			for(int i=0;i<6;i++)
				vin[i+11]=pBuf[0x19+i];
			vin[17]='\0';
			CString strVin=(CString)vin;
			m_SVer.SetContent(strVin);//(CCommFunc::byteToHexStr(pBuf, 0x64, 2, _T(" ")));
		}
		else if(pBuf[0x100]==0)//马牌
		{
			char xx[11];
			for(int i=0;i<3;i++)
				xx[i]=pBuf[0x55+i];
			for(int i=0;i<7;i++)
				xx[i+3]=pBuf[0x61+i];
			xx[10]='\0';
			CString strs=(CString)xx;
			m_MBLJH.SetContent(strs);
			m_HVer.SetContent(CCommFunc::byteToHexStr(pBuf, 0x04, 3, _T(" ")));
			//m_SVer.SetContent(CCommFunc::byteToHexStr(pBuf, 0x15, 3, _T(" ")));
		}
	}
	else//gbox
	{
		byte tmp[100];	
		tmp[0]=pBuf[0x04];
		tmp[1]=pBuf[0x05];
		tmp[2]=pBuf[0x06];
		tmp[3]=pBuf[0x07];
		tmp[4]=pBuf[0x11];
		m_MBLJH.SetContent(CCommFunc::byteToHexStr(tmp, 0, 5, _T(" ")));
		m_HVer.SetContent(CCommFunc::byteToHexStr(pBuf, 0x12, 2, _T(" ")));
		m_SVer.SetContent(CCommFunc::byteToHexStr(pBuf, 0x14, 2, _T(" ")));
	}
//  	CFileDialog fileDlgS(FALSE);
//  	fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
//  	fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
//  	strTemp.LoadString(NULL,IDS_SaveFileTitle,CCarKey3App::LANGID);
//  	fileDlgS.m_ofn.lpstrTitle=strTemp;	
//  	fileDlgS.m_ofn.nMaxFile = MAX_PATH;	
//  	CString strFileName;//=_T("ECUInfo");
//  	if(m_ModelType.GetCurSel()==0)
//  		strFileName=_T("ISMInfo");
//  	else
//  		strFileName=_T("GBoxInfo");
//  	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
//  	if (IDOK==fileDlgS.DoModal())
//  	{
//  		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
//  		file.Write(pBuf,0x100);
//  		file.Close();
//  		strTemp.LoadString(NULL,IDS_SaveFileOK,CCarKey3App::LANGID);
//  		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
//  	}
//  	strFileName.ReleaseBuffer();
	// 	byte status=pBuf[0]&0x0f;
	// 	switch(status)
	// 	{
	// 	case 0x0c:
	// 		m_bInitialised = true;
	// 		m_bTPremoved = true;
	// 		m_bPersonalised = true;
	// 		m_bActivated = true;
	// 		break;
	// 	case 0x04:
	// 		m_bInitialised = true;
	// 		m_bTPremoved = true;
	// 		m_bPersonalised = true;
	// 		m_bActivated = false;
	// 		break;
	// 	case 0x00:
	// 		m_bInitialised = true;
	// 		m_bTPremoved = true;
	// 		m_bPersonalised = false;
	// 		m_bActivated = false;
	// 		break;
	// 	case 0x02:
	// 		m_bInitialised = true;
	// 		m_bTPremoved = false;
	// 		m_bPersonalised = false;
	// 		m_bActivated = false;
	// 		break;
	// 	}		
	// 	UpdateData(FALSE);
	// 	byte xx=m_EZSData.AllFile[4];
	// 	m_EZSData.AllFile[4]=pBuf[1];
	// 	COLORREF clr;
	// 	m_EZSData.GetKeyStatus(0,&clr);
	// 	m_Key0Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(1,&clr);
	// 	m_Key1Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(2,&clr);
	// 	m_Key2Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(3,&clr);
	// 	m_Key3Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(4,&clr);
	// 	m_Key4Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(5,&clr);
	// 	m_Key5Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(6,&clr);
	// 	m_Key6Status.SetBackgroundColor(clr);
	// 	m_EZSData.GetKeyStatus(7,&clr);
	// 	m_Key7Status.SetBackgroundColor(clr);
	// 	m_EZSData.AllFile[4]=xx;
	// 	m_Version.SetContent(CCommFunc::byteToHexStr(pBuf, 4, 3, _T(" ")));
	// 	m_MBLJH.SetContent(CCommFunc::byteToHexStr(pBuf, 0x10, 5, _T(" ")));
	// 	if(pBuf[0x26]==0xaa)
	// 	{
	// 		m_Locked.ShowWindow(TRUE);
	// 		m_Locked.SetBackgroundColor(RGB(0xdc,0x14,0x3c));
	// 	}
	// 	else
	// 	{
	// 		m_Locked.ShowWindow(FALSE);
	// 	}
	// 	delete[] pBuf;
	MessageBeep(MB_ICONEXCLAMATION);
	strTemp.LoadString(IDS_FinishCANInfo);
	ShowLog(strTemp);
}


// CDlgISM_GBox 消息处理程序


void CDlgISM_GBox::OnSelchangeModel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bReaded=false;
	CString strTemp;
	strTemp.LoadString(IDS_RenewPassCalc);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
}

void CDlgISM_GBox::ShowLog( CString str )
{
	m_StatusBar.SetText(str, 0, 0);
}

void CDlgISM_GBox::OnBnReadData()
{
	//OnBnReadInfo();
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
	byte* pBuf=NULL;
	if(m_ModelType.GetCurSel()==0)
		pBuf=CCarKey3App::TheHIDDevice.ISMReadData(&m_Progress,&Result);
	else
		pBuf=CCarKey3App::TheHIDDevice.GBoxReadData(&m_Progress,&Result);
	if(pBuf==NULL)
	{
		ShowLog(Result.GetHIDResult());
		return;
	}
	strTemp.LoadString(IDS_RenewPassCalc);
	SetDlgItemText(IDC_BUTTON4,strTemp);
//  	CFileDialog fileDlgS(FALSE);
//  	fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
//  	fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
//  	strTemp.LoadString(NULL,IDS_SaveFileTitle,CCarKey3App::LANGID);
//  	fileDlgS.m_ofn.lpstrTitle=strTemp;	
//  	fileDlgS.m_ofn.nMaxFile = MAX_PATH;	
//  	CString strFileName;//=_T("ECUInfo");
//  	if(m_ModelType.GetCurSel()==0)
//  		strFileName=_T("ISMData");
//  	else
//  		strFileName=_T("GBoxData");
//  	fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
//  	if (IDOK==fileDlgS.DoModal())
//  	{
//  		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
//  		file.Write(pBuf,0x100);
//  		file.Close();
//  		strTemp.LoadString(NULL,IDS_SaveFileOK,CCarKey3App::LANGID);
//  		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
//  	}
//  	strFileName.ReleaseBuffer();
	m_bReaded=true;
	for(int i=0;i<0x200;i++)
		IGData[i]=pBuf[i];
	delete[] pBuf;
	ShowData();
	MessageBeep(MB_ICONEXCLAMATION);
	strTemp.LoadString(IDS_FinishReadData);
	ShowLog(strTemp);
	MessageBox(strTemp);	
}

void CDlgISM_GBox::OnEnDealerEdit()
{
	if(bDealerEdit)
		return;
	CString strTemp;
	UpdateData(TRUE);
	if (m_bReaded && m_bActivated)
	{

	}
	else
	{
		strTemp.LoadString(IDS_RenewPassCalc);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		return;
	}
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
			if(p[i]!=IGData[0x80+i])
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
				if(cpc.HashOut[i]!=IGData[0x80+i])
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
					DPSW[i]=p[i];
			}
		}
	}
	delete[] p;	
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

void CDlgISM_GBox::OnBnRenew()
{
	CString strTemp,str,strTemp1;
	strTemp.LoadString(IDS_EZSRenew);
	GetDlgItem(IDC_BUTTON4)->GetWindowText(str);
	if(str==strTemp)//renew
	{		
		strTemp.LoadString(IDS_StartRenew);
		ShowLog(strTemp);
		CInteractionData Result;
		if(m_ModelType.GetCurSel()==0)
			CCarKey3App::TheHIDDevice.ISMRenew(DPSW,&m_Progress,&Result);
		else
			CCarKey3App::TheHIDDevice.GBoxRenew(DPSW,&m_Progress,&Result);
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
			}
		}
		else
		{
			m_Progress.SetPos(0);
			strTemp.LoadString(IDS_PSWStop);
			GetDlgItem(IDC_BUTTON4)->SetWindowText(strTemp);
			EnableControl(FALSE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			strTemp.LoadString(IDS_ServerTip9);
			ShowLog(strTemp);
			g_bDPassThreadRunning=true;
			g_bDPassThreadExit=false;
			CDPassThreadParam* tParam=new CDPassThreadParam();
			tParam->hWnd=this->m_hWnd;
			tParam->pDealer=&(IGData[0x80]);
			tParam->pDPassResultProc=IG_DPassResultProc;
			tParam->pProgress=&m_Progress;
			tParam->pStatusBar=&m_StatusBar;
			tParam->pSSID=SSID;
			tParam->dlg=this;
			_beginthread(DPassThread,0,(void *)(tParam));
		}
	}
}

void CDlgISM_GBox::EnableControl( BOOL bEn )
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bEn);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEn);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(bEn);
	//GetDlgItem(IDC_BUTTON4)->EnableWindow(bEn);
	GetDlgItem(IDC_EDIT19)->EnableWindow(bEn);
	GetDlgItem(IDC_COMBO1)->EnableWindow(bEn);
	//UpdateData(TRUE);
	CString strTemp;
	if (m_bReaded && m_bActivated)
	{
		strTemp.LoadString(IDS_RenewPassCalc);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	}
	else
	{
		strTemp.LoadString(IDS_RenewPassCalc);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	}
}

void CDlgISM_GBox::ShowData()
{
// 	if(m_ModelType.GetCurSel()==0)//ism
// 	{
 		m_Key0.SetContent(CCommFunc::byteToHexStr(IGData, 0, 8, _T(" ")));
 		m_Key1.SetContent(CCommFunc::byteToHexStr(IGData, 0x10, 8, _T(" ")));
 		m_Key2.SetContent(CCommFunc::byteToHexStr(IGData, 0x20, 8, _T(" ")));
 		m_Key3.SetContent(CCommFunc::byteToHexStr(IGData, 0x30, 8, _T(" ")));
 		m_Key4.SetContent(CCommFunc::byteToHexStr(IGData, 0x40, 8, _T(" ")));
 		m_Key5.SetContent(CCommFunc::byteToHexStr(IGData, 0x50, 8, _T(" ")));
 		m_Key6.SetContent(CCommFunc::byteToHexStr(IGData, 0x60, 8, _T(" ")));
 		m_Key7.SetContent(CCommFunc::byteToHexStr(IGData, 0x70, 8, _T(" ")));
// 
		bDealerEdit=true;
		m_Dealer.SetContent(CCommFunc::byteToHexStr(IGData, 0x80, 8, _T(" ")));
		bDealerEdit=false;
 		m_SerialNum.SetContent(CCommFunc::byteToHexStr(IGData, 0xa0, 8, _T(" ")));
 		m_Transport.SetContent(CCommFunc::byteToHexStr(IGData, 0x90, 8, _T(" ")));

		byte status=IGData[0xb1]&0x0f;
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
		CString strTemp;
		if (m_bReaded && m_bActivated)
		{
			strTemp.LoadString(IDS_RenewPassCalc);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		}
		else
		{
			strTemp.LoadString(IDS_RenewPassCalc);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		}
// 	}
// 	else
// 	{
// 
// 	}	
}
