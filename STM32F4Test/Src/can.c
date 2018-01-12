/**
  ******************************************************************************
  * File Name          : CAN.c
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
unsigned short canRxDataIndex;
unsigned char canRxData[1024];
HAL_StatusTypeDef CanStatus;

//void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
//{
//	unsigned int i;
//	__HAL_CAN_ENABLE_IT(&hcan1,CAN_IT_FMP0);	//重新开启 FIF00 消息挂号中断
//}

void CanRead_ByPC( void )
{
	unsigned char i=0,rxRetry,NeedRxPocket;
	unsigned short txRetry;	
	CAN_FilterConfTypeDef CAN1_FilerConf;
	hcan1.pTxMsg->StdId=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxStdId; //标准标识符
	hcan1.pTxMsg->ExtId=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxExtId; //扩展标识符(29 位)
	hcan1.pTxMsg->IDE=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxIDE; //使用标准帧
	hcan1.pTxMsg->RTR=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxRTR; //数据帧
	hcan1.pTxMsg->DLC=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxDLC;
	for(i=0;i<hcan1.pTxMsg->DLC;i++)
		hcan1.pTxMsg->Data[i]=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxData[i];
	if(WithPC_RxBuf.RxStruct.CommandData.CANCommand.bTxOnly)
	{
		txRetry=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxRetry;
		while(txRetry!=0)
		{
			CanStatus=HAL_CAN_Transmit(&hcan1,WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxTimeOut);
			if(CanStatus==HAL_OK)
			{
				CP1616_Client_SendOK(WithPC_Command_CANRead);
				return;
			}
			txRetry--;
		}
		if(txRetry==0)
		{
			CP1616_Client_SendError(WithPC_Command_CANRead,2);
			return;
		}
	}
	else
	{
		CAN1_FilerConf.FilterIdHigh=(WithPC_RxBuf.RxStruct.CommandData.CANCommand.NeedStdId)<<5; //32 位 ID
		CAN1_FilerConf.FilterIdLow=0|CAN_ID_STD;
		CAN1_FilerConf.FilterMaskIdHigh=((WithPC_RxBuf.RxStruct.CommandData.CANCommand.NeedExtId<<3)>>16)&0xffff; //32 位 MASK
		CAN1_FilerConf.FilterMaskIdLow=(WithPC_RxBuf.RxStruct.CommandData.CANCommand.NeedExtId<<3)&0xffff|CAN_ID_EXT;;
		CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器 0 关联到 FIFO0
		CAN1_FilerConf.FilterNumber=0; //过滤器 0
		CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDLIST;
		CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
		CAN1_FilerConf.FilterActivation=ENABLE; //激活滤波器 0
		CAN1_FilerConf.BankNumber=14;
		HAL_CAN_ConfigFilter(&hcan1,&CAN1_FilerConf);
		rxRetry=WithPC_RxBuf.RxStruct.CommandData.CANCommand.RxRetry;	
		canRxDataIndex=0;
		while(rxRetry!=0)
		{
			txRetry=WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxRetry;
			while(txRetry!=0)
			{
				CanStatus=HAL_CAN_Transmit(&hcan1,WithPC_RxBuf.RxStruct.CommandData.CANCommand.TxTimeOut);
				if(CanStatus==HAL_OK)
				{
					break;
				}
				txRetry--;
			}
			if(txRetry==0)
			{
				CP1616_Client_SendError(WithPC_Command_CANRead,2);
				return;
			}
			NeedRxPocket=WithPC_RxBuf.RxStruct.CommandData.CANCommand.NeedRxPocket;
			while(NeedRxPocket!=0)
			{
				CanStatus=HAL_CAN_Receive(&hcan1,CAN_FIFO0,WithPC_RxBuf.RxStruct.CommandData.CANCommand.RxTimeOut);
				if(CanStatus==HAL_OK)
				{
					if(WithPC_RxBuf.RxStruct.CommandData.CANCommand.RxIDE==hcan1.pRxMsg->IDE)
					{
						for(i=0;i<8;i++)
						{
							canRxData[canRxDataIndex++]=hcan1.pRxMsg->Data[i];
						}
						NeedRxPocket--;
						continue;						
					}
					else
					{
						break;
					}				
				}
				else
				{
					break;
				}	
			}
			if(NeedRxPocket==0)
			{
				CP1616_Client_SendData(WithPC_Command_CANRead,canRxData,canRxDataIndex);
				return;
			}
			else
				rxRetry--;
		}
		if(rxRetry==0)
		{
			CP1616_Client_SendError(WithPC_Command_CANRead,3);
			return;
		}
	}	
}
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_1TQ;
  hcan1.Init.BS1 = CAN_BS1_1TQ;
  hcan1.Init.BS2 = CAN_BS2_1TQ;
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = DISABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = DISABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN1 GPIO Configuration    
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
