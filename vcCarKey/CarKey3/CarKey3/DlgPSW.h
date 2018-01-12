#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgPSW 对话框

class CDlgPSW : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPSW)

public:
	CDlgPSW(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPSW();

// 对话框数据
	enum { IDD = IDD_DLG_PSW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnReadData();
	BOOL m_bQuanDiu;
	virtual BOOL OnInitDialog();
	CComboBox m_ModelType;
	CProgressCtrl m_Progress;
	int m_nMode;
	byte IRdt[0x40*0x400];
	CStatusBarCtrl m_StatusBar;
	void ShowLog(CString str);
	LRESULT OnHIDMsgBack(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCbnSelchangeModel();	
	bool m_bRunning;
	bool m_bThreadExit;
	afx_msg void OnClose();
	afx_msg void OnBnHPassWork();
	CString m_strFilePath;
	CString m_strFileName;
	int m_nQuanDiuType;
};
