<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Top.aspx.cs" Inherits="服务器端.Top" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <link rel="stylesheet" type="text/css" href="./Style/skin.css" />
    <script type="text/javascript">
        function logout() {
            if (window.confirm('您确定要退出吗？')) {
                
            }
        }       
    </script>
</head>
<body>
    <form id="form1" runat="server">
    <body>
        <table cellpadding="0" width="100%" height="64" background="./Images/top_top_bg.gif">
            <tr valign="top">
                <td width="50%"><%--<a href="javascript:void(0)"><img style="border:none" src="./Images/logo.png" /></a>--%></td>
                <td width="30%" style="padding-top:13px;font:15px Arial,SimSun,sans-serif;color:#FFF">管理员：
                    <b><asp:LoginName ID="LoginName1" runat="server" /></b> 您好，感谢登陆使用！</td>
                <td style="padding-top:10px;" align="center"><a href="javascript:void(0)"><img style="border:none" src="./Images/index.gif" /></a></td>
                <td style="padding-top:10px;" align="center">
                    <asp:ImageButton ID="ImageButton1" runat="server" 
                        ImageUrl="~/Admin/Images/out.gif" onclientclick="logout();" 
                        style="border:none" onclick="ImageButton1_Click"/>
                    </td>
            </tr>
        </table>
    </body>
    </form>
</body>
</html>
