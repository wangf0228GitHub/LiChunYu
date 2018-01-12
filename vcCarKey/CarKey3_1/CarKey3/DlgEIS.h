#pragma once
#include "CarKey3Dlg.h"
#include "afxwin.h"
#include "..\..\CarKeyCommon\EZSData.h"
#include "..\..\CarKeyCommon\WfEdit.h"
#include "..\..\CarKeyCommon\XColorStatic.h"


// CDlgEIS 对话框
class CDlgEIS : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEIS)

public:
	CDlgEIS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEIS();

// 对话框数据
	enum { IDD = IDD_DLG_EIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CCarKey3Dlg* m_pMainDlg;
	HANDLE m_hUsbEventHandle;
	CStatusBarCtrl m_StatusBar;
	int m_nMode;
	CComboBox m_ModelType;
	CComboBox m_Portal;
	CComboBox m_CANformat;
	CComboBox m_WorkArea;
	CPortalOP m_PortalOP;
	CToolBar m_ToolBar;
	void setOption();
	afx_msg void OnSelchangeModel();

	CWfEdit m_SSID;
	CWfEdit m_Odo;
	CWfEdit m_PSW;
	CWfEdit m_SerialNum;
	CWfEdit m_Transport;
	CWfEdit m_Dealer;
	CWfEdit m_VIN;

	BOOL m_bInitialised;
	BOOL m_bTPremoved;
	BOOL m_bPersonalised;
	BOOL m_bActivated;
	CWfEdit m_LastKey;
	CWfEdit m_PreviousKey;

	CWfEdit m_Key0;
	CWfEdit m_Key1;
	CWfEdit m_Key2;
	CWfEdit m_Key3;
	CWfEdit m_Key4;
	CWfEdit m_Key5;
	CWfEdit m_Key6;
	CWfEdit m_Key7;

	bool m_bReaded;
	bool m_bCANRead;
	CXColorStatic m_Key0Status;
	CXColorStatic m_Key1Status;
	CXColorStatic m_Key2Status;
	CXColorStatic m_Key3Status;
	CXColorStatic m_Key4Status;
	CXColorStatic m_Key5Status;
	CXColorStatic m_Key6Status;
	CXColorStatic m_Key7Status;
	CProgressCtrl m_Progress;
	CEZSData m_EZSData;
	afx_msg void OnBnReadData();
	void IRRead();
	void CANRead();
	void ShowLog( CString strTemp );
	void ClearCtrl();
	void SetShowData();
	void EnableBotton(BOOL bEn);
	bool m_bNewModle;
	afx_msg void OnBnReadInfo();
	afx_msg void OnCbnSelchangePortal();
	afx_msg void OnBnCANWrite();
	bool CheckData();
	afx_msg void OnBnLoadPSW();
	afx_msg void OnBnSaveSKC();
	afx_msg void OnBnEISTest();
	afx_msg void OnDealerEdit();
	afx_msg void OnBnReadMillsClicked();
	afx_msg void OnBnEZSRenewClicked();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnCanedit();
	afx_msg void OnDisedit();
	afx_msg void OnBnWriteVIN();
// 	bool m_bRunning;
// 	bool m_bThreadExit;
	afx_msg void OnClose();
};
