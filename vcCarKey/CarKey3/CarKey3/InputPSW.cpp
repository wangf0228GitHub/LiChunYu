// InputPSW.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "InputPSW.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"


// CInputPSW 对话框

IMPLEMENT_DYNAMIC(CInputPSW, CDialogEx)

CInputPSW::CInputPSW(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputPSW::IDD, pParent)
{
	bPSW=false;
}

CInputPSW::~CInputPSW()
{
}

void CInputPSW::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_PSW);
}


BEGIN_MESSAGE_MAP(CInputPSW, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CInputPSW::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CInputPSW::OnBnClickedButton1)
END_MESSAGE_MAP()


// CInputPSW 消息处理程序
BOOL CInputPSW::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	CString strTemp;	
	strTemp.LoadString(IDS_STRING370);
	SetWindowText(strTemp);

	strTemp.LoadString(IDS_STRING371);
	SetDlgItemText(IDC_BUTTON1,strTemp); 

	strTemp.LoadString(IDS_STRING372);
	SetDlgItemText(IDC_BUTTON2,strTemp); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CInputPSW::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	bPSW=false;
	PostMessage(WM_SYSCOMMAND,SC_CLOSE);
}


void CInputPSW::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	bool bOK=false;
	m_PSW.GetWindowText(str);
	int len;
	byte* p=CCommFunc::strToHexByte(str,_T(" "),&len);
	if(p==NULL)
		bOK=false;
	else if(len!=8)
		bOK=false;
	else
		bOK=true;
	if(bOK)
	{
		for(int i=0;i<8;i++)
			PSW[i]=p[i];
		bPSW=true;
		PostMessage(WM_SYSCOMMAND,SC_CLOSE);
	}
	else
	{
		CString strTemp;
		strTemp.LoadString(IDS_ServerTip21);
		MessageBox(strTemp);
	}
}
