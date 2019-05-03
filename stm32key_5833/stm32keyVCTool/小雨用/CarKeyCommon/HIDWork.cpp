#include "stdafx.h"
#include "HIDWork.h"
#include "CommFunc.h"
#include "InteractionData.h"
#include "PassCalc.h"
#include ".\..\CarKey3\CarKey3\resource.h"
#include "..\CarKey3\CarKey3\CarKey3.h"
#include "VMProtectSDK.h"

CHIDWork::CHIDWork(void)
{
}


CHIDWork::~CHIDWork(void)
{
}
void CHIDWork::KeyData_UartRenew(CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/)
{
	VMProtectBegin("KeyData_UartRenew");
	// TODO: Add your control notification handler code here
	CInteractionData tx;

	tx.buff[0] = 0x12;	
	pProgress->SetRange(0,1);
	pProgress->SetPos(0);
	pResult->hidResult=HIDResult_OK;
	CInteractionData rx = DeviceInteraction(tx);
	if (!rx.bOK || rx.CheckDataList() == false)
	{
		pResult->hidResult=HIDResult_RenewToo;
	}
	pProgress->SetPos(1);
	pProgress->UpdateWindow();
	//------------------
	VMProtectEnd();
}
void CHIDWork::KeyData_UartProgram(CProgressCtrl* pProgress,CInteractionData* pResult,byte* pBuf/*,CStatusBarCtrl* pStatusBar*/)
{
	VMProtectBegin("KeyData_UartProgram");
	// TODO: Add your control notification handler code here
	CInteractionData tx;
	pResult->hidResult=HIDResult_OK;
	tx.buff[0] = 0x11;	
	pProgress->SetRange(0,6);
	pProgress->SetPos(0);
	for(int i=0;i<6;i++)
	{
		tx.buff[1]=i;
		for(int j=0;j<16;j++)
			tx.buff[2+j]=pBuf[i*16+j];
		CInteractionData rx = DeviceInteraction(tx);
		if (!rx.bOK || rx.CheckDataList() == false)
		{
			if(rx.buff[1]==0x03)
				pResult->hidResult=HIDResult_USED;
			else
				pResult->hidResult=HIDResult_WriteError;
			return;
		}
		pProgress->SetPos(i+1);
		pProgress->UpdateWindow();
	}
	//------------------
	VMProtectEnd();
}
byte* CHIDWork::KeyData_UartReadIR(CProgressCtrl* pProgress,CInteractionData* pResult/*,CStatusBarCtrl* pStatusBar*/)
{
	VMProtectBegin("HIDWork_UartReadIR");
	// TODO: Add your control notification handler code here
	byte* pBuf=new byte[256];
	CInteractionData tx;
	pResult->hidResult=HIDResult_OK;
	tx.buff[0] = 0x10;	
	pProgress->SetRange(0,16);
	pProgress->SetPos(0);
	for(int i=0;i<16;i++)
	{
		tx.buff[1]=i;
		CInteractionData rx = DeviceInteraction(tx);
		if (!rx.bOK || rx.CheckDataList() == false)
		{
			pResult->hidResult=HIDResult_ReadError;
			return NULL;
		}
		pProgress->SetPos(i+1);
		pProgress->UpdateWindow();
		for(int j=0;j<16;j++)
			pBuf[i*16+j]=rx.buff[2+j];
	}
	return pBuf;
	//------------------
	VMProtectEnd();
}

void CHIDWork::KeyData_SetRenewTimes(int n,CInteractionData* pResult)
{
	VMProtectBegin("KeyData_SetRenewTimes");
	// TODO: Add your control notification handler code here
	byte pp[8]={0x05,0x55,0x92,0x38,0x47,0x96,0xad,0xe7};
	CInteractionData tx;
	pResult->hidResult=HIDResult_OK;
	tx.buff[0] = 0x13;	
	CInteractionData rx = DeviceInteraction(tx);
	if (!rx.bOK || rx.CheckDataList() == false)
	{
		pResult->hidResult=HIDResult_ReadError;
		return;
	}
	CPassCalc hashCalc;
	for(int i=0;i<8;i++)
		hashCalc.HashIn[i]=rx.buff[2+i]^pp[i];
	hashCalc.sub_HashCalc(n);
	tx.buff[0]=0x14;
	for(int i=0;i<8;i++)
		tx.buff[1+i]=hashCalc.HashOut[i]^pp[i];
	rx = DeviceInteraction(tx);
	if (!rx.bOK || rx.CheckDataList() == false)
	{
		pResult->hidResult=HIDResult_ReadError;
		return;
	}
	//------------------
	VMProtectEnd();
}

void CHIDWork::KeyData_UnPRD( CInteractionData* pResult )
{
	VMProtectBegin("KeyData_SetRenewTimes");
	// TODO: Add your control notification handler code here
	byte UnRPD[9]={'w','f','l','o','v','e','l','x','y'}; 
	CInteractionData tx;
	pResult->hidResult=HIDResult_OK;
	tx.buff[0] = 0x15;
	for(int i=0;i<9;i++)
		tx.buff[1+i]=UnRPD[i];
	CInteractionData rx = DeviceInteraction(tx);
	if (!rx.bOK || rx.CheckDataList() == false)
	{
		pResult->hidResult=HIDResult_ReadError;
		return;
	}
	//------------------
	VMProtectEnd();
}
