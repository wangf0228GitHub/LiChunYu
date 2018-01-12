#pragma once


// CISM7GDlg 对话框

class CISM7GDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CISM7GDlg)

public:
	CISM7GDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CISM7GDlg();
	CBrush m_bkBrush;
// 对话框数据
	enum { IDD = IDD_ISM7G_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
