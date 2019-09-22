#ifndef __HardwareProfile_H__
#define __HardwareProfile_H__

/************************************************************************/
/* SIM                                                                  */
/************************************************************************/
#include "gpio.h"
#include "wfSys.h"

#define Verify_Sum

#define LeftTimesL 0
#define LeftTimesM 1
#define LeftTimesH 2


#define Verify_byteXOR



#define ROMVer 0x76
//21DF  写过是05FB 学习过 15EB
//02FE  写过是04FC 学习过 14EC
#define ROM_9E 0x21

//0xa0 0xa1 射频学习状态存储
//0xd0~0xee 存储当前hash值
//0xff 剩余renew次数

#define LED_ON() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)
#define LED_OFF() HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)
#define PowerLed() LED_ON();wfDelay_ms(35);LED_OFF()
/************************************************************************/
/* IO                                                                   */
/************************************************************************/
#if KeyWork
	#define BATState_ON GPIO_PIN_SET
	#define BATState_OFF GPIO_PIN_RESET
	#define BAT_ON() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_ON)
	#define BAT_OFF() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_OFF)
	#define bBATON() HAL_GPIO_ReadPin(PowerHold_GPIO_Port,PowerHold_Pin)

	#define bOnCarPower() HAL_GPIO_ReadPin(bOnCar_GPIO_Port,bOnCar_Pin)
	#define OnCarPowerState_ON GPIO_PIN_SET
	#define OnCarPowerState_OFF GPIO_PIN_RESET
#else
	#define BAT_ON() 
	#define BAT_OFF() 
	#define bBATON() 1
	
	#define bOnCarPower() 1	
#endif

/************************************************************************/
/* SPI                                                                  */
/************************************************************************/
#ifdef KeyWork
	#define SimSPI_SPI3
	#define SimSPI_Delay_Ex
	#define SimSPI_Delay() NOP10();NOP10();NOP10();NOP10()

	#define SIMSPI_SCL_Low() HAL_GPIO_WritePin(ATA583X_SCLK_GPIO_Port,ATA583X_SCLK_Pin,GPIO_PIN_RESET)
	#define SIMSPI_SCL_High() HAL_GPIO_WritePin(ATA583X_SCLK_GPIO_Port,ATA583X_SCLK_Pin,GPIO_PIN_SET)

	#define SIMSPI_SDO_Low() HAL_GPIO_WritePin(ATA583X_MOSI_GPIO_Port,ATA583X_MOSI_Pin,GPIO_PIN_RESET)
	#define SIMSPI_SDO_High() HAL_GPIO_WritePin(ATA583X_MOSI_GPIO_Port,ATA583X_MOSI_Pin,GPIO_PIN_SET)

	#define SIMSPI_SDI_Read() HAL_GPIO_ReadPin(ATA583X_MISO_GPIO_Port,ATA583X_MISO_Pin)
#else
	#define SIMSPI_SCL_Low() 
	#define SIMSPI_SCL_High() 

	#define SIMSPI_SDO_Low() 
	#define SIMSPI_SDO_High() 

	#define SIMSPI_SDI_Read() 1
#endif
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


#ifdef KeyWork

#define BATState_ON GPIO_PIN_SET
#define BATState_OFF GPIO_PIN_RESET
#define BAT_ON() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_ON)
#define BAT_OFF() HAL_GPIO_WritePin(PowerHold_GPIO_Port,PowerHold_Pin,BATState_OFF)
#define bBATON() HAL_GPIO_ReadPin(PowerHold_GPIO_Port,PowerHold_Pin)

#define SimSPI_SPI3
#define SimSPI_Delay_Ex
#define SimSPI_Delay() NOP10();NOP10();NOP10();NOP10()

#define SIMSPI_SCL_Low() HAL_GPIO_WritePin(ATA583X_SCLK_GPIO_Port,ATA583X_SCLK_Pin,GPIO_PIN_RESET)
#define SIMSPI_SCL_High() HAL_GPIO_WritePin(ATA583X_SCLK_GPIO_Port,ATA583X_SCLK_Pin,GPIO_PIN_SET)

#define SIMSPI_SDO_Low() HAL_GPIO_WritePin(ATA583X_MOSI_GPIO_Port,ATA583X_MOSI_Pin,GPIO_PIN_RESET)
#define SIMSPI_SDO_High() HAL_GPIO_WritePin(ATA583X_MOSI_GPIO_Port,ATA583X_MOSI_Pin,GPIO_PIN_SET)

#define SIMSPI_SDI_Read() HAL_GPIO_ReadPin(ATA583X_MISO_GPIO_Port,ATA583X_MISO_Pin)
#endif

#endif
