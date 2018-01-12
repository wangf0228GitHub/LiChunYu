using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using WFNetLib;
using WFNetLib.USB.HID;
using WFNetLib.StringFunc;
using WFNetLib.Log;

namespace 设备注册
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        IntPtr m_hUsbEventHandle;
        private void Form1_Load(object sender, EventArgs e)
        {
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            timer1.Enabled = true;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            ReHIDUI();
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
                timer1.Enabled = false;
                textBox2.Text = string.Format("{0:X2}.{1:X2}", HIDDevice.TheHIDDevice.HexVer[0], HIDDevice.TheHIDDevice.HexVer[1]);
                textBox1.Text = StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID," ");
                button1.Enabled = true;
            }
            else
            {
                //checkForDeviceUpdatesToolStripMenuItem.Enabled = false;
                //textBox1.Text = "No Device";
                //timer1.Enabled = true;
                //textBox2.Text = "";
                //button1.Enabled = false;
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

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                WFHttpWebResponse WebResponse = new WFHttpWebResponse();
                string url;
                byte[] ss = StringsFunction.strToHexByte(textBox1.Text, " ");
                url = "http://www.mercedes-tool.com:9999/" + "AdminAPI/RegTE.aspx?ID=" + StringsFunction.byteToHexStr(ss, "");
                url = url + "&HexVer=" + textBox2.Text;
                
                HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
                string ret = WebResponse.Content;
                if (ret == "ok")
                {
                    TextLog.AddTextLog(StringsFunction.byteToHexStr(ss, ""), System.Windows.Forms.Application.StartupPath + "\\regLog.txt", false);
                    MessageBox.Show("注册成功");                    
                }
                else if (ret == "reged")
                    MessageBox.Show("已经注册");
                else
                    MessageBox.Show("注册失败:" + ret);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            WFHttpWebResponse web = new WFHttpWebResponse();
            web.CreateGetHttpResponse(@"http://192.168.0.158/");
            web.CreatePostHttpResponse(@"http://192.168.0.158/login", "username=root&password=root");
            HttpWebResponse hr = web.CreateGetHttpResponse(@"http://192.168.0.158/m1_user_search_get_status");
            m1_Status[] ret = WFJSON.JsonDeserializeBySingleData<m1_Status[]>(web.Content);
            foreach (m1_Status m1 in ret)
            {
                web.CreateGetHttpResponse(@"http://192.168.0.158/m1_search_user_remove?request_id=" + m1.id);
                //http://192.168.0.158/m1_search_user_remove?request_id=484
            }
        }
    }
}
