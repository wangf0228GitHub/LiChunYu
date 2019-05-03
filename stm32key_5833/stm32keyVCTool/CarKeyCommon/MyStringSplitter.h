#pragma once
/*
使用方法：
CSplitStr Split;
Split.SetSplitFlag(",");//设定分隔符，默认值也是“，”
Split.SetSequenceAsOne(TRUE);//设定联系的分割符如何处理
Split.SetData(m_sText);//读取要分割的字符串
CStringArray array;//读取结果存放的数组
Split.GetSplitStrArray(array);
for (int i=0; i< array.GetSize(); i++)
{
m_List.AddString(array.GetAt(i));

}

*/
class CMySplitStr  
{
private:
	//连续的切分的标志符号当成一个标志处理
	BOOL    m_bSequenceAsOne;
	//被切分的文本
	CString m_sData;
	//切分的标志符号
	CString m_sSplitFlag;
public:
	//得到切分好的文本串
	void GetSplitStrArray(CStringArray &array);
	//得到被切分的文本
	CString GetData();
	//设置被切分的文本
	void SetData(CString sData,bool bTrim=false);
	//得到切分参数
	BOOL GetSequenceAsOne() {return m_bSequenceAsOne;};
	//设置切分参数,连续的分隔符是否按一个处理，true为按一个处理
	void SetSequenceAsOne(BOOL bSequenceAsOne) {m_bSequenceAsOne = bSequenceAsOne;};
	//得到切分标志
	CString GetSplitFlag() {return m_sSplitFlag;};
	//设置切分标志
	void SetSplitFlag(CString sSplitFlag) {m_sSplitFlag = sSplitFlag;};

	CMySplitStr();
	virtual ~CMySplitStr();

};
