using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using WFNetLib;
using System.Diagnostics;

namespace PCTest
{
    public class SetUartBaudRate
    {
        public static byte[] GetTxData(ushort BaudRate)
        {
            byte[] ret = new byte[2];
            ret[0] = BytesOP.GetLowByte(BaudRate);
            ret[1] = BytesOP.GetHighByte(BaudRate);
            return ret;
        }
    }
    public class UARTCommand
    {
        //taken from c struct in platform sdk 
        const int ParamLen = 4;
        public ushort Retry;
        public ushort RxTimeOut;
        public byte[] Data;//[UART_WithPC_BUFMAX-6];
        public byte[] GetTxData()
        {
            byte[] ret = new byte[Data.Length + ParamLen];
            ret[0] = BytesOP.GetLowByte(Retry);
            ret[1] = BytesOP.GetHighByte(Retry);
            ret[2] = BytesOP.GetLowByte(RxTimeOut);
            ret[3] = BytesOP.GetHighByte(RxTimeOut);
            for (int i = 0; i < Data.Length; i++)
            {
                ret[ParamLen + i] = Data[i];
            }
            return ret;
        }
    }
    public class SetCANBaudRateCommand
    {        
        //工作设定相关  2+1+1+1+1=7
        public ushort Prescaler;
        public byte BS1;
        public byte BS2;
        public byte SJW;
        public byte Param;
        public byte RNum;//总线电阻序号
        public byte[] GetTxData()
        {
            byte[] ret = new byte[7];
            ret[0] = BytesOP.GetLowByte(Prescaler);
            ret[1] = BytesOP.GetHighByte(Prescaler);
            ret[2] = BS1;
            ret[3] = BS2;
            ret[4] = SJW;
            ret[5] = Param;
            ret[6] = RNum;
            return ret;
        }
    }
    public class CANCommand
    {
        public const byte CAN_ID_STD = 0x00;
        public const byte CAN_ID_EXT = 0x04;

        public const byte CAN_RTR_DATA = 0x00;
        public const byte CAN_RTR_REMOTE = 0x02; 
        //taken from c struct in platform sdk 
//         //工作设定相关  2+1+1+1+1=6
// 		public ushort Prescaler;
// 		public byte BS1;
// 		public byte BS2;
// 		public byte SJW;
// 		public byte Param;
				
		//发送请求	2+2+4+4+8+2+2=22			
		public ushort TxStdId;//Min_Data = 0 and Max_Data = 0x7FF
        public ushort TxBak1;
		public uint TxExtId;//Min_Data = 0 and Max_Data = 0x1FFFFFFF
		public byte TxIDE;//CAN_Identifier_Type
		public byte TxRTR;//CAN_remote_transmission_request
		public byte TxDLC;//Min_Data = 0 and Max_Data = 8
		public byte TxBak2;//备用
		public byte[] TxData;

		public ushort TxRetry;
		public ushort TxTimeOut;
		//接收需求配置  2+2+2+2+4=12
		public byte bTxOnly;
		public byte RxIDE;
		public byte RxRetry;
        public byte NeedRxPocket;
		public ushort RxTimeOut;
		public ushort NeedStdId;
		public uint NeedExtId;//Min_Data = 0 and Max_Data = 0x1FFFFFFF	
	    public CANCommand()
        {
            TxIDE = CANCommand.CAN_ID_STD;
            TxRTR = CANCommand.CAN_RTR_DATA;
            TxDLC = 8;
            RxIDE = CANCommand.CAN_ID_STD;
            bTxOnly = 0;
            TxData=new byte[8];
            NeedRxPocket = 1;
            RxRetry = 3;
            RxTimeOut = 200;
            TxRetry = 3;
            TxTimeOut = 5;
        }
        public byte[] GetTxData()
        {
            byte[] ret = new byte[24+12];
            ret[0] = BytesOP.GetLowByte(TxStdId);
            ret[1] = BytesOP.GetHighByte(TxStdId);
            ret[2] = BytesOP.GetLowByte(TxBak1);
            ret[3] = BytesOP.GetHighByte(TxBak1);
            ret[4] = BytesOP.GetLowByte(BytesOP.GetLowShort(TxExtId));
            ret[5] = BytesOP.GetHighByte(BytesOP.GetLowShort(TxExtId));
            ret[6] = BytesOP.GetLowByte(BytesOP.GetHighShort(TxExtId));
            ret[7] = BytesOP.GetHighByte(BytesOP.GetHighShort(TxExtId));
            ret[8] = TxIDE;
            ret[9] = TxRTR;
            ret[10] = TxDLC;
            ret[11] = TxBak2;//
            for (int i = 0; i < 8; i++)
            {
                ret[12+i] = TxData[i];
            }
            ret[20] = BytesOP.GetLowByte(TxRetry);
            ret[21] = BytesOP.GetHighByte(TxRetry);
            ret[22] = BytesOP.GetLowByte(TxTimeOut);
            ret[23] = BytesOP.GetHighByte(TxTimeOut);


            ret[24] = bTxOnly;
            ret[25] = RxIDE;
            ret[26] = RxRetry;
            ret[27] = NeedRxPocket;
            ret[28] = BytesOP.GetLowByte(RxTimeOut);
            ret[29] = BytesOP.GetHighByte(RxTimeOut);
            ret[30] = BytesOP.GetLowByte(NeedStdId);
            ret[31] = BytesOP.GetHighByte(NeedStdId);
            ret[32] = BytesOP.GetLowByte(BytesOP.GetLowShort(NeedExtId));
            ret[33] = BytesOP.GetHighByte(BytesOP.GetLowShort(NeedExtId));
            ret[34] = BytesOP.GetLowByte(BytesOP.GetHighShort(NeedExtId));
            ret[35] = BytesOP.GetHighByte(BytesOP.GetHighShort(NeedExtId)); 
            return ret;
        }
    }
    public class ComProc
    {
        SerialPort com;
        public ComProc(string PortName)
        {
            com= new SerialPort();
            com.PortName = PortName;
            com.BaudRate = 115200;
            com.ReadTimeout = 5000;
        }
        public stm32f4_RxPacket CommandProc(ushort command,  byte[] txdata,int rxTimeOut)
        {
            byte[] txBuf = stm32f4_TxPacket.MakeCP1616Packet(command,  txdata);  
            if (!WFNetLib.WFGlobal.OpenSerialPort(ref com, "stm32f4 pc端"))
                return null;
            Debug.WriteLine(WFNetLib.StringFunc.StringsFunction.byteToHexStr(txBuf, " "));
            com.ReadTimeout = rxTimeOut;
            com.Write(txBuf, 0, txBuf.Length);
            stm32f4_RxPacket ret = new stm32f4_RxPacket(command);
            while (true)
            {
                try
                {
                    if (ret.DataPacketed((byte)com.ReadByte()))
                    {
                        com.Close();
                        return ret;
                    }
                }
                catch// (Exception ex)
                {
                    //Debug.WriteLine("血糖通信失败" + ex.Message);
                    //MessageBox.Show("发送错误");
                    //textBox2.Text = "发送错误";
                    com.Close();
                    return null;
                }
            }
            
        }
    }
}
