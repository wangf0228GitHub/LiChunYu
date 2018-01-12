#ifndef __ESLProc_h__
#define __ESLProc_h__

/************************************************************************/
/* √¸¡Ó∂®“Â                                                             */
/************************************************************************/

#define UART_ESLProc_Instance USART2
#define UART_ESLProc_Handle huart2

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
} _ESLProc_RxBuf;        // general flags

void ESL_ReadProc(void);
#endif


