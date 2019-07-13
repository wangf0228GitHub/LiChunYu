/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ATA583X_CS_Pin GPIO_PIN_13
#define ATA583X_CS_GPIO_Port GPIOC
#define IIC_SCL_Pin GPIO_PIN_4
#define IIC_SCL_GPIO_Port GPIOH
#define IIC_SDA_Pin GPIO_PIN_5
#define IIC_SDA_GPIO_Port GPIOH
#define DS0_Pin GPIO_PIN_0
#define DS0_GPIO_Port GPIOB
#define ATA583X_PWR_ON_Pin GPIO_PIN_4
#define ATA583X_PWR_ON_GPIO_Port GPIOB
#define ATA583X_MISO_Pin GPIO_PIN_5
#define ATA583X_MISO_GPIO_Port GPIOB
#define ATA583X_MOSI_Pin GPIO_PIN_6
#define ATA583X_MOSI_GPIO_Port GPIOB
#define ATA583X_SCLK_Pin GPIO_PIN_7
#define ATA583X_SCLK_GPIO_Port GPIOB
#define ATA583X_TMDO_Pin GPIO_PIN_8
#define ATA583X_TMDO_GPIO_Port GPIOB
#define ATA583X_TMDI_Pin GPIO_PIN_9
#define ATA583X_TMDI_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef union
{	
	struct
	{
		unsigned bLED:1;
		unsigned bTxFinish:1;
		unsigned bIRRxFrame:1;
		unsigned bFirstIC:1;
		unsigned bOldBAT:1;
		unsigned bFuncRet:1;
		unsigned bIRRxH:1;

		unsigned ATA583X_bFirstIC:1;
		unsigned ATA583X_bRxFrame:1;

		unsigned b3933Wake:1;
	}Bits;
	uint32_t all;
}_gFlags;
extern volatile _gFlags gFlags;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
