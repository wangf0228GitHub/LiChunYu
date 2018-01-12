using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using WFNetLib;
using System.Diagnostics;
using System.Threading;

namespace PCTest
{    
    public class ComProc
    {
        SerialPort com;
        public ComProc(string PortName)
        {
            com= new SerialPort();
            com.PortName = PortName;
            com.BaudRate = 9600;
            com.ReadTimeout = 2000;
        }
        public byte[] CommandProc(byte[] txdata)
        { 
            if (!WFNetLib.WFGlobal.OpenSerialPort(ref com, ""))
                return null;
            Debug.WriteLine(WFNetLib.StringFunc.StringsFunction.byteToHexStr(txdata, " "));
            byte[] txWakeUp = new byte[1];
            txWakeUp[0] = 0x0f;
            com.Write(txWakeUp, 0, txWakeUp.Length);
            Thread.Sleep(20);
            com.Write(txdata, 0, txdata.Length);
            byte[] ret = new byte[11];
            int rxCount = 0;
            while (true)
            {
                try
                {
                    ret[rxCount] = (byte)com.ReadByte();
                    rxCount++;
                    if (rxCount==11)
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
