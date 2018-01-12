#ifndef __EISProc_h__
#define __EISProc_h__

/************************************************************************/
/* √¸¡Ó∂®“Â                                                             */
/************************************************************************/

#define UART_EISProc_Instance USART1
#define UART_EISProc_Handle huart1

typedef union   
{
	struct
	{
		unsigned char FrameHeader;
		unsigned char CommandIndex;
		unsigned char Data[8];
		unsigned char Verify;
		unsigned char bak;
	}RxStruct;
	unsigned char AllData[12];
} _EISProc_RxBuf;        // general flags

void EIS_ReadProc(void);
#endif


