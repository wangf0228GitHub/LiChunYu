#pragma once


// CELVDlg 对话框

class CELVDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CELVDlg)

public:
	CELVDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CELVDlg();
	CBrush m_bkBrush;

// 对话框数据
	enum { IDD = IDD_ELV_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
