using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] clc = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox1.Text, " ");
            WFNetLib.lcyHashCal hc = new WFNetLib.lcyHashCal();
            byte[] ret=hc.HashCal(clc, (int)numericUpDown1.Value);
            textBox2.Text = WFNetLib.StringFunc.StringsFunction.byteToHexStr(ret, " ");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] clc = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox3.Text, " ");
            byte[] ret = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox5.Text, " ");
            WFNetLib.lcyHashCal hc = new WFNetLib.lcyHashCal();
            bool bok=false;
            for (int i = 0; i < 200000; i++)
            {
                clc = hc.HashCal(clc);
                bok = true;
                for (int j = 0; j < 8; j++)
                {
                    if (clc[j] != ret[j])
                    {
                        bok = false;
                        break;
                    }
                }
                if (bok)
                {
                    textBox4.Text = (i+1).ToString();
                    return;
                }
            }
            textBox4.Text = "没找到";
        }
    }
}
