#ifndef __SPIROM_Conf_h__
#define __SPIROM_Conf_h__

#include "SimSPI.h"

#define SPIROM_PageRollover 0x001f
#define SPIROM_NeedWIP

#define SPIROM_CS_Low() HAL_GPIO_WritePin(EE_CS_GPIO_Port,EE_CS_Pin,GPIO_PIN_RESET)
#define SPIROM_CS_High() HAL_GPIO_WritePin(EE_CS_GPIO_Port,EE_CS_Pin,GPIO_PIN_SET)

#define SPIROM_WP_Low() HAL_GPIO_WritePin(EE_WP_GPIO_Port,EE_WP_Pin,GPIO_PIN_RESET)
#define SPIROM_WP_High() HAL_GPIO_WritePin(EE_WP_GPIO_Port,EE_WP_Pin,GPIO_PIN_SET)

#define SPIROM_SPIInit() SimSPI2_Init()
#define SPIROM_SPIProc(c) SimSPI2_Proc(c)

#endif


