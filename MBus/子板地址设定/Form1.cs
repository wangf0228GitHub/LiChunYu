using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 子板地址设定
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                if (comboBox1.SelectedIndex == -1)
                {
                    MessageBox.Show("请选择设备所对应串口!");
                    return;
                }
                serialPort1.PortName = comboBox1.Text;
                try
                {
                    serialPort1.Open();
                }
                catch
                {
                    MessageBox.Show("所选串口打开失败!");
                    return;
                }
            }
            byte[] tx = new byte[6];
            tx[0] = 0x16;
            tx[1] = (byte)numericUpDown1.Value;
            tx[2] = 0x02;
            tx[3] = (byte)numericUpDown2.Value;
            tx[4] = (byte)(tx[0] + tx[1] + tx[2] + tx[3]);
            tx[5] = 0x0d;
            textBox1.AppendText("发送:" + byteToHexStr(tx, 0, tx.Length, " ") + "\r\n");
            serialPort1.Write(tx, 0, tx.Length);
            byte[] rx = new byte[6];
            int rxRount = 0;
            while (true)
            {
                try
                {
                    byte rxrx = (byte)serialPort1.ReadByte();
                    rx[rxRount++] = rxrx;
                    switch (rxRount)
                    {
                        case 1:
                            if (rx[0] != 0x16)
                                rxRount = 0;
                            break;
                        case 2:
                            if (rx[1] != tx[3])
                                rxRount = 0;
                            break;
                        // 	case 3:
                        // 		break;
                        // 	case 4:
                        // 		break;
                        case 6:
                            textBox1.AppendText("接收:" + byteToHexStr(rx, 0,rx.Length, " "));
                            if (rx[5] != 0x0d)
                                rxRount = 0;
                            else
                            {
                                byte sum= (byte)(rx[0] + rx[1] + rx[2] + rx[3]);
                                if (sum == rx[4])
                                {
                                    MessageBox.Show("设定成功");
                                }
                                else
                                    rxRount = 0;
                            }
                            break;
                        default:
                            break;
                    }                    
                }
                catch
                {
                    MessageBox.Show("连接超时");
                    return;
                }
            }
        }
        public string byteToHexStr(byte[] bytes, int offset, int count, string spilt)
        {
            StringBuilder strB = new StringBuilder();
            if (bytes != null)
            {
                for (int i = 0; i < count; i++)
                {
                    strB.Append(bytes[offset + i].ToString("X2"));
                    strB.Append(spilt);
                }
            }
            return strB.ToString();
        }
    }
    
}
