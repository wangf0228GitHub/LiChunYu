using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using WFNetLib;
using WFNetLib.StringFunc;
using DAL;
using System.Net;
using System.Text;
using System.IO;
using System.Configuration;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class HPassAdd : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
            string LinuxServer = ConfigurationManager.AppSettings["LinuxServer"];
            try
            {                
                string post = WFHttpWebResponse.PostInput(Request.InputStream);
                if (string.IsNullOrEmpty(post))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                post = post.Substring(0, post.Length - 1);
                byte[] rx = StringsFunction.strToHexByte(post, "");
                byte[] DeviceID = new byte[4];
                byte[] SSID = new byte[4];
                byte[] data=null;
                if (rx.Length == (0x401 * 0x08))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        DeviceID[i] = rx[i];
                        SSID[i] = rx[4 + i];
                    }
                    data = new byte[(0x400 * 0x08)];
                    for (int i = 0; i < (0x400 * 0x08); i++)
                    {
                        data[i] = rx[8 + i];
                    }
                }
                else if (rx.Length == (0x801 * 0x08))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        DeviceID[i] = rx[i];
                        SSID[i] = rx[4 + i];
                    }
                    data = new byte[(0x800 * 0x08)];
                    for (int i = 0; i < (0x800 * 0x08); i++)
                    {
                        data[i] = rx[8 + i];
                    }
                }
                else
                {
                    webAPIFunc.GetRetString(ErrType.ErrFileLen);
                    Response.Write(ret);
                    return;
                }
                byte[] txData = GlobalFunc.Encrypt(data, DeviceID, SSID);
                string strDeviceID = StringsFunction.byteToHexStr(DeviceID, "");
                string strSSID = StringsFunction.byteToHexStr(SSID, "");
                DeviceInfoData did = DeviceInfoDataDBOption.Get(strDeviceID);
                if (did == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.NoRegDevice);
                    Response.Write(ret);
                    return;
                }
                else
                {
                    int ut = HPassWorkLogDataDBOption.GetUseTimes(strDeviceID);
                    if (ut > did.HPassTimes)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.MaxUseTimes);
                        Response.Write(ret);
                        return;
                    }
                }
                int index = HPassWorkLogDataDBOption.GetIndex(strDeviceID,strSSID);
                index++;
                string fileName = strDeviceID + strSSID + "_" + index.ToString()+".txt";
                WFHttpWebResponse web = new WFHttpWebResponse();
                HttpWebResponse webRet = web.CreateGetHttpResponse(LinuxServer);
                if(webRet==null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissServer);
                    Response.Write(ret);
                    return;
                }
                webRet=web.CreatePostHttpResponse(LinuxServer+@"/login", "username=root&password=root");
                if(webRet==null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissServer);
                    Response.Write(ret);
                    return;
                }
                webRet=web.CreateGetHttpResponse(LinuxServer+@"/search_m1_user");
                if(webRet==null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissServer);
                    Response.Write(ret);
                    return;
                }
                HttpWebRequest request = null;
                request = WebRequest.Create(LinuxServer+@"/m1_user_search_add") as HttpWebRequest;
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
                sb.Append("Content-Disposition: form-data; name=\"search_source\"; filename=\"" + fileName + "\"");
                sb.Append("\r\n");
                sb.Append("Content-Type: text/plain");
                sb.Append("\r\n\r\n");
                string head = sb.ToString();
                byte[] form_data = Encoding.ASCII.GetBytes(head);
                //结尾
                byte[] foot_data = Encoding.ASCII.GetBytes("\r\n-----------------------------" + boundary + "--\r\n");
                //数据
                StringBuilder sb1 = new StringBuilder();
                if (rx.Length == (0x401 * 0x08))
                {
                    for (int i = 0; i < 0x400; i++)
                    {
                        sb1.Append(StringsFunction.byteToHexStr(txData, i * 8, 8, " "));
                        sb1.Append("\r\n");
                    }
                }
                else
                {
                    for (int i = 0; i < 0x800; i++)
                    {
                        sb1.Append(StringsFunction.byteToHexStr(txData, i * 8, 8, " "));
                        sb1.Append("\r\n");
                    }
                }
                sb1.Remove(sb1.Length - 2, 2);
                TextLog.AddTextLog(sb1.ToString(), Global.txtLogFolder +"HPassLog\\"+ fileName, false);
                byte[] pBuf = Encoding.ASCII.GetBytes(sb1.ToString());
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
                try
                {
                    HttpWebResponse fileResponse = request.GetResponse() as HttpWebResponse;
                    string Content;
                    if (fileResponse.Headers["ContentEncoding"] != null)
                    {
                        Stream receiveStream = fileResponse.GetResponseStream();
                        StreamReader sr = new StreamReader(receiveStream, Encoding.GetEncoding(fileResponse.Headers["ContentEncoding"].ToString()));
                        Content = sr.ReadToEnd();
                    }
                    else
                    {
                        try
                        {
                            Stream receiveStream = fileResponse.GetResponseStream();
                            StreamReader sr = new StreamReader(receiveStream);
                            Content = sr.ReadToEnd();
                        }
                        catch
                        {

                        }
                    }
                    if (fileResponse.StatusCode == HttpStatusCode.OK)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.retOK,fileName);
                        HPassWorkLogData hl = new HPassWorkLogData();
                        hl.DeviceID = strDeviceID;
                        hl.SSID = strSSID;
                        hl.FileName = fileName;
                        hl.IP = aspNetFunc.getIp();
                        HPassWorkLogDataDBOption.Insert(hl);
                        Response.Write(ret);
                        return;
                    }
                }
                catch (System.Exception ex)
                {
                    ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                    Response.Write(ret);
                    TextLog.AddTextLog("Add_updatafile:" + ex.Message, Global.txtLogFolder + "HPass.txt", true);
                    return;             
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Add_Unkown:" + ex.Message, Global.txtLogFolder + "HPass.txt", true);
            }
            Response.Write(ret);
        }
    }
}