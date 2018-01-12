using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using WFNetLib;

namespace PCTest
{    
    public partial class ESLForm : Form
    {
        public ESLForm()
        {
            InitializeComponent();
        }
        UARTCommand uartCommand;
        private void ESLForm_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            cbCom.Items.Clear();
            cbCom.Items.AddRange(ports);
            cbCom.Text = "COM5";            
            uartCommand = new UARTCommand();
            uartCommand.Retry = 3;
            uartCommand.RxTimeOut = 500;
            uartCommand.Data = new byte[11];
            uartCommand.Data[0] = 0x76;
            uartCommand.Data[1] = 0;
            uartCommand.Data[2] = 0;//命令
            uartCommand.Data[3] = 0;
            uartCommand.Data[4] = 0;
            uartCommand.Data[5] = 0;
            uartCommand.Data[6] = 0;
            uartCommand.Data[7] = 0;
            uartCommand.Data[8] = 0;
            uartCommand.Data[9] = 0;
            uartCommand.Data[10] = 76;//校验
        }
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            byte[] Cmd=new byte[0x03]{0x90,0x91,0xC7};
            textBox1.Clear();
            stm32f4_RxPacket rx1 = MainForm.comProc.CommandProc(3, SetUartBaudRate.GetTxData(9600), 1000);
            if (rx1 == null)
            {
                textBox1.AppendText("接收错误\r\n");
                return;
            }
            for (int i = 0; i < Cmd.Length; i++)
            {
                uartCommand.Data[2] = Cmd[i];
                uartCommand.Data[10] = Verify.GetVerify_byteSum(uartCommand.Data, 0, 10);
                textBox1.AppendText("发送:"+WFNetLib.StringFunc.StringsFunction.byteToHexStr(uartCommand.Data, " ")+"\r\n");
                stm32f4_RxPacket rx = MainForm.comProc.CommandProc(2, uartCommand.GetTxData(),1000);
                if (rx == null)
                {
                    textBox1.AppendText("接收错误\r\n");
                }
                else
                {
                    textBox1.AppendText("接收:"+ WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx.Data, " ")+"\r\n");
                } 
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            byte[] Cmd=new byte[0x34]{
		0x00,0x01,0x02,0x03,0x10,0x11,0x12,0x13,0x20,0x21,0x22,0x23,0x30,0x31,0x32,0x33,
		0x40,0x41,0x42,0x43,0x50,0x51,0x52,0x53,0x60,0x61,0x62,0x63,0x70,0x71,0x72,0x73,
		0x80,0x81,0x82,0x83,0x90,0x91,0x92,0x93,0x94,0xA0,0xB0,0xC0,0xC2,0xC3,0xC5,0xC7,
		0xC8,0xC9,0xCA,0xCB,};
            textBox1.Clear();
            stm32f4_RxPacket rx1 = MainForm.comProc.CommandProc(3, SetUartBaudRate.GetTxData(9600), 1000);
            if (rx1 == null)
            {
                textBox1.AppendText("接收错误\r\n");
                return;
            }
            for (int i = 0; i < Cmd.Length; i++)
            {
                uartCommand.Data[2] = Cmd[i];
                uartCommand.Data[10] = Verify.GetVerify_byteSum(uartCommand.Data, 0, 10);
                textBox1.AppendText("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(uartCommand.Data, " ") + "\r\n");
                stm32f4_RxPacket rx = MainForm.comProc.CommandProc(2,  uartCommand.GetTxData(),3000);
                if (rx == null)
                {
                    textBox1.AppendText("接收错误\r\n");
                }
                else
                {
                    textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx.Data, " ") + "\r\n");
                }
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }
    }    
}
