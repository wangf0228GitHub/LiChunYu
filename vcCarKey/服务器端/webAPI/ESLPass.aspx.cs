using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using WFNetLib;
using System.Data;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class ESLPass : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string DeviceID, pass,sTime;
            string ret=webAPIFunc.GetRetString(ErrType.UnkownErr);
            try
            {
                DeviceID = Request.QueryString["DeviceID"];
                pass = Request.QueryString["pass"];
                sTime = Request.QueryString["sTime"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(DeviceID) || string.IsNullOrEmpty(pass))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                if (DeviceInfoDataDBOption.Get(DeviceID) == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.NoRegDevice);
                    Response.Write(ret);
                    return;
                }
                DataTable dt = ESLRecodeDataDBOption.DataTableSelect(pass);
                if (dt.Rows.Count == 0)
                {
                    if (sTime == "-1")//新的查询
                    {
                        int que = ESLQueueDataDBOption.GetIndex(pass);
                        if (que == -1)//新的查询
                        {
                            ESLQueueData d = new ESLQueueData();
                            d.Pass = pass;
                            d.DeviceID = DeviceID;
                            d.IP = WFNetLib.aspNetFunc.getIp();
                            ESLQueueDataDBOption.Insert(d);
                            que = ESLQueueDataDBOption.GetIndex(pass);
                            ret = webAPIFunc.GetRetString(2, que.ToString() + "|" + d.sTime.ToString());
//                             ret = "2," + que.ToString();
//                             ret += "," + d.sTime.ToString();
                            if (ESLWork.bCalc == false)
                            {
                                ESLWork.autoResetEvent.Set();
                            }
                        }
                        else
                        {
                            ret = "3," + que.ToString();
                        }
                    }
                    else
                    {
                        dt = ESLWorkLogDataDBOption.DataTableSelect(pass, WFNetLib.aspNetFunc.getIp(), DeviceID, sTime);
                        if (dt.Rows.Count != 0)
                        {
                            ret = "-4,";
                        }
                        else
                        {
                            int que = ESLQueueDataDBOption.GetIndex(pass);
                            if (que == -1)//新的查询
                            {
                                ESLQueueData d = new ESLQueueData();
                                d.Pass = pass;
                                d.DeviceID = DeviceID;
                                d.IP = WFNetLib.aspNetFunc.getIp();
                                ESLQueueDataDBOption.Insert(d);
                                que = ESLQueueDataDBOption.GetIndex(pass);
                                ret = webAPIFunc.GetRetString(2, que.ToString() + "|" + d.sTime.ToString());
//                                 ret = "2," + que.ToString();
//                                 ret += "," + d.sTime.ToString();
                                if (ESLWork.bCalc == false)
                                {
                                    ESLWork.autoResetEvent.Set();
                                }
                            }
                            else
                            {
                                ret = "3," + que.ToString();
                            }
                        }
                    }
                }
                else
                {
                    ret = "1,";
//                     for (int i = 0; i < dt.Rows.Count; i++)
//                     {
                    ret += dt.Rows[0]["Ret"];// +",";
//                     }
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);//"-100," + ex.Message;
                TextLog.AddTextLog("Unkown:" + ex.Message, Global.txtLogFolder + "ESLPass.txt", true);
            }
            Response.Write(ret);
        }
    }
}