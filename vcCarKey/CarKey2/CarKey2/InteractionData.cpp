// InteractionData.cpp : 实现文件
//

#include "stdafx.h"
#include "CarKey2.h"
#include "InteractionData.h"


// CInteractionData

IMPLEMENT_DYNAMIC(CInteractionData, CWnd)

CInteractionData::CInteractionData()
{
	Init();
}

CInteractionData::CInteractionData(byte* pBuf)
{
	first = pBuf[0];
	for (int i = 0; i < 0x40; i++)
	{
		buff[i] = pBuf[i + 1];
	}
	bOK=true;
	mimaok=false;
}

CInteractionData::~CInteractionData()
{
}

byte* CInteractionData::GetDataList()
{
	byte *list=new byte[65];
	list[0] = first;
	for (int i = 0; i < 0x40; i++)
	{
		list[i + 1] = buff[i];
	}
	return list;
}

bool CInteractionData::CheckDataList()
{
	if (buff[2] != buff[0])
	{
		return false;
	}
	else
		return true;
}

void CInteractionData::SetData(byte* pBuf)
{
	first = pBuf[0];
	for (int i = 0; i < 0x40; i++)
	{
		buff[i] = pBuf[i + 1];
	}
	bOK=true;
}

void CInteractionData::Init()
{
	first = 0;
	for (int i = 0; i < 0x40; i++)
	{
		buff[i] = 0;
	}
	buff[8] = 0x01;//HIDDevice.SwVer[1];//L
	buff[9] = 0x20;//HIDDevice.SwVer[0];//H
	rxCount=0;
	bOK=false;
}

CString CInteractionData::GetHIDResult()
{
	CString str;
	switch(hidResult)
	{
	case HIDResult_USBError:
		str.LoadString(IDS_USBError);
		break;
	case HIDResult_MimaErr:
		str.LoadString(IDS_MimaErr);
		break;
	case HIDResult_RxError:
		str.LoadString(IDS_RxError);
		break;
	case HIDResult_ReadError:
		str.LoadString(IDS_ReadError);
		break;
	case HIDResult_WriteError:
		str.LoadString(IDS_WriteError);
		break;
	case HIDResult_RenewError:
		str.LoadString(IDS_RenewError);
		break;
	case HIDResult_ProgramError:
		str.LoadString(IDS_ProgramError);
		break;
	case HIDResult_GetPSWError1:
		str.LoadString(IDS_GetPSWError1);
		break;
	case HIDResult_GetPSWError2:
		str.LoadString(IDS_GetPSWError2);
		break;
	case HIDResult_GetPSWError3:
		str.LoadString(IDS_GetPSWError3);
		break;
	case HIDResult_ReadEZSError1:
		str.LoadString(IDS_ReadEZSError1);
		break;
	case HIDResult_ReadEZSError2:
		str.LoadString(IDS_ReadEZSError2);
		break;//_T("红外读取电子点火开关数据错误2");
	case HIDResult_ReadEZSError3:
		str.LoadString(IDS_ReadEZSError3);
		break;
	case HIDResult_ReadEZSError4:
		str.LoadString(IDS_ReadEZSError4);
		break;
	case HIDResult_ReadEZSError5:
		str.LoadString(IDS_ReadEZSError5);
		break;
	case HIDResult_CANErr:
		str.LoadString(IDS_CANErr);
		break;
	case HIDResult_CANWriteWait:
		str.LoadString(IDS_CANWriteWait);
		break;
	default:
		return _T("");
	}
	return str;
}



// CInteractionData 消息处理程序


