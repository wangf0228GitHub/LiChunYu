using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 钥匙次数计算
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            byte[] h = strToHexByte(textBox1.Text, " ");
            if (h.Length != 3)
            {
                MessageBox.Show("输入数据有误");
                return;
            }
            int lift = h[2] * 32 * 128 + h[1] * 128 + h[0];
            textBox2.Text = lift.ToString();
        }
        private void button2_Click(object sender, EventArgs e)
        {
            int lift;
            if (!int.TryParse(textBox2.Text, out lift))
            {
                MessageBox.Show("输入数据有误");
                return;
            }
            byte[] h = new byte[3];
            h[2] = (byte)(lift / (32 * 128));
            lift = lift - h[2] * 32 * 128;
            h[1] = (byte)(lift / (128));
            lift = lift - h[1]* 128;
            h[0] = (byte)lift;
            textBox1.Text = byteToHexStr(h, 0, 3, " ");
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
    }
}
