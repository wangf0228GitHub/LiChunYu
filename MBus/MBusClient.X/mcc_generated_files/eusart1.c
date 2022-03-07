/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F15344
        Driver Version    :  2.1.1
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/
#include "eusart1.h"
//#include "pin_manager.h"
#include "..\FlagDefine.h"
#include "mcc_generated_files/mcc.h"


/**
  Section: Macro Declarations
*/

#define EUSART1_TX_BUFFER_SIZE 8
#define EUSART1_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
*/

//volatile uint8_t eusart1RxHead = 0;
//volatile uint8_t eusart1RxTail = 0;
volatile uint8_t eusart1RxBuffer[EUSART1_RX_BUFFER_SIZE];
//volatile eusart1_status_t eusart1RxStatusBuffer[EUSART1_RX_BUFFER_SIZE];
volatile uint8_t eusart1RxCount;
volatile eusart1_status_t eusart1RxLastError;

/**
  Section: EUSART1 APIs
*/
void (*EUSART1_RxDefaultInterruptHandler)(void);

void (*EUSART1_FramingErrorHandler)(void);
void (*EUSART1_OverrunErrorHandler)(void);
void (*EUSART1_ErrorHandler)(void);

void EUSART1_DefaultFramingErrorHandler(void);
void EUSART1_DefaultOverrunErrorHandler(void);
void EUSART1_DefaultErrorHandler(void);


extern uint16_t flashBlock[WRITE_FLASH_BLOCKSIZE];
void RxAnswer(uint8_t command)
{
    uint8_t i,sum=0;
    EUSART1_Write(0x16);
    EUSART1_Write(MBusAddr);
    EUSART1_Write(command);
    EUSART1_Write(0x01);
    sum=(uint8_t)(sum+0x16);
    sum=(uint8_t)(sum+MBusAddr);
    sum=(uint8_t)(sum+command);
    sum=(uint8_t)(sum+0x01);
    EUSART1_Write(sum);
    EUSART1_Write(0x0d);
}
void RxProc()
{
    uint16_t flash;
    switch (eusart1RxBuffer[2])
    {
    case 0x01://����
        Doorlock_LAT=Lock_ON;
        RxAnswer(0x01);
        break;
    case 0x10://�޸ĵ�ַ
        flash=0x3030+((uint16_t)(eusart1RxBuffer[2]>>4))<<8+(uint16_t)(eusart1RxBuffer[2]&0x0f);
        FLASH_WriteWord(FlashAddr+4, flashBlock, flash);
        RxAnswer(0x02);
    default:
        break;
    }
    eusart1RxCount=0;
}


void EUSART1_Initialize(void)
{
    // disable interrupts before changing states
    PIE3bits.RC1IE = 0;
    EUSART1_SetRxInterruptHandler(EUSART1_Receive_ISR);
    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // SP1BRGL 160; 
    SP1BRGL = 0xA0;

    // SP1BRGH 1; 
    SP1BRGH = 0x01;


    EUSART1_SetFramingErrorHandler(EUSART1_DefaultFramingErrorHandler);
    EUSART1_SetOverrunErrorHandler(EUSART1_DefaultOverrunErrorHandler);
    EUSART1_SetErrorHandler(EUSART1_DefaultErrorHandler);

    eusart1RxLastError.status = 0;


//    eusart1RxHead = 0;
//    eusart1RxTail = 0;
    eusart1RxCount = 0;

    // enable receive interrupt
    PIE3bits.RC1IE = 1;
}

//bool EUSART1_is_tx_ready(void)
//{
//    return (bool)(PIR3bits.TX1IF && TX1STAbits.TXEN);
//}
//
//bool EUSART1_is_rx_ready(void)
//{
//    return (bool)(eusart1RxCount ? true : false);
//}
//
//bool EUSART1_is_tx_done(void)
//{
//    return TX1STAbits.TRMT;
//}
//
//eusart1_status_t EUSART1_get_last_status(void){
//    return eusart1RxLastError;
//}

//uint8_t EUSART1_Read(void)
//{
//    uint8_t readValue  = 0;
//    
//    while(0 == eusart1RxCount)
//    {
//    }
//
//    eusart1RxLastError = eusart1RxStatusBuffer[eusart1RxTail];
//
//    readValue = eusart1RxBuffer[eusart1RxTail++];
//    if(sizeof(eusart1RxBuffer) <= eusart1RxTail)
//    {
//        eusart1RxTail = 0;
//    }
//    PIE3bits.RC1IE = 0;
//    eusart1RxCount--;
//    PIE3bits.RC1IE = 1;
//
//    return readValue;
//}

void EUSART1_Write(uint8_t txData)
{
    while(0 == PIR3bits.TX1IF)
    {
    }

    TX1REG = txData;    // Write the data byte to the USART.
}



void EUSART1_Receive_ISR(void)
{
    
    eusart1RxLastError.status = 0;

    if(RC1STAbits.FERR){
        eusart1RxLastError.ferr = 1;
        EUSART1_FramingErrorHandler();
    }

    if(RC1STAbits.OERR){
        eusart1RxLastError.oerr = 1;
        EUSART1_OverrunErrorHandler();
    }
    
    if(eusart1RxLastError.status){
        EUSART1_ErrorHandler();
    } else {
        EUSART1_RxDataHandler();
    }
    
    // or set custom function using EUSART1_SetRxInterruptHandler()
}

void EUSART1_RxDataHandler(void){
    // use this default receive interrupt handler code
	uint8_t rx= RC1REG;
    uint8_t i,sum=0;
	if (gFlags.bMBusRx)
		return;
    RxTick=SystemTick;
	eusart1RxBuffer[eusart1RxCount++] = rx;
	switch (eusart1RxCount)
	{
	case 1:
		if (eusart1RxBuffer[0] != 0x16)
			eusart1RxCount = 0;
		break;
	case 2:
		if (eusart1RxBuffer[1] != MBusAddr)
			eusart1RxCount = 0;
		break;
// 	case 3:
// 		break;
// 	case 4:
// 		break;
	case 6:
		if (eusart1RxBuffer[5] != 0x0d)
			eusart1RxCount = 0;
		else
		{
            for(i=0;i<5;i++)
            {
                sum+=eusart1RxBuffer[i];
            }
            if(sum==eusart1RxBuffer[4])
			    gFlags.bMBusRx = 1;
            else
                eusart1RxCount=0;
		}
		break;
	default:
		break;
	}
    if(sizeof(eusart1RxBuffer) <= eusart1RxCount)
    {
        eusart1RxCount = 0;
    }
}

void EUSART1_DefaultFramingErrorHandler(void){}

void EUSART1_DefaultOverrunErrorHandler(void){
    // EUSART1 error - restart

    RC1STAbits.CREN = 0;
    RC1STAbits.CREN = 1;

}

void EUSART1_DefaultErrorHandler(void){
    EUSART1_RxDataHandler();
}

void EUSART1_SetFramingErrorHandler(void (* interruptHandler)(void)){
    EUSART1_FramingErrorHandler = interruptHandler;
}

void EUSART1_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    EUSART1_OverrunErrorHandler = interruptHandler;
}

void EUSART1_SetErrorHandler(void (* interruptHandler)(void)){
    EUSART1_ErrorHandler = interruptHandler;
}


void EUSART1_SetRxInterruptHandler(void (* interruptHandler)(void)){
    EUSART1_RxDefaultInterruptHandler = interruptHandler;
}
/**
  End of File
*/