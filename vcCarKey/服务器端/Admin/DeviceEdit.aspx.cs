using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;

namespace 服务器端.Admin
{
    public partial class DeviceEdit : System.Web.UI.Page
    {
        string strid;
        DeviceInfoData ed;
        protected void Page_Load(object sender, EventArgs e)
        {
            strid = Request.QueryString["id"]; 
            if (!IsPostBack)
            {
                DropDownList1.Items.Clear();
                DropDownList2.Items.Clear();
                for (int i = 1; i < 20; i++)
                {
                    DropDownList1.Items.Add(i.ToString());
                    DropDownList2.Items.Add(i.ToString());
                }
                DropDownList3.Items.Clear();
                DropDownList3.Items.Add("所有");
                DropDownList3.Items.Add("国内");
                DropDownList3.Items.Add("国外");
                if (strid != null && strid != "")//修改
                {
                    ed = DeviceInfoDataDBOption.Get(strid);
                    if (ed.bHPass)
                        CheckBox1.Checked = true;
                    else
                        CheckBox1.Checked = false;
                    if (ed.bDPass)
                        CheckBox2.Checked = true;
                    else
                        CheckBox2.Checked = false;
                    if (ed.bESLPass)
                        CheckBox3.Checked = true;
                    else
                        CheckBox3.Checked = false;
                    if (ed.bKeyData)
                        CheckBox4.Checked = true;
                    else
                        CheckBox4.Checked = false;
                    DropDownList1.SelectedIndex = ed.HPassTimes - 1;
                    DropDownList2.SelectedIndex = ed.DPassTimes - 1;
                    if (ed.bak1 == "所有")
                        DropDownList3.SelectedIndex = 0;
                    else if (ed.bak1 == "国内")
                        DropDownList3.SelectedIndex = 1;
                    else if (ed.bak1 == "国外")
                        DropDownList3.SelectedIndex = 2;
                    else
                        DropDownList3.SelectedIndex = 1;
                }
                else
                {
                    Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('缺少数据');GetDataAndClose(); </script>");
                }
            }
        }

        protected void Button3_Click(object sender, EventArgs e)
        {
            ed = DeviceInfoDataDBOption.Get(strid);
            if (CheckBox1.Checked)
                ed.bHPass = true;
            else
                ed.bHPass = false;
            if (CheckBox2.Checked)
                ed.bDPass = true;
            else
                ed.bDPass = false;
            if (CheckBox3.Checked)
                ed.bESLPass = true;
            else
                ed.bESLPass = false;
            if (CheckBox4.Checked)
                ed.bKeyData = true;
            else
                ed.bKeyData = false;
            ed.HPassTimes = DropDownList1.SelectedIndex + 1;
            ed.DPassTimes = DropDownList2.SelectedIndex + 1;
            switch (DropDownList3.SelectedIndex)
            {
                case 0:
                    ed.bak1 = "所有";
                    break;
                case 1:
                    ed.bak1 = "国内";
                    break;
                case 2:
                    ed.bak1 = "国外";
                    break;
                default:
                    ed.bak1 = "国内";
                    break;
            }
            DeviceInfoDataDBOption.Update(ed);
            //Response.Write("<script language=javascript>alert('修改成功');if (window.opener != undefined){ window.opener.returnValue ='" + str + "'; window.close();}else{window.returnValue = '" + str + "';}window.close();</script>");
            Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('修改成功');GetDataAndClose();</script>");//
        }
    }
}