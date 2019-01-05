/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcyHash.h"
#include "lcyIRDecode.h"
#include "..\wf\OnCarProc.h"
#include "..\wf\Variables.h"
#include "wfSys.h"
#include "..\wf\Function.h"
#include "..\wf\ButtonProc.h"
#include "SimSPI.h"
#include "..\wf\AS3933.h"
#include "..\wf\ATA5824.h"
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
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const uint8_t eeprom[160]=
{
	0x00, 0x49, 0xE4, 0x8C, 0x8C, 0xD2, 0x00, 0x15, //0x00～07
	0x01, 0xB8, 0x98, 0x61, 0x5A, 0xB2, 0x09, 0x78, //0x08～0F
	0x98, 0x63, 0xA4, 0x8A, 0xCD, 0xD5, 0xB5, 0xEB, //0x10～17
	0x3C, 0xDD, 0x73, 0xED, 0xBB, 0x8C, 0x13, 0x9C, //0x18～1F
	0x41, 0xAB, 0xCA, 0x86, 0x8B, 0xB8, 0x3C, 0x30, //0x20～27
	0xB9, 0x20, 0x29, 0x36, 0x64, 0x48, 0xFC, 0x53, //0x28～2F
	0x29, 0xB5, 0x7C, 0xC9, 0xFB, 0x2E, 0x1B, 0xB3, //0x30～37
	0x2F, 0x13, 0xAC, 0x5E, 0xA7, 0x8F, 0xBA, 0x2F, //0x38～3F
	0xC0, 0x48, 0x17, 0x71, 0x76, 0x41, 0x7F, 0xB1, //0x40～47
	0xE7, 0x1E, 0x2E, 0x10, 0xB3, 0xE3, 0xCE, 0xA6, //0x48～4F
	0xD3, 0xE3, 0x96, 0x1F, 0x91, 0x90, 0x05, 0xDC, //0x50～57
	0xFB, 0x9E, 0xDD, 0x23, 0xE6, 0x6C, 0x58, 0x8D, //0x58～5F
	0x50, 0x36, 0xBF, 0xB9, 0xAF, 0x62, 0x3F, 0x68, //0x60～67
	0x77, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, //0x68～6F
	0xFF, 0x7E, 0x97, 0xC7, 0xDF, 0xFD, 0x58, 0xDA, //0x70～77
	0x27, 0x8F, 0x54, 0x7F, 0xE4, 0xFF, 0xFF, 0xFF, //0x78～7F
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //0x80～87
	0xE0, 0xA8, 0x16, 0x0E, 0xB8, 0x98, 0x61, 0x5A, //0x88～8F
	0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, //0x90～97
	0x73, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x21, 0xDF, //0x98～9F
};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t addr,intiEE[162];
	uint32_t i;
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
  MX_ADC_Init();
  MX_TIM2_Init();
  MX_TIM21_Init();
  MX_TIM22_Init();
  /* USER CODE BEGIN 2 */
  gFlags.all=0;
  wfDelay_init(32);
  gFlags.bTxFinish=1; 
  /************************************************************************/
  /* test                                                                */
  /************************************************************************/
  //for(i=0;i<160;i+=8)
  //{
	 // RomData_WriteBytes(i,(uint8_t*)(&eeprom[i]),8);
  //}  
//      for(i=0;i<162;i++)
//    	  intiEE[i]=0x00;
//      RomData_WriteBytes(0,intiEE,162);

      //AS3933_COMM(AS3933_COMM_PResetDefault);
      //AS3933_Init();
	/************************************************************************/
	/* 5824test                                                                     */
	/************************************************************************/


   //HAL_GPIO_WritePin(ATA5824_PWR_ON_GPIO_Port,ATA5824_PWR_ON_Pin,GPIO_PIN_SET);
   //ATA5824_Init();
  
   //  	GetKeyState();
   //  	GetKeyParam();//获得钥匙当前相关数据
   //	while(1)
   //	{
   //		ATA5824_WaitRx(-1);
   //		UsedDEC();
   //		GetKeyState();
   //		GetKeyParam();//获得钥匙当前相关数据
   //		wfDelay_ms(1000);
   //	}
	/************************************************************************/
	/*  红外test                                                            */
	/************************************************************************/
//  	while(1)
//  	{
//  		wfDelay_ms(1000);
//  		for(i=0;i<16;i++)
//  		{
//  			IRTxList[i]=i;
//  		}
//  		IRTxCount=16;
//  		CarIRTxProc();
//  	}
  /************************************************************************/
  /* 3933                                                                 */
  /************************************************************************/
//    AS3933_COMM(AS3933_COMM_PResetDefault);
//    AS3933_Init();
//    while(bAS3933Wake()==GPIO_PIN_RESET);
//   NVIC_SystemReset();
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  GetKeyState();
//  GetKeyParam();
  if(bOnCarPower())
  {
	  AS3933_COMM(AS3933_COMM_PResetDefault);
	  AS3933_Init();  
	  GetKeyState();
	  GetKeyParam();//获得钥匙当前相关数据
	  OnCarProc();
  }
  else 
  {
	  ReadButton();
	  if(curKeyStateFlags.keyValue!=NoKey)
	  {
		  AS3933_COMM(AS3933_COMM_PResetDefault);
		  AS3933_Init();  
		  ButtionProc();
	  }
	  else if(bAS3933Wake())
	  {
		  if(RomStateFlags.bRomWrited && RomStateFlags.bStudy)
		  {
			  BAT_ON();	 		  
			  ATA5824_Init();
			  ATA5824_WaitRx(100);
			  BAT_OFF();		
			  while(1);
		  }
		  else
		  {
			  BAT_OFF();
			  while(1);
			  //NVIC_SystemReset();
		  }
	  }
	  else
	  {
		  AS3933_COMM(AS3933_COMM_PResetDefault);
		  AS3933_Init();  
#ifdef KeepPower
		  if(bOnCarPower()==OnCarPowerState_OFF)
			  NVIC_SystemReset();
#else
		  LEDFlash();
#endif  
	  }
	  BAT_OFF();
	  while(1)
	  {
#ifdef KeepPower
		  if(bOnCarPower()==OnCarPowerState_OFF)
			  NVIC_SystemReset();
#else
		  LEDFlash();
#endif
	  }	  
  }

   
   
//   AS3933_Init();
//   ATA5824_Init();
//   HAL_Delay(10);

  GetKeyState();
  GetKeyParam();//获得钥匙当前相关数据
//  ButtionProc();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(bOnCarPower())
	  {
		  OnCarProc();
	  }
// 	  else
// 	  {
// 
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
  while(1) 
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
