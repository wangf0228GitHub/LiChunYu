using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.Data;
using WFNetLib.StringFunc;
using System.Diagnostics;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class GetSoftData : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string DeviceID,PocketIndex;
            string ret =webAPIFunc.GetRetString(ErrType.UnkownErr);
            try
            {
                DeviceID = Request.QueryString["DeviceID"];
                PocketIndex = Request.QueryString["PocketIndex"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(DeviceID) || string.IsNullOrEmpty(PocketIndex))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                int index;
                if (!int.TryParse(PocketIndex, out index))
                {
                    ret = webAPIFunc.GetRetString(ErrType.ErrParam);
                    Response.Write(ret);
                    return;
                }
                if(index==1)
                {
                    DeviceInfoData did = DeviceInfoDataDBOption.Get(DeviceID);
                    if (did == null)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.NoRegDevice);
                        Response.Write(ret);
                        return;
                    }
                }
                DataTable dt = UpdateDataDBOption.SoftDataTableSelect();
                byte[] data = (byte [])dt.Rows[0][1];                
                if(index==0)
                {
                    ret = "1,"+data.Length.ToString();
                    Response.Write(ret);
                    return;
                }
                else
                {
                    int size = 2048*5; // 0480;
                    int count=data.Length/size;
                    int left=data.Length%size;
                    int x=size*(index-1);
                    string str;
                    if (index > count)//最后一包
                    {
                        ret = "1,";
                        str = StringsFunction.byteToHexStr(data, x, left, "");
                        Response.Write(ret+str);
                        return;
                    }
                    else
                    {
                        ret = "1,";
                        str = StringsFunction.byteToHexStr(data, x, size, "");
                        ret = ret + str;
                        Debug.WriteLine(ret);
                        Response.Write(ret);
                        return;
                    }
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("GetSoftData_Unkown:" + ex.Message, Global.txtLogFolder + "Update.txt", true);
            }
            Response.Write(ret);
        }
    }
}