#pragma once
#include "afxcmn.h"


// CDlgKeyData 对话框

class CDlgKeyData : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgKeyData)

public:
	CDlgKeyData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgKeyData();

// 对话框数据
	enum { IDD = IDD_DLG_KeyData };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bAllKey;
	BOOL m_bKey0;
	BOOL m_bKey1;
	BOOL m_bKey2;
	BOOL m_bKey3;
	BOOL m_bKey4;
	BOOL m_bKey5;
	BOOL m_bKey6;
	BOOL m_bKey7;
	BOOL m_bAllFormat;
	BOOL m_b11;
	BOOL m_b41;
	BOOL m_b51;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck17();
	virtual BOOL OnInitDialog();
	byte pBIN[0x400+7];
	CProgressCtrl m_Progress;
	CStatusBarCtrl m_StatusBar;
	bool m_bRunning;
	bool m_bThreadExit;
	void ShowLog(CString str);
	afx_msg void OnBnClickedButton1();
	void EnableControl( BOOL bEn );
};
