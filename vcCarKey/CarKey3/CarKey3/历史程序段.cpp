//EIS中DPass获取线程
 void DPassThread(void* p)
 {
 	VMProtectBegin("DPassThread");
 	CDlgEIS* dlgEIS=(CDlgEIS*)(p);
 	CString serverid,strTemp,strTemp1,strRet;
 	int retry=10;
 	serverid=_T("");
 	bool bFind=false;
 	dlgEIS->m_Progress.SetRange(0,100);
 	dlgEIS->m_Progress.SetPos(0);
 	dlgEIS->m_Progress.UpdateWindow();
 	while(retry!=0)
 	{
 		if(!dlgEIS->m_bRunning)
 			break;
 		if(CCarKey3App::withServer.AddDPass(CCarKey3App::TheHIDDevice.HexID,&(dlgEIS->m_EZSData.AllFile[0x10]),strRet,dlgEIS->m_EZSData.AllFile))
 		{
 			if(strRet==_T(""))
 			{
 				retry--;
 				continue;
 			}
 			CMySplitStr MySplit;
 			MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
 			MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
 			MySplit.SetData(strRet);//读取要分割的字符串
 			CStringArray strArray;//读取结果存放的数组
 			MySplit.GetSplitStrArray(strArray);
 			int retCode=_ttoi(strArray.GetAt(0));
 			if(retCode==1)
 			{
 				if(strArray.GetCount()==2)
 				{
 					serverid=strArray.GetAt(1);
 					break;
 				}
 				else
 				{
 					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 					strTemp1.Format(strTemp,1002);
 					MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 					dlgEIS->ShowLog(strTemp1);
 					break;
 				}
 			}
 			else if(retCode==2)
 			{
 				if(strArray.GetCount()==2)
 				{
 					CString strAnwser=strArray.GetAt(1);
 					CMySplitStr MySplit1;
 					MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
 					MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
 					MySplit1.SetData(strAnwser);//读取要分割的字符串
 					CStringArray strArray1;//读取结果存放的数组
 					MySplit1.GetSplitStrArray(strArray1);
 					int len;
 					byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
 					byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,dlgEIS->m_EZSData.AllFile);
 					for(int i=0;i<8;i++)
 						dlgEIS->m_EZSData.DPSW[i]=pDSW[i];
 					dlgEIS->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
 					strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
 					MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
 					dlgEIS->ShowLog(strTemp);
 					bFind=true;
 					dlgEIS->m_Progress.SetPos(100);
 					dlgEIS->m_Progress.UpdateWindow();
 					delete[] p;
 					delete[] pDSW;
 					TRACE(CCommFunc::byteToHexStr(dlgEIS->m_EZSData.DPSW,8,_T("")));
 					strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
 					dlgEIS->SetDlgItemText(IDC_BUTTON18,strTemp);
 					dlgEIS->m_bThreadExit=true;
 					dlgEIS->m_bRunning=false;
 					break;
 				}
 				else
 				{
 					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 					strTemp1.Format(strTemp,1002);
 					MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 					dlgEIS->ShowLog(strTemp1);
 					break;
 				}
 			}
 			else if(retCode==100)
 			{
 				strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);				
 				MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
 				dlgEIS->ShowLog(strTemp);
 				break;
 			}
 			else if(retCode==-50)
 			{
 				strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
 				MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
 				dlgEIS->ShowLog(strTemp);
 				break;
 			}
 			else
 			{
 				strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 				strTemp1.Format(strTemp,retCode);
 				MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 				dlgEIS->ShowLog(strTemp1);
 				break;
 			}								
 		}
 		else
 			retry--;
 	}
 	if(serverid!=_T("") && !bFind)
 	{
 		retry=10;
 		while(retry!=0)
 		{		
 			if(!dlgEIS->m_bRunning)
 				break;
 			for(int i=1;i<11;i++)
 			{
 				Sleep(1000);
 // 				dlgEIS->m_Progress.SetPos(i);
 // 				dlgEIS->m_Progress.UpdateWindow();
 			}
 			if(CCarKey3App::withServer.FindDPass(CCarKey3App::TheHIDDevice.HexID,serverid,strRet,dlgEIS->m_EZSData.AllFile))
 			{
 				if(strRet==_T(""))
 				{
 					retry--;
 					continue;
 				}
 				CMySplitStr MySplit;
 				MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
 				MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
 				MySplit.SetData(strRet);//读取要分割的字符串
 				CStringArray strArray;//读取结果存放的数组
 				MySplit.GetSplitStrArray(strArray);
 				int retCode=_ttoi(strArray.GetAt(0));
 				if(retCode==1)
 				{
 					if(strArray.GetCount()==2)
 					{
 						CString strAnwser=strArray.GetAt(1);
 						if(strAnwser==_T(""))
 						{
 							strTemp.LoadString(NULL,IDS_ServerTip4,CCarKey3App::LANGID);
 							MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
 							dlgEIS->ShowLog(strTemp);
 							break;
 						}
 						else
 						{
 							CString strAnwser=strArray.GetAt(1);
 							CMySplitStr MySplit1;
 							MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
 							MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
 							MySplit1.SetData(strAnwser);//读取要分割的字符串
 							CStringArray strArray1;//读取结果存放的数组
 							MySplit1.GetSplitStrArray(strArray1);
 							int len;
 							byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
 							byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,dlgEIS->m_EZSData.AllFile);
 							for(int i=0;i<8;i++)
 								dlgEIS->m_EZSData.DPSW[i]=pDSW[i];
 							dlgEIS->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
 							strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
 							dlgEIS->ShowLog(strTemp);
 							bFind=true;
 							dlgEIS->m_Progress.SetPos(100);
 							dlgEIS->m_Progress.UpdateWindow();
 							delete[] p;
 							delete[] pDSW;
 							TRACE(CCommFunc::byteToHexStr(dlgEIS->m_EZSData.DPSW,8,_T("")));
 							strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
 							dlgEIS->SetDlgItemText(IDC_BUTTON18,strTemp);
 							dlgEIS->m_bThreadExit=true;
 							dlgEIS->m_bRunning=false;
 							break;
 						}
 						break;
 					}
 					else
 					{
 						strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 						strTemp1.Format(strTemp,1002);
 						MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 						dlgEIS->ShowLog(strTemp1);
 						break;
 					}
 				}
 				else if(retCode==2)
 				{
 					if(strArray.GetCount()==2)
 					{
 						strTemp.LoadString(NULL,IDS_ServerTip5,CCarKey3App::LANGID);
 						int xx=_ttoi(strArray.GetAt(1));
 						dlgEIS->m_Progress.SetPos(xx);
 						dlgEIS->m_Progress.UpdateWindow();
 						strTemp1.Format(strTemp,xx);
 						dlgEIS->ShowLog(strTemp1);
 					}
 					else
 					{
 						strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 						strTemp1.Format(strTemp,1002);
 						MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 						dlgEIS->ShowLog(strTemp1);
 						break;
 					}
 				}
 				else if(retCode==3)
 				{
 					if(strArray.GetCount()==2)
 					{
 						strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
 						int xx=_ttoi(strArray.GetAt(1));
 						strTemp1.Format(strTemp,xx);
 						dlgEIS->ShowLog(strTemp1);
 					}
 					else
 					{
 						strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 						strTemp1.Format(strTemp,1002);
 						MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 						dlgEIS->ShowLog(strTemp1);
 						break;
 					}
 				}
 				else if(retCode==4)
 				{
 					strTemp.LoadString(NULL,IDS_ServerTip7,CCarKey3App::LANGID);
 					MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
 					dlgEIS->ShowLog(strTemp);
 					break;
 				}
 				else
 				{
 					strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
 					strTemp1.Format(strTemp,retCode);
 					MessageBoxEx(dlgEIS->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
 					dlgEIS->ShowLog(strTemp);
 					break;
 				}
 			}
 			else
 				retry--;
 		}
 // 		if(retry==0)
 // 		{
 // 			strTemp.LoadString(nullIDS_ServerTip10);
 // 			MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,0);
 // 			dlgEIS->ShowLog(strTemp);
 // 		}
 	}
 	if(bFind)
 	{
 		strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
 		dlgEIS->GetDlgItem(IDC_BUTTON18)->EnableWindow(TRUE);
 	
 	}
 	else
 	{
 		strTemp.LoadString(NULL,IDS_RenewPassCalc,CCarKey3App::LANGID);
 		dlgEIS->GetDlgItem(IDC_BUTTON18)->EnableWindow(TRUE);
 	}
 	dlgEIS->SetDlgItemText(IDC_BUTTON18,strTemp);
 	dlgEIS->EnableBotton(TRUE);
 	dlgEIS->m_bThreadExit=true;
 	dlgEIS->m_bRunning=false;
 	VMProtectEnd();
 }

 //ECU中DPass获取线程
 void ECUDPassThread(void* p)
 {
	 VMProtectBegin("ECUDPassThread");
	 CDlgECU* dlgECU=(CDlgECU*)(p);
	 CString serverid,strTemp,strTemp1,strRet;
	 int retry=10;
	 serverid=_T("");
	 bool bFind=false;
	 dlgECU->m_Progress.SetRange(0,100);
	 dlgECU->m_Progress.SetPos(0);
	 dlgECU->m_Progress.UpdateWindow();
	 byte ecuSSID[4];
	 for(int i=0;i<4;i++)
		 ecuSSID[i]=i;
	 while(retry!=0)
	 {
		 if(!dlgECU->m_bRunning)
			 break;		
		 if(CCarKey3App::withServer.AddDPass(CCarKey3App::TheHIDDevice.HexID,&(dlgECU->ECUData[0x80]),strRet,ecuSSID))
		 {
			 if(strRet==_T(""))
			 {
				 retry--;
				 continue;
			 }
			 CMySplitStr MySplit;
			 MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
			 MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
			 MySplit.SetData(strRet);//读取要分割的字符串
			 CStringArray strArray;//读取结果存放的数组
			 MySplit.GetSplitStrArray(strArray);
			 int retCode=_ttoi(strArray.GetAt(0));
			 if(retCode==1)
			 {
				 if(strArray.GetCount()==2)
				 {
					 serverid=strArray.GetAt(1);
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,1002);
					 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgECU->ShowLog(strTemp1);
					 break;
				 }
			 }
			 else if(retCode==2)
			 {
				 if(strArray.GetCount()==2)
				 {
					 CString strAnwser=strArray.GetAt(1);
					 CMySplitStr MySplit1;
					 MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
					 MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
					 MySplit1.SetData(strAnwser);//读取要分割的字符串
					 CStringArray strArray1;//读取结果存放的数组
					 MySplit1.GetSplitStrArray(strArray1);
					 int len;
					 byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
					 byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,ecuSSID);
					 for(int i=0;i<8;i++)
						 dlgECU->m_EZSData.DPSW[i]=pDSW[i];
					 dlgECU->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
					 strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
					 MessageBoxEx(dlgECU->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgECU->ShowLog(strTemp);
					 bFind=true;
					 dlgECU->m_Progress.SetPos(100);
					 dlgECU->m_Progress.UpdateWindow();
					 delete[] p;
					 delete[] pDSW;
					 TRACE(CCommFunc::byteToHexStr(dlgECU->m_EZSData.DPSW,8,_T("")));
					 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
					 dlgECU->SetDlgItemText(IDC_BUTTON4,strTemp);
					 dlgECU->m_bThreadExit=true;
					 dlgECU->m_bRunning=false;
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,1002);
					 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgECU->ShowLog(strTemp1);
					 break;
				 }
			 }
			 else if(retCode==100)
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);				
				 MessageBoxEx(dlgECU->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgECU->ShowLog(strTemp);
				 break;
			 }
			 else if(retCode==-50)
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
				 MessageBoxEx(dlgECU->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgECU->ShowLog(strTemp);
				 break;
			 }
			 else
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				 strTemp1.Format(strTemp,retCode);
				 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgECU->ShowLog(strTemp1);
				 break;
			 }								
		 }
		 else
			 retry--;
	 }
	 if(serverid!=_T("") || !bFind)
	 {
		 retry=10;
		 while(retry!=0)
		 {		
			 if(!dlgECU->m_bRunning)
				 break;
			 for(int i=1;i<11;i++)
			 {
				 Sleep(1000);
				 // 				dlgEIS->m_Progress.SetPos(i);
				 // 				dlgEIS->m_Progress.UpdateWindow();
			 }
			 if(CCarKey3App::withServer.FindDPass(CCarKey3App::TheHIDDevice.HexID,serverid,strRet,ecuSSID))
			 {
				 if(strRet==_T(""))
				 {
					 retry--;
					 continue;
				 }
				 CMySplitStr MySplit;
				 MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
				 MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
				 MySplit.SetData(strRet);//读取要分割的字符串
				 CStringArray strArray;//读取结果存放的数组
				 MySplit.GetSplitStrArray(strArray);
				 int retCode=_ttoi(strArray.GetAt(0));
				 if(retCode==1)
				 {
					 if(strArray.GetCount()==2)
					 {
						 CString strAnwser=strArray.GetAt(1);
						 if(strAnwser==_T(""))
						 {
							 strTemp.LoadString(NULL,IDS_ServerTip4,CCarKey3App::LANGID);
							 MessageBoxEx(dlgECU->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
							 dlgECU->ShowLog(strTemp);
							 break;
						 }
						 else
						 {
							 CString strAnwser=strArray.GetAt(1);
							 CMySplitStr MySplit1;
							 MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
							 MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
							 MySplit1.SetData(strAnwser);//读取要分割的字符串
							 CStringArray strArray1;//读取结果存放的数组
							 MySplit1.GetSplitStrArray(strArray1);
							 int len;
							 byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
							 byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,ecuSSID);
							 for(int i=0;i<8;i++)
								 dlgECU->m_EZSData.DPSW[i]=pDSW[i];
							 dlgECU->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
							 strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
							 dlgECU->ShowLog(strTemp);
							 bFind=true;
							 dlgECU->m_Progress.SetPos(100);
							 dlgECU->m_Progress.UpdateWindow();
							 delete[] p;
							 delete[] pDSW;
							 TRACE(CCommFunc::byteToHexStr(dlgECU->m_EZSData.DPSW,8,_T("")));
							 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
							 dlgECU->SetDlgItemText(IDC_BUTTON4,strTemp);
							 dlgECU->m_bThreadExit=true;
							 dlgECU->m_bRunning=false;
							 break;
						 }
						 break;
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgECU->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==2)
				 {
					 if(strArray.GetCount()==2)
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip5,CCarKey3App::LANGID);
						 int xx=_ttoi(strArray.GetAt(1));
						 dlgECU->m_Progress.SetPos(xx);
						 dlgECU->m_Progress.UpdateWindow();
						 strTemp1.Format(strTemp,xx);
						 dlgECU->ShowLog(strTemp1);
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgECU->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==3)
				 {
					 if(strArray.GetCount()==2)
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
						 int xx=_ttoi(strArray.GetAt(1));
						 strTemp1.Format(strTemp,xx);
						 dlgECU->ShowLog(strTemp1);
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgECU->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==4)
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip7,CCarKey3App::LANGID);
					 MessageBoxEx(dlgECU->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgECU->ShowLog(strTemp);
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,retCode);
					 MessageBoxEx(dlgECU->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgECU->ShowLog(strTemp);
					 break;
				 }
			 }
			 else
				 retry--;
		 }
		 // 		if(retry==0)
		 // 		{
		 // 			strTemp.LoadString(IDS_ServerTip10);
		 // 			MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,0);
		 // 			dlgEIS->ShowLog(strTemp);
		 // 		}
	 }
	 if(bFind)
	 {
		 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
		 dlgECU->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);

	 }
	 else
	 {
		 strTemp.LoadString(NULL,IDS_RenewPassCalc,CCarKey3App::LANGID);
		 dlgECU->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	 }
	 dlgECU->SetDlgItemText(IDC_BUTTON4,strTemp);
	 dlgECU->EnableControl(TRUE);
	 dlgECU->m_bThreadExit=true;
	 dlgECU->m_bRunning=false;
	 VMProtectEnd();
 }

  //ESL中DPass获取线程
 void ESLDPassThread(void* p)
 {
	 VMProtectBegin("ESLDPassThread");
	 CDlgESL* dlgESL=(CDlgESL*)(p);
	 CString serverid,strTemp,strTemp1,strRet;
	 int retry=10;
	 serverid=_T("");
	 bool bFind=false;
	 dlgESL->m_Progress.SetRange(0,100);
	 dlgESL->m_Progress.SetPos(0);
	 dlgESL->m_Progress.UpdateWindow();
	 byte eslSSID[4];
	 for(int i=0;i<4;i++)
		 eslSSID[i]=i;
	 while(retry!=0)
	 {
		 if(!dlgESL->m_bRunning)
			 break;		
		 if(CCarKey3App::withServer.AddDPass(CCarKey3App::TheHIDDevice.HexID,&(dlgESL->ESLData[0x100]),strRet,eslSSID))
		 {
			 if(strRet==_T(""))
			 {
				 retry--;
				 continue;
			 }
			 CMySplitStr MySplit;
			 MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
			 MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
			 MySplit.SetData(strRet);//读取要分割的字符串
			 CStringArray strArray;//读取结果存放的数组
			 MySplit.GetSplitStrArray(strArray);
			 int retCode=_ttoi(strArray.GetAt(0));
			 if(retCode==1)
			 {
				 if(strArray.GetCount()==2)
				 {
					 serverid=strArray.GetAt(1);
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,1002);
					 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgESL->ShowLog(strTemp1);
					 break;
				 }
			 }
			 else if(retCode==2)
			 {
				 if(strArray.GetCount()==2)
				 {
					 CString strAnwser=strArray.GetAt(1);
					 CMySplitStr MySplit1;
					 MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
					 MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
					 MySplit1.SetData(strAnwser);//读取要分割的字符串
					 CStringArray strArray1;//读取结果存放的数组
					 MySplit1.GetSplitStrArray(strArray1);
					 int len;
					 byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
					 byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,eslSSID);
					 for(int i=0;i<8;i++)
						 dlgESL->m_EZSData.DPSW[i]=pDSW[i];
					 dlgESL->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
					 strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
					 MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgESL->ShowLog(strTemp);
					 bFind=true;
					 dlgESL->m_Progress.SetPos(100);
					 dlgESL->m_Progress.UpdateWindow();
					 delete[] p;
					 delete[] pDSW;
					 TRACE(CCommFunc::byteToHexStr(dlgESL->m_EZSData.DPSW,8,_T("")));
					 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
					 dlgESL->SetDlgItemText(IDC_BUTTON4,strTemp);
					 dlgESL->m_bThreadExit=true;
					 dlgESL->m_bRunning=false;
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,1002);
					 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgESL->ShowLog(strTemp1);
					 break;
				 }
			 }
			 else if(retCode==100)
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);				
				 MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgESL->ShowLog(strTemp);
				 break;
			 }
			 else if(retCode==-50)
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
				 MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgESL->ShowLog(strTemp);
				 break;
			 }
			 else
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				 strTemp1.Format(strTemp,retCode);
				 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgESL->ShowLog(strTemp1);
				 break;
			 }								
		 }
		 else
			 retry--;
	 }
	 if(serverid!=_T("") || !bFind)
	 {
		 retry=10;
		 while(retry!=0)
		 {		
			 if(!dlgESL->m_bRunning)
				 break;
			 for(int i=1;i<11;i++)
			 {
				 Sleep(1000);
				 // 				dlgEIS->m_Progress.SetPos(i);
				 // 				dlgEIS->m_Progress.UpdateWindow();
			 }
			 if(CCarKey3App::withServer.FindDPass(CCarKey3App::TheHIDDevice.HexID,serverid,strRet,eslSSID))
			 {
				 if(strRet==_T(""))
				 {
					 retry--;
					 continue;
				 }
				 CMySplitStr MySplit;
				 MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
				 MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
				 MySplit.SetData(strRet);//读取要分割的字符串
				 CStringArray strArray;//读取结果存放的数组
				 MySplit.GetSplitStrArray(strArray);
				 int retCode=_ttoi(strArray.GetAt(0));
				 if(retCode==1)
				 {
					 if(strArray.GetCount()==2)
					 {
						 CString strAnwser=strArray.GetAt(1);
						 if(strAnwser==_T(""))
						 {
							 strTemp.LoadString(NULL,IDS_ServerTip4,CCarKey3App::LANGID);
							 MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
							 dlgESL->ShowLog(strTemp);
							 break;
						 }
						 else
						 {
							 CString strAnwser=strArray.GetAt(1);
							 CMySplitStr MySplit1;
							 MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
							 MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
							 MySplit1.SetData(strAnwser);//读取要分割的字符串
							 CStringArray strArray1;//读取结果存放的数组
							 MySplit1.GetSplitStrArray(strArray1);
							 int len;
							 byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
							 byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,eslSSID);
							 for(int i=0;i<8;i++)
								 dlgESL->m_EZSData.DPSW[i]=pDSW[i];
							 dlgESL->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
							 strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
							 dlgESL->ShowLog(strTemp);
							 bFind=true;
							 dlgESL->m_Progress.SetPos(100);
							 dlgESL->m_Progress.UpdateWindow();
							 delete[] p;
							 delete[] pDSW;
							 TRACE(CCommFunc::byteToHexStr(dlgESL->m_EZSData.DPSW,8,_T("")));
							 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
							 dlgESL->SetDlgItemText(IDC_BUTTON4,strTemp);
							 dlgESL->m_bThreadExit=true;
							 dlgESL->m_bRunning=false;
							 break;
						 }
						 break;
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgESL->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==2)
				 {
					 if(strArray.GetCount()==2)
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip5,CCarKey3App::LANGID);
						 int xx=_ttoi(strArray.GetAt(1));
						 dlgESL->m_Progress.SetPos(xx);
						 dlgESL->m_Progress.UpdateWindow();
						 strTemp1.Format(strTemp,xx);
						 dlgESL->ShowLog(strTemp1);
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgESL->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==3)
				 {
					 if(strArray.GetCount()==2)
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
						 int xx=_ttoi(strArray.GetAt(1));
						 strTemp1.Format(strTemp,xx);
						 dlgESL->ShowLog(strTemp1);
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgESL->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==4)
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip7,CCarKey3App::LANGID);
					 MessageBoxEx(dlgESL->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgESL->ShowLog(strTemp);
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,retCode);
					 MessageBoxEx(dlgESL->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgESL->ShowLog(strTemp);
					 break;
				 }
			 }
			 else
				 retry--;
		 }
		 // 		if(retry==0)
		 // 		{
		 // 			strTemp.LoadString(IDS_ServerTip10);
		 // 			MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,0);
		 // 			dlgEIS->ShowLog(strTemp);
		 // 		}
	 }
	 if(bFind)
	 {
		 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
		 dlgESL->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);

	 }
	 else
	 {
		 strTemp.LoadString(NULL,IDS_RenewPassCalc,CCarKey3App::LANGID);
		 dlgESL->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	 }
	 dlgESL->SetDlgItemText(IDC_BUTTON4,strTemp);
	 dlgESL->EnableControl(TRUE);
	 dlgESL->m_bThreadExit=true;
	 dlgESL->m_bRunning=false;
	 VMProtectEnd();
 }


 //IG中DPass获取线程
 void IGDPassThread(void* p)
 {
	 VMProtectBegin("IGDPassThread");
	 CDlgISM_GBox* dlgIG=(CDlgISM_GBox*)(p);
	 CString serverid,strTemp,strTemp1,strRet;
	 int retry=10;
	 serverid=_T("");
	 bool bFind=false;
	 dlgIG->m_Progress.SetRange(0,100);
	 dlgIG->m_Progress.SetPos(0);
	 dlgIG->m_Progress.UpdateWindow();
	 byte IG_SSID[4];
	 for(int i=0;i<4;i++)
		 IG_SSID[i]=i;
	 while(retry!=0)
	 {
		 if(!dlgIG->m_bRunning)
			 break;
		 byte* pDealer=NULL;
		 // 		if(dlgIG->m_ModelType.GetCurSel()==0)//ism
		 // 		{
		 pDealer=&(dlgIG->IGData[0x80]);
		 //}
		 if(CCarKey3App::withServer.AddDPass(CCarKey3App::TheHIDDevice.HexID,pDealer,strRet,IG_SSID))
		 {
			 if(strRet==_T(""))
			 {
				 retry--;
				 continue;
			 }
			 CMySplitStr MySplit;
			 MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
			 MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
			 MySplit.SetData(strRet);//读取要分割的字符串
			 CStringArray strArray;//读取结果存放的数组
			 MySplit.GetSplitStrArray(strArray);
			 int retCode=_ttoi(strArray.GetAt(0));
			 if(retCode==1)
			 {
				 if(strArray.GetCount()==2)
				 {
					 serverid=strArray.GetAt(1);
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,1002);
					 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgIG->ShowLog(strTemp1);
					 break;
				 }
			 }
			 else if(retCode==2)
			 {
				 if(strArray.GetCount()==2)
				 {
					 CString strAnwser=strArray.GetAt(1);
					 CMySplitStr MySplit1;
					 MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
					 MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
					 MySplit1.SetData(strAnwser);//读取要分割的字符串
					 CStringArray strArray1;//读取结果存放的数组
					 MySplit1.GetSplitStrArray(strArray1);
					 int len;
					 byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
					 byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,IG_SSID);
					 for(int i=0;i<8;i++)
						 dlgIG->m_EZSData.DPSW[i]=pDSW[i];
					 dlgIG->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
					 strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
					 MessageBoxEx(dlgIG->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgIG->ShowLog(strTemp);
					 bFind=true;
					 dlgIG->m_Progress.SetPos(100);
					 dlgIG->m_Progress.UpdateWindow();
					 delete[] p;
					 delete[] pDSW;
					 TRACE(CCommFunc::byteToHexStr(dlgIG->m_EZSData.DPSW,8,_T("")));
					 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
					 dlgIG->SetDlgItemText(IDC_BUTTON4,strTemp);
					 dlgIG->m_bThreadExit=true;
					 dlgIG->m_bRunning=false;
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,1002);
					 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgIG->ShowLog(strTemp1);
					 break;
				 }
			 }
			 else if(retCode==100)
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip3,CCarKey3App::LANGID);				
				 MessageBoxEx(dlgIG->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgIG->ShowLog(strTemp);
				 break;
			 }
			 else if(retCode==-50)
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip25,CCarKey3App::LANGID);				
				 MessageBoxEx(dlgIG->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgIG->ShowLog(strTemp);
				 break;
			 }
			 else
			 {
				 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
				 strTemp1.Format(strTemp,retCode);
				 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
				 dlgIG->ShowLog(strTemp1);
				 break;
			 }								
		 }
		 else
			 retry--;
	 }
	 if(serverid!=_T("") || !bFind)
	 {
		 retry=10;
		 while(retry!=0)
		 {		
			 if(!dlgIG->m_bRunning)
				 break;
			 for(int i=1;i<11;i++)
			 {
				 Sleep(1000);
				 // 				dlgEIS->m_Progress.SetPos(i);
				 // 				dlgEIS->m_Progress.UpdateWindow();
			 }
			 if(CCarKey3App::withServer.FindDPass(CCarKey3App::TheHIDDevice.HexID,serverid,strRet,IG_SSID))
			 {
				 if(strRet==_T(""))
				 {
					 retry--;
					 continue;
				 }
				 CMySplitStr MySplit;
				 MySplit.SetSplitFlag(_T(","));//设定分隔符，默认值也是“，”
				 MySplit.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
				 MySplit.SetData(strRet);//读取要分割的字符串
				 CStringArray strArray;//读取结果存放的数组
				 MySplit.GetSplitStrArray(strArray);
				 int retCode=_ttoi(strArray.GetAt(0));
				 if(retCode==1)
				 {
					 if(strArray.GetCount()==2)
					 {
						 CString strAnwser=strArray.GetAt(1);
						 if(strAnwser==_T(""))
						 {
							 strTemp.LoadString(NULL,IDS_ServerTip4,CCarKey3App::LANGID);
							 MessageBoxEx(dlgIG->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
							 dlgIG->ShowLog(strTemp);
							 break;
						 }
						 else
						 {
							 CString strAnwser=strArray.GetAt(1);
							 CMySplitStr MySplit1;
							 MySplit1.SetSplitFlag(_T("-"));//设定分隔符，默认值也是“，”
							 MySplit1.SetSequenceAsOne(FALSE);//设定联系的分割符如何处理
							 MySplit1.SetData(strAnwser);//读取要分割的字符串
							 CStringArray strArray1;//读取结果存放的数组
							 MySplit1.GetSplitStrArray(strArray1);
							 int len;
							 byte* p=CCommFunc::strToHexByte(strArray1.GetAt(1),_T(""),&len);
							 byte* pDSW=CCommFunc::Encrypt(p,8,CCarKey3App::TheHIDDevice.HexID,IG_SSID);
							 for(int i=0;i<8;i++)
								 dlgIG->m_EZSData.DPSW[i]=pDSW[i];
							 dlgIG->GetDlgItem(IDC_EDIT19)->SetWindowText(_T("XX XX XX XX XX XX XX XX"));
							 strTemp.LoadString(NULL,IDS_ServerTip2,CCarKey3App::LANGID);
							 dlgIG->ShowLog(strTemp);
							 bFind=true;
							 dlgIG->m_Progress.SetPos(100);
							 dlgIG->m_Progress.UpdateWindow();
							 delete[] p;
							 delete[] pDSW;
							 TRACE(CCommFunc::byteToHexStr(dlgIG->m_EZSData.DPSW,8,_T("")));
							 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
							 dlgIG->SetDlgItemText(IDC_BUTTON4,strTemp);
							 dlgIG->m_bThreadExit=true;
							 dlgIG->m_bRunning=false;
							 break;
						 }
						 break;
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgIG->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==2)
				 {
					 if(strArray.GetCount()==2)
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip5,CCarKey3App::LANGID);
						 int xx=_ttoi(strArray.GetAt(1));
						 dlgIG->m_Progress.SetPos(xx);
						 dlgIG->m_Progress.UpdateWindow();
						 strTemp1.Format(strTemp,xx);
						 dlgIG->ShowLog(strTemp1);
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgIG->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==3)
				 {
					 if(strArray.GetCount()==2)
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip6,CCarKey3App::LANGID);
						 int xx=_ttoi(strArray.GetAt(1));
						 strTemp1.Format(strTemp,xx);
						 dlgIG->ShowLog(strTemp1);
					 }
					 else
					 {
						 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
						 strTemp1.Format(strTemp,1002);
						 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
						 dlgIG->ShowLog(strTemp1);
						 break;
					 }
				 }
				 else if(retCode==4)
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip7,CCarKey3App::LANGID);
					 MessageBoxEx(dlgIG->m_hWnd,strTemp,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgIG->ShowLog(strTemp);
					 break;
				 }
				 else
				 {
					 strTemp.LoadString(NULL,IDS_ServerTip1,CCarKey3App::LANGID);
					 strTemp1.Format(strTemp,retCode);
					 MessageBoxEx(dlgIG->m_hWnd,strTemp1,_T(""),MB_OK,CCarKey3App::LANGID);
					 dlgIG->ShowLog(strTemp);
					 break;
				 }
			 }
			 else
				 retry--;
		 }
		 // 		if(retry==0)
		 // 		{
		 // 			strTemp.LoadString(IDS_ServerTip10);
		 // 			MessageBoxEx(dlgEIS->m_hWnd,strTemp,_T(""),MB_OK,0);
		 // 			dlgEIS->ShowLog(strTemp);
		 // 		}
	 }
	 if(bFind)
	 {
		 strTemp.LoadString(NULL,IDS_EZSRenew,CCarKey3App::LANGID);
		 dlgIG->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);

	 }
	 else
	 {
		 strTemp.LoadString(NULL,IDS_RenewPassCalc,CCarKey3App::LANGID);
		 dlgIG->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	 }
	 dlgIG->SetDlgItemText(IDC_BUTTON4,strTemp);
	 dlgIG->EnableControl(TRUE);
	 dlgIG->m_bThreadExit=true;
	 dlgIG->m_bRunning=false;
	 VMProtectEnd();
 }