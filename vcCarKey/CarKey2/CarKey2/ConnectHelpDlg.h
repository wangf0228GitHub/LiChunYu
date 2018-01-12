#pragma once
#include "afxwin.h"


// CConnectHelpDlg 对话框

class CConnectHelpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectHelpDlg)

public:
	CConnectHelpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectHelpDlg();

// 对话框数据
	enum { IDD = IDD_ConnectHelpDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_bkBrush;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CStatic m_Pic;
	CButton m_DCL;
};
