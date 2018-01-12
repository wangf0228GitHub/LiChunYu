using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib;

namespace 钥匙写入数据加密
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] sin = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox1.Text, " ");
            if (sin.Length == 8)
            {
                byte[] sout = lcyHexDecode.Decode(sin);
                textBox2.Text=WFNetLib.StringFunc.StringsFunction.byteToHexStr(sout, " ");
            }
            else
            {
                MessageBox.Show("输入的数据格式有误");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
//             byte[] sin = new byte[8];
//             for(int i=0;i<8;i++)
//                 sin[i]=(byte)(i+1);
//             byte[] sout = lcyHexDecode.Decode(sin);
        }
    }
}
