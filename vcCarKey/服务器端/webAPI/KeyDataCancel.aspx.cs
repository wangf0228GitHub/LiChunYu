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
    public partial class KeyDataCancel : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string queueID;
            string ret = "-2000";
            try
            {
                queueID = Request.QueryString["queueID"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(queueID))//没有这两个变量
                {
                    ret = "-1";
                    Response.Write(ret);
                    return;
                }
                KeyDataQueueData kd=new KeyDataQueueData();
                kd.ID=int.Parse(queueID);
                KeyDataQueueDataDBOption.delete(kd);
                ret = "1";
                Response.Write(ret);
                return;
            }
            catch (System.Exception ex)
            {
                ret = "-100," + ex.Message;
                TextLog.AddTextLog("Cancel_Unkown:" + ex.Message, Global.txtLogFolder + "KeyData.txt", true);
            }
            Response.Write(ret);
        }
    }
}