// DlgKeyData.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgKeyData.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "..\..\CarKeyCommon\MyStringSplitter.h"
#include "..\..\CarKeyCommon\VMProtectSDK.h"

int CALLBACK BrowseCallBackFun(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)  
{  
	CString strTemp;
	switch(uMsg)  
	{  
	case BFFM_INITIALIZED:  //选择文件夹对话框初始化  
		//设置默认路径为lpData即'D:\'  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);  
		//在STATUSTEXT区域显示当前路径  
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, lpData);  
		//设置选择文件夹对话框的标题  
		strTemp.LoadString(IDS_KeyData5);
		::SetWindowText(hwnd, strTemp);   
		break;  
	case BFFM_SELCHANGED:   //选择文件夹变更时  
		{  
			TCHAR pszPath[MAX_PATH];  
			//获取当前选择路径  
			SHGetPathFromIDList((LPCITEMIDLIST)lParam, pszPath);  
			//在STATUSTEXT区域显示当前路径  
			::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)pszPath);  
		}  
		break;  
	}  
	return 0;  
}  
void exitKeyDataThread(CDlgKeyData* dlgKeyData)
{
	CString strTemp;
	strTemp.LoadString(NULL,IDS_KeyData4,CCarKey3App::LANGID);
	dlgKeyData->GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
	dlgKeyData->EnableControl(TRUE);
	dlgKeyData->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	dlgKeyData->m_bThreadExit=true;
	dlgKeyData->m_bRunning=false;
}
void KeyDataThread(void* p)
{
	VMProtectBegin("KeyDataThread");
	CDlgKeyData* dlgKeyData=(CDlgKeyData*)(p);
	CString queueID,strTemp,strTemp1,strRet;
	int retry=10;
	queueID=_T("");
	bool bOK=false;
	dlgKeyData->m_Progress.SetRange(0,10);
	dlgKeyData->m_Progress.SetPos(0);
	dlgKeyData->m_Progress.UpdateWindow();
	strTemp.LoadString(NULL,IDS_KeyData7,CCarKey3App::LANGID);
	dlgKeyData->ShowLog(strTemp);
	retry=10;
	bOK=false;
	while(retry!=0)
	{
		if(!dlgKeyData->m_bRunning)
		{
			strTemp.LoadString(NULL,IDS_ServerTip16,CCarKey3App::LANGID);
			dlgKeyData->ShowLog(strTemp);
			break;
		}
		CWebRetData webRet;
		if(CCarKey3App::withServer.AddKeyData(dlgKeyData->pBIN,0x400+7,webRet))
		{
			if(webRet.retStatus==web_OK)
			{
				queueID=webRet.strResult;
				bOK=true;
				strTemp.LoadString(NULL,IDS_KeyData9,CCarKey3App::LANGID);
				dlgKeyData->ShowLog(strTemp);
				break;
			}			
			else if(webRet.retStatus==web_NoRegDevice)
			{
				strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
				MessageBoxEx(dlgKeyData->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgKeyData->ShowLog(strTemp);
				break;
			}
			else
			{
				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				strTemp1.Format(strTemp,webRet.retStatus);
				MessageBoxEx(dlgKeyData->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgKeyData->ShowLog(strTemp1);
				break;
			}								
		}
		else
			retry--;
	}
	if(!bOK)
	{
		exitKeyDataThread(dlgKeyData);
		return;
	}
	retry=10;
	bOK=false;
	while(retry!=0)
	{
		if(!dlgKeyData->m_bRunning)
		{
			strTemp.LoadString(NULL,IDS_ServerTip16,CCarKey3App::LANGID);
			dlgKeyData->ShowLog(strTemp);
			break;
		}		
		for(int i=1;i<11;i++)
		{
			Sleep(1000);
			dlgKeyData->m_Progress.SetPos(i);
			dlgKeyData->m_Progress.UpdateWindow();
		}
		CWebRetData webRet;
		if(CCarKey3App::withServer.StatusKeyData(queueID,webRet))
		{		
			if(webRet.retStatus==web_OK)
			{
				bOK=true;
				break;
			}			
			else if(webRet.retStatus==2)//刚开始排队
			{
				int queue=_ttoi(webRet.strResult);
				if(queue==1)
				{
					strTemp.LoadString(NULL,IDS_KeyData8,CCarKey3App::LANGID);
					dlgKeyData->ShowLog(strTemp);
				}
				else
				{
					strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,queue-1);
					dlgKeyData->ShowLog(strTemp1);
				}
			}
			else if(webRet.retStatus==4 || webRet.retStatus==3)//完成了，发生错误
			{
				strTemp.LoadString(NULL,IDS_KeyData16,CCarKey3App::LANGID);
				MessageBoxEx(dlgKeyData->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgKeyData->ShowLog(strTemp);
				break;
			}
			else
			{
				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				strTemp1.Format(strTemp,webRet.retStatus);
				MessageBoxEx(dlgKeyData->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgKeyData->ShowLog(strTemp1);
				break;
			}								
		}
		else
			retry--;
	}
	if(!bOK)
	{
		exitKeyDataThread(dlgKeyData);
		return;
	}
	TCHAR pszPath[MAX_PATH];  
	strTemp.LoadString(NULL,IDS_KeyData6,CCarKey3App::LANGID);
	BROWSEINFO bi;   
	bi.hwndOwner      = dlgKeyData->GetSafeHwnd();  
	bi.pidlRoot       = NULL;  
	bi.pszDisplayName = NULL;   
	bi.lpszTitle      = strTemp;   
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;  
	bi.lpfn           = BrowseCallBackFun;   
	bi.lParam         = (LPARAM)TEXT("");  
	bi.iImage         = 0;   

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
	if (pidl == NULL)  
	{  
		exitKeyDataThread(dlgKeyData);
		return;  
	}  
	if (!SHGetPathFromIDList(pidl, pszPath))
	{  
		exitKeyDataThread(dlgKeyData);
		return;  
	}
	int formatCount=3;
	int keyCount=8;
	CString keyIndex;
	CString format;
	bool bGet=false;
	strTemp.LoadString(NULL,IDS_KeyData10,CCarKey3App::LANGID);
	dlgKeyData->ShowLog(strTemp);
	while(formatCount!=0)
	{
		if(!dlgKeyData->m_bRunning)
		{
			strTemp.LoadString(NULL,IDS_ServerTip16,CCarKey3App::LANGID);
			dlgKeyData->ShowLog(strTemp);
			break;
		}
		bGet=false;
		if(formatCount==3)
		{
			if(dlgKeyData->m_b51)
			{
				format=_T("51");				
			}
			else
			{
				formatCount--;
				keyCount=8;
				continue;
			}
		}
		else if(formatCount==2)
		{
			if(dlgKeyData->m_b41)
			{
				format=_T("41");				
			}
			else
			{
				formatCount--;
				keyCount=8;
				continue;
			}
		}
		else if(formatCount==1)
		{
			if(dlgKeyData->m_b11)
			{
				format=_T("11");				
			}
			else
			{
				formatCount--;
				keyCount=8;
				continue;
			}
		}
		keyCount--;
		switch(keyCount)
		{
		case 7:
			if(dlgKeyData->m_bKey7)
			{
				keyIndex="7";
				bGet=true;
			}
			break;
		case 6:
			if(dlgKeyData->m_bKey6)
			{
				keyIndex="6";
				bGet=true;
			}
			break;
		case 5:
			if(dlgKeyData->m_bKey5)
			{
				keyIndex="5";
				bGet=true;
			}
			break;
		case 4:
			if(dlgKeyData->m_bKey4)
			{
				keyIndex="4";
				bGet=true;
			}
			break;
		case 3:
			if(dlgKeyData->m_bKey3)
			{
				keyIndex="3";
				bGet=true;
			}
			break;
		case 2:
			if(dlgKeyData->m_bKey2)
			{
				keyIndex="2";
				bGet=true;
			}
			break;
		case 1:
			if(dlgKeyData->m_bKey1)
			{
				keyIndex="1";
				bGet=true;
			}
			break;
		case 0:
			if(dlgKeyData->m_bKey0)
			{
				keyIndex="0";
				bGet=true;
			}
			break;
		default:
			formatCount--;
			keyCount=8;
			break;
		}
		if(!bGet)
			continue;
		retry=10;		
		while(retry!=0)
		{
			if(!dlgKeyData->m_bRunning)
			{
				strTemp.LoadString(NULL,IDS_ServerTip16,CCarKey3App::LANGID);
				dlgKeyData->ShowLog(strTemp);
				break;
			}
			CWebRetData webRet;
			if(CCarKey3App::withServer.KeyDataGet(queueID,keyIndex,format,webRet))
			{				
				if(webRet.retStatus=web_OK)
				{
					CString strData=webRet.strResult;
					int len;
					byte* pSave=CCommFunc::strToHexByte(strData,_T(""),&len);
					CString fileName=pszPath;
					fileName+=_T("\\key_")+keyIndex+_T("_")+CCommFunc::byteToHexStr(dlgKeyData->pBIN,7,4,_T(""))+_T("_")+format+_T(".bin");
					CFile file(fileName,CFile::modeCreate|CFile::modeWrite);
					file.Write(pSave,len);
					file.Close();
					delete[] pSave;
					break;
				}		
				else if(webRet.retStatus==web_NoHexBin)
				{
					strTemp.LoadString(NULL,IDS_ServerTip29,CCarKey3App::LANGID);
					CString fileName=_T("key_")+keyIndex+_T("_")+CCommFunc::byteToHexStr(dlgKeyData->pBIN,7,4,_T(""))+_T("_")+format+_T(".bin");
					strTemp1.Format(strTemp,fileName);
					MessageBoxEx(dlgKeyData->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					dlgKeyData->ShowLog(strTemp1);
					break;
				}
				else
				{
					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,webRet.retStatus);
					//MessageBoxEx(dlgKeyData->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					dlgKeyData->ShowLog(strTemp1);
					break;
				}								
			}
			else
				retry--;
		}
	}
	exitKeyDataThread(dlgKeyData);
	strTemp.LoadString(NULL,IDS_KeyData11,CCarKey3App::LANGID);
	strTemp1.Format(strTemp,pszPath);
	dlgKeyData->ShowLog(strTemp1);	
	VMProtectEnd();
}
// CDlgKeyData 对话框

IMPLEMENT_DYNAMIC(CDlgKeyData, CDialogEx)

CDlgKeyData::CDlgKeyData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgKeyData::IDD, pParent)
	, m_bAllKey(FALSE)
	, m_bKey0(FALSE)
	, m_bKey1(FALSE)
	, m_bKey2(FALSE)
	, m_bKey3(FALSE)
	, m_bKey4(FALSE)
	, m_bKey5(FALSE)
	, m_bKey6(FALSE)
	, m_bKey7(FALSE)
	, m_bAllFormat(FALSE)
	, m_b11(FALSE)
	, m_b41(FALSE)
	, m_b51(FALSE)
{
	m_bRunning=false;
	m_bThreadExit=true;
}

CDlgKeyData::~CDlgKeyData()
{
}

void CDlgKeyData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bAllKey);
	DDX_Check(pDX, IDC_CHECK3, m_bKey0);
	DDX_Check(pDX, IDC_CHECK5, m_bKey1);
	DDX_Check(pDX, IDC_CHECK6, m_bKey2);
	DDX_Check(pDX, IDC_CHECK7, m_bKey3);
	DDX_Check(pDX, IDC_CHECK8, m_bKey4);
	DDX_Check(pDX, IDC_CHECK9, m_bKey5);
	DDX_Check(pDX, IDC_CHECK15, m_bKey6);
	DDX_Check(pDX, IDC_CHECK16, m_bKey7);
	DDX_Check(pDX, IDC_CHECK17, m_bAllFormat);
	DDX_Check(pDX, IDC_CHECK18, m_b11);
	DDX_Check(pDX, IDC_CHECK19, m_b41);
	DDX_Check(pDX, IDC_CHECK20, m_b51);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
}


BEGIN_MESSAGE_MAP(CDlgKeyData, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgKeyData::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK17, &CDlgKeyData::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgKeyData::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgKeyData 消息处理程序


void CDlgKeyData::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_bAllKey)
	{
		m_bKey0=TRUE;
		m_bKey1=TRUE;
		m_bKey2=TRUE;
		m_bKey3=TRUE;
		m_bKey4=TRUE;
		m_bKey5=TRUE;
		m_bKey6=TRUE;
		m_bKey7=TRUE;
	}
	else
	{
		m_bKey0=FALSE;
		m_bKey1=FALSE;
		m_bKey2=FALSE;
		m_bKey3=FALSE;
		m_bKey4=FALSE;
		m_bKey5=FALSE;
		m_bKey6=FALSE;
		m_bKey7=FALSE;
	}
	if (CCommFunc::GetBit(pBIN[7+4], 7))
		m_bKey0=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 6))
		m_bKey1=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 5))
		m_bKey2=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 4))
		m_bKey3=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 3))
		m_bKey4=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 2))
		m_bKey5=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 1))
		m_bKey6=FALSE;
	if (CCommFunc::GetBit(pBIN[7+4], 0))
		m_bKey7=FALSE;
	UpdateData(FALSE);
}


void CDlgKeyData::OnBnClickedCheck17()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_bAllFormat)
	{
		m_b11=TRUE;
		m_b41=TRUE;
		m_b51=TRUE;
	}
	else
	{
		m_b11=FALSE;
		m_b41=FALSE;
		m_b51=FALSE;
	}
	UpdateData(FALSE);
}


BOOL CDlgKeyData::OnInitDialog()
{
	CString strTemp;
	CDialogEx::OnInitDialog();
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,0,0), this, 0);
	int strPartDim[1]= {-1}; //分割数量
	m_StatusBar.SetParts(1, strPartDim);
	//设置状态栏文本
	m_StatusBar.SetText(_T(""), 0, 0);
	//下面是在状态栏中加入图标
	m_StatusBar.SetIcon(1, SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE));//为第二个分栏中加的图标
	// TODO:  在此添加额外的初始化
	if (CCommFunc::GetBit(pBIN[7+4], 7))
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 6))
		GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 5))
		GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 4))
		GetDlgItem(IDC_CHECK7)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 3))
		GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 2))
		GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 1))
		GetDlgItem(IDC_CHECK15)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 0))
		GetDlgItem(IDC_CHECK16)->EnableWindow(FALSE);
	strTemp.LoadString(IDS_KeyData2);
	GetDlgItem(IDC_STATIC1)->SetWindowText(strTemp);
	strTemp.LoadString(IDS_KeyData3);
	GetDlgItem(IDC_STATIC2)->SetWindowText(strTemp);
	strTemp.LoadString(IDS_KeyData4);
	GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
	strTemp.LoadString(IDS_KeyData1);
	SetWindowText(strTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgKeyData::ShowLog( CString str )
{
	m_StatusBar.SetText(str, 0, 0);
}


void CDlgKeyData::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	pBIN[4]=0xff;
// 	pBIN[5]=0xff;
// 	pBIN[6]=0xff;
// 	m_bRunning=true;
// 	m_bThreadExit=false;
// 	_beginthread(KeyDataThread,0,(void *)(this));

	CString strTemp,strTemp1,str;
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
		for(int i=0;i<4;i++)
			pBIN[i]=CCarKey3App::TheHIDDevice.HexID[i];
		UpdateData(TRUE);
		pBIN[4]=0x00;
		pBIN[5]=0x00;
		pBIN[6]=0x00;
		if(m_bKey0)
			pBIN[5] |= 0x01;
		if(m_bKey1)
			pBIN[5] |= 0x02;
		if(m_bKey2)
			pBIN[5] |= 0x04;
		if(m_bKey3)
			pBIN[5] |= 0x08;
		if(m_bKey4)
			pBIN[5] |= 0x10;
		if(m_bKey5)
			pBIN[5] |= 0x20;
		if(m_bKey6)
			pBIN[5] |= 0x40;
		if(m_bKey7)
			pBIN[5] |= 0x80;
		if(m_b11)
			pBIN[6] |= 0x01;
		if(m_b41)
			pBIN[6] |= 0x02;
		if(m_b51)
			pBIN[6] |= 0x04;
// 		if(m_bAllKey)
// 			pBIN[4] |= 0x01;
		if(pBIN[5]==0x00)
		{
			strTemp.LoadString(IDS_KeyData14);
			MessageBox(strTemp);
			return;
		}
		if(pBIN[6]==0x00)
		{
			strTemp.LoadString(IDS_KeyData15);
			MessageBox(strTemp);
			return;
		}
		m_Progress.SetPos(0);
		strTemp.LoadString(IDS_PSWStop);
		GetDlgItem(IDC_BUTTON1)->SetWindowText(strTemp);
		m_bRunning=true;
		m_bThreadExit=false;
		EnableControl(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		_beginthread(KeyDataThread,0,(void *)(this));
	}
}

void CDlgKeyData::EnableControl( BOOL bEn )
{
	GetDlgItem(IDC_CHECK1)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK3)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK5)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK6)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK7)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK8)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK9)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK15)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK16)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK17)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK18)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK19)->EnableWindow(bEn);
	GetDlgItem(IDC_CHECK20)->EnableWindow(bEn);
	if (CCommFunc::GetBit(pBIN[7+4], 7))
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 6))
		GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 5))
		GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 4))
		GetDlgItem(IDC_CHECK7)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 3))
		GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 2))
		GetDlgItem(IDC_CHECK9)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 1))
		GetDlgItem(IDC_CHECK15)->EnableWindow(FALSE);
	if (CCommFunc::GetBit(pBIN[7+4], 0))
		GetDlgItem(IDC_CHECK16)->EnableWindow(FALSE);
}
