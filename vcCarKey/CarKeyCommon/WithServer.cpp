#include "StdAfx.h"
#include "WithServer.h"
#include "Http.h"
#include <afx.h>
#include "CommFunc.h"
#include "MyStringSplitter.h"
#include "VMProtectSDK.h"
#include "..\CarKey3\CarKey3\CarKey3.h"

bool g_bDPassThreadRunning;
bool g_bDPassThreadExit;
void DPassThread(void* p)
{
	VMProtectBegin("DPassThread");
	g_bDPassThreadExit=false;
	CDPassThreadParam* tParam=(CDPassThreadParam*)(p);
	CString serverid,strTemp,strTemp1,strRet;
	int retry=10;
	serverid=_T("");
	bool bFind=false;
	tParam->pProgress->SetRange(0,100);
	tParam->pProgress->SetPos(0);
	tParam->pProgress->UpdateWindow();
	byte* pDSW=NULL;
	while(retry!=0)
	{
		if(!g_bDPassThreadRunning)
			break;
		CWebRetData webRet;
		if(CCarKey3App::withServer.AddDPass(CCarKey3App::TheHIDDevice.HexID,tParam->pDealer,tParam->pSSID,webRet))
		{		
			if(webRet.retStatus==web_OK)			
			{
				serverid=webRet.strResult;
				break;
			}
			else if(webRet.retStatus==web_MaxUseTimes)
			{
				strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);				
				MessageBoxEx(tParam->hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				tParam->pStatusBar->SetText(strTemp, 0, 0);
				break;
			}
			else if(webRet.retStatus==web_NoRegDevice)
			{
				strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
				MessageBoxEx(tParam->hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				tParam->pStatusBar->SetText(strTemp, 0, 0);
				break;
			}
			else if(webRet.retStatus==2)//直接返回了结果
			{
				CStringArray strArray;
				CCarKey3App::withServer.GetRetList(webRet.strResult,_T("-"),strArray);				
				int len;
				byte* p=CCommFunc::strToHexByte(strArray.GetAt(1),_T(""),&len);
				pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,tParam->pSSID);
				delete[] p;
				bFind=true;
				tParam->pProgress->SetPos(100);
				tParam->pProgress->UpdateWindow();
				TRACE(CCommFunc::byteToHexStr(pDSW,8,_T("")));
				break;
				// 				for(int i=0;i<8;i++)
				// 				 	dlgEIS->m_EZSData.DPSW[i]=pDSW[i];
				// 				dlgEIS->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
				// 				strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
				// 				MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				// 				dlgEIS->ShowLog(strTemp);

				// 				TRACE(CCommFunc::byteToHexStr(dlgEIS->m_EZSData.DPSW,8,_T("")));
				// 				strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
				// 				dlgEIS->SetDlgItemText(IDC_BUTTON18,strTemp);
				// 				dlgEIS->m_bThreadExit=true;
				// 				dlgEIS->m_bRunning=false;
				//				break;
			}
			else
			{
				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				strTemp1.Format(strTemp,webRet.retStatus);
				MessageBoxEx(tParam->hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				tParam->pStatusBar->SetText(strTemp1, 0, 0);
				break;
			}								
		}
		else
			retry--;
	}	
	if(serverid!=_T("") && !bFind)
	{
		retry=10;
		tParam->pProgress->SetRange(0,10);
		tParam->pProgress->SetPos(0);
		tParam->pProgress->UpdateWindow();
		while(retry!=0)
		{		
			if(!g_bDPassThreadRunning)
				break;
			for(int i=1;i<11;i++)
			{
				Sleep(1000);
				tParam->pProgress->SetPos(i);
				tParam->pProgress->UpdateWindow();
				// 				dlgEIS->m_Progress.SetPos(i);
				// 				dlgEIS->m_Progress.UpdateWindow();
			}
			CWebRetData webRet;
			if(CCarKey3App::withServer.FindDPass(CCarKey3App::TheHIDDevice.HexID,serverid,tParam->pSSID,webRet))
			{
				if(webRet.retStatus==web_OK)			
				{
					CString strAnwser=webRet.strResult;
					if(strAnwser==_T(""))
					{
						strTemp.LoadString(NULL,IDS_ServerTip4,CCarKey3App::LANGID);
						MessageBoxEx(tParam->hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
						tParam->pStatusBar->SetText(strTemp, 0, 0);
						break;
					}
					else
					{
						CStringArray strArray;
						CCarKey3App::withServer.GetRetList(webRet.strResult,_T("-"),strArray);				
						int len;
						byte* p=CCommFunc::strToHexByte(strArray.GetAt(1),_T(""),&len);
						pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,tParam->pSSID);
						delete[] p;
						bFind=true;
						tParam->pProgress->SetPos(100);
						tParam->pProgress->UpdateWindow();
						TRACE(CCommFunc::byteToHexStr(pDSW,8,_T("")));
						break;
					}
					break;
				}
				else if(webRet.retStatus==web_MaxUseTimes)
				{
					strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);				
					MessageBoxEx(tParam->hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					tParam->pStatusBar->SetText(strTemp1, 0, 0);
					break;
				}
				else if(webRet.retStatus==web_NoRegDevice)
				{
					strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
					MessageBoxEx(tParam->hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					tParam->pStatusBar->SetText(strTemp1, 0, 0);
					break;
				}
				else if(webRet.retStatus==2)
				{
					
					strTemp.LoadString(NULL,IDS_ServerTip5,CCarKey3App::LANGID);
					int xx=_ttoi(webRet.strResult);
					tParam->pProgress->SetPos(xx);
					tParam->pProgress->UpdateWindow();
					strTemp1.Format(strTemp,xx);
					tParam->pStatusBar->SetText(strTemp1, 0, 0);
				}
				else if(webRet.retStatus==3)
				{
					strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
					int xx=_ttoi(webRet.strResult);
					strTemp1.Format(strTemp,xx);
					tParam->pStatusBar->SetText(strTemp1, 0, 0);
				}
				else if(webRet.retStatus==4)
				{
					strTemp.LoadString(NULL,IDS_ServerTip7,CCarKey3App::LANGID);
					MessageBoxEx(tParam->hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					tParam->pStatusBar->SetText(strTemp1, 0, 0);
					break;
				}
				else
				{
					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					strTemp1.Format(strTemp,webRet.retStatus);
					MessageBoxEx(tParam->hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					tParam->pStatusBar->SetText(strTemp1, 0, 0);
					break;
				}
			}
			else
				retry--;
		}
		if(retry==0)
		{
			strTemp.LoadString(NULL,IDS_ServerTip10,CCarKey3App::LANGID);
			MessageBoxEx(tParam->hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
			tParam->pStatusBar->SetText(strTemp, 0, 0);
		}
	}
	g_bDPassThreadRunning=false;
	g_bDPassThreadExit=true;
	tParam->pDPassResultProc(tParam->dlg,pDSW);//对话框处理界面
	if(pDSW!=NULL)
		delete[] pDSW;
	delete tParam;
	VMProtectEnd();
}
CWithServer::CWithServer(void)
{
	m_bOnline=false;
//#ifdef _DEBUG
//	//strUrl=_T("http://localhost:12891/");
//	strUrl=_T("http://www.mercedes-tool.com:9999/");
//	//strUrl=_T("http://chunyulee.vicp.net:13035/");
//#else
//	//strUrl=_T("http://www.mercedes-tool.com:9999/");
//	strUrl=_T("http://chunyulee.vicp.net/");
//#endif
//	//strUrl=_T("http://localhost:12891/");
//	//strUrl=_T("http://192.168.0.104/");
	strUrl=_T("http://chunyulee.vicp.net/");
}

CWithServer::~CWithServer(void)
{
	
}
bool CWithServer::CheckUrl()
{
	CHttp http;
 	strUrl=_T("http://www.mercedes-tool.com:9999/");
 	CString str=http.get(strUrl+_T("webAPI/GetSoftVer.aspx"));
 	if(str!=_T(""))
 	{
 		return true;
 	}
	strUrl=_T("http://chunyulee.vicp.net/");
	str=http.get(strUrl+_T("webAPI/GetSoftVer.aspx"));
	if(str!=_T(""))
	{
		return true;
	}
	return true;
}
bool CWithServer::CheckSoftVer(CWebRetData &webRet)
{
	m_bOnline=false;
	CHttp http;
	//CString str=http.get(_T("http://www.baidu.com/"));	
	CString str=http.get(strUrl+_T("webAPI/GetSoftVer.aspx"));
	if(str==_T(""))
	{
		m_bOnline=false;
		return false;
	}
	else
	{
		GetRet(str,webRet);
		return true;
//  		CMySplitStr MySplit;
//  		MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
//  		MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
//  		MySplit.SetData(str);//读取要分割的字符串
//  		CStringArray strArray;//读取结果存放的数组
//  		MySplit.GetSplitStrArray(strArray);
// 		webRet.retStatus=(webResult)_ttoi(strArray.GetAt(0));
// 		for(int i=1;i<strArray.GetCount();i++)
// 			webRet.strArray.Add(strArray.GetAt(i));
// 		return true;
//  		if(strArray.GetCount()==3)
//  		{
// 			
//  			SoftVer=_ttof(strArray.GetAt(0));
//  			HerVer=_ttof(strArray.GetAt(1));
//  			m_bOnline=true;
//  			return true;
//  		}
//  		else
//  			return false;
	}
}

// bool CWithServer::GetESLPass(byte* pDeviceID,byte* pBuf,CString sTime,CString &Ret)
// {
// 	try
// 	{
// 		CHttp http;		
// 		CString url;
// 		url = strUrl+_T("webAPI/ESLPass.aspx?DeviceID=");//0445615C";
// 		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
// 		url = url + "&pass=" + CCommFunc::byteToHexStr(pBuf,8,_T(""));
// 		url = url + "&sTime=" + sTime;
// 		CString str=http.get(url);
// 		TRACE(str+_T("\r\n"));
// 		Ret=str;
// 	}	
// 	catch (CException* e)
// 	{
// 		//MessageBox(e->m)
// 		return false;
// 	}
// 	return true;
// }

bool CWithServer::GetESLPass( byte* pDeviceID,byte* pBuf,CString sTime,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/ESLPass.aspx?DeviceID=");//0445615C";
		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
		url = url + "&pass=" + CCommFunc::byteToHexStr(pBuf,8,_T(""));
		url = url + "&sTime=" + sTime;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

bool CWithServer::AddDPass( byte* pDeviceID,byte* pBuf,byte* pSSID,CWebRetData &webRet)
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/DPassAdd.aspx?DeviceID=");//0445615C";
		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
		url = url + "&pass=" + CCommFunc::byteToHexStr(pBuf,8,_T(""));
		url = url + "&SSID=" + CCommFunc::byteToHexStr(pSSID,4,_T(""));
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

// bool CWithServer::AddDPass( byte* pDeviceID,byte* pBuf,CString &webRet,byte* pSSID )
// {
// 
// 	try
// 	{
// 		CHttp http;		
// 		CString url;
// 		url = strUrl+_T("webAPI/DPassAdd.aspx?DeviceID=");//0445615C";
// 		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
// 		url = url + "&pass=" + CCommFunc::byteToHexStr(pBuf,8,_T(""));
// 		url = url + "&SSID=" + CCommFunc::byteToHexStr(pSSID,4,_T(""));
// 		CString str=http.get(url);
// 		TRACE(str+_T("\r\n"));
// 		webRet=str;
// 	}	
// 	catch (CException* e)
// 	{
// 		//MessageBox(e->m)
// 		return false;
// 	}
// 	return true;
// }
// 
// bool CWithServer::FindDPass( byte* pDeviceID,CString serverid,CString &Ret ,byte* pSSID)
// {
// 	try
// 	{
// 		CHttp http;		
// 		CString url;
// 		url = strUrl+_T("webAPI/DPassStatus.aspx?DeviceID=");//0445615C";
// 		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
// 		url = url + "&serverid=" + serverid;
// 		url = url + "&SSID=" + CCommFunc::byteToHexStr(pSSID,4,_T(""));
// 		CString str=http.get(url);
// 		TRACE(str+_T("\r\n"));
// 		Ret=str;
// 	}	
// 	catch (CException* e)
// 	{
// 		//MessageBox(e->m)
// 		return false;
// 	}
// 	return true;
// }

bool CWithServer::FindDPass( byte* pDeviceID,CString serverid,byte* pSSID,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/DPassStatus.aspx?DeviceID=");//0445615C";
		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
		url = url + "&serverid=" + serverid;
		url = url + "&SSID=" + CCommFunc::byteToHexStr(pSSID,4,_T(""));
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

// bool CWithServer::UploadHPass( byte* pDeviceID,CString &Ret,CWebRetData &webRet )
// {
// 	try
// 	{
// 		CHttp http;		
// 		CString url;
// 		url = strUrl+_T("webAPI/HPassWork.aspx");
// 		CString str=CCommFunc::byteToHexStr(pDeviceID,100,_T(""));
// 		str=http.post(url,str);
// 		TRACE(str+_T("\r\n"));
// 		GetRet(str,webRet);
// 	}	
// 	catch (CException* e)
// 	{
// 		//MessageBox(e->m)
// 		return false;
// 	}
// 	return true;
// }

bool CWithServer::UploadHPass( byte* pBuf,int count,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/HPassAdd.aspx");
		CString str=CCommFunc::byteToHexStr(pBuf,count,_T(""));
		str=http.post(url,str);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

bool CWithServer::HPassStatus( CString fileName,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/HPassStatus.aspx?FileName=");//0445615C";
		url+=fileName;
// 		srand((int)time(0));
// 		int r=rand();
// 		CString strqq;
// 		strqq.Format(_T("&strAdd=%d"),r);
// 		url+=strqq;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

bool CWithServer::GetSoftPocket( byte* pDeviceID,int index,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/GetSoftData.aspx?DeviceID=");//0445615C";
		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
		CString csStr;
		csStr.Format(_T("%d"), index);
		url = url + "&PocketIndex=" + csStr;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}
bool CWithServer::GetHexPocket( byte* pDeviceID,int index,CWebRetData &webRet)
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/GetHexData.aspx?DeviceID=");//0445615C";
		url+=CCommFunc::byteToHexStr(pDeviceID,4,_T(""));
		CString csStr;
		csStr.Format(_T("%d"), index);
		url = url + "&PocketIndex=" + csStr;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}
bool CWithServer::AddKeyData( byte* pBuf,int count,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/KeyDataAdd.aspx");
		CString str=CCommFunc::byteToHexStr(pBuf,count,_T(""));
		str=http.post(url,str);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

bool CWithServer::CancelKeyData( CString queueID,CString &Ret )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/KeyDataCancel.aspx?queueID=");//0445615C";
		url = url + queueID;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		Ret=str;
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

bool CWithServer::StatusKeyData( CString queueID,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/KeyDataStatus.aspx?queueID=");//0445615C";
		url = url + queueID;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

bool CWithServer::KeyDataGet( CString queueID,CString keyIndex,CString format,CWebRetData &webRet )
{
	try
	{
		CHttp http;		
		CString url;
		url = strUrl+_T("webAPI/KeyDataGet.aspx?queueID=");//0445615C";
		url = url + queueID;
		url = url + "&keyIndex="+keyIndex;
		url = url + "&format="+format;
		CString str=http.get(url);
		TRACE(str+_T("\r\n"));
		GetRet(str,webRet);
	}	
	catch (CException* e)
	{
		//MessageBox(e->m)
		return false;
	}
	return true;
}

void CWithServer::GetRet(CString str,CWebRetData &webRet )
{
	CMySplitStr MySplit;
	MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
	MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
	MySplit.SetData(str);//读取要分割的字符串
	CStringArray strArray;//读取结果存放的数组
	MySplit.GetSplitStrArray(strArray);
	webRet.retStatus=(webResult)_ttoi(strArray.GetAt(0));
	webRet.strResult=strArray.GetAt(1);
}

void CWithServer::GetRetList( CString str,CString split,CStringArray &array )
{
	CMySplitStr MySplit;
	MySplit.SetSplitFlag(split);//设定分隔符，默认值也是“，”
	MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
	MySplit.SetData(str);//读取要分割的字符串
	MySplit.GetSplitStrArray(array);
}


