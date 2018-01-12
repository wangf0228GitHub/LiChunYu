#pragma once
#include "afxwin.h"


// CDlgShowMessage 对话框

class CDlgShowMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowMessage)

public:
	CDlgShowMessage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShowMessage();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Message;
	CString m_strMessage;
	virtual BOOL OnInitDialog();
};
