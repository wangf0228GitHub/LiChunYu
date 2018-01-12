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
using System.IO;
using System.Threading;

namespace PCTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        byte[] Read_76_TxList;
        private void Form1_Load(object sender, EventArgs e)
        {
            comProc=new ComProc("COM19");
            string[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            cbCom.Items.Clear();
            cbCom.Items.AddRange(ports);
            cbCom.Text = "COM19";
            Read_76_TxList = new byte[11];
            Read_76_TxList[0] = 0x76;
            Read_76_TxList[1] = 0;
            Read_76_TxList[2] = 0;//命令
            Read_76_TxList[3] = 0;
            Read_76_TxList[4] = 0;
            Read_76_TxList[5] = 0;
            Read_76_TxList[6] = 0;
            Read_76_TxList[7] = 0;
            Read_76_TxList[8] = 0;
            Read_76_TxList[9] = 0;
            Read_76_TxList[10] = 76;//校验
        }
        ComProc comProc;
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            Read_76_TxList = new byte[11];
            Read_76_TxList[0] = 0x76;
            Read_76_TxList[1] = 0;
            Read_76_TxList[2] = 0;//命令
            Read_76_TxList[3] = 0;
            Read_76_TxList[4] = 0;
            Read_76_TxList[5] = 0;
            Read_76_TxList[6] = 0;
            Read_76_TxList[7] = 0;
            Read_76_TxList[8] = 0;
            Read_76_TxList[9] = 0;
            Read_76_TxList[10] = 76;//校验
            byte[] Cmd = new byte[0x03] { 0x90, 0x91, 0xC7 };
            textBox1.Clear();
            //byte[] Cmd = new byte[0x03] { 0x01, 0x02, 0x03 };
            for (int i = 0; i < Cmd.Length; i++)
            {
                Read_76_TxList[2] = Cmd[i];
                Read_76_TxList[10] = Verify.GetVerify_byteSum(Read_76_TxList, 0, 10);
                textBox1.AppendText("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(Read_76_TxList, " ") + "\r\n");
                byte[] rx = comProc.CommandProc(Read_76_TxList);
                if (rx == null)
                {
                    textBox1.AppendText("接收错误\r\n");
                }
                else
                {
                    textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx, " ") + "\r\n");
                }
                Thread.Sleep(50);
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            Read_76_TxList = new byte[11];
            Read_76_TxList[0] = 0x76;
            Read_76_TxList[1] = 0;
            Read_76_TxList[2] = 0;//命令
            Read_76_TxList[3] = 0;
            Read_76_TxList[4] = 0;
            Read_76_TxList[5] = 0;
            Read_76_TxList[6] = 0;
            Read_76_TxList[7] = 0;
            Read_76_TxList[8] = 0;
            Read_76_TxList[9] = 0;
            Read_76_TxList[10] = 76;//校验
            byte[] Cmd = new byte[0x34]{
		0x00,0x01,0x02,0x03,0x10,0x11,0x12,0x13,0x20,0x21,0x22,0x23,0x30,0x31,0x32,0x33,
		0x40,0x41,0x42,0x43,0x50,0x51,0x52,0x53,0x60,0x61,0x62,0x63,0x70,0x71,0x72,0x73,
		0x80,0x81,0x82,0x83,0x90,0x91,0x92,0x93,0x94,0xA0,0xB0,0xC0,0xC2,0xC3,0xC5,0xC7,
		0xC8,0xC9,0xCA,0xCB,};
            textBox1.Clear();
            for (int i = 0; i < Cmd.Length; i++)
            {
                Read_76_TxList[2] = Cmd[i];
                Read_76_TxList[10] = Verify.GetVerify_byteSum(Read_76_TxList, 0, 10);
                textBox1.AppendText("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(Read_76_TxList, " ") + "\r\n");
                byte[] rx = comProc.CommandProc(Read_76_TxList);
                if (rx == null)
                {
                    textBox1.AppendText("接收错误\r\n");
                }
                else
                {
                    textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx, " ") + "\r\n");
                }
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            Read_76_TxList = new byte[11];
            Read_76_TxList[0] = 0x52;
            Read_76_TxList[1] = 0;
            Read_76_TxList[2] = 0xbb;//psw
            Read_76_TxList[3] = 0x85;
            Read_76_TxList[4] = 0x52;
            Read_76_TxList[5] = 0xdf;
            Read_76_TxList[6] = 0x0a;
            Read_76_TxList[7] = 0x3b;
            Read_76_TxList[8] = 0xa1;
            Read_76_TxList[9] = 0xd6;
            Read_76_TxList[10] = 76;//校验
           
            textBox1.Clear();
            Read_76_TxList[10] = Verify.GetVerify_byteSum(Read_76_TxList, 0, 10);
            textBox1.AppendText("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(Read_76_TxList, " ") + "\r\n");
            byte[] rx = comProc.CommandProc(Read_76_TxList);
            if (rx == null)
            {
                textBox1.AppendText("接收错误\r\n");
            }
            else
            {
                textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx, " ") + "\r\n");
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            byte[,] TXbuf = new byte[0x11, 0x0b]{
		{0x15,0x01,0x7C,0xFE,0xD6,0x5C,0x68,0xFF,0x98,0x9C,0x00},//0
		{0x96,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//1
		{0x96,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//2
		{0x96,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//3
		{0x96,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//4
		{0x96,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//5
		{0x96,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//6
		{0x96,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//7
		{0x96,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//8
		{0x96,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//9
		{0x96,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//A--psw
		{0x96,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//B--sum all
		{0xA0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//C
		{0xA0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//D
		{0xA0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//E
		{0x9A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//F
		{0x9A,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//0
	};
            byte[,] RXbuf = new byte[0x11, 0x0b]{
		{0x8A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//0
		{0x8B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//1
		{0x8B,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//2
		{0x8B,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//3
		{0x8B,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//4
		{0x8B,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//5
		{0x8B,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//6
		{0x8B,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//7
		{0x8B,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//8
		{0x8B,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//9
		{0x8B,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//A
		{0x8B,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//B
		{0xA1,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//C
		{0xA1,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//D
		{0xA1,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//E
		{0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//F
		{0x98,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//0
	};
            byte SumH, SumL;
            bool fWR = false;
            bool fSam = false;
            int i, j;
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            byte[] pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            for (j = 0; j < 8; j++)
            {
                for (i = 0; i < 8; i++)
                {
                    TXbuf[j + 0x01, i + 0x02] = pBuf[0x270 + 0x10 * j + i];
                }
            }
            //
            for (i = 0; i < 8; i++)
            {
                TXbuf[0x09, i + 0x02] = pBuf[0x10 + i];//delaer
            }
            
            //等待加入密码
            for (i = 0; i < 8; i++)
            {
                TXbuf[0x0A, i + 0x02] = 0x00;//cPSW
            }

//             TXbuf[0x0A,  0x02] = 0x47;
//             TXbuf[0x0A, 0x03] = 0x80;
//             TXbuf[0x0A, 0x04] = 0x22;
//             TXbuf[0x0A, 0x05] = 0x71;
//             TXbuf[0x0A, 0x06] = 0x85;
//             TXbuf[0x0A, 0x07] = 0x87;
//             TXbuf[0x0A, 0x08] = 0x2f;
//             TXbuf[0x0A, 0x09] = 0x9b;

            for (j = 0; j < 3; j++)
            {
                TXbuf[j + 0x0C, 0x02] = pBuf[0x02];
                TXbuf[j + 0x0C, 0x03] = pBuf[0x00];
                TXbuf[j + 0x0C, 0x06] = pBuf[0x03];
                TXbuf[j + 0x0C, 0x09] = pBuf[0x01];
            }
            //
            for (i = 0; i < 7; i++)
            {
                TXbuf[0x0F, i + 0x02] = pBuf[0x303 + i];
            }
            for (i = 0; i < 6; i++)
            {
                TXbuf[0x10, i + 0x02] = pBuf[0x30A + i];
            }
            TXbuf[0x10, 0x08] = pBuf[0x310];
            //--
            byte Sum;
            for (j = 0; j < 0x11; j++)
            {
                Sum = 0x00;
                for (i = 0; i < 0x0A; i++)
                {
                    Sum += TXbuf[j, i];
                }
                TXbuf[j, 0x0A] = Sum;
            }
            //
            SumH = 0x00;
            SumL = 0x00;
            for (j = 0; j < 0x0A; j++)
            {
                for (i = 0; i < 0x08; i++)
                {
                    if ((SumL + TXbuf[j + 0x01, i + 0x02]) > 0xFF)
                    {
                        SumH++;
                    }
                    SumL += TXbuf[j + 0x01, i + 0x02];
                }
            }
            TXbuf[0x0B, 0x09] = SumH;
            TXbuf[0x0B, 0x08] = SumL;
            TXbuf[0x0B, 0x0A] = (byte)(TXbuf[0x0B, 0x0A] + SumH + SumL);//总校验包的校验和

            //---接收比较缓存整理
            lcyHashCal cpCalc = new lcyHashCal();
            for (j = 0; j < 0x0B; j++)
            {
                for (i = 0; i < 0x08; i++)
                {
                    cpCalc.HashIn[i] = TXbuf[j + 0x01, i + 0x02];
                }
                cpCalc.HashOnce();
                for (i = 0; i < 0x08; i++)
                {
                    RXbuf[j + 0x01, i + 0x02] = cpCalc.HashOut[i];
                }
            }
            //
            for (j = 0; j < 3; j++)
            {
                RXbuf[j + 0x0C, 0x02] = pBuf[0x02];
                RXbuf[j + 0x0C, 0x03] = pBuf[0x00];
                RXbuf[j + 0x0C, 0x06] = pBuf[0x03];
                RXbuf[j + 0x0C, 0x09] = pBuf[0x01];
            }
            textBox1.Clear();
            for (i = 0; i < 0x11; i++)
            {
//                 Read_76_TxList[2] = Cmd[i];
//                 Read_76_TxList[10] = Verify.GetVerify_byteSum(Read_76_TxList, 0, 10);
                Read_76_TxList=new byte[11];
                for(j=0;j<11;j++)
                    Read_76_TxList[j]=TXbuf[i,j];
                textBox1.AppendText("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(Read_76_TxList, " ") + "\r\n");
                byte[] rx = comProc.CommandProc(Read_76_TxList);
                if (rx == null)
                {
                    textBox1.AppendText("接收错误\r\n");
                }
                else
                {
                    textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx, " ") + "\r\n");
                }
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            Read_76_TxList = new byte[11];
            Read_76_TxList[0] = 0x30;
            Read_76_TxList[1] = 0;
            Read_76_TxList[2] = 0x09;//psw
            Read_76_TxList[3] = 0x4a;
            Read_76_TxList[4] = 0x3f;
            Read_76_TxList[5] = 0x6e;
            Read_76_TxList[6] = 0x3a;
            Read_76_TxList[7] = 0x49;
            Read_76_TxList[8] = 0x8d;
            Read_76_TxList[9] = 0x0b;
            Read_76_TxList[10] = 76;//校验

            textBox1.Clear();
            Read_76_TxList[10] = Verify.GetVerify_byteSum(Read_76_TxList, 0, 10);
            textBox1.AppendText("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(Read_76_TxList, " ") + "\r\n");
            byte[] rx = comProc.CommandProc(Read_76_TxList);
            if (rx == null)
            {
                textBox1.AppendText("接收错误\r\n");
            }
            else
            {
                textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx, " ") + "\r\n");
            }
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }
    }
}
