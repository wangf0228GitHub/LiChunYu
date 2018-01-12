
// wfHashCalcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wfHashCalc.h"
#include "wfHashCalcDlg.h"
#include "afxdialogex.h"
#include "..\CarKeyCommon\PassCalc.h"
#include "..\CarKeyCommon\CommFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
bool CwfHashCalcDlg::CheckHPass(CString filePath,byte* answer,byte* query,byte* psw)
{
	CString strOut=_T("");
	CPassCalc cp;
	for(int i=0;i<8;i++)
	{
		cp.HashIn[i]=answer[i];
	}
	cp.sub_HashOnce();
	for(int i=0;i<8;i++)
	{
		if(query[i]!=cp.HashOut[i])
		{
			return false;
		}
	}
	CFile file(filePath,CFile::modeRead);
	CString str,strTemp,strTemp1,strInfo;
	//StrName=fileDlg.GetPathName();			//file path
	ULONGLONG dwFileLen=file.GetLength();
	byte* pBuf=new byte[dwFileLen+1];
	pBuf[dwFileLen]=0;
	file.Read(pBuf,dwFileLen);
	file.Close();
	if(dwFileLen==(1024*20*4+8))
	{		
		byte dID[4],sID[4];
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+8,1024*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode);
		byte FileA[0x400*0x08];
		for(int j=0;j<0x400;j++)
		{
			for(int i=0;i<8;i++)
			{
				FileA[0x08*j+i]=pData[0x40*j+0x30+i]^pData[0x40*j+0x21+i];
			}
		}
		
		for(int j=0;j<0x400;j++)
		{	
			bool bSame=true;
			for(int i=0;i<8;i++)
			{
				if(FileA[0x08*j+i]!=query[i])
				{			
					bSame=false;
					break;
				}
			}
			if(bSame)
			{
				//---
				byte TPA[0x08],TPB[0x08],TPC[0x08],TPD[0x08],TPE[0x08];
				for(int i=0;i<0x08;i++)
				{
					TPA[i]=answer[i];
				}
				for(int i=0;i<0x08;i++)
				{
					TPB[i]=pData[0x40*j+0x11+i];
				}
				TPC[0]=0;
				TPC[1]=0;
				TPC[2]=0;
				TPC[3]=0;
				TPC[4]=pData[0x40*j+0x01];
				TPC[5]=pData[0x40*j+0x02];
				TPC[6]=pData[0x40*j+0x03];
				TPC[7]=pData[0x40*j+0x00];
				//---				
				for(int i=0;i<0x08;i++)
				{
					psw[i]=TPA[i]^TPB[i]^TPC[i];
				}
				int err=0;
				byte Ftmp[0x40];
				for(int j=0;j<0x400;j++)
				{
					for(int i=0;i<0x40;i++)
					{
						Ftmp[i]=pData[0x40*j+i];//取出一组数据到暂存
					}
					for(int i=0;i<0x08;i++)
					{
						TPA[i]=Ftmp[0x11+i];
						TPB[i]=Ftmp[0x30+i];
						TPE[i]=Ftmp[0x21+i];//最终待比较结果
					}
					TPC[0x00]=0x00;
					TPC[0x01]=0x00;
					TPC[0x02]=0x00;
					TPC[0x03]=0x00;
					TPC[0x04]=Ftmp[0x01];
					TPC[0x05]=Ftmp[0x02];
					TPC[0x06]=Ftmp[0x03];
					TPC[0x07]=Ftmp[0x00];
					//
					//---
					CPassCalc cp1;
					for(int i=0;i<0x08;i++)
					{
						cp1.HashIn[i]=TPA[i]^psw[i]^TPC[i];
					}
					cp1.sub_HashOnce();
					for(int i=0;i<0x08;i++)
					{
						TPD[i]=cp1.HashOut[i]^TPB[i];
					}
					//
					bool fCHK=true;
					for(int i=0;i<0x08;i++)
					{
						if(TPD[i]!=TPE[i])
						{
							fCHK=false;
							break;
						}
					}
					if(false==fCHK)
					{
						CString xxx;
						xxx.Format(_T("%d\r\n"),j);
						strOut+=xxx;
						err++;
						if(err>50)
						{
							delete[] pBuf;
							delete[] pDecode;
							delete[] pData;
							GetDlgItem(IDC_EDIT4)->SetWindowText(strOut);
							return false;
						}
					}
				}
				delete[] pBuf;
				delete[] pDecode;
				delete[] pData;
				GetDlgItem(IDC_EDIT4)->SetWindowText(strOut);
				return true;
			}
		}
		delete[] pBuf;
		delete[] pDecode;
		delete[] pData;
		return false;
	}
	else if(dwFileLen==(2048*20*4+8))
	{		
		byte dID[4],sID[4];
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+8,2048*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode,true);
		byte FileA[0x800*0x08];
		byte TMa[0x08],TMb[0x08];
		byte TPA[0x08],TPB[0x08],TPC[0x08],TPD[0x08],TPE[0x08];
		for(int j=0;j<0x400;j++)
		{			
			for(int i=0;i<8;i++)
			{
				TMa[i]=pData[0x80*j+0x21+i];
			}
			for(int i=0;i<5;i++)
			{
				TMb[i]=pData[0x80*j+0x35+i];
			}
			for(int i=0;i<3;i++)
			{
				TMb[0x05+i]=pData[0x80*j+0x52+i];
			}
			for(int i=0;i<8;i++)
			{
				FileA[0x10*j+0x00+i]=TMa[i]^TMb[i];
			}
			for(int i=0;i<8;i++)
			{
				FileA[0x10*j+0x08+i]=pData[0x80*j+0x72+i];
			}
		}
		bool bSame=true;
		for(int j=0;j<0x800;j++)
		{
			bSame=true;
			for(int i=0;i<8;i++)
			{
				if(FileA[0x08*j+i]!=query[i])
				{			
					bSame=false;
					break;
				}
			}
			if(bSame)
			{
				byte FT[0x80];
				for(int i=0;i<0x80;i++)
				{
					FT[i]=pData[0x80*(j/2)+i];
				}
				//
				for(int i=0;i<0x08;i++)
				{
					TPA[i]=answer[i];
				}
				//
				if(0x00==j%2)
				{
					for(int i=0;i<0x02;i++)
					{
						TPB[i]=FT[i+0x00];
					}
					for(int i=0;i<0x06;i++)
					{
						TPB[i+0x02]=FT[i+0x11];
					}
					for(int i=0;i<0x08;i++)
					{
						TPC[i]=TPB[i]^TPA[i];
					}
					//
					for(int i=0;i<0x04;i++)
					{
						psw[i]=TPC[i+0x04];
					}
					for(int i=0;i<0x04;i++)
					{
						psw[i+0x04]=TPC[i];
					}
				}
				else
				{
					for(int i=0;i<0x07;i++)
					{
						TPB[i]=FT[i+0x33];
					}
					TPB[0x07]=FT[0x52];
					//
					for(int i=0;i<0x08;i++)
					{
						TPC[i]=TPB[i]^TPA[i];
					}
					//
					for(int i=0;i<0x03;i++)
					{
						psw[i]=TPC[i+0x05];
					}
					for(int i=0;i<0x05;i++)
					{
						psw[i+0x03]=TPC[i];
					}
				}
				break;				
			}
		}
		if(bSame)
		{
			byte Ftmp[0x80];
			int err=0;
			for(int j=0;j<0x400;j++)
			{
				for(int i=0;i<0x80;i++)
				{
					Ftmp[i]=pData[0x80*j+i];//取出一组数据到暂存
				}
				for(int i=0;i<0x07;i++)
				{
					TPA[i]=Ftmp[0x33+i];
				}
				TPA[0x07]=Ftmp[0x52];
				//
				TPB[0x00]=psw[0x03];
				TPB[0x01]=psw[0x04];
				TPB[0x02]=psw[0x05];
				TPB[0x03]=psw[0x06];
				TPB[0x04]=psw[0x07];
				TPB[0x05]=psw[0x00];
				TPB[0x06]=psw[0x01];
				TPB[0x07]=psw[0x02];
				//
				for(int i=0;i<0x08;i++)
				{
					TPC[i]=Ftmp[0x72+i];//最终待比较结果
				}
				//---
				CPassCalc cp1;
				for(int i=0;i<0x08;i++)
				{
					cp1.HashIn[i]=TPA[i]^TPB[i];
				}
				cp1.sub_HashOnce();
				for(int i=0;i<0x08;i++)
				{
					TPD[i]=cp1.HashOut[i];
				}
				//
				bool fCHK=true;
				for(int i=0;i<0x08;i++)
				{
					if(TPD[i]!=TPC[i])
					{
						fCHK=false;
					}
				}
				if(false==fCHK)
				{
					CString xxx;
					xxx.Format(_T("%d\r\n"),j);
					strOut+=xxx;
					TRACE(xxx);
					err++;
					if(err>50)
					{
						delete[] pBuf;
						delete[] pDecode;
						delete[] pData;
						GetDlgItem(IDC_EDIT4)->SetWindowText(strOut);
						return false;
					}
				}			
			}
			delete[] pBuf;
			delete[] pDecode;
			delete[] pData;
			GetDlgItem(IDC_EDIT4)->SetWindowText(strOut);
			return true;
		}
	}

	return false;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwfHashCalcDlg 对话框




CwfHashCalcDlg::CwfHashCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwfHashCalcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwfHashCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CwfHashCalcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CwfHashCalcDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CwfHashCalcDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CwfHashCalcDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CwfHashCalcDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CwfHashCalcDlg 消息处理程序

BOOL CwfHashCalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("56 35 A3 57 86 EE 8F 90"));
	GetDlgItem(IDC_EDIT2)->SetWindowText(_T("EB D2 4B 7A 13 24 23 C3"));
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CwfHashCalcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwfHashCalcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwfHashCalcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwfHashCalcDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE);
	CString strTemp,strTemp1;
	fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	if (IDOK!=fileDlg.DoModal())
		return;
	CFile file(fileDlg.GetPathName(),CFile::modeRead);
	//StrName=fileDlg.GetPathName();			//file path
	ULONGLONG dwFileLen=file.GetLength();
	byte* pBuf=new byte[dwFileLen+1];
	pBuf[dwFileLen]=0;
	file.Read(pBuf,dwFileLen);
	file.Close();
	if(dwFileLen==(1024*20*4+8))
	{		
		byte dID[4],sID[4];
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+8,1024*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode);
		byte tx[0x400*0x08];
		for(int j=0;j<1024;j++)
		{
			for(int i=0;i<8;i++)
			{
				tx[0x08*j+i]=pData[0x40*j+0x30+i]^pData[0x40*j+0x21+i];
			}
		}
		delete[] pBuf;
		delete[] pDecode;
		delete[] pData;
		CString strW=_T("");
		for(int i=0;i<1023;i++)
		{
			strW=strW+CCommFunc::byteToHexStr(tx,i*8,8,_T(" "));
			strW+=_T(" \r\n");
		}
		strW=strW+CCommFunc::byteToHexStr(tx,1023*8,8,_T(" "))+_T(" ");
		CFileDialog fileDlgS(FALSE);
		fileDlgS.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
		fileDlgS.m_ofn.lpstrDefExt=_T("txt");
		if (IDOK==fileDlgS.DoModal())
		{
			CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
			int len;			
			char* pLog=CCommFunc::CStringToChar(strW,&len);
			file.Write(pLog,len*sizeof(CHAR));
			file.Close();
			delete[] pLog;
		}
	}
	else
	{
		MessageBox(_T("文件大小有误"));
	}
}


void CwfHashCalcDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE);
	CString strTemp,strTemp1;
	fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	if (IDOK!=fileDlg.DoModal())
		return;
	CFile file(fileDlg.GetPathName(),CFile::modeRead);
	//StrName=fileDlg.GetPathName();			//file path
	ULONGLONG dwFileLen=file.GetLength();
	byte* pBuf=new byte[dwFileLen+1];
	pBuf[dwFileLen]=0;
	file.Read(pBuf,dwFileLen);
	file.Close();
	if(dwFileLen==(2048*20*4+8))
	{		
		byte dID[4],sID[4];
		for(int i=0;i<4;i++)
		{
			dID[i]=pBuf[i];
			sID[i]=pBuf[i+4];
		}
		byte* pDecode=CCommFunc::Encrypt(pBuf+8,2048*20*4,dID,sID);
		byte* pData=CCommFunc::PSWDecrypt(pDecode,true);
// 		CFileDialog fileDlgS1(FALSE);
// 		fileDlgS1.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
// 		fileDlgS1.m_ofn.lpstrDefExt=_T("BIN");
// 		if (IDOK==fileDlgS1.DoModal())
// 		{
// 			CFile file(fileDlgS1.GetPathName(),CFile::modeCreate|CFile::modeWrite);
// 			file.Write(pData,2048*16*4);
// 			file.Close();
// 		}
		byte tx[0x800*0x08];
		byte TMa[0x08],TMb[0x08];
		for(int j=0;j<0x400;j++)
		{
			for(int i=0;i<8;i++)
			{
				TMa[i]=pData[0x80*j+0x21+i];
			}
			for(int i=0;i<5;i++)
			{
				TMb[i]=pData[0x80*j+0x35+i];
			}
			for(int i=0;i<3;i++)
			{
				TMb[0x05+i]=pData[0x80*j+0x52+i];
			}
			for(int i=0;i<8;i++)
			{
				tx[0x10*j+0x00+i]=TMa[i]^TMb[i];
			}
			for(int i=0;i<8;i++)
			{
				tx[0x10*j+0x08+i]=pData[0x80*j+0x72+i];
			}
		}
		delete[] pBuf;
		delete[] pDecode;
		delete[] pData;
		CString strW=_T("");
		for(int i=0;i<2047;i++)
		{
			strW=strW+CCommFunc::byteToHexStr(tx,i*8,8,_T(" "));
			strW+=_T(" \r\n");
		}
		strW=strW+CCommFunc::byteToHexStr(tx,2047*8,8,_T(" "))+_T(" ");
		CFileDialog fileDlgS(FALSE);
		fileDlgS.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
		fileDlgS.m_ofn.lpstrDefExt=_T("txt");
		if (IDOK==fileDlgS.DoModal())
		{
			CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
			int len;			
			char* pLog=CCommFunc::CStringToChar(strW,&len);
			file.Write(pLog,len*sizeof(CHAR));
			file.Close();
			delete[] pLog;
		}
	}
	else
	{
		MessageBox(_T("文件大小有误"));
	}
}


void CwfHashCalcDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strAnwser,strQuery,strPSW;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strAnwser);
	GetDlgItem(IDC_EDIT2)->GetWindowText(strQuery);
	int len;
	byte* anwser=CCommFunc::strToHexByte(strAnwser,_T(" "),&len);
	if(len!=8)
	{
		MessageBox(_T("H-1输入有误"));
	}
	byte* query=CCommFunc::strToHexByte(strQuery,_T(" "),&len);
	if(len!=8)
	{
		MessageBox(_T("Data输入有误"));
	}
	CFileDialog fileDlg(TRUE);
	CString strTemp,strTemp1;
	fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	if (IDOK!=fileDlg.DoModal())
		return;
	byte psw[8];
	CString str=fileDlg.GetPathName();
	if(CheckHPass(str,anwser,query,psw))
	{
		GetDlgItem(IDC_EDIT3)->SetWindowText(CCommFunc::byteToHexStr(psw,8,_T(" ")));
	}
	////StrName=fileDlg.GetPathName();			//file path
	//ULONGLONG dwFileLen=file.GetLength();
	//byte* pBuf=new byte[dwFileLen+1];
	//pBuf[dwFileLen]=0;
	//file.Read(pBuf,dwFileLen);
	//file.Close();
	//if(dwFileLen==(1024*20*4+8))
	//{		
	//	byte dID[4],sID[4];
	//	for(int i=0;i<4;i++)
	//	{
	//		dID[i]=pBuf[i];
	//		sID[i]=pBuf[i+4];
	//	}
	//	byte* pDecode=CCommFunc::Encrypt(pBuf+8,1024*20*4,dID,sID);
	//	byte* pData=CCommFunc::PSWDecrypt(pDecode);
	//	byte tx[0x400*0x08];
	//	for(int j=0;j<1024;j++)
	//	{
	//		for(int i=0;i<8;i++)
	//		{
	//			tx[0x08*j+i]=pData[0x40*j+0x30+i]^pData[0x40*j+0x21+i];
	//		}
	//	}
	//	delete[] pBuf;
	//	delete[] pDecode;
	//	delete[] pData;
	//	CString strW=_T("");
	//	for(int i=0;i<1023;i++)
	//	{
	//		strW=strW+CCommFunc::byteToHexStr(tx,i*8,8,_T(" "));
	//		strW+=_T(" \r\n");
	//	}
	//	strW=strW+CCommFunc::byteToHexStr(tx,1023*8,8,_T(" "))+_T(" ");
	//	CFileDialog fileDlgS(FALSE);
	//	fileDlgS.m_ofn.lpstrFilter=_T("Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0");
	//	fileDlgS.m_ofn.lpstrDefExt=_T("txt");
	//	if (IDOK==fileDlgS.DoModal())
	//	{
	//		CFile file(fileDlgS.GetPathName(),CFile::modeCreate|CFile::modeWrite);
	//		int len;			
	//		char* pLog=CCommFunc::CStringToChar(strW,&len);
	//		file.Write(pLog,len*sizeof(CHAR));
	//		file.Close();
	//		delete[] pLog;
	//	}
	//}
	//else
	//{
	//	MessageBox(_T("文件大小有误"));
	//}
}


void CwfHashCalcDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strAnwser,strQuery,strPSW;
	GetDlgItem(IDC_EDIT4)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT1)->GetWindowText(strAnwser);
	GetDlgItem(IDC_EDIT2)->GetWindowText(strQuery);
	int len;
	byte* anwser=CCommFunc::strToHexByte(strAnwser,_T(" "),&len);
	if(len!=8)
	{
		MessageBox(_T("H-1输入有误"));
	}
	byte* query=CCommFunc::strToHexByte(strQuery,_T(" "),&len);
	if(len!=8)
	{
		MessageBox(_T("Data输入有误"));
	}
	CFileDialog fileDlg(TRUE);
	CString strTemp,strTemp1;
	fileDlg.m_ofn.lpstrFilter=_T("BIN Files(*.BIN)\0*.BIN\0All Files(*.*)\0*.*");
	if (IDOK!=fileDlg.DoModal())
		return;
	byte psw[8];
	CString str=fileDlg.GetPathName();
	if(CheckHPass(str,anwser,query,psw))
	{
		CString str=CCommFunc::byteToHexStr(psw,8,_T(" "));
		GetDlgItem(IDC_EDIT3)->SetWindowText(str);
	}
	else
	{
		MessageBox(_T("没有找到密码"));
	}
}
