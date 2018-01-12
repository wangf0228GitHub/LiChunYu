#pragma once

#include <afxinet.h>
#include <atlimage.h>

///////////////////////////////////////////////////////////////////
/*
HTTP协议操作类
功能：实现HTTP的GET和POST操作
作者：kjuu@yahoo.cn 2268190059
时间：2011-10-20 到 2011-10-24
*/
////////////////////////////////////////////////////////////////////
enum webResult
{
	web_OK=1,
	web_MaxUseTimes=100,
	web_MissParam=-1,
	web_ErrParam=-2,
	web_MissServer = -3,
	web_ErrServerRet = -4,
	web_NoHexBin=-11,
	web_NoRegDevice=-50,
	web_UnkownErr=-100
};
class CWebRetData
{
public:
	webResult retStatus;
	CString strResult;
};
typedef void (*DPassResultProc)(void*,byte*);
extern bool g_bDPassThreadRunning;
extern bool g_bDPassThreadExit;
extern void DPassThread(void* p);
class CDPassThreadParam
{
public:
	void* dlg;
	byte* pDealer;
	byte* pSSID;
	CProgressCtrl* pProgress;
	CStatusBarCtrl* pStatusBar;
	HWND hWnd;
	DPassResultProc pDPassResultProc;
};
class CWithServer:CWnd
{
public:
	bool m_bOnline;
	
//	bool GetESLPass(byte* pDeviceID,byte* pBuf,CString sTime,CString &Ret);
	
// 	bool AddDPass(byte* pDeviceID,byte* pBuf,CString &webRet,byte* pSSID);
// 	bool FindDPass(byte* pDeviceID,CString serverid,CString &Ret,byte* pSSID);
	
// 	bool UploadHPass(byte* pDeviceID,CString &Ret,byte* pSSID);
// 	bool UploadHPass(byte* pBuf,int count,CString &Ret);
//	bool HPassStatus(CString fileName,CString &Ret);
//	bool GetSoftPocket(byte* pDeviceID,int index,CString &Ret);

//	bool AddKeyData(byte* pBuf,int count,CString &Ret);
	bool CancelKeyData(CString queueID,CString &Ret);
//	bool StatusKeyData(CString queueID,CString &Ret);
//	bool KeyDataGet(CString queueID,CString keyIndex,CString format,CString &Ret);
	


	bool CheckSoftVer(CWebRetData &webRet);
	bool AddDPass(byte* pDeviceID,byte* pBuf,byte* pSSID,CWebRetData &webRet);
	bool FindDPass(byte* pDeviceID,CString serverid,byte* pSSID,CWebRetData &webRet);
	void GetRet(CString str,CWebRetData &webRet);
	CString strUrl;
	void GetRetList(CString str,CString split,CStringArray &array);
	bool GetESLPass(byte* pDeviceID,byte* pBuf,CString sTime,CWebRetData &webRet);
	bool AddKeyData(byte* pBuf,int count,CWebRetData &webRet);
	bool StatusKeyData(CString queueID,CWebRetData &webRet);
	bool KeyDataGet(CString queueID,CString keyIndex,CString format,CWebRetData &webRet);
	bool UploadHPass(byte* pBuf,int count,CWebRetData &webRet);
	bool HPassStatus(CString fileName,CWebRetData &webRet);
	bool GetSoftPocket(byte* pDeviceID,int index,CWebRetData &webRet);
public:
	CWithServer(void);

public:
	~CWithServer(void);
	bool GetHexPocket( byte* pDeviceID,int index,CWebRetData &webRet );
	
};


