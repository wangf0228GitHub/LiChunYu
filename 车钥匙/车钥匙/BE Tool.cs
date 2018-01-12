using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib.USB.HID;
using System.IO;
using WFNetLib;

namespace 车钥匙
{
    public partial class BE_ToolForm : Form
    {
        public BE_ToolForm()
        {
            InitializeComponent();            
        }
        IntPtr m_hUsbEventHandle;
        private void Form4_Load(object sender, EventArgs e)
        {
            this.Icon = Properties.Resources.MI;
            if (HIDDevice.TheHIDDevice != null)
            {
                toolStripStatusLabel1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
            }
            else
            {
                this.Close();
            }
            progressBar1.Visible = false;
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);            
        }
        protected override void WndProc(ref Message m)
        {
            try
            {
                if (m.Msg == 0x0219)
                {
                    switch (m.WParam.ToInt32())
                    {
                        case 0x8004:
                            try
                            {
                                HIDDevice.TheHIDDevice.Dispose();
                                HIDDevice.TheHIDDevice = null;
                            }
                            catch { }
                            this.Close();
                            break;
                        default:
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            base.WndProc(ref m);
        }
        private void Form4_FormClosing(object sender, FormClosingEventArgs e)
        {
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
        }
        byte[] PSWpub = new byte[0x18];
        bool PSWrd = false;
        private void button1_Click(object sender, EventArgs e)
        {
//             if (MessageBox.Show("Attention! After performing the procedure of reading the password key becomes disabled! Need to overwrite it! Continue?", "Waring", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No)
//                 return;
            textBox4.Text = "";
            textBox3.Text = "";
            textBox2.Text = "";
            textBox1.Text = "";
            WFGlobal.WaitMS(20);
            button1.Enabled = false;
            button2.Enabled = false;
            progressBar1.Visible = true;
            progressBar1.Value = 0;
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
                MessageBox.Show("Authorization information error!!", "Error");
                progressBar1.Visible = false;
                button1.Enabled = true;
                button2.Enabled = true;
                return;
            }
            progressBar1.Value = 33;
            WFGlobal.WaitMS(500);
            toolStripStatusLabel2.Text = "读取中......";
            PSWrd = false;            
            InteractionData tx = new InteractionData();
            tx.buff[0] = 0x13;
            tx.buff[1] = 0x55;	//主机-->设备
            InteractionData rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);            
            if (rx == null)
            {
                toolStripStatusLabel2.Text = "USB Error!!";
                toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.CheckDataList() == false)
            {
                toolStripStatusLabel2.Text = "Reciece Error!!";
                toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.buff[1] == 0x03)
            {
                toolStripStatusLabel2.Text = "Get PSW Error 3 !!";
                toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.buff[1] != 0x00)
            {
                toolStripStatusLabel2.Text = "Get PSW Error 2 !!";
                toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                progressBar1.Value = 66;
                WFGlobal.WaitMS(500);
                tx = new InteractionData();
                tx.buff[0] = 0x01;
                tx.buff[1] = 0xaa;	//主机<--设备
                tx.buff[4] = 0x20;
                rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
                if (rx == null)
                {
                    toolStripStatusLabel2.Text = "USB Error!!";
                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                }
                else if (rx.CheckDataList() == false)
                {
                    toolStripStatusLabel2.Text = "Reciece Error!!";
                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                }
                else if (rx.buff[1] != 0x01)
                {
                    toolStripStatusLabel2.Text = "Get PSW Error 2 !!";
                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                }
                else
                {                    
                    toolStripStatusLabel2.Text = "读取成功!!";

                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
                    byte[] BufPassWord = new byte[0x18];
                    for (int i = 0; i < 0x18; i++)
                    {
                        BufPassWord[i] = rx.buff[i + 0x10];
                    }
                    for (int i = 0; i < 0x18; i++)
                    {
                        PSWpub[i] = BufPassWord[i];
                    }
                    //
                    //PSW
//                     PSWpub[0x00] = BufPassWord[0x07];
//                     PSWpub[0x01] = BufPassWord[0x0F];
//                     PSWpub[0x02] = BufPassWord[0x00];
//                     PSWpub[0x03] = BufPassWord[0x01];
//                     PSWpub[0x04] = BufPassWord[0x02];
//                     PSWpub[0x05] = BufPassWord[0x03];
//                     PSWpub[0x06] = BufPassWord[0x04];
//                     PSWpub[0x07] = BufPassWord[0x05];
//                     PSWpub[0x08] = BufPassWord[0x06];
//                     //SSID
//                     PSWpub[0x09] = BufPassWord[0x0B];
//                     PSWpub[0x0A] = BufPassWord[0x0C];
//                     PSWpub[0x0B] = BufPassWord[0x0D];
//                     PSWpub[0x0C] = BufPassWord[0x0E];
//                     //HASH
//                     PSWpub[0x0D] = BufPassWord[0x0A];
//                     PSWpub[0x0E] = BufPassWord[0x09];
//                     PSWpub[0x0F] = BufPassWord[0x08];
//                     PSWpub[0x10] = BufPassWord[0x17];
//                     PSWpub[0x11] = BufPassWord[0x16];
//                     PSWpub[0x12] = BufPassWord[0x15];
//                     PSWpub[0x13] = BufPassWord[0x14];
//                     PSWpub[0x14] = BufPassWord[0x13];
//                     //OTHERS
//                     PSWpub[0x15] = BufPassWord[0x12];
//                     PSWpub[0x16] = BufPassWord[0x11];
//                     PSWpub[0x17] = BufPassWord[0x10];

                    textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x0d, 8, " ");//HASH
                    textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x01, 8, " ");//PSD
                    textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(PSWpub, 0x09, 4, " ");//serial
                    textBox2.Text = (BufPassWord[0x09] & 0x07).ToString();//key
                    PSWrd = true;
                    progressBar1.Value = 100;
                    MessageBox.Show("Read PSW OK!!");
                }
            }
            progressBar1.Visible = false;
            button1.Enabled = true;
            button2.Enabled = true;
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (PSWrd == false)
            {
                MessageBox.Show("没有数据用于存储!!");
                return;
            }
            byte[] outBuf = new byte[0xA0];	//最大64M内存
            for (int i = 0; i < 0xa0; i++)
                outBuf[i] = 0xff;
            for (int i = 0; i < 0x18; i++)
            {
                outBuf[i] = PSWpub[i];
            }
            FileInfo f;
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(saveFileDialog1.FileName);
            Stream stream = File.OpenWrite(f.FullName);
            stream.Write(outBuf, 0, outBuf.Length);
            stream.Flush();
            stream.Close();
            toolStripStatusLabel2.Text = "保存成功";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
        }
    }
}
