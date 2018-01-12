#pragma once
#include "..\..\CarKeyCommon\WfEdit.h"
#include "CarKey3Dlg.h"
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

	int m_nMode;
	CWfEdit m_rKeyID;
	CWfEdit m_rKeyNum;
	CWfEdit m_rLife;
	CWfEdit m_rLifeHex;
	CWfEdit m_rStatus;
	CWfEdit m_rVersion;
	CWfEdit m_rMark;
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
	afx_msg void OnBnReadPSW();
	CButton m_btReadPSW;
	CButton m_btSavePSW;
	bool m_bGetPSW;
	byte m_PSWBuf[0x18];
	afx_msg void OnBnSavePSW();
	void KeyMessageDlg(CString str);
	void ShowLog(CString str);
	void OnBnRenew();
	CCarKey3Dlg* m_pMainDlg;

	CStatusBarCtrl m_StatusBar;

	CButton m_raIR;
	CButton m_raOnPCB;
	afx_msg void OnEnChangeEdit12();
	CComboBox m_ModelType;
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedButton1();
};
