using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using WFNetLib.PacketProc;

namespace PCTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] command = new string[]{"连接"
                                        ,"ESL读取"};
            cbCommand.Items.AddRange(command);
            cbCommand.SelectedIndex = 0;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] txData = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox1.Text, " ");
            //byte[] txBuf = stm32f4_TxPacket.MakeCP1616Packet((ushort)(cbCommand.SelectedIndex + 1), (ushort)nRetry.Value, (ushort)nTimeOut.Value, txData);
            stm32f4_RxPacket rx = MainForm.comProc.CommandProc((ushort)(cbCommand.SelectedIndex + 1), txData,3000);
            if (rx == null)
            {
                textBox2.Text = "发送错误";
            }
            else
            {
                textBox2.Text = WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx.Data, " ");
            }
            textBox2.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }
    }
}
