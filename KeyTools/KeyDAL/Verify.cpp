#include "Verify.h"

byte GetVerify_byteSum( byte* pBuff, int start, int count )
{
	byte sum = 0;
	for (int i = 0; i < count; i++)
		sum = (byte)(sum + pBuff[i + start]);
	return sum;
}
