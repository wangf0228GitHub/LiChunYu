#ifndef __HardwareProfile_H__
#define __HardwareProfile_H__

/************************************************************************/
/* SIM                                                                  */
/************************************************************************/
#include "gpio.h"
#include "wfSys.h"

#define SimSPI_SPI3
#define SimSPI_Delay_Ex
#define SimSPI_Delay() NOP10();NOP10();NOP10();NOP10()

#define SIMSPI_SCL_Low() HAL_GPIO_WritePin(ATA583X_SCLK_GPIO_Port,ATA583X_SCLK_Pin,GPIO_PIN_RESET)
#define SIMSPI_SCL_High() HAL_GPIO_WritePin(ATA583X_SCLK_GPIO_Port,ATA583X_SCLK_Pin,GPIO_PIN_SET)

#define SIMSPI_SDO_Low() HAL_GPIO_WritePin(ATA583X_MOSI_GPIO_Port,ATA583X_MOSI_Pin,GPIO_PIN_RESET)
#define SIMSPI_SDO_High() HAL_GPIO_WritePin(ATA583X_MOSI_GPIO_Port,ATA583X_MOSI_Pin,GPIO_PIN_SET)

#define SIMSPI_SDI_Read() HAL_GPIO_ReadPin(ATA583X_MISO_GPIO_Port,ATA583X_MISO_Pin)

#define SimSPI2

#define SimSPI2_SPI3
#define SimSPI2_Delay_Ex
#define SimSPI2_Delay() //asm("nop");//wfDelay_us(1);

#define SIMSPI2_SCL_Low() HAL_GPIO_WritePin(EE_SCLK_GPIO_Port,EE_SCLK_Pin,GPIO_PIN_RESET)
#define SIMSPI2_SCL_High() HAL_GPIO_WritePin(EE_SCLK_GPIO_Port,EE_SCLK_Pin,GPIO_PIN_SET)

#define SIMSPI2_SDO_Low() HAL_GPIO_WritePin(EE_MOSI_GPIO_Port,EE_MOSI_Pin,GPIO_PIN_RESET)
#define SIMSPI2_SDO_High() HAL_GPIO_WritePin(EE_MOSI_GPIO_Port,EE_MOSI_Pin,GPIO_PIN_SET)

#define SIMSPI2_SDI_Read() HAL_GPIO_ReadPin(EE_MISO_GPIO_Port,EE_MISO_Pin)

/************************************************************************/
/* SPIROM                                                               */
/************************************************************************/
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
