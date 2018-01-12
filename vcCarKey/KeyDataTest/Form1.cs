using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using DAL;
using System.Threading;
using 服务器端;
using System.IO;
using WFNetLib.StringFunc;
using WFNetLib;

namespace KeyDataTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            WFNetLib.ADO.SQLServerOP.ConnectionString = "Data Source=.\\SQL2008;Initial Catalog=LCY_Server;Persist Security Info=True;User ID=sa;Password=1001001";
            KeyDataQueueDataDBOption.deleteAll();
            Thread t = new Thread(KeyDataWork.KeyDataWorkThread);
            t.Start();
        }
        string sTime;
        string DeviceID = "0445615C";
        private void button1_Click(object sender, EventArgs e)
        {
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            byte[] pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            KeyDataQueueData kdq = new KeyDataQueueData();
            //kdq.IP = WFGlobal.getIp();
            kdq.DeviceID = DeviceID;
            kdq.format = 7;
            kdq.KeyOption = 0x01ff;
            kdq.SSID = StringsFunction.byteToHexStr(pBuf, 0, 4, "");
            kdq.BIN = StringsFunction.byteToHexStr(pBuf, 0, 1024, "");
            int id=KeyDataQueueDataDBOption.Insert(kdq);
            if (KeyDataWork.bCalc == false)
            {
                KeyDataWork.autoResetEvent.Set();
            }
        }
    }
}
