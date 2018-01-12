// DlgShowMessage.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "DlgShowMessage.h"
#include "afxdialogex.h"


// CDlgShowMessage 对话框

IMPLEMENT_DYNAMIC(CDlgShowMessage, CDialogEx)

CDlgShowMessage::CDlgShowMessage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgShowMessage::IDD, pParent)
{

}

CDlgShowMessage::~CDlgShowMessage()
{
}

void CDlgShowMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Message);
}


BEGIN_MESSAGE_MAP(CDlgShowMessage, CDialogEx)
END_MESSAGE_MAP()


// CDlgShowMessage 消息处理程序


BOOL CDlgShowMessage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Message.SetWindowText(m_strMessage);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
