// WfEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "WfEdit.h"


// CWfEdit

IMPLEMENT_DYNAMIC(CWfEdit, CEdit)

CWfEdit::CWfEdit()
{
	m_bLoad=false;
	//m_sysColor=GetSysColor(COLOR_WINDOW);
	m_sysColor=RGB(255,255,255);
	m_bkBrush.CreateSolidBrush(RGB(0,255,0));      // 红色背景色
	//m_sysBrush.CreateSolidBrush(m_sysColor);
	m_sysBrush.CreateSolidBrush(RGB(255,255,255));
	m_bIsHex=true;
	m_bSysColor=true;
}

CWfEdit::~CWfEdit()
{
	m_bkBrush.DeleteObject();
	m_sysBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CWfEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CWfEdit 消息处理程序


HBRUSH CWfEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  在此更改 DC 的任何特性
	if(m_bLoad)
	{
		CString str;
		this->GetWindowText(str);
		if(str==m_strContent)
		{
			if(m_bSysColor)
			{
				pDC->SetBkColor(m_sysColor);
				return m_sysBrush;
			}
			return NULL;
		}
		else
		{
			pDC->SetBkColor(RGB(0,255,0));
			return m_bkBrush;
		}
	}
	else
	{
		if(m_bSysColor)
		{
			pDC->SetBkColor(m_sysColor);
			return m_sysBrush;
		}
		return NULL;
	}
	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
	return NULL;
}


void CWfEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bIsHex)
	{
		if ( (nChar >= '0' && nChar <= '9') || 
			(nChar >= 'a' && nChar <= 'f') || 
			(nChar >= 'A' && nChar <= 'F') || 
			nChar == VK_BACK || 
			nChar == VK_DELETE || nChar==' ') //msdn的virtual key
		{ 
			CEdit::OnChar(nChar, nRepCnt, nFlags); 
		} 
	}
	else
		CEdit::OnChar(nChar, nRepCnt, nFlags); 
}

void CWfEdit::SetContent(CString str)
{
	m_strContent=str;
	m_bLoad=true;
	SetWindowText(m_strContent);
}

void CWfEdit::Clear()
{
	m_bLoad=false;
	SetContent(_T(""));
}
