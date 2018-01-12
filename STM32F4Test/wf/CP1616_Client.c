#include "main.h"
#include "CP1616_Client.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "ESLProc.h"
#include "can.h"
#include "tim.h"
volatile _WithPC_RxBuf WithPC_RxBuf;
volatile unsigned short WithPC_RxBufIndex;
volatile unsigned short WithPC_NeedRxCount;
volatile unsigned int WithPC_NoRxTick;
volatile _WithPC_TxBuf WithPC_TxBuf;
const unsigned int CAN_BS1[16]=
{
	CAN_BS1_1TQ,
	CAN_BS1_2TQ,
	CAN_BS1_3TQ,
	CAN_BS1_4TQ,
	CAN_BS1_5TQ,
	CAN_BS1_6TQ,
	CAN_BS1_7TQ,
	CAN_BS1_8TQ,
	CAN_BS1_9TQ,
	CAN_BS1_10TQ,
	CAN_BS1_11TQ,
	CAN_BS1_12TQ,
	CAN_BS1_13TQ,
	CAN_BS1_14TQ,
	CAN_BS1_15TQ,
	CAN_BS1_16TQ
};
const unsigned int CAN_BS2[8]=
{
	CAN_BS2_1TQ,
	CAN_BS2_2TQ,
	CAN_BS2_3TQ,
	CAN_BS2_4TQ,
	CAN_BS2_5TQ,
	CAN_BS2_6TQ,
	CAN_BS2_7TQ,
	CAN_BS2_8TQ
};
const unsigned int CAN_SJW[4]=
{
	CAN_SJW_1TQ,
	CAN_SJW_2TQ,
	CAN_SJW_3TQ,
	CAN_SJW_4TQ
};
void ProcCommandFromPC( void )
{
	unsigned char sum;
	sum=GetVerify_Sum((unsigned char *)WithPC_RxBuf.AllRxData,WithPC_RxBufIndex-2);
	if(sum!=WithPC_RxBuf.AllRxData[WithPC_RxBufIndex-2])
	{
		CP1616_Client_EndProcCommand();
		return;
	}	
	//LED_DS1_Toggle();
	switch(WithPC_RxBuf.RxStruct.CommandIndex)//命令字
	{
	case WithPC_Command_Connect:
		CP1616_Client_EndProcCommand();
		CP1616_Client_SendOK(1);		
		break;
	case WithPC_Command_ESL:
		ESL_ReadProc();
		CP1616_Client_EndProcCommand();		
		break;
	case WithPC_Command_SetUartBaudRate:
		UART_ESLProc_Handle.Init.BaudRate=WithPC_RxBuf.RxStruct.CommandData.SetUartBaudRate.BaudRate;
		if (HAL_UART_Init(&UART_ESLProc_Handle) == HAL_OK)
		{
			CP1616_Client_SendOK(WithPC_Command_SetUartBaudRate);
		}
		else
		{
			CP1616_Client_SendError(WithPC_Command_SetUartBaudRate,1);		
		}
		CP1616_Client_EndProcCommand();	
		break;
	case WithPC_Command_SetCANBaudRate:
		hcan1.Init.Prescaler = WithPC_RxBuf.RxStruct.CommandData.SetCANBaudRate.Prescaler;
		hcan1.Init.BS1=CAN_BS1[WithPC_RxBuf.RxStruct.CommandData.SetCANBaudRate.BS1-1];
		hcan1.Init.BS2=CAN_BS2[WithPC_RxBuf.RxStruct.CommandData.SetCANBaudRate.BS2-1];
		hcan1.Init.SJW=CAN_SJW[WithPC_RxBuf.RxStruct.CommandData.SetCANBaudRate.SJW-1];
		switch(WithPC_RxBuf.RxStruct.CommandData.SetCANBaudRate.RNum)
		{
		case 0:
			HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAN1_A0_GPIO_Port, CAN1_A0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CAN1_A1_GPIO_Port, CAN1_A1_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAN1_A0_GPIO_Port, CAN1_A0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAN1_A1_GPIO_Port, CAN1_A1_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAN1_A0_GPIO_Port, CAN1_A0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(CAN1_A1_GPIO_Port, CAN1_A1_Pin, GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(CAN1_EN_GPIO_Port, CAN1_EN_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAN1_A0_GPIO_Port, CAN1_A0_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(CAN1_A1_GPIO_Port, CAN1_A1_Pin, GPIO_PIN_SET);
			break;
		}
		if (HAL_CAN_Init(&hcan1) == HAL_OK)
		{		
			CP1616_Client_SendOK(WithPC_Command_SetCANBaudRate);	
		}
		else
		{
			
			CP1616_Client_SendError(WithPC_Command_SetCANBaudRate,1);
		}
		CP1616_Client_EndProcCommand();	
		break;	
	case WithPC_Command_CANRead:
		CanRead_ByPC();	
		CP1616_Client_EndProcCommand();		
		break;
	case WithPC_Command_IRWork:
		IRWork();
		CP1616_Client_EndProcCommand();	
		break;
	case WithPC_Command_IREn:
		if(WithPC_RxBuf.RxStruct.CommandData.IREn.bEn==0)
		{
			HAL_GPIO_WritePin(IR1_EN1_GPIO_Port, IR1_EN1_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(IR1_EN1_GPIO_Port, IR1_EN1_Pin, GPIO_PIN_SET);
		}
		CP1616_Client_EndProcCommand();	
		CP1616_Client_SendOK(WithPC_Command_IREn);
		break;
	}
}
void CP1616_Client_Init(void)
{
	CP1616_Client_EndProcCommand();
	WithPC_TxBuf.TxStruct.FrameHeader=0x1616;
}
void CP1616_Client_EndProcCommand(void)
{
	gFlags.Bits.bWithPC_Rx=0;
	WithPC_NeedRxCount=0xffff;
	WithPC_RxBufIndex=0;
}
void CP1616_Client_ProcRx(unsigned char rx)
{
	WithPC_NoRxTick=HAL_GetTick();
	if(gFlags.Bits.bWithPC_Rx==1)
 		return;
	WithPC_RxBuf.AllRxData[WithPC_RxBufIndex++]=rx;
	if(WithPC_RxBufIndex>7)
	{
 		if(rx==0x0d && WithPC_RxBufIndex==WithPC_NeedRxCount)
 		{			
 			gFlags.Bits.bWithPC_Rx=1;			
 		}
 		else if(WithPC_RxBufIndex>WithPC_NeedRxCount)
 		{
 			WithPC_RxBufIndex=0;
 			WithPC_NeedRxCount=0xffff;
 		}
 		else if(WithPC_RxBufIndex>UART_WithPC_BUFMAX)
 		{
 			WithPC_RxBufIndex=0;
 			WithPC_NeedRxCount=0xffff;				
 		}
	}
	else if(WithPC_RxBufIndex==6)//数据包长度
	{
 		WithPC_NeedRxCount=WithPC_RxBuf.RxStruct.DataLen+UART_WithPC_RxFrameOther;
	}
// 	else if(WithPC_RxBufIndex==4)//收到命令
// 	{
//  		
// 	}
	else if(WithPC_RxBufIndex==2)
	{
 		if(rx!=0x16)
 		{
 			WithPC_RxBufIndex=0;
 		}
	}
	else if(WithPC_RxBufIndex==1)
	{
 		if(rx!=0x16)
 		{
 			WithPC_RxBufIndex=0;
 		}
	}	
}
unsigned char GetVerify_Sum(unsigned char* pBuff,unsigned int Count )
{
	unsigned char sum,i;
	sum=0;
	for(i=0;i<Count;i++)
	{
		sum=(unsigned char)(sum+pBuff[i]);
	}
	return sum;
}

void CP1616_Client_SendData(unsigned short CommandIndex,unsigned char* pBuff,unsigned short Count)
{
	unsigned short i;
	WithPC_TxBuf.TxStruct.bError=0;
	WithPC_TxBuf.TxStruct.CommandIndex=CommandIndex;
	WithPC_TxBuf.TxStruct.DataLen=Count;
	for(i=0;i<Count;i++)
	{
 		WithPC_TxBuf.TxStruct.Data[i]=pBuff[i];
	}
	WithPC_TxBuf.TxStruct.Data[Count]=GetVerify_Sum((unsigned char *)WithPC_TxBuf.AllData,Count+UART_WithPC_TxFrameOther-2);
	WithPC_TxBuf.TxStruct.Data[Count+1]=0x0d;
	HAL_UART_Transmit(&UART_WithPC_Handle,(unsigned char*)WithPC_TxBuf.AllData,Count+UART_WithPC_TxFrameOther,1000);
	CP1616_Client_EndProcCommand();
}
void CP1616_Client_SendOK(unsigned char CommandIndex)
{
	CP1616_Client_SendData(CommandIndex,"OK",2);
}

void CP1616_Client_SendError( unsigned char CommandIndex ,unsigned char errCode)
{
	WithPC_TxBuf.TxStruct.bError=1;
	WithPC_TxBuf.TxStruct.CommandIndex=CommandIndex;
	WithPC_TxBuf.TxStruct.ErrCode=errCode;
	WithPC_TxBuf.TxStruct.DataLen=0;
	WithPC_TxBuf.TxStruct.Data[0]=GetVerify_Sum((unsigned char*)WithPC_TxBuf.AllData,UART_WithPC_TxFrameOther-2);
	WithPC_TxBuf.TxStruct.Data[1]=0x0d;
	HAL_UART_Transmit(&UART_WithPC_Handle,(unsigned char*)WithPC_TxBuf.AllData,UART_WithPC_TxFrameOther,1000);
	CP1616_Client_EndProcCommand();
}

