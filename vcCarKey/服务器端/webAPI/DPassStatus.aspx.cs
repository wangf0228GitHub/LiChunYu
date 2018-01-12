using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.Configuration;
using WFNetLib;
using System.Net;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class DPassStatus : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string DeviceID, serverid, SSID;
            string ret="";
            try
            {
                DeviceID = Request.QueryString["DeviceID"];
                serverid = Request.QueryString["serverid"];
                SSID = Request.QueryString["SSID"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(DeviceID) || string.IsNullOrEmpty(serverid) || string.IsNullOrEmpty(SSID))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                string LinuxServer = ConfigurationManager.AppSettings["LinuxServer"];
                Linux_101 WebResponse = new Linux_101();
                string xml = string.Format("<?xml version=\"1.0\"?><query><client>{0}</client><id>{1}</id></query>", DeviceID, serverid);
                int retry = 10;
                string status;
                string progress;
                int x;
                while (retry != 0)
                {
                    HttpWebResponse response = WebResponse.CreatePostHttpResponse(LinuxServer + "/cgi-device/device.cgi?command=status101", xml);
                    if (response == null)
                    {
                        //textBox1.AppendText("连接服务器失败\r\n");
                        retry--;
                        continue;
                    }
                    x = WebResponse.Content.IndexOf("no such request");
                    if (x != -1)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.ErrServerRet);
                        Response.Write(ret);
                        return;
                    }
                    x = WebResponse.Content.IndexOf("<status>");
                    if (x == -1)
                    {
                        //textBox1.AppendText("未查询到status数据\r\n");  
                        retry--;
                        continue;
                    }
                    status = WebResponse.Content.Substring(x + "<status>".Length, WebResponse.Content.IndexOf("</status>") - x - "<status>".Length);
                    if (status == "finished")
                    {
                        x = WebResponse.Content.IndexOf("<result>");
                        if (x == -1)
                        {
                            ret = webAPIFunc.GetRetString(ErrType.retOK);
                            Response.Write(ret);
                            return;
                        }
                        else
                        {
                            x = WebResponse.Content.IndexOf("<query>");
                            string query, answer;
                            if (x == -1)
                            {
                                ret = webAPIFunc.GetRetString(ErrType.retOK);
                                Response.Write(ret);
                                return;
                            }
                            else
                            {
                                query = WebResponse.Content.Substring(x + "<query>".Length, WebResponse.Content.IndexOf("</query>") - x - "<query>".Length);
                            }
                            x = WebResponse.Content.IndexOf("<answer>");
                            if (x == -1)
                            {
                                ret = webAPIFunc.GetRetString(ErrType.retOK);
                                Response.Write(ret);
                                return;
                            }
                            else
                            {
                                answer = WebResponse.Content.Substring(x + "<answer>".Length, WebResponse.Content.IndexOf("</answer>") - x - "<answer>".Length);
                            }
                            byte[] ss1 = WFNetLib.StringFunc.StringsFunction.strToHexByte(query, " ");
                            byte[] ss2 = WFNetLib.StringFunc.StringsFunction.strToHexByte(answer, " ");
                            DPassRecodeData dr = new DPassRecodeData();
                            dr.Dealer = WFNetLib.StringFunc.StringsFunction.byteToHexStr(ss1, "");
                            dr.DPass = WFNetLib.StringFunc.StringsFunction.byteToHexStr(ss2, "");
                            DPassRecodeDataDBOption.Insert(dr);
                            DPassWorkLogData dl = new DPassWorkLogData();
                            dl.DeviceID = DeviceID;
                            dl.IP = WFNetLib.aspNetFunc.getIp();
                            dl.Pass = dr.Dealer;
                            dl.Ret = dr.DPass;
                            dl.SSID = SSID;
                            DPassWorkLogDataDBOption.Insert(dl);
                            byte[] dID = WFNetLib.StringFunc.StringsFunction.strToHexByte(DeviceID, "");
                            byte[] sID = WFNetLib.StringFunc.StringsFunction.strToHexByte(SSID, "");
                            byte[] DPass = WFNetLib.StringFunc.StringsFunction.strToHexByte(dr.DPass, "");
                            byte[] DDPass = GlobalFunc.Encrypt(DPass, dID, sID);
                            ret = "1," + dr.Dealer + "-" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(DDPass, "");
                            Response.Write(ret);
                            return;
                        }
                    }
                    else if (status == "rejected")
                    {
                        ret = "4,";
                        Response.Write(ret);
                        return;
                    }
                    else if (status == "queued")
                    {
                        x = WebResponse.Content.IndexOf("<queue>");
                        if (x != -1)
                        {
                            progress = WebResponse.Content.Substring(x + "<queue>".Length, WebResponse.Content.IndexOf("</queue>") - x - "<queue>".Length);
                            ret = "3,"+progress;
                            Response.Write(ret);
                            return;
                        }
                        else
                        {
                            ret = "3,-1";
                            Response.Write(ret);
                            return;
                        }
                    }
                    else if (status == "inprogress")
                    {
                        x = WebResponse.Content.IndexOf("<progress>");
                        if (x != -1)
                        {
                            progress = WebResponse.Content.Substring(x + "<progress>".Length, WebResponse.Content.IndexOf("</progress>") - x - "<progress>".Length);
                            ret = "2," + progress;
                            Response.Write(ret);
                            return;
                        }
                        else
                        {
                            ret = "2,-1";
                            Response.Write(ret);
                            return;
                        }
                    }
                }
                if (retry == 0)
                {
                    ret = "-99,";
                    Response.Write(ret);
                    return;
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Status_Unkown:" + ex.Message, Global.txtLogFolder + "DPass.txt", true);
            }
            Response.Write(ret);
        }
    }
}