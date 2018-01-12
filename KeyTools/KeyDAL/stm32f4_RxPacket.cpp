#include "stm32f4_RxPacket.h"
#include "GlobalFunc.h"


stm32f4_RxPacket::stm32f4_RxPacket(void)
{
}


stm32f4_RxPacket::~stm32f4_RxPacket(void)
{
}

void stm32f4_RxPacket::Reset()
{
	NeedCommand=0xffff;
	RxCount=0;
}

bool stm32f4_RxPacket::DataPaketed( byte rx )
{
	if(RxCount < stm32f4_RxPacket_pDataIndex)
	{
		Data[RxCount++] = rx;
		if (RxCount == 1)
		{
			if (Data[0] != 0x16)
			{
				RxCount = 0;
			}
		}
		else if (RxCount == 2)
		{
			if (Data[1] != 0x16)
			{
				RxCount = 0;
			}
		}
		else if (RxCount == 4)
		{
			Command = MakeShort(Data[3], Data[2]);
		}
		else if (RxCount == 6)
		{
			DataLen = MakeShort(Data[5], Data[4]);
			//if((DataLen+)>)
		}
		else if (RxCount == 7)
			bError = Data[6];
		else if (RxCount == 8)
			ErrCode = Data[7];
	}
	else
	{
		Data[RxCount] = rx;
		RxCount++;
		if (RxCount == (stm32f4_RxPacket_pDataIndex + DataLen + 2))
		{
			if (Data[RxCount - 1] == 0x0d)
			{
// 				byte s1 = Verify.GetVerify_byteSum(Header.Data);
// 				byte s2 = Verify.GetVerify_byteSum(Data, Data.Length - 2);
// 				s1 = (byte)(s1 + s2);
// 				if (s1 == Data[Data.Length - 2])
					return true;
			}
			else
			{
				RxCount = 0;
				return false;
			}
		}
	}
	return false;
}
