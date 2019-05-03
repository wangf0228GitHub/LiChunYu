// InteractionData.cpp : 实现文件
//

#include "stdafx.h"
#ifdef CarKey2
#include ".\..\CarKey2\CarKey2\resource.h"
#else
#include ".\..\CarKey3\CarKey3\resource.h"
#endif
#include "InteractionData.h"
#include "..\CarKey3\CarKey3\CarKey3.h"


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
	if (buff[1] != 0x01)
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
		str.LoadString(NULL,IDS_USBError,CCarKey3App::LANGID);
		break;
	case HIDResult_MimaErr:
		str.LoadString(NULL,IDS_MimaErr,CCarKey3App::LANGID);
		break;
	case HIDResult_RxError:
		str.LoadString(NULL,IDS_RxError,CCarKey3App::LANGID);
		break;
	case HIDResult_ReadError:
		str.LoadString(NULL,IDS_ReadError,CCarKey3App::LANGID);
		break;
	case HIDResult_WriteError:
		str.LoadString(NULL,IDS_WriteError,CCarKey3App::LANGID);
		break;
	case HIDResult_RenewError:
		str.LoadString(NULL,IDS_RenewError,CCarKey3App::LANGID);
		break;
	case HIDResult_ProgramError:
		str.LoadString(NULL,IDS_ProgramError,CCarKey3App::LANGID);
		break;
	case HIDResult_GetPSWError1:
		str.LoadString(NULL,IDS_GetPSWError1,CCarKey3App::LANGID);
		break;
	case HIDResult_GetPSWError2:
		str.LoadString(NULL,IDS_GetPSWError2,CCarKey3App::LANGID);
		break;
	case HIDResult_GetPSWError3:
		str.LoadString(NULL,IDS_GetPSWError3,CCarKey3App::LANGID);
		break;
	case HIDResult_ReadEZSError1:
		str.LoadString(NULL,IDS_ReadEZSError1,CCarKey3App::LANGID);
		break;
	case HIDResult_ReadEZSError2:
		str.LoadString(NULL,IDS_ReadEZSError2,CCarKey3App::LANGID);
		break;//_T("红外读取电子点火开关数据错误2");
	case HIDResult_ReadEZSError3:
		str.LoadString(NULL,IDS_ReadEZSError3,CCarKey3App::LANGID);
		break;
	case HIDResult_ReadEZSError4:
		str.LoadString(NULL,IDS_ReadEZSError4,CCarKey3App::LANGID);
		break;
	case HIDResult_ReadEZSError5:
		str.LoadString(NULL,IDS_ReadEZSError5,CCarKey3App::LANGID);
		break;
	case HIDResult_CANErr:
		str.LoadString(NULL,IDS_CANErr,CCarKey3App::LANGID);
		break;
	case HIDResult_CANWriteWait:
		str.LoadString(NULL,IDS_CANWriteWait,CCarKey3App::LANGID);
		break;
	case HIDResult_InsertKeyTimeOut:
		str.LoadString(NULL,IDS_InsertKeyTimeOut,CCarKey3App::LANGID);
		break;
	case HIDResult_RenewToo:
		str.LoadString(NULL,IDS_RENEWTOO,CCarKey3App::LANGID);
		break;
	case HIDResult_RENEWError_a2:
		return _T("RENEWError_a2");
		break;
	case HIDResult_RenewError_b:
		return _T("RENEWError_b");
		break;
	case HIDResult_RenewError_bb:
		return _T("RENEWError_bb");
		break;
	case HIDResult_UserStop:
		str.LoadString(NULL,IDS_UserStop,CCarKey3App::LANGID);
		break;
	case HIDResult_ReadEZSError6:
		str.LoadString(NULL,IDS_ReadEZSError6,CCarKey3App::LANGID);
		break;
	case HIDResult_USED:
		str.LoadString(NULL,IDS_USED,CCarKey3App::LANGID);
		break;
	default:
		return _T("");
	}
	return str;
}



// CInteractionData 消息处理程序


