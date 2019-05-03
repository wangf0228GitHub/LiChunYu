#pragma once


// CWFMaskedEidt

class CWFMaskedEidt : public CMFCMaskedEdit
{
	DECLARE_DYNAMIC(CWFMaskedEidt)

public:
	CWFMaskedEidt();
	CWFMaskedEidt::CWFMaskedEidt(bool bhex);
	virtual ~CWFMaskedEidt();
	bool bHex;
private:
	CBrush m_bkBrush;
	CBrush m_sysBrush;
	COLORREF m_sysColor;
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool m_bLoad;
	CString m_strContent;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


