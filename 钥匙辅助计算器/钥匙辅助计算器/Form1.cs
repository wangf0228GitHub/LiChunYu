﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib.StringFunc;

namespace 钥匙辅助计算器
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
			byte[] ret = hc.HashCal(clc, (int)numericUpDown1.Value);
			textBox2.Text = WFNetLib.StringFunc.StringsFunction.byteToHexStr(ret, " ");
		}

		private void button2_Click(object sender, EventArgs e)
		{
			byte[] clc = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox3.Text, " ");
			byte[] ret = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox5.Text, " ");
			WFNetLib.lcyHashCal hc = new WFNetLib.lcyHashCal();
			bool bok = false;
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
					textBox4.Text = (i + 1).ToString();
					return;
				}
			}
			textBox4.Text = "没找到";
		}

		private void button4_Click(object sender, EventArgs e)
		{
			byte[] h = StringsFunction.strToHexByte(textBox7.Text, " ");
			if (h.Length != 3)
			{
				MessageBox.Show("输入数据有误");
				return;
			}
			int lift = h[2] * 32 * 128 + h[1] * 128 + h[0];
			textBox6.Text = lift.ToString();
		}

		private void button3_Click(object sender, EventArgs e)
		{
			int lift;
			if (!int.TryParse(textBox6.Text, out lift))
			{
				MessageBox.Show("输入数据有误");
				return;
			}
			byte[] h = new byte[3];
			h[2] = (byte)(lift / (32 * 128));
			lift = lift - h[2] * 32 * 128;
			h[1] = (byte)(lift / (128));
			lift = lift - h[1] * 128;
			h[0] = (byte)lift;
			textBox7.Text = StringsFunction.byteToHexStr(h, 0, 3, " ");
		}

		private void button5_Click(object sender, EventArgs e)
		{
			int i;
			byte[] PSW = StringsFunction.strToHexByte(textBox13.Text, " ");
			byte[] curHash = StringsFunction.strToHexByte(textBox12.Text, " ");
			byte KeyTimes = byte.Parse(textBox11.Text);
			byte keyValue = byte.Parse(textBox10.Text);

			WFNetLib.lcyHashCal hc = new WFNetLib.lcyHashCal();
			byte[] ram93 = hc.HashCal(curHash);
			byte[] KeyData = new byte[8];

			for (i = 0; i < 8; i++)
				KeyData[i] = (byte)(ram93[i] ^ PSW[i]);
			//6
			KeyData[0] ^= ram93[0];
			KeyData[6] ^= KeyTimes;
			KeyData[7] ^= keyValue;

			byte[] h1 = hc.HashCal(KeyData);

			for (i = 0; i < 8; i++)
				KeyData[i] = (byte)(ram93[i] ^ h1[i]);

			textBox9.Text = StringsFunction.byteToHexStr(ram93, " ");
			textBox8.Text = StringsFunction.byteToHexStr(KeyData, " ");
		}

        private void button6_Click(object sender, EventArgs e)
        {
            byte[] b1 = StringsFunction.strToHexByte(textBox16.Text, " ");
            byte[] b2 = StringsFunction.strToHexByte(textBox15.Text, " ");
            byte[] bout = new byte[8];
            for (int i = 0; i < 8; i++)
            {
                bout[i] = (byte)(b1[i] ^ b2[i]);                
            }
            textBox14.Text = StringsFunction.byteToHexStr(bout, " ");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            byte[] RX = StringsFunction.strToHexByte(textBox17.Text, " ");
            byte[] PSW = StringsFunction.strToHexByte(textBox18.Text, " ");

            byte[] xor1 = new byte[8];
            for (int i = 0; i < 8; i++)
                xor1[i] = (byte)(RX[i] ^ PSW[i]);
            textBox19.Text = StringsFunction.byteToHexStr(xor1, " ");

            byte[] fzz = StringsFunction.strToHexByte(textBox20.Text, " ");
            for (int i = 0; i < 4; i++)
                xor1[i+4] = (byte)(xor1[i+4] ^ fzz[i]);
            textBox21.Text = StringsFunction.byteToHexStr(xor1, " ");

            WFNetLib.lcyHashCal hc = new WFNetLib.lcyHashCal();
            byte[] hash = hc.HashCal(xor1);
            textBox22.Text = StringsFunction.byteToHexStr(hash, " ");

            byte[] key27 = StringsFunction.strToHexByte(textBox23.Text, " ");
            for (int i = 0; i < 8; i++)
                xor1[i] = (byte)(hash[i] ^ key27[i]);
            textBox24.Text = StringsFunction.byteToHexStr(xor1, " ");
        }

        private void button8_Click(object sender, EventArgs e)
        {
            byte[] RX = StringsFunction.strToHexByte(textBox17.Text, " ");
            byte[] PSW = StringsFunction.strToHexByte(textBox18.Text, " ");

            byte[] xor1 = new byte[8];
            for (int i = 0; i < 8; i++)
                xor1[i] = (byte)(RX[i] ^ PSW[i]);
            textBox19.Text = StringsFunction.byteToHexStr(xor1, " ");

            byte[] fzz = StringsFunction.strToHexByte(textBox20.Text, " ");
            for (int i = 0; i < 4; i++)
                xor1[i + 4] = (byte)(xor1[i + 4] ^ fzz[i]);
            textBox21.Text = StringsFunction.byteToHexStr(xor1, " ");
        }
	}
}
