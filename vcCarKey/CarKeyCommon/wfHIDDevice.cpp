// HIDDevice.cpp : 实现文件
//

#include "stdafx.h"
#ifdef CarKey2
#include ".\..\CarKey2\CarKey2\resource.h"
#else
#include ".\..\CarKey3\CarKey3\resource.h"
#endif
#include "wfHIDDevice.h"
#include "InteractionData.h"
#include "CommFunc.h"
#include "..\CarKey3\CarKey3\CarKey3.h"
extern "C" {
	// This file is in the Windows DDK available from Microsoft.
#include "hidsdi.h"
#include "dbt.h"
#include <setupapi.h>
#include "PassCalc.h"
#include "..\CarKey3\CarKey3\InputPSW.h"
#include "VMProtectSDK.h"
}
// CwfHIDDevice

IMPLEMENT_DYNAMIC(CwfHIDDevice, CWnd)

CwfHIDDevice::CwfHIDDevice()
{
	MyDeviceDetected=FALSE;
	VendorID=0x03d8;
	ProductID=0x000d;//0x0729;
	m_pProgress=NULL;
}

CwfHIDDevice::~CwfHIDDevice()
{
}


BEGIN_MESSAGE_MAP(CwfHIDDevice, CWnd)
END_MESSAGE_MAP()


bool CwfHIDDevice::FindDevice()
{
	VMProtectBegin("HIDDevice1");
	if(CCarKey3App::LANGID==0x0804)
	{
		ProductID=0x0729;
		//ProductID=0x000d;
	}
	else
	{
		ProductID=0x000d;//0x0729;
	}
	GUID HidGuid;
	//取得类别
	HidD_GetHidGuid(&HidGuid);	
	//获取一个设备信息群，包含指定类的所有设备
	HANDLE hDevInfo;
	hDevInfo = SetupDiGetClassDevs(
		&HidGuid,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_INTERFACEDEVICE
		);
	SP_DEVICE_INTERFACE_DATA devInfoData;
	devInfoData.cbSize = sizeof(devInfoData);
	int MemberIndex = 0;
	bool LastDevice = FALSE;
	LONG Result;
	ULONG Length;
	PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;
	ULONG Required;
	HIDD_ATTRIBUTES Attributes;
	do
	{
		Result=SetupDiEnumDeviceInterfaces(
			hDevInfo,
			0,
			&HidGuid,
			MemberIndex,
			&devInfoData
			);

		if (Result != 0)
		{			
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				NULL, 
				0, 
				&Length, 
				NULL);

			detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
			detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			//获取信息群内一个设备的信息
			Result = SetupDiGetDeviceInterfaceDetail 
				(hDevInfo, 
				&devInfoData, 
				detailData, 
				Length, 
				&Required, 
				NULL);
			DeviceHandle=CreateFile 
				(detailData->DevicePath, 
				0, 
				FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING, 
				0, 
				NULL);
			Attributes.Size = sizeof(Attributes);

			//获取厂商与产品，版本号
			Result = HidD_GetAttributes 
				(DeviceHandle, 
				&Attributes
				);
			MyDeviceDetected = FALSE;

			if( (Attributes.VendorID == VendorID))// && (Attributes.ProductID == ProductID))
			{
				MyDeviceDetected = TRUE;
// 				USB路径代表它的名字，用于拔插设备时显示
// 								MyDevicePathName = detailData->DevicePath;	
// 				
// 								//Register to receive device notifications.
// 				
// 								RegisterForDeviceNotifications();
				PHIDP_PREPARSED_DATA	PreparsedData;	//struct
				//获取保存设备能力信息的缓冲器的句柄
				HidD_GetPreparsedData(DeviceHandle, &PreparsedData);
				HIDP_CAPS Capabilities;
				HidP_GetCaps(PreparsedData, &Capabilities);
				OutputReportByteLength=Capabilities.OutputReportByteLength;
				InputReportByteLength=Capabilities.InputReportByteLength;
				HidD_FreePreparsedData(PreparsedData);
				//DeviceUsage = (Capabilities.UsagePage * 0x100) + Capabilities.Usage;

				// Get a handle for writing Output reports.

				WriteHandle=CreateFile 
					(detailData->DevicePath, 
					GENERIC_WRITE, 
					FILE_SHARE_READ|FILE_SHARE_WRITE, 
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING, 
					0, 
					NULL);
				ReadHandle=CreateFile 
					(detailData->DevicePath, 
					GENERIC_READ, 
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					(LPSECURITY_ATTRIBUTES)NULL, 
					OPEN_EXISTING, 
					FILE_FLAG_OVERLAPPED, 
					NULL);
				if (hEventObject == NULL)	//0
				{
					hEventObject = CreateEvent 
						(NULL, 
						TRUE, 
						TRUE, 
						_T(""));
					HIDOverlapped.hEvent = hEventObject;
					HIDOverlapped.Offset = 0;
					HIDOverlapped.OffsetHigh = 0;
				}
			}
			else
			{
				CloseHandle(DeviceHandle);
				DeviceHandle=INVALID_HANDLE_VALUE;
			}
			free(detailData);
		} //if (Result != 0)
		else
		{			
			LastDevice=TRUE;
		}
		MemberIndex = MemberIndex + 1;
	}
	while ((LastDevice == FALSE) && (MyDeviceDetected == FALSE));
	if(MyDeviceDetected==true)
	{
		if(CheckDevice())
			return true;
		else
		{
			MyDeviceDetected=FALSE;
		}
	}
	return false;
	VMProtectEnd();
}

CInteractionData CwfHIDDevice::DeviceInteraction(CInteractionData tx)
{
	return DeviceInteraction(tx,40);
}

CInteractionData CwfHIDDevice::DeviceInteraction(CInteractionData tx,int n)
{
	DWORD	BytesWritten = 0;
	INT		Index =0;
	BOOL	Result=FALSE;
	DWORD NumberOfBytesRead;
	CInteractionData rx;
// 	try
// 	{
		if (WriteHandle != INVALID_HANDLE_VALUE)
		{
			byte* out=tx.GetDataList();
			Result = WriteFile(
				WriteHandle,
				out,
				OutputReportByteLength,
				&BytesWritten,
				NULL);
			delete[] out;
		}

		if (Result==FALSE)
		{
			CloseHandles();
			MyDeviceDetected = FALSE;
			return rx;
			//MessageBox("WriteFile Error","WriteFile",MB_OK);
		}
// 	}
// 	catch (CException* e)
// 	{
// 		throw _T("USBWirteError!!"+e->GetErrorMessage());
// 	}
	byte InputReport[256];
	if (ReadHandle != INVALID_HANDLE_VALUE)
	{
		Result = ReadFile 
		(ReadHandle, 
		InputReport, 
		InputReportByteLength, 
		&NumberOfBytesRead,
		(LPOVERLAPPED) &HIDOverlapped); 
	}
	if(m_pProgress==NULL)
	{
		Result = WaitForSingleObject(hEventObject,30000);
		switch (Result)
		{
		case WAIT_OBJECT_0:
			{
				rx.SetData(InputReport);
			}
			break;
		case WAIT_TIMEOUT:
			{			
				Result = CancelIo(ReadHandle);
				CloseHandles();
				MyDeviceDetected = FALSE;
				break;
			}
		default:
			{			
				CloseHandles();
				MyDeviceDetected = FALSE;
				break;
			}
		}
	}
	else
	{
		bool bOK=false;
		while(true)
		{
			Result = WaitForSingleObject(hEventObject,500);
			switch (Result)
			{
			case WAIT_OBJECT_0:
				{
					rx.SetData(InputReport);
					bOK=true;
				}
				break;
			case WAIT_TIMEOUT:
				{			
					
				}
				break;
			default:
				{			
					CloseHandles();
					MyDeviceDetected = FALSE;
					break;
				}
			}
			if(bOK)
			{
				break;
			}				
			n--;
			m_pProgress->SetPos(n);
			m_pProgress->UpdateWindow();
			if(n==0)
			{				
				break;
			}
		}
		m_pProgress->SetPos(0);
		if(!bOK)
		{
			Result = CancelIo(ReadHandle);
			CloseHandles();
			MyDeviceDetected = FALSE;
		}
	}
	ResetEvent(hEventObject);
	return rx;
}

void CwfHIDDevice::CloseHandles()
{
	//Close open handles.
	if (DeviceHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(DeviceHandle);
		DeviceHandle=INVALID_HANDLE_VALUE;
	}

	if (ReadHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(ReadHandle);
		ReadHandle=INVALID_HANDLE_VALUE;
	}

	if (WriteHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(WriteHandle);
		WriteHandle=INVALID_HANDLE_VALUE;
	}
}

HANDLE CwfHIDDevice::RegisterForUsbEvents(HANDLE hWnd)
{
	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;  

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);  
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE; 
	GUID gHid;
	HidD_GetHidGuid(&gHid);
	DevBroadcastDeviceInterface.dbcc_classguid = gHid;  
	return RegisterDeviceNotification(hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
}

BOOL CwfHIDDevice::UnregisterForUsbEvents(HANDLE hHandle)
{
	return UnregisterDeviceNotification(hHandle);
}

void CwfHIDDevice::Close()
{
	MyDeviceDetected=false;
	CloseHandles();
}
const byte B[4]={0x2E,0x74,0x5C,0x3A};
void CwfHIDDevice::DECRYPT()
{
	VMProtectBegin("HIDDevice2");
	int CNT0 = 0;
	byte j = 5;
	byte i;
	byte MASK;
	byte tmp_K;
	tmp_K = 0;
	while (CNT0 < 528) //0x210
	{
		if ((CNT0 & 0x0007) == 0)
		{
			if (j == 7)
			{
				j = 0;
			}
			else
			{
				j++;
			}
			tmp_K = KEY[j];
		}

		if ((HOP[1] & 0x08) == 0x08)
		{
			MASK = 0x10;
		}
		else
		{
			MASK = 0x01;
		}
		if ((HOP[2] & 0x01) == 0x01)
		{
			MASK = (byte)(MASK << 2);
		}
		if ((HOP[3] & 0x01) == 0x01)
		{
			MASK = (byte)(MASK << 1);
		}

		if ((HOP[0] & 0x02) == 0x02)
		{
			i = 0x01;
		}
		else
		{
			i = 0x00;
		}

		if ((HOP[0] & 0x40) == 0x40)
		{
			i = (byte)(i + 0x02);
		}

		if ((MASK & B[i]) == 0)
		{
			MASK = 0x00;
		}
		else
		{
			MASK = 0x80;
		}
		MASK = (byte)(MASK ^ HOP[2] ^ HOP[0] ^ tmp_K);

		HOP[0] = (byte)(HOP[0] << 1);
		if ((HOP[1] & 0x80) == 0x80)
		{
			HOP[0] = (byte)(HOP[0] | 0x01);
		}
		else
		{
			HOP[0] = (byte)(HOP[0] & 0xFE);
		}

		HOP[1] = (byte)(HOP[1] << 1);
		if ((HOP[2] & 0x80) == 0x80)
		{
			HOP[1] = (byte)(HOP[1] | 0x01);
		}
		else
		{
			HOP[1] = (byte)(HOP[1] & 0xFE);
		}

		HOP[2] = (byte)(HOP[2] << 1);
		if ((HOP[3] & 0x80) == 0x80)
		{
			HOP[2] = (byte)(HOP[2] | 0x01);
		}
		else
		{
			HOP[2] = (byte)(HOP[2] & 0xFE);
		}

		HOP[3] = (byte)(HOP[3] << 1);
		if ((MASK & 0x80) == 0x80)
		{
			HOP[3] = (byte)(HOP[3] | 0x01);
		}
		else
		{
			HOP[3] = (byte)(HOP[3] & 0xFE);
		}

		tmp_K = (byte)(tmp_K << 1);
		CNT0++;
	}
	VMProtectEnd();
}

void CwfHIDDevice::ENCODE()
{
	VMProtectBegin("HIDDevice3");
	int CNT0 = 0;
	byte j = 0;
	byte i;
	byte MASK;
	byte tmp_K;
	tmp_K = 0;
	while (CNT0 < 528) //0x210
	{
		if ((CNT0 & 0x0007) == 0)
		{
			if (j == 0)
			{
				j = 7;
			}
			else
			{
				j--;
			}
			tmp_K = KEY[j];
		}

		if ((HOP[1] & 0x10) == 0x10)
		{
			MASK = 0x10;
		}
		else
		{
			MASK = 0x01;
		}
		if ((HOP[2] & 0x02) == 0x02)
		{
			MASK = (byte)(MASK << 2);
		}
		if ((HOP[3] & 0x02) == 0x02)
		{
			MASK = (byte)(MASK << 1);
		}

		if ((HOP[0] & 0x04) == 0x04)
		{
			i = 0x01;
		}
		else
		{
			i = 0x00;
		}

		if ((HOP[0] & 0x80) == 0x80)
		{
			i = (byte)(i + 0x02);
		}

		if ((MASK & B[i]) == 0)
		{
			MASK = 0x00;
		}
		else
		{
			MASK = 0x01;
		}

		MASK = (byte)(MASK ^ HOP[1] ^ HOP[3] ^ tmp_K);

		HOP[3] = (byte)(HOP[3] >> 1);
		if ((HOP[2] & 0x01) == 0x01)
		{
			HOP[3] = (byte)(HOP[3] | 0x80);
		}
		else
		{
			HOP[3] = (byte)(HOP[3] & 0x7F);
		}

		HOP[2] = (byte)(HOP[2] >> 1);
		if ((HOP[1] & 0x01) == 0x01)
		{
			HOP[2] = (byte)(HOP[2] | 0x80);
		}
		else
		{
			HOP[2] = (byte)(HOP[2] & 0x7F);
		}

		HOP[1] = (byte)(HOP[1] >> 1);
		if ((HOP[0] & 0x01) == 0x01)
		{
			HOP[1] = (byte)(HOP[1] | 0x80);
		}
		else
		{
			HOP[1] = (byte)(HOP[1] & 0x7F);
		}

		HOP[0] = (byte)(HOP[0] >> 1);
		if ((MASK & 0x01) == 0x01)
		{
			HOP[0] = (byte)(HOP[0] | 0x80);
		}
		else
		{
			HOP[0] = (byte)(HOP[0] & 0x7F);
		}

		tmp_K = (byte)(tmp_K >> 1);
		CNT0++;
	}
	VMProtectEnd();
}

bool CwfHIDDevice::CheckDevice()
{
	VMProtectBegin("HIDDevice4");
	CInteractionData tx;

	tx.buff[0] = 0x03;
	tx.buff[1] = 0x55;	//主机-->设备
	CInteractionData rx = DeviceInteraction(tx);
	if (!rx.bOK || rx.CheckDataList() == false)
		return false;
	HexID[0] = rx.buff[0x10];
	HexID[1] = rx.buff[0x11];
	HexID[2] = rx.buff[0x12];
	HexID[3] = rx.buff[0x13];
// 	if (HexID[0] != 0x05 || HexID[1] != 0x1c || HexID[2] != 0xda || HexID[3] != 0x1b)
// 		return false;
	tx.Init();
	rx = DeviceInteraction(tx);
	if (!rx.bOK || rx.CheckDataList() == false)
		return false;
	HexVer[0] = rx.buff[0x10];
	HexVer[1] = rx.buff[0x11];
// 	if(HexVer[0]<0x20)
// 		return false;
	return true;
	VMProtectEnd();
}

CInteractionData CwfHIDDevice::Check_mima()
{
	VMProtectBegin("HIDDevice5");
	CInteractionData tx;

	tx.buff[0] = 0x03;
	tx.buff[1] = 0x55;	//主机-->设备
	CInteractionData rx = DeviceInteraction(tx);
	if (!rx.bOK)
	{
		rx.hidResult= HIDResult_USBError;
		return rx;
	}
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
		return rx;
	}
	KEY[0] = 0x02;
	KEY[1] = 0x58;
	KEY[2] = 0xBE;
	KEY[3] = 0x24;
	//             KEY[4] = 0x8A;
	//             KEY[5] = 0x95;
	//             KEY[6] = 0x60;
	//             KEY[7] = 0x3D;

	KEY[4] = (byte)(HexID[0] - 0x77);
	KEY[5] = (byte)(HexID[1] - 0x55);
	KEY[6] = (byte)(HexID[2] - 0x33);
	KEY[7] = (byte)(HexID[3] - 0x11);

	HOP[0] = rx.buff[0x14];
	HOP[1] = rx.buff[0x15];
	HOP[2] = rx.buff[0x16];
	HOP[3] = rx.buff[0x17];

	DECRYPT();
	if ((HOP[0x02] == rx.buff[0x18]) && (HOP[3] == rx.buff[0x19]))
		rx.mimaok=true;
	else
	{
		rx.mimaok=false;
		rx.hidResult=HIDResult_MimaErr;
	}
	return rx;
	VMProtectEnd();
}

CInteractionData CwfHIDDevice::ReadKeyData(byte type)
{
	CInteractionData tx;
	CInteractionData rx;
	tx.buff[0] = type;
	tx.buff[1] = 0x55;	//主机-->设备 
	rx = DeviceInteraction(tx);
	if (!rx.bOK)
		rx.hidResult= HIDResult_USBError;
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
	}
	else if (rx.buff[1] == 0x00)
	{
		rx.hidResult= HIDResult_OK;
	}
	else
	{
		rx.hidResult= HIDResult_ReadError;
	}
	return rx;
}

CInteractionData CwfHIDDevice::ReadPSW1(byte type)
{
	CInteractionData tx;
	CInteractionData rx;
	tx.buff[0] = type;
	tx.buff[1] = 0x55;	//主机-->设备 
	rx = DeviceInteraction(tx);
	if (!rx.bOK)
		rx.hidResult= HIDResult_USBError;
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
	}
	else if (rx.buff[1] == 0x03)
	{
		rx.hidResult= HIDResult_GetPSWError3;
	}
	else if (rx.buff[1] == 0x00)
	{
		rx.hidResult= HIDResult_OK;
	}
	else
	{
		rx.hidResult= HIDResult_GetPSWError1;
	}
	return rx;
}

CInteractionData CwfHIDDevice::ReadPSW2()
{
	CInteractionData tx;
	CInteractionData rx;
	tx.buff[0] = 0x01;
	tx.buff[1] = 0xaa;	//主机<--设备
	tx.buff[4] = 0x20;
	rx = DeviceInteraction(tx);
	if (!rx.bOK)
		rx.hidResult= HIDResult_USBError;
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
	}
	else if (rx.buff[1] != 0x01)
	{
		rx.hidResult= HIDResult_GetPSWError2;		
	}
	else
	{
		rx.hidResult= HIDResult_OK;
	}
	return rx;
}

CInteractionData CwfHIDDevice::WriteKeyData1(int step)
{
	CInteractionData tx;
	CInteractionData rx;
	tx.buff[0] = 0x01;
	tx.buff[1] = 0x55;	//主机-->设备
	int len=(step+1)*0x30;
	if(len<0xa0)
	{
		tx.buff[4] = 0x30;//本次传送数据的长度；
	}
	else
	{
		tx.buff[4]=0xa0-step*0x30;
	}
	tx.buff[5] += step*0x30;
	for (int i = 0; i < tx.buff[4]; i++)
	{
		tx.buff[i + 0x10] = KeyDataBuf[tx.buff[5]+i];
	}
	rx = DeviceInteraction(tx);
	if (!rx.bOK)
		rx.hidResult= HIDResult_USBError;
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
	}
	else if (rx.buff[1] == 0x00)
	{
		rx.hidResult= HIDResult_OK;
	}
	else
	{
		rx.hidResult= HIDResult_ReadError;
	}
	return rx;
}

CInteractionData CwfHIDDevice::WriteKeyData2(byte type)
{
	CInteractionData tx;
	CInteractionData rx;
	tx.buff[0] = type;
	tx.buff[1] = 0x55;	//主机-->设备 
	rx = DeviceInteraction(tx);
	if (!rx.bOK)
		rx.hidResult= HIDResult_USBError;
	else if (rx.CheckDataList() == false)
	{
		rx.hidResult= HIDResult_RxError;
	}
	else if (rx.buff[1] == 0x00)
	{
		rx.hidResult= HIDResult_OK;
	}
	else
	{
		rx.hidResult= HIDResult_ReadError;
	}
	return rx;
}
// CwfHIDDevice 消息处理程序


