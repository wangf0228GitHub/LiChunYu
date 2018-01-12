<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ReportList.aspx.cs" Inherits="服务器端.Mobile.ReportList" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html"; charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0 user-scalable=no"/>
    <meta name="format-detection" content="telephone=no"/>
    <link href="css/font-awesome.min.css" rel="stylesheet"/>
    <link rel="stylesheet" type="text/css" href="css/style.css"/>
    <!-- 从官方下载的文件放在项目的 jquery-mobile 目录中 -->  
    <link rel="stylesheet" href="jquery-mobile/jquery.mobile-1.4.5.min.css" />
    <script type="text/javascript" src="jquery-mobile/jquery.mobile-1.4.5.min.js"></script>
    <script src="js/jquery-1.7.1.min.js" type="text/javascript"></script>
    <script type="text/javascript" language="javascript" src="js/jquery.gcjs.js"></script>
    <style type="text/css">
        body {margin:0px; background:#efefef; font-family:'微软雅黑'; -moz-appearance:none;}
        a {text-decoration:none;}    
    
        .wfText {text-align:center; font-size:16px; line-height:44px;}
        .wfInput {border-style: none;
                border-color: inherit;
                border-width: 0px;
                float:left; margin:0px; outline:none; font-size:16px; color:#666;padding-left:5px;
                padding-right: 0px;
                padding-top: 0px;
                padding-bottom: 0px;
                height:44px;
            }
        .wfTR {height:44px; width:100%; padding:0px; margin:0px; border:0px; outline:none; font-size:16px; color:#666;padding-left:5px;}
    
        .address_main {height:auto;width:94%; padding:0px 3%; border-bottom:1px solid #f0f0f0; border-top:1px solid #f0f0f0; margin-top:14px; background:#fff;}
        .address_main .line {height:44px; width:100%; border-bottom:1px solid #f0f0f0; line-height:44px;}

        .address_main .line input {float:left; height:44px; width:100%; padding:0px; margin:0px; border:0px; outline:none; font-size:16px; color:#666;padding-left:5px;}
        .address_main .line select  { border:none;height:25px;width:100%;color:#666;font-size:16px;}
        .address_sub1 {height:44px; margin:14px 10px; background:#ff4f4f; border-radius:4px; text-align:center; font-size:16px; line-height:44px; color:#fff;}

        .address_sub2 {height:44px; margin:14px 10px; background:#ddd; border-radius:4px; text-align:center; font-size:16px; line-height:44px; color:#666; border:1px solid #d4d4d4;}

        .style5
        {
            width: 100%;
        }
        .style6
        {
            width: 121px;
        }
    </style>
    <title>体检报告列表</title>
</head>
<body>
    <div id="container1">
        <div class="page_topbar">
        <div class="title">体检报告列表</div>
        </div>
    </div>
    <form id="form1" runat="server">
    <div id="container">
        <div class="address_main">
            <table class="style5" >
                <tr>
                    <td align="right" class="style6" style="padding: 0px 10px 0px 0px">
                        姓名:</td>
                    <td>
                        <asp:Literal ID="lName" runat="server"></asp:Literal></td>
                </tr>
                <tr>
                    <td colspan="2" style="border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;">   </td>
                </tr>
                <tr>
                    <td align="right" class="style6" style="padding: 0px 10px 0px 0px">
                        出生日期:</td>
                    <td>
                        <asp:Literal ID="lBirthday" runat="server"></asp:Literal></td>
                </tr>
                <tr>
                    <td colspan="2" style="border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;">   </td>
                </tr>
                <tr>
                    <td align="right" class="style6" style="padding: 0px 10px 0px 0px">
                        性别:</td>
                    <td>
                        <asp:Literal ID="lSex" runat="server"></asp:Literal></td>
                </tr>
                <tr>
                    <td colspan="2" style="border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;">   </td>
                </tr>
                <tr>
                    <td align="right" class="style6" style="padding: 0px 10px 0px 0px">
                        民族:</td>
                    <td>
                        <asp:Literal ID="lNation" runat="server"></asp:Literal></td>
                </tr>  
                <tr>
                    <td colspan="2" style="border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;">   </td>
                </tr>
                <%--<tr>
                    <td align="center" colspan="2">
                        <a href="Report.aspx"> </a>
                        点击以下体检日期链接查看体检报告</td>
                </tr>  
                <tr>
                    <td colspan="2" style="border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;">   </td>
                </tr> --%>  
                <asp:Literal ID="listText" runat="server"></asp:Literal>         
            </table>
        </div>
    </div>
    </form>
</body>
</html>
