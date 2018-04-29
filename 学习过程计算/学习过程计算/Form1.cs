using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 学习过程计算
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] psw = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox1.Text," ");
            byte[] rx = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox2.Text, " ");
            byte[] step = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox3.Text, " ");
            byte[] curHash = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox6.Text, " ");
            byte[] ret = new byte[8];
            for (int i = 0; i < 8; i++)
            {
                ret[i] = (byte)(psw[i] ^ rx[i]);
            }
            for (int i = 0; i < 4; i++)
            {
                ret[i+4] = (byte)(ret[4+i] ^step[i]);
            }
            WFNetLib.lcyHashCal hashCal = new WFNetLib.lcyHashCal();
            for (int i = 0; i < 8; i++)
            {
                hashCal.HashIn[i] = ret[i];
            }
            hashCal.HashOnce();
            textBox4.Text = WFNetLib.StringFunc.StringsFunction.byteToHexStr(hashCal.HashOut, " ");
            for (int i = 0; i < 8; i++)
            {
                ret[i] = (byte)(hashCal.HashOut[i] ^ curHash[i]);
            }
            textBox5.Text = WFNetLib.StringFunc.StringsFunction.byteToHexStr(ret, " ");
        }
    }
}
