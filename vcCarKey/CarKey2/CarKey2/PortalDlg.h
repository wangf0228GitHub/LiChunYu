#pragma once
#include "DumpCalcDlg.h"
#include "afxcmn.h"
#include "afxwin.h"
#include ".\..\..\CarKeyCommon\EZSData.h"


class CPortalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPortalDlg)

public:
	CPortalDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPortalDlg();
	CBrush m_bkBrush;
// 对话框数据
	enum { IDD = IDD_Portal_DIALOG };
	CDumpCalcDlg* m_pDumpCalcDlg;
	CPortalOP* m_pPortalOP;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnReadData();
	CProgressCtrl m_Progress;
	void ShowLog(CString str);
	CEdit m_Log;
	CString m_strLog;
	afx_msg void OnBnReadCANInfo();
	afx_msg void OnBnOpenFile();
	virtual BOOL OnInitDialog();
	void SetCANShow(byte* outBuf);
	afx_msg void OnBnCANWrite();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//CEZSData m_EZSData;
	CButton m_raKLine;
	CButton m_raIR;
	CButton m_raCAN;
	CButton m_raOBD;
	CButton m_raDirect;
	CButton m_raWork1;
	CButton m_raWork2;
	CButton m_raWork3;
	CStatic m_CANConnect;
	afx_msg void OnBnIRRadio();
	afx_msg void OnBnCANRadio();
	void CANRead();
	void IRRead();
	CButton m_btReadInfo;
	CButton m_btCANWrite;
	afx_msg void OnBnSaveData();
	bool CPortalDlg::CheckData();
	afx_msg void OnBnLoadPSW();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	CString m_strModelName;
	CButton m_btELV;
};
