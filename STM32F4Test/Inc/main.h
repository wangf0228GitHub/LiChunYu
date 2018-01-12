/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "FlagDefine.h"
#include "CP1616_Client.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define IRTx_Pin GPIO_PIN_3
#define IRTx_GPIO_Port GPIOA
#define IK_TX_Pin GPIO_PIN_5
#define IK_TX_GPIO_Port GPIOA
#define IK_EN_Pin GPIO_PIN_4
#define IK_EN_GPIO_Port GPIOC
#define CAN1_A0_Pin GPIO_PIN_3
#define CAN1_A0_GPIO_Port GPIOD
#define CAN1_A1_Pin GPIO_PIN_4
#define CAN1_A1_GPIO_Port GPIOD
#define CAN1_EN_Pin GPIO_PIN_7
#define CAN1_EN_GPIO_Port GPIOD
#define IR1_EN1_Pin GPIO_PIN_0
#define IR1_EN1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define MAKE_SHORT(h,l)	(((unsigned short)h<<8)+l)
#define MAKE_BYTE(h,l)	((h<<4)+l)

#define LOW_NIBBLE(x)	(x&0x0F)
#define HIGH_NIBBLE(x)	(x>>4)
// #define LED_DS0_OFF() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET)
// #define LED_DS0_ON() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET)
// #define LED_DS0_Toggle() HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9)
// 
// #define LED_DS1_OFF() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET)
// #define LED_DS1_ON() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET)
// #define LED_DS1_Toggle() HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10)
// 
// 
// 
// #define BEEP_ON() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET)
// #define BEEP_OFF() HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET)

#define GetDeltaTick(a) (HAL_GetTick() - a)
extern unsigned int FlashTick;
extern volatile _GFlags gFlags; 
extern unsigned int gTemp32[50];
extern volatile unsigned char gTempIndex;
extern char MessgeBuf[200];
extern unsigned char aRxBuffer_WithPC[1];
// extern unsigned char IRTxList[100];
// extern unsigned char IRTxCount;
// extern unsigned char IRTxIndex;
// extern unsigned char IRRxList[50];
// extern unsigned char IRRxIndex;
/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
