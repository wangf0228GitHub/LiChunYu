<%@ Page Title="主页" Language="C#"  AutoEventWireup="true"
    CodeBehind="Default.aspx.cs" Inherits="服务器端._Default" %>
<html xmlns="http://www.w3.org/1999/xhtml" lang="zh-CN">
<head>
    <title>数据服务器 - 后台管理中心</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="description" content="数据服务器后台管理系统" />
    <meta name="keywords" content="数据服务器 ,网站管理系统,企业网站" />
    <meta name="Author" content="XinRui" />
    <meta name="CopyRight" content="锋行天下" />
    <link rel="stylesheet" type="text/css" href="./Style/skin.css" />
    <style type="text/css">
        .style1
        {
            height: 70px;
        }
    </style>
</head>
<body>
    <form id="form1" runat="server">
    <table width="100%">
            <!-- 顶部部分 -->
            <tr height="41"><td colspan="2" background="./Images/login_top_bg.gif">&nbsp;</td></tr>
            <!-- 主体部分 -->
            <tr style="background:url(./Images/login_bg.jpg) repeat-x;" height="532">
                <!-- 主体左部分 -->
                <td id="left_cont">
                    <table width="100%" height="100%">
                        <tr height="155"><td colspan="2">&nbsp;</td></tr>
                        <tr>
                            <td width="20%" rowspan="2">&nbsp;</td>
                            <td width="60%">
                                <table width="100%">
                                    <tr><td align="right" class="style1"><img src="./Images/logo.gif" title="锋行天下" alt="锋行天下" /></td></tr>
                                    <tr height="274">
                                        <td valign="top" align="right">                                        
                                            <%--<ul>
                                                <li>1- 企业门户站建立的首选方案...</li>
                                                <li>2- 一站通式的整合方式，方便用户使用...</li>
                                                <li>3- 强大的后台系统，管理内容易如反掌...</li>
                                                <li><img src="./Images/icon_demo.gif" />&nbsp;<a href="javascript:void(0)">使用说明</a>&nbsp;&nbsp;<span> <a target="_blank" href="http://wpa.qq.com/msgrd?v=3&uin=609307843&site=qq&menu=yes" onfocus="this.blur()"><img border="0" src="http://wpa.qq.com/pa?p=2:609307843:41" alt="瑞曼为您服务" title="锋行天下"></a> </span></li>
                                            </ul>--%>
                                        </td>
                                    </tr>
                                </table>
                            <td width="15%" rowspan="2">&nbsp;</td>
                            </td>
                        </tr>
                        <tr><td colspan="2">&nbsp;</td></tr>
                    </table>
                </td>
                <!-- 主体右部分 -->
                <td id="right_cont">
                    <table height="100%">
                        <tr height="30%"><td colspan="3">&nbsp;</td></tr>
                        <tr>
                            <td width="30%" rowspan="5">&nbsp;</td>
                            <td valign="top" id="form">
                                数据服务器 - 后台管理中心<br/>
                                <asp:LoginView ID="LoginView1" runat="server">
                                    <LoggedInTemplate>
                                        当前登陆用户为：<asp:LoginName ID="LoginName1" runat="server" />
                                        <br />
                                        <br />
                                        <br />
                                        <br />
                                        <asp:Button ID="Button2" runat="server" Text="进入" 
                                            Width="132px" onclick="Button2_Click" />
                                        <br />
                                        <br />
                                        <br />
                                        <br />
                                        <asp:Button ID="Button1" runat="server" onclick="Button1_Click" Text="注销" 
                                            Width="132px" />
                                    </LoggedInTemplate>
                                    <AnonymousTemplate>
                                        <asp:Login ID="Login1" runat="server" onloggedin="Login1_LoggedIn">
                                            <LayoutTemplate>
                                                <table border="0" cellpadding="1" cellspacing="0" 
                                                    style="border-collapse:collapse;">
                                                    <tr>
                                                        <td>
                                                            <table border="0" cellpadding="0">
                                                                <tr>
                                                                    <td align="center" colspan="2">
                                                                        欢迎您登录使用</td>
                                                                </tr>
                                                                <tr>
                                                                    <td align="right">
                                                                        <asp:Label ID="UserNameLabel" runat="server" AssociatedControlID="UserName" 
                                                                            CssClass="login_txt">用户名:</asp:Label>
                                                                    </td>
                                                                    <td>
                                                                        <asp:TextBox ID="UserName" runat="server" Font-Size="Small"></asp:TextBox>
                                                                        <asp:RequiredFieldValidator ID="UserNameRequired" runat="server" 
                                                                            ControlToValidate="UserName" ErrorMessage="必须填写“用户名”。" ToolTip="必须填写“用户名”。" 
                                                                            ValidationGroup="ctl00$Login1">*</asp:RequiredFieldValidator>
                                                                    </td>
                                                                </tr>
                                                                <tr>
                                                                    <td align="right">
                                                                        <asp:Label ID="PasswordLabel" runat="server" AssociatedControlID="Password" 
                                                                            CssClass="login_txt">密码:</asp:Label>
                                                                    </td>                                                    
                                                                    <td>
                                                                        <asp:TextBox ID="Password" runat="server" TextMode="Password" 
                                                                            Font-Size="Small"></asp:TextBox>
                                                                        <%--<img src="image/luck.gif" width="19" height="18" alt="">--%>
                                                                        <asp:RequiredFieldValidator ID="PasswordRequired" runat="server" 
                                                                            ControlToValidate="Password" ErrorMessage="必须填写“密码”。" ToolTip="必须填写“密码”。" 
                                                                            ValidationGroup="ctl00$Login1">*</asp:RequiredFieldValidator>                                                        
                                                                    </td>
                                                                </tr>
                                                                <%--<tr>
                                                                    <td align="right">
                                                                        <asp:Label ID="VerifyLabel" runat="server" AssociatedControlID="AuthCode1" 
                                                                            CssClass="login_txt">校验码:</asp:Label>
                                                                    </td>
                                                                    <td align="right">
                                                                        <verify:AuthCode ID="AuthCode1" runat="server" TextBoxClass="authText" EnableClientValidate="true" IsChangeImageOnPostback="true"
                                                                            SubmitControlID="LoginButton" EnableViewState="False" />
                                                                    </td>                                           
                                                                </tr>       --%>                                         
                                                                <tr>
                                                                    <td align="center" colspan="2" style="color:Red;">
                                                                        <asp:Literal ID="FailureText" runat="server" EnableViewState="False"></asp:Literal>
                                                                    </td>
                                                                </tr>
                                                                <tr>
                                                                    <td align="right" colspan="2">
                                                                        <asp:Button ID="LoginButton" runat="server" CommandName="Login" Text="登录" 
                                                                            ValidationGroup="ctl00$Login1" onclick="LoginButton_Click"/>
                                                                    </td>
                                                                </tr>
                                                            </table>
                                                        </td>
                                                    </tr>
                                                </table>
                                            </LayoutTemplate>
                                        </asp:Login>
                                    </AnonymousTemplate>
                                </asp:LoginView>
<%--                                <form action="" method="">
                                    <table valign="top" width="50%">
                                        <tr><td colspan="2"><h4 style="letter-spacing:1px;font-size:16px;">RainMan 网站管理后台</h4></td></tr>
                                        <tr><td>管理员：</td><td><input type="text" name="" value="" /></td></tr>
                                        <tr><td>密&nbsp;&nbsp;&nbsp;&nbsp;码：</td><td><input type="password" name="" value="" /></td></tr>
                                        <tr><td>验证码：</td><td><input type="text" name="" value="" style="width:80px;"/></td></tr>
                                        <tr class="bt" align="center"><td>&nbsp;<input type="submit" value="登陆" /></td><td>&nbsp;<input type="reset" value="重填" /></td></tr>
                                    </table>
                                </form>--%>
                            </td>
                            <td rowspan="5">&nbsp;</td>
                        </tr>
                        <tr><td colspan="3">&nbsp;</td></tr>
                    </table>
                </td>
            </tr>
            <!-- 底部部分 -->
            <tr id="login_bot"><td colspan="2"><p>Copyright wangf 网络工作室</p></td></tr>
        </table>
    </form>
</body>
</html>
