using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib.PacketProc;

namespace ATA5702下载程序
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CP1616PacketHead.Addr_SIZE = 0;
            CP1616PacketHead.DataLen_SIZE = 1;
            CP1616PacketHead.CommandLen_SIZE = 1;
            CP1616PacketHead.bCheckVerify = false;
            CP1616PacketHead.CalcHeaderLen();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            byte[] data=new byte[64+2];
            byte[] tx = CP1616Packet.MakeCP1616Packet(1, 0, data);
        }
    }
}
