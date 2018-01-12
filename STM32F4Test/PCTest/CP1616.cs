using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib;
using System.IO.Ports;

namespace PCTest
{
    /// <summary>
    /// 包头
    /// </summary>
    public class stm32f4_TxPacketHead
    {
        /// <summary>
        /// 包头大小
        /// </summary>
        public static Int32 DataLen_SIZE = 2;
        public static Int32 CommandLen_SIZE = 2;
        public static Int32 HeaderBak_SIZE = 2;
        public static Int32 HEAD_SIZE = 8;

        public ushort Command;
        public ushort DataLen;
       
        public byte[] Data;
        public stm32f4_TxPacketHead()
        {
            Data=new byte[HEAD_SIZE];
        }
    }
    public class stm32f4_TxPacket
    {
        public stm32f4_TxPacketHead Header;
        public byte[] Data;
        public int RxCount;
        public byte[] RxList;
        public ushort NeedCommand;
        public static Int32 BUFFER_SIZE = 1024;
        public stm32f4_TxPacket(ushort com,UInt16 addr)
        {
            NeedCommand=com;
            Header=new stm32f4_TxPacketHead();
            RxCount = 0;
        }
        public stm32f4_TxPacket()
        {
            NeedCommand=0xff;
            Header=new stm32f4_TxPacketHead();
            RxCount = 0;
        }
//         public static byte[] MakeCP1616Packet(ushort com, byte b)
//         {
//             return MakeCP1616Packet(com, new byte[] { b });
//         }
        public static byte[] MakeCP1616Packet(ushort com, byte[] data)
        {
            byte[] txbuffer;
            if(data!=null)
            {
                txbuffer = new byte[stm32f4_TxPacketHead.HEAD_SIZE + data.Length + 2];
                txbuffer[0] = 0x16;
                txbuffer[1] = 0x16;
                txbuffer[2] = BytesOP.GetLowByte(com);
                txbuffer[3] = BytesOP.GetHighByte(com);                
                txbuffer[4] = BytesOP.GetLowByte((UInt16)data.Length);
                txbuffer[5] = BytesOP.GetHighByte((UInt16)data.Length);
                txbuffer[6] = 0x00;
                txbuffer[7] = 0x00;
//                 txbuffer[6] = BytesOP.GetLowByte(retry);
//                 txbuffer[7] = BytesOP.GetHighByte(retry);
//                 txbuffer[8] = BytesOP.GetLowByte(timeout);
//                 txbuffer[9] = BytesOP.GetHighByte(timeout); 
                for (int i = 0; i < data.Length; i++)
                {
                    txbuffer[i + stm32f4_TxPacketHead.HEAD_SIZE] = data[i];
                }
                txbuffer[stm32f4_TxPacketHead.HEAD_SIZE + data.Length] = Verify.GetVerify_byteSum(txbuffer, stm32f4_TxPacketHead.HEAD_SIZE + data.Length);
                txbuffer[stm32f4_TxPacketHead.HEAD_SIZE + data.Length + 1] = 0x0d;
            }
            else
            {
                txbuffer = new byte[stm32f4_TxPacketHead.HEAD_SIZE+ 2];
                txbuffer[0] = 0x16;
                txbuffer[1] = 0x16;
                txbuffer[2] = BytesOP.GetLowByte(com);
                txbuffer[3] = BytesOP.GetHighByte(com);
                txbuffer[4] = 0;
                txbuffer[5] = 0;
                txbuffer[6] = 0;
                txbuffer[7] = 0;
//                 txbuffer[6] = BytesOP.GetLowByte(retry);
//                 txbuffer[7] = BytesOP.GetHighByte(retry);
//                 txbuffer[8] = BytesOP.GetLowByte(timeout);
//                 txbuffer[9] = BytesOP.GetHighByte(timeout); 
                txbuffer[stm32f4_TxPacketHead.HEAD_SIZE] = Verify.GetVerify_byteSum(txbuffer, stm32f4_TxPacketHead.HEAD_SIZE);
                txbuffer[stm32f4_TxPacketHead.HEAD_SIZE + 1] = 0x0d;
            }            
            return txbuffer;
        }        
//         public static stm32f4_TxPacket CP1616ComProc(ref SerialPort serialPort, byte com, ushort addr, uint b)
//         {
//             ushort b1, b2;
//             b1 = BytesOP.GetHighShort(b);
//             b2 = BytesOP.GetLowShort(b);
//             return CP1616ComProc(ref serialPort, com, addr, new byte[4] { BytesOP.GetHighByte(b1), BytesOP.GetLowByte(b1), BytesOP.GetHighByte(b2), BytesOP.GetLowByte(b2) },5);
//         }
//         public static stm32f4_TxPacket CP1616ComProc(ref SerialPort serialPort, byte com, ushort addr, ushort b)
//         {
//             return CP1616ComProc(ref serialPort, com, addr, new byte[2] { BytesOP.GetHighByte(b), BytesOP.GetLowByte(b) },5);
//         }
//         public static stm32f4_TxPacket CP1616ComProc(ref SerialPort serialPort, byte com, ushort addr, byte b)
//         {
//             return CP1616ComProc(ref serialPort, com, addr, new byte[1] { b },5);
//         }
//         public static stm32f4_TxPacket CP1616ComProc(ref SerialPort serialPort, byte com,ushort addr, byte[] data,int retry)
//         {
//             stm32f4_TxPacket ret = new stm32f4_TxPacket(com,addr);
//             byte[] tx;
//             tx = stm32f4_TxPacket.MakeCP1616Packet(com, addr, data);
//             serialPort.Write(tx, 0, tx.Length);
//             while (retry != 0)
//             {
//                 while (true)
//                 {
//                     try
//                     {
//                         if (ret.DataPacketed((byte)serialPort.ReadByte()))
//                         {
//                             return ret;
//                         }
//                     }
//                     catch// (Exception ex)
//                     {
//                         //Debug.WriteLine("血糖通信失败" + ex.Message);
//                         break;
//                     }
//                 }
//                 retry--;
//             }
//             return null;
//         }
    }
    /// <summary>
    /// 包头
    /// </summary>
    public class stm32f4_RxPacketHead
    {
        /// <summary>
        /// 包头大小
        /// </summary>        
        public static Int32 DataLen_SIZE = 2;
        public static Int32 CommandLen_SIZE = 2;
        public static Int32 HEAD_SIZE = 8;
        public ushort Command;
        public UInt16 DataLen = 0;
        public byte bError;
        public byte ErrCode;
        public byte[] Data;
        public stm32f4_RxPacketHead()
        {
            Data = new byte[HEAD_SIZE];
        }
    }
    public class stm32f4_RxPacket
    {
        public stm32f4_RxPacketHead Header;
        public byte[] Data;
        public int RxCount;
        public byte[] RxList;
        public ushort NeedCommand;
        public static Int32 BUFFER_SIZE = 1024;
        public stm32f4_RxPacket(ushort com)
        {
            NeedCommand = com;
            Header = new stm32f4_RxPacketHead();
            RxCount = 0;
        }
        public stm32f4_RxPacket()
        {
            NeedCommand = 0xff;
            Header = new stm32f4_RxPacketHead();
            RxCount = 0;
        }        
        public bool DataPacketed(byte rx)
        {
            //判断读取的字节数+缓冲区已有字节数是否超过缓冲区总大小
            if (RxCount < stm32f4_RxPacketHead.HEAD_SIZE)
            {
                Header.Data[RxCount++] = rx;
                if (RxCount == 1)
                {
                    if (Header.Data[0] != 0x16)
                    {
                        RxCount = 0;
                    }
                }
                else if (RxCount == 2)
                {
                    if (Header.Data[1] != 0x16)
                    {
                        RxCount = 0;
                    }
                }
                else if (RxCount == 4)
                {
                    Header.Command = BytesOP.MakeShort(Header.Data[3], Header.Data[2]);
                }
                else if (RxCount == 6)
                {
                    Header.DataLen = BytesOP.MakeShort(Header.Data[5], Header.Data[4]);
                    Data = new byte[Header.DataLen + 2];
                }
                else if (RxCount == 7)
                    Header.bError = Header.Data[6];
                else if (RxCount == 8)
                    Header.ErrCode = Header.Data[7];
            }
            else
            {
                Data[RxCount - stm32f4_RxPacketHead.HEAD_SIZE] = rx;
                RxCount++;
                if (RxCount == (stm32f4_RxPacketHead.HEAD_SIZE + Header.DataLen + 2))
                {
                    if (Data[Data.Length - 1] == 0x0d)
                    {
                        byte s1 = Verify.GetVerify_byteSum(Header.Data);
                        byte s2 = Verify.GetVerify_byteSum(Data, Data.Length - 2);
                        s1 = (byte)(s1 + s2);
                        if (s1 == Data[Data.Length - 2])
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
        public static stm32f4_RxPacket CP1616ComProc(ref SerialPort serialPort, ushort com, int retry)
        {
            stm32f4_RxPacket ret = new stm32f4_RxPacket(com);            
            while (retry != 0)
            {
                while (true)
                {
                    try
                    {
                        if (ret.DataPacketed((byte)serialPort.ReadByte()))
                        {
                            return ret;
                        }
                    }
                    catch// (Exception ex)
                    {
                        //Debug.WriteLine("血糖通信失败" + ex.Message);
                        break;
                    }
                }
                retry--;
            }
            return null;
        }
    }
}
