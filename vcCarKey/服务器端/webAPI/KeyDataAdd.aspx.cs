using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using WFNetLib;
using DAL;
using System.Data;
using WFNetLib.StringFunc;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class KeyDataAdd : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
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
                KeyDataQueueData kd=new KeyDataQueueData();
                kd.DeviceID = StringsFunction.byteToHexStr(rx, 0, 4, "");
                kd.KeyOption = BytesOP.MakeShort(rx[4], rx[5]);
                kd.format = rx[6];
                kd.IP = aspNetFunc.getIp();
                kd.SSID =  StringsFunction.byteToHexStr(rx, 7, 4, "");
                kd.BIN = StringsFunction.byteToHexStr(rx, 7, 1024, "");
                if (DeviceInfoDataDBOption.Get(kd.DeviceID) == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.NoRegDevice);
                    Response.Write(ret);
                    return;
                }
                int queueID = KeyDataQueueDataDBOption.Insert(kd);
                if (KeyDataWork.bCalc == false)
                {
                    KeyDataWork.autoResetEvent.Set();
                }
                ret = webAPIFunc.GetRetString(ErrType.retOK, queueID.ToString());
                Response.Write(ret);
                return;                
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Add_Unkown:" + ex.Message, Global.txtLogFolder + "KayData.txt", true);
            }
            Response.Write(ret);
        }
    }
}