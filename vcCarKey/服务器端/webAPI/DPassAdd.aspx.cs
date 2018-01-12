using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using WFNetLib;
using System.Configuration;
using System.Net;
using System.Data;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class DPassAdd : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string DeviceID, Dealer,SSID;
            string ret="";
            try
            {
                DeviceID = Request.QueryString["DeviceID"];
                Dealer = Request.QueryString["pass"];
                SSID = Request.QueryString["SSID"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(DeviceID) || string.IsNullOrEmpty(Dealer) || string.IsNullOrEmpty(SSID))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                DeviceInfoData did = DeviceInfoDataDBOption.Get(DeviceID);
                if (did == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.NoRegDevice);
                    Response.Write(ret);
                    return;
                }
                else
                {
                    int ut = DPassWorkLogDataDBOption.GetUseTimes(DeviceID);
                    if (ut > did.DPassTimes)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.MaxUseTimes);
                        Response.Write(ret);
                        return;
                    }
                }
                DataTable dt = DPassRecodeDataDBOption.DataTableSelect(Dealer);
                if (dt.Rows.Count != 0)
                {
                    byte[] dID = WFNetLib.StringFunc.StringsFunction.strToHexByte(DeviceID, "");
                    byte[] sID = WFNetLib.StringFunc.StringsFunction.strToHexByte(SSID, "");
                    byte[] DPass = WFNetLib.StringFunc.StringsFunction.strToHexByte(dt.Rows[0]["DPass"].ToString(), "");
                    byte[] DDPass = GlobalFunc.Encrypt(DPass, dID, sID);
                    DPassWorkLogData dl = new DPassWorkLogData();
                    dl.DeviceID = DeviceID;
                    dl.IP = WFNetLib.aspNetFunc.getIp();
                    dl.Pass = dt.Rows[0]["Dealer"].ToString();
                    dl.Ret = dt.Rows[0]["DPass"].ToString();
                    dl.SSID = SSID;
                    DPassWorkLogDataDBOption.Insert(dl);
                    ret = "2," + dt.Rows[0]["Dealer"] + "-" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(DDPass, "");
                    Response.Write(ret);
                    return;
                }
                else
                {
                    string LinuxServer = ConfigurationManager.AppSettings["LinuxServer"];
                    Linux_101 WebResponse = new Linux_101();
                    byte[] bD = WFNetLib.StringFunc.StringsFunction.strToHexByte(Dealer, "");
                    string Dealer1 = WFNetLib.StringFunc.StringsFunction.byteToHexStr(bD, " ");
                    string xml = string.Format("<?xml version=\"1.0\"?><query><client>{0}</client><data>{1}</data></query>", DeviceID, Dealer1);
                    HttpWebResponse response = WebResponse.CreatePostHttpResponse(LinuxServer + "/cgi-device/device.cgi?command=search101", xml);
                    if (response == null)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.MissServer);
                        Response.Write(ret);
                        return;
                    }
                    string id = "";
                    int x;
                    x = WebResponse.Content.IndexOf("<id>");
                    if (x == -1)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.ErrServerRet);
                        Response.Write(ret);
                        return;
                    }
                    id = WebResponse.Content.Substring(x + "<id>".Length, WebResponse.Content.IndexOf("</id>") - x - "<id>".Length);
                    ret = webAPIFunc.GetRetString(ErrType.retOK,id);                
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Add_Unkown:" + ex.Message, Global.txtLogFolder + "DPass.txt", true);
            }
            Response.Write(ret);
        }
    }
}