#pragma once
#include "CarKey3Dlg.h"
#include "afxwin.h"
#include "..\..\CarKeyCommon\XColorStatic.h"
#include "..\..\CarKeyCommon\WfEdit.h"
#include "afxcmn.h"


// CDlgESL 对话框

class CDlgESL : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgESL)

public:
	CDlgESL(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgESL();
	CCarKey3Dlg* m_pMainDlg;
	CToolBar m_ToolBar;
	CImageList m_ImageList;
	CStatusBarCtrl m_StatusBar;

// 对话框数据
	enum { IDD = IDD_DLG_ESL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnReadInfo();
	void ShowLog(CString str);
	CComboBox m_ModelType;
	BOOL m_bInitialised;
	BOOL m_bTPremoved;
	BOOL m_bPersonalised;
	BOOL m_bActivated;
	CXColorStatic m_Locked;
	CXColorStatic m_Key0Status;
	CXColorStatic m_Key1Status;
	CXColorStatic m_Key2Status;
	CXColorStatic m_Key3Status;
	CXColorStatic m_Key4Status;
	CXColorStatic m_Key5Status;
	CXColorStatic m_Key6Status;
	CXColorStatic m_Key7Status;
	CEZSData m_EZSData;
	byte SSID[4];

	CWfEdit m_Dealer;
	CWfEdit m_VIN;
	CWfEdit m_Key0;
	CWfEdit m_Key1;
	CWfEdit m_Key2;
	CWfEdit m_Key3;
	CWfEdit m_Key4;
	CWfEdit m_Key5;
	CWfEdit m_Key6;
	CWfEdit m_Key7;
	CWfEdit m_MBLJH;
	CWfEdit m_Version;

	afx_msg void OnBnReadData();
	int GetKey(int index);
	byte ESLData[0x200];
	void ShowData();
	bool bIsSame(byte* p1,byte* p2);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	bool CheckData();
	afx_msg void OnBnWriteESL();
	bool m_bLoadData;
	afx_msg void OnBnRenewESL();
	afx_msg void OnEnDealerEdit();
	void EnableControl(BOOL bEn);
	bool m_bReaded;
	CProgressCtrl m_Progress;
	byte ESLPass[8];
	bool m_bRunning;
	bool m_bThreadExit;
};
