using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.IO;
using System.Data.SqlClient;
using System.Data;

namespace 服务器端.Admin
{
    public partial class HexUpdate : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                Double HexVer = UpdateDataDBOption.GetHexVer();
                Label1.Text = HexVer.ToString("0.00");
            }
        }

        protected void Button2_Click(object sender, EventArgs e)
        {
            double ver;
            if (!double.TryParse(TextBox1.Text, out ver))
            {
                Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('输入的版本号必须为数字'); </script>");
                return;
            }
//             if (!FileUpload1.HasFile)
//             {
//                 Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('需要上传文件'); </script>");
//                 return;
//             }
//             if (Path.GetExtension(FileUpload1.FileName).ToLower()!=".bin")
//             {
//                 Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('必须加载bin文件'); </script>");
//                 return;
//             }
//             if (FileUpload1.FileBytes.Length != 0x7010)
//             {
//                 Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('文件大小错误'); </script>");
//                 return;
//             }
            using (SqlConnection conn = new SqlConnection(WFNetLib.ADO.SQLServerOP.ConnectionString))
            {
//                 String sql = "update [Update] set HexVer=@h,HexBin=@b";
//                 SqlCommand cmd = new SqlCommand(sql, conn);
//                 cmd.Parameters.AddWithValue("@h", TextBox1.Text);
//                 cmd.Parameters.AddWithValue("@b", FileUpload1.FileBytes);                
//                 conn.Open();
//                 try
//                 {
//                     cmd.ExecuteNonQuery();
//                     Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('上传成功'); </script>");
//                     Double HexVer = UpdateDataDBOption.GetHexVer();
//                     Label1.Text = HexVer.ToString("0.00");
//                 }
//                 catch (Exception ex)
//                 {
//                     Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('"+ex.Message+"'); </script>");
//                 }
                String sql = "update [Update] set HexVer=@h";
                SqlCommand cmd = new SqlCommand(sql, conn);
                cmd.Parameters.AddWithValue("@h", TextBox1.Text);
                conn.Open();
                try
                {
                    cmd.ExecuteNonQuery();
                    Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('设定成功'); </script>");
                    Double HexVer = UpdateDataDBOption.GetHexVer();
                    Label1.Text = HexVer.ToString("0.00");
                }
                catch (Exception ex)
                {
                    Page.ClientScript.RegisterStartupScript(this.GetType(), "", " <script lanuage=javascript>alert('" + ex.Message + "'); </script>");
                }

            }            
        }

        protected void FileUpload1_Load(object sender, EventArgs e)
        {
            
        }
    }
}