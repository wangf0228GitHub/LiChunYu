/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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
#define REL_0_Pin GPIO_PIN_0
#define REL_0_GPIO_Port GPIOC
#define REL_1_Pin GPIO_PIN_1
#define REL_1_GPIO_Port GPIOC
#define REL_2_Pin GPIO_PIN_2
#define REL_2_GPIO_Port GPIOC
#define REL_3_Pin GPIO_PIN_3
#define REL_3_GPIO_Port GPIOC
#define REL_4_Pin GPIO_PIN_4
#define REL_4_GPIO_Port GPIOC
#define REL_5_Pin GPIO_PIN_5
#define REL_5_GPIO_Port GPIOC
#define REL_6_Pin GPIO_PIN_6
#define REL_6_GPIO_Port GPIOC
#define REL_7_Pin GPIO_PIN_7
#define REL_7_GPIO_Port GPIOC
#define REL_8_Pin GPIO_PIN_8
#define REL_8_GPIO_Port GPIOC
#define REL_9_Pin GPIO_PIN_9
#define REL_9_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
#define REL_ON GPIO_PIN_SET
#define REL_OFF GPIO_PIN_RESET
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
