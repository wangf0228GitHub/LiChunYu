using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using WFNetLib.USB.HID;
using System.Threading;
using WFNetLib;

namespace 车钥匙
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }
        public byte[] LoadBuf=null;
        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            if (stream.Length == 0xa0)
            {
                LoadBuf = new byte[stream.Length];
                stream.Read(LoadBuf, 0, (int)stream.Length);
                stream.Close(); 
                textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(LoadBuf, 1, 8, " ");
                textBox2.Text = (LoadBuf[9] & 0x07).ToString();
//                 byte x;
//                 x = LoadBuf[9];
//                 LoadBuf[9] = (byte)(LoadBuf[9] & 0xF8);
                textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(LoadBuf, 9, 4, " ");
//                LoadBuf[9] = x;
                this.Text = "Key Module-" + f.FullName;
                toolStripStatusLabel6.Text = "Data Load OK";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
                //-->逆序预处理
                byte Tmp;
                for (int i = 0, j = 0x08; i < 4; i++, j--)
                {
                    Tmp = LoadBuf[i + 0x01];
                    LoadBuf[i + 0x01] = LoadBuf[j];
                    LoadBuf[j] = Tmp;
                }
                for (int i = 0, j = 0x0C; i < 2; i++, j--)
                {
                    Tmp = LoadBuf[i + 0x09];
                    LoadBuf[i + 0x09] = LoadBuf[j];
                    LoadBuf[j] = Tmp;
                }
                //<--逆序预处理
            }                       
            else
            {
                stream.Close();
                LoadBuf = null;
                toolStripStatusLabel6.Text = "文件错误";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                MessageBox.Show("文件错误!!");                
                textBox3.Text = "";
                textBox2.Text = "";
                textBox1.Text = "";
            }
        }
        IntPtr m_hUsbEventHandle;
        private void Form2_Load(object sender, EventArgs e)
        {
            this.Icon = Properties.Resources.MI;
            progressBar1.Visible = false;
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            toolStripStatusLabel5.Text = " ";
            timer1.Enabled = true;
            //this.Enabled = false;
        }
        void HIDUI()
        {
            if (HIDDevice.TheHIDDevice != null)
            {
                toolStripStatusLabel5.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
                ushort hexver = BytesOP.MakeShort(HIDDevice.TheHIDDevice.HexVer[0], HIDDevice.TheHIDDevice.HexVer[1]);
                if (hexver < 0x2001)
                {
                    toolStripButton3.Visible = false;
                    toolStripButton5.Visible = false;
                    toolStripSeparator3.Visible = false;
                    toolStripSeparator2.Visible = false;
                } 
                timer1.Enabled = false;
                //this.Enabled = true;
            }
            else
            {
                toolStripStatusLabel5.Text = "No Device";
                timer1.Enabled = true;
                //this.Enabled = false;
            }
        }
        protected override void WndProc(ref Message m)
        {
            try
            {
                if (m.Msg == 0x0219)
                {
                    switch (m.WParam.ToInt32())
                    {
                        case 0x8000:
                            if (HIDDevice.TheHIDDevice == null)
                            {
                                HIDDevice.TheHIDDevice = HIDDevice.FindDevice();
                            }
                            HIDUI();
                            break;
                        case 0x8004:
                            try
                            {
                                HIDDevice.TheHIDDevice.Dispose();
                                HIDDevice.TheHIDDevice = null;
                            }
                            catch { }
                            HIDUI();
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

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                MessageBox.Show("请连接设备");
                return;
            }
            textBox4.Text = "";
            textBox5.Text = "";
            textBox6.Text = "";
            textBox7.Text = "";
            textBox9.Text = "";
            textBox10.Text = "";
            textBox11.Text = "";
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
                MessageBox.Show("Authorization information error!!","Error");
                return;
            }
            toolStripStatusLabel6.Text = "读取中......";
            toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
            WFGlobal.WaitMS(5);
            InteractionData tx = new InteractionData();
            if (radioButton1.Checked)
                tx.buff[0] = 0x08;
//             else if (radioButton2.Checked)
//                 tx.buff[0] = 0x09;
            else if (radioButton3.Checked)
                tx.buff[0] = 0x10;
            else
            {
                toolStripStatusLabel6.Text = "No Options!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                return;
            }
            tx.buff[1] = 0x55;	//主机-->设备
//             tx.buff[8] = HIDDevice.SwVer[1];//L
//             tx.buff[9] = HIDDevice.SwVer[0];//H
            InteractionData rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
            if (rx == null)
            {
                toolStripStatusLabel6.Text = "USB Error!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.CheckDataList() == false)
            {
                toolStripStatusLabel6.Text = "Reciece Error!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.buff[1] != 0)
            {
                toolStripStatusLabel6.Text = "读取错误!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                toolStripStatusLabel6.Text = "读取成功";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
                if (0x55 == rx.buff[0x10])
                {
                    textBox4.Text = (rx.buff[0x15] & 0x07).ToString();//key
                    //rx.buff[0x15] = (byte)(rx.buff[0x15] & 0xF8);
                    textBox5.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(rx.buff, 0x15, 4, " ");//serial                    
                    byte[] CvtIn = new byte[3];
                    CvtIn[0] = rx.buff[0x14];//0x2F;
                    CvtIn[1] = rx.buff[0x13];//0x1F;
                    CvtIn[2] = rx.buff[0x12];//0x7F;
                    textBox11.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(CvtIn, " ");//counts    
                    int lift = rx.buff[0x14] * 32 * 128 + rx.buff[0x13] * 128 + rx.buff[0x12];
                    textBox7.Text = lift.ToString();
                }
                else
                {
                    textBox5.Text = "";
                    textBox4.Text = "";
                    textBox11.Text = "";
                }
                if (0x55 == rx.buff[0x20])
                {
                    textBox6.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(rx.buff, 0x25, 2, " ");//Status
                    textBox10.Text = rx.buff[0x23].ToString("X2");//Ver.Soft
                    textBox9.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(rx.buff, 0x33, 6, " ");//Remark
                }
                else
                {
                    textBox6.Text = "";
                    textBox10.Text = "";
                    textBox9.Text = "";
                }
            }
        }

        
        void Renew()
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                MessageBox.Show("请连接设备");
                return;
            }
            if (MessageBox.Show("是否要擦除？", "注意", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No)
                return;
            this.Enabled = false;
            progressBar1.Visible = true;
            progressBar1.Value = 0;            
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
                MessageBox.Show("Authorization information error!!", "Error");
                this.Enabled = true;
                progressBar1.Visible = false;
                return;
            }
            progressBar1.Value = 50;
            toolStripStatusLabel6.Text = "擦除中......";
            toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
            WFGlobal.WaitMS(5); 
            InteractionData tx = new InteractionData();
            if (radioButton1.Checked)
                tx.buff[0] = 0x0A;
//             else if (radioButton2.Checked)
//                 tx.buff[0] = 0x0B;
            else if (radioButton3.Checked)
                tx.buff[0] = 0x12;
            else
            {
                toolStripStatusLabel6.Text = "No Options!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                return;
            }
            tx.buff[1] = 0x55;	//主机-->设备
//             tx.buff[8] = HIDDevice.SwVer[1];//L
//             tx.buff[9] = HIDDevice.SwVer[0];//H
            InteractionData rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
            if (rx == null)
            {
                toolStripStatusLabel6.Text = "USB Error!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.CheckDataList() == false)
            {
                toolStripStatusLabel6.Text = "Reciece Error!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
            }
            else if (rx.buff[1] != 0)
            {
                toolStripStatusLabel6.Text = "擦除错误!!";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
            }
            else
            {
                toolStripStatusLabel6.Text = "擦除成功";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
                progressBar1.Value = 100;
                MessageBox.Show("擦除成功!!");
            }
            this.Enabled = true;
            progressBar1.Visible = false;
        }
        private void btnProgram_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                MessageBox.Show("请连接设备");
                return;
            }     
            if (LoadBuf==null)
            {
                toolStripButton1_Click(null, null);
                return;
            }
            try
            {
                this.Enabled = false;                
                toolStripStatusLabel6.Text = "烧写中......";
                toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
                WFGlobal.WaitMS(5);
                if (HIDDevice.TheHIDDevice.Check_mima() == false)
                {
                    MessageBox.Show("Authorization information error!!", "Error");
                    return;
                }
                progressBar1.Visible = true;
                progressBar1.Value = 0;
                InteractionData tx = new InteractionData();
                InteractionData rx;
                tx.buff[0] = 0x01;
                tx.buff[1] = 0x55;	//主机-->设备
                //             tx.buff[8] = HIDDevice.SwVer[1];//L
                //             tx.buff[9] = HIDDevice.SwVer[0];//H
                int k = 0;
                byte Length = 0xA0;
                byte tmpLeng;
                while (Length != 0x00)
                {
                    if (Length >= 0x30)
                    {
                        tmpLeng = 0x30;
                        tx.buff[4] = 0x30;//本次传送数据的长度；
                        Length -= 0x30;
                    }
                    else
                    {
                        tmpLeng = Length;
                        tx.buff[4] = Length;//本次传送数据的长度；
                        Length = 0x00;
                    }

                    for (int i = 0; i < tmpLeng; i++)
                    {
                        tx.buff[i + 0x10] = LoadBuf[k++];
                    }
                    rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
                    if (rx == null)
                    {
                        toolStripStatusLabel6.Text = "USB Error!!";
                        toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                        return;
                    }
                    else if (rx.CheckDataList() == false)
                    {
                        toolStripStatusLabel6.Text = "Reciece Error!!";
                        toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                        return;
                    }
                    else
                    {
                        tx.buff[5] += tmpLeng;
                        progressBar1.Value += 20;
                        WFGlobal.WaitMS(5);
                    }

                }
                tx = new InteractionData();
                if (radioButton1.Checked)
                    tx.buff[0] = 0x06;
//                 else if (radioButton2.Checked)
//                     tx.buff[0] = 0x07;
                else if (radioButton3.Checked)
                    tx.buff[0] = 0x11;
                else
                {
                    toolStripStatusLabel6.Text = "No Options!!";
                    toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                    return;
                }
                tx.buff[1] = 0x55;	//主机-->设备
                //             tx.buff[8] = HIDDevice.SwVer[1];//L
                //             tx.buff[9] = HIDDevice.SwVer[0];//H
                rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
                if (rx == null)
                {
                    toolStripStatusLabel6.Text = "USB Error!!";
                    toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                }
                else if (rx.CheckDataList() == false)
                {
                    toolStripStatusLabel6.Text = "Reciece Error!!";
                    toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                }
                else if (rx.buff[1] != 0)
                {
                    toolStripStatusLabel6.Text = "烧写错误!!";
                    toolStripStatusLabel6.ForeColor = System.Drawing.Color.Red;
                    progressBar1.Value = 100;
                    MessageBox.Show("Program Error!!");
                }
                else
                {
                    toolStripStatusLabel6.Text = "烧写成功!!";
                    toolStripStatusLabel6.ForeColor = System.Drawing.Color.Green;
                    progressBar1.Value = 100;
                    MessageBox.Show("烧写成功!!");
                }
            }
            finally
            {
                progressBar1.Visible = false;
                this.Enabled = true;  
            }
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                MessageBox.Show("请连接设备");
                return;
            }
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
            Form4 f = new Form4();
            f.ShowDialog();
            Form2_Load(null, null);
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                MessageBox.Show("请连接设备");
                return;
            }
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
            BE_ToolForm f = new BE_ToolForm();
            f.ShowDialog();
            Form2_Load(null, null);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Renew();   
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {

        }
        void ReHIDUI()
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                HIDDevice.TheHIDDevice = HIDDevice.FindDevice();
            }
            HIDUI();
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            ReHIDUI();
        }
    }
}
