using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.Data;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class GetSoftVer : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            try
            {
                double v1, v2;
                v1 = UpdateDataDBOption.GetSoftVer();
                v2 = UpdateDataDBOption.GetHexVer();
                Response.Write(((int)ErrType.retOK).ToString() + "," + v1.ToString("f2") + "|" + v2.ToString("f2"));
            }
            catch (System.Exception ex)
            {
                Response.Write( "-100," + ex.Message);
                TextLog.AddTextLog("GetVer_Unkown:" + ex.Message, Global.txtLogFolder + "Update.txt", true);
            }            
        }
    }
}