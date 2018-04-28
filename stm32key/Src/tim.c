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
#include "..\wf\Variables.h"
#include "..\wf\IRProc.h"
#include "..\..\..\WF_Device\wfDefine.h"
__IO uint32_t tError=0;
__IO uint32_t tICTimes=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint32_t i,x;	
	if(htim->Instance==htim2.Instance)//pwm
	{
		if(gFlags.bIRTx)//发送，应为pwm中断
		{
			if(IRTxIndex<IRTxCount)
			{
				i=IRTxIndex>>1;
				if(GetBit(IRTxIndex,0))
					x=HIGH_NIBBLE(IRTxList[i]);
				else
					x=LOW_NIBBLE(IRTxList[i]);
				x=x<<6;
				x=x+1019;
				htim->Instance->ARR=x;
				IRTxIndex++;
			}
			else
			{		
				__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC3);
				HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3); 					
			}
		}
		else//接收状态，定时器中断表示接收出错，重新置接收状态位
		{			
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_4);
			HAL_TIM_Base_Stop_IT(&htim2);
			gFlags.bFirstIC=1;
			IRRxCount=0;
			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
		}
	}
}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim2.Instance)//pwm
	{
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
		{
			HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_3);
			HAL_TIM_Base_Stop_IT(htim);	
			gFlags.bTxFinish=1;
		}
	}
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint8_t h,l;
	uint32_t read;
	if(htim->Instance==htim2.Instance)//捕捉
	{
		if(gFlags.bFirstIC)
		{
			gFlags.bFirstIC=0;
			htim2.Instance->CNT=0;
			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
			HAL_TIM_Base_Start_IT(&htim2);	
			gFlags.bIRRxH=0;
		}
		else
		{
			read=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_4)+13;
			tICTimes++;
			if(read<992)
			{
				tError=tICTimes;
				return;
			}
			if(gFlags.bIRRxH)//第二个字节，开始组合
			{
				IRRxByteH=read;
				htim2.Instance->CNT=0;
				h=ThranslateIRRx(IRRxByteH);
				l=ThranslateIRRx(IRRxByteL);
				if(h>0x10 || l>0x10)
				{
					IRRxCount=0;
					IRRxNeedCount=0xffff;
				}
				else
				{
					IRRxList[IRRxCount++]=MAKE_BYTE(h,l);
				}
				gFlags.bIRRxH=0;
				if(IRRxCount>2)
				{
					if(IRRxCount==IRRxNeedCount)
					{
						HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_4);
						HAL_TIM_Base_Stop_IT(htim);	
						IRRxTick=HAL_GetTick();
						gFlags.bIRRxFrame=1;
					}
				}
				else if(IRRxCount==2)//用于判断接收数据的长度
				{
					switch(IRCommand)
					{
					case 0x08:
					case 0x0f:
					case 0x6a:
					case 0x71:
						HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_4);
						HAL_TIM_Base_Stop_IT(htim);	
						IRRxTick=HAL_GetTick();
						gFlags.bIRRxFrame=1;
						break;
					case 0x73:
					case 0x79:
						IRRxNeedCount=2+2;
						break;
					case 0x7a:
						IRRxNeedCount=7+2;
						break;
					case 0x26:
					case 0x63:
					case 0x3e:
					case 0x39:
						IRRxNeedCount=8+2;
						break;
					default:
						IRRxNeedCount=9+2;
						break;
					}
				}
				else if(IRRxCount==1)//0x10
				{
					if(IRRxList[0]!=0x10)
						IRRxCount=0;
				}
			}
			else
			{
				IRRxByteL=read;
				htim2.Instance->CNT=0;
				gFlags.bIRRxH=1;
			}
		}
	}
}
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 31;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 20;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(tim_pwmHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    /**TIM2 GPIO Configuration    
    PA3     ------> TIM2_CH4 
    */
    GPIO_InitStruct.Pin = IRTx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(IRTx_GPIO_Port, &GPIO_InitStruct);

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */
  
    /**TIM2 GPIO Configuration    
    PA1     ------> TIM2_CH2
    PA2     ------> TIM2_CH3 
    */
    GPIO_InitStruct.Pin = RFIRTx_Pin|CarIRTx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }

}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA1     ------> TIM2_CH2
    PA2     ------> TIM2_CH3
    PA3     ------> TIM2_CH4 
    */
    HAL_GPIO_DeInit(GPIOA, RFIRTx_Pin|CarIRTx_Pin|IRTx_Pin);

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
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
