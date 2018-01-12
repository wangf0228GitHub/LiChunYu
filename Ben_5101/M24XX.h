#ifndef __M24XX_h__
#define __M24XX_h__

#ifndef M24XX_RetryCount
#define M24XX_RetryCount 2
#endif

#ifndef M24XX_ADDR_W
#define  M24XX_ADDR_W	0xa0//Ѱַ�ֽ�д
#endif
#ifndef M24XX_ADDR_R
#define  M24XX_ADDR_R	0xa1//Ѱַ�ֽڶ�
#endif
void M24XX_Init(void);


unsigned char M24XX_Read(unsigned char *pBuff,unsigned int Count,unsigned char Address);
unsigned char M24XX_Write(unsigned char *pBuff,unsigned char Count,unsigned char Address);


#endif

