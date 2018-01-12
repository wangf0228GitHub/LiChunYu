// UpdateSoft.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "UpdateSoft.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\MyStringSplitter.h"
#include "..\..\CarKeyCommon\CommFunc.h"
#include "..\..\CarKeyCommon\InteractionData.h"


// CUpdateSoft 对话框

IMPLEMENT_DYNAMIC(CUpdateSoft, CDialogEx)

CUpdateSoft::CUpdateSoft(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateSoft::IDD, pParent)
{
	m_bOK=false;
	m_bClose=false;
	m_nUpType=0;
}

CUpdateSoft::~CUpdateSoft()
{
}

void CUpdateSoft::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CUpdateSoft, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CUpdateSoft 消息处理程序


void CUpdateSoft::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bClose)
		CDialogEx::OnClose();
}
void UpDateSoftThread(void* p)
{
	CUpdateSoft* dlgUp=(CUpdateSoft*)(p);
	CString ret,strTemp,strTemp1;
	CWebRetData webRet;
	if(CCarKey3App::withServer.GetSoftPocket(CCarKey3App::TheHIDDevice.HexID,0,webRet))
	{
		int FileSize=0;
		if(webRet.retStatus==web_OK)
		{
			FileSize=_ttoi(webRet.strResult);
			byte* pFile=new byte[FileSize];
			dlgUp->m_ProgressCtrl.SetRange(0,100);
			dlgUp->m_ProgressCtrl.SetPos(0);
			dlgUp->m_ProgressCtrl.UpdateWindow();
			int index=1;
			int retry=0;
			int dataIndex=0;
			while(true)
			{
				if(CCarKey3App::withServer.GetSoftPocket(CCarKey3App::TheHIDDevice.HexID,index,webRet))
				{
					if(webRet.retStatus==web_OK)
					{
						int len;
						byte* rx=CCommFunc::strToHexByte(webRet.strResult,_T(""),&len);
						for(int i=0;i<len;i++)
						{
							pFile[dataIndex++]=rx[i];
						}
						dlgUp->m_ProgressCtrl.SetPos(dataIndex*100/FileSize);
						dlgUp->m_ProgressCtrl.UpdateWindow();
						if(dataIndex>=FileSize)
						{
							dlgUp->m_bOK=true;
							CFile file(_T("Mercedes Tool.exe.up"),CFile::modeCreate|CFile::modeWrite);
							file.Write(pFile,FileSize);
							file.Close();
							strTemp.LoadString(NULL,IDS_ServerTip23,CCarKey3App::LANGID);
							MessageBoxEx(dlgUp->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
							dlgUp->m_bClose=true;
							PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
							return;
						}
						index++;
					}					
					else
					{
						strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						strTemp1.Format(strTemp,webRet.retStatus);
						MessageBoxEx(dlgUp->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						dlgUp->m_bClose=true;
						PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
						return;
					}
				}
			}
		}
		else
		{
			strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
			strTemp1.Format(strTemp,webRet.retStatus);
			MessageBoxEx(dlgUp->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
			dlgUp->m_bClose=true;
			PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
			return;
		}
		dlgUp->m_bClose=true;
		PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
	}
	else
	{
		strTemp.LoadString(NULL,IDS_ServerTip22,CCarKey3App::LANGID);
		MessageBoxEx(dlgUp->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
		dlgUp->m_bClose=true;
		PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
	}
}
void UpDateHexThread(void* p)
{
	CUpdateSoft* dlgUp=(CUpdateSoft*)(p);
	CString ret,strTemp,strTemp1;
	CWebRetData webRet;
	int FileSize=0;
	byte* pFile;
	if(CCarKey3App::withServer.GetHexPocket(CCarKey3App::TheHIDDevice.HexID,0,webRet))
	{
		if(webRet.retStatus==web_OK)
		{
			FileSize=_ttoi(webRet.strResult);
			pFile=new byte[FileSize];
			dlgUp->m_ProgressCtrl.SetRange(0,100);
			dlgUp->m_ProgressCtrl.SetPos(0);
			dlgUp->m_ProgressCtrl.UpdateWindow();			
		}
		else
		{
			strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
			strTemp1.Format(strTemp,webRet.retStatus);
			MessageBoxEx(dlgUp->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
			dlgUp->m_bClose=true;
			PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
			return;
		}		
	}
	else
	{
		strTemp.LoadString(NULL,IDS_ServerTip22,CCarKey3App::LANGID);
		MessageBoxEx(dlgUp->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
		dlgUp->m_bClose=true;
		PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
	}
	int index=1;
	int retry=0;
	int dataIndex=0;
	while(true)
	{
		if(CCarKey3App::withServer.GetHexPocket(CCarKey3App::TheHIDDevice.HexID,index,webRet))
		{
			if(webRet.retStatus==web_OK)
			{
				int len;
				byte* rx=CCommFunc::strToHexByte(webRet.strResult,_T(""),&len);
				for(int i=0;i<len;i++)
				{
					pFile[dataIndex++]=rx[i];
				}
				dlgUp->m_ProgressCtrl.SetPos(dataIndex*100/FileSize);
				dlgUp->m_ProgressCtrl.UpdateWindow();
				if(dataIndex>=FileSize)
				{
					dlgUp->m_bOK=true;
					// 							CString strID=CCommFunc::byteToHexStr(CCarKey3App::TheHIDDevice.HexID,4,_T(""));
					// 							CFile file(strID+_T(".newbin"),CFile::modeCreate|CFile::modeWrite);
					// 							file.Write(pFile,FileSize);
					// 							file.Close();
					//开始硬件升级
					CInteractionData Result;
					CCarKey3App::TheHIDDevice.UpdateDevice(pFile,&Result,&dlgUp->m_ProgressCtrl);
					if(Result.GetHIDResult()==_T(""))
					{						
					 	MessageBeep(MB_ICONEXCLAMATION);
						strTemp.LoadString(NULL,IDS_ServerTip30,CCarKey3App::LANGID);
						MessageBoxEx(dlgUp->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
// 					 	strTemp.LoadString(IDS_FinishEZSRenew);
// 					 	MessageBox(strTemp);
					}
					else
					{
// 						MessageBox(Result.GetHIDResult());
// 					 	ShowLog(Result.GetHIDResult());
						//strTemp.LoadString(NULL,IDS_FinishEZSRenew,CCarKey3App::LANGID);
						MessageBoxEx(dlgUp->m_hWnd,Result.GetHIDResult(),_T(""),MB_OK,CCarKey3App::LANGID);
					}
					dlgUp->m_bClose=true;
					PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
					// 							strTemp.LoadString(NULL,IDS_ServerTip23,CCarKey3App::LANGID);
					// 							MessageBoxEx(dlgUp->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);							
					return;
				}
				index++;
			}
			else
			{
				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				strTemp1.Format(strTemp,webRet.retStatus);
				MessageBoxEx(dlgUp->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				dlgUp->m_bClose=true;
				PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
				return;
			}
		}
		else
		{
			strTemp.LoadString(NULL,IDS_ServerTip22,CCarKey3App::LANGID);
			MessageBoxEx(dlgUp->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
			dlgUp->m_bClose=true;
			PostMessage(dlgUp->m_hWnd,WM_CLOSE,0,0);
		}
	}
}
BOOL CUpdateSoft::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(m_nUpType==0)
		_beginthread(UpDateSoftThread,0,(void *)(this));
	else
	{
		_beginthread(UpDateHexThread,0,(void *)(this));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
