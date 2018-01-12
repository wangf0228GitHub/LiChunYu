#pragma once


// CCANInfo 对话框

class CCANInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CCANInfo)

public:
	CCANInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCANInfo();

// 对话框数据
	enum { IDD = IDD_Dlg_CANInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	byte* pBuf;
	afx_msg void OnBnClickedButton1();
};
