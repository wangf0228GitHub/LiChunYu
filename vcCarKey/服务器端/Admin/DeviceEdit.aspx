<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="DeviceEdit.aspx.cs" Inherits="服务器端.Admin.DeviceEdit" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>设备信息编辑</title>
    <script type="text/javascript">
        function closewindow() {
            window.close();
        }
        function GetDataAndClose() {
            var array = new Array(1);
            if (window.opener != undefined) {
                window.opener.returnValue = array;
            }
            else {
                window.returnValue = array;
            }
            window.close();
            //closewindow();
        }
    </script>
    <base target="_self"/>
    <style type="text/css">
        .style1
        {
            width: 100%;
        }
        </style>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <table class="style1">            
            <tr>
                <td align="right" width="50%">
                    HPass</td>
                <td>
                    <asp:CheckBox ID="CheckBox1" runat="server" />
                </td>
            </tr>
            <tr>
                <td align="right" width="50%">
                    DPass</td>
                <td>
                    <asp:CheckBox ID="CheckBox2" runat="server" />
                </td>
            </tr>
            <tr>
                <td align="right" width="50%">
                    ESLPass</td>
                <td>
                    <asp:CheckBox ID="CheckBox3" runat="server" />
                </td>
            </tr>
            <tr>
                <td align="right" width="50%">
                    KeyData</td>
                <td>
                    <asp:CheckBox ID="CheckBox4" runat="server" />
                </td>
            </tr>
            <tr>
                <td align="right" width="50%">
                    HPass 次/天</td>
                <td>
                    <asp:DropDownList ID="DropDownList1" runat="server">
                    </asp:DropDownList>
                </td>
            </tr>
            <tr>
                <td align="right" width="50%">
                    DPass 次/天</td>
                <td>
                    <asp:DropDownList ID="DropDownList2" runat="server">
                    </asp:DropDownList>
                </td>
            </tr>   
            <tr>
                <td align="right" width="50%">
                    地域</td>
                <td>
                    <asp:DropDownList ID="DropDownList3" runat="server">
                    </asp:DropDownList>
                </td>
            </tr>          
            <tr>
                <td align="right" width="50%">
                    <asp:Button ID="Button3" runat="server" style="margin-left: 0px" Text="确定" 
                        Width="66px" onclick="Button3_Click" />
                </td>
                <td>
                    <asp:Button ID="Button4" runat="server" Text="取消" Width="74px" 
                        onclientclick="closewindow();" CausesValidation="False" />
                </td>
            </tr>
        </table>
    </div>
    </form>
</body>
</html>
