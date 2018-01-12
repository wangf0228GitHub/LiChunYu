using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.Security;
using DAL;
using System.Data;
using System.Text;

namespace 服务器端.Mobile
{
    public partial class ReportList : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                if (!HttpContext.Current.User.Identity.IsAuthenticated)
                {
                    Response.Redirect("~/Mobile/Login.aspx");
                    return;
                }
                if (!HttpContext.Current.User.IsInRole("会员"))
                {
                    Response.Redirect("~/Mobile/Login.aspx");
                    return;
                }
//                 UserInfoData userInfo=UserInfoDataDBOption.Get(HttpContext.Current.User.Identity.Name);
//                 lName.Text = userInfo.Name;
//                 lSex.Text = userInfo.Sex;
//                 lNation.Text = userInfo.Nation;
//                 lBirthday.Text = userInfo.Born;
//                 MedicalResultData m = new MedicalResultData();
//                 m.ID = HttpContext.Current.User.Identity.Name;                
//                 DataTable dt = MedicalResultDataDBOption.DataTableSelect("0",m);
//                 StringBuilder sb=new StringBuilder();
//                 if (dt.Rows.Count == 0)
//                 {
//                     sb.Append("<tr>\r\n");                    
//                     sb.Append("<td align=\"center\" colspan=\"2\">");
//                     sb.Append("你当前没有任何体检记录\r\n");
//                     sb.Append("</td>");
//                     sb.Append("</tr>");
//                     listText.Text = sb.ToString();
//                 }
//                 else
//                 {
//                     sb.Append("<tr>\r\n");
//                     sb.Append("<td align=\"center\" colspan=\"2\">\r\n");
//                     sb.Append("点击以下体检日期链接查看体检报告\r\n");
//                     sb.Append("</td>\r\n");
//                     sb.Append("</tr>\r\n");
// 
//                     sb.Append("<tr>\r\n");
//                     sb.Append("<td colspan=\"2\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                     sb.Append("</td>\r\n");
//                     sb.Append("</tr>\r\n");
//                     for (int i = 0; i < dt.Rows.Count; i++)
//                     {
//                         DataRow dr = dt.Rows[i];
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\" colspan=\"2\">\r\n");
//                         sb.Append("<a href=\"Report.aspx?time=" + dr["Time"].ToString() + "\">");
//                         sb.Append(dr["Time"].ToString()+"\r\n");
//                         sb.Append("</a>");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"2\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                     }
//                     listText.Text = sb.ToString();
//                 }                
            }
        }
    }
}