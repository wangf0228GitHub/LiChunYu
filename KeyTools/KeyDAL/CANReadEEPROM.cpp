#include "CANReadEEPROM.h"
#include "VMProtectSDK.h"
#include "Serial.h"
#include "ConstDefine.h"

CANReadEEPROM canReadEEPROM[10];
#define EEPROMDataMaxSize 1024*10
byte EEPROMData[EEPROMDataMaxSize];//最大文件内容10k
int EEPROMDataLen;//EEPROM文件大小

byte EEPROMData_tempRx[EEPROMDataMaxSize];
int EEPROMData_tempRxLen;
bool __stdcall CANReadEEPROMProc( int mode )
{
	VMProtectBegin("CANReadEEPROMProc");
	nErrorCode=ErrorCode_NoErr;
	if(mode==6)
		mode=3;
	if(mode==9)
		mode=8;
	EEPROMData_tempRxLen=0;
	if(gSerialProcCallback==NULL)
	{
		nErrorCode=ErrorCode_NoSerialProcCallback;
		return false;
	}
	gSetProgressCallback(SetProgress_Max,canReadEEPROM[mode].PacketCount);
	gSetProgressCallback(SetProgress_Step,1);
	gSetProgressCallback(SetProgress_Value,0);
	canReadEEPROM[mode].WakeUp.MakePacket();
	MakeTxPacket(Command_CANRead,canReadEEPROM[mode].WakeUp.Packet,CANCommand_PacketLen);
	if(gSerialProcCallback(Command_CANRead,stm32f4_TxData,0,stm32f4_TxDataLen,canReadEEPROM[mode].WakeUp.SerialReadTimeout,canReadEEPROM[mode].WakeUp.SerialRetry)==false)
	{
		nErrorCode=ErrorCode_SerialProcError;
		return false;
	}	
	gSetProgressCallback(SetProgress_StepIn,0);
	if(RxPacket.bError!=0)
	{
		nErrorCode=RxPacket.ErrCode;
		return false;
	}
	int nErr=0;	
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
	{
		//int errCount=0;		
		for(int j=0;j<canReadEEPROM[mode].GetDataSeqLen;)
		{
			if(canReadEEPROM[mode].GetDataSeq[j].bNeedReadReg)
				canReadEEPROM[mode].GetDataSeq[j].TxData[canReadEEPROM[mode].GetDataSeq[j].ReadRegAddr]=canReadEEPROM[mode].ReadReg[i];
			canReadEEPROM[mode].GetDataSeq[j].MakePacket();
			MakeTxPacket(Command_CANRead,canReadEEPROM[mode].GetDataSeq[j].Packet,CANCommand_PacketLen);
			if(gSerialProcCallback(Command_CANRead,stm32f4_TxData,0,stm32f4_TxDataLen,canReadEEPROM[mode].GetDataSeq[j].SerialReadTimeout,canReadEEPROM[mode].GetDataSeq[j].SerialRetry)==false)
			{
				nErrorCode=ErrorCode_SerialProcError;
				return false;
			}			
			if(RxPacket.bError!=0)
			{
				nErrorCode=RxPacket.ErrCode;
				return false;
			}
			bool bNext=true;
			bool bNextFrame=false;
			switch(mode)
			{
			case 1://01,不用检查
				{
					byte check01[6]={0x10,0x0C,0x71,0x22,0x70,0x02};
					check01[5]=canReadEEPROM[mode].ReadReg[i];
					if(j==1)
					{
						bNext=true;
						for(int k=0;k<6;k++)
						{
							if(check01[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=false;
								break;
							}
						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x10;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
								// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
								// 								return false;
							}
						}
					}
				}
				break;
			case 2://02,检查当前接收是否为“03,7F,31,78,00,00,00,00 此包译为要求重发命令”
				{
					byte check02[8]={ 0x03, 0x7f, 0x031, 0x78, 0x00, 0x00, 0x00, 0x00 };
					if(j==0)
					{
						bNext=false;
						for(int k=0;k<8;k++)
						{
							if(check02[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=true;
								break;
							}
						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x18;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
								// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
								// 								return false;
							}
						}
					}
				}
				break;
			case 3://03,需要检查
				{
					byte check03[8]={0x10,0x0E,0x71,0x01,0x00,0x22,0x70,0x00};
					check03[7]=canReadEEPROM[mode].ReadReg[i];
					if(j==0)
					{
						bNext=true;
						for(int k=0;k<8;k++)
						{
							if(check03[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=false;
								break;
							}
						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x18;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
								// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
								// 								return false;
							}
						}
					}
				}
				break;
			case 4://
				{
					byte check04[6]={0x10,0x0C,0x71,0x22,0x70,0x02};
					check04[5]=canReadEEPROM[mode].ReadReg[i];
					if(j==1)
					{
						bNext=true;
						for(int k=0;k<6;k++)
						{
							if(check04[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=false;
								break;
							}
						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x18;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
								// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
								// 								return false;
							}
						}
					}
				}
				break;
			case 5://05,需要检查
				{
					byte check05[8]={0x10,0x0E,0x71,0x01,0x00,0x22,0x70,0x00};					
					check05[7]=canReadEEPROM[mode].ReadReg[i];
					if(j==0)
					{
						bNext=true;
						for(int k=0;k<8;k++)
						{
							if(check05[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=false;
								break;
							}
						}
// 						if(!bNext)
// 						{
// 							byte check05_1[8]={0x03,0x7F,0x31,0x12,0x00,0x00,0x00,0x00};
// 							for(int k=0;k<8;k++)
// 							{
// 								if(check05_1[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
// 								{
// 									bNext=false;
// 									break;
// 								}
// 							}
// 						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x18;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
// 								return false;
							}
						}
					}
				}
				break;
			case 7://07,需要检查
				{
					byte check07[8]={0x10,0x0E,0x71,0x01,0x00,0x22,0x70,0x00};
					check07[7]=canReadEEPROM[mode].ReadReg[i];
					if(j==0)
					{
						bNext=true;
						for(int k=0;k<8;k++)
						{
							if(check07[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=false;
								break;
							}
						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x18;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
								// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
								// 								return false;
							}
						}
					}
				}
				break;
			case 8://08,需要检查
				{
					byte check08[8]={0x10,0x0E,0x71,0x01,0x00,0x22,0x70,0x00};
					check08[7]=canReadEEPROM[mode].ReadReg[i];
					if(j==0)
					{
						bNext=true;
						for(int k=0;k<8;k++)
						{
							if(check08[k]!=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k])
							{
								bNext=false;
								break;
							}
						}
						if(!bNext)
						{
							nErr++;
							if(nErr>10)
							{
								bNextFrame=true;
								for(int k=0;k<0x18;k++)
								{
									EEPROMData_tempRx[EEPROMData_tempRxLen++]=0x00;
								}
								// 								nErrorCode=ErrorCode_CanReadEEPROM_nErr;
								// 								return false;
							}
						}
					}
				}
				break;
			}
			if(bNextFrame)
			{
				nErr=0;
				break;
			}
			if(bNext)
			{				
				nErr=0;
				if(!canReadEEPROM[mode].GetDataSeq[j].bTxOnly)
				{
					for(int k=0;k<RxPacket.DataLen;k++)
					{
						EEPROMData_tempRx[EEPROMData_tempRxLen++]=RxPacket.Data[stm32f4_RxPacket_pDataIndex+k];
					}
				}
				j++;
			}
		}
		gSetProgressCallback(SetProgress_StepIn,0);
	}	
	memset(EEPROMData,0xFF,EEPROMDataMaxSize);

	switch(mode)
	{
	case 1://01款
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x10+0x06];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x10+0x07];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x10+0x09];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x10+0x0a];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x10+0x0b];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x10+0x0c];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x10+0x0d];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x10+0x0e];
		}
		break;
	case 2:
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x18+0x09];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x18+0x0a];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x18+0x0b];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x18+0x0c];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x18+0x0d];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x18+0x0e];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x18+0x0f];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x18+0x11];
		}
		break;
	case 3:
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x18+0x09];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x18+0x0a];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x18+0x0b];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x18+0x0c];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x18+0x0d];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x18+0x0e];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x18+0x0f];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x18+0x11];
		}		
		break;
	case 4:
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x18+0x0e];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x18+0x0f];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x18+0x11];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x18+0x12];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x18+0x13];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x18+0x14];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x18+0x15];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x18+0x16];
		}		
		break;
	case 5:
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x18+0x09];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x18+0x0a];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x18+0x0b];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x18+0x0c];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x18+0x0d];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x18+0x0e];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x18+0x0f];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x18+0x11];
		}		
		break;
	case 7:
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x18+0x09];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x18+0x0a];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x18+0x0b];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x18+0x0c];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x18+0x0d];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x18+0x0e];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x18+0x0f];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x18+0x11];
		}		
		break;
	case 8:
		for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)
		{
			EEPROMData[i*0x10+0]=EEPROMData_tempRx[i*0x18+0x09];
			EEPROMData[i*0x10+1]=EEPROMData_tempRx[i*0x18+0x0a];
			EEPROMData[i*0x10+2]=EEPROMData_tempRx[i*0x18+0x0b];
			EEPROMData[i*0x10+3]=EEPROMData_tempRx[i*0x18+0x0c];
			EEPROMData[i*0x10+4]=EEPROMData_tempRx[i*0x18+0x0d];
			EEPROMData[i*0x10+5]=EEPROMData_tempRx[i*0x18+0x0e];
			EEPROMData[i*0x10+6]=EEPROMData_tempRx[i*0x18+0x0f];
			EEPROMData[i*0x10+7]=EEPROMData_tempRx[i*0x18+0x11];
		}		
		break;
	}
	EEPROMDataLen=0x400;
	return true;
	VMProtectEnd();
}

CANReadEEPROM::CANReadEEPROM(void)
{
}


CANReadEEPROM::~CANReadEEPROM(void)
{
}
void initCANReadEEPROM_08()
{
	VMProtectBegin("initCANReadEEPROM_08");
	int mode=8;
	/************************************************************************/
	/* 08款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=2;//共2帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧，0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.TxStdId = 0x612;
	byte temp1[8]={ 0x02, 0x10, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 200;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 1;
	canReadEEPROM[mode].WakeUp.NeedStdId=0x482;
	/************************************************************************/
	/* 第1帧:  0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x612;

	byte temp3[8]={ 0x06,0x31,0x01,0x00,0x22,0x6F,0x00,0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[0].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[0].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[0].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[0].ReadRegAddr=6;
	/************************************************************************/
	/* 第2帧:   0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x612;
	byte temp5[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp5[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[2].bNeedSaveData=true;
	canReadEEPROM[mode].GetDataSeq[1].NeedRxPocket=2;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}
void initCANReadEEPROM_07()
{
	VMProtectBegin("initCANReadEEPROM_07");
	int mode=7;
	/************************************************************************/
	/* 07款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=2;//共2帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧，0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.TxStdId = 0x612;
	byte temp1[8]={ 0x02, 0x10, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 200;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 1;
	canReadEEPROM[mode].WakeUp.NeedStdId=0x482;
	/************************************************************************/
	/* 第1帧:  0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x612;

	byte temp3[8]={ 0x06, 0x31, 0x01, 0x00, 0x22, 0x6F, 0x00, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[0].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[0].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[0].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[0].ReadRegAddr=6;
	/************************************************************************/
	/* 第2帧:   0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x612;
	byte temp4[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp4[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[1].bNeedSaveData=true;
	canReadEEPROM[mode].GetDataSeq[1].NeedRxPocket=2;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}
void initCANReadEEPROM_05()
{
	VMProtectBegin("initCANReadEEPROM_05");
	int mode=5;
	/************************************************************************/
	/* 05款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=2;//共2帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧，0x4e0发，0x5ff收                                             */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.TxStdId = 0x4e0;
	byte temp1[8]={ 0x02, 0x10, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 200;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 1;
	canReadEEPROM[mode].WakeUp.NeedStdId=0x5ff;
	/************************************************************************/
	/* 第1帧:  0x4e0发，0x5ff收                                             */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x4e0;

	byte temp3[8]={ 0x06, 0x31, 0x01, 0x00, 0x22, 0x6F, 0x00, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[0].NeedStdId = 0x5ff;
//	canReadEEPROM[mode].GetDataSeq[0].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[0].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[0].ReadRegAddr=6;
	/************************************************************************/
	/* 第2帧:   0x4e0发，0x5ff收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x4e0;
	byte temp4[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp4[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x5ff;
//	canReadEEPROM[mode].GetDataSeq[1].bNeedSaveData=true;
	canReadEEPROM[mode].GetDataSeq[1].NeedRxPocket=2;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}
void initCANReadEEPROM_04()
{
	VMProtectBegin("initCANReadEEPROM_04");
	int mode=4;
	/************************************************************************/
	/* 04款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=3;//共2帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧，0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.TxStdId = 0x612;
	byte temp1[8]={ 0x02, 0x10, 0x92, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 200;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 1;
	canReadEEPROM[mode].WakeUp.NeedStdId=0x482;
	/************************************************************************/
	/* 第1帧:  0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x612;

	byte temp3[8]={ 0x04,0x31,0x22,0x6F,0x00,0xFF,0xFF,0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[0].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[0].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[0].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[0].ReadRegAddr=4;
	/************************************************************************/
	/* 第2帧:   0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x612;
	byte temp4[8]={ 0x02,0x3E,0x02,0xFF,0xFF,0xFF,0xFF,0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp4[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[1].bNeedSaveData=true;
	/************************************************************************/
	/* 第3帧:   0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[2].TxStdId = 0x612;
	byte temp5[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[2].TxData[i] = temp5[i];
	canReadEEPROM[mode].GetDataSeq[2].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[2].bNeedSaveData=true;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}
void initCANReadEEPROM_03()
{
	VMProtectBegin("initCANReadEEPROM_03");
	int mode=3;
	/************************************************************************/
	/* 03款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=2;//共2帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧，0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.TxStdId = 0x612;
	byte temp1[8]={ 0x02, 0x10, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 200;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 1;
	canReadEEPROM[mode].WakeUp.NeedStdId=0x482;
	/************************************************************************/
	/* 第1帧:  0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x612;

	byte temp3[8]={ 0x06, 0x31, 0x01, 0x00, 0x22, 0x6F, 0x00, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[0].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[0].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[0].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[0].ReadRegAddr=6;
	/************************************************************************/
	/* 第2帧:   0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x612;
	byte temp4[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp4[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[1].bNeedSaveData=true;
	canReadEEPROM[mode].GetDataSeq[1].NeedRxPocket=2;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}
void initCANReadEEPROM_02()
{
	VMProtectBegin("initCANReadEEPROM_02");
	int mode=2;
	/************************************************************************/
	/* 02款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=2;//共2帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧，0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.TxStdId = 0x612;
	byte temp1[8]={ 0x02, 0x10, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 200;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 1;
	canReadEEPROM[mode].WakeUp.NeedStdId=0x482;
	/************************************************************************/
	/* 第1帧:  0x612发，0x482收                                             */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x612;

	byte temp3[8]={ 0x06, 0x31, 0x01, 0x03, 0x22, 0x6F, 0x00, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[0].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[0].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[0].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[0].ReadRegAddr=6;	
	/************************************************************************/
	/* 第2帧:   0x612发，0x482收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x612;
	byte temp4[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp4[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x482;
//	canReadEEPROM[mode].GetDataSeq[1].bNeedSaveData=true;
	canReadEEPROM[mode].GetDataSeq[1].NeedRxPocket=2;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}
void initCANReadEEPROM_01()
{
	VMProtectBegin("initCANReadEEPROM_01");
	int mode=1;
	/************************************************************************/
	/* 01款                                                                 */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeqLen=3;//共3帧循环要数
	canReadEEPROM[mode].ReadRegLen=0x2f;
	for(int i=0;i<canReadEEPROM[mode].ReadRegLen;i++)//需读取的地址为0~0x2f
		canReadEEPROM[mode].ReadReg[i]=i;
	/************************************************************************/
	/* 唤醒帧                                                               */
	/************************************************************************/
	canReadEEPROM[mode].WakeUp.bTxOnly=1;
	canReadEEPROM[mode].WakeUp.TxStdId = 0x01c;
	byte temp1[8]={ 0x02, 0x10, 0x92, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].WakeUp.TxData[i] = temp1[i];
	canReadEEPROM[mode].WakeUp.TxRetry = 10;
	canReadEEPROM[mode].WakeUp.TxTimeOut = 10;

	/************************************************************************/
	/* 第1帧:只发不收                                                       */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[0].bTxOnly = 1;
	canReadEEPROM[mode].GetDataSeq[0].TxStdId = 0x01c;
	byte temp2[8]={ 0x02, 0x3E, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[0].TxData[i] = temp2[i];
	canReadEEPROM[mode].GetDataSeq[0].TxRetry = 10;
	canReadEEPROM[mode].GetDataSeq[0].TxTimeOut = 10;
	/************************************************************************/
	/* 第2帧:  0x4e0发，0x5ff收                                             */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[1].TxStdId = 0x4e0;

	byte temp3[8]={ 0x04, 0x31, 0x22, 0x6F, 0x00, 0xFF, 0xFF, 0xFF };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[1].TxData[i] = temp3[i];
	canReadEEPROM[mode].GetDataSeq[1].NeedStdId = 0x5ff;
//	canReadEEPROM[mode].GetDataSeq[1].bNeedSaveData=true;

	canReadEEPROM[mode].GetDataSeq[1].bNeedReadReg=true;
	canReadEEPROM[mode].GetDataSeq[1].ReadRegAddr=4;	
	/************************************************************************/
	/* 第3帧:   0x4e0发，0x5ff收                                            */
	/************************************************************************/
	canReadEEPROM[mode].GetDataSeq[2].TxStdId = 0x4e0;
	byte temp4[8]={ 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
	for(int i=0;i<8;i++)
		canReadEEPROM[mode].GetDataSeq[2].TxData[i] = temp4[i];
	canReadEEPROM[mode].GetDataSeq[2].NeedStdId = 0x5ff;
//	canReadEEPROM[mode].GetDataSeq[2].bNeedSaveData=true;
	/************************************************************************/
	/* 包数量，用于控制进度条                                               */
	/************************************************************************/
	canReadEEPROM[mode].PacketCount=1+canReadEEPROM[mode].ReadRegLen;
	VMProtectEnd();
}

void initCANReadEEPROM()
{	
	initCANReadEEPROM_01();
	initCANReadEEPROM_02();
	initCANReadEEPROM_03();//03,06
	initCANReadEEPROM_04();
	initCANReadEEPROM_05();
	initCANReadEEPROM_07();
	initCANReadEEPROM_08();//08,09
}

void __stdcall GetEEPROMData( byte* pBuf,int& len )
{
	VMProtectBegin("GetEEPROMData");
	for(int i=0;i<EEPROMDataLen;i++)
		pBuf[i]=EEPROMData[i];
	len=EEPROMDataLen;
	VMProtectEnd();
}

