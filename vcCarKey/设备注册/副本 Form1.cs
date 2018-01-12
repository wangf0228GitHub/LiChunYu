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
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace 设备注册
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        IntPtr m_hUsbEventHandle;
        private void Form1_Load(object sender, EventArgs e)
        {
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
            timer1.Enabled = true;
            Random random = new Random();
            random.NextBytes(pass);
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
                textBox1.Text = "No Device";
                timer1.Enabled = true;
                textBox2.Text = "";
                button1.Enabled = false;
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
                Random random = new Random();
                random.NextBytes(pass);
                WFHttpWebResponse WebResponse = new WFHttpWebResponse();
                string url;
                url = "http://localhost:11890/" + "AdminAPI/RegTE.aspx?ID="+StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID,"");
                url = url + "&HexVer=20.02";
                HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
                string ret = WebResponse.Content;
                if (ret== "ok")
                    MessageBox.Show("注册成功");
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
        byte[] pass = new byte[8];
        string sTime="-1";
        bool bFirst=true;
        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                WFHttpWebResponse WebResponse = new WFHttpWebResponse();
                string url;
                url = "http://localhost:11890/" + "webAPI/ESLPass.aspx?DeviceID=051cdbd9";
                url = url + "&pass=" + StringsFunction.byteToHexStr(pass, "");
                url = url + "&sTime=" + sTime;
                HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
                string ret = WebResponse.Content;
                MessageBox.Show(ret);
                if (ret.IndexOf("sTime=") != -1)
                {
                    sTime = ret.Substring(ret.IndexOf("sTime=") + "sTime=".Length);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            sTime = "-1";
            Random random = new Random();
            random.NextBytes(pass);
        }
        string serverid;
        private void button5_Click(object sender, EventArgs e)
        {
            try
            {
                WFHttpWebResponse WebResponse = new WFHttpWebResponse();
                string url;
                url = "http://localhost:11890/" + "webAPI/DPassAdd.aspx?DeviceID=051cdbd9";
                url = url + "&pass=" + StringsFunction.byteToHexStr(pass, "");
                textBox2.Text = url;
                HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
                string ret = WebResponse.Content;
                MessageBox.Show(ret);
                if (ret.IndexOf("ok,id=") != -1)
                {
                    serverid = ret.Substring("ok,id=".Length);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            try
            {
                WFHttpWebResponse WebResponse = new WFHttpWebResponse();
                string url;
                url = "http://localhost:11890/" + "webAPI/DPassStatus.aspx?DeviceID=0445615C";
                url = url + "&serverid=" + serverid;
                HttpWebResponse hr = WebResponse.CreateGetHttpResponse(url);
                string ret = WebResponse.Content;
                MessageBox.Show(ret);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button7_Click(object sender, EventArgs e)
        {
//             HPassUpload sss=new HPassUpload();
//             sss.
            POSTfile();
        }
        public string POSTfile()
        {
            WFHttpWebResponse web = new WFHttpWebResponse();
            web.CreateGetHttpResponse(@"http://192.168.0.158");
            web.CreatePostHttpResponse(@"http://chunyulee.vicp.net:44693/login", "username=root&password=root");
            web.CreateGetHttpResponse(@"http://chunyulee.vicp.net:44693/search_m1_user");

            HttpWebRequest request = null;
            request = WebRequest.Create(@"http://chunyulee.vicp.net:44693/m1_user_search_add") as HttpWebRequest;

            request.Method = "POST";
            string boundary = DateTime.Now.Ticks.ToString("x");
            FileInfo f;
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return null;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            byte[] pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            //请求
            request.ContentType = "multipart/form-data; boundary=---------------------------" + boundary;

            request.CookieContainer = new CookieContainer();
            if (web.Cookies != null && web.Cookies.Count != 0)
            {
                foreach (Cookie c in web.Cookies)
                {
                    request.CookieContainer.Add(c);
                }
            }

            //组织表单数据
            StringBuilder sb = new StringBuilder();
            sb.Append("-----------------------------" + boundary);
            sb.Append("\r\n");
            sb.Append("Content-Disposition: form-data; name=\"search_source\"; filename=\"" + f.Name + "\"");
            sb.Append("\r\n");
            sb.Append("Content-Type: text/plain");
            sb.Append("\r\n\r\n");
            //             sb.Append(ss);
            //             sb.Append("\r\n");
            //             sb.Append("-----------------------------" + boundary + "--/r/n");            

            string head = sb.ToString();
            byte[] form_data = Encoding.ASCII.GetBytes(head);
            //结尾
            byte[] foot_data = Encoding.ASCII.GetBytes("\r\n-----------------------------" + boundary + "--\r\n");

            //post总长度
            long length = form_data.Length + pBuf.Length + foot_data.Length;
            request.ContentLength = length;
            using (Stream requestStream = request.GetRequestStream())
            {
                textBox3.Clear();
                requestStream.Write(form_data, 0, form_data.Length);
                string str = Encoding.ASCII.GetString(form_data);
                textBox3.AppendText(Encoding.ASCII.GetString(form_data));
                //文件内容
                requestStream.Write(pBuf, 0, pBuf.Length);
                str = Encoding.ASCII.GetString(pBuf);
                textBox3.AppendText(Encoding.ASCII.GetString(pBuf));
                //结尾
                requestStream.Write(foot_data, 0, foot_data.Length);
                str = Encoding.ASCII.GetString(foot_data);
                textBox3.AppendText(Encoding.ASCII.GetString(foot_data));
            }  
//             Stream requestStream = req.GetRequestStream();
//             //发送表单参数
//             requestStream.Write(form_data, 0, form_data.Length);
//             //文件内容
//             requestStream.Write(pBuf, 0, pBuf.Length);
//             //结尾
//             requestStream.Write(foot_data, 0, foot_data.Length);
//             requestStream.Close();
            try
            {
                HttpWebResponse Response = request.GetResponse() as HttpWebResponse;
                string Content;
                if (Response.Headers["ContentEncoding"] != null)
                {
                    Stream receiveStream = Response.GetResponseStream();
                    StreamReader sr = new StreamReader(receiveStream, Encoding.GetEncoding(Response.Headers["ContentEncoding"].ToString()));
                    Content = sr.ReadToEnd();
                }
                else
                {
                    try
                    {
                        Stream receiveStream = Response.GetResponseStream();
                        StreamReader sr = new StreamReader(receiveStream);
                        Content = sr.ReadToEnd();
                    }
                    catch
                    {

                    }
                }
            }
            catch (System.Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
            while (true)
            {
                web.CreateGetHttpResponse(@"http://chunyulee.vicp.net:44693/m1_user_search_get_status");
            }
// 
//             
//             
// 
//             //响应
//             WebResponse pos = req.GetResponse();
//             StreamReader sr = new StreamReader(pos.GetResponseStream(), Encoding.UTF8);
//             string html = sr.ReadToEnd().Trim();
//             sr.Close();
//             if (pos != null)
//             {
//                 pos.Close();
//                 pos = null;
//             }
//             if (req != null)
//             {
//                 req = null;
//             }
//             return html;
            return null;
        }

        private void button8_Click(object sender, EventArgs e)
        {
            WFHttpWebResponse web = new WFHttpWebResponse();
            web.CreateGetHttpResponse(@"http://192.168.0.158/");
            web.CreatePostHttpResponse(@"http://192.168.0.158/login", "username=root&password=root");
            HttpWebResponse hr = web.CreateGetHttpResponse(@"http://192.168.0.158/m1_user_search_get_status");
            m1_Status[] ret = WFJSON.JsonDeserializeBySingleData<m1_Status[]>(web.Content);
            foreach (m1_Status m1 in ret)
            {
                web.CreateGetHttpResponse(@"http://192.168.0.158/m1_search_user_remove?request_id="+m1.id);
                //http://192.168.0.158/m1_search_user_remove?request_id=484
            }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            byte[] pBuf = new byte[256];
            byte[] key = new byte[8];
            for(int i=0;i<256;i++)
                pBuf[i] = (byte)i;
            for (int i = 0; i < 8; i++)
                key[i] = (byte)i;
            byte[] ddd = DataCrypto.Encrypt(pBuf, key);
            byte[] ddd1 = DataCrypto.Encrypt(ddd, key);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            FileInfo f;
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            byte[] pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();

            WFHttpWebResponse web = new WFHttpWebResponse();
            HttpWebResponse webRet=web.CreateGetHttpResponse(@"http://chunyulee.vicp.net:44693/");
            web.CreatePostHttpResponse(@"http://chunyulee.vicp.net:44693/login", "username=root&password=root");
            int file=30;
            string filename = "wf_Test_";
            while(file!=0)
            {                
                file--;
                web.CreateGetHttpResponse(@"http://chunyulee.vicp.net:44693/search_m1_user");

                HttpWebRequest request = null;
                request = WebRequest.Create(@"http://chunyulee.vicp.net:44693/m1_user_search_add") as HttpWebRequest;

                request.Method = "POST";
                string boundary = DateTime.Now.Ticks.ToString("x");

                //请求
                request.ContentType = "multipart/form-data; boundary=---------------------------" + boundary;

                request.CookieContainer = new CookieContainer();
                if (web.Cookies != null && web.Cookies.Count != 0)
                {
                    foreach (Cookie c in web.Cookies)
                    {
                        request.CookieContainer.Add(c);
                    }
                }

                //组织表单数据
                StringBuilder sb = new StringBuilder();
                sb.Append("-----------------------------" + boundary);
                sb.Append("\r\n");
                sb.Append("Content-Disposition: form-data; name=\"search_source\"; filename=\"" + filename+file.ToString() + ".txt\"");
                sb.Append("\r\n");
                sb.Append("Content-Type: text/plain");
                sb.Append("\r\n\r\n");
                //             sb.Append(ss);
                //             sb.Append("\r\n");
                //             sb.Append("-----------------------------" + boundary + "--/r/n");            

                string head = sb.ToString();
                byte[] form_data = Encoding.ASCII.GetBytes(head);
                //结尾
                byte[] foot_data = Encoding.ASCII.GetBytes("\r\n-----------------------------" + boundary + "--\r\n");

                //post总长度
                long length = form_data.Length + pBuf.Length + foot_data.Length;
                request.ContentLength = length;
                using (Stream requestStream = request.GetRequestStream())
                {
                    textBox3.Clear();
                    requestStream.Write(form_data, 0, form_data.Length);
                    string str = Encoding.ASCII.GetString(form_data);
                    textBox3.AppendText(Encoding.ASCII.GetString(form_data));
                    //文件内容
                    requestStream.Write(pBuf, 0, pBuf.Length);
                    str = Encoding.ASCII.GetString(pBuf);
                    textBox3.AppendText(Encoding.ASCII.GetString(pBuf));
                    //结尾
                    requestStream.Write(foot_data, 0, foot_data.Length);
                    str = Encoding.ASCII.GetString(foot_data);
                    textBox3.AppendText(Encoding.ASCII.GetString(foot_data));
                }
                //             Stream requestStream = req.GetRequestStream();
                //             //发送表单参数
                //             requestStream.Write(form_data, 0, form_data.Length);
                //             //文件内容
                //             requestStream.Write(pBuf, 0, pBuf.Length);
                //             //结尾
                //             requestStream.Write(foot_data, 0, foot_data.Length);
                //             requestStream.Close();
                try
                {
                    HttpWebResponse Response = request.GetResponse() as HttpWebResponse;
                    string Content;
                    if (Response.Headers["ContentEncoding"] != null)
                    {
                        Stream receiveStream = Response.GetResponseStream();
                        StreamReader sr = new StreamReader(receiveStream, Encoding.GetEncoding(Response.Headers["ContentEncoding"].ToString()));
                        Content = sr.ReadToEnd();
                    }
                    else
                    {
                        try
                        {
                            Stream receiveStream = Response.GetResponseStream();
                            StreamReader sr = new StreamReader(receiveStream);
                            Content = sr.ReadToEnd();
                        }
                        catch
                        {

                        }
                    }
                    HttpWebResponse hr = web.CreateGetHttpResponse(@"http://chunyulee.vicp.net:44693/m1_user_search_get_status");
                    //m1_Status[] ret = WFJSON.JsonDeserializeBySingleData<m1_Status[]>(web.Content);
                }
                catch (System.Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                }
                //Thread.Sleep(60000);
            }
            m1_Status[] ret = WFJSON.JsonDeserializeBySingleData<m1_Status[]>(web.Content);
        }
    }
    public class DataCrypto
    {
        public static byte[] Encrypt(byte[] pBuf, byte[] key)
        {
            byte[] ret = new byte[pBuf.Length];
            int c = pBuf.Length / 8;
            for (int i = 0; i < c;i++ )
            {
                for (int j = 0; j < 8; j++)
                {
                    ret[i * 8 + j] = (byte)(key[j]^pBuf[i * 8 + j]);
                }
            }
            return ret;
        }
    }
    public class m1_result
    {
        public string id;
        public string query;
        public string answer;
        public string createdAt;
        public string updatedAt;
        public string request_id;
    }
    public class m1_user_request
    {
        public bool show;
        public string createdAt;
        public string updatedAt;
        public string userID;
        public string requestID;
    }
    public class m1_Status
    {
        public string id;
        public string status;
        public string search_id;
        public int progress;
        public string type;
        public byte[] data;
        public string source_ip;
        public string createdAt;
        public string updatedAt;
        public string dvice_id;
        public m1_user_request user_request;
        public m1_result[] results;
    }
    public class get_m1_status
    {
        public m1_Status[] Status;
    }
}
