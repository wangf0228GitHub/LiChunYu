// DlgPSW.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgPSW.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "..\..\CarKeyCommon\HIDDevice.h"
#include "..\..\CarKeyCommon\HIDWork.h"
#include "..\..\CarKeyCommon\MyStringSplitter.h"
#include "..\..\CarKeyCommon\PassCalc.h"
#include "..\..\CarKeyCommon\VMProtectSDK.h"

void SuanMiMaThread(void* p)
{
	VMProtectBegin("SuanMiMaThread");
	CDlgPSW* dlgPSW=(CDlgPSW*)(p);
	CString str,strTemp,strTemp1,strInfo;
	CInteractionData Result;
	byte* pBuf=NULL;
	CFileDialog fileDlgS(FALSE);
	fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
	strTemp.LoadString(NULL,IDS_SaveFileTitle,CCarKey3App::LANGID);
	fileDlgS.m_ofn.lpstrTitle=strTemp;	
	fileDlgS.m_ofn.nMaxFile = MAX_PATH;	
	if(!dlgPSW->m_bQuanDiu)
	{		
		pBuf=CCarKey3App::TheHIDDevice.SuanMiMa(dlgPSW->m_hWnd,dlgPSW->m_nMode,&(dlgPSW->m_Progress),&Result,&(dlgPSW->m_StatusBar),&(dlgPSW->m_bRunning));
		if(pBuf==NULL)
		{
			dlgPSW->ShowLog(Result.GetHIDResult());	
			dlgPSW->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
			dlgPSW->GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
			dlgPSW->GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
			dlgPSW->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			strTemp.LoadString(NULL,IDS_PSWCaiJi,CCarKey3App::LANGID);
			dlgPSW->GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
			strTemp.LoadString(NULL,IDS_PSWShangChuan,CCarKey3App::LANGID);
			dlgPSW->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
			dlgPSW->m_bThreadExit=true;
			dlgPSW->m_bRunning=false;
			dlgPSW->OnCbnSelchangeModel();
			MessageBoxEx(dlgPSW->m_hWnd,Result.GetHIDResult(),_T(""),MB_OK,CCarKey3App::LANGID);
			return;
		}		
		//DWORD dwFileLen;
		//byte *pBuf;
		//unsigned int i,j;

		//unsigned char CNT;
		//unsigned char TP[0x08];
		//char DataT[16+1];
		//unsigned char FileA[0x400*0x08];
		//unsigned char FileB[0x400*0x20];//0x1a
		////--->
		//CFileDialog fileDlg(TRUE);
		//CString strTemp,strTemp1;
		//strTemp.LoadString(IDS_OpenFileTitle);
		//strTemp1.Format(strTemp,_T("EIS"));
		//fileDlg.m_ofn.lpstrTitle=strTemp1;
		//fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
		//if (IDOK!=fileDlg.DoModal())
		//	return;
		//CFile file(fileDlg.GetPathName(),CFile::modeRead);
		////StrName=fileDlg.GetPathName();			//file path
		//dwFileLen=file.GetLength();
		//pBuf=new byte[dwFileLen+1];
		//pBuf[dwFileLen]=0;
		//file.Read(pBuf,dwFileLen);
		//file.Close();
		strTemp.LoadString(NULL,IDS_PSWTip15,CCarKey3App::LANGID);
		dlgPSW->ShowLog(strTemp);
		MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
		byte ssid[4];
		for(int i=0;i<4;i++)
			ssid[i]=pBuf[4+i];
		ssid[0]=ssid[0]&0xf8;
		str=CCommFunc::byteToHexStr(ssid, 0x00, 4, _T(""));
		CString strFileName=str+_T("_ScanKey");
		fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
		if (IDOK==fileDlgS.DoModal())
		{		
			CFile file(fileDlgS.GetPathName()+_T(".bak"),CFile::modeCreate|CFile::modeWrite);
			file.Write(pBuf,0x40*0x400);
			file.Close();
			strTemp.LoadString(NULL,IDS_SaveFileOK,CCarKey3App::LANGID);
			dlgPSW->ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());			
// 			加密
// 						byte FileA[8192];//0x400*0x08
// 						for(int j=0;j<1024;j++)
// 						{
// 							for(int i=0;i<8;i++)
// 							{
// 								FileA[0x08*j+i]=pBuf[0x40*j+0x30+i]^pBuf[0x40*j+0x21+i];
// 							}
// 						}
			//加密
			byte* pEncrypt=CCommFunc::PSWEncrypt(pBuf);
			byte* encode=CCommFunc::Encrypt(pEncrypt,1024*20*4,CCarKey3App::TheHIDDevice.HexID,ssid);
			byte save[1024*20*4+8];
			for(int x=0;x<4;x++)
			{
				save[x]=CCarKey3App::TheHIDDevice.HexID[x];
				save[x+4]=ssid[x];
			}
			for(int x=0;x<1024*20*4;x++)
			{
				save[8+x]=encode[x];
			}
			delete[] encode;
			delete[] pEncrypt;
			CFile file1(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
			file1.Write(save,1024*20*4+8);
			file1.Close();
		}	
		strFileName.ReleaseBuffer();
		delete[] pBuf;
		dlgPSW->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		dlgPSW->GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
		dlgPSW->GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
		dlgPSW->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		strTemp.LoadString(NULL,IDS_PSWCaiJi,CCarKey3App::LANGID);
		dlgPSW->GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
		strTemp.LoadString(NULL,IDS_PSWShangChuan,CCarKey3App::LANGID);
		dlgPSW->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
		dlgPSW->m_bThreadExit=true;
		dlgPSW->m_bRunning=false;
		dlgPSW->OnCbnSelchangeModel();
	}
	else
	{
		if(dlgPSW->m_nQuanDiuType==3)//03款
		{
			pBuf=CCarKey3App::TheHIDDevice.QuanDiu03(dlgPSW->m_hWnd,dlgPSW->m_nMode,&(dlgPSW->m_Progress),&Result,&(dlgPSW->m_StatusBar),&(dlgPSW->m_bRunning));
		}
		else if(dlgPSW->m_nQuanDiuType==2)//02款
		{
			pBuf=CCarKey3App::TheHIDDevice.QuanDiu02(dlgPSW->m_hWnd,dlgPSW->m_nMode,&(dlgPSW->m_Progress),&Result,&(dlgPSW->m_StatusBar),&(dlgPSW->m_bRunning));
		}
		if(pBuf==NULL)
		{
			dlgPSW->ShowLog(Result.GetHIDResult());	
			dlgPSW->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
			dlgPSW->GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
			dlgPSW->GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
			dlgPSW->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			strTemp.LoadString(NULL,IDS_PSWCaiJi,CCarKey3App::LANGID);
			dlgPSW->GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
			strTemp.LoadString(NULL,IDS_PSWShangChuan,CCarKey3App::LANGID);
			dlgPSW->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
			dlgPSW->m_bThreadExit=true;
			dlgPSW->m_bRunning=false;
			dlgPSW->OnCbnSelchangeModel();
			MessageBoxEx(dlgPSW->m_hWnd,Result.GetHIDResult(),_T(""),MB_OK,CCarKey3App::LANGID);
			return;
		}
		//DWORD dwFileLen;
		//byte *pBuf;
		//unsigned int i,j;

		//unsigned char CNT;
		//unsigned char TP[0x08];
		//char DataT[16+1];
		//unsigned char FileA[0x400*0x08];
		//unsigned char FileB[0x400*0x20];//0x1a
		////--->
		//CFileDialog fileDlg(TRUE);
		//CString strTemp,strTemp1;
		//strTemp.LoadString(IDS_OpenFileTitle);
		//strTemp1.Format(strTemp,_T("EIS"));
		//fileDlg.m_ofn.lpstrTitle=strTemp1;
		//fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
		//if (IDOK!=fileDlg.DoModal())
		//	return;
		//CFile file(fileDlg.GetPathName(),CFile::modeRead);
		////StrName=fileDlg.GetPathName();			//file path
		//dwFileLen=file.GetLength();
		//pBuf=new byte[dwFileLen+1];
		//pBuf[dwFileLen]=0;
		//file.Read(pBuf,dwFileLen);
		//file.Close();
		strTemp.LoadString(NULL,IDS_PSWTip15,CCarKey3App::LANGID);
		dlgPSW->ShowLog(strTemp);
		MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
		byte ssid[4];
		if(dlgPSW->m_nQuanDiuType==3)//03款
		{
			for(int i=0;i<4;i++)
				ssid[i]=pBuf[2+i];
		}
		else if(dlgPSW->m_nQuanDiuType==2)//02款
		{
			for(int i=0;i<4;i++)
				ssid[i]=pBuf[4+i];
		}		
		ssid[0]=ssid[0]&0xf8;
		str=CCommFunc::byteToHexStr(ssid, 0x00, 4, _T(""));
		CString strFileName=str+_T("_ScanEIS");
		fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
		if (IDOK==fileDlgS.DoModal())
		{
			CFile file(fileDlgS.GetPathName()+_T(".bak"),CFile::modeCreate|CFile::modeWrite);
			file.Write(pBuf,0x80*0x400);
			file.Close();
			strTemp.LoadString(NULL,IDS_SaveFileOK,CCarKey3App::LANGID);
			dlgPSW->ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
			//加密
			//byte FileA[0x800*0x08];//0x400*0x08
			//byte TMa[0x08],TMb[0x08];
			//for(int j=0;j<0x400;j++)
			//{
			//	for(int i=0;i<8;i++)
			//	{
			//		TMa[i]=pBuf[0x80*j+0x21+i];
			//	}
			//	for(int i=0;i<5;i++)
			//	{
			//		TMb[i]=pBuf[0x80*j+0x35+i];
			//	}
			//	for(int i=0;i<3;i++)
			//	{
			//		TMb[0x05+i]=pBuf[0x80*j+0x52+i];
			//	}
			//	for(int i=0;i<8;i++)
			//	{
			//		FileA[0x10*j+0x00+i]=TMa[i]^TMb[i];
			//	}
			//	for(int i=0;i<8;i++)
			//	{
			//		FileA[0x10*j+0x08+i]=pBuf[0x80*j+0x72+i];
			//	}
			//}
			byte* pEncrypt=CCommFunc::PSWEncrypt(pBuf,true);
			byte* encode=CCommFunc::Encrypt(pEncrypt,2048*20*4,CCarKey3App::TheHIDDevice.HexID,ssid);
			byte save[2048*20*4+16];
			for(int x=0;x<4;x++)
			{
				save[x]=CCarKey3App::TheHIDDevice.HexID[x];
				save[x+4]=ssid[x];
			}
			save[8]=dlgPSW->m_nQuanDiuType;
			for(int x=0;x<2048*20*4;x++)
			{
				save[16+x]=encode[x];
			}
			delete[] encode;
			CFile file1(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
			file1.Write(save,2048*20*4+16);
			file1.Close();
		}	
		strFileName.ReleaseBuffer();
		delete[] pBuf;
		dlgPSW->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		dlgPSW->GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
		dlgPSW->GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
		dlgPSW->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		strTemp.LoadString(NULL,IDS_PSWCaiJi,CCarKey3App::LANGID);
		dlgPSW->GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
		strTemp.LoadString(NULL,IDS_PSWShangChuan,CCarKey3App::LANGID);
		dlgPSW->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
		dlgPSW->m_bThreadExit=true;
		dlgPSW->m_bRunning=false;
		dlgPSW->OnCbnSelchangeModel();
	}	
	VMProtectEnd();
}
// CDlgPSW 对话框

IMPLEMENT_DYNAMIC(CDlgPSW, CDialogEx)

CDlgPSW::CDlgPSW(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPSW::IDD, pParent)
	, m_bQuanDiu(FALSE)
{
	m_bRunning=false;
	m_bThreadExit=true;
}

CDlgPSW::~CDlgPSW()
{
}

void CDlgPSW::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK2, m_bQuanDiu);
	DDX_Control(pDX, IDC_COMBO4, m_ModelType);
	DDX_Control(pDX, IDC_PROGRESS3, m_Progress);
}


BEGIN_MESSAGE_MAP(CDlgPSW, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgPSW::OnBnReadData)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDlgPSW::OnCbnSelchangeModel)
	//ON_MESSAGE(HIDMSG,OnHIDMsgBack)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgPSW::OnBnHPassWork)
END_MESSAGE_MAP()


// CDlgPSW 消息处理程序


void CDlgPSW::OnBnReadData()
{
	//m_bQuanDiu=TRUE;
// 	_beginthread(SuanMiMaThread,0,(void *)(this));
// 	return;
//  	DWORD dwFileLen;
//  	byte *pBuf;
//  	unsigned int i,j;
//  
//  	unsigned char CNT;
//  	unsigned char TP[0x08];
//  	char DataT[16+1];
//  	unsigned char FileA[0x400*0x08];
//  	unsigned char FileB[0x400*0x20];//0x1a
//  	//--->
//  	CFileDialog fileDlg(TRUE);
//  	CString strTemp,strTemp1;
//  	strTemp.LoadString(IDS_OpenFileTitle);
//  	strTemp1.Format(strTemp,_T("EIS"));
//  	fileDlg.m_ofn.lpstrTitle=strTemp1;
//  	fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
//  	if (IDOK!=fileDlg.DoModal())
//  		return;
//  	CFile file(fileDlg.GetPathName(),CFile::modeRead);
//  	//StrName=fileDlg.GetPathName();			//file path
//  	dwFileLen=file.GetLength();
//  	pBuf=new byte[dwFileLen+1];
//  	pBuf[dwFileLen]=0;
//  	file.Read(pBuf,dwFileLen);
//  	file.Close();
//  	byte* pSave=CCommFunc::PSWEncrypt(pBuf);
//  	CFileDialog fileDlgS(FALSE);
//  	fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0EZS Files(*.EZS)\0*.EZS\0All Files(*.*)\0*.*");
//  	fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
//  	strTemp.LoadString(IDS_SaveFileTitle);
//  	fileDlgS.m_ofn.lpstrTitle=strTemp;
//  
//  	if (IDOK==fileDlgS.DoModal())
//  	{		
//  		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
//  		file.Write(pSave,1024*20*4);
//  		file.Close();
//  		strTemp.LoadString(IDS_SaveFileOK);
//  		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
//  		byte *pSave1=CCommFunc::PSWDecrypt(pSave);
//  		CFile file1(fileDlgS.GetPathName()+_T("qwe"),CFile::modeCreate|CFile::modeWrite);
//  		file1.Write(pSave1,1024*16*4);
//  		file1.Close();
//  		strTemp.LoadString(IDS_SaveFileOK);
//  		ShowLog(strTemp+_T(":")+fileDlgS.GetPathName());
//  	}
//  	return;
  	CString str,strTemp,strTemp1,strInfo;
  	strTemp.LoadString(IDS_PSWStop);
  	GetDlgItem(IDC_BUTTON1)->GetWindowText(str);
  	if(str==strTemp)
  	{
  		strTemp.LoadString(IDS_PSWTip12);
  		strTemp1.LoadString(IDS_Warring);
  		if(MessageBox(strTemp, strTemp1,MB_YESNO) == IDYES)
  		{
  			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
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
  			//ShowLog(strx);
  			MessageBox(strx);
  			return;
  		}	
  		strTemp.LoadString(IDS_StartCANInfo);
  		ShowLog(strTemp);	
  		UpdateData(TRUE);
  		strTemp.LoadString(IDS_PSWStop);
  		GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
  		m_bRunning=true;
  		m_bThreadExit=false;
  		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
  		GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);
  		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
  		_beginthread(SuanMiMaThread,0,(void *)(this));
  	}
	
}
void CDlgPSW::ShowLog( CString str )
{
	m_StatusBar.SetText(str, 0, 0);
}

BOOL CDlgPSW::OnInitDialog()
{
	CDialogEx::OnInitDialog();	 
	// TODO:  在此添加额外的初始化
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
	CWnd *pWnd;	
	pWnd = GetDlgItem(IDC_CHECK2);     
	pWnd->EnableWindow(FALSE);
	OnCbnSelchangeModel();
	CString strTemp;
	strTemp.LoadString(IDS_PSWCaiJi);
	GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
	strTemp.LoadString(IDS_PSWShangChuan);
	GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);

	strTemp.LoadString(IDS_PSWUI1);
	GetDlgItem(IDC_STATIC20)->SetWindowText(strTemp);

	strTemp.LoadString(IDS_PSWUI2);
	GetDlgItem(IDC_STATIC24)->SetWindowText(strTemp);

	strTemp.LoadString(IDS_PSWUI3);
	GetDlgItem(IDC_CHECK2)->SetWindowText(strTemp);

	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);

	int strPartDim[1]= {-1}; //分割数量
	m_StatusBar.SetParts(1, strPartDim);
	//设置状态栏文本
	m_StatusBar.SetText(_T(""), 0, 0);
	//m_StatusBar.SetText(_T("2"), 1, 0);
	//下面是在状态栏中加入图标
	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPSW::OnCbnSelchangeModel()
{
	// TODO: 在此添加控件通知处理程序代码
	int sel=m_ModelType.GetCurSel();
	CWnd *pWnd;	
	pWnd = GetDlgItem(IDC_CHECK2);     
	pWnd->EnableWindow(FALSE);
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
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=3;
		}
		break;
	case 2://W166-NEC
		{
			m_nMode=7;//08款==166 09款==246 166和246这两个是一样的
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=3;
		}
		break;
	case 3://W204-NEC
		{
			m_nMode=1;//02款==204	
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=2;
		}
		break;
	case 4://W207-NEC
		{
			m_nMode=1;//02款==204
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=2;
		}
		break;
	case 5://W212-NEC-old
		{
			m_nMode=2;//03款==212old
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=3;
		}
		break;
	case 6://W212-NEC-new
		{
			m_nMode=5;//06款==212new
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=3;
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
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=3;
		}
		break;
	case 9://W246-NEC
		{
			m_nMode=8;//09款==246
			pWnd->EnableWindow(TRUE);
			m_nQuanDiuType=3;
		}
		break;
	}
}

LRESULT CDlgPSW::OnHIDMsgBack( WPARAM wParam,LPARAM lParam )
{
// 	CString strTemp,strTemp1;
// 	strTemp.LoadString(IDS_PSWTime);
// 	strTemp1.Format(strTemp,0,0);
// 	m_StatusBar.SetText(strTemp1,0,0);	
// 	int minute,second;
// 	dwStart   =   GetTickCount(); 
// 	m_Progress.SetRange(0,0x400);   //设置进度条的范围为0-1000	
// 	m_Progress.SetPos(0);
// 	m_Progress.UpdateWindow();
	if(wParam==1)//采集中,设置状态栏
	{
		CString* str = (CString*)lParam;
		m_StatusBar.SetText(*str,0,0);
	}
	return 0;
}



void CDlgPSW::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bRunning && !m_bThreadExit)
		return;
	CDialogEx::OnClose();
}
void exitHPassThread(void* p)
{
	CDlgPSW* dlgPSW=(CDlgPSW*)(p);
	CString strTemp;
	dlgPSW->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	dlgPSW->GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
	dlgPSW->GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
	dlgPSW->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	strTemp.LoadString(NULL,IDS_PSWCaiJi,CCarKey3App::LANGID);
	dlgPSW->GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
	strTemp.LoadString(NULL,IDS_PSWShangChuan,CCarKey3App::LANGID);
	dlgPSW->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
	dlgPSW->m_bThreadExit=true;
	dlgPSW->m_bRunning=false;
}
bool CheckHPass(CString filePath,byte* answer,byte* query,byte* psw)
{
	CPassCalc cp;
	for(int i=0;i<8;i++)
	{
		cp.HashIn[i]=answer[i];
	}
	cp.sub_HashOnce();
	for(int i=0;i<8;i++)
	{
		if(query[i]!=cp.HashOut[i])
		{
			return false;
		}
	}
	CFile file(filePath,CFile::modeRead);
	CString str,strTemp,strTemp1,strInfo;
	//StrName=fileDlg.GetPathName();			//file path
	UINT dwFileLen=(UINT)file.GetLength();
	byte* pBuf=new byte[dwFileLen+1];
	pBuf[dwFileLen]=0;
	file.Read(pBuf,dwFileLen);
	file.Close();
	if(dwFileLen==(1024*20*4+8))
	{		
		byte dID[4],sID[4];
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+8,1024*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode);
		byte FileA[0x400*0x08];
		for(int j=0;j<0x400;j++)
		{
			for(int i=0;i<8;i++)
			{
				FileA[0x08*j+i]=pData[0x40*j+0x30+i]^pData[0x40*j+0x21+i];
			}
		}

		for(int j=0;j<0x400;j++)
		{	
			bool bSame=true;
			for(int i=0;i<8;i++)
			{
				if(FileA[0x08*j+i]!=query[i])
				{			
					bSame=false;
					break;
				}
			}
			if(bSame)
			{
				//---
				byte TPA[0x08],TPB[0x08],TPC[0x08],TPD[0x08],TPE[0x08];
				for(int i=0;i<0x08;i++)
				{
					TPA[i]=answer[i];
				}
				for(int i=0;i<0x08;i++)
				{
					TPB[i]=pData[0x40*j+0x11+i];
				}
				TPC[0]=0;
				TPC[1]=0;
				TPC[2]=0;
				TPC[3]=0;
				TPC[4]=pData[0x40*j+0x01];
				TPC[5]=pData[0x40*j+0x02];
				TPC[6]=pData[0x40*j+0x03];
				TPC[7]=pData[0x40*j+0x00];
				//---				
				for(int i=0;i<0x08;i++)
				{
					psw[i]=TPA[i]^TPB[i]^TPC[i];
				}
				int err=0;
				byte Ftmp[0x40];
				for(int j=0;j<0x400;j++)
				{
					for(int i=0;i<0x40;i++)
					{
						Ftmp[i]=pData[0x40*j+i];//取出一组数据到暂存
					}
					for(int i=0;i<0x08;i++)
					{
						TPA[i]=Ftmp[0x11+i];
						TPB[i]=Ftmp[0x30+i];
						TPE[i]=Ftmp[0x21+i];//最终待比较结果
					}
					TPC[0x00]=0x00;
					TPC[0x01]=0x00;
					TPC[0x02]=0x00;
					TPC[0x03]=0x00;
					TPC[0x04]=Ftmp[0x01];
					TPC[0x05]=Ftmp[0x02];
					TPC[0x06]=Ftmp[0x03];
					TPC[0x07]=Ftmp[0x00];
					//
					//---
					CPassCalc cp1;
					for(int i=0;i<0x08;i++)
					{
						cp1.HashIn[i]=TPA[i]^psw[i]^TPC[i];
					}
					cp1.sub_HashOnce();
					for(int i=0;i<0x08;i++)
					{
						TPD[i]=cp1.HashOut[i]^TPB[i];
					}
					//
					bool fCHK=true;
					for(int i=0;i<0x08;i++)
					{
						if(TPD[i]!=TPE[i])
						{
							fCHK=false;
							break;
						}
					}
					if(false==fCHK)
					{
						err++;
						if(err>50)
						{
							delete[] pBuf;
							delete[] pDecode;
							delete[] pData;
							return false;
						}
					}
				}
				delete[] pBuf;
				delete[] pDecode;
				delete[] pData;
				return true;
			}
		}
		delete[] pBuf;
		delete[] pDecode;
		delete[] pData;
		return false;
	}
	else if(dwFileLen==(2048*20*4+16))
	{		
		byte dID[4],sID[4];
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+16,2048*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode,true);
		byte FileA[0x800*0x08];
		byte TMa[0x08],TMb[0x08];
		byte TPA[0x08],TPB[0x08],TPC[0x08],TPD[0x08],TPE[0x08];
		if(pBuf[8]==3)//03款
		{
			for(int j=0;j<0x400;j++)
			{			
				for(int i=0;i<8;i++)
				{
					TMa[i]=pData[0x80*j+0x21+i];
				}
				for(int i=0;i<5;i++)
				{
					TMb[i]=pData[0x80*j+0x35+i];
				}
				for(int i=0;i<3;i++)
				{
					TMb[0x05+i]=pData[0x80*j+0x52+i];
				}
				for(int i=0;i<8;i++)
				{
					FileA[0x10*j+0x00+i]=TMa[i]^TMb[i];
				}
				for(int i=0;i<8;i++)
				{
					FileA[0x10*j+0x08+i]=pData[0x80*j+0x72+i];
				}
			}
			bool bSame=true;
			for(int j=0;j<0x800;j++)
			{
				bSame=true;
				for(int i=0;i<8;i++)
				{
					if(FileA[0x08*j+i]!=query[i])
					{			
						bSame=false;
						break;
					}
				}
				if(bSame)
				{
					byte FT[0x80];
					for(int i=0;i<0x80;i++)
					{
						FT[i]=pData[0x80*(j/2)+i];
					}
					//
					for(int i=0;i<0x08;i++)
					{
						TPA[i]=answer[i];
					}
					//
					if(0x00==j%2)
					{
						for(int i=0;i<0x02;i++)
						{
							TPB[i]=FT[i+0x00];
						}
						for(int i=0;i<0x06;i++)
						{
							TPB[i+0x02]=FT[i+0x11];
						}
						for(int i=0;i<0x08;i++)
						{
							TPC[i]=TPB[i]^TPA[i];
						}
						//
						for(int i=0;i<0x04;i++)
						{
							psw[i]=TPC[i+0x04];
						}
						for(int i=0;i<0x04;i++)
						{
							psw[i+0x04]=TPC[i];
						}
					}
					else
					{
						for(int i=0;i<0x07;i++)
						{
							TPB[i]=FT[i+0x33];
						}
						TPB[0x07]=FT[0x52];
						//
						for(int i=0;i<0x08;i++)
						{
							TPC[i]=TPB[i]^TPA[i];
						}
						//
						for(int i=0;i<0x03;i++)
						{
							psw[i]=TPC[i+0x05];
						}
						for(int i=0;i<0x05;i++)
						{
							psw[i+0x03]=TPC[i];
						}
					}
					break;				
				}
			}
			if(bSame)
			{
				byte Ftmp[0x80];
				int err=0;
				for(int j=0;j<0x400;j++)
				{
					for(int i=0;i<0x80;i++)
					{
						Ftmp[i]=pData[0x80*j+i];//取出一组数据到暂存
					}
					for(int i=0;i<0x07;i++)
					{
						TPA[i]=Ftmp[0x33+i];
					}
					TPA[0x07]=Ftmp[0x52];
					//
					TPB[0x00]=psw[0x03];
					TPB[0x01]=psw[0x04];
					TPB[0x02]=psw[0x05];
					TPB[0x03]=psw[0x06];
					TPB[0x04]=psw[0x07];
					TPB[0x05]=psw[0x00];
					TPB[0x06]=psw[0x01];
					TPB[0x07]=psw[0x02];
					//
					for(int i=0;i<0x08;i++)
					{
						TPC[i]=Ftmp[0x72+i];//最终待比较结果
					}
					//---
					CPassCalc cp1;
					for(int i=0;i<0x08;i++)
					{
						cp1.HashIn[i]=TPA[i]^TPB[i];
					}
					cp1.sub_HashOnce();
					for(int i=0;i<0x08;i++)
					{
						TPD[i]=cp1.HashOut[i];
					}
					//
					bool fCHK=true;
					for(int i=0;i<0x08;i++)
					{
						if(TPD[i]!=TPC[i])
						{
							fCHK=false;
						}
					}
					if(false==fCHK)
					{
						CString xxx;
						xxx.Format(_T("%d\r\n"),j);
						TRACE(xxx);
						err++;
						if(err>50)
						{
							delete[] pBuf;
							delete[] pDecode;
							delete[] pData;
							return false;
						}
					}			
				}
				delete[] pBuf;
				delete[] pDecode;
				delete[] pData;
				return true;
			}
		}
		else//02款
		{
			for(int j=0;j<0x400;j++)
			{
				for(int i=0;i<8;i++)
				{
					TMa[i]=pData[0x40*j+0x30+i];//串口收到的数据
				}
				for(int i=0;i<8;i++)
				{
					TMb[i]=pData[0x40*j+0x21+i];//27开头的数据
				}
				for(int i=0;i<8;i++)
				{
					FileA[0x10*j+0x00+i]=TMa[i]^TMb[i];
				}
			}
			bool fSam;
			bool fCAL=false;
			int j=0,i;
			while(false==fCAL)
			{
				for(i=0;i<8;i++)
				{
					TPB[i]=FileA[0x10*j+i];
				}
				fSam=true;
				for(i=0;i<0x08;i++)
				{
					if(query[i]!=TPB[i])
					{
						fSam=false;
					}
				}
				if(false==fSam)
				{
					j++;
					if(j>=0x400)
					{
						fCAL=true;
					}
				}
				else
				{
					fCAL=true;
				}
			}
			//---
			if(true==fSam)
			{
				for(i=0;i<8;i++)
				{
					TPA[i]=answer[i];//dis hash temp save
				}
				TPB[0]=0x00;
				TPB[1]=0x00;
				TPB[2]=0x00;
				TPB[3]=0x00;
				TPB[4]=pData[0x40*j+0x01];
				TPB[5]=pData[0x40*j+0x02];
				TPB[6]=pData[0x40*j+0x03];
				TPB[7]=pData[0x40*j+0x00];
				//
				for(i=0;i<0x08;i++)
				{
					TPC[i]=pData[0x40*j+0x11+i];
				}
				//
				for(i=0;i<0x08;i++)
				{
					psw[i]=TPA[i]^TPB[i]^TPC[i];
				}
			}
			if(true==fSam)
			{
				//
				//---<
				int err=0;
				bool fSam=true;
				bool fCAL=false;
				unsigned char Ftmp[0x80];
				j=0;
				while(false==fCAL)
				{
					for(i=0;i<0x40;i++)
					{
						Ftmp[i]=pData[0x40*j+i];//取出一组数据到暂存
					}

					for(i=0;i<8;i++)
					{
						TPA[i]=Ftmp[0x11+i];//26开头的数据
					}
					TPB[0]=0x00;
					TPB[1]=0x00;
					TPB[2]=0x00;
					TPB[3]=0x00;
					TPB[4]=Ftmp[0x01];
					TPB[5]=Ftmp[0x02];
					TPB[6]=Ftmp[0x03];
					TPB[7]=Ftmp[0x00];

					for(i=0;i<8;i++)
					{
						TPC[i]=Ftmp[0x21+i];//27开头的数据
					}

					for(i=0;i<8;i++)
					{
						TPD[i]=Ftmp[0x30+i];//串口收到的数据
					}
					//
					CPassCalc cp1;
					for(i=0;i<8;i++)
					{
						cp1.HashIn[i]=TPA[i]^psw[i]^TPB[i];
					}
					cp1.sub_HashOnce();
					for(i=0;i<8;i++)
					{
						TPE[i]=cp1.HashOut[i]^TPC[i];
					}

					bool fCHK=true;
					for(i=0;i<0x08;i++)
					{
						if(TPD[i]!=TPE[i])
						{
							fCHK=false;
						}
					}
					if(false==fCHK)
					{
						CString xxx;
						xxx.Format(_T("%d\r\n"),j);
						TRACE(xxx);
						err++;
						if(err>50)
						{
							delete[] pBuf;
							delete[] pDecode;
							delete[] pData;
							return false;
						}
					}
					else
					{
						j++;
						if(j>=0x400)//1024组
						{
							fCAL=true;//计算结尾处
						}
					}
				}
				delete[] pBuf;
				delete[] pDecode;
				delete[] pData;
				return true;
			}
		}		
	}
	return false;
}
void HPassThread(void* p)
{
	VMProtectBegin("HPassThread");
	CDlgPSW* dlgPSW=(CDlgPSW*)(p);
	CFile file(dlgPSW->m_strFilePath,CFile::modeRead);
	CString str,strTemp,strTemp1,strInfo;
	//StrName=fileDlg.GetPathName();			//file path
	UINT dwFileLen=(UINT)file.GetLength();
	byte* pBuf=new byte[dwFileLen+1];
	pBuf[dwFileLen]=0;
	file.Read(pBuf,dwFileLen);
	file.Close();
	byte ds[8];
	for(int i=0;i<8;i++)
		ds[i]=pBuf[i];
	CString ret;
	bool bUp;
	CString fileName;
	strTemp.LoadString(NULL,IDS_ServerTip13,CCarKey3App::LANGID);
	dlgPSW->ShowLog(strTemp);
	byte dID[4],sID[4];
	CWebRetData webRet;
	dlgPSW->m_Progress.SetRange(0,100);
	dlgPSW->m_Progress.SetPos(0);
	dlgPSW->m_Progress.UpdateWindow();
	if(dwFileLen==(1024*20*4+8))
	{
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+8,1024*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode);
		byte tx[0x401*0x08],tx1[0x400*0x08];
		for(int i=0;i<8;i++)
			tx[i]=pBuf[i];
		for(int j=0;j<1024;j++)
		{
		 	for(int i=0;i<8;i++)
		 	{
		 		tx1[0x08*j+i]=pData[0x40*j+0x30+i]^pData[0x40*j+0x21+i];
		 	}
		}
		byte* pDecode1=CCommFunc::Encrypt(tx1,0x400*0x08,dID,sID);
		for(int i=0;i<0x400*0x08;i++)
		{
			tx[8+i]=pDecode1[i];
		}
		bUp=CCarKey3App::withServer.UploadHPass(tx,0x401*0x08,webRet);
		delete[] pBuf;
		delete[] pDecode;
		delete[] pDecode1;
		delete[] pData;
	}
	else if(dwFileLen==(2048*20*4+16))
	{		
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+16,2048*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode,true);		
		byte tx[0x801*0x08],tx1[0x800*0x08];
		for(int i=0;i<8;i++)
			tx[i]=pBuf[i];
		byte TMa[0x08],TMb[0x08];
		if(pBuf[8]==3)//03款
		{			
			for(int j=0;j<0x400;j++)
			{
				for(int i=0;i<8;i++)
				{
					TMa[i]=pData[0x80*j+0x21+i];
				}
				for(int i=0;i<5;i++)
				{
					TMb[i]=pData[0x80*j+0x35+i];
				}
				for(int i=0;i<3;i++)
				{
					TMb[0x05+i]=pData[0x80*j+0x52+i];
				}
				for(int i=0;i<8;i++)
				{
					tx1[0x10*j+0x00+i]=TMa[i]^TMb[i];
				}
				for(int i=0;i<8;i++)
				{
					tx1[0x10*j+0x08+i]=pData[0x80*j+0x72+i];
				}
			}
			byte* pDecode1=CCommFunc::Encrypt(tx1,0x800*0x08,dID,sID);
			for(int i=0;i<0x800*0x08;i++)
			{
				tx[8+i]=pDecode1[i];
			}
			bUp=CCarKey3App::withServer.UploadHPass(tx,0x801*0x08,webRet);
			delete[] pBuf;
			delete[] pDecode;
			delete[] pData;
			delete[] pDecode1;
		}
		else
		{
			for(int j=0;j<0x400;j++)
			{
				for(int i=0;i<8;i++)
				{
					TMa[i]=pData[0x40*j+0x30+i];//串口收到的数据
				}
				for(int i=0;i<8;i++)
				{
					TMb[i]=pData[0x40*j+0x21+i];//27开头的数据
				}
				for(int i=0;i<8;i++)
				{
					tx1[0x10*j+0x00+i]=TMa[i]^TMb[i];
				}
			}
			byte tx2[0x400*0x08];
			for(int j=0;j<0x400;j++)
			{
				for(int i=0;i<8;i++)
				{
					tx2[0x08*j+i]=tx1[0x10*j+i];
				}
			}
// 			CFileDialog fileDlgS(FALSE);
// 			fileDlgS.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
// 			fileDlgS.m_ofn.lpstrDefExt=_T("BIN");
// 			strTemp.LoadString(NULL,IDS_SaveFileTitle,CCarKey3App::LANGID);
// 			fileDlgS.m_ofn.lpstrTitle=strTemp;	
// 			fileDlgS.m_ofn.nMaxFile = MAX_PATH;	
// 			CString strFileName=_T("xxxxx");
// 			fileDlgS.m_ofn.lpstrFile=strFileName.GetBuffer(MAX_PATH);
// 			if (IDOK==fileDlgS.DoModal())
// 			{
// 				CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
// 				file.Write(tx2,0x400*0x08);
// 				file.Close();
// 			}
// 			strFileName.ReleaseBuffer();
			byte* pDecode1=CCommFunc::Encrypt(tx2,0x400*0x08,dID,sID);
			for(int i=0;i<0x400*0x08;i++)
			{
				tx[8+i]=pDecode1[i];
			}
			bUp=CCarKey3App::withServer.UploadHPass(tx,0x401*0x08,webRet);
			delete[] pBuf;
			delete[] pDecode;
			delete[] pDecode1;
			delete[] pData;
		}
		//--
// 		unsigned char FileB[0x800*0x20];//0x1a
// 		unsigned char TP[0x08];	
// 		char DataT[16+1];
// 		for(int j=0;j<0x400;j++)
// 		{
// 			for(int i=0;i<8;i++)
// 			{
// 				TP[i]=tx1[0x10*j+i];
// 			}
// 			//CNT = 8;
// 			CCommFunc::cal_asic(8,TP,DataT);
// 			for(int i=0;i<8;i++)
// 			{
// 				FileB[0x1a*j+3*i+0]=DataT[0x02*i+0];
// 				FileB[0x1a*j+3*i+1]=DataT[0x02*i+1];
// 				FileB[0x1a*j+3*i+2]=' ';
// 			}
// 			FileB[0x1a*j+0x18]=0x0D;
// 			FileB[0x1a*j+0x19]=0x0A;
// 			//
// 		}
// 		CFileDialog fileDlgSx(FALSE);
// 		fileDlgSx.m_ofn.lpstrTitle=_T("保存为文件:*.txt");
// 		fileDlgSx.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
// 		fileDlgSx.m_ofn.lpstrDefExt=_T("txt");
// 		if (IDOK==fileDlgSx.DoModal())
// 		{
// 			CFile file(fileDlgSx.GetPathName(),CFile::modeCreate|CFile::modeWrite);
// 			//file.Write(FileA,0x400*0x08);
// 			file.Write(FileB,0x400*0x1A);
// 			file.Close();
// 		}
		
	}
	else
	{
		strTemp.LoadString(NULL,IDS_ServerTip14,CCarKey3App::LANGID);
		MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
		dlgPSW->ShowLog(strTemp);	
		exitHPassThread(p);
		return;
	}	
	if(bUp)
	{
		if(webRet.retStatus!=web_OK)
		{
			if(webRet.retStatus==web_MaxUseTimes)
			{
				strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);
				MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgPSW->ShowLog(strTemp);
				exitHPassThread(p);
				return;
			}
			else if(webRet.retStatus==web_NoRegDevice)
			{
				strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);
				MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgPSW->ShowLog(strTemp);
				exitHPassThread(p);
				return;
			}
			else
			{
				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				strTemp1.Format(strTemp,webRet.retStatus);
				MessageBoxEx(dlgPSW->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgPSW->ShowLog(strTemp1);
				exitHPassThread(p);
				return;
			}	
		}
		fileName=webRet.strResult;
		//继续获取状态
		strTemp.LoadString(NULL,IDS_ServerTip15,CCarKey3App::LANGID);
		//MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,0);
		dlgPSW->ShowLog(strTemp);
		int err=0;
		int noId=0;
		while(true)
		{
			if(!dlgPSW->m_bRunning)
			{
				strTemp.LoadString(NULL,IDS_ServerTip16,CCarKey3App::LANGID);
				dlgPSW->ShowLog(strTemp);
				exitHPassThread(p);
				return;
			}
			for(int i=1;i<11;i++)
			{
				Sleep(1000);
				// 						dlgPSW->m_Progress.SetPos(i);	
				// 						dlgPSW->m_Progress.UpdateWindow();
			}
			if(CCarKey3App::withServer.HPassStatus(fileName,webRet))
			{
				if(webRet.retStatus==web_OK)
				{
					dlgPSW->m_Progress.SetPos(100);	
					dlgPSW->m_Progress.UpdateWindow();
					CStringArray strArray;//读取结果存放的数组
					CCarKey3App::withServer.GetRetList(webRet.strResult,_T("|"),strArray);
					if(strArray.GetCount()>1)
					{
						bool bPass=false;
						for(int i=0;i<strArray.GetCount();i++)
						{
							CStringArray strArray1;
							CCarKey3App::withServer.GetRetList(strArray.GetAt(i),_T("-"),strArray1);
							if(strArray1.GetCount()==2)
							{
								int len;
								byte* query=CCommFunc::strToHexByte(strArray1.GetAt(0),_T(""),&len);
								byte* anwser=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
								byte psw[8];
								if(CheckHPass(dlgPSW->m_strFilePath,anwser,query,psw))//找到密码
								{
									strTemp.LoadString(NULL,IDS_ServerTip26,CCarKey3App::LANGID);
									strTemp1.Format(strTemp,CCommFunc::byteToHexStr(psw,8,_T(" ")));
									dlgPSW->ShowLog(strTemp1);
									MessageBoxEx(dlgPSW->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
									delete[] query;
									delete[] anwser;
									CCarKey3App::m_bPass=true;
									for(int i=0;i<4;i++)
										CCarKey3App::PassSSID[i]=sID[i];
									for(int i=0;i<8;i++)
										CCarKey3App::HPassword[i]=psw[i];
									bPass=true;
									//写入日志中
									CString strLog;
									CStdioFile file;													
									if(file.Open(_T("Password.log"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyNone))
									{
										CString strPart_Time,strPart_SSID,strPart_PSW;
										SYSTEMTIME sysTime = {0};
										GetLocalTime(&sysTime);
										strPart_Time.Format(_T("[%04d-%02d-%02d %02d:%02d:%02d]"), sysTime.wYear,
											sysTime.wMonth, sysTime.wDay, 
											sysTime.wHour, sysTime.wMinute, sysTime.wSecond 
											);
										strPart_SSID=CCommFunc::byteToHexStr(CCarKey3App::PassSSID,4,_T(" "));
										strPart_PSW=CCommFunc::byteToHexStr(CCarKey3App::HPassword,8,_T(" "));
										strLog.Format(CCarKey3App::strLogFormat,strPart_Time,strPart_SSID,strPart_PSW);
										file.SeekToEnd();
										file.WriteString(strLog);
										file.Close();
									}
									break;
								}
								delete[] query;
								delete[] anwser;
							}											
						}
						if(!bPass)
						{
							strTemp.LoadString(NULL,IDS_ServerTip18,CCarKey3App::LANGID);
							dlgPSW->ShowLog(strTemp);
							MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
						}
					}
					else
					{
						strTemp.LoadString(NULL,IDS_ServerTip18,CCarKey3App::LANGID);
						dlgPSW->ShowLog(strTemp);
						MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					}									
					exitHPassThread(p);
					return;
				}
				else if(webRet.retStatus==2)
				{
					int progress=_ttoi(webRet.strResult);
					if(progress==-1)
					{
						strTemp.LoadString(NULL,IDS_ServerTip17,CCarKey3App::LANGID);
						dlgPSW->ShowLog(strTemp);
					}
					else
					{
						strTemp.LoadString(NULL,IDS_ServerTip5,CCarKey3App::LANGID);					
						dlgPSW->m_Progress.SetPos(progress);	
						dlgPSW->m_Progress.UpdateWindow();
						strTemp1.Format(strTemp,progress);
						dlgPSW->ShowLog(strTemp1);
					}
				}
				else if(webRet.retStatus==3)
				{
					noId++;
					if(noId>3)
					{
						strTemp.LoadString(NULL,IDS_ServerTip19,CCarKey3App::LANGID);
						MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
						dlgPSW->ShowLog(strTemp);
						exitHPassThread(p);
						return;
					}
				}
				else
				{
					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,webRet.retStatus);
					MessageBoxEx(dlgPSW->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					dlgPSW->ShowLog(strTemp1);
					exitHPassThread(p);
					return;
				}
			}	
			else
			{
				err++;
				if(err>5)
				{
					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,1001);
					MessageBoxEx(dlgPSW->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					dlgPSW->ShowLog(strTemp1);
					exitHPassThread(p);
					return;
				}
			}
		}
		exitHPassThread(p);
		return;		
	}
	else
	{
		strTemp.LoadString(NULL,IDS_ServerTip20,CCarKey3App::LANGID);
		MessageBoxEx(dlgPSW->m_hWnd,strTemp,_T(""),MB_OK,0);
		dlgPSW->ShowLog(strTemp);
		exitHPassThread(p);
		return;
	}
	VMProtectEnd();
}

void CDlgPSW::OnBnHPassWork()
{	
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString str,strTemp,strTemp1,strInfo;
	strTemp.LoadString(IDS_PSWStop);
	GetDlgItem(IDC_BUTTON2)->GetWindowText(str);
	if(str==strTemp)
	{
		strTemp.LoadString(IDS_PSWTip12);
		strTemp1.LoadString(IDS_Warring);
		if(MessageBox(strTemp, strTemp1,MB_YESNO) == IDYES)
		{
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			strTemp.LoadString(IDS_ServerTip8);
			ShowLog(strTemp);
			m_bRunning=false;
		}
	}
	else
	{
		//--->
		CFileDialog fileDlg(TRUE);
		CString strTemp,strTemp1;
		strTemp.LoadString(IDS_OpenFileTitle);
		strTemp1.Format(strTemp,_T("PSW"));
		fileDlg.m_ofn.lpstrTitle=strTemp1;
		fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
		if (IDOK!=fileDlg.DoModal())
			return;
		m_strFilePath=fileDlg.GetPathName();	
		strTemp=fileDlg.GetFileName();
		m_strFileName=strTemp.Mid(0,strTemp.Find(_T(".")));
		strTemp.LoadString(IDS_PSWStop);
		GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
		m_bRunning=true;
		m_bThreadExit=false;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		_beginthread(HPassThread,0,(void *)(this));
	}
}
