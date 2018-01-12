#pragma once
#include ".\..\..\CarKeyCommon\WfEdit.h"
#include ".\..\..\CarKeyCommon\xcolorstatic.h"
#include ".\..\..\CarKeyCommon\EZSData.h"


// CDumpCalcDlg 对话框

class CDumpCalcDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDumpCalcDlg)

public:
	CDumpCalcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDumpCalcDlg();

// 对话框数据
	enum { IDD = IDD_DumpCalc_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWfEdit m_Key0;
	afx_msg void OnBnOpenFile();
	afx_msg void OnBnPSWIn();
	CXColorStatic m_Key0Status;
	CXColorStatic m_Key1Status;
	CXColorStatic m_Key2Status;
	CXColorStatic m_Key3Status;
	CXColorStatic m_Key4Status;
	CXColorStatic m_Key5Status;
	CXColorStatic m_Key6Status;
	CXColorStatic m_Key7Status;
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
	virtual BOOL OnInitDialog();
	void ClearCtrl();
	CWfEdit m_Key1;
	CWfEdit m_Key2;
	CWfEdit m_Key3;
	CWfEdit m_Key4;
	CWfEdit m_Key5;
	CWfEdit m_Key6;
	CWfEdit m_Key7;
	CEZSData m_EZSData;
	afx_msg void OnBnSaveData();
	afx_msg void OnBnExportTXT();
	CBrush m_bkBrush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEdit9();
	CString GetDataLog();
};
