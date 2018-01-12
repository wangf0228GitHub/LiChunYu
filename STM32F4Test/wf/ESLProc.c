#include "main.h"
#include "CP1616_Client.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include "ESLProc.h"


void ESL_ReadProc( void )
{
	unsigned short retry=WithPC_RxBuf.RxStruct.CommandData.UARTCommand.Retry;
	unsigned char wakeUp[1];
	_ESLProc_RxBuf ESLProc_RxBuf;
	wakeUp[0]=0x00;
	while(retry!=0)
	{
		HAL_UART_Transmit(&UART_ESLProc_Handle,wakeUp,1,100);
		HAL_Delay(10);
		HAL_UART_Transmit(&UART_ESLProc_Handle,(unsigned char*)WithPC_RxBuf.RxStruct.CommandData.UARTCommand.Data,WithPC_RxBuf.RxStruct.DataLen-UARTCommand_DataOffset,100);
		while(1)
		{
			if(HAL_UART_Receive(&UART_ESLProc_Handle, ESLProc_RxBuf.AllData,1,WithPC_RxBuf.RxStruct.CommandData.UARTCommand.RxTimeOut)==HAL_OK)
			{
				if(ESLProc_RxBuf.RxStruct.FrameHeader==0x78)
				{
					if(HAL_UART_Receive(&UART_ESLProc_Handle, &ESLProc_RxBuf.AllData[1],10,WithPC_RxBuf.RxStruct.CommandData.UARTCommand.RxTimeOut)==HAL_OK)
					{
						CP1616_Client_SendData(WithPC_Command_ESL,ESLProc_RxBuf.AllData,11);
						return;
					}
					break;
				}
			}
			else
				break;
		}
		retry--;
	}
	if(retry==0)
	{
		CP1616_Client_SendError(WithPC_Command_ESL,2);
	}
}

