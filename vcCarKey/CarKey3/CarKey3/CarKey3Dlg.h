
// CarKey3Dlg.h : 头文件
//

#pragma once
#include "afxext.h"


// CCarKey3Dlg 对话框
class CCarKey3Dlg : public CDialogEx
{
// 构造
public:
	CCarKey3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CARKEY3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void HIDUI();
	void ReHIDUI();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	CString m_strPSW;
	CStatusBarCtrl m_StatusBar;
	HANDLE m_hUsbEventHandle;
	CBitmapButton m_btnMKEY;
	CBitmapButton m_btnEIS;
	CBitmapButton m_btnESL;
	CBitmapButton m_btnNEC_CHIP;
	CBitmapButton m_btnNEC_KEYLESS;
	CBitmapButton m_btnNEC_ESL;
	bool m_bShow;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnPSW();
	afx_msg void OnLanguage32775();
	afx_msg void OnLanguageEnglish();
	void LoadString();
	afx_msg void OnBnEIS();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnKey();
	afx_msg void OnBnESL();
	
	afx_msg void OnBnECU();
	afx_msg void OnBnIG();
};