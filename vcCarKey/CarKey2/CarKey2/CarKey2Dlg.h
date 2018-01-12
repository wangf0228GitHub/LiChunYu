
// CarKey2Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCarKey2Dlg dialog
class CCarKey2Dlg : public CDialogEx
{
// Construction
public:
	CCarKey2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CARKEY2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	HANDLE m_hUsbEventHandle;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void HIDUI();
	void ReHIDUI();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
	CStatic m_Pic;
	afx_msg void OnLbnDblclkList1();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CStatusBarCtrl m_StatusBar;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strPSW;
	afx_msg void OnLanguage32775();
	afx_msg void OnLanguageEnglish();
	void LoadString();
};
