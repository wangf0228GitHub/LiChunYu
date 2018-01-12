#pragma once
#include "afxcmn.h"


// CUpdateSoft 对话框

class CUpdateSoft : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateSoft)

public:
	CUpdateSoft(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpdateSoft();

// 对话框数据
	enum { IDD = IDD_DLG_UpdateSoft };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	int m_nUpType;
	bool m_bOK;
	bool m_bClose;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_ProgressCtrl;
};
