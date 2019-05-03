#pragma once


// CDlgISM 对话框

class CDlgISM : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgISM)

public:
	CDlgISM(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgISM();

// 对话框数据
	enum { IDD = IDD_DLG_ESL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
