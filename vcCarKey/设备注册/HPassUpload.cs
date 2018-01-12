using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using System.Windows.Forms;
using WFNetLib;

namespace 设备注册
{
    public class HPassUpload
    {
        public string POSTfile()
        {
            WFHttpWebResponse web = new WFHttpWebResponse();
           
            web.CreateGetHttpResponse(@"http://chunyulee.vicp.net:44693/");
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
            byte[] form_data = Encoding.UTF8.GetBytes(head);
            //结尾
            byte[] foot_data = Encoding.UTF8.GetBytes("\r\n-----------------------------" + boundary + "–-\r\n");

            //post总长度
            long length = form_data.Length + pBuf.Length + foot_data.Length;
            request.ContentLength = length;
            using (Stream requestStream = request.GetRequestStream())
            {
                requestStream.Write(form_data, 0, form_data.Length);
                //文件内容
                requestStream.Write(pBuf, 0, pBuf.Length);
                //结尾
                requestStream.Write(foot_data, 0, foot_data.Length);
            }  
//             Stream requestStream = req.GetRequestStream();
//             //发送表单参数
//             requestStream.Write(form_data, 0, form_data.Length);
//             //文件内容
//             requestStream.Write(pBuf, 0, pBuf.Length);
//             //结尾
//             requestStream.Write(foot_data, 0, foot_data.Length);
//             requestStream.Close();
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
    }
}
