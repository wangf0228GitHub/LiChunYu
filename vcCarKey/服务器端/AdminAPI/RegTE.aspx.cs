using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using WFNetLib.StringFunc;
using WFNetLib;
using DAL;

namespace 服务器端.webAPI
{
    public partial class softAPI_Reg : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string id, HexVer;
            string ret;
            try
            {
                id = Request.QueryString["ID"];
                HexVer = Request.QueryString["HexVer"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(id) || string.IsNullOrEmpty(HexVer))//没有这两个变量
                {
                    ret = "no data";
                }
                else
                {
                    byte[] bid = StringsFunction.strToHexByte(id, "");
                    double ver = double.Parse(HexVer);
                    if (DeviceInfoDataDBOption.Get(id) != null)
                    {
                        ret = "reged";
                    }
                    else
                    {
                        DeviceInfoData di = new DeviceInfoData();
                        di.ID = id;
                        di.HexVer =ver;
                        DeviceInfoDataDBOption.Insert(di);
                        ret = "ok";
                    }
                }
            }
            catch (System.Exception ex)
            {
                ret = "Exception" + ex.Message;
            }
            Response.Write(ret);
        }
    }
}