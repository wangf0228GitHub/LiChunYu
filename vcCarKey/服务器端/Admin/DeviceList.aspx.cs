using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.Data;

namespace 服务器端.Admin
{
    public partial class DeviceList : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (HiddenField1.Value == null || HiddenField1.Value == "1")
            {
                HiddenField1.Value = "";
                BindDisplayList();
            }
            if (!IsPostBack)
            {
                DropDownList1.Items.Clear();
                DropDownList1.Items.Add("请选择");
                DataTable dt= DeviceInfoDataDBOption.DataTableSelect();
                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    DropDownList1.Items.Add(dt.Rows[i]["ID"].ToString());
                }                
                //保存当前查询状态
                DataTable tb = DeviceInfoDataDBOption.DataTableSelect_addrownum();
                BindDisplayList();
            }
        }
        private void BindDisplayList()
        {
            BindDisplayList(0);
        }        
        private void BindDisplayList(int pageIndex)
        {
            DataTable tb;
            if(HiddenField2.Value=="0")
               tb = DeviceInfoDataDBOption.DataTableSelect_addrownum();
            else if(HiddenField2.Value=="1")
                tb = DeviceInfoDataDBOption.DataTableSelect_byID(TextBox1.Text);
            else
                tb = DeviceInfoDataDBOption.DataTableSelect_byID(DropDownList1.Text);        
            WFWebLib.GridViewOperation.BindPagingGridView(GridView1, tb.DefaultView, pageIndex);
        }

        protected void GridView1_RowCreated(object sender, GridViewRowEventArgs e)
        {
            if (e.Row.RowType == DataControlRowType.Pager)
                WFWebLib.GridViewOperation.GridRowCreated(sender, e);
        }

        protected void GridView1_PageIndexChanging(object sender, GridViewPageEventArgs e)
        {
            BindDisplayList(e.NewPageIndex);
        }

        protected void GridView1_RowDataBound(object sender, GridViewRowEventArgs e)
        {
            if (e.Row.RowType == DataControlRowType.DataRow)
            {
                string id = e.Row.Cells[1].Text;
                ((ImageButton)e.Row.Cells[10].Controls[0]).Attributes.Add("onclick", "if(!showdialog('" + id + "')) return");
                ((ImageButton)e.Row.Cells[11].Controls[0]).Attributes.Add("onclick", "if(!window.confirm('确定删除此课程')) return;");
            }
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            HiddenField2.Value = "1";
            BindDisplayList();
        }

        protected void Button2_Click(object sender, EventArgs e)
        {
            if (DropDownList1.SelectedIndex == 0)
            {
                HiddenField2.Value = "0";
            }
            else
            {
                HiddenField2.Value = "2";
            }
            BindDisplayList();
        }

        protected void GridView1_RowDeleting(object sender, GridViewDeleteEventArgs e)
        {
            GridViewRow dr = GridView1.Rows[e.RowIndex];
            string id = dr.Cells[1].Text;
            DeviceInfoData dd = new DeviceInfoData();
            dd.ID = id;
            DeviceInfoDataDBOption.Delete(dd);
        }
    }
}