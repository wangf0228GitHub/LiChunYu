using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib;
using uint8_t = System.Byte;
using uint16_t = System.UInt16;
using uint32_t = System.UInt32;
namespace 钥匙滚步测试
{
	public partial class Form1 : Form
	{
		void GetKeyParam()
		{
			uint8_t i, x;
			RomData_ReadBytes(0x9e, ref RomDatas, 2);
			if ((RomDatas[0] == 0xff && RomDatas[1] == 0xff) || (RomDatas[0] == 0x00 && RomDatas[1] == 0x00))//尚未初始化
			{
				BAT_ON();
				RomDatas[0] = ROM_9E;
				RomDatas[1] = ROM_9F;
				RomData_WriteBytes(0x9e, RomDatas, 2);//初始化ROM版本
			}
			RomData_ReadBytes(0x9e, ref RomDatas, 2);
			x = RomDatas[0];
			x += RomDatas[1];
			// 			RomStateFlags.bRomWrited = 0;
			// 			RomStateFlags.bStudy = 0;
			// 			if (x != 0)
			// 				NVIC_SystemReset();//存储区无法初始化，系统复位
			// 			if (GetBit(RomDatas[0], 2) != 0)//写过了
			// 			{
			// 				RomStateFlags.bRomWrited = 1;
			// 			}
			// 			if (GetBit(RomDatas[0], 4) != 0)//学习过
			// 			{
			// 				RomStateFlags.bStudy = 1;
			// 			}
			// 			if (RomStateFlags.bRomWrited)//写过了，获取系统参数
			// 			{
			EE00 = RomData_ReadByte(0x00);
			EE00 >>= 6;
			RomData_ReadBytes(0x01, ref PSW, 8);
			RomData_ReadBytes(0x09, ref SSID, 4);
			//使用次数低位
			//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
			//70-6f-6e-6d-6c-6b-6a-69-70
			GetCalcTimes69();
			//使用次数高位
			GetLeftTimeBlock(LeftTimesH);
			//使用次数中位
			GetLeftTimeBlock(LeftTimesM);
			//使用次数低位所用段，同时找到当前hash
			GetLeftTimeBlock(LeftTimesL);
			RomData_ReadBytes(LeftTimesAddr[LeftTimesL], ref RomDatas, 8);
			x = (uint8_t)(LeftTimes69 & 0x03);
			if (x == 0)
			{
				for (i = 0; i < 8; i++)
				{
					curHash[i] = RomDatas[7 - i];
				}
			}
			else
			{
				for (i = 0; i < 8; i++)
				{
					lcyHashIn[i] = RomDatas[7 - i];
				}
				//lcyHashCalc(x);
				lcyHashOut = LCYHashCal.HashCal(lcyHashIn, x);
				for (i = 0; i < 8; i++)
				{
					curHash[i] = lcyHashOut[i];
				}				
			}
			int step = LeftTimes[LeftTimesH] * 32 * 128 + LeftTimes[LeftTimesM] * 128 + LeftTimes69;
			byte[] need = needHash[step];
			for (int ww = 0; ww < 8; ww++)
			{
				if (curHash[ww] != need[ww])
				{
					bExit = true;
					MessageBox.Show("出错了");
					break;
				}
			}
			this.Invoke((EventHandler)(delegate
			{
				textBox1.Text = step.ToString();
				//progressBar1.PerformStep();
			}));
		}
		bool bFuncRet;

		//获得使用次数
		void GetLeftTimeBlock(uint8_t nBlock)
		{
			uint8_t t1, t2, addr1, addr2, tTop;
			switch (nBlock)
			{
				case LeftTimesH:
					addr1 = 0x2d;
					addr2 = 0x37;
					tTop = 0x2f;
					break;
				case LeftTimesM:
					addr1 = 0x41;
					addr2 = 0x4b;
					tTop = 0x1f;
					break;
				case LeftTimesL:
					addr1 = 0x55;
					addr2 = 0x5f;
					tTop = 0x00;
					break;
				default:
					return;
			}
			CheckDataBlockVerify(addr1);
			t1 = RomDatas[8];
			//if(!gFlags.bFuncRet)
			if (!bFuncRet)
			{
				t1 = 0xff;
			}
			CheckDataBlockVerify(addr2);
			t2 = RomDatas[8];
			//if(!gFlags.bFuncRet)
			if (!bFuncRet)
			{
				t2 = 0xff;
			}
			LeftTimesAddr[nBlock] = 0xff;//异常值
			if ((t1 != 0xff) && (t2 != 0xff))
			{
				if (nBlock != LeftTimesL)
				{
					if (t1 > t2)//用大的
					{
						if ((t2 == 0) && (t1 == tTop))//例如：00 1f则使用00
						{
							LeftTimesAddr[nBlock] = addr2;
							OtherLeftTimesAddr[nBlock] = addr1;
							LeftTimes[nBlock] = t2;
						}
						else//否则正常使用大的
						{
							LeftTimesAddr[nBlock] = addr1;
							OtherLeftTimesAddr[nBlock] = addr2;
							LeftTimes[nBlock] = t1;
						}
					}
					else//t2>t1
					{
						if ((t1 == 0) && (t2 == tTop))//例如：00 1f则使用00
						{
							LeftTimesAddr[nBlock] = addr1;
							OtherLeftTimesAddr[nBlock] = addr2;
							LeftTimes[nBlock] = 0;
						}
						else//否则正常使用大的
						{
							LeftTimesAddr[nBlock] = addr2;
							OtherLeftTimesAddr[nBlock] = addr1;
							LeftTimes[nBlock] = t2;
						}
					}
					return;//返回
				}
				else
				{
					tTop = (uint8_t)(LeftTimes69 & 0xfc);
					if (t1 == tTop)
					{
						LeftTimesAddr[nBlock] = addr1;
						OtherLeftTimesAddr[nBlock] = addr2;
						LeftTimes[nBlock] = t1;
						return;
					}
					else if (t2 == tTop)
					{
						LeftTimesAddr[nBlock] = addr2;
						OtherLeftTimesAddr[nBlock] = addr1;
						LeftTimes[nBlock] = t2;
						return;
					}
					else//两段都不匹配，需要重新生成低位hash
					{
						t1 = 0xff;
						t2 = 0xff;
					}
				}
			}
			else if (t1 != 0xff)//第一段匹配，则重新生成第二段
			{
				LeftTimesAddr[nBlock] = addr1;
				OtherLeftTimesAddr[nBlock] = addr2;
				LeftTimes[nBlock] = t1;
			}
			else if (t2 != 0xff)
			{
				LeftTimesAddr[nBlock] = addr2;
				OtherLeftTimesAddr[nBlock] = addr1;
				LeftTimes[nBlock] = t2;
			}
			//IRTx_10_33_SSID();
			if (t1 != 0xff || t2 != 0xff)//需要重新生成第二段
			{

				switch (nBlock)
				{
					case LeftTimesH:
						if (LeftTimes[LeftTimesH] == 0)//高位为0则不再生成第二段
							break;
						else
						{
							t2 = (uint8_t)(LeftTimes[LeftTimesH] - 1);
							if (t2 >= 0x24)
							{
								t1 = 0x24;
								addr1 = 0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152								
							}
							else if (t2 >= 0x18)
							{
								t1 = 0x18;
								addr1 = 0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
							}
							else if (t2 >= 0xc)
							{
								t1 = 0x0c;
								addr1 = 0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
							}
							else
							{
								t1 = 0x00;
								addr1 = 0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
							}
							// 							if ((LeftTimes[LeftTimesH] == 0x2e) ||
							// 								(LeftTimes[LeftTimesH] == 0x24) ||
							// 								(LeftTimes[LeftTimesH] == 0x18) ||
							// 								(LeftTimes[LeftTimesH] == 0x0c)
							// 								)//此时生成高位段4096hash
							// 							{
							// 								RomData_ReadBytes(addr1, ref RomDatas, 8);
							// 								addr2 = 0xa0;
							// 								RomData_WriteBytes(addr2, RomDatas, 8);
							// 								for (i = 0; i < 8; i++)
							// 								{
							// 									lcyHashIn[i] = RomDatas[7 - i];
							// 								}
							// 								for (j = 1; j < 12; j++)
							// 								{
							// 									//HashCalc_N(4096);
							// 									// 				for (i = 0; i < 8; i++)
							// 									// 				{
							// 									// 					RomDatas[7 - i] = lcyHashOut[i];
							// 									// 				}
							// 									lcyHashIn = LCYHashCal.HashCal(lcyHashIn, 4096);
							// 									for (i = 0; i < 8; i++)
							// 									{
							// 										RomDatas[7 - i] = lcyHashIn[i];
							// 									}
							// 									addr2 += 8;
							// 									RomData_WriteBytes(addr2, RomDatas, 8);//写入当前次数段，且使其匹配
							// 								}
							// 							}
							t1 = (uint8_t)(t2 - t1);
							// 							addr2=(uint8_t)(0xa0+(t1<<3));
							// 							RomData_ReadBytes(addr2, ref RomDatas, 8);
							// 							RomDatas[8] = (uint8_t)(LeftTimes[LeftTimesH] - 1);
							// 							//RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
							// 							RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
							// 							RomData_WriteBytes(OtherLeftTimesAddr[LeftTimesH], RomDatas, 10);//写入当前次数段，且使其匹配
							FixDataBlock(OtherLeftTimesAddr[LeftTimesH], addr1, 4096, t1, (uint8_t)(LeftTimes[LeftTimesH] - 1));
						}
						break;
					case LeftTimesM:
						if (LeftTimes[LeftTimesM] == 0)//中位为0，则需要向高位借位
						{
							if (LeftTimes[LeftTimesH] == 0)//高位为0，则无法借位，不再生成中位第二段
								break;
							else//借位生成中位
							{
								FixDataBlock(OtherLeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesH], 128, 0x1f, 0x1f);
							}
						}
						else
						{
							FixDataBlock(OtherLeftTimesAddr[LeftTimesM], LeftTimesAddr[LeftTimesH], 128, (uint8_t)(LeftTimes[LeftTimesM] - 1), (uint8_t)(LeftTimes[LeftTimesM] - 1));
						}
						break;
					case LeftTimesL:
						if (LeftTimes[LeftTimesL] == 0)//低位为0，则需要向中位借位
						{
							if ((LeftTimes[LeftTimesM] == 0) && (LeftTimes[LeftTimesH] == 0))//中位、高位都为0，则无法借位，不再生成低位第二段
								break;
							else//借位
							{
								FixDataBlock(OtherLeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesM], 0x7c, 1, 0x7c);
							}
						}
						else
						{
							FixDataBlock(OtherLeftTimesAddr[LeftTimesL], LeftTimesAddr[LeftTimesM], (uint8_t)(LeftTimes[LeftTimesL] - 4), 1, (uint8_t)(LeftTimes[LeftTimesL] - 4));
						}
						break;
				}
			}
			else//两段都不匹配，需要用小的次数从新生成
			{
				t1 = RomData_ReadByte((uint8_t)(addr1 + 8));
				t2 = RomData_ReadByte((uint8_t)(addr2 + 8));
				if (t1 < t2)
				{
					LeftTimesAddr[nBlock] = addr1;
					OtherLeftTimesAddr[nBlock] = addr2;
					LeftTimes[nBlock] = t1;
				}
				else
				{
					LeftTimesAddr[nBlock] = addr2;
					OtherLeftTimesAddr[nBlock] = addr1;
					LeftTimes[nBlock] = t2;
				}
				switch (nBlock)
				{
					case LeftTimesH:
						if (LeftTimes[LeftTimesH] == 0)//高位为0则仅生成第一段
						{
							RomData_ReadBytes(0x0d, ref RomDatas, 8);
							RomDatas[8] = 0;
							//RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
							RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
							RomData_WriteBytes(LeftTimesAddr[LeftTimesH], RomDatas, 10);//写入当前次数段，且使其匹配
						}
						else
						{
							t2 = (uint8_t)(LeftTimes[LeftTimesH] - 1);
							if (t2 >= 0x24)
							{
								t1 = 0x24;
								addr1 = 0x25;//RomData_ReadBytes(0x25,RomDatas,8);//49152+49152+49152
							}
							else if (t2 >= 0x18)
							{
								t1 = 0x18;
								addr1 = 0x1d;//RomData_ReadBytes(0x1d,RomDatas,8);//49152+49152
							}
							else if (t2 >= 0xc)
							{
								t1 = 0x0c;
								addr1 = 0x15;//RomData_ReadBytes(0x15,RomDatas,8);//49152+49152
							}
							else
							{
								t1 = 0x00;
								addr1 = 0x0d;//RomData_ReadBytes(0x0d,RomDatas,8);//49152
							}
							t1 = (uint8_t)(t2 - t1);
							FixDataBlock(OtherLeftTimesAddr[LeftTimesH], addr1, 4096, t1, (uint8_t)(LeftTimes[LeftTimesH] - 1));//修复-1段
							FixDataBlock(LeftTimesAddr[LeftTimesH], OtherLeftTimesAddr[LeftTimesH], 4096, 1, LeftTimes[LeftTimesH]);//修复当前
						}
						break;
					case LeftTimesM:
						if (LeftTimes[LeftTimesM] == 0)//中位为0，则需要向高位借位
						{
							RomData_ReadBytes(LeftTimesAddr[LeftTimesH], ref RomDatas, 8);
							RomDatas[8] = 0;
							//RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
							RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
							RomData_WriteBytes(LeftTimesAddr[LeftTimesM], RomDatas, 10);
							if (LeftTimes[LeftTimesH] == 0)//高位为0，则仅生成当前段			
								break;
							else//借位生成中位
							{
								FixDataBlock(OtherLeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesH], 128, 0x1f, 0x1f);
							}
						}
						else
						{
							FixDataBlock(OtherLeftTimesAddr[LeftTimesM], LeftTimesAddr[LeftTimesH], 128, (uint8_t)(LeftTimes[LeftTimesM] - 1), (uint8_t)(LeftTimes[LeftTimesM] - 1));
							FixDataBlock(LeftTimesAddr[LeftTimesM], OtherLeftTimesAddr[LeftTimesM], 128, 1, LeftTimes[LeftTimesM]);
						}
						break;
					case LeftTimesL:
						LeftTimes[LeftTimesL] = (uint8_t)(LeftTimes69 & 0xfc);
						if (LeftTimes[LeftTimesL] == 0)//低位为0，则需要向中位借位
						{
							RomData_ReadBytes(LeftTimes[LeftTimesM], ref RomDatas, 8);
							RomDatas[8] = 0;
							//RomDatas[9]=GetVerify_byteXOR(RomDatas,9);
							RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);
							RomData_WriteBytes(LeftTimesAddr[LeftTimesL], RomDatas, 10);
							if ((LeftTimes[LeftTimesM] == 0) && (LeftTimes[LeftTimesH] == 0))//中位、高位都为0，则无法借位，不再生成低位第二段
								break;
							else//借位
							{
								FixDataBlock(OtherLeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesM], 0x7c, 1, 0x7c);
							}
						}
						else
						{
							FixDataBlock(OtherLeftTimesAddr[LeftTimesL], LeftTimesAddr[LeftTimesM], (uint8_t)(LeftTimes[LeftTimesL] - 4), 1, (uint8_t)(LeftTimes[LeftTimesL] - 4));
							FixDataBlock(LeftTimesAddr[LeftTimesL], OtherLeftTimesAddr[LeftTimesL], 4, 1, LeftTimes[LeftTimesL]);
						}
						break;
				}
			}
		}
		//fixAddr:要修复的地址
		//hashAddr：使用的hash的地址xiuf
		//stepLen:修复滚步步长
		//times：目标次数
		void FixDataBlock(uint8_t fixAddr, uint8_t hashAddr, uint16_t stepLen, uint8_t loopTimes, uint8_t leftTiems)
		{
			uint8_t i;
			RomData_ReadBytes(hashAddr, ref RomDatas, 8);
			if ((loopTimes != 0) && (stepLen!=0))
			{
				for (i = 0; i < 8; i++)
				{
					lcyHashIn[i] = RomDatas[7 - i];
				}
// 				for (i = 0; i < loopTimes; i++)
// 				{
// 					//lcyHashIn=LCYHashCal.HashCal(lcyHashIn, stepLen);
// 					HashCalc_N(stepLen);
// 				}
// 				for (i = 0; i < 8; i++)
// 				{
// 					RomDatas[7 - i] = lcyHashOut[i];
// 				}
				for (i = 0; i < loopTimes; i++)
				{
					lcyHashIn=LCYHashCal.HashCal(lcyHashIn, stepLen);					
				}
				for (i = 0; i < 8; i++)
				{
					RomDatas[7 - i] = lcyHashIn[i];
				}
			}
			RomDatas[8] = leftTiems;
			RomDatas[9] = Verify.GetVerify_byteXOR(RomDatas, 9);//GetVerify_byteXOR(RomDatas,9);	
			RomData_WriteBytes(fixAddr, RomDatas, 10);//写入当前次数段，且使其匹配
		}
		//sub_1442_USEDEC:
		//使用次数减一
		void UsedDEC()//(void)
		{
			uint8_t x;
			if (LeftTimes69 == 0)//0-1需要借位，写入0x7f
			{
				if (LeftTimes[LeftTimesM] == 0 && LeftTimes[LeftTimesH] == 0)//所有次数已经用尽
				{
					NVIC_SystemReset();//复位
				}
				else
				{
					ReverseRom(LeftTimesAddr[LeftTimesM] + 9);//破坏中位当前使用段					
					if (LeftTimes[LeftTimesM] == 0)//中段使用次数为零，则需要向高位借位
					{
						ReverseRom(LeftTimesAddr[LeftTimesH] + 9);
					}
					ReverseRom(LeftTimesAddr[LeftTimesL] + 9);//破坏低位当前使用段

					/************************************************************************/
					/*                                                                      */
					/************************************************************************/
					ReverseRom(OtherLeftTimesAddr[LeftTimesM] + 9);//破坏中位当前使用段					
					if (LeftTimes[LeftTimesM] == 0)//中段使用次数为零，则需要向高位借位
					{
						ReverseRom(OtherLeftTimesAddr[LeftTimesH] + 9);
					}
					ReverseRom(OtherLeftTimesAddr[LeftTimesL] + 9);//破坏低位当前使用段
					/************************************************************************/
					/*                                                                      */
					/************************************************************************/
					LeftTimes69 = 0x7f;
				}
			}
			else
			{
				x = (uint8_t)(LeftTimes69 & 0x03);
				if (x == 0x00)//需要切换低位段
				{
					ReverseRom(LeftTimesAddr[LeftTimesL] + 9);//破坏低位当前使用段
				}
				LeftTimes69--;//使用次数减一
			}
			RomData_WriteByte(LeftTimes69Addr, 0xff);//原地址写0xff，写一个写入减一的
			if (LeftTimes69Addr == 0x69)
			{
				RomData_WriteByte(0x70, LeftTimes69);
			}
			else
			{
				RomData_WriteByte(LeftTimes69Addr - 1, LeftTimes69);
			}
			GetKeyParam();
		}
		//反转指定地址的数据，若为0xff则写为0，反之若不是0xff则写为0xff
		void ReverseRom(uint8_t addr)
		{
			if (RomData_ReadByte(addr) != 0xff)
			{
				RomData_WriteByte(addr, 0xff);
			}
			else
			{
				RomData_WriteByte(addr, 0);
			}
		}
		//sub_13EB_Get69to70_FirstNFF
		//获得69~70存储区中第一个不为0xff的单元，其数据为计算次数
		//70-6f-6e-6d-6c-6b-6a-69-70
		void GetCalcTimes69()//(void)
		{
			uint8_t i, x;
			RomData_ReadBytes(0x69, ref RomDatas, 8);
			for (i = 8; i != 0; i--)
			{
				x = (uint8_t)(i - 1);
				if (RomDatas[x] != 0xff)
					break;
			}
			if (i == 0)//没有非ff单元,出错，将0x69写入00，最少的低位次数
			{
				LeftTimes69 = 0;
				RomData_WriteByte(0x69, 0);
				// 		CalcTimes_D2=0;
				// 		CalcTimes_BF=0;
				LeftTimes69Addr = 0x69;
				return;
			}
			else
			{
				//容错：再判断下一个是否也为非ff
				if (i == 1)//无需再判断
				{
					LeftTimes69 = RomDatas[0];
					// 			CalcTimes_D2=CalcTimes[0]&0xfc;
					// 			CalcTimes_BF=CalcTimes[0]&0x03;
					LeftTimes69Addr = 0x69;
					return;
				}
				else
				{
					x = (uint8_t)(i - 2);
					if (RomDatas[x] != 0xff)//出现了两个，使用下一个
					{
						LeftTimes69 = RomDatas[x];
						// 				CalcTimes_D2=CalcTimes[0]&0xfc;
						// 				CalcTimes_BF=CalcTimes[0]&0x03;
						LeftTimes69Addr = (uint8_t)(0x69 + x);
						ReverseRom(0x68 + i);//修正rom
					}
					else//没错，则使用此值
					{
						x = (uint8_t)(i - 1);
						LeftTimes69 = RomDatas[x];
						// 				CalcTimes_D2=CalcTimes[0]&0xfc;
						// 				CalcTimes_BF=CalcTimes[0]&0x03;
						LeftTimes69Addr = (uint8_t)(0x69 + x);
					}
				}
				//rom容错:再判断前一个是否也为非ff,只有70单元时需要判断
				if (i == 8)//70单元
				{
					if (RomDatas[0] != 0xff)
					{
						ReverseRom(0x69);
					}
				}
			}
		}
		void HashCalc_N(uint32_t nCount)
		{
			//uint32_t i;
			//for (i = 0; i < nCount; i += 4)
			//{
			//    lcyHashCalc(4);
			//    IRTx_10_33_SSID();
			//}
		}
		//检查数据段校验
		void CheckDataBlockVerify(uint8_t Addr)
		{
			uint8_t x;
			//gFlags.bFuncRet=0;	
			bFuncRet = false;
			RomData_ReadBytes(Addr, ref RomDatas, 10);
			if (RomDatas[8] > 0x7f)
			{
				//gFlags.bFuncRet=0;
				bFuncRet = false;
				return;
			}
			x = Verify.GetVerify_byteXOR(RomDatas, 9);//GetVerify_byteXOR(RomDatas,9);
			if (x == RomDatas[9])
				//gFlags.bFuncRet=1;
				bFuncRet = true;
		}
	}
}
