#include "USBInteraction.h"
#include "usbd_customhid.h"
#include "Variables.h"
#include "Function.h"
#include "lcyHash.h"
#include "..\..\..\WF_Device\SPIROM.h"
#include "..\..\..\WF_Device\Verify.h"
#include "..\..\..\WF_Device\wfDefine.h"
#include "..\..\..\WF_Device\lcyIRDecode.h"
extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t usbTxBuf[64];
//命令帧格式
//cc pp data
//回复命令帧格式
//cc ss data
#define USBDataIndex 2
const uint8_t UnRPD[9]={'w','f','l','o','v','e','l','x','y'}; 
const uint8_t pp[8]={0x05,0x55,0x92,0x38,0x47,0x96,0xad,0xe7};
void USBProc(uint8_t* pBuf)
{
	uint32_t i,j,addr,renew; 
	uint8_t x;
	uint8_t rrr[16];	
	_RenewTimes RenewTimes;
	usbTxBuf[0]=pBuf[0];
	usbTxBuf[1]=0x01;
	switch(pBuf[0])
	{
	case 0x03://获取硬件ID
		usbTxBuf[USBDataIndex]=0x01;
		usbTxBuf[USBDataIndex+1]=0x02;
		usbTxBuf[USBDataIndex+2]=0x03;
		usbTxBuf[USBDataIndex+3]=0x04;
		break;
	case 0x00://获取硬件版本
		usbTxBuf[USBDataIndex]=0x01;
		usbTxBuf[USBDataIndex+1]=0x02;
		break;
	case 0x10://读钥匙数据
		addr=pBuf[1]<<4;
		RomData_ReadBytes(addr,&usbTxBuf[USBDataIndex],16);
		break;
	case 0x11://写钥匙数据
		GetKeyState();
		if(RomStateFlags.Bits.bRomWrited)
		{
			usbTxBuf[1]=0x03;
			break;
		}
		addr=pBuf[1]<<4;
		RomData_WriteBytes(addr,&pBuf[2],16);
		if(pBuf[1]==5)//最后一帧，生成ee数据
		{
			CheckDataBlockVerify(0x55);
			x=RomData_ReadByte(0x55+8);
			if((!gFlags.Bits.bFuncRet) || (x>0x7f))
			{
				usbTxBuf[1]=0x02;
				break;
			}
			x=x|0x03;//低位次数
			//生成0x73段
			RomData_ReadBytes(0x55, RomDatas, 8);
			for (i = 0; i < 8; i++)
			{
				lcyHashIn[i] = RomDatas[7 - i];
			}
			lcyHashCalc(3);
			for (i = 0; i < 8; i++)
			{
				RomDatas[7 - i] = lcyHashOut[i];
			}
			RomDatas[8]=x;
			RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
			RomData_WriteBytes(0x73, RomDatas, 10);//写入当前次数段，且使其匹配
			//生成使用次数
			for(i=0;i<8;i++)
			{
				RomDatas[i]=0xff;
			}
			addr=0;
			i=0x7f;
			while(1)
			{
				if(i==x)
				{
					break;
				}
				i--;
				if(addr==0)
					addr=7;
				else
					addr--;
			}
			RomDatas[addr]=i;
			RomData_WriteBytes(0x69, RomDatas, 8);

			RomData_ReadBytes(0x5f, RomDatas, 9);	
			RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
			RomData_WriteBytes(0x5f, RomDatas, 10);
			ReverseRom(0x5f+9);

			x=RomData_ReadByte(0x98);
			if(x==0xff)//写入电池电压
			{
				RomDatas[0]=0x73;
				RomDatas[1]=0x0c;
				RomData_WriteBytes(0x98,RomDatas,2);
			}
			RomDatas[0]=0x01;
			RomDatas[1]=0x01;
			RomDatas[2]=0x01;
			RomDatas[3]=0x01;
			x=RomData_ReadByte(0x00);
			if(GetBit(x,7)==0)
				RomDatas[4]=0xff;
			else
				RomDatas[4]=0x00;
			RomDatas[5]=0xff;
			RomData_WriteBytes(0x90,RomDatas,6);
			ChangeKeyState(0x04);
			BreakMyBlock();
			GetKeyState();
			GetKeyParam();//获得钥匙当前相关数据
		}
		break;
	case 0x12://擦除钥匙数据
		wfFlash_ReadDoubleWords(0,&RenewTimes.data,1);
		lcyHashInit(EEPSW);
		renew=100;
		for(i=0;i<(MAXRENEW+1);i++)
		{
			lcyHashOnce();
			for(j=0;j<8;j++)
			{
				if(lcyHashOut[j]!=RenewTimes.dataList[j])
					break;
			}
			if(j==8)//验算成功
			{
				renew=i;
				break;
			}
			lcyHashInit(lcyHashOut);
		}
		if(renew!=100 && renew!=0)//还有刷新次数
		{
			for(i=0;i<16;i++)
				rrr[i]=0;
			for(i=0;i<16;i++)
			{
				addr=i<<4;
				RomData_WriteBytes(addr,rrr,16);
			}
			usbTxBuf[2]=renew;
			for(i=0;i<8;i++)
				lcyHashIn[i]=EEPSW[i];
			lcyHashCalc(renew);
			for(i=0;i<8;i++)
				RenewTimes.dataList[i]=lcyHashOut[i];
			STMFLASH_Write(0,&RenewTimes.data,1);
		}
		else
		{
			usbTxBuf[1]=0x02;
		}
		break;
	case 0x13://读出秘钥
		for(i=0;i<8;i++)
			usbTxBuf[USBDataIndex+i]=EEPSW[i]^pp[i];
		break;
	case 0x14://设定最大刷新次数.
		for(i=0;i<8;i++)
			RenewTimes.dataList[i]=pBuf[1+i]^pp[i];
		STMFLASH_Write(0,&RenewTimes.data,1);
		break;
	case 0x15://解除写保护
		for(i=0;i<9;i++)
		{
			if(pBuf[1+i]!=UnRPD[i])
			{
				usbTxBuf[1]=0x02;
				break;
			}
		}
		if(i==9)//验证成功，解除写保护
		{
			LED_ON();
			HAL_FLASH_Unlock();
			HAL_FLASH_OB_Unlock();
			FLASH_OBProgramInitTypeDef obData;
			HAL_FLASHEx_OBGetConfig(&obData);
			obData.OptionType=OPTIONBYTE_RDP;
			obData.RDPLevel=OB_RDP_LEVEL_0;			
			HAL_FLASHEx_OBProgram(&obData);
			HAL_FLASH_OB_Lock();
			HAL_FLASH_Lock();
			
//			if ( (ret = HAL_FLASHEx_OBProgram(&obData)) != HAL_OK)
// 			obData.DATAAddress = OB_DATA_ADDRESS_DATA0;
// 			obData.DATAData = 0xAE;  //自定义数据
// 			obData.OptionType = OPTIONBYTE_DATA;
// 			if ( (ret = HAL_FLASHEx_OBProgram(&obData)) != HAL_OK){
// 				extern FLASH_ProcessTypeDef pFlash;
// 				char tmp[50] = {0};        
// 				sprintf(tmp, "write optby fail for %x, %08x", ret, pFlash.ErrorCode);
// 				debug(tmp, strlen(tmp));
// 				return -1;
// 			}
// 			obData.DATAAddress = OB_DATA_ADDRESS_DATA1;
// 			obData.DATAData = 0xC9;  //自定义数据
// 			if ((ret = HAL_FLASHEx_OBProgram(&obData)) != HAL_OK){
// 				char tmp[50] = {0};
// 				sprintf(tmp, "write optby fail for %x", ret);
// 				debug(tmp, strlen(tmp));
// 				return -1;
// 			}
		}
		break;
	default:
		break;
	}
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, usbTxBuf,64);
}
