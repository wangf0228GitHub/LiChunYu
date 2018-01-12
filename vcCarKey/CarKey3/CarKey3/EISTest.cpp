// EISTest.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey3.h"
#include "EISTest.h"
#include "afxdialogex.h"
#include "..\..\CarKeyCommon\CommFunc.h"
void EIS0(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x03]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x04]==0x07)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x2d || pBuf[0x03]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x1d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x03)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x07)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x0f)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x1f)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x3f)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS1(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x05]==0x05)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x2d || pBuf[0x04]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x1d || pBuf[0x04]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x05]==0x00)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x01)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x05)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x0d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x1d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x3d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS2(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x05]==0x05)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x2d || pBuf[0x04]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x1d || pBuf[0x04]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x05]==0x02)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x01)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x05)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x0d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x1d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x3d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS3(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x03]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x04]==0x07)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x2d || pBuf[0x03]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x03]==0x1d || pBuf[0x03]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x03)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x07)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x0f)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x1f)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x04]==0x3f)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS4(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x05]==0x05)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x2d || pBuf[0x04]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x1d || pBuf[0x04]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x05]==0x00)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x03)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x05)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x0b)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x1b)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x3b)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS5(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x05]==0x05)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x2d || pBuf[0x04]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x1d || pBuf[0x04]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x05]==0x02)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x01)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x05)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x0d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x1d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x3d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS6(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x05]==0x05)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x2d || pBuf[0x04]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x1d || pBuf[0x04]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x05]==0x00)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x03)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x05)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x0b)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x1b)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x3b)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EIS7(byte* pBuf,CEISTest* EISTest)
{
	//启动授权
	if(pBuf[0x04]==0x00)
	{
		EISTest->m_bKey2=FALSE;
		EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x01)
	{
		EISTest->m_bKey2=TRUE;
		if(pBuf[0x05]==0x05)
			EISTest->m_bH02=TRUE;
		else
			EISTest->m_bH02=FALSE;
		EISTest->m_bACC2=FALSE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x05)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=FALSE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x2d || pBuf[0x04]==0x0d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=FALSE;
	}
	else if(pBuf[0x04]==0x1d || pBuf[0x04]==0x3d)
	{
		EISTest->m_bKey2=TRUE;
		EISTest->m_bH02=TRUE;
		EISTest->m_bACC2=TRUE;
		EISTest->m_bFire2=TRUE;
		EISTest->m_bStart2=TRUE;
	}
	//EIS状态
	if(pBuf[0x05]==0x02)
	{
		EISTest->m_bKey1=FALSE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x01)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=FALSE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x05)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=FALSE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x0d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=FALSE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x1d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=FALSE;
	}
	else if(pBuf[0x05]==0x3d)
	{
		EISTest->m_bKey1=TRUE;
		EISTest->m_bH01=TRUE;
		EISTest->m_bACC1=TRUE;
		EISTest->m_bFire1=TRUE;
		EISTest->m_bStart1=TRUE;
	}
}
void EISTestThread(void* p)
{
	CEISTest* EISTest=(CEISTest*)(p);
	CString str,strTemp,strTemp1,strInfo;
	CInteractionData Result;
	byte* pBuf=NULL;
	int Index=0;
	unsigned char rdEEDT[0x09][0x06]={
		{0x10,0x0B,0x0E,0x0D,0x0F,0x00},//1----
		{0x0E,0x05,0x0C,0x0B,0x0D,0x00},//2----
		{0x0E,0x05,0x0C,0x0B,0x0D,0x00},//3----
		{0x0A,0x05,0x08,0x07,0x09,0x00},//4----
		{0x0F,0x05,0x0D,0x0C,0x0E,0x00},//5----
		{0x0E,0x05,0x0C,0x0B,0x0D,0x00},//6----
		{0x0F,0x05,0x0D,0x0B,0x0E,0x00},//7----
		{0x0F,0x05,0x0D,0x0C,0x0E,0x00},//8----
		{0x0F,0x05,0x0D,0x0C,0x0E,0x00},//9----
	};
	while(EISTest->m_bRunning)
	{
		pBuf=CCarKey3App::TheHIDDevice.EISTest(EISTest->m_nMode,&Result,&Index);
		if(pBuf==NULL)
		{
// 			EISTest->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
// 			EISTest->GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
// 			strTemp.LoadString(IDS_STRING367);
// 			EISTest->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
// 			strTemp.LoadString(IDS_STRING368);
// 			EISTest->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
// 			EISTest->m_bThreadExit=true;
// 			EISTest->m_bRunning=false;
// 			return;
			Index=0;
			continue;
		}
		strInfo=CCommFunc::byteToHexStr(pBuf,8,_T(" "));
		TRACE(strInfo+_T("\r\n"));
		switch(EISTest->m_nMode)
		{
		case 0:
			EIS0(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			//EISTest->UpdateData(FALSE);
			break;
		case 1:
			EIS1(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		case 2:
			EIS2(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		case 3:
			EIS3(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		case 4:
			EIS4(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		case 5:
			EIS5(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		case 6:
			EIS6(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		case 7:
			EIS7(pBuf,EISTest);
			PostMessage(EISTest->m_hWnd,WM_MY_MESSAGE,0,0);
			break;
		}
		delete[] pBuf;
	}
	Index=rdEEDT[EISTest->m_nMode][0x04];
	CCarKey3App::TheHIDDevice.EISTest(EISTest->m_nMode,&Result,&Index);
	EISTest->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	EISTest->GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	strTemp.LoadString(IDS_STRING367);
	EISTest->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
	strTemp.LoadString(IDS_STRING368);
	EISTest->GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
	EISTest->m_bThreadExit=true;
	EISTest->m_bRunning=false;
}
// CEISTest 对话框

IMPLEMENT_DYNAMIC(CEISTest, CDialogEx)

CEISTest::CEISTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEISTest::IDD, pParent)
	, m_bKey2(FALSE)
	, m_bH02(FALSE)
	, m_bACC2(FALSE)
	, m_bFire2(FALSE)
	, m_bStart2(FALSE)
	, m_bKey1(FALSE)
	, m_bH01(FALSE)
	, m_bACC1(FALSE)
	, m_bFire1(FALSE)
	, m_bStart1(FALSE)
{
	m_bRunning=false;
	m_bThreadExit=true;
}

CEISTest::~CEISTest()
{
}

void CEISTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK3, m_bKey2);
	DDX_Check(pDX, IDC_CHECK5, m_bH02);
	DDX_Check(pDX, IDC_CHECK7, m_bACC2);
	DDX_Check(pDX, IDC_CHECK9, m_bFire2);
	DDX_Check(pDX, IDC_CHECK16, m_bStart2);
	DDX_Check(pDX, IDC_CHECK1, m_bKey1);
	DDX_Check(pDX, IDC_CHECK4, m_bH01);
	DDX_Check(pDX, IDC_CHECK6, m_bACC1);
	DDX_Check(pDX, IDC_CHECK8, m_bFire1);
	DDX_Check(pDX, IDC_CHECK15, m_bStart1);
}


BEGIN_MESSAGE_MAP(CEISTest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CEISTest::OnBnEISTest)
	ON_MESSAGE(WM_MY_MESSAGE ,MyMessage) 
END_MESSAGE_MAP()


// CEISTest 消息处理程序


BOOL CEISTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strTemp;
	strTemp.LoadString(IDS_STRING373);
	SetWindowText(strTemp); 

	strTemp.LoadString(IDS_STRING360);
	SetDlgItemText(IDC_STATIC1,strTemp); 

	strTemp.LoadString(IDS_STRING361);
	SetDlgItemText(IDC_STATIC2,strTemp); 

	strTemp.LoadString(IDS_STRING362);
	SetDlgItemText(IDC_STATIC3,strTemp); 

	strTemp.LoadString(IDS_STRING363);
	SetDlgItemText(IDC_STATIC4,strTemp); 

	strTemp.LoadString(IDS_STRING364);
	SetDlgItemText(IDC_STATIC5,strTemp); 

	strTemp.LoadString(IDS_STRING365);
	SetDlgItemText(IDC_STATIC6,strTemp); 

	strTemp.LoadString(IDS_STRING366);
	SetDlgItemText(IDC_STATIC7,strTemp);

	strTemp.LoadString(IDS_STRING367);
	SetDlgItemText(IDC_BUTTON2,strTemp); 

	strTemp.LoadString(IDS_STRING368);
	SetDlgItemText(IDC_BUTTON3,strTemp); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEISTest::OnBnEISTest()
{
	// TODO: 在此添加控件通知处理程序代码
 	CString str,strTemp,strTemp1,strInfo;
 	strTemp.LoadString(IDS_STRING369);
 	GetDlgItem(IDC_BUTTON2)->GetWindowText(str);
 	if(str==strTemp)
 	{
//  		strTemp.LoadString(IDS_PSWTip12);
//  		strTemp1.LoadString(IDS_Warring);
//  		if(MessageBox(strTemp, strTemp1,MB_YESNO) == IDYES)
//  		{
  			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
  			m_bRunning=false;
//  		}
 	}
 	else
 	{
 		CInteractionData rx;
 		rx=CCarKey3App::TheHIDDevice.Check_mima();
 		if(rx.mimaok!=true)
 		{
 			CString strx=rx.GetHIDResult();
 			//ShowLog(strx);
 			MessageBox(strx);
 			return;
 		}	
//  		strTemp.LoadString(IDS_StartCANInfo);
//  		ShowLog(strTemp);
		m_bKey2=FALSE;
		m_bH02=FALSE;
		m_bACC2=FALSE;
		m_bFire2=FALSE;
		m_bStart2=FALSE;
		m_bKey1=FALSE;
		m_bH01=FALSE;
		m_bACC1=FALSE;
		m_bFire1=FALSE;
		m_bStart1=FALSE;
 		UpdateData(FALSE);
 		strTemp.LoadString(IDS_STRING369);
 		GetDlgItem(IDC_BUTTON2)->SetWindowText(strTemp);
 		m_bRunning=true;
 		m_bThreadExit=false;
 		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
 		_beginthread(EISTestThread,0,(void *)(this));
 	}
}

LRESULT CEISTest::MyMessage( WPARAM wParam, LPARAM lParam )
{
	UpdateData(FALSE);
	return 0;
}
