using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;

namespace 服务器端.Admin
{
    public partial class DeviceSet : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                DropDownList1.Items.Clear();
                DropDownList2.Items.Clear();
                for (int i = 1; i < 20; i++)
                {
                    DropDownList1.Items.Add(i.ToString());
                    DropDownList2.Items.Add(i.ToString());
                }
            }
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            int ret = DeviceInfoDataDBOption.SetTimes(DropDownList1.SelectedIndex + 1, DropDownList2.SelectedIndex + 1);
            Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('设定成功，共有"+ret.ToString()+"个设备参数被设置'); </script>");
        }
    }
}