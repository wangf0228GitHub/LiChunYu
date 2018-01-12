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
    public partial class KeyDataStatus : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string queueID;
            string ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
            try
            {
                queueID = Request.QueryString["queueID"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(queueID))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                KeyDataWorkLogData kdl=KeyDataWorkLogDataDBOption.Get(int.Parse(queueID));
                if (kdl != null)//已完成
                {
                    if(kdl.bOK)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.retOK); 
                        Response.Write(ret);
                        return;
                    }
                    else
                    {
                        ret = "4,";
                        Response.Write(ret);
                        return;
                    }
                }
                int index = KeyDataQueueDataDBOption.GetIndex(queueID);
                if (index == -1)//申请的数据无效
                {
                    ret = "3,";
                    Response.Write(ret);
                    return;
                }
                else
                {
                    ret = "2,"+index.ToString();
                    Response.Write(ret);
                    return;
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Status_Unkown:" + ex.Message, Global.txtLogFolder + "KeyData.txt", true);
            }
            Response.Write(ret);
        }
    }
}