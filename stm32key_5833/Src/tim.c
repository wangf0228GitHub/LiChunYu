/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "..\wf\Variables.h"
#include "..\..\..\WF_Device\wfDefine.h"
#include "..\wf\OnCarProc.h"
#include "..\wf\ATA5833.h"
uint32_t irddd[1000];
uint32_t irdddcc;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint32_t x;	
	//uint8_t rxBitList[1000],rxByteList[100],rxBitList1[1000];
	//uint32_t rxBitLen=0,rxByteLen=0,rxBitLen1=0,nBit;
	//uint32_t bData=0,bOdd=0;
	if(htim->Instance==htim2.Instance)//pwm
	{
		switch(TimWorkType)
		{
		case CarIRRx://接收状态，定时器中断表示接收出错，重新置接收状态位								
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_4);
			HAL_TIM_Base_Stop_IT(&htim2);
			gFlags.Bits.bFirstIC=1;
			IRRxCount=0;
			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
			break;
		case CarIRTx://车载红外发射
			if(IRTxIndex<IRTxCount)
			{
				x=IRTxDataList[IRTxIndex];
				x=x<<6;
				x=x+1000;
				htim->Instance->ARR=x;//单周期pwm发送
				IRTxIndex++;
			}
			else if(IRTxIndex==IRTxCount)
			{				
				//最后一个半字节需启动pwm中断，完成最后一个低电平脉冲
				__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_CC3);
				HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_3); 					
			}
			break;
			// 		case RFTx://射频发射，500us中断，反转电平
			// 			if(GetBit(IRTxDataList[IRTxIndex],RFTxBitIndex))
			// 			{
			// 				RFDataHigh();
			// 			}
			// 			else
			// 			{
			// 				RFDataLow();
			// 			}
			// 			RFTxBitIndex++;
			// 			if(RFTxBitIndex>=8)//发送完成1个字节
			// 			{
			// 				RFTxBitIndex=0;
			// 				IRTxIndex++;
			// 				if(IRTxIndex>=IRTxCount)//全部发送完成
			// 				{
			// 					HAL_TIM_Base_Stop_IT(&htim2);	
			// 					gFlags.Bits.bTxFinish=1;
			// 				}
			// 			}
			// 			break;
		case RFIRTx://车载红外发射
			HAL_TIM_Base_Stop_IT(&htim2);	
			RFIRPulseTimes=0;
			htim2.Instance->ARR=16;
			htim2.Instance->CNT=0;			
			HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);//数据发送完成，再次启动头脉冲
			break;
		default:
			break;
		}		
	}
	else if(htim->Instance==htim16.Instance)//帧完成
	{
		HAL_TIM_IC_Stop_IT(&htim16,TIM_CHANNEL_1);
		HAL_TIM_Base_Stop_IT(&htim16);
		gFlags.Bits.ATA583X_bRxFrame=1;		
	}
}
//红外发送的最后半字节需要pwm中断来结束发送序列
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{	
	uint32_t x;
	if(htim->Instance==htim2.Instance)//pwm
	{		
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)//车载发送已完成最后一个低电平脉冲,即发送完成
		{
			HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_3);
			HAL_TIM_Base_Stop_IT(htim);	
			gFlags.Bits.bTxFinish=1;
		}
		else if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)//射频发送头
		{
			RFIRPulseTimes++;
			if(RFIRPulseTimes>7)
			{
				HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_2);//停止头脉冲
				if(IRTxIndex>=IRTxCount)
				{					
					HAL_TIM_Base_Stop_IT(htim);	
					gFlags.Bits.bTxFinish=1;
				}
				else
				{
					HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_2);
					x=IRTxDataList[IRTxIndex];
					x=x<<8;
					x=x+1113;
					htim->Instance->ARR=x;//定时器中断启动，发送数据
					__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
					HAL_TIM_Base_Start_IT(&htim2); 
					IRTxIndex++;
				}
			}
		}
	}	
}
//红外接收
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint8_t h,l;
	uint32_t read;
	if(htim->Instance==htim2.Instance)//捕捉
	{
		if(gFlags.Bits.bFirstIC)
		{
			gFlags.Bits.bFirstIC=0;
			htim2.Instance->CNT=0;
			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
			HAL_TIM_Base_Start_IT(&htim2);	
			gFlags.Bits.bIRRxH=0;
			irdddcc=0;
		}
		else
		{
			read=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_4)+26;
			irddd[irdddcc++]=read;
			if(read<992)
			{
				return;
			}
			if(gFlags.Bits.bIRRxH)//第二个字节，开始组合
			{
				IRRxByteH=read;
				htim2.Instance->CNT=0;
				h=ThranslateCarIRRx(IRRxByteH);
				l=ThranslateCarIRRx(IRRxByteL);
				if(h>0x10 || l>0x10)
				{
					IRRxCount=0;
					IRRxNeedCount=0xffff;
				}
				else
				{
					IRRxList[IRRxCount++]=MAKE_BYTE(h,l);
				}
				gFlags.Bits.bIRRxH=0;
				if(IRRxCount>2)
				{
					if(IRRxCount==IRRxNeedCount)
					{
						HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_4);
						HAL_TIM_Base_Stop_IT(htim);	
						IRRxTick=HAL_GetTick();
						gFlags.Bits.bIRRxFrame=1;
					}
				}
				else if(IRRxCount==2)//用于判断接收数据的长度
				{
					switch(CarIRCommand)
					{
					case 0x08:
					case 0x0f:
					case 0x6a:
					case 0x71:
					case 0x46:
						HAL_TIM_PWM_Stop_IT(htim,TIM_CHANNEL_4);
						HAL_TIM_Base_Stop_IT(htim);	
						IRRxTick=HAL_GetTick();
						gFlags.Bits.bIRRxFrame=1;
						break;
					case 0x38:
						IRRxNeedCount=1+2;
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
				gFlags.Bits.bIRRxH=1;
			}
		}
	}
	else if(htim->Instance==htim16.Instance)
	{
		ATA583X_ICProc();
	}
}
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim16;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 79;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 6;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 20;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_MspPostInit(&htim2);

}
/* TIM16 init function */
void MX_TIM16_Init(void)
{
  TIM_IC_InitTypeDef sConfigIC = {0};

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 79;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 130;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim16, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_pwmHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA3     ------> TIM2_CH4 
    */
    GPIO_InitStruct.Pin = IRRx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(IRRx_GPIO_Port, &GPIO_InitStruct);

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM16)
  {
  /* USER CODE BEGIN TIM16_MspInit 0 */

  /* USER CODE END TIM16_MspInit 0 */
    /* TIM16 clock enable */
    __HAL_RCC_TIM16_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM16 GPIO Configuration    
    PB8     ------> TIM16_CH1 
    */
    GPIO_InitStruct.Pin = ATA583X_TMDO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF14_TIM16;
    HAL_GPIO_Init(ATA583X_TMDO_GPIO_Port, &GPIO_InitStruct);

    /* TIM16 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
  /* USER CODE BEGIN TIM16_MspInit 1 */

  /* USER CODE END TIM16_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA1     ------> TIM2_CH2
    PA2     ------> TIM2_CH3 
    */
    GPIO_InitStruct.Pin = RFIRTx_Pin|CarIRTx_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
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
    HAL_GPIO_DeInit(GPIOA, RFIRTx_Pin|CarIRTx_Pin|IRRx_Pin);

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM16)
  {
  /* USER CODE BEGIN TIM16_MspDeInit 0 */

  /* USER CODE END TIM16_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM16_CLK_DISABLE();
  
    /**TIM16 GPIO Configuration    
    PB8     ------> TIM16_CH1 
    */
    HAL_GPIO_DeInit(ATA583X_TMDO_GPIO_Port, ATA583X_TMDO_Pin);

    /* TIM16 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
  /* USER CODE BEGIN TIM16_MspDeInit 1 */

  /* USER CODE END TIM16_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
