#pragma once
#include "afxwin.h"


// CECUDlg 对话框

class CECUDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CECUDlg)

public:
	CECUDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CECUDlg();

// 对话框数据
	enum { IDD = IDD_ECU_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_bkBrush;
};
