/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile _GFlags gFlags;
uint32_t FlashTick;
unsigned int gTemp32[50];
volatile unsigned char gTempIndex;
char MessgeBuf[200];
CanTxMsgTypeDef CANTxMessage; //发送消息
CanRxMsgTypeDef CANRxMessage; //接收消息



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	unsigned int i,x;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
  gTempIndex=0;


  CP1616_Client_Init();
  FlashTick=HAL_GetTick();
  hcan1.pTxMsg=&CANTxMessage; //发送消息
  hcan1.pRxMsg=&CANRxMessage;  


  TIM14->CR1 |= TIM_CR1_URS;
  __HAL_TIM_CLEAR_FLAG(&htim14, TIM_FLAG_UPDATE);
  HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);


  TIM3->CR1 |= TIM_CR1_URS;  
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);

  TIM13->CR1 |= TIM_CR1_URS;  
  __HAL_TIM_ENABLE_IT(&htim13, TIM_IT_UPDATE);
  /************************************************************************/
  /* 临时代码                                                             */
  /************************************************************************/

  
   
   
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  if(HAL_UART_Receive_IT(&UART_WithPC_Handle,aRxBuffer_WithPC,1)!=HAL_OK)
	  Error_Handler();
//   __HAL_TIM_CLEAR_IT(&htim14, TIM_IT_UPDATE);
//   //htim14.Instance->CR1 |=TIM_CR1_URS;
//    HAL_TIM_Base_Start_IT(&htim14); 
//    //HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
//    HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
// //   HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);
	while (1)
	{
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
 		if(GetDeltaTick(FlashTick)>500)
 		{
 			FlashTick=HAL_GetTick();
// 			if(CAN1_Send_Msg(txData[0],8)==0)
// 			{
// 				if(HAL_CAN_Receive(&hcan1,CAN_FIFO0,100000)!=HAL_OK)
// 					LED_DS1_OFF();
// 				else
// 					LED_DS1_ON();
// 			}
// 			else
// 			    LED_DS1_OFF();
//  			if(TIM14->CCR1==249)
  				//TIM14->CCR1=1;
//  			else
//  				TIM14->CCR1++;
//  			LED_DS0_Toggle();
//  			LED_DS1_Toggle();
 			//HAL_UART_Transmit(&huart2,txBuf,3,1000);
 		}
//		if(gFlags.Bits.bCap)
//		{
//// 			x=sprintf(MessgeBuf,"%ld us\r\n",gTemp32[0]);
//// 			HAL_UART_Transmit(&huart2,(unsigned char*)MessgeBuf,x,1000);
//// 			gFlags.Bits.bCap=0;
//// 			HAL_TIM_IC_Start_IT(&htim5,TIM_CHANNEL_1);
//		}
		if(gFlags.Bits.bWithPC_Rx)
		{
			ProcCommandFromPC();
		}
	}
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
