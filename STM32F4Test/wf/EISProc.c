#include "main.h"
#include "CP1616_Client.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "EISProc.h"


void EIS_CanReadProc( void )
{
	unsigned short retry=WithPC_RxBuf.RxStruct.CommandData.CANCommand.Retry;
	CAN1Init_ByPC();
// 	UART_EISProc_Handle.Init.BaudRate=WithPC_RxBuf.RxStruct.CommandData.UARTCommand.BaudRate;
// 	if (HAL_UART_Init(&UART_EISProc_Handle) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// 	while(retry!=0)
// 	{
// 		HAL_UART_Transmit(&UART_EISProc_Handle,wakeUp,1,100);
// 		HAL_Delay(10);
// 		HAL_UART_Transmit(&UART_EISProc_Handle,WithPC_RxBuf.RxStruct.CommandData.UARTCommand.Data,WithPC_RxBuf.RxStruct.DataLen-UARTCommand_DataOffset,100);
// 		while(1)
// 		{
// 			if(HAL_UART_Receive(&UART_EISProc_Handle, EISProc_RxBuf.AllData,1,WithPC_RxBuf.RxStruct.CommandData.UARTCommand.RxTimeOut)==HAL_OK)
// 			{
// 				if(EISProc_RxBuf.RxStruct.FrameHeader==0x78)
// 				{
// 					if(HAL_UART_Receive(&UART_EISProc_Handle, &EISProc_RxBuf.AllData[1],10,WithPC_RxBuf.RxStruct.CommandData.UARTCommand.RxTimeOut)==HAL_OK)
// 					{
// 						CP1616_Client_SendData(WithPC_Command_EIS,EISProc_RxBuf.AllData,11);
// 						return;
// 					}
// 					break;
// 				}
// 			}
// 			else
// 				break;
// 		}
// 		retry--;
// 	}
// 	if(retry==0)
// 	{
// 		CP1616_Client_SendError(WithPC_Command_EIS);
// 	}
}

