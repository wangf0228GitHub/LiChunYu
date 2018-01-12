
#include "stdafx.h"
#include "MyStringSplitter.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySplitStr::CMySplitStr()
{
	SetData(_T(""));
	SetSequenceAsOne(FALSE);
	SetSplitFlag(_T(","));

}

CMySplitStr::~CMySplitStr()
{

}

void CMySplitStr::SetData(CString sData,bool bTrim)
{
	m_sData = sData;
	if(bTrim)
	{
		m_sData.TrimLeft();
		m_sData.TrimRight();
	}	
}

CString CMySplitStr::GetData()
{
	return m_sData;
}

void CMySplitStr::GetSplitStrArray(CStringArray &array)
{
	CString sData = GetData();
	CString sSplitFlag = GetSplitFlag();
	/*if (sData.Right(1) != sSplitFlag)*/ sData +=sSplitFlag;

	CString sTemp;
	int pos =-1;
	while ((pos=sData.Find(sSplitFlag,0)) != -1)
	{
		sTemp = sData.Left(pos);
		if (!GetSequenceAsOne()) 
		{
			array.Add(sTemp);
		}
		else
		{
			//if (!sTemp.IsEmpty() && sTemp !="") ////连续的分隔符视为单个处理
			//{
				array.Add(sTemp);
			//}
		}
		sData = sData.Right(sData.GetLength() - pos - 1);
	}
}
