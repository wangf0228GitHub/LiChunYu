using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.Data;
using WFNetLib.StringFunc;
using WFNetLib.Log;
using WFNetLib;
using System.IO;

namespace 服务器端.webAPI
{
    public partial class GetHexData : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string DeviceID, PocketIndex;
            string ret = "";
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
                if (index == 1)
                {
                    DeviceInfoData did = DeviceInfoDataDBOption.Get(DeviceID);
                    if (did == null)
                    {
                        ret = webAPIFunc.GetRetString(ErrType.NoRegDevice);
                        Response.Write(ret);
                        return;
                    }
                }
                string fileName=Global.hexBINFolder + DeviceID + ".bin";
                if (!FileOP.IsExist(fileName, FileOPMethod.File))
                {
                    ret = webAPIFunc.GetRetString(ErrType.NoHexBin);
                    Response.Write(ret);
                    return;
                }
                FileInfo f;
                f = new FileInfo(fileName);
                Stream stream = File.OpenRead(f.FullName);
                byte[] pBuf = new byte[stream.Length];
                stream.Read(pBuf, 0, (int)stream.Length);
                stream.Close();
                if (index == 0)
                {
                    ret = webAPIFunc.GetRetString(ErrType.retOK, pBuf.Length.ToString());
                    Response.Write(ret);
                    return;
                }
                else
                {
                    int size = 1024 * 5; // 0480;
                    int count = pBuf.Length / size;
                    int left = pBuf.Length % size;
                    int x = size * (index - 1);
                    string str;
                    if (index > count)//最后一包
                    {
                        //ret = ((int)ErrType.retOK).ToString() + ",";
                        str = StringsFunction.byteToHexStr(pBuf, x, left, "");
                        Response.Write(webAPIFunc.GetRetString(ErrType.retOK, str));
                        return;
                    }
                    else
                    {
                        //ret = ((int)ErrType.retOK).ToString() + ",";
                        str = StringsFunction.byteToHexStr(pBuf, x, size, "");
                        ret = ret + str;
                        //Debug.WriteLine(ret);
                        Response.Write(webAPIFunc.GetRetString(ErrType.retOK, str));
                        return;
                    }
                }
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("GetHexData_Unkown:" + ex.Message, Global.txtLogFolder + "Update.txt", true);
            }
            Response.Write(ret);
        }
    }
}