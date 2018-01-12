// ConnectHelpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "ConnectHelpDlg.h"
#include "afxdialogex.h"


// CConnectHelpDlg 对话框

IMPLEMENT_DYNAMIC(CConnectHelpDlg, CDialogEx)

CConnectHelpDlg::CConnectHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConnectHelpDlg::IDD, pParent)
{

}

CConnectHelpDlg::~CConnectHelpDlg()
{
	m_bkBrush.DeleteObject();
}

void CConnectHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_Pic);
	DDX_Control(pDX, IDC_RADIO1, m_DCL);
}


BEGIN_MESSAGE_MAP(CConnectHelpDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, &CConnectHelpDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CConnectHelpDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CConnectHelpDlg 消息处理程序


HBRUSH CConnectHelpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(nCtlColor ==CTLCOLOR_DLG)
		return m_bkBrush; //返加绿色刷子
	if (nCtlColor = CTLCOLOR_STATIC)
	{
		switch(pWnd->GetDlgCtrlID()) 
		{		
		case IDC_RADIO1:
		case IDC_RADIO2:		
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


BOOL CConnectHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bkBrush.CreateSolidBrush(RGB(166,202,240));
	m_DCL.SetCheck(true);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CConnectHelpDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   
	bitmap.LoadBitmap(IDB_BITMAP5);
	// 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_Pic.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  
}


void CConnectHelpDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   
	bitmap.LoadBitmap(IDB_BITMAP16);
	// 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_Pic.SetBitmap(hBmp); 
}
