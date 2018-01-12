using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace wfHashCal
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            if (stream.Length == 1024 * 18 * 4 + 8)
            {
                byte[] LoadBuf = new byte[stream.Length];
                stream.Read(LoadBuf, 0, (int)stream.Length);
                stream.Close();
                byte[] DeviceID = new byte[4];
                byte[] SSID = new byte[4];
                for (int i = 0; i < 4; i++)
                {
                    DeviceID[i] = LoadBuf[i];
                    SSID[i] = LoadBuf[i + 4];
                }
                byte[] data=new byte[1024*18*4];
                for(int i=0;i<1024*18*4;i++)
                {
                    data[i]=LoadBuf[i+8];
                }
                byte[] pDecode=GlobalFunc.Encrypt(data,DeviceID,SSID);
		        byte[] pData=GlobalFunc.PSWDecrypt(pDecode,false);
		        byte[] tx=new byte[0x401*0x08];
		        for(int i=0;i<8;i++)
			        tx[i]=LoadBuf[i];
		        for(int j=0;j<1024;j++)
		        {
		 	        for(int i=0;i<8;i++)
		 	        {
		 		        tx[8+0x08*j+i]=(byte)(pData[0x40*j+0x30+i]^pData[0x40*j+0x21+i]);
		 	        }
		        }
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < 0x400; i++)
                {
                    sb.Append(byteToHexStr(tx, i * 8+8, 8, " "));
                    sb.Append("\r\n");
                }
                sb.Remove(sb.Length - 2, 2);
                FileStream fs;
                if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                    return;
                fs = new FileStream(saveFileDialog1.FileName, FileMode.Create);
                StreamWriter sw = new StreamWriter(fs);
                fs.Seek(0, SeekOrigin.End);
                sw.WriteLine(sb.ToString());
                //清空缓冲区
                sw.Flush();
                //关闭流
                sw.Close();
                fs.Close();/**/
                MessageBox.Show("RF文件转换成功!!");
            }
            else
            { 
                MessageBox.Show("文件错误!!");
            }
        }
        public static string byteToHexStr(byte[] bytes, string spilt)
        {
            return byteToHexStr(bytes, 0, bytes.Length, spilt);
        }
        public static string byteToHexStr(byte[] bytes, int offset, int count, string spilt)
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

        private void button2_Click(object sender, EventArgs e)
        {
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            if (stream.Length == 0x801 * 0x08)
            {
                byte[] LoadBuf = new byte[stream.Length];
                stream.Read(LoadBuf, 0, (int)stream.Length);
                stream.Close();
                byte[] DeviceID = new byte[4];
                byte[] SSID = new byte[4];
                byte[] data = new byte[0x800 * 0x08];
                for (int i = 0; i < 4; i++)
                {
                    DeviceID[i] = LoadBuf[i];
                    SSID[i] = LoadBuf[i + 4];
                }
                for (int i = 0; i < 16384; i++)
                    data[i] = LoadBuf[i + 8];
                byte[] pSave = GlobalFunc.Encrypt(data, DeviceID, SSID);
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < 0x800; i++)
                {
                    sb.Append(byteToHexStr(pSave, i * 8, 8, " "));
                    sb.Append("\r\n");
                }
                sb.Remove(sb.Length - 2, 2);
                FileStream fs;
                if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                    return;
                fs = new FileStream(saveFileDialog1.FileName, FileMode.Create);
                StreamWriter sw = new StreamWriter(fs);
                fs.Seek(0, SeekOrigin.End);
                sw.WriteLine(sb.ToString());
                //清空缓冲区
                sw.Flush();
                //关闭流
                sw.Close();
                fs.Close();/**/
                MessageBox.Show("RF文件转换成功!!");
            }
            else
            {
                MessageBox.Show("文件错误!!");
            }
        }
    }
    public class GlobalFunc
    {
        public static byte[] PSWDecrypt(byte[] pBuf, bool bQuanDiu)
        {
            byte[] ret;
            int count;
            if (bQuanDiu)
            {
                ret = new byte[2048 * 16 * 4];
                count = 2048 * 4;
            }
            else
            {
                ret = new byte[1024 * 16 * 4];
                count = 1024 * 4;
            }

            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    ret[i * 16 + j] = pBuf[i * 18 + j * 2 + 1];
                }
                for (int j = 9; j < 16; j++)
                {
                    ret[i * 16 + j] = 0x00;
                }
            }
            return ret;
        }
        public static int EncryptOffset = 5;
        public static byte[] Encrypt(byte[] pBuf, byte[] DeviceID, byte[] SSID)
        {
            return Encrypt(pBuf, DeviceID, SSID, 0);
        }
        public static byte[] Encrypt(byte[] pBuf, byte[] DeviceID, byte[] SSID, int offset)
        {
            byte[] ret = new byte[pBuf.Length];
            byte[] key = new byte[8];
            for (int i = 0; i < 4; i++)
            {
                key[i] = (byte)(DeviceID[i] + EncryptOffset + offset);
                key[i + 4] = (byte)(SSID[i] + EncryptOffset + offset);
            }
            int c = pBuf.Length / 8;
            for (int i = 0; i < c; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    ret[i * 8 + j] = (byte)(key[j] ^ pBuf[i * 8 + j]);
                }
            }
            return ret;
        }        
    }
}
