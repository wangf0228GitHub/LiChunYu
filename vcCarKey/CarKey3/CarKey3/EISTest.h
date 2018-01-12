#pragma once


// CEISTest 对话框
#define WM_MY_MESSAGE WM_USER+100
class CEISTest : public CDialogEx
{
	DECLARE_DYNAMIC(CEISTest)

public:
	CEISTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEISTest();

// 对话框数据
	enum { IDD = IDD_DLG_TestEIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT MyMessage(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bKey2;
	BOOL m_bH02;
	BOOL m_bACC2;
	BOOL m_bFire2;
	BOOL m_bStart2;
	BOOL m_bKey1;
	BOOL m_bH01;
	BOOL m_bACC1;
	BOOL m_bFire1;
	BOOL m_bStart1;
	bool m_bRunning;
	bool m_bThreadExit;
	int m_nMode;
	afx_msg void OnBnEISTest();
};
