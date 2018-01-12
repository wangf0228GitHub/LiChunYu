using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace 服务器端.webAPI
{
    public partial class TestIP : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            HttpRequest request = HttpContext.Current.Request; 
            string result = request.ServerVariables["HTTP_X_FORWARDED_FOR"]; 
            if (string.IsNullOrEmpty(result)) 
            { 
                result = request.ServerVariables["REMOTE_ADDR"]; 
            } 
            if (string.IsNullOrEmpty(result)) 
            { 
                result = request.UserHostAddress;
            } 
            if (string.IsNullOrEmpty(result)) 
            { 
                result = "0.0.0.0"; 
            }
            string s1=HttpContext.Current.Request.UserHostAddress;
            string s2 = HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"];
            string strHostName = System.Net.Dns.GetHostName();
            string s3 = System.Net.Dns.GetHostAddresses(strHostName).GetValue(0).ToString();
            string s4 = HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"];
            string s5 = WFNetLib.aspNetFunc.getIp();
            string s6,s7="";
            string s8 = System.Web.HttpContext.Current.Request.ServerVariables["HTTP_VIA"];
            if (System.Web.HttpContext.Current.Request.ServerVariables["HTTP_VIA"] != null)
            {
                s6 = System.Web.HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"].Split(new char[] { ',' })[0];
                s7 = System.Web.HttpContext.Current.Request.ServerVariables["HTTP_X_FORWARDED_FOR"];
            }
            else
                s6=System.Web.HttpContext.Current.Request.ServerVariables["REMOTE_ADDR"];
            Response.Write(result + "|" + s1 + "|" + s2 + "|" + s3 + "|" + s4 + "|" + s5 + "|" + s6 + "|" + s7+"|"+s8);
        }
    }
}