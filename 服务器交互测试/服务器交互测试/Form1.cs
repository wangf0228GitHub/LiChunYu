using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;

namespace 服务器交互测试
{
    public partial class Form1 : Form
    {
        WFHttpWebResponse WebResponse;
        public Form1()
        {
            InitializeComponent();
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            textBox1.AppendText("-------------------------"+DateTime.Now.ToString()+"-----------------------------------------\r\n");
            textBox2.AppendText("-------------------------" + DateTime.Now.ToString() + "-----------------------------------------\r\n");
            WebResponse = new WFHttpWebResponse();
            string xml = string.Format("<?xml version=\"1.0\"?><query><client>{0}</client><data>{1}</data></query>",toolStripTextBox3.Text,toolStripTextBox2,Text);
            HttpWebResponse ret = WebResponse.CreatePostHttpResponse("http://"+toolStripTextBox1.Text+"/cgi-device/device.cgi?command=search101", xml);
            if (ret == null)
            {
                textBox1.AppendText("连接服务器失败\r\n");
                return;
            }
            textBox2.AppendText("conneted to server!");
            textBox1.AppendText(WebResponse.Content + "\r\n");
            string id = "";
            int x;
            x = WebResponse.Content.IndexOf("<id>");
            if (x == -1)
            {
                textBox1.AppendText("未查询到id数据\r\n");
                return;
            }
            id = WebResponse.Content.Substring(x + "<id>".Length, WebResponse.Content.IndexOf("</id>") - x - "<id>".Length);
            xml = string.Format("<?xml version=\"1.0\"?><query><client>53475766</client><id>{0}</id></query>",id);
            int retry = 10;
            string status;
            string progress;
            while(retry!=0)
            {
                ret = WebResponse.CreatePostHttpResponse("http://" + toolStripTextBox1.Text + "/cgi-device/device.cgi?command=status101", xml);
                if (ret == null)
                {
                    textBox1.AppendText("连接服务器失败\r\n");
                    retry--;
                    continue;
                }
                textBox1.AppendText(WebResponse.Content+"\r\n");
                x = WebResponse.Content.IndexOf("<status>");
                if (x == -1)
                {
                    textBox1.AppendText("未查询到status数据\r\n");                    
                }
                status = WebResponse.Content.Substring(x + "<status>".Length, WebResponse.Content.IndexOf("</status>") - x - "<status>".Length);
                if (status == "finished" || status == "rejected")
                    break;
                else if (status == "inprogress")
                {
                    x = WebResponse.Content.IndexOf("<progress>");
                    if (x != -1)
                    {
                        progress = WebResponse.Content.Substring(x + "<progress>".Length, WebResponse.Content.IndexOf("</progress>") - x - "<progress>".Length);
                        progressBar1.Value = int.Parse(progress);
                    }
                }

            }
            if(retry!=0)
                textBox1.AppendText("查询完成\r\n");
            else
                textBox1.AppendText("查询失败\r\n");            
        }        
    }
}
