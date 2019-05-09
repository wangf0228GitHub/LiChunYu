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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32l4xx_hal.h"

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
#define WAKE_Pin GPIO_PIN_0
#define WAKE_GPIO_Port GPIOA
#define RFIRTx_Pin GPIO_PIN_1
#define RFIRTx_GPIO_Port GPIOA
#define CarIRTx_Pin GPIO_PIN_2
#define CarIRTx_GPIO_Port GPIOA
#define IRRx_Pin GPIO_PIN_3
#define IRRx_GPIO_Port GPIOA
#define EE_WP_Pin GPIO_PIN_4
#define EE_WP_GPIO_Port GPIOA
#define EE_MISO_Pin GPIO_PIN_5
#define EE_MISO_GPIO_Port GPIOA
#define EE_CS_Pin GPIO_PIN_6
#define EE_CS_GPIO_Port GPIOA
#define EE_MOSI_Pin GPIO_PIN_7
#define EE_MOSI_GPIO_Port GPIOA
#define EE_SCLK_Pin GPIO_PIN_0
#define EE_SCLK_GPIO_Port GPIOB
#define Tailgate_Pin GPIO_PIN_1
#define Tailgate_GPIO_Port GPIOB
#define UnLock_Pin GPIO_PIN_2
#define UnLock_GPIO_Port GPIOB
#define Lock_Pin GPIO_PIN_10
#define Lock_GPIO_Port GPIOB
#define FindCar_Pin GPIO_PIN_11
#define FindCar_GPIO_Port GPIOB
#define bOnCar_Pin GPIO_PIN_12
#define bOnCar_GPIO_Port GPIOB
#define PowerHold_Pin GPIO_PIN_13
#define PowerHold_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOB
#define AS3933_MISO_Pin GPIO_PIN_15
#define AS3933_MISO_GPIO_Port GPIOB
#define AS3933_SCLK_Pin GPIO_PIN_9
#define AS3933_SCLK_GPIO_Port GPIOA
#define AS3933_CS_Pin GPIO_PIN_10
#define AS3933_CS_GPIO_Port GPIOA
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
#define BATState_ON GPIO_PIN_SET
#define BATState_OFF GPIO_PIN_RESET
#define BAT_ON() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_ON)
#define BAT_OFF() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_OFF)
#define bBATON() HAL_GPIO_ReadPin(PowerHold_GPIO_Port,PowerHold_Pin)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
