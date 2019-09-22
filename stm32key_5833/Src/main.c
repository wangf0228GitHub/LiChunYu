/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "..\..\..\WF_Device\SimSPI.h"
#include "..\wf\ATA5833.h"
#include "..\..\..\WF_Device\SPIROM.h"
#include "..\wf\Variables.h"
#include "..\..\..\WF_Device\lcyIRDecode.h"
#include "..\..\..\WF_Device\lcyHash.h"
#include "..\wf\Function.h"
#include "..\wf\AS3933.h"
#include "..\wf\OnCarProc.h"
#include "..\wf\ButtonProc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t eedata[256];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
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
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  MX_TIM16_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  gFlags.all=0;
  wfDelay_init(80);
  gFlags.Bits.bTxFinish=1; 
  HAL_GPIO_WritePin(ATA583X_PWR_ON_GPIO_Port,ATA583X_PWR_ON_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(ATA583X_CS_GPIO_Port,ATA583X_CS_Pin,GPIO_PIN_SET);
  SimSPI_Init();
  SimSPI2_Init();
  SPIROM_Init();
  LED_OFF();
  GenerateEEPSW();
  bAS3933NeedChange=0;
  /************************************************************************/
  /* 芯片只读                                                             */
  /************************************************************************/
  FLASH_OBProgramInitTypeDef obData;
  HAL_FLASHEx_OBGetConfig(&obData);
  if(obData.RDPLevel==OB_RDP_LEVEL_0)
  {
	  HAL_FLASH_Unlock();
	  HAL_FLASH_OB_Unlock();
	  FLASH_OBProgramInitTypeDef obData;
	  HAL_FLASHEx_OBGetConfig(&obData);
	  obData.OptionType=OPTIONBYTE_RDP;
	  obData.RDPLevel=OB_RDP_LEVEL_1;
	  HAL_FLASHEx_OBProgram(&obData);
	  HAL_FLASH_OB_Lock();
	  HAL_FLASH_Lock();
  }
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  if(HAL_GPIO_ReadPin(USB_EN_GPIO_Port,USB_EN_Pin))
  {
	  MX_USB_DEVICE_Init();
	  while(1)
	  {
		  gTempIndex++;
		  ledFlash(500);
	  }
  }
  /************************************************************************/
  /* 3933唤醒测试		                                                    */
  /************************************************************************/
//   AS3933_Init();
//   while(1)
//   {
// 	  ReadANT();
// 	  if(bAS3933Wake())
// 	  {
// 		  wfDelay_us(2920);
// 		  AS3933_COMM(AS3933_COMM_ClearWake);
// 	  }
//   }
  /************************************************************************/
  /*                                                                      */
  /************************************************************************/
  //   CheckEEPSW();
  /************************************************************************/
  /* ee全部擦除                                                           */
  /************************************************************************/
//   for(i=0;i<255;i++)
// 	  intiEE[i]=0x00;
//   SPIROM_WriteArray(0,intiEE,255);
//   //while(1);
//   /************************************************************************/
//   /* 强行初始ee                                                           */
//   /************************************************************************/
//   for(i=0;i<160;i+=8)
//   {
// 	  RomData_WriteBytes(i,(uint8_t*)(&eeprom[i]),8);
//   }  
//   while(1);
//   for(i=0;i<8;i++)
// 	  lcyHashIn[i]=i+1;
//   LED_ON();
//   wfDelay_us(10);
//   LED_OFF();
//   //ReadEEPSW();//8ms
//   //GenerateEEPSW();//4ms
//   //lcyIRDecode();//12ms
//   //lcyHashOnce();//2ms
//   CheckEEPSW();//11.5ms
//   LED_ON();
  //ATA583X_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
   //SPIROM_ReadArray(0,intiEE,255);
//   RomData_ReadBytes(0,intiEE,255);
  //GetKeyState();
  //  GetKeyParam();
  //CarIRTx_Test0016();

  /************************************************************************/
  /* 无线测试                                                             */
  /************************************************************************/
  
//   GetKeyState();
// 	ATA583X_Init();	
// 	ATA583X_WaitRx(-1);
//// 	LED_OFF();
// 	HAL_Delay(100);
//   LED_ON();
//   ReadANT();
//   LED_OFF();
//   while(1);
  //BAT_ON();
  //GetKeyState();
  //ATA583X_Init();
  ////ATA583X_WaitRx(-1);
  //for(i=0;i<20;i++)
	 // ATA583X_TxList[i]=i;
  //ATA583X_TxCount=5;
  //while(1)
  //{
	 // ATA583X_TxFrameProc();
	 // wfDelay_ms(1000);
  //}	
   //CheckEEPSW();
//   RomStateFlags.Bits.bRFStudy=0;//射频注册成功
 // ChangeRFState(0xff);
//   GetKeyState();  
  /************************************************************************/
  /* 正经程序部分                                                         */
  /************************************************************************/
  if(bOnCarPower())
  {
	  AS3933_Init();  
	  GetKeyState();
	  GetKeyParam();//获得钥匙当前相关数据
	  OnCarProc();
	  BAT_OFF();
	  AS3933_Init();  
	  NVIC_SystemReset();
  }
  else 
  {
	  ReadButton();
	  if(curKeyStateFlags.keyValue!=NoKey)
	  {
		  AS3933_Init();  
		  ButtionProc();
	  }
	  else if(bAS3933Wake())
	  {
		  //PowerLed();
		  LED_ON();
		  GetKeyState();
		  if(RomStateFlags.Bits.bRomWrited && RomStateFlags.Bits.bStudy)
		  {
			  BAT_ON();	 		  
			  ATA583X_Init();
			  ATA583X_WaitRx(80);	
			  if(bAS3933NeedChange)
			  {
				  AS3933Change();
			  }
			  BAT_OFF();	
			  AS3933_COMM(AS3933_COMM_ClearWake);	
			  NVIC_SystemReset();
		  }
		  else
		  {
			  AS3933_Init();  
			  NVIC_SystemReset();
		  }
	  }
	  else
	  {
		  BAT_OFF();
		  AS3933_Init();  
		  NVIC_SystemReset();
	  }	  
  }
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
