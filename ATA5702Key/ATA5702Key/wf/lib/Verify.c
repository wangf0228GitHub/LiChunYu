#include "Verify.h"

uint8_t GetVerify_Sum(uint8_t* pBuff,uint8_t Count)
{
	uint8_t sum;
	uint8_t i;
	sum=0;
	for(i=0;i<Count;i++)
	{
		sum=(uint8_t)(sum+pBuff[i]);
	}
	return sum;
}

uint8_t GetVerify_byteXOR(uint8_t* pBuff, uint8_t Count)
{
	uint8_t ret,i;
	ret=*pBuff;
	for(i=1;i<Count;i++)
	{
		ret=ret^pBuff[i];
	}
	return ret;
}
uint8_t GetVerify_byteXOR_WithOrigV(uint8_t origV,uint8_t* pBuff, uint8_t Count)
{
	uint8_t ret,i;
	ret=origV;
	for(i=0;i<Count;i++)
	{
		ret=ret^pBuff[i];
	}
	return ret;
}



