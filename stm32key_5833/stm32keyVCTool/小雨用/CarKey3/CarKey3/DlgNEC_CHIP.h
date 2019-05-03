#pragma once
#include "..\..\CarKeyCommon\WfEdit.h"
#include "afxwin.h"


// CDlgNEC_CHIP 对话框

class CDlgNEC_CHIP : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNEC_CHIP)

public:
	CDlgNEC_CHIP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNEC_CHIP();
	CToolBar m_ToolBar;
	CImageList m_ImageList;
// 对话框数据
	enum { IDD = IDD_DLG_NEC_CHIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonBin();
	
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	HANDLE m_hUsbEventHandle;
	afx_msg void OnClose();
	afx_msg void OnBnReadKeyData();
	CProgressCtrl m_Progress;
	CWfEdit m_wPWD;
	CWfEdit m_wKeyNum;
	CWfEdit m_wKeyID;
	CBrush m_bkBrush;
	afx_msg void OnBnWrite();
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	bool m_bGetPSW;
	byte m_PSWBuf[0x18];
	void KeyMessageDlg(CString str);
	void ShowLog(CString str);
	void OnBnRenew();	

	CStatusBarCtrl m_StatusBar;


	afx_msg void OnEnChangeEdit12();

	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedButton1();
	void HIDUI();
	void ReHIDUI();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_strLog;
	CComboBox m_nRenewTimes;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
