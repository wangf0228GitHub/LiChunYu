/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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
#include "tim.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */

/************************************************************************/
/* IR工作区                                                             */
/************************************************************************/
unsigned char IRTxList[100];//用于生成要发送的序列，最多50个字节
volatile unsigned char IRTxCount;//要发送序列的长度
unsigned char IRTxIndex;//当前发送序列的索引
unsigned char NoICRxTimes;
GPIO_TypeDef* IRTx_GPIOx;
uint16_t IRTx_GPIO_Pin;
volatile unsigned int IRICDataList[100];
volatile unsigned char IRICIndex;
unsigned char IRRxList[50];
unsigned char IRRxIndex;
void IRWork(void)
{
	unsigned char i,j;
	unsigned short tBase;
	for(i=0;i<WithPC_RxBuf.RxStruct.CommandData.IRWork.count;i++)	
	{
		IRTxList[i+i]=LOW_NIBBLE(WithPC_RxBuf.RxStruct.CommandData.IRWork.TxData[i]);
		IRTxList[i+i+1]=HIGH_NIBBLE(WithPC_RxBuf.RxStruct.CommandData.IRWork.TxData[i]);
	}
	IRTxCount=WithPC_RxBuf.RxStruct.CommandData.IRWork.count<<1;
	IRTxIndex=0;
	gFlags.Bits.bIRTxPulse=1;
	gFlags.Bits.bIRTxFinish=0;
	IRTx_GPIOx=IRTx_GPIO_Port;//当前控制的为红外IR工作管脚
	IRTx_GPIO_Pin=IRTx_Pin;

	TIM14->ARR=IRTxPulseLen;
	TIM14->CNT=0;
	HAL_GPIO_WritePin(IRTx_GPIOx, IRTx_GPIO_Pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(&htim14); 
	while(!gFlags.Bits.bIRTxFinish);
	NoICRxTimes=0;
	gFlags.Bits.bIRRxFinish=0;	
	IRICIndex=0;
	gFlags.Bits.bFirstIC=1;
	HAL_TIM_IC_Start_IT(&htim13,TIM_CHANNEL_1);
	while(!gFlags.Bits.bIRRxFinish);
	if(IRICIndex==0)//超时接收
	{
		CP1616_Client_SendError(WithPC_Command_IRWork,1);
	}
	else
	{
		for(i=0;i<IRICIndex;i++)
		{
			if(IRICDataList[i]>992)
			{
				tBase=992+64;
				for(j=0;j<16;j++)
				{
					if(IRICDataList[i]<tBase)
						break;
					tBase+=64;
				}
				if(j==16)
				{
					CP1616_Client_SendError(WithPC_Command_IRWork,2);
					return;
				}
				else
				{
					IRICDataList[i]=j;
				}
			}
		}
		IRRxIndex=0;
		for(i=0;i<IRICIndex;i+=2)
		{
			IRRxList[IRRxIndex]=MAKE_BYTE(IRICDataList[i+1],IRICDataList[i]);
			IRRxIndex++;
		}
		CP1616_Client_SendData(WithPC_Command_IRWork,IRRxList,IRRxIndex);
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM14)
	{
		unsigned short tx=4096-IRTxPulseLen-200;
		//HAL_GPIO_TogglePin(TestIO_GPIO_Port, TestIO_Pin);
		HAL_GPIO_TogglePin(IRTx_GPIOx, IRTx_GPIO_Pin);
		if(gFlags.Bits.bIRTxPulse)
		{
			tx=tx+((unsigned short)(IRTxList[IRTxIndex])*245);			
			gFlags.Bits.bIRTxPulse=0;
			if(IRTxIndex==IRTxCount)
			{
				HAL_TIM_Base_Stop_IT(&htim14);
				gFlags.Bits.bIRTxFinish=1;
// 				bFirstIC=1;
// 				NoICRxTimes=0;
// 				TIM13->CR1 |= TIM_CR1_URS;
// 				gTempIndex=0;
// 				__HAL_TIM_ENABLE_IT(&htim13, TIM_IT_UPDATE);
// 				HAL_TIM_IC_Start_IT(&htim13,TIM_CHANNEL_1);
			}
			IRTxIndex++;
// 			if(IRTxIndex==IRTxCount)
// 			{
// 				HAL_TIM_Base_Stop_IT(&htim14);
// 				gFlags.Bits.bIRTxFinish=1;
// 			}
		}
		else
		{
			tx=IRTxPulseLen;
			gFlags.Bits.bIRTxPulse=1;						
		}
		TIM14->ARR=tx;	
		TIM14->CNT=0;
	}
	else if(htim->Instance==TIM13)
	{
		if(gFlags.Bits.bFirstIC)
		{
			NoICRxTimes++;
			if(NoICRxTimes>200)
			{
				gFlags.Bits.bIRRxFinish=1;
				HAL_TIM_IC_Stop_IT(&htim13,TIM_CHANNEL_1);
			}
		}
		else
		{
			gFlags.Bits.bIRRxFinish=1;
			HAL_TIM_IC_Stop_IT(&htim13,TIM_CHANNEL_1);
// 			unsigned char i,j;
// 			unsigned short tBase;
// 			for(i=0;i<gTempIndex;i++)
// 			{
// 				if(gTemp32[i]>992)
// 				{
// 					tBase=992+64;
// 					for(j=0;j<16;j++)
// 					{
// 						if(gTemp32[i]<tBase)
// 							break;
// 						tBase+=64;
// 					}
// 					if(j==16)
// 					{
// 
// 					}
// 					else
// 					{
// 						gTemp32[i]=j;
// 					}
// 				}
// 			}
// 			IRRxIndex=0;
// 			for(i=0;i<gTempIndex;i+=2)
// 			{
// 				IRRxList[IRRxIndex]=MAKE_BYTE(gTemp32[i+1],gTemp32[i]);
// 				IRRxIndex++;
// 			}
		}
	}
// 	else if(htim->Instance==TIM3)
// 	{
// 		if(bFirstIC!=0)
// 		{
// 			NoICRxTimes++;
// 			if(NoICRxTimes>200)
// 			{
// 				HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
// 			}
// 		}
// 		else
// 		{			
// 			HAL_GPIO_WritePin(IR1_EN1_GPIO_Port, IR1_EN1_Pin, GPIO_PIN_RESET);
// 			unsigned char i,j;
// 			unsigned short tBase;
// 			for(i=0;i<gTempIndex;i++)
// 			{
// 				if(gTemp32[i]>992)
// 				{
// 					tBase=992+64;
// 					for(j=0;j<16;j++)
// 					{
// 						if(gTemp32[i]<tBase)
// 							break;
// 						tBase+=64;
// 					}
// 					if(j==16)
// 					{
// 
// 					}
// 					else
// 					{
// 						gTemp32[i]=j;
// 					}
// 				}
// 			}
// 			IRRxIndex=0;
// 			for(i=0;i<gTempIndex;i+=2)
// 			{
// 				IRRxList[IRRxIndex]=MAKE_BYTE(gTemp32[i+1],gTemp32[i]);
// 				IRRxIndex++;
// 			}
// 			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
// 		}
// 	}
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断
{	
	//HAL_TIM_IC_Start_IT(&htim13,TIM_CHANNEL_1);
	if(htim->Instance==TIM13)
	{
		TIM13->CNT=0;
		if(gFlags.Bits.bFirstIC)
		{			
			gFlags.Bits.bFirstIC=0;
		}
		else
		{
			IRICDataList[IRICIndex++]=HAL_TIM_ReadCapturedValue(&htim13,TIM_CHANNEL_1)+19;
// 			if(gTempIndex==16)
// 			{
// 				
// 				HAL_TIM_IC_Stop_IT(&htim13,TIM_CHANNEL_1);
// 			}
			TIM13->CNT=0;
		}
	}
// 	if(htim->Instance==TIM3)
// 	{
// 		TIM3->CNT=0;
// 		if(bFirstIC!=0)
// 		{			
// 			bFirstIC=0;
// 		}
// 		else
// 		{
// 			gTemp32[gTempIndex++]=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1)+19;
// 			// 			if(gTempIndex==16)
// 			// 			{
// 			// 				
// 			// 				HAL_TIM_IC_Stop_IT(&htim13,TIM_CHANNEL_1);
// 			// 			}
// 			TIM3->CNT=0;
// 		}
// 	}
}
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 84;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 3000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 8;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* TIM13 init function */
void MX_TIM13_Init(void)
{
  TIM_IC_InitTypeDef sConfigIC;

  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 84;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 3000;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim13) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 8;
  if (HAL_TIM_IC_ConfigChannel(&htim13, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
/* TIM14 init function */
void MX_TIM14_Init(void)
{

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 21;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 1024;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  
    /**TIM3 GPIO Configuration    
    PA6     ------> TIM3_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM13)
  {
  /* USER CODE BEGIN TIM13_MspInit 0 */

  /* USER CODE END TIM13_MspInit 0 */
    /* TIM13 clock enable */
    __HAL_RCC_TIM13_CLK_ENABLE();
  
    /**TIM13 GPIO Configuration    
    PF8     ------> TIM13_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* TIM13 interrupt Init */
    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  /* USER CODE BEGIN TIM13_MspInit 1 */

  /* USER CODE END TIM13_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspInit 0 */

  /* USER CODE END TIM14_MspInit 0 */
    /* TIM14 clock enable */
    __HAL_RCC_TIM14_CLK_ENABLE();

    /* TIM14 interrupt Init */
    HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  /* USER CODE BEGIN TIM14_MspInit 1 */

  /* USER CODE END TIM14_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  
    /**TIM3 GPIO Configuration    
    PA6     ------> TIM3_CH1 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM13)
  {
  /* USER CODE BEGIN TIM13_MspDeInit 0 */

  /* USER CODE END TIM13_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM13_CLK_DISABLE();
  
    /**TIM13 GPIO Configuration    
    PF8     ------> TIM13_CH1 
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_8);

    /* TIM13 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
  /* USER CODE BEGIN TIM13_MspDeInit 1 */

  /* USER CODE END TIM13_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspDeInit 0 */

  /* USER CODE END TIM14_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM14_CLK_DISABLE();

    /* TIM14 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  /* USER CODE BEGIN TIM14_MspDeInit 1 */

  /* USER CODE END TIM14_MspDeInit 1 */
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
