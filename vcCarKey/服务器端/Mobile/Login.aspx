<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="服务器端.Mobile.Login" %>

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

        .style1
        {
            width: 56px;
        }
        .style2
        {
            width: 56px;
            height: 44px;
        }
        .style3
        {
            height: 44px;
        }
        .style4
        {
            width: 120px;
        }
    </style>
    <title>登陆</title>
</head>
<body>

<!-- <script src="http://code.jquery.com/jquery-2.1.4.min.js"></script> -->
    <div id="container1">
        <div class="page_topbar">
        <div class="title">用户登陆</div>
        </div>
    </div>
    <form id="form1" runat="server">    
        <div id="container">
            <div class="address_main">
                <asp:Login ID="Login2" runat="server" Width="100%" 
                    DestinationPageUrl="~/Mobile/ReportList.aspx" onloggingin="Login2_LoggingIn">
                    <LayoutTemplate>
                        <table cellpadding="1" cellspacing="0" width="100%" style="border-collapse:collapse;">
                            <tr>
                                <td>
                                    <table cellpadding="0" width="100%" class="wfText" >
                                        <tr>
                                            <td align="right" class="style2" >
                                                <asp:Label ID="UserNameLabel" runat="server" AssociatedControlID="UserName">用户名:</asp:Label>
                                            </td>
                                            <td class="style3" >
                                                <asp:TextBox ID="UserName" runat="server" CssClass="wfInput" Width="100%" placeholder="您的身份证号"></asp:TextBox>
                                            </td>
                                            <td class="style4" >
                                                <asp:RequiredFieldValidator ID="UserNameRequired" runat="server" 
                                                    ControlToValidate="UserName" ErrorMessage="必须填写“用户名”。" ToolTip="必须填写“用户名”。" 
                                                    ValidationGroup="Login2">*</asp:RequiredFieldValidator>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td align="right" colspan="3">
                                                 <hr style="height:1px;border-top:1px #f0f0f0; border-style: solid none none none; border-width: 1px medium medium medium; border-top-color: #f0f0f0; border-right-color: inherit; border-bottom-color: inherit; border-left-color: inherit;"/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td align="right" class="style1">
                                                <asp:Label ID="PasswordLabel" runat="server" AssociatedControlID="Password" >密码:</asp:Label>
                                            </td>
                                            <td class="style3">
                                                <asp:TextBox ID="Password" runat="server" TextMode="Password" CssClass="wfInput" Width="100%" placeholder="您注册的手机号"></asp:TextBox>
                                            </td>
                                            <td>
                                                <asp:RequiredFieldValidator ID="PasswordRequired" runat="server" 
                                                    ControlToValidate="Password" ErrorMessage="必须填写“密码”。" ToolTip="必须填写“密码”。" 
                                                    ValidationGroup="Login2">*</asp:RequiredFieldValidator>
                                            </td>
                                        </tr>  
                                        <tr>
                                            <td align="right" colspan="3">
                                                 <hr style="height:1px;border-top:1px #f0f0f0; border-style: solid none none none; border-width: 1px medium medium medium; border-top-color: #f0f0f0; border-right-color: inherit; border-bottom-color: inherit; border-left-color: inherit;"/>
                                            </td>
                                        </tr>                              
                                        <tr>
                                            <td align="center" colspan="3" style="color:Red;">
                                                <asp:Literal ID="FailureText" runat="server" EnableViewState="False"></asp:Literal>
                                            </td>
                                        </tr>                                
                                        <tr>
                                            <td align="right" colspan="3">
                                                <asp:Button ID="LoginButton" runat="server" CommandName="Login" 
                                                    Text="登录" 
                                                    ValidationGroup="Login2"  CssClass="address_sub1" Width="100%" />
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                        </table>
                    </LayoutTemplate>
                </asp:Login>                
            </div>
        </div>
    </form>
</body>
</html>
