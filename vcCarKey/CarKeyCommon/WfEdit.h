#pragma once


// CWfEdit

class CWfEdit : public CEdit
{
	DECLARE_DYNAMIC(CWfEdit)

public:
	CWfEdit();
	virtual ~CWfEdit();
	void SetContent(CString str);
	bool bChange;
	bool m_bIsHex;
	bool m_bSysColor;
	void Clear();
private:
	CBrush m_bkBrush;
	CBrush m_sysBrush;
	COLORREF m_sysColor;
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool m_bLoad;
	CString m_strContent;
	
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


