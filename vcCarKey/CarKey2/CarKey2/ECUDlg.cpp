// ECUDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "ECUDlg.h"
#include "afxdialogex.h"


// CECUDlg 对话框

IMPLEMENT_DYNAMIC(CECUDlg, CDialogEx)

CECUDlg::CECUDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CECUDlg::IDD, pParent)
{
	m_bkBrush.CreateSolidBrush(RGB(166,202,240));
}

CECUDlg::~CECUDlg()
{
	m_bkBrush.DeleteObject();
}

void CECUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CECUDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CECUDlg 消息处理程序


HBRUSH CECUDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
			pDC->SetBkColor(RGB(166,202,240));
			pDC->SetBkMode(TRANSPARENT);      //不显示文字背景
			hbr = (HBRUSH)::GetStockObject(NULL_BRUSH); //编辑框背景。注意：和文字背景不是一个意思。
			break;
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
