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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "usart.h"
#define IRDebugTim htim13
#define IRDebugTim_TIM_CHANNEL TIM_CHANNEL_1
uint32_t bFirstIC;
uint32_t bIRRx;
uint32_t IRRxCount;
uint32_t IRRxList[50];
uint32_t IRRxByteIndex;
uint8_t IRRxByte[2];
uint8_t	UartTx[50];
uint8_t	UartErrTx[3];
uint8_t UartTxCount;
__IO uint32_t test1;
__IO uint32_t test2;
void InitIRDebug(void)
{
	while(1)
	{
		if(HAL_GPIO_ReadPin(bOnCar_GPIO_Port,bOnCar_Pin)==GPIO_PIN_SET)
			break;
	}
	UartErrTx[0]=0x55;
	UartErrTx[2]=0x55;
	bFirstIC=1;
	IRRxCount=0;
	bIRRx=0;
	IRRxByteIndex=0;
	__HAL_TIM_CLEAR_IT(&IRDebugTim, TIM_IT_UPDATE);
	HAL_TIM_IC_Start_IT(&IRDebugTim, IRDebugTim_TIM_CHANNEL);	
	test2=test1;
	test1=0;
}
void IRRxProc(void)
{
	uint32_t i,x,y;
	if(bIRRx!=0)
	{
// 		if((IRRxCount&0x01)!=0)
// 		{
// 			InitIRDebug();
// 			UartErrTx[1]=0x01;
// 			HAL_UART_Transmit(&huart1,UartErrTx,3,1000);			
// 			return;
// 		}
		for(i=0;i<IRRxCount;i++)
		{
			if(IRRxList[i]>992)
			{
				x=992+64;
				for(y=0;y<16;y++)
				{
					if(IRRxList[i]<x)
						break;
					x+=64;
				}
				if(y>=16)
				{
// 					InitIRDebug();
// 					UartErrTx[1]=0x02;
// 					HAL_UART_Transmit(&huart1,UartErrTx,3,1000);			
// 					return;
// 					gFlags.bIRRxErr=1;
// 					return;
					IRRxList[i]=0;
				}
				else
				{
					IRRxList[i]=y;
				}
			}
			else
			{
				IRRxList[i]=0;
// 				InitIRDebug();
// 				for(i=0;i<10;i++)
// 					UartTx[i]=0x55;
// 				UartErrTx[1]=0x03;
// 				HAL_UART_Transmit(&huart1,UartErrTx,3,1000);			
// 				return;
// 				gFlags.bIRRxErr=1;
// 				return;
			}
		}	
		if((IRRxCount&0x01)!=0)
			IRRxCount--;
		for(i=0;i<IRRxCount;i+=2)
			UartTx[i>>1]=((IRRxList[i+1]<<4)+(IRRxList[i]&0x0f));		
		UartTxCount=IRRxCount>>1;
		InitIRDebug();
		HAL_UART_Transmit(&huart1,UartTx,UartTxCount,1000);		
	}

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint32_t i,x;	
	if(htim->Instance==IRDebugTim.Instance)//pwm
	{
		//接收状态，定时器中断表示接收超时，一帧结束
		HAL_TIM_IC_Stop_IT(&IRDebugTim,IRDebugTim_TIM_CHANNEL);		
		HAL_TIM_Base_Stop_IT(&IRDebugTim);	
		bIRRx=1;
	}
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==IRDebugTim.Instance)//捕捉
	{
		test1++;
		if(bFirstIC!=0)
		{
			bFirstIC=0;
 			//HAL_TIM_IC_Stop_IT(&IRDebugTim,IRDebugTim_TIM_CHANNEL);
 			//HAL_TIM_Base_Stop(&IRDebugTim);
			IRDebugTim.Instance->CNT=0;
			__HAL_TIM_CLEAR_IT(&IRDebugTim, TIM_IT_UPDATE);
			HAL_TIM_Base_Start_IT(&IRDebugTim);
			//HAL_TIM_ReadCapturedValue(&IRDebugTim,IRDebugTim_TIM_CHANNEL);	
			//HAL_TIM_IC_Start_IT(&IRDebugTim, IRDebugTim_TIM_CHANNEL);			
		}
		else
		{			
			IRRxList[IRRxCount++]=HAL_TIM_ReadCapturedValue(&IRDebugTim,IRDebugTim_TIM_CHANNEL)+10;
			IRDebugTim.Instance->CNT=0;
		}
	}
}
/* USER CODE END 0 */

TIM_HandleTypeDef htim13;

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
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim13, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(tim_baseHandle->Instance==TIM13)
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
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM13)
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
