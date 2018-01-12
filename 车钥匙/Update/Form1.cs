using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib;
using System.Security.Cryptography;
using System.Net;
using System.Runtime.Serialization.Json;
using System.IO;
using WFNetLib.USB.HID;
using System.Threading;

namespace Update
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        public static string args;
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
        private void Form1_Load(object sender, EventArgs e)
        {
            //MessageBox.Show(Form1.args);
            label1.Text = "Detecting device............";
            //m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            //ReHIDUI();
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
                this.Text = "Update MKT  ---------" + WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
                timer2.Enabled = false;
                if(Form1.args=="")
                {
                    licupdate();
                    WFNetLib.WFGlobal.WaitMS(5000);
                }
                softupdate();
                WFNetLib.WFGlobal.WaitMS(5000);
                if (bsoftupdate)
                {
                    ProcessHelper.StartProcess("Mercedes Key Tool.exe");                    
                }
                this.Close();
            }
            else
            {
                this.Text = "Update MKT";
                timer2.Enabled = true;
            }
        }
        void licupdate()
        {
            
            label1.Text = "connecting to server for lic update";
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
                    label1.Text = "Updating for lic to " + calldata.to_version;
                    progressBar1.Value = 0;
                    System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                    float percent = 0;
                    try
                    {
                        string URL = calldata.fileurl;
                        string filename = "Mercedes Key Tool.lic";
                        System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(URL);
                        System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
                        long totalBytes = myrp.ContentLength;
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
                            
                            osize = st.Read(by, 0, (int)by.Length);

                            percent = (float)totalDownloadedByte / (float)totalBytes * 100;
                            label2.Text = "Download " + percent.ToString("f2") + "%";
                            progressBar1.Value = (int)percent;
                            System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                        }
                        label2.Text = "Download completed";
                        svd.SetVerifyData_LicUpdateTime(licdt);
                        so.Close();
                        st.Close();
                        WFNetLib.WFGlobal.WaitMS(3000);
                    }
                    catch (System.Exception ex)
                    {
                        label1.Text = "Update for lic is error :" + ex.Message;
                        System.Windows.Forms.Application.DoEvents();
                    }
                }
            }
            else
            {
                label1.Text = "This is the latest lic version---" + calldata.to_version;
                //svd.SetVerifyData_LicUpdateTime(DateTime.Now);
            }
        }
        bool bsoftupdate = false;
        void softupdate()
        {
            label1.Text = "connecting to server for soft update";
            System.Windows.Forms.Application.DoEvents();
            WFHttpWebResponse WebResponse = new WFHttpWebResponse();
            string url;
            url = "http://qc.iehlj.com/index.php?m=content&a=softupdate&device=";
            url += WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
            url += "&version=V0";
            //url += svd.SoftVersion.ToString("f2");
            HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
            DataContractJsonSerializer s = new DataContractJsonSerializer(typeof(JsonSoftData));
            MemoryStream stream = new MemoryStream(System.Text.Encoding.UTF8.GetBytes(WebResponse.Content));
            JsonSoftData calldata = (JsonSoftData)s.ReadObject(stream);
            if (calldata.status == 0)//有更新
            {
                //if(MessageBox.Show("是否更新到最新版本"+calldata.to_version),"是否更新")
                float sersoft = float.Parse(calldata.to_version.Substring(1));
                if (sersoft > svd.SoftVersion || !File.Exists("Mercedes Key Tool.exe"))
                {
                    if (ProcessHelper.IsRunningProcess("Mercedes Key Tool"))
                    {
                        //                     if (MessageBox.Show("是否更新到最新版本" + calldata.to_version, "是否更新", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
                        //                     {
                        //                         
                        //                     }
                        ProcessHelper.CloseProcess("Mercedes Key Tool");
                    }
                    label1.Text = "Soft updating to " + calldata.to_version;
                    System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                    float percent = 0;
                    try
                    {
                        //ProgressBar prog = progressBar1;
                        string URL = calldata.fileurl;
                        string filename = "Mercedes Key Tool.exe";
                        System.Net.HttpWebRequest Myrq = (System.Net.HttpWebRequest)System.Net.HttpWebRequest.Create(URL);
                        System.Net.HttpWebResponse myrp = (System.Net.HttpWebResponse)Myrq.GetResponse();
                        long totalBytes = myrp.ContentLength;

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
                            osize = st.Read(by, 0, (int)by.Length);
                            percent = (float)totalDownloadedByte / (float)totalBytes * 100;
                            progressBar1.Value = (int)percent;
                            label2.Text = "Download " + percent.ToString("f2") + "%";
                            System.Windows.Forms.Application.DoEvents(); //必须加注这句代码，否则label1将因为循环执行太快而来不及显示信息
                        }
                        progressBar1.Value = 100;
                        label2.Text = "Download completed";
                        System.Windows.Forms.Application.DoEvents();
                        WFNetLib.WFGlobal.WaitMS(2000);
                        svd.SetVerifyData_SoftUpdateTime(DateTime.Now);
                        so.Close();
                        st.Close();
                        bsoftupdate = true;
                    }
                    catch (System.Exception)
                    {
                        throw;
                        //break;
                    }
                }
                else
                {
                    label1.Text = "This is the latest soft version---V" + svd.SoftVersion.ToString("f2");
                    svd.SetVerifyData_SoftUpdateTime(DateTime.Now);
                }
            }
            else
            {
                label1.Text = "This is the latest soft version---V" + svd.SoftVersion.ToString("f2");
                svd.SetVerifyData_SoftUpdateTime(DateTime.Now);
            }
        }
        private void timer2_Tick(object sender, EventArgs e)
        {
            timer2.Enabled = false;
            ReHIDUI();
        }
        SoftVerifyData svd = null;
        private void Form1_Shown(object sender, EventArgs e)
        {
            //licupdate();            
            try
            {
                svd = new SoftVerifyData();
                svd.GetVerifyData();
            }
            catch (CryptographicException ex)//数据被更改
            {
                MessageBox.Show("fatal error:" + ex.Message);
                this.Close();
            }
            ReHIDUI();
        }
    }    
}
