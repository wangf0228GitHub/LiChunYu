using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace hash计算
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] clc = strToHexByte(textBox1.Text, " ");
            lcyHashCal hc = new lcyHashCal();
            byte[] ret = hc.HashCal(clc);
            textBox2.Text = byteToHexStr(ret, " ");
        }
        public byte[] strToHexByte(string hexString, string spilt)
        {
            if (spilt != "")
                hexString = hexString.Replace(spilt, "");
            if ((hexString.Length % 2) != 0)
                hexString += " ";
            byte[] returnBytes = new byte[hexString.Length / 2];
            for (int i = 0; i < returnBytes.Length; i++)
                returnBytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
            return returnBytes;
        }
        public string byteToHexStr(byte[] bytes, string spilt)
        {
            StringBuilder strB = new StringBuilder();
            if (bytes != null)
            {
                for (int i = 0; i < bytes.Length; i++)
                {
                    strB.Append(bytes[i].ToString("X2"));
                    strB.Append(spilt);
                }
            }
            return strB.ToString();
        }

    }
}
