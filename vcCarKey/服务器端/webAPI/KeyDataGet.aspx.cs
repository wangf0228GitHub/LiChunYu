using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class KeyDataGet : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string queueID, keyIndex, format;
            string ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
            try
            {
                queueID = Request.QueryString["queueID"];
                keyIndex = Request.QueryString["keyIndex"];
                format = Request.QueryString["format"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(queueID) || string.IsNullOrEmpty(keyIndex) || string.IsNullOrEmpty(format))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                string bin = KeyDataWorkLogDataDBOption.GetFile(queueID, keyIndex, format);
                if (bin == null || bin=="")
                {
                    ret = webAPIFunc.GetRetString(ErrType.NoHexBin);
                    Response.Write(ret);
                    return;
                }
                else
                {
                    ret = "1,"+bin;
                    Response.Write(ret);
                    return;
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Get_Unkown:" + ex.Message, Global.txtLogFolder + "KeyData.txt", true);
            }
            Response.Write(ret);
        }
    }
}