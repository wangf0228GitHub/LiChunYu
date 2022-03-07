/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t huart2Rx;
uint8_t huart3Rx;

uint8_t LT_RxList[20];
uint32_t LT_RxIndex;
uint32_t bLTRx;


uint8_t M_RxList[20];
uint8_t M_TxList[10];
uint32_t M_RxIndex;
uint32_t bMRx;

uint8_t MBusAddr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t MBusLock(void)
{
	uint8_t sum = 0,i;
	uint8_t rel;
	M_TxList[0]=0x16;
	M_TxList[1]=MBusAddr;
	M_TxList[2]=0x01;
	M_TxList[3]=00;
	sum = (uint8_t)(sum + 0x16);
	sum = (uint8_t)(sum + MBusAddr);
	sum = (uint8_t)(sum + 0x01);
	sum = (uint8_t)(sum + 0x00);
	M_TxList[4]=sum;
	M_TxList[5]=0x0d;
	rel = MBusAddr / 20;
	GPIOC->BSRR = 0xff00;
	GPIOC->BSRR = rel;
	HAL_Delay(100);
	HAL_UART_Transmit(&huart3, M_TxList, 6, 100);//ะ่าช25ms
	//HAL_TIM_Base_Start_IT(&htim14);
	if (HAL_UART_Receive(&huart3, M_RxList, 6, 500) == HAL_OK)
	{
		if(M_RxList[0]!=0x16)
			return HAL_ERROR;
		if (M_RxList[1] != MBusAddr)
			return HAL_ERROR;
		if (M_RxList[2] != 0x01)
			return HAL_ERROR;
		if (M_RxList[3] != 0x01)
			return HAL_ERROR;
		sum = 0;
		for (i = 0; i < 4; i++)
		{
			sum += M_RxList[i];
		}
		if (M_RxList[4] != sum)
			return HAL_ERROR;
		if (M_RxList[5] != 0x0d)
			return HAL_ERROR;
		return HAL_OK;
	}
	else
	{
		return HAL_ERROR;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	float k, b, a,fx;
	k = 0.04578;
	b = -93.75;
	a = 10;
	uint32_t AD = 1910;
	uint16_t ret;
	fx = k*AD + b;
	fx = fx*a;
	ret = (uint16_t)ret;
	return 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
  while (HAL_UART_Receive_IT(&huart2, &huart2Rx, 1) == HAL_OK);
  //while (HAL_UART_Receive_IT(&huart3, &huart3Rx, 1) == HAL_OK);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (bLTRx)
	  {
		  switch (LT_RxList[2])
		  {
		  case 1:
			  MBusAddr = (LT_RxList[6] - '0') * 100 + (LT_RxList[7] - '0') * 10 + (LT_RxList[8] - '0') * 1;
			  if (MBusLock() == HAL_OK)
				  HAL_UART_Transmit(&huart2, LT_RxList, LT_RxIndex, 500);
			  else
			  {
				  LT_RxList[5] = '1';
				  HAL_UART_Transmit(&huart2, LT_RxList, LT_RxIndex, 500);
			  }				  
			  break;
		  default:
			  break;
		  }
	  }
	  if (huart2.RxState != HAL_UART_STATE_BUSY_RX)
	  {
		  while (HAL_UART_Receive_IT(&huart2, &huart2Rx, 1) == HAL_OK);
	  }
// 	  if (huart3.RxState != HAL_UART_STATE_BUSY_RX)
// 	  {
// 		  while (HAL_UART_Receive_IT(&huart3, &huart3Rx, 1) == HAL_OK);
// 	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
