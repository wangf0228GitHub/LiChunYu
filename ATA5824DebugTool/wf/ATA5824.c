#include "ATA5824.h"
#include "SimSPI.h"
#include "..\Inc\tim.h"
uint32_t ATA5824_bFirstIC;
_ATA5824_ICDataStruct ATA5824_ICData[ATA5824_ICData_BufLen];
uint32_t ATA5824_ICDataIndex;
void ATA5824_ICProc(void)
{
	uint32_t read;
	if(ATA5824_bFirstIC!=0)//µÚÒ»´Î²¶×½
	{
		ATA5824_bFirstIC=0;
		htim4.Instance->CNT=0;
		__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
		HAL_TIM_Base_Start_IT(&htim4);	
	}
	else
	{
		read=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);//+13;
		htim4.Instance->CNT=0;
		ATA5824_ICData[ATA5824_ICDataIndex].time=read;
		ATA5824_ICData[ATA5824_ICDataIndex].pinState=HAL_GPIO_ReadPin(SPI_MISO_GPIO_Port,SPI_MISO_Pin);
		ATA5824_ICDataIndex++;		
	}
}
void ATA5824_RxStart(void)
{
	ATA5824_bFirstIC=1;
	ATA5824_ICDataIndex=0;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x27);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
 	__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
 	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);	
}
void ATA5824_IDLE(void)
{
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
	SimSPI_Proc(0x60);
	SimSPI_Proc(0x20);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
}
void ATA5824_Init(void)
{
	uint8_t rx;
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
	SimSPI_Init();
	HAL_Delay(1);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);

 	rx=SimSPI_Proc(0x60);
 	rx=SimSPI_Proc(0x20);
 
 	rx=SimSPI_Proc(0x61);
 	rx=SimSPI_Proc(0xb1);
 
 	rx=SimSPI_Proc(0x62);
 	rx=SimSPI_Proc(0x7b);
 
 	rx=SimSPI_Proc(0x63);
 	rx=SimSPI_Proc(0x00);
 
 	rx=SimSPI_Proc(0x64);
 	rx=SimSPI_Proc(0x9b);
 
 	rx=SimSPI_Proc(0x65);
 	rx=SimSPI_Proc(0xfa);
 
 	rx=SimSPI_Proc(0x66);
 	rx=SimSPI_Proc(0xa8);
 
 	rx=SimSPI_Proc(0x67);
 	rx=SimSPI_Proc(0x50);

	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
}