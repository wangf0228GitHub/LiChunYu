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



/************************************************************************/
/* SPIROM                                                               */
/************************************************************************/
#include "SimSPI.h"


#define SimI2C_Delay_Ex
#define SimI2C_Delay() wfDelay_ms(1);


#define SIMI2C_SCL_Low() HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_RESET)
#define SIMI2C_SCL_High() HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_SET)

#define SIMI2C_SDA_Low() HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_RESET)
#define SIMI2C_SDA_High() HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_SET)

#define SIMI2C_SDA_Read() HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin)

#define SIMI2C_SDA_OUT() GPIO_InitTypeDef GPIO_InitStruct_Out;  \
	GPIO_InitStruct_Out.Pin = IIC_SDA_Pin;  \
	GPIO_InitStruct_Out.Mode = GPIO_MODE_OUTPUT_PP;  \
	GPIO_InitStruct_Out.Pull = GPIO_PULLUP;  \
	GPIO_InitStruct_Out.Speed = GPIO_SPEED_FREQ_LOW;  \
	HAL_GPIO_Init(IIC_SDA_GPIO_Port, &GPIO_InitStruct_Out);	

#define SIMI2C_SDA_IN() GPIO_InitTypeDef GPIO_InitStruct_In;  \
	GPIO_InitStruct_In.Pin = IIC_SDA_Pin;  \
	GPIO_InitStruct_In.Mode = GPIO_MODE_INPUT;  \
	GPIO_InitStruct_In.Pull = GPIO_PULLUP;  \
	HAL_GPIO_Init(IIC_SDA_GPIO_Port, &GPIO_InitStruct_In);
#include "SimI2C.h"

#endif
