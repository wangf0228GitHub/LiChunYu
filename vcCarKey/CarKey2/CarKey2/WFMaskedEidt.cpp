// WFMaskedEidt.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "WFMaskedEidt.h"


// CWFMaskedEidt

IMPLEMENT_DYNAMIC(CWFMaskedEidt, CMFCMaskedEdit)

CWFMaskedEidt::CWFMaskedEidt()
{
	bHex=false;
	m_bLoad=false;
	//m_sysColor=GetSysColor(COLOR_WINDOW);
	m_sysColor=GetSysColor(COLOR_HIGHLIGHT);
	m_bkBrush.CreateSolidBrush(RGB(0,255,0));      // 红色背景色
	m_sysBrush.CreateSolidBrush(m_sysColor);
}

CWFMaskedEidt::CWFMaskedEidt(bool bhex)
{
	bHex=bhex;
}
CWFMaskedEidt::~CWFMaskedEidt()
{
}


BEGIN_MESSAGE_MAP(CWFMaskedEidt, CMFCMaskedEdit)
	ON_WM_CHAR()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CWFMaskedEidt 消息处理程序




void CWFMaskedEidt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	if(bHex)
// 	{
// 		if((nChar>='0' && nChar <='9') || (nChar>='a' && nChar<='f') || (nChar>='A' && nChar<='F'))
// 		{
// 			CMFCMaskedEdit::OnChar(nChar, nRepCnt, nFlags);
// 		}
// 		else
// 			return;
// 	}
	CMFCMaskedEdit::OnChar(nChar, nRepCnt, nFlags);
}


HBRUSH CWFMaskedEidt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CMFCMaskedEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	if(m_bLoad)
	{
		CString str;
		this->GetWindowText(str);
		if(str==m_strContent)
		{
			pDC->SetBkColor(m_sysColor);
			return m_sysBrush;
		}
		else
		{
			pDC->SetBkColor(RGB(0,255,0));
			return m_bkBrush;
		}
	}
	else
	{
		pDC->SetBkColor(m_sysColor);
		return m_sysBrush;
	}
	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
