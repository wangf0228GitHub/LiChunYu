#include "ATA5824.h"
#include "SimSPI.h"
#include "..\Inc\tim.h"
#include "Variables.h"
#include "wfSys.h"
#include "AS3933.h"
#include "Function.h"
#include "Verify.h"
_ATA5824_ICDataStruct ATA5824_ICData[ATA5824_ICData_BufLen];
uint8_t ATA5824_TxList[20];
uint32_t ATA5824_TxCount;
uint32_t ATA5824_RxTick;
uint8_t ATA5824_RxList[20];
uint32_t ATA5824_RxCount;
uint32_t ATA5824_ICDataIndex;
#define TMDODelay() wfDelay_us(43)
void ATA5824_TxFrameProc(void);
void ATA5824_Test(void)
{
	uint8_t i;
	HAL_GPIO_WritePin(ATA5824_PWR_ON_GPIO_Port,ATA5824_PWR_ON_Pin,GPIO_PIN_SET);
	ATA5824_Init();
	while(1)
	{
		gFlags.ATA5824_bRxFrame=0;
		ATA5824_RxStart();
		while(!gFlags.ATA5824_bRxFrame);
		ATA5824_IDLE();
		ATA5824_FrameProc();//曼码解码
		wfDelay_ms(2);
	}
// 	for(i=0;i<8;i++)
// 		ATA5824_TxList[i]=i;
// 	ATA5824_TxCount=8;
// 
// 	while(1)
// 	{
// 		ATA5824_TxStart();
// 		wfDelay_ms(1);
// 		ATA5824_TxFrameProc();
// 		ATA5824_IDLE();
// 		wfDelay_ms(1000);
// 	}
	while(1);
}


void ATA5824_Exit(void)
{
	AS3933_Init();
	BAT_OFF();
	while(1);
}
void ATA5824_WorkProc(void)
{
	uint8_t i;
 	GetKeyState();
 	if(RomStateFlags.bRomWrited==0)
 	{
 		ATA5824_Exit();
 	}
	HAL_GPIO_WritePin(ATA5824_PWR_ON_GPIO_Port,ATA5824_PWR_ON_Pin,GPIO_PIN_SET);
	wfDelay_ms(2);
	ATA5824_Init();
	ATA5824_RxStart();
	GetKeyParam();	
	while(1)//第1帧:SSID
	{
// 		if(GetDeltaTick(ATA5824_RxTick)>100)//超时，退出
// 		{
// 			ATA5824_Exit();
// 		}
		if(gFlags.ATA5824_bRxFrame)
		{
			ATA5824_IDLE();
			ATA5824_FrameProc();//曼码解码
			if(ATA5824_RxCount==5)
			{
				if(ATA5824_RxList[0]==0x4a ||ATA5824_RxList[0]==0x4e)
				{
					
					if(ATA5824_RxList[1]==(SSID[0]&0xf8) &&
						ATA5824_RxList[2]==(SSID[1]) &&
						ATA5824_RxList[3]==(SSID[2]))// &&
						//ATA5824_RxList[4]==(SSID[0]&0x07))
					{
						wfDelay_ms(10);
						ATA5824_TxList[0]=0x78;
						ATA5824_TxCount=1;
						ATA5824_TxFrameProc();
						break;
					}
					else
					{
						ATA5824_TxList[0]=0x71;
						ATA5824_TxCount=1;
						ATA5824_TxFrameProc();

						ATA5824_Exit();
					}
				}
				else
				{
					ATA5824_RxStart();
				}
			}
			else
			{
				ATA5824_RxStart();
			}
		}
	}
	ATA5824_RxStart();
	ATA5824_RxTick=HAL_GetTick();
	while(1)//第2帧:10个运算的数据
	{
		if(GetDeltaTick(ATA5824_RxTick)>500)//超时，退出
		{
			ATA5824_Exit();
		}
		if(gFlags.ATA5824_bRxFrame)
		{
			ATA5824_IDLE();
			ATA5824_FrameProc();//曼码解码
			if(ATA5824_RxCount==10)
			{
				if(ATA5824_RxList[0]==0x50)
				{
					ATA5824_TxList[0]=0x5c;
					ATA5824_TxList[1]=LeftTimes[LeftTimesL];
					ATA5824_TxList[2]=LeftTimes[LeftTimesM];
					ATA5824_TxList[3]=LeftTimes[LeftTimesH];
					ATA5824_TxList[4]=GetVerify_Sum(ATA5824_TxList,4);
					ATA5824_TxCount=5;
					ATA5824_TxFrameProc();

					GetKeyWorkValue(&ATA5824_RxList[1],0x50);	
					for(i=0;i<8;i++)
					{
						ATA5824_TxList[1+i]=WorkValueDatas[i];
					}
					ATA5824_TxList[0]=0x27;					
					ATA5824_TxList[9]=GetVerify_Sum(ATA5824_TxList,9);
					ATA5824_TxCount=10;
					ATA5824_TxFrameProc();

					ATA5824_Exit();
				}
				else
				{
					ATA5824_RxStart();
				}
			}
			else
			{
				ATA5824_RxStart();
			}
		}
	}
}
void ATA5824_WaitRx(uint32_t timeOut)
{
	uint8_t i;	
	ATA5824_RxStart();
	while(1)//第1帧:SSID
	{
		if(timeOut!=-1)
		{
			if(GetDeltaTick(ATA5824_RxTick)>timeOut)//超时，退出
			{
				return;
				//ATA5824_Exit();
			}
		}
		if(gFlags.ATA5824_bRxFrame)
		{
			ATA5824_IDLE();
			ATA5824_FrameProc();//曼码解码
			if(ATA5824_RxCount==5)
			{
				if(ATA5824_RxList[0]==0x4a ||ATA5824_RxList[0]==0x4e)
				{
					if(ATA5824_RxList[1]==(SSID[0]&0xf8) &&
						ATA5824_RxList[2]==(SSID[1]) &&
						ATA5824_RxList[3]==(SSID[2]))// &&
						//ATA5824_RxList[4]==(SSID[0]&0x07))
					{
						wfDelay_ms(14);
						ATA5824_TxList[0]=0x78;
						ATA5824_TxCount=1;
						ATA5824_TxFrameProc();
						wfDelay_ms(20);
						ATA5824_RxStart();
						//return;
					}
					else
					{
						ATA5824_RxStart();
// 						ATA5824_TxList[0]=0x71;
// 						ATA5824_TxCount=1;
// 						ATA5824_TxFrameProc();
// 
// 						return;
						//ATA5824_Exit();
					}
				}
				else
				{
					ATA5824_RxStart();
				}
			}
			else if(ATA5824_RxCount==10)
			{
				if(ATA5824_RxList[0]==0x50)
				{
					ATA5824_TxList[0]=0x5c;
					ATA5824_TxList[1]=LeftTimes69;//LeftTimes[LeftTimesL];
					ATA5824_TxList[2]=LeftTimes[LeftTimesM];
					ATA5824_TxList[3]=LeftTimes[LeftTimesH];
					ATA5824_TxList[4]=GetVerify_Sum(ATA5824_TxList,4);
					ATA5824_TxCount=5;
					ATA5824_TxFrameProc();

					GetKeyWorkValue(&ATA5824_RxList[1],0x50);	
					for(i=0;i<8;i++)
					{
						ATA5824_TxList[1+i]=WorkValueDatas[i];
					}
					ATA5824_TxList[0]=0x27;					
					ATA5824_TxList[9]=GetVerify_Sum(ATA5824_TxList,9);
					ATA5824_TxCount=10;
					ATA5824_TxFrameProc();
					return;
				}
				else
				{
					ATA5824_RxStart();
				}
			}
			else
			{
				ATA5824_RxStart();
			}
		}
	}	
}
void ATA5824_TxFrameProc(void)
{
	uint32_t i,j;
	uint8_t x;
	uint32_t Pre0=20;
	ATA5824_TxStart();
	wfDelay_ms(1);
	for(i=0;i<Pre0;i++)
	{
		SIMSPI_SDO_High();
		TMDODelay();
		SIMSPI_SDO_Low();
		TMDODelay();
	}
	SIMSPI_SDO_Low();
	TMDODelay();
	SIMSPI_SDO_High();
	TMDODelay();
	for(i=0;i<ATA5824_TxCount;i++)
	{
		x=ATA5824_TxList[i];
		for (j=0;j<8;j++)//先低位
		{
			if((x&0x01)==0x01)
			{
				SIMSPI_SDO_Low();
				TMDODelay();
				SIMSPI_SDO_High();
				TMDODelay();
			}
			else
			{
				SIMSPI_SDO_High();
				TMDODelay();
				SIMSPI_SDO_Low();
				TMDODelay();
			}
			x=x>>1;
		}
	}	
	SIMSPI_SDO_Low();
	TMDODelay();
	SIMSPI_SDO_High();
	TMDODelay();
	SIMSPI_SDO_Low();
	TMDODelay();
	ATA5824_IDLE();
}
void ATA5824_FrameProc(void)
{
	uint32_t i,x;	
	uint8_t rxBitList[500],rxBitList1[500];
	uint32_t rxBitLen=0,rxBitLen1=0,nBit;
	uint32_t bData=0,bOdd=0;	
	ATA5824_RxCount=0;
	for(i=0;i<ATA5824_ICDataIndex;i++)
	{
		rxBitList[rxBitLen++]=ATA5824_ICData[i].pinState;
		if(ATA5824_ICData[i].time>60)//全位脉宽保存两个半位值
		{
			rxBitList[rxBitLen++]=ATA5824_ICData[i].pinState;
			if(bData==0)//判断出奇偶数项
			{
				if(i%2==0)
				{
					bOdd=1;//奇数项
				}
				else
				{
					bOdd=0;//偶数项
				}
				bData=1;
			}
		}
	}
	for(i=0;i<rxBitLen;i+=2)
	{
		if(bOdd==0)//偶数项
		{
			rxBitList1[rxBitLen1++]=rxBitList[i];
		}
		else
		{
			rxBitList1[rxBitLen1++]=rxBitList[i+1];
		}
	}
	bData=0;
	nBit=0;
	for(i=0;i<rxBitLen1;i++)
	{
		if(bData==0)//去掉同步头
		{
			if(rxBitList1[i]==1)
			{
				bData=1;
			}
		}
		else
		{
			if(rxBitList1[i]==1)
			{
				ATA5824_RxList[ATA5824_RxCount] |= 0x80;
			}
			else
			{
				ATA5824_RxList[ATA5824_RxCount] &= 0x7f;
			}
			nBit++;
			if(nBit>=8)
			{
				ATA5824_RxCount++;
				nBit=0;
			}
			else
			{
				ATA5824_RxList[ATA5824_RxCount] =ATA5824_RxList[ATA5824_RxCount] >>1;
			}
		}
	}
	//帧接收完成，等待处理
	//ATA5824_RxStart();
}
void ATA5824_ICProc(void)
{
	uint32_t read;
 	if(gFlags.ATA5824_bFirstIC!=0)//第一次捕捉
 	{
 		gFlags.ATA5824_bFirstIC=0;
 		htim22.Instance->CNT=0;
 		__HAL_TIM_CLEAR_IT(&htim22, TIM_IT_UPDATE);
 		HAL_TIM_Base_Start_IT(&htim22);	
 	}
 	else
 	{
 		read=HAL_TIM_ReadCapturedValue(&htim22,TIM_CHANNEL_2);//+13;
 		htim22.Instance->CNT=0;
 		ATA5824_ICData[ATA5824_ICDataIndex].time=read;
 		ATA5824_ICData[ATA5824_ICDataIndex].pinState=HAL_GPIO_ReadPin(ATA5824_MISO_TMDI_GPIO_Port,ATA5824_MISO_TMDI_Pin);
 		ATA5824_ICDataIndex++;		
 	}
}
void ATA5824_RxStart(void)
{
	gFlags.ATA5824_bRxFrame=0;	
	gFlags.ATA5824_bFirstIC=1;
	ATA5824_ICDataIndex=0;
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x25);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
 	__HAL_TIM_CLEAR_IT(&htim22, TIM_IT_UPDATE);
 	HAL_TIM_IC_Start_IT(&htim22, TIM_CHANNEL_2);	
	ATA5824_RxTick=HAL_GetTick();
}
void ATA5824_TxStart(void)
{
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x23);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}
void ATA5824_IDLE(void)
{
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x21);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}
void ATA5824_Init(void)
{
	uint8_t rx;
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SimSPI_Init();
	HAL_Delay(1);
	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_SET);

 	rx=SimSPI_Proc(0x60);
 	rx=SimSPI_Proc(0x21);
 
 	rx=SimSPI_Proc(0x61);
 	rx=SimSPI_Proc(0xb1);
 
 	rx=SimSPI_Proc(0x62);
 	rx=SimSPI_Proc(0x7b);
 
 	rx=SimSPI_Proc(0x63);
 	rx=SimSPI_Proc(0x00);
 
 	rx=SimSPI_Proc(0x64);
 	rx=SimSPI_Proc(0x9b);
 
 	rx=SimSPI_Proc(0x65);
 	rx=SimSPI_Proc(0xfa);
 
 	rx=SimSPI_Proc(0x66);
 	rx=SimSPI_Proc(0xa8);
 
 	rx=SimSPI_Proc(0x67);
 	rx=SimSPI_Proc(0x50);

	HAL_GPIO_WritePin(ATA5824_CS_GPIO_Port,ATA5824_CS_Pin,GPIO_PIN_RESET);
	SIMSPI_SDO_Low();
}