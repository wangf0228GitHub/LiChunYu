<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="left.aspx.cs" Inherits="服务器端.left" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <script src="./Js/prototype.lite.js" type="text/javascript"></script>
    <script src="./Js/moo.fx.js" type="text/javascript"></script>
    <script src="./Js/moo.fx.pack.js" type="text/javascript"></script>
    <link rel="stylesheet" type="text/css" href="./Style/skin.css" />
    <script type="text/javascript">
        window.onload = function () {
            var contents = document.getElementsByClassName('content');
            var toggles = document.getElementsByClassName('type');

            var myAccordion = new fx.Accordion(
            toggles, contents, { opacity: true, duration: 400 }
            );
            myAccordion.showThisHideOpen(contents[0]);
            for (var i = 0; i < document.getElementsByTagName("a").length; i++) {
                var dl_a = document.getElementsByTagName("a")[i];
                dl_a.onfocus = function () {
                    this.blur();
                }
            }
        }
    </script>
</head>
<body>
    <form id="form1" runat="server">
    <table width="100%" height="280" border="0" cellpadding="0" cellspacing="0" bgcolor="#EEF2FB">
        <tr>
            <td width="182" valign="top">
                <div id="container">
                    <h1 class="type"><a href="javascript:void(0)">设备详情</a></h1>
                    <div class="content">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
                            <tr>
                                <td><img src="./Images/menu_top_line.gif" width="182" height="5" /></td>
                            </tr>
                        </table>
                        <ul class="RM">
                            <li><a href="DeviceList.aspx" target="main">设备信息</a></li>
                            <li><a href="DeviceSet.aspx" target="main">设备设定</a></li>
                        </ul>
                    </div>
                    <h1 class="type"><a href="javascript:void(0)">工作日志</a></h1>
                    <div class="content">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
                            <tr>
                                <td><img src="./Images/menu_top_line.gif" width="182" height="5" /></td>
                            </tr>
                        </table>
                        <ul class="RM">
                            <li><a href="ESLWorkLog.aspx" target="main">ESL日志</a></li>
                            <li><a href="DPassWorkLog.aspx" target="main">DPass日志</a></li>
                            <li><a href="HPassWorkLog.aspx" target="main">HPass日志</a></li>
                            <li><a href="DPassWorkLog.aspx" target="main">KeyData日志</a></li>
                        </ul>
                    </div>
                    <h1 class="type"><a href="javascript:void(0)">系统升级</a></h1>
                    <div class="content">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
                            <tr>
                                <td><img src="./Images/menu_top_line.gif" width="182" height="5" /></td>
                            </tr>
                        </table>
                        <ul class="RM">
                            <li><a href="HexUpdate.aspx" target="main">硬件升级</a></li>
                            <li><a href="SoftUpdate.aspx" target="main">软件升级</a></li>
                        </ul>
                    </div>
                    <!-- *********** -->
                    <%--
                    <h1 class="type"><a href="javascript:void(0)">会员管理</a></h1>
                    <div class="content">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
                            <tr>
                                <td><img src="./Images/menu_top_line.gif" width="182" height="5" /></td>
                            </tr>
                        </table>
                        <ul class="RM">
                            <li><a href="./mem_reg.html" target="main">注册设置</a></li>
                            <li><a href="./mem_chk.html" target="main">审核设置</a></li>
                            <li><a href="./mem_add.html" target="main">添加会员</a></li>
                            <li><a href="./mem_list.html" target="main">会员管理</a></li>
                        </ul>
                    </div>
                    <h1 class="type"><a href="javascript:void(0)">系统设置</a></h1>
                    <div class="content">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
                            <tr>
                                <td><img src="./Images/menu_top_line.gif" width="182" height="5" /></td>
                            </tr>
                        </table>
                        <ul class="RM">
                            <li><a href="./sys.html" target="main">网站设置</a></li>
                            <li><a href="./admin.html" target="main">管理员设置</a></li>
                            <li><a href="javascript:void(0)" target="main">模板设置</a></li>
                        </ul>
                    </div>
                    <!-- *********** -->
                    <h1 class="type"><a href="javascript:void(0)">其它设置</a></h1>
                    <div class="content">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
                            <tr>
                                <td><img src="./Images/menu_top_line.gif" width="182" height="5" /></td>
                            </tr>
                        </table>
                        <ul class="RM">
                            <li><a href="javascript:void(0)" target="main">友情连接</a></li>
                            <li><a href="javascript:void(0)" target="main">在线留言</a></li>
                            <li><a href="javascript:void(0)" target="main">网站投票</a></li>
                            <li><a href="javascript:void(0)" target="main">邮箱设置</a></li>
                            <li><a href="javascript:void(0)" target="main">图片上传</a></li>
                        </ul>
                    </div>--%>
                    <!-- *********** -->
                </div>
            </td>
        </tr>
    </table>
    </form>
</body>
</html>
