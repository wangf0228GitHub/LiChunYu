#pragma once
#include "..\..\CarKeyCommon\XColorStatic.h"
#include "..\..\CarKeyCommon\WfEdit.h"
#include "CarKey3Dlg.h"


// CDlgECU 对话框

class CDlgECU : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgECU)

public:
	CDlgECU(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgECU();
	CCarKey3Dlg* m_pMainDlg;
	CStatusBarCtrl m_StatusBar;
// 对话框数据
	enum { IDD = IDD_DLG_ECU };

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
	CXColorStatic m_Key0Status;
	CXColorStatic m_Key1Status;
	CXColorStatic m_Key2Status;
	CXColorStatic m_Key3Status;
	CXColorStatic m_Key4Status;
	CXColorStatic m_Key5Status;
	CXColorStatic m_Key6Status;
	CXColorStatic m_Key7Status;
	byte DPSW[8];
	byte SSID[4];

	CWfEdit m_Dealer;
	CWfEdit m_SerialNum;
	CWfEdit m_Transport;
	CWfEdit m_SVer;
	CWfEdit m_Key0;
	CWfEdit m_Key1;
	CWfEdit m_Key2;
	CWfEdit m_Key3;
	CWfEdit m_Key4;
	CWfEdit m_Key5;
	CWfEdit m_Key6;
	CWfEdit m_Key7;
	CWfEdit m_MBLJH;
	CWfEdit m_HVer;

	afx_msg void OnBnReadData();
	int GetKey(int index);
	byte ECUData[0x200];
	void ShowData();
	bool bIsSame(byte* p1,byte* p2);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	bool CheckData();
	afx_msg void OnBnECUGetPass();
	bool m_bLoadData;
	afx_msg void OnBnRenew();
	afx_msg void OnEnDealerEdit();
	void EnableControl(BOOL bEn);
	bool m_bReaded;
	CProgressCtrl m_Progress;
	bool fEcuMd;
};
