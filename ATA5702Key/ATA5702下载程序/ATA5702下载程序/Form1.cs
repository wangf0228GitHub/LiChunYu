using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib.PacketProc;
using System.IO;
using WFNetLib;
using System.Threading;

namespace ATA5702下载程序
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CP1616PacketHead.Addr_SIZE = 0;
            CP1616PacketHead.DataLen_SIZE = 1;
            CP1616PacketHead.CommandLen_SIZE = 1;
            CP1616PacketHead.bCheckVerify = false;
            CP1616PacketHead.CalcHeaderLen();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            comList.Items.Clear();
            comList.Items.AddRange(ports);
            comList.SelectedIndex = 0;           
        }
        byte[] LoadBuf;
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            if (stream.Length != 32576)
            {
                MessageBox.Show("打开程序文件大小有误，请确认!!");
                return;
            }
            this.Text = f.FullName;

            LoadBuf = new byte[stream.Length];
            stream.Read(LoadBuf, 0, (int)stream.Length);
            stream.Close();
            StringBuilder sb = new StringBuilder();
            textBox1.Text = "";
            int i = 0;
            for (i = 0; i < LoadBuf.Length; i++)
            {
                if (i % 8 == 0 && i != 0)
                    sb.Append("//0x" + (i - 8).ToString("X2") + "～" + (i - 1).ToString("X2") + "\r\n");
                sb.Append("0x" + LoadBuf[i].ToString("X2") + ", ");
            }
            if (i % 8 == 0 && i != 0)
                sb.Append("//0x" + (i - 8).ToString("X2") + "～" + (i - 1).ToString("X2") + "\r\n");
            textBox1.Text = sb.ToString();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            serialPort1.PortName = comList.Text;
            progressBar1.Value = 0;
            if (!WFGlobal.OpenSerialPort(ref serialPort1))
                return;
            try
            {
                byte[] data = new byte[66];
                ushort addr;
                //byte[] tx;
                //0x6000/64=384
                for (int i = 0; i < 383; i++)
                {    
                    addr=(ushort)(0x8000+i*64);
                    data[0] = BytesOP.GetHighByte(addr);
                    data[1] = BytesOP.GetLowByte(addr);
                    for (int j = 0; j < 64; j++)
                        data[2+j] = LoadBuf[i * 64 + j];
                    CP1616Packet ret = CP1616Packet.CP1616ComProc(ref serialPort1, 1, 1, data, 3);
                    if (ret==null)
                    {
                        MessageBox.Show("第"+(i+1).ToString()+"帧数据写入出错");
                        serialPort1.Close();
                        return;
                    }
                    progressBar1.PerformStep();
//                     tx = CP1616Packet.MakeCP1616Packet(1, 1, data);
//                     serialPort1.Write(tx, 0, tx.Length);
//                     
//                     Thread.Sleep(500);
                }
                addr = (ushort)(0x8000 + 383 * 64);
                data[0] = BytesOP.GetHighByte(addr);
                data[1] = BytesOP.GetLowByte(addr);
                for (int j = 0; j < 64; j++)
                    data[2 + j] = LoadBuf[383 * 64 + j];
                CP1616Packet ret2 = CP1616Packet.CP1616ComProc(ref serialPort1, 2, 1, data, 3);
                if (ret2 == null)
                {
                    MessageBox.Show("第384帧数据写入出错");
                    serialPort1.Close();
                    return;
                }
                progressBar1.PerformStep();
                MessageBox.Show("程序写入成功，请重新上电");
                serialPort1.Close();
            }
            catch 
            {
                MessageBox.Show("发生错误");
                serialPort1.Close();
            }
        }
    }
}
