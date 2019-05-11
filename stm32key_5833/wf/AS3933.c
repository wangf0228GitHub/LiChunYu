#include "AS3933.h"
#include "wfSys.h"
#include "..\Inc\tim.h"
#include "Variables.h"

#define AS3933_SCL_Low() HAL_GPIO_WritePin(AS3933_SCLK_GPIO_Port,AS3933_SCLK_Pin,GPIO_PIN_RESET)
#define AS3933_SCL_High() HAL_GPIO_WritePin(AS3933_SCLK_GPIO_Port,AS3933_SCLK_Pin,GPIO_PIN_SET)

#define AS3933_SDO_Low() HAL_GPIO_WritePin(AS3933_MOSI_GPIO_Port,AS3933_MOSI_Pin,GPIO_PIN_RESET)
#define AS3933_SDO_High() HAL_GPIO_WritePin(AS3933_MOSI_GPIO_Port,AS3933_MOSI_Pin,GPIO_PIN_SET)

#define AS3933_SDI_Read() HAL_GPIO_ReadPin(AS3933_MISO_GPIO_Port,AS3933_MISO_Pin)

uint8_t AS3933_Proc(uint8_t c)
{
	uint8_t i,ret;
	ret=0x00;
	for(i=0;i<8;i++)
	{
		AS3933_SDO_Low();
		if((c&0x80)==0x80)	//判断待发送的数据位是0或1
		{
			AS3933_SDO_High();	//待发送数据位是1
		}	
		c=c<<1;	//判断待发送的数据位是0或1			
		//AS3933_Delay();
		AS3933_SCL_High();//上升沿发送	
		//AS3933_Delay();
		AS3933_SCL_Low();	//下降沿接收数据
		ret=ret<<1;
		//AS3933_Delay();
		if(AS3933_SDI_Read()==1)
			ret|=0x01;						
	}
	AS3933_SCL_Low();	//空闲为低电平
	return ret;
}
void AS3933_COMM(uint8_t com)		//Ð´AS3933ÃüÁî
{
	HAL_GPIO_WritePin(AS3933_CS_GPIO_Port,AS3933_CS_Pin,GPIO_PIN_SET);
	AS3933_Proc(com);
	HAL_GPIO_WritePin(AS3933_CS_GPIO_Port,AS3933_CS_Pin,GPIO_PIN_RESET);
}
void AS3933_WriteReg(uint8_t addr,uint8_t data)		//Ð´AS3933ÃüÁî
{
	HAL_GPIO_WritePin(AS3933_CS_GPIO_Port,AS3933_CS_Pin,GPIO_PIN_SET);	
	AS3933_Proc(addr&0x3f);
	AS3933_Proc(data);
	HAL_GPIO_WritePin(AS3933_CS_GPIO_Port,AS3933_CS_Pin,GPIO_PIN_RESET);
}
uint8_t AS3933_ReadReg(uint8_t addr)		//Ð´AS3933ÃüÁî
{
	uint8_t ret;
	HAL_GPIO_WritePin(AS3933_CS_GPIO_Port,AS3933_CS_Pin,GPIO_PIN_SET);
	addr=addr&0x3f;
	addr=addr|0x40;
	AS3933_Proc(addr);
	ret=AS3933_Proc(0xff);
	HAL_GPIO_WritePin(AS3933_CS_GPIO_Port,AS3933_CS_Pin,GPIO_PIN_RESET);
	return ret;
}
void AS3933_Init(void)
{
//	uint8_t reg[20],i;
// 	AS3933_SCL_Low();	
 	AS3933_COMM(AS3933_COMM_PResetDefault);
// 	HAL_Delay(30);
// 	AS3933_COMM(AS3933_COMM_ClearWake);//直接命令：清除wake状态，重回listenning模式
// 	HAL_Delay(30);				//延时30ms

// 	AS3933_WriteReg(0x01, 0x20);
//  	HAL_Delay(30);				//延时30ms	
//  	AS3933_WriteReg(0x11, 0x1f); 	//R17:设置通道1的谐振电容  180P
//  	HAL_Delay(10);					    //延时10ms  等待设置完毕
//  	AS3933_WriteReg(0x10, 0xc1);		//R16:将通道1的谐振信号输出到DAT脚
//  	HAL_Delay(30);					    //30ms 观察时间
// // 
//  	AS3933_WriteReg(0x12, 0x1f); 	//R18:设置通道2的谐振电容  130P
//  	HAL_Delay(10);							//延时10ms  等待设置完毕
//  	AS3933_WriteReg(0x10, 0x42);		//R16:将通道2的谐振信号输出到DAT脚
//  	HAL_Delay(30);							//30ms 观察时间
// // 
//  	AS3933_WriteReg(0x13, 0x1f); 	//R19:设置通道3的谐振电容  130P
//  	HAL_Delay(10);							//延时10ms  等待设置完毕
//  	AS3933_WriteReg(0x10, 0x44); 	//R16:将通道3的谐振信号输出到DAT脚
//  	HAL_Delay(30);					    //30ms 观察时间
//  	AS3933_WriteReg(0x10, 0x00);	 	//R16:以上为校正各通道的谐振频率，可以通过DAT脚观察（示波器或者逻辑分析仪）

//	AS3933_WriteReg(0x08, 0xe0);		//R8寄存器设置：唤醒频率15-23kHz
	/******************以上为天线调谐配置*******************/

	AS3933_WriteReg(0x00, 0x7E);		//R0寄存器设置：32位Pattern，On/Off模式，开启Scan mode ，通道1、2、3全部开启，
	//Masks data on DAT pin before wake-up (DAT_MASK = 0 → data not masked; DAT_MASK = 1 → data masked) 
	AS3933_WriteReg(0x01, 0x00);			//R1寄存器设置：双向使能AGC（上-下）,使能天线阻尼器，禁止曼彻斯特编码，Correlator禁止，内部晶振


	AS3933_WriteReg(0x02, 0x02);		//R2寄存器设置：接收频率23-150Khz，降低数据限幅器的绝对阈值
	//AS3933_WriteReg(0X04, 0Xff);		//R4寄存器设置：ON/OFF模式下OFF时长4mS,天线阻尼电阻27K ，无增益衰减
	AS3933_WriteReg(0X04, 0X00);		//R4寄存器设置：ON/OFF模式下OFF时长4mS,天线阻尼电阻27K ，无增益衰减
	AS3933_WriteReg(0x05, 0x3A);		//R5寄存器设置：第二个唤醒前导码0x3A
	AS3933_WriteReg(0x06, 0xC3);		//R6寄存器设置：第一个唤醒前导码0xC3
	AS3933_WriteReg(0x07, 0x2B);		//R7寄存器设置：超时设置50mS,波特率12
	AS3933_WriteReg(0x08, 0xe0);		//R8寄存器设置：唤醒频率15-23kHz
//  	for(i=0;i<20;i++)
//  		reg[i]=AS3933_ReadReg(i);
	AS3933_COMM(AS3933_COMM_ClearWake);
}
void ReadANT(void)
{
	ANTCount=0;
	ANTFlags.antValue=0;
	htim6.Instance->ARR=2920;
	AS3933_COMM(AS3933_COMM_ClearWake);	
	htim6.Instance->CNT=0;
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start(&htim6);
	while(1)
	{
		if(bAS3933Wake())
		{
			ANTFlags.Bits.bZuoHou=1;
		}
		if(__HAL_TIM_GET_FLAG(&htim6,TIM_FLAG_UPDATE))
			break;
	}
	HAL_TIM_Base_Stop(&htim6);

	AS3933_COMM(AS3933_COMM_ClearWake);	
	htim6.Instance->CNT=0;
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start(&htim6);
	while(1)
	{
		if(bAS3933Wake())
		{
			ANTFlags.Bits.bYouHou=1;
		}
		if(__HAL_TIM_GET_FLAG(&htim6,TIM_FLAG_UPDATE))
			break;
	}
	HAL_TIM_Base_Stop(&htim6);

	AS3933_COMM(AS3933_COMM_ClearWake);	
	htim6.Instance->CNT=0;
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start(&htim6);
	while(1)
	{
		if(bAS3933Wake())
		{
			ANTFlags.Bits.bJiaShiShi=1;
		}
		if(__HAL_TIM_GET_FLAG(&htim6,TIM_FLAG_UPDATE))
			break;
	}
	HAL_TIM_Base_Stop(&htim6);

	AS3933_COMM(AS3933_COMM_ClearWake);	
	htim6.Instance->CNT=0;
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_TIM_Base_Start(&htim6);
	while(1)
	{
		if(bAS3933Wake())
		{
			ANTFlags.Bits.bCheNei=1;			
		}
		if(__HAL_TIM_GET_FLAG(&htim6,TIM_FLAG_UPDATE))
			break;
	}
	HAL_TIM_Base_Stop(&htim6);
	if(ANTFlags.Bits.bCheNei)
		ANTCount++;
	if(ANTFlags.Bits.bZuoHou)
		ANTCount++;
	if(ANTFlags.Bits.bYouHou)
		ANTCount++;
	if(ANTFlags.Bits.bJiaShiShi)
		ANTCount++;
}
