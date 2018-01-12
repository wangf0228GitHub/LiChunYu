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
using WFNetLib;
using System.Diagnostics;
using System.Security.Cryptography;
using System.Net;
using System.Runtime.Serialization.Json;
using System.Threading;
using WFNetLib.Strings.CryptoService;

namespace 车钥匙
{
    public partial class Form1 : Form
    {        
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
            this.Hide();
            Form2 f = new Form2();
            f.ShowDialog();            
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            ReHIDUI();
            this.Show();
        }
        void ReHIDUI()
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                HIDDevice.TheHIDDevice = HIDDevice.FindDevice(); 
            }
            HIDUI();
        }
        void HIDUI()
        {
            if (HIDDevice.TheHIDDevice != null)
            {                
                toolStripStatusLabel2.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
                timer1.Enabled = false;
//                 if (!bUPdate)
//                 {
//                     bUPdate = true;
//                     try
//                     {
//                         WFHttpWebResponse WebResponse = new WFHttpWebResponse();
//                         string url;
//                         url = "http://www.baidu.com";
//                         HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
//                     }
//                     catch
//                     {
//                         return;
//                     }
//                     bUpOK1 = false;
//                     bUpOK2 = false;
//                     bUpOK3 = false;
//                     bMessge = false;
//                     try
//                     {
//                         checkForSoftUpdatesToolStripMenuItem2_Click(null, null);
//                         if (bClose)
//                             return;
//                         //bUpOK1 = true;
//                         checkForDeviceUpdatesToolStripMenuItem_Click(null, null);
//                         //bUpOK2 = true;
//                         checkForDeviceUpdatesToolStripMenuItem1_Click(null, null);
//                     }
//                     catch { }
//                     bMessge = true;
//                     //if (bUpOK1 && bUpOK2 && bUpOK3)
// 
//                 }
                ushort hexver= BytesOP.MakeShort(HIDDevice.TheHIDDevice.HexVer[0], HIDDevice.TheHIDDevice.HexVer[1]);
                if (hexver < 0x2001)
                {
                    button1.Visible = true;
                    button2.Visible = false;
                    button3.Visible = false;
                }
                else if (hexver < 0x2100)
                {
                    button1.Visible = true;
                    button2.Visible = true;
                    button3.Visible = false;
                }
                else
                {
                    button1.Visible = true;
                    button2.Visible = true;
                    button3.Visible = true;
                }                
            }
            else
            {
                //checkForDeviceUpdatesToolStripMenuItem.Enabled = false;
                toolStripStatusLabel2.Text = "No Device";
                timer1.Enabled = true;
                button1.Visible = false;
                button2.Visible = false;
                button3.Visible = false;
            }
        }
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
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
        IntPtr m_hUsbEventHandle;
        private void Form1_Load(object sender, EventArgs e)
        {            
            //this.Text = "Mercedes Key Tool ---"+HIDDevice.SoftVersion.ToString("f2");
            this.Icon = Properties.Resources.MI;
            progressBar1.Visible = false;
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            toolStripStatusLabel2.Text = "No Device";
            timer1.Enabled = true;
            button1.Visible = false;
            button2.Visible = false;
            button3.Visible = false;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
            this.Hide();
            Form3 f = new Form3();
            f.ShowDialog();
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            ReHIDUI();
            this.Show();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            ReHIDUI();
        }
        void UpdateDevice(byte[] pBuf)
        {
            if (pBuf.Length != 0x7010)
            {
                if(bMessge)
                {
                    MessageBox.Show("The file for device update is error len!!");
                }
                toolStripStatusLabel3.Text = "The file for device update is error len!!";
                return;
            }
            if (pBuf[0x7000] != HIDDevice.TheHIDDevice.HexID[0] ||
                    pBuf[0x7001] != HIDDevice.TheHIDDevice.HexID[1] ||
                    pBuf[0x7002] != HIDDevice.TheHIDDevice.HexID[2] ||
                    pBuf[0x7003] != HIDDevice.TheHIDDevice.HexID[3])
            {
                if (bMessge)
                {
                    MessageBox.Show("The file does not match with the device!!");
                }
                toolStripStatusLabel3.Text = "The file does not match with the device!!";
                return;
            }
            ushort oldVer = BytesOP.MakeShort(HIDDevice.TheHIDDevice.HexVer[0], HIDDevice.TheHIDDevice.HexVer[1]);
            ushort newVer = BytesOP.MakeShort(pBuf[0x7004], pBuf[0x7005]);
            if (newVer < oldVer)
            {
                if (bMessge)
                {
                    MessageBox.Show("File version below equipment version!!!!!");
                }
                toolStripStatusLabel3.Text = "File version below equipment version!!!!!";
                return;
            }
            //ushort crc = Verify.GetVerify_CRC16(pBuf, 0x700e);yte
            byte[] pDtOut = new byte[2];
            byte temp, N, Carry;
            pDtOut[0] = 0x00;		//CRC_HI
            pDtOut[1] = 0x00;		//CRC_LO
            for (int i = 0; i < 0x700E; i++)
            {
                temp = pBuf[i];
                for (int j = 0; j < 8; j++)
                {
                    N = (byte)(0x80 & temp);
                    pDtOut[0] = (byte)(pDtOut[0] ^ N);
                    if ((pDtOut[0] & 0x80) == 0x80)
                    {
                        pDtOut[0] = (byte)(pDtOut[0] ^ 0x08);
                        pDtOut[1] = (byte)(pDtOut[1] ^ 0x10);
                        Carry = 0x80;
                    }
                    else
                    {
                        Carry = 0x00;
                    }
                    pDtOut[0] = (byte)(pDtOut[0] << 1);
                    if ((pDtOut[1] & 0x80) == 0x80)
                    {
                        pDtOut[0] = (byte)(pDtOut[0] | 0x01);
                    }
                    else
                    {
                        pDtOut[0] = (byte)(pDtOut[0] & 0xFE);
                    }

                    pDtOut[1] = (byte)(pDtOut[1] << 1);
                    if ((Carry & 0x80) == 0x80)
                    {
                        pDtOut[1] = (byte)(pDtOut[1] | 0x01);
                    }
                    else
                    {
                        pDtOut[1] = (byte)(pDtOut[1] & 0xFE);
                    }
                    temp = (byte)(temp << 1);
                }
            }
            ushort crc = BytesOP.MakeShort(pDtOut[0], pDtOut[1]);
            ushort filecrc = BytesOP.MakeShort(pBuf[0x700e], pBuf[0x700f]);
            if (crc != filecrc)
            {
                if (bMessge)
                {
                    MessageBox.Show("Calibration error file!!!!!");
                }
                toolStripStatusLabel3.Text = "Calibration error file!!!!!";
                return;
            }
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
//                 if (bMessge)
//                 {
//                     MessageBox.Show("Authorization information error!!", "Error");
//                 }
//                 toolStripStatusLabel3.Text = "Authorization information error!!";
//                 return;
            }
            if (bMessge)
            {
                MessageBox.Show("Device will begin to upgrade!!", "Waring", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            this.Enabled = false;
            progressBar1.Visible = true;
            progressBar1.Value = 0;
            try
            {
                toolStripStatusLabel3.Text = "Device Updating......";
                WFGlobal.WaitMS(20);
                int k = 0;
                int Length = 0x7000;//
                InteractionData tx = new InteractionData();
                tx.buff[0] = 0x01;
                tx.buff[1] = 0x55;	//主机-->设备
                tx.buff[4] = 0x20;	//本次传送数据的长度；
                byte JL = 0;
                InteractionData rx;
                byte Ads_U = 0x00;
                byte Ads_H = 0x10;//0x60
                byte Ads_L = 0x00;
                while (Length != 0x00)
                {
                    Length -= 0x20;
                    if (JL == 0)
                    {
                        tx.buff[5] = 0x00;	//操作的地址，L,送数据时仅用一个地址字节
                    }
                    else
                    {
                        tx.buff[5] = 0x20;	//操作的地址，L,送数据时仅用一个地址字节
                    }

                    for (int i = 0; i < 0x20; i++)
                    {
                        tx.buff[i + 0x10] = pBuf[k++];
                    }
                    rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
                    if (rx == null)
                    {
                        
                        toolStripStatusLabel3.Text = "USB Error!!";
                        MessageBox.Show("USB Error!!");
                        return;
                    }
                    else if (rx.CheckDataList() == false)
                    {
                        toolStripStatusLabel3.Text = "USB Reciece Error!!";
                        MessageBox.Show("USB Reciece Error!!");
                        return;
                    }
                    //每传送两串数据，发一次写命令，设备把数据写入内部FLASH
                    JL++;
                    if (2 == JL)
                    {
                        JL = 0;
                        //--data cal
                        InteractionData tx1 = new InteractionData();
                        tx1.buff[0] = 0x02;
                        rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx1);
                        if (rx == null)
                        {
                            toolStripStatusLabel3.Text = "USB Error!!";
                            MessageBox.Show("USB Error!!");
                            return;
                        }
                        else if (rx.CheckDataList() == false)
                        {
                            toolStripStatusLabel3.Text = "Reciece Error!!";
                            MessageBox.Show("USB Reciece Error!!");
                            return;
                        }
                        //--
                        tx1.buff[0] = 0x05;
                        tx1.buff[1] = 0x55;	//主机-->设备                            
                        tx1.buff[5] = Ads_L;
                        tx1.buff[6] = Ads_H;
                        tx1.buff[7] = Ads_U;
                        rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx1);
                        if (rx == null)
                        {
                            toolStripStatusLabel3.Text = "USB Error!!";
                            MessageBox.Show("USB Error!!");
                            return;
                        }
                        else if (rx.CheckDataList() == false)
                        {
                            toolStripStatusLabel3.Text = "Reciece Error!!";
                            MessageBox.Show("USB Reciece Error!!");
                            return;
                        }
                        Ads_L += 0x40;
                        if (0x00 == Ads_L)
                        {
                            Ads_H++;
                        }
                    }
                    progressBar1.Value = (0x7000 - Length) * 98 / 0x7000;
                    WFGlobal.WaitMS(10);
                }
                tx = new InteractionData();
                tx.buff[0] = 0x0f;
                rx = HIDDevice.TheHIDDevice.DeviceInteraction(tx);
                if (rx == null)
                {
                    toolStripStatusLabel3.Text = "USB Error!!";
                    MessageBox.Show("USB Error!!");
                    return;
                }
                else if (rx.CheckDataList() == false)
                {
                    toolStripStatusLabel3.Text = "Reciece Error!!";
                    MessageBox.Show("USB Reciece Error!!");
                    return;
                }
                if(HIDDevice.TheHIDDevice.CheckDevice())
                {
                    progressBar1.Value = 100;
                    toolStripStatusLabel3.Text = "Device Update Finish";
                    WFGlobal.WaitMS(100);
                    svd.SetVerifyData_BINUpdateTime(DateTime.Now);
                    if (bMessge)
                    {
                        MessageBox.Show("Device Update Finish");
                    }
                }
                else
                {
                    MessageBox.Show("Device Update Error!!");
                    return;
                }
            }
            finally
            {
                this.Enabled = true;
                progressBar1.Visible = false;
                progressBar1.Value = 0;
            }
        }
        private void useBINFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FileInfo f;
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            byte[] pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            UpdateDevice(pBuf);
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutForm f = new AboutForm();
            f.ShowDialog();
        }

        private void checkForSoftUpdatesToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                toolStripStatusLabel3.Text = "No Device,Cann't Update";
                MessageBox.Show("No Device,Cann't Update,Please insert the Device!!!");
                return;
            }
            toolStripStatusLabel3.Text = "connecting to server for soft update";
            System.Windows.Forms.Application.DoEvents();
            WFHttpWebResponse WebResponse = new WFHttpWebResponse();
            string url;
            url = "http://qc.iehlj.com/index.php?m=content&a=softupdate&device=";
            url += WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
            url += "&version=V0";
            //url += HIDDevice.SoftVersion.ToString("f2");
            HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
            DataContractJsonSerializer s = new DataContractJsonSerializer(typeof(JsonSoftData));
            MemoryStream stream = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(WebResponse.Content));
            JsonSoftData calldata = (JsonSoftData)s.ReadObject(stream);
            if (calldata.status == 0)//有更新
            {
                float sersoft = float.Parse(calldata.to_version.Substring(1));
                if(sersoft>svd.SoftVersion)
                {
                    try
                    {
                        ProcessHelper.StartProcess("Update MKT.exe");
                        this.Close();
                        bClose = true;
                    }
                    catch// (System.Exception ex)
                    {
                        toolStripStatusLabel3.Text = "cannot find 'Update MKT.exe'";
                        if (bMessge)
                        {
                            MessageBox.Show("cannot find 'Update MKT.exe'!!!");
                        }
                    }
                }
                else
                {
                    svd.SetVerifyData_SoftUpdateTime(DateTime.Now);
                    toolStripStatusLabel3.Text = "This is the latest soft version";
                    if (bMessge)
                    {
                        MessageBox.Show("This is the latest soft version!!");
                    }
                }
            }
            else
            {
                svd.SetVerifyData_SoftUpdateTime(DateTime.Now);
                toolStripStatusLabel3.Text = "This is the latest soft version";
                if (bMessge)
                {
                    MessageBox.Show("This is the latest soft version!!");
                }
            }
        }
        SoftVerifyData svd = null;
        bool bUpOK1 = false;
        bool bUpOK2 = false;
        bool bUpOK3 = false;
        bool bClose = false;
        bool bMessge = false;
        bool bUPdate = false;
        private void Form1_Shown(object sender, EventArgs e)
        {
//             File.Delete("Mercedes Key Tool Update.exe");
// /*            MessageBox.Show("1");*/
//             try
//             {
//                 svd = new SoftVerifyData();
//                 svd.GetVerifyData();
//                 if (svd.SoftVersion != HIDDevice.SoftVersion)
//                 {
//                     svd.Make_mkt();
//                     File.Delete("Mercedes Key Tool Update.exe");
//                 }
//             }
//             catch (CryptographicException ex)//数据被更改
//             {
//                 MessageBox.Show("系统关键数据被更改:" + ex.Message);
//                 this.Close();
//             }
//             this.Enabled = false;
//             WFGlobal.WaitMS(1000);
//             this.Enabled = true;
//             ReHIDUI();
//             while (true)
//             {
//                 bUpOK1 = false;
//                 bUpOK2 = false;
//                 bUpOK3 = false;
//                 bMessge = false;
//                 try
//                 {
//                     svd.GetVerifyData();
//                     bUpOK1 = false;
//                     if (svd.SoftUpdateTime > DateTime.Now || svd.SoftUpdateTime.AddDays(7) < DateTime.Now)
//                     {
//                         try
//                         {
//                             WFHttpWebResponse WebResponse = new WFHttpWebResponse();
//                             string url;
//                             url = "http://www.baidu.com";
//                             HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
//                             //软件更新
//                             checkForSoftUpdatesToolStripMenuItem2_Click(null, null);
//                             if (bClose)
//                                 return;
//                         }
//                         catch { }
//                     }
//                     else
//                         bUpOK1 = true;
//                     if (svd.BINUpdateTime > DateTime.Now || svd.BINUpdateTime.AddDays(7) < DateTime.Now)
//                     {
//                         try
//                         {
//                             WFHttpWebResponse WebResponse = new WFHttpWebResponse();
//                             string url;
//                             url = "http://www.baidu.com";
//                             HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
//                             //软件更新
//                             checkForDeviceUpdatesToolStripMenuItem_Click(null, null);
//                         }
//                         catch { }
//                     }
//                     else
//                         bUpOK2 = true;
//                     if (svd.LicUpdateTime > DateTime.Now || svd.LicUpdateTime.AddDays(7) < DateTime.Now)
//                     {
//                         try
//                         {
//                             WFHttpWebResponse WebResponse = new WFHttpWebResponse();
//                             string url;
//                             url = "http://www.baidu.com";
//                             HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
//                             //软件更新
//                             checkForDeviceUpdatesToolStripMenuItem1_Click(null, null);
//                         }
//                         catch { }
//                     }
//                     else
//                         bUpOK3 = true;
//                     if (bUpOK1 && bUpOK2 && bUpOK3)
//                         break;
//                     else
//                     {
//                         if (HIDDevice.TheHIDDevice == null)
//                             ReHIDUI();
//                     }
//                 }
//                 catch
//                 {
// 
//                 }
//             }
//             bMessge = true;            
        }

        private void checkForDeviceUpdatesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                toolStripStatusLabel3.Text = "No Device,Cann't Update";
                MessageBox.Show("No Device,Cann't Update,Please insert the Device!!!");
                return;
            }
            toolStripStatusLabel3.Text = "connecting to server for device update";
            System.Windows.Forms.Application.DoEvents();
            WFHttpWebResponse WebResponse = new WFHttpWebResponse();
            string url;
            url = "http://qc.iehlj.com/index.php?m=content&a=yingupdate&device=";
            url += WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
            HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
            DataContractJsonSerializer s = new DataContractJsonSerializer(typeof(JsonSoftData));
            MemoryStream stream = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(WebResponse.Content));
            JsonSoftData calldata = (JsonSoftData)s.ReadObject(stream);
            if (calldata.status == 0)//有更新
            {
                string strcurBin = HIDDevice.TheHIDDevice.HexVer[0].ToString("X2") +"."+ HIDDevice.TheHIDDevice.HexVer[1].ToString("X2");
                float serBin = float.Parse(calldata.to_version.Substring(1));
                float curBin = float.Parse(strcurBin);
                if (serBin>curBin)
                {
                    //下载lic
                    toolStripStatusLabel3.Text = "Updating for Device";
                    progressBar1.Value = 0;
                    progressBar1.Visible = true;
                    System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                    float percent = 0;
                    try
                    {
                        string URL = calldata.fileurl;
                        //string filename = "123.lic";
                        System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(URL);
                        System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
                        long totalBytes = myrp.ContentLength;                        
                        byte[] theBIN = new byte[totalBytes];
                        long BINOffset = 0;
                        System.IO.Stream st = myrp.GetResponseStream();
                        //System.IO.Stream so = new System.IO.FileStream("123.bin", System.IO.FileMode.Create);
                        long totalDownloadedByte = 0;
                        byte[] by = new byte[1024];
                        int osize = st.Read(by, 0, (int)by.Length);
                        while (osize > 0)
                        {
                            totalDownloadedByte = osize + totalDownloadedByte;
                            System.Windows.Forms.Application.DoEvents();
                            //so.Write(by, 0, osize);
                            for (int i = 0; i < osize; i++)
                            {
                                theBIN[BINOffset++] = by[i];
                            }
                            osize = st.Read(by, 0, (int)by.Length);
                            percent = (float)totalDownloadedByte / (float)totalBytes * 100;
                            progressBar1.Value = (int)(percent);
                            toolStripStatusLabel3.Text = "Download for device " + percent.ToString("f2") + "%";
                            System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                        }
                        toolStripStatusLabel3.Text = "Download device OK,wait for update device...";
                        System.Windows.Forms.Application.DoEvents();
                        //so.Close();
                        st.Close();
                        WFGlobal.WaitMS(2000);
                        UpdateDevice(theBIN);
                        progressBar1.Visible = false;
                        progressBar1.Value = 0;
                    }
                    catch (System.Exception ex)
                    {
                        if (bMessge)
                        {
                            MessageBox.Show(ex.Message);
                        }
                        toolStripStatusLabel3.Text = "Device Update is error";
                        progressBar1.Visible = false;
                        progressBar1.Value = 0;
                        //throw;
                        //break;
                    }
                }
                else
                {
                    svd.SetVerifyData_BINUpdateTime(DateTime.Now);
                    if (bMessge)
                    {
                        MessageBox.Show("This is the latest Device version");
                    }
                    toolStripStatusLabel3.Text = "This is the latest Device version";
                }
            }
            else
            {
                toolStripStatusLabel3.Text = "This is the latest Device version";
                if (bMessge)
                {                    
                    MessageBox.Show("This is the latest Device version");
                }
                svd.SetVerifyData_BINUpdateTime(DateTime.Now);
            }
        }

        private void checkForDeviceUpdatesToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (HIDDevice.TheHIDDevice == null)
            {
                toolStripStatusLabel3.Text = "No Device,Cann't Update";
                MessageBox.Show("No Device,Cann't Update,Please insert the Device!!!");
                return;
            }
            toolStripStatusLabel3.Text = "connecting to server for lic update";
            System.Windows.Forms.Application.DoEvents();
            WFHttpWebResponse WebResponse = new WFHttpWebResponse();
            string url;
            url = "http://qc.iehlj.com/index.php?m=content&a=shouupdate&device=";
            url += WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
            HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
            DataContractJsonSerializer s = new DataContractJsonSerializer(typeof(JsonSoftData));
            MemoryStream stream = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(WebResponse.Content));
            JsonSoftData calldata = (JsonSoftData)s.ReadObject(stream);
            if (calldata.status == 0)//有更新
            {
                DateTime licdt = DateTime.Parse(calldata.to_version);
                if (licdt.Date > svd.LicUpdateTime.Date || !File.Exists("Mercedes Key Tool.lic"))
                {
                    //下载lic
                    toolStripStatusLabel3.Text = "Updating for Lic";
                    progressBar1.Value = 0;
                    progressBar1.Visible = true;
                    System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                    float percent = 0;
                    try
                    {
                        ProgressBar prog = progressBar1;
                        string URL = calldata.fileurl;
                        string filename = "Mercedes Key Tool.lic";
                        System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(URL);
                        System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
                        long totalBytes = myrp.ContentLength;
                        if (prog != null)
                        {
                            prog.Maximum = (int)totalBytes;
                        }
                        System.IO.Stream st = myrp.GetResponseStream();
                        System.IO.Stream so = new System.IO.FileStream(filename, System.IO.FileMode.Create);
                        long totalDownloadedByte = 0;
                        byte[] by = new byte[1024];
                        int osize = st.Read(by, 0, (int)by.Length);
                        while (osize > 0)
                        {
                            totalDownloadedByte = osize + totalDownloadedByte;
                            System.Windows.Forms.Application.DoEvents();
                            so.Write(by, 0, osize);
                            if (prog != null)
                            {
                                prog.Value = (int)totalDownloadedByte;
                            }
                            osize = st.Read(by, 0, (int)by.Length);

                            percent = (float)totalDownloadedByte / (float)totalBytes * 100;
                            toolStripStatusLabel3.Text = "Downloading for lic " + percent.ToString("f2") + "%";
                            System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                        }
                        toolStripStatusLabel3.Text = "Download Lic OK!";
                        svd.SetVerifyData_LicUpdateTime(licdt);
                        so.Close();
                        st.Close();
                        if (bMessge)
                        {
                            MessageBox.Show("Lic update completed!!");
                        }
                        progressBar1.Visible = false;
                        progressBar1.Value = 0;
                    }
                    catch (System.Exception ex)
                    {
                        if (bMessge)
                        {
                            MessageBox.Show(ex.Message);
                        }
                        toolStripStatusLabel3.Text = "Lic Update is error";
                        progressBar1.Visible = false;
                        progressBar1.Value = 0;
                        //throw;
                        //break;
                    }
                }
                else
                {
                    toolStripStatusLabel3.Text = "This is the latest lic version";
                    //svd.SetVerifyData_LicUpdateTime(licdt);
                    if (bMessge)
                    {
                        MessageBox.Show("This is the latest lic version");
                    }
                }
            }
            else
            {
                toolStripStatusLabel3.Text = "This is the latest lic version";
                //svd.SetVerifyData_LicUpdateTime(DateTime.Now);
                if (bMessge)
                {
                    MessageBox.Show("This is the latest lic version");
                }
            }
        }

        private void testToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {

        }    
    }
    [System.Runtime.Serialization.DataContract]
    public class JsonSoftData
    {
        [System.Runtime.Serialization.DataMember]
        public int status;
        [System.Runtime.Serialization.DataMember]
        public string message;
        [System.Runtime.Serialization.DataMember]
        public string to_version;
        [System.Runtime.Serialization.DataMember]
        public string to_release;
        [System.Runtime.Serialization.DataMember]
        public string fileurl;
        [System.Runtime.Serialization.DataMember]
        public string time;
    }
}
