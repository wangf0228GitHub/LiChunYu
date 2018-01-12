using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.Configuration;
using System.Management;
using System.IO;
//using System.Web.Security;
using System.Data;
using Microsoft.VisualBasic;

namespace WFNetLib.StringFunc
{
    public abstract class StringsFunction
    {
    #region 正则表达式的使用

        /// <summary>
        /// 判断输入的字符串是否完全匹配正则
        /// </summary>
        /// <param name="RegexExpression">正则表达式</param>
        /// <param name="str">待判断的字符串</param>
        /// <returns></returns>
        public static bool IsValiable(string RegexExpression, string str)
        {
            bool blResult = false;

            Regex rep = new Regex(RegexExpression, RegexOptions.IgnoreCase);

            //blResult = rep.IsMatch(str);
            Match mc = rep.Match(str);

            if (mc.Success)
            {
                if (mc.Value == str) blResult = true;
            }


            return blResult;
        }
        /// <summary>
        /// 判断输入的用户名是否符合规则，用户名由minL-maxL位的字母或数字组成或下划线
        /// </summary>
        /// <param name="minL">最短长度</param>
        /// <param name="maxL">最大长度</param>
        /// <param name="str">待判断的字符串</param>
        /// <returns></returns>
        public static bool IsValiable_UserName(string str, int minL, int maxL)
        {
            bool blResult = false;
            string RegexExpression = string.Format(@"^\w{{{0},{1}}}$", minL, maxL);
            Regex rep = new Regex(RegexExpression, RegexOptions.IgnoreCase);

            //blResult = rep.IsMatch(str);
            Match mc = rep.Match(str);

            if (mc.Success)
            {
                if (mc.Value == str) blResult = true;
            }


            return blResult;
        }
        /// <summary>
        /// 判断输入的用户名是否符合规则，用户名由4-16位的字母或数字组成或下划线
        /// </summary>
        /// <param name="str">待判断的字符串</param>
        /// <returns></returns>
        public static bool IsValiable_UserName(string str)
        {
            return IsValiable_UserName(str, 4, 16);
        }

        /// <summary>
        /// 转换代码中的URL路径为绝对URL路径
        /// </summary>
        /// <param name="sourceString">源代码</param>
        /// <param name="replaceURL">替换要添加的URL</param>
        /// <returns>string</returns>
        public static string convertURL(string sourceString, string replaceURL)
        {
            Regex rep = new Regex(" (src|href|background|value)=('|\"|)([^('|\"|)http://].*?)('|\"| |>)");
            sourceString = rep.Replace(sourceString, " $1=$2" + replaceURL + "$3$4");

            return sourceString;
        }

        /// <summary>
        /// 获取代码中所有图片的以HTTP开头的URL地址
        /// </summary>
        /// <param name="sourceString">代码内容</param>
        /// <returns>ArrayList</returns>
        public static ArrayList GetImgFileUrl(string sourceString)
        {
            ArrayList imgArray = new ArrayList();

            Regex r = new Regex("<IMG(.*?)src=('|\"|)(http://.*?)('|\"| |>)", RegexOptions.IgnoreCase | RegexOptions.Compiled);

            MatchCollection mc = r.Matches(sourceString);
            for (int i = 0; i < mc.Count; i++)
            {
                if (!imgArray.Contains(mc[i].Result("$3")))
                {
                    imgArray.Add(mc[i].Result("$3"));
                }
            }

            return imgArray;
        }

        /// <summary>
        /// 获取代码中所有文件的以HTTP开头的URL地址
        /// </summary>
        /// <param name="sourceString">代码内容</param>
        /// <returns>ArrayList</returns>
        public static Hashtable getFileUrlPath(string sourceString)
        {
            Hashtable url = new Hashtable();

            Regex r = new Regex(" (src|href|background|value)=('|\"|)(http://.*?)('|\"| |>)",
                RegexOptions.IgnoreCase | RegexOptions.Compiled);

            MatchCollection mc = r.Matches(sourceString);
            for (int i = 0; i < mc.Count; i++)
            {
                if (!url.ContainsValue(mc[i].Result("$3")))
                {
                    url.Add(i, mc[i].Result("$3"));
                }
            }

            return url;
        }

        /// <summary>
        /// 获取一条SQL语句中的所参数
        /// </summary>
        /// <param name="sql">SQL语句</param>
        /// <returns></returns>
        public static ArrayList SqlParame(string sql)
        {
            ArrayList list = new ArrayList();
            Regex r = new Regex(@"@(?<x>[0-9a-zA-Z]*)", RegexOptions.IgnoreCase | RegexOptions.Compiled);
            MatchCollection mc = r.Matches(sql);
            for (int i = 0; i < mc.Count; i++)
            {
                list.Add(mc[i].Result("$1"));
            }

            return list;
        }

        /// <summary>
        /// 获取一条SQL语句中的所参数
        /// </summary>
        /// <param name="sql">SQL语句</param>
        /// <returns></returns>
        public static ArrayList OracleParame(string sql)
        {
            ArrayList list = new ArrayList();
            Regex r = new Regex(@":(?<x>[0-9a-zA-Z]*)", RegexOptions.IgnoreCase | RegexOptions.Compiled);
            MatchCollection mc = r.Matches(sql);
            for (int i = 0; i < mc.Count; i++)
            {
                list.Add(mc[i].Result("$1"));
            }

            return list;
        }

        /// <summary>
        /// 将HTML代码转化成纯文本
        /// </summary>
        /// <param name="sourceHTML">HTML代码</param>
        /// <returns></returns>
        public static string convertText(string sourceHTML)
        {
            string strResult = "";
            Regex r = new Regex("<(.*?)>", RegexOptions.IgnoreCase | RegexOptions.Compiled);
            MatchCollection mc = r.Matches(sourceHTML);

            if (mc.Count == 0)
            {
                strResult = sourceHTML;
            }
            else
            {
                strResult = sourceHTML;

                for (int i = 0; i < mc.Count; i++)
                {
                    strResult = strResult.Replace(mc[i].ToString(), "");
                }
            }

            return strResult;
        }
    #endregion

    #region 自定义处理

        


        /// <summary>
        /// 按照指定格式输出时间
        /// </summary>
        /// <param name="NowDate">时间</param>
        /// <param name="type">输出类型</param>
        /// <returns></returns>
        public static string WriteDate(string NowDate, int type)
        {
            double TimeZone = 0;
            DateTime NewDate = DateTime.Parse(NowDate).AddHours(TimeZone);
            string strResult = "";

            switch (type)
            {
                case 1:
                    strResult = NewDate.ToString();
                    break;
                case 2:
                    strResult = NewDate.ToShortDateString().ToString();
                    break;
                case 3:
                    strResult = NewDate.Year + "年" + NewDate.Month + "月" + NewDate.Day + "日 " + NewDate.Hour + "点" + NewDate.Minute + "分" + NewDate.Second + "秒";
                    break;
                case 4:
                    strResult = NewDate.Year + "年" + NewDate.Month + "月" + NewDate.Day + "日";
                    break;
                case 5:
                    strResult = NewDate.Year + "年" + NewDate.Month + "月" + NewDate.Day + "日 " + NewDate.Hour + "点" + NewDate.Minute + "分";
                    break;
                case 6:
                    strResult = NewDate.Year + "-" + NewDate.Month + "-" + NewDate.Day + "  " + NewDate.Hour + ":" + NewDate.Minute;
                    break;
                default:
                    strResult = NewDate.ToString();
                    break;
            }
            return strResult;
        }


        private static int Instr(string strA, string strB)
        {
            if (string.Compare(strA, strA.Replace(strB, "")) > 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }


        /// <summary>
        /// 判断客户端操作系统和浏览器的配置
        /// </summary>
        /// <param name="Info">客户端返回的头信息(Request.UserAgent)</param>
        /// <param name="Type">获取类型：1为操作系统， 2为浏览器</param>
        /// <returns></returns>
        public static string GetInfo(string Info, int Type)
        {

            string GetInfo = "";
            switch (Type)
            {
                case 1:
                    if (Instr(Info, @"NT 5.1") > 0)
                    {
                        GetInfo = "操作系统：Windows XP";
                    }
                    else if (Instr(Info, @"Tel") > 0)
                    {
                        GetInfo = "操作系统：Telport";
                    }
                    else if (Instr(Info, @"webzip") > 0)
                    {
                        GetInfo = "操作系统：操作系统：webzip";
                    }
                    else if (Instr(Info, @"flashget") > 0)
                    {
                        GetInfo = "操作系统：flashget";
                    }
                    else if (Instr(Info, @"offline") > 0)
                    {
                        GetInfo = "操作系统：offline";
                    }
                    else if (Instr(Info, @"NT 5") > 0)
                    {
                        GetInfo = "操作系统：Windows 2000";
                    }
                    else if (Instr(Info, @"NT 4") > 0)
                    {
                        GetInfo = "操作系统：Windows NT4";
                    }
                    else if (Instr(Info, @"98") > 0)
                    {
                        GetInfo = "操作系统：Windows 98";
                    }
                    else if (Instr(Info, @"95") > 0)
                    {
                        GetInfo = "操作系统：Windows 95";
                    }
                    else
                    {
                        GetInfo = "操作系统：未知";
                    }
                    break;
                case 2:
                    if (Instr(Info, @"NetCaptor 6.5.0") > 0)
                    {
                        GetInfo = "浏 览 器：NetCaptor 6.5.0";
                    }
                    else if (Instr(Info, @"MyIe 3.1") > 0)
                    {
                        GetInfo = "浏 览 器：MyIe 3.1";
                    }
                    else if (Instr(Info, @"NetCaptor 6.5.0RC1") > 0)
                    {
                        GetInfo = "浏 览 器：NetCaptor 6.5.0RC1";
                    }
                    else if (Instr(Info, @"NetCaptor 6.5.PB1") > 0)
                    {
                        GetInfo = "浏 览 器：NetCaptor 6.5.PB1";
                    }
                    else if (Instr(Info, @"MSIE 6.0b") > 0)
                    {
                        GetInfo = "浏 览 器：Internet Explorer 6.0b";
                    }
                    else if (Instr(Info, @"MSIE 6.0") > 0)
                    {
                        GetInfo = "浏 览 器：Internet Explorer 6.0";
                    }
                    else if (Instr(Info, @"MSIE 5.5") > 0)
                    {
                        GetInfo = "浏 览 器：Internet Explorer 5.5";
                    }
                    else if (Instr(Info, @"MSIE 5.01") > 0)
                    {
                        GetInfo = "浏 览 器：Internet Explorer 5.01";
                    }
                    else if (Instr(Info, @"MSIE 5.0") > 0)
                    {
                        GetInfo = "浏 览 器：Internet Explorer 5.0";
                    }
                    else if (Instr(Info, @"MSIE 4.0") > 0)
                    {
                        GetInfo = "浏 览 器：Internet Explorer 4.0";
                    }
                    else
                    {
                        GetInfo = "浏 览 器：未知";
                    }
                    break;
            }
            return GetInfo;
        }


        /// <summary>
        /// 获取服务器本机的MAC地址
        /// </summary>
        /// <returns></returns>
//         public static string GetMAC_Address()
//         {
//             string strResult = "";
// 
//             ManagementObjectSearcher query = new ManagementObjectSearcher("SELECT * FROM Win32_NetworkAdapterConfiguration");
//             ManagementObjectCollection queryCollection = query.Get();
//             foreach (ManagementObject mo in queryCollection)
//             {
//                 if (mo["IPEnabled"].ToString() == "True") strResult = mo["MacAddress"].ToString();
//             }
// 
//             return strResult;
//         }



        /// <summary>
        /// 转换文件路径中不规则字符
        /// </summary>
        /// <param name="path"></param>
        /// <returns>string</returns>
        public static string convertDirURL(string path)
        {
            return AddLast(path.Replace("/", "\\"), "\\");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="str"></param>
        /// <returns>string</returns>
        public static string convertXmlString(string str)
        {
            return "<![CDATA[" + str + "]]>";
        }

        /// <summary>
        /// 转换一个double型数字串为时间，起始 0 为 1970-01-01 08:00:00
        /// 原理就是，每过一秒就在这个数字串上累加一
        /// </summary>
        /// <param name="d">double 型数字</param>
        /// <returns>DateTime</returns>
        public static DateTime ConvertIntDateTime(double d)
        {
            DateTime time = DateTime.MinValue;

            DateTime startTime = DateTime.Parse("1970-01-01 08:00:00");

            time = startTime.AddSeconds(d);

            return time;
        }

        /// <summary>
        /// 转换时间为一个double型数字串，起始 0 为 1970-01-01 08:00:00
        /// 原理就是，每过一秒就在这个数字串上累加一
        /// </summary>
        /// <param name="time">时间</param>
        /// <returns>double</returns>
        public static double ConvertDateTimeInt(DateTime time)
        {
            double intResult = 0;

            DateTime startTime = DateTime.Parse("1970-01-01 08:00:00");

            intResult = (time - startTime).TotalSeconds;

            return intResult;
        }


        /// <summary>
        /// 获取一个URL中引用的文件名称（包括后缀符）
        /// </summary>
        /// <param name="url">URL地址</param>
        /// <returns>string</returns>
        public static string GetFileName(string url)
        {
            //string[] Name = StringsFunction.SplitArray(url,'/');
            //return Name[Name.Length - 1];

            return System.IO.Path.GetFileName(url);
        }

        /// <summary>
        /// 检测某一字符串的第一个字符是否与指定的
        /// 字符一致，否则在该字符串前加上这个字符
        /// </summary>
        /// <param name="Strings">字符串</param>
        /// <param name="Str">字符</param>
        /// <returns>返回 string</returns>
        public static string AddFirst(string Strings, string Str)
        {
            string strResult = "";
            if (Strings.StartsWith(Str))
            {
                strResult = Strings;
            }
            else
            {
                strResult = String.Concat(Str, Strings);
            }
            return strResult;
        }


        /// <summary>
        /// 检测某一字符串的最后一个字符是否与指定的
        /// 字符一致，否则在该字符串末尾加上这个字符
        /// </summary>
        /// <param name="Strings">字符串</param>
        /// <param name="Str">字符</param>
        /// <returns>返回 string</returns>
        public static string AddLast(string Strings, string Str)
        {
            string strResult = "";
            if (Strings.EndsWith(Str))
            {
                strResult = Strings;
            }
            else
            {
                strResult = String.Concat(Strings, Str);
            }
            return strResult;
        }

        /// <summary>
        /// 检测某一字符串的第一个字符是否与指定的
        /// 字符一致，相同则去掉这个字符
        /// </summary>
        /// <param name="Strings">字符串</param>
        /// <param name="Str">字符</param>
        /// <returns>返回 string</returns>
        public static string DelFirst(string Strings, string Str)
        {
            string strResult = "";
            if (Strings.Length == 0) throw new Exception("原始字符串长度为零");

            if (Strings.StartsWith(Str))
            {
                strResult = Strings.Substring(Str.Length, Strings.Length - 1);
            }
            else
            {
                strResult = Strings;
            }

            return strResult;
        }

        /// <summary>
        /// 检测某一字符串的最后一个字符是否与指定的
        /// 字符一致，相同则去掉这个字符
        /// </summary>
        /// <param name="Strings">字符串</param>
        /// <param name="Str">字符</param>
        /// <returns>返回 string</returns>
        public static string DelLast(string Strings, string Str)
        {
            string strResult = "";

            if (Strings.EndsWith(Str))
            {
                strResult = Strings.Substring(0, Strings.Length - Str.Length);
            }
            else
            {
                strResult = Strings;
            }

            return strResult;
        }


        
        /// <summary>
        /// 对字符串进行 HTML 编码操作
        /// </summary>
        /// <param name="str">字符串</param>
        /// <returns></returns>
        public static string strEncode(string str)
        {
            str = str.Replace("&", "&amp;");
            str = str.Replace("'", "''");
            str = str.Replace("\"", "&quot;");
            str = str.Replace(" ", "&nbsp;");
            str = str.Replace("<", "&lt;");
            str = str.Replace(">", "&gt;");
            str = str.Replace("\r", "<br_wf>");
            str = str.Replace("\n", "<br>");
            return str;
        }


        /// <summary>
        /// 对 HTML 字符串进行解码操作
        /// </summary>
        /// <param name="str">字符串</param>
        /// <returns></returns>
        public static string strDecode(string str)
        {
            str = str.Replace("<br_wf>", "\r");
            str = str.Replace("<br>", "\n");
            str = str.Replace("&gt;", ">");
            str = str.Replace("&lt;", "<");
            str = str.Replace("&nbsp;", " ");
            str = str.Replace("&quot;", "\"");
            str = str.Replace("&amp;", "&");
            return str;
        }

        /// <summary>
        /// 对脚本程序进行处理
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static string convertScript(string str)
        {
            string strResult = "";
            if (str != "")
            {
                StringReader sr = new StringReader(str);
                string rl;
                do
                {
                    strResult += sr.ReadLine();
                } while ((rl = sr.ReadLine()) != null);
            }

            strResult = strResult.Replace("\"", "&quot;");

            return strResult;
        }


        /// <summary>
        /// 将一个字符串以某一特定字符分割成字符串数组
        /// </summary>
        /// <param name="Strings">字符串</param>
        /// <param name="str">分割字符</param>
        /// <returns>string[]</returns>
        public static string[] SplitArray(string Strings, char str)
        {
            string[] strArray = Strings.Trim().Split(new char[] { str });

            return strArray;
        }

        /*
                /// <summary>
                /// 将一个字符串以某一字符分割成数组
                /// </summary>
                /// <param name="Strings">字符串</param>
                /// <param name="str">分割字符</param>
                /// <returns>string[]</returns>
                public static string[] SplitArray(string Strings, string str)
                {
                    Regex r = new Regex(str);
                    string[] strArray = r.Split(Strings.Trim());

                    return strArray;
                }

        */

        /// <summary>
        /// 检测一个字符串，是否存在于一个以固定分割符分割的字符串中
        /// </summary>
        /// <param name="str">字符串</param>
        /// <param name="Strings">固定分割符分割的字符串</param>
        /// <param name="Str">分割符</param>
        /// <returns></returns>
        public static bool InArray(string str, string Strings, char Str)
        {
            bool blResult = false;

            string[] array = SplitArray(Strings, Str);
            for (int i = 0; i < array.Length; i++)
            {
                if (str == array[i])
                {
                    blResult = true;
                    break;
                }
            }

            return blResult;
        }

        /*
                /// <summary>
                /// 检测一个字符串，是否存在于一个以固定分割符分割的字符串中
                /// </summary>
                /// <param name="str">字符串</param>
                /// <param name="Strings">固定分割符分割的字符串</param>
                /// <param name="Str">分割符</param>
                /// <returns></returns>
                public static bool InArray(string str, string Strings, string Str)
                {
                    bool blResult = false;

                    string[] array = SplitArray(Strings, Str);
                    for(int i = 0; i < array.Length; i++)
                    {
                        if(str == array[i])
                        {
                            blResult = true;
                            break;
                        }
                    }

                    return blResult;
                }
                */

        /// <summary>
        /// 检测一个字符串，是否存在于一个以固定分割符分割的字符串中
        /// </summary>
        /// <param name="str">字符串</param>
        /// <param name="array">字符串数组</param>
        /// <returns></returns>
        public static bool InArray(string str, string[] array)
        {
            bool blResult = false;

            for (int i = 0; i < array.Length; i++)
            {
                if (str == array[i])
                {
                    blResult = true;
                    break;
                }
            }

            return blResult;
        }


        /// <summary>
        /// 检测值是否有效，为 null 或 "" 均为无效
        /// </summary>
        /// <param name="obj">要检测的值</param>
        /// <returns></returns>
        public static bool CheckValiable(object obj)
        {
            if (Object.Equals(obj, null) || Object.Equals(obj, string.Empty))
                return false;
            else
                return true;
        }
    #endregion

    #region 对字符串的加密/解密

        
        


        /// <summary>
        /// 对字符串进行加密
        /// </summary>
        /// <param name="Passowrd">待加密的字符串</param>
        /// <returns>string</returns>
//         public static string Encrypt(string Passowrd)
//         {
//             string strResult = "";
// 
//             FormsAuthenticationTicket ticket = new FormsAuthenticationTicket(Passowrd, true, 2);
//             strResult = FormsAuthentication.Encrypt(ticket).ToString();
// 
//             return strResult;
//         }


        

    #endregion

    #region 构造获取分页操作SQL语句

    #region ConstructSplitSQL
        /// <summary>
        /// 获取分页操作SQL语句(对于排序的字段必须建立索引，优化分页提取方式)
        /// </summary>
        /// <param name="tblName">操作表名称</param>
        /// <param name="fldName">排序的索引字段</param>
        /// <param name="PageIndex">当前页</param>
        /// <param name="PageSize">每页显示记录数</param>
        /// <param name="totalRecord">总记录数</param>
        /// <param name="OrderType">排序方式(0升序，1为降序)</param>
        /// <param name="strWhere">检索的条件语句，不需要再加WHERE关键字</param>
        /// <returns></returns>
        public static string ConstructSplitSQL(string tblName,
                                                string fldName,
                                                int PageIndex,
                                                int PageSize,
                                                int totalRecord,
                                                int OrderType,
                                                string strWhere)
        {
            string strSQL = "";
            string strOldWhere = "";
            string rtnFields = "*";

            // 构造检索条件语句字符串
            if (strWhere != "")
            {
                // 去除不合法的字符，防止SQL注入式攻击
                strWhere = strWhere.Replace("'", "''");
                strWhere = strWhere.Replace("--", "");
                strWhere = strWhere.Replace(";", "");

                strOldWhere = " AND " + strWhere + " ";

                strWhere = " WHERE " + strWhere + " ";
            }

            // 升序操作
            if (OrderType == 0)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    //strSQL += "WHERE (" + fldName + " >= ( SELECT MAX(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                    strSQL += strWhere + "ORDER BY " + fldName + " ASC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " > ( SELECT MAX(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                }
            }
            // 降序操作
            else if (OrderType == 1)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    //strSQL += "WHERE (" + fldName + " <= ( SELECT MIN(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                    strSQL += strWhere + "ORDER BY " + fldName + " DESC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " < ( SELECT MIN(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                }
            }
            else // 异常处理
            {
                throw new DataException("未指定任何排序类型。0升序，1为降序");
            }

            return strSQL;
        }


        /// <summary>
        /// 获取分页操作SQL语句(对于排序的字段必须建立索引)
        /// </summary>
        /// <param name="tblName">操作表名</param>
        /// <param name="fldName">操作索引字段名称</param>
        /// <param name="PageIndex">当前页</param>
        /// <param name="PageSize">每页显示记录数</param>
        /// <param name="rtnFields">返回字段集合，中间用逗号格开。返回全部用“*”</param>
        /// <param name="OrderType">排序方式(0升序，1为降序)</param>
        /// <param name="strWhere">检索的条件语句，不需要再加WHERE关键字</param>
        /// <returns></returns>
        public static string ConstructSplitSQL(string tblName,
                                                string fldName,
                                                int PageIndex,
                                                int PageSize,
                                                string rtnFields,
                                                int OrderType,
                                                string strWhere)
        {
            string strSQL = "";
            string strOldWhere = "";

            // 构造检索条件语句字符串
            if (strWhere != "")
            {
                // 去除不合法的字符，防止SQL注入式攻击
                strWhere = strWhere.Replace("'", "''");
                strWhere = strWhere.Replace("--", "");
                strWhere = strWhere.Replace(";", "");

                strOldWhere = " AND " + strWhere + " ";

                strWhere = " WHERE " + strWhere + " ";
            }

            // 升序操作
            if (OrderType == 0)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    //strSQL += "WHERE (" + fldName + " >= ( SELECT MAX(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                    strSQL += strWhere + "ORDER BY " + fldName + " ASC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " > ( SELECT MAX(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                }
            }
            // 降序操作
            else if (OrderType == 1)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    //strSQL += "WHERE (" + fldName + " <= ( SELECT MIN(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                    strSQL += strWhere + "ORDER BY " + fldName + " DESC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " < ( SELECT MIN(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                }
            }
            else // 异常处理
            {
                throw new DataException("未指定任何排序类型。0升序，1为降序");
            }

            return strSQL;
        }


        /// <summary>
        /// 获取分页操作SQL语句(对于排序的字段必须建立索引)
        /// </summary>
        /// <param name="tblName">操作表名</param>
        /// <param name="fldName">操作索引字段名称</param>
        /// <param name="unionCondition">用于连接的条件，例如: LEFT JOIN UserInfo u ON (u.UserID = b.UserID)</param>
        /// <param name="PageIndex">当前页</param>
        /// <param name="PageSize">每页显示记录数</param>
        /// <param name="rtnFields">返回字段集合，中间用逗号格开。返回全部用“*”</param>
        /// <param name="OrderType">排序方式，0升序，1为降序</param>
        /// <param name="strWhere">检索的条件语句，不需要再加WHERE关键字</param>
        /// <returns></returns>
        public static string ConstructSplitSQL(string tblName,
            string fldName,
            string unionCondition,
            int PageIndex,
            int PageSize,
            string rtnFields,
            int OrderType,
            string strWhere)
        {
            string strSQL = "";
            string strOldWhere = "";

            // 构造检索条件语句字符串
            if (strWhere != "")
            {
                // 去除不合法的字符，防止SQL注入式攻击
                strWhere = strWhere.Replace("'", "''");
                strWhere = strWhere.Replace("--", "");
                strWhere = strWhere.Replace(";", "");

                strOldWhere = " AND " + strWhere + " ";

                strWhere = " WHERE " + strWhere + " ";
            }

            // 升序操作
            if (OrderType == 0)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + unionCondition + " ";

                    //strSQL += "WHERE (" + fldName + " >= ( SELECT MAX(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                    strSQL += strWhere + "ORDER BY " + fldName + " ASC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + unionCondition + " ";

                    strSQL += "WHERE (" + fldName + " > ( SELECT MAX(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                }
            }
            // 降序操作
            else if (OrderType == 1)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + unionCondition + " ";

                    //strSQL += "WHERE (" + fldName + " <= ( SELECT MIN(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                    strSQL += strWhere + "ORDER BY " + fldName + " DESC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + unionCondition + " ";

                    strSQL += "WHERE (" + fldName + " < ( SELECT MIN(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                }
            }
            else // 异常处理
            {
                throw new DataException("未指定任何排序类型。0升序，1为降序");
            }

            return strSQL;
        }
    #endregion


    #region ConstructSplitSQL_TOP


        /// <summary>
        /// 获取分页操作SQL语句(对于排序的字段必须建立索引)
        /// </summary>
        /// <param name="tblName">操作表名</param>
        /// <param name="fldName">操作索引字段名称</param>
        /// <param name="PageIndex">当前页</param>
        /// <param name="PageSize">每页显示记录数</param>
        /// <param name="rtnFields">返回字段集合，中间用逗号格开。返回全部用“*”</param>
        /// <param name="OrderType">排序方式(0升序，1为降序)</param>
        /// <param name="strWhere">检索的条件语句，不需要再加WHERE关键字</param>
        /// <returns></returns>
        public static string ConstructSplitSQL_TOP(string tblName,
                                                    string fldName,
                                                    int PageIndex,
                                                    int PageSize,
                                                    string rtnFields,
                                                    int OrderType,
                                                    string strWhere)
        {
            string strSQL = "";
            string strOldWhere = "";

            // 构造检索条件语句字符串
            if (strWhere != "")
            {
                // 去除不合法的字符，防止SQL注入式攻击
                strWhere = strWhere.Replace("'", "''");
                strWhere = strWhere.Replace("--", "");
                strWhere = strWhere.Replace(";", "");

                strOldWhere = " AND " + strWhere + " ";

                strWhere = " WHERE " + strWhere + " ";
            }

            // 升序操作
            if (OrderType == 0)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += strWhere + " ORDER BY " + fldName + " ASC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " > ( SELECT MAX(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                }
            }
            // 降序操作
            else if (OrderType == 1)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += strWhere + " ORDER BY " + fldName + " DESC";
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " < ( SELECT MIN(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                }
            }
            else // 异常处理
            {
                throw new DataException("未指定任何排序类型。0升序，1为降序");
            }

            return strSQL;
        }

    #endregion


    #region ConstructSplitSQL_sort(指定排序的表达式)

        /// <summary>
        /// 获取分页操作SQL语句(对于排序的字段必须建立索引)
        /// </summary>
        /// <param name="tblName">操作表名</param>
        /// <param name="fldName">操作索引字段名称</param>
        /// <param name="PageIndex">当前页</param>
        /// <param name="PageSize">每页显示记录数</param>
        /// <param name="rtnFields">返回字段集合，中间用逗号格开。返回全部用“*”</param>
        /// <param name="OrderType">排序方式(0升序，1为降序)</param>
        /// <param name="sort">排序表达式</param>
        /// <param name="strWhere">检索的条件语句，不需要再加WHERE关键字</param>
        /// <returns></returns>
        public static string ConstructSplitSQL_sort(string tblName,
            string fldName,
            int PageIndex,
            int PageSize,
            string rtnFields,
            int OrderType,
            string sort,
            string strWhere)
        {
            string strSQL = "";
            string strOldWhere = "";

            // 构造检索条件语句字符串
            if (strWhere != "")
            {
                // 去除不合法的字符，防止SQL注入式攻击
                strWhere = strWhere.Replace("'", "''");
                strWhere = strWhere.Replace("--", "");
                strWhere = strWhere.Replace(";", "");

                strOldWhere = " AND " + strWhere + " ";

                strWhere = " WHERE " + strWhere + " ";
            }

            if (sort != "") sort = " ORDER BY " + sort;

            // 升序操作
            if (OrderType == 0)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    //strSQL += "WHERE (" + fldName + " >= ( SELECT MAX(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " ASC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " ASC";
                    strSQL += strWhere + sort;
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " > ( SELECT MAX(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + sort + " ) AS T )) ";

                    strSQL += strOldWhere + sort;
                }
            }
            // 降序操作
            else if (OrderType == 1)
            {
                if (PageIndex == 1)
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    //strSQL += "WHERE (" + fldName + " <= ( SELECT MIN(" + fldName + ") FROM (SELECT TOP 1 " + fldName + " FROM " + tblName + strWhere + " ORDER BY " + fldName + " DESC ) AS T )) ";

                    //strSQL += strOldWhere + "ORDER BY " + fldName + " DESC";
                    strSQL += strWhere + sort;
                }
                else
                {
                    strSQL += "SELECT TOP " + PageSize + " " + rtnFields + " FROM " + tblName + " ";

                    strSQL += "WHERE (" + fldName + " < ( SELECT MIN(" + fldName + ") FROM (SELECT TOP " + ((PageIndex - 1) * PageSize) + " " + fldName + " FROM " + tblName + strWhere + sort + " ) AS T )) ";

                    strSQL += strOldWhere + sort;
                }
            }
            else // 异常处理
            {
                throw new DataException("未指定主索引排序类型。0升序，1为降序");
            }

            return strSQL;
        }

    #endregion

    #endregion

    #region "字符串的处理函数集"

        /// <summary>
        /// 字符串的处理函数集
        /// </summary>
//         public class Str
//         {
            /// <summary>
            /// 将 Stream 转化成 string
            /// </summary>
            /// <param name="s">Stream流</param>
            /// <returns>string</returns>
            public static string ConvertStreamToString(Stream s)
            {
                string strResult = "";
                StreamReader sr = new StreamReader(s, Encoding.UTF8);

                Char[] read = new Char[256];

                // Read 256 charcters at a time.    
                int count = sr.Read(read, 0, 256);

                while (count > 0)
                {
                    // Dump the 256 characters on a string and display the string onto the console.
                    string str = new String(read, 0, count);
                    strResult += str;
                    count = sr.Read(read, 0, 256);
                }


                // 释放资源
                sr.Close();

                return strResult;
            }
            public static byte[] strToHexByte(string hexString,string spilt)
            {                
                if(spilt!="")
                    hexString = hexString.Replace(spilt, "");
                if ((hexString.Length % 2) != 0)
                    hexString += " ";
                byte[] returnBytes = new byte[hexString.Length / 2];
                for (int i = 0; i < returnBytes.Length; i++)
                    returnBytes[i] = Convert.ToByte(hexString.Substring(i * 2, 2), 16);
                return returnBytes;
            }
            public static string byteToHexStr(byte[] bytes, string spilt)
            {
                return byteToHexStr(bytes, 0, bytes.Length, spilt);
            }
            public static string byteToHexStr(byte[] bytes,int offset,int count, string spilt)
            {
                StringBuilder strB = new StringBuilder();
                if (bytes != null)
                {
                    for (int i = 0; i < count;i++ )
                    {
                        strB.Append(bytes[offset+i].ToString("X2"));
                        strB.Append(spilt);
                    }
                }
                return strB.ToString();
            }
            public static string byteToHexStr(List<byte> bytes,string spilt)
            {
                StringBuilder strB = new StringBuilder();
                if (bytes != null)
                {
                    foreach (byte b in bytes)
                    {
                        strB.Append(b.ToString("X2"));
                        strB.Append(spilt);
                    }                    
                }
                return strB.ToString();
            }

        //
            public static ushort[] strToHexushort(string hexString, string spilt)
            {
                if (spilt != "")
                    hexString = hexString.Replace(spilt, "");
                if ((hexString.Length % 4) != 0)
                    hexString += " ";
                ushort[] returnBytes = new ushort[hexString.Length / 4];
                for (int i = 0; i < returnBytes.Length; i++)
                    returnBytes[i] = BytesOP.MakeShort(Convert.ToByte(hexString.Substring(4 * i, 2), 16), Convert.ToByte(hexString.Substring((4 * i) + 2, 2), 16));
                return returnBytes;
            }
            public static string ushortToHexStr(ushort[] bytes, string spilt)
            {
                return ushortToHexStr(bytes, 0, bytes.Length, spilt);
            }
            public static string ushortToHexStr(ushort[] bytes, int offset, int count, string spilt)
            {
                StringBuilder strB = new StringBuilder();
                if (bytes != null)
                {
                    for (int i = 0; i < count;i++ )
                    {
                        strB.Append(BytesOP.GetHighByte(bytes[offset+i]).ToString("X2"));
                        strB.Append(spilt);
                        strB.Append(BytesOP.GetLowByte(bytes[offset + i]).ToString("X2"));
                        strB.Append(spilt);
                    }
                }
                return strB.ToString();
            }
            public static string ushortToHexStr(List<ushort> bytes, string spilt)
            {
                StringBuilder strB = new StringBuilder();
                if (bytes != null)
                {
                    foreach (byte b in bytes)
                    {
                        strB.Append(BytesOP.GetHighByte(b).ToString("X2"));
                        strB.Append(spilt);
                        strB.Append(BytesOP.GetLowByte(b).ToString("X2"));
                        strB.Append(spilt);
                    }                    
                }
                return strB.ToString();
            }

            /// <summary>
            /// 返回字符串的字节数组，一个汉字字符相当于两个单位长度(使用Encoding类)
            /// </summary>
            /// <param name="str">指定字符串</param>
            /// <returns></returns>
            public static byte[] getByteList_gb2312(string str)
            {
                Encoding gb2312 = Encoding.GetEncoding("gb2312");
                return gb2312.GetBytes(str);
            }

            /// <summary>
            /// 由字符串的字节数组返回字符串，一个汉字字符相当于两个单位长度(使用Encoding类)
            /// </summary>
            /// <param name="bytes">指定字符串字节数组</param>
            /// <param name="index">第一个要解码的字节的索引</param>
            /// <param name="count">要解码的字节数</param>
            /// <returns></returns>
            public static string getString_gb2312(byte[] bytes, int index, int count)
            {
                Encoding gb2312 = Encoding.GetEncoding("gb2312");
                return gb2312.GetString(bytes, index, count);
            }
            /// <summary>
            /// 对传递的参数字符串进行处理，防止注入式攻击
            /// </summary>
            /// <param name="str">传递的参数字符串</param>
            /// <returns>String</returns>
            public static string ConvertSql(string str)
            {
                str = str.Trim();
                str = str.Replace("'", "''");
                str = str.Replace(";--", "");
                str = str.Replace("=", "");
                str = str.Replace(" or ", "");
                str = str.Replace(" and ", "");

                return str;
            }


            /// <summary>
            /// 格式化占用空间大小的输出
            /// </summary>
            /// <param name="size">大小</param>
            /// <returns>返回 String</returns>
            public static string FormatNUM(long size)
            {
                decimal NUM;
                string strResult;

                if (size > 1073741824)
                {
                    NUM = (Convert.ToDecimal(size) / Convert.ToDecimal(1073741824));
                    strResult = NUM.ToString("N") + " M";
                }
                else if (size > 1048576)
                {
                    NUM = (Convert.ToDecimal(size) / Convert.ToDecimal(1048576));
                    strResult = NUM.ToString("N") + " M";
                }
                else if (size > 1024)
                {
                    NUM = (Convert.ToDecimal(size) / Convert.ToDecimal(1024));
                    strResult = NUM.ToString("N") + " KB";
                }
                else
                {
                    strResult = size + " 字节";
                }

                return strResult;
            }

            /// <summary>
            /// 判断字符串是否为有效的邮件地址
            /// </summary>
            /// <param name="email"></param>
            /// <returns></returns>
            public static bool IsValidEmail(string email)
            {
                return Regex.IsMatch(email, @"^.+\@(\[?)[a-zA-Z0-9\-\.]+\.([a-zA-Z]{2,3}|[0-9]{1,3})(\]?)$");
            }

            /// <summary>
            /// 判断字符串是否为有效的URL地址
            /// </summary>
            /// <param name="url"></param>
            /// <returns></returns>
            public static bool IsValidURL(string url)
            {
                return Regex.IsMatch(url, @"^(http|https|ftp)\://[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(:[a-zA-Z0-9]*)?/?([a-zA-Z0-9\-\._\?\,\'/\\\+&%\$#\=~])*[^\.\,\)\(\s]$");
            }

            /// <summary>
            /// 判断字符串是否为Int类型的
            /// </summary>
            /// <param name="val"></param>
            /// <returns></returns>
            public static bool IsValidInt(string val)
            {
                return Regex.IsMatch(val, @"^[1-9]\d*\.?[0]*$");
            }

            /// <summary>
            /// 检测字符串是否全为正整数
            /// </summary>
            /// <param name="str"></param>
            /// <returns></returns>
            public static bool IsNum(string str)
            {
                bool blResult = true;//默认状态下是数字

                if (str == "")
                    blResult = false;
                else
                {
                    foreach (char Char in str)
                    {
                        if (!char.IsNumber(Char))
                        {
                            blResult = false;
                            break;
                        }
                    }
                    if (blResult)
                    {
                        if (int.Parse(str) == 0)
                            blResult = false;
                    }
                }
                return blResult;
            }

            /// <summary>
            /// 检测字符串是否全为数字型
            /// </summary>
            /// <param name="str"></param>
            /// <returns></returns>
            public static bool IsDouble(string str)
            {
                bool blResult = true;//默认状态下是数字

                if (str == "")
                    blResult = false;
                else
                {
                    foreach (char Char in str)
                    {
                        if (!char.IsNumber(Char) && Char.ToString() != "-")
                        {
                            blResult = false;
                            break;
                        }
                    }
                }
                return blResult;
            }

            /// <summary>
            /// 输出由同一字符组成的指定长度的字符串
            /// </summary>
            /// <param name="Char">输出字符，如：A</param>
            /// <param name="i">指定长度</param>
            /// <returns></returns>
            public static string Strings(char Char, int i)
            {
                string strResult = null;

                for (int j = 0; j < i; j++)
                {
                    strResult += Char;
                }
                return strResult;
            }           


            /// <summary>
            /// 以日期为标准获得一个绝对的名称
            /// </summary>
            /// <returns>返回 String</returns>
            public static string MakeName()
            {
                /*
                string y = DateTime.Now.Year.ToString();
                string m = DateTime.Now.Month.ToString();
                string d = DateTime.Now.Day.ToString();
                string h = DateTime.Now.Hour.ToString();
                string n = DateTime.Now.Minute.ToString();
                string s = DateTime.Now.Second.ToString();
                return y + m + d + h + n + s;
                */

                return DateTime.Now.ToString("yyMMddHHmmss");
            }

            /// <summary>
            /// 返回字符串的真实长度，一个汉字字符相当于两个单位长度
            /// </summary>
            /// <param name="str">指定字符串</param>
            /// <returns></returns>
            public static int Len(string str)
            {
                int intResult = 0;

                foreach (char Char in str)
                {
                    if ((int)Char > 127)
                        intResult += 2;
                    else
                        intResult++;
                }
                return intResult;
            }
            /// <summary>
            /// 返回字符串的真实长度，一个汉字字符相当于两个单位长度(使用Encoding类)
            /// </summary>
            /// <param name="str">指定字符串</param>
            /// <returns></returns>
            public static int getLen(string str,Encoding encoding)
            {
                int intResult = 0;
                byte[] bytes = encoding.GetBytes(str);
                intResult = bytes.Length;
                return intResult;
            }
            public static int getLen(string str)
            {
                return getLen(str, Encoding.GetEncoding("gb2312"));                
            }

            /// <summary>
            /// 按照字符串的实际长度截取指定长度的字符串
            /// </summary>
            /// <param name="str">字符串</param>
            /// <param name="Length">指定长度</param>
            /// <returns></returns>
            public static string CutLen(string str, int Length)
            {
                int i = 0, j = 0;

                foreach (char Char in str)
                {
                    if ((int)Char > 127)
                        i += 2;
                    else
                        i++;

                    if (i > Length)
                    {
                        str = str.Substring(0, j - 2) + "...";
                        break;
                    }
                    j++;
                }
                return str;
            }


            /// <summary>
            /// 获取指定长度的纯数字随机数字串
            /// </summary>
            /// <param name="intLong">数字串长度</param>
            /// <returns>字符串</returns>
            public static string RandomNUM(int intLong)
            {
                string strResult = "";

                Random r = new Random();
                for (int i = 0; i < intLong; i++)
                {
                    strResult = strResult + r.Next(10);
                }

                return strResult;
            }

            /// <summary>
            /// 获取一个由26个小写字母组成的指定长度的随即字符串
            /// </summary>
            /// <param name="intLong">指定长度</param>
            /// <returns></returns>
            public static string RandomSTR(int intLong)
            {
                string strResult = "";
                string[] array = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };

                Random r = new Random();

                for (int i = 0; i < intLong; i++)
                {
                    strResult += array[r.Next(26)];
                }

                return strResult;
            }

            /// <summary>
            /// 获取一个由数字和26个小写字母组成的指定长度的随即字符串
            /// </summary>
            /// <param name="intLong">指定长度</param>
            /// <returns></returns>
            public static string RandomNUMSTR(int intLong)
            {
                string strResult = "";
                string[] array = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" };

                Random r = new Random();

                for (int i = 0; i < intLong; i++)
                {
                    strResult += array[r.Next(36)];
                }

                return strResult;
            }



            /// <summary>
            /// 将指定字符串中的汉字转换为拼音首字母的缩写，其中非汉字保留为原字符
            /// </summary>
            /// <param name="text"></param>
            /// <returns></returns>
            public static string ConvertSpellFirst(string text)
            {
                char pinyin;
                byte[] array;
                StringBuilder sb = new StringBuilder(text.Length);
                foreach (char c in text)
                {
                    pinyin = c;
                    array = Encoding.Default.GetBytes(new char[] { c });

                    if (array.Length == 2)
                    {
                        int i = array[0] * 0x100 + array[1];

                        if (i < 0xB0A1) pinyin = c;
                        else
                            if (i < 0xB0C5) pinyin = 'a';
                            else
                                if (i < 0xB2C1) pinyin = 'b';
                                else
                                    if (i < 0xB4EE) pinyin = 'c';
                                    else
                                        if (i < 0xB6EA) pinyin = 'd';
                                        else
                                            if (i < 0xB7A2) pinyin = 'e';
                                            else
                                                if (i < 0xB8C1) pinyin = 'f';
                                                else
                                                    if (i < 0xB9FE) pinyin = 'g';
                                                    else
                                                        if (i < 0xBBF7) pinyin = 'h';
                                                        else
                                                            if (i < 0xBFA6) pinyin = 'g';
                                                            else
                                                                if (i < 0xC0AC) pinyin = 'k';
                                                                else
                                                                    if (i < 0xC2E8) pinyin = 'l';
                                                                    else
                                                                        if (i < 0xC4C3) pinyin = 'm';
                                                                        else
                                                                            if (i < 0xC5B6) pinyin = 'n';
                                                                            else
                                                                                if (i < 0xC5BE) pinyin = 'o';
                                                                                else
                                                                                    if (i < 0xC6DA) pinyin = 'p';
                                                                                    else
                                                                                        if (i < 0xC8BB) pinyin = 'q';
                                                                                        else
                                                                                            if (i < 0xC8F6) pinyin = 'r';
                                                                                            else
                                                                                                if (i < 0xCBFA) pinyin = 's';
                                                                                                else
                                                                                                    if (i < 0xCDDA) pinyin = 't';
                                                                                                    else
                                                                                                        if (i < 0xCEF4) pinyin = 'w';
                                                                                                        else
                                                                                                            if (i < 0xD1B9) pinyin = 'x';
                                                                                                            else
                                                                                                                if (i < 0xD4D1) pinyin = 'y';
                                                                                                                else
                                                                                                                    if (i < 0xD7FA) pinyin = 'z';
                    }

                    sb.Append(pinyin);
                }

                return sb.ToString();
            }


            /// <summary>
            /// 将指定字符串中的汉字转换为拼音字母，其中非汉字保留为原字符
            /// </summary>
            /// <param name="text">要转换的文本内容</param>
            /// <returns>string</returns>
            public static string ConvertSpellFull(string text)
            {
                #region 初始化定义
                int[] pyvalue = new int[]{-20319,-20317,-20304,-20295,-20292,-20283,-20265,-20257,-20242,-20230,-20051,-20036,-20032,-20026,
												   -20002,-19990,-19986,-19982,-19976,-19805,-19784,-19775,-19774,-19763,-19756,-19751,-19746,-19741,-19739,-19728,
												   -19725,-19715,-19540,-19531,-19525,-19515,-19500,-19484,-19479,-19467,-19289,-19288,-19281,-19275,-19270,-19263,
												   -19261,-19249,-19243,-19242,-19238,-19235,-19227,-19224,-19218,-19212,-19038,-19023,-19018,-19006,-19003,-18996,
												   -18977,-18961,-18952,-18783,-18774,-18773,-18763,-18756,-18741,-18735,-18731,-18722,-18710,-18697,-18696,-18526,
												   -18518,-18501,-18490,-18478,-18463,-18448,-18447,-18446,-18239,-18237,-18231,-18220,-18211,-18201,-18184,-18183,
												   -18181,-18012,-17997,-17988,-17970,-17964,-17961,-17950,-17947,-17931,-17928,-17922,-17759,-17752,-17733,-17730,
												   -17721,-17703,-17701,-17697,-17692,-17683,-17676,-17496,-17487,-17482,-17468,-17454,-17433,-17427,-17417,-17202,
												   -17185,-16983,-16970,-16942,-16915,-16733,-16708,-16706,-16689,-16664,-16657,-16647,-16474,-16470,-16465,-16459,
												   -16452,-16448,-16433,-16429,-16427,-16423,-16419,-16412,-16407,-16403,-16401,-16393,-16220,-16216,-16212,-16205,
												   -16202,-16187,-16180,-16171,-16169,-16158,-16155,-15959,-15958,-15944,-15933,-15920,-15915,-15903,-15889,-15878,
												   -15707,-15701,-15681,-15667,-15661,-15659,-15652,-15640,-15631,-15625,-15454,-15448,-15436,-15435,-15419,-15416,
												   -15408,-15394,-15385,-15377,-15375,-15369,-15363,-15362,-15183,-15180,-15165,-15158,-15153,-15150,-15149,-15144,
												   -15143,-15141,-15140,-15139,-15128,-15121,-15119,-15117,-15110,-15109,-14941,-14937,-14933,-14930,-14929,-14928,
												   -14926,-14922,-14921,-14914,-14908,-14902,-14894,-14889,-14882,-14873,-14871,-14857,-14678,-14674,-14670,-14668,
												   -14663,-14654,-14645,-14630,-14594,-14429,-14407,-14399,-14384,-14379,-14368,-14355,-14353,-14345,-14170,-14159,
												   -14151,-14149,-14145,-14140,-14137,-14135,-14125,-14123,-14122,-14112,-14109,-14099,-14097,-14094,-14092,-14090,
												   -14087,-14083,-13917,-13914,-13910,-13907,-13906,-13905,-13896,-13894,-13878,-13870,-13859,-13847,-13831,-13658,
												   -13611,-13601,-13406,-13404,-13400,-13398,-13395,-13391,-13387,-13383,-13367,-13359,-13356,-13343,-13340,-13329,
												   -13326,-13318,-13147,-13138,-13120,-13107,-13096,-13095,-13091,-13076,-13068,-13063,-13060,-12888,-12875,-12871,
												   -12860,-12858,-12852,-12849,-12838,-12831,-12829,-12812,-12802,-12607,-12597,-12594,-12585,-12556,-12359,-12346,
												   -12320,-12300,-12120,-12099,-12089,-12074,-12067,-12058,-12039,-11867,-11861,-11847,-11831,-11798,-11781,-11604,
												   -11589,-11536,-11358,-11340,-11339,-11324,-11303,-11097,-11077,-11067,-11055,-11052,-11045,-11041,-11038,-11024,
												   -11020,-11019,-11018,-11014,-10838,-10832,-10815,-10800,-10790,-10780,-10764,-10587,-10544,-10533,-10519,-10331,
												   -10329,-10328,-10322,-10315,-10309,-10307,-10296,-10281,-10274,-10270,-10262,-10260,-10256,-10254};

                string[] pystr = new string[]{"a","ai","an","ang","ao","ba","bai","ban","bang","bao","bei","ben","beng","bi","bian","biao",
												   "bie","bin","bing","bo","bu","ca","cai","can","cang","cao","ce","ceng","cha","chai","chan","chang","chao","che","chen",
												   "cheng","chi","chong","chou","chu","chuai","chuan","chuang","chui","chun","chuo","ci","cong","cou","cu","cuan","cui",
												   "cun","cuo","da","dai","dan","dang","dao","de","deng","di","dian","diao","die","ding","diu","dong","dou","du","duan",
												   "dui","dun","duo","e","en","er","fa","fan","fang","fei","fen","feng","fo","fou","fu","ga","gai","gan","gang","gao",
												   "ge","gei","gen","geng","gong","gou","gu","gua","guai","guan","guang","gui","gun","guo","ha","hai","han","hang",
												   "hao","he","hei","hen","heng","hong","hou","hu","hua","huai","huan","huang","hui","hun","huo","ji","jia","jian",
												   "jiang","jiao","jie","jin","jing","jiong","jiu","ju","juan","jue","jun","ka","kai","kan","kang","kao","ke","ken",
												   "keng","kong","kou","ku","kua","kuai","kuan","kuang","kui","kun","kuo","la","lai","lan","lang","lao","le","lei",
												   "leng","li","lia","lian","liang","liao","lie","lin","ling","liu","long","lou","lu","lv","luan","lue","lun","luo",
												   "ma","mai","man","mang","mao","me","mei","men","meng","mi","mian","miao","mie","min","ming","miu","mo","mou","mu",
												   "na","nai","nan","nang","nao","ne","nei","nen","neng","ni","nian","niang","niao","nie","nin","ning","niu","nong",
												   "nu","nv","nuan","nue","nuo","o","ou","pa","pai","pan","pang","pao","pei","pen","peng","pi","pian","piao","pie",
												   "pin","ping","po","pu","qi","qia","qian","qiang","qiao","qie","qin","qing","qiong","qiu","qu","quan","que","qun",
												   "ran","rang","rao","re","ren","reng","ri","rong","rou","ru","ruan","rui","run","ruo","sa","sai","san","sang",
												   "sao","se","sen","seng","sha","shai","shan","shang","shao","she","shen","sheng","shi","shou","shu","shua",
												   "shuai","shuan","shuang","shui","shun","shuo","si","song","sou","su","suan","sui","sun","suo","ta","tai",
												   "tan","tang","tao","te","teng","ti","tian","tiao","tie","ting","tong","tou","tu","tuan","tui","tun","tuo",
												   "wa","wai","wan","wang","wei","wen","weng","wo","wu","xi","xia","xian","xiang","xiao","xie","xin","xing",
												   "xiong","xiu","xu","xuan","xue","xun","ya","yan","yang","yao","ye","yi","yin","ying","yo","yong","you",
												   "yu","yuan","yue","yun","za","zai","zan","zang","zao","ze","zei","zen","zeng","zha","zhai","zhan","zhang",
												   "zhao","zhe","zhen","zheng","zhi","zhong","zhou","zhu","zhua","zhuai","zhuan","zhuang","zhui","zhun","zhuo",
												   "zi","zong","zou","zu","zuan","zui","zun","zuo"};
                #endregion

                byte[] array = new byte[2];
                string returnstr = "";
                int chrasc = 0;
                int i1 = 0;
                int i2 = 0;
                char[] nowchar = text.ToCharArray();
                for (int j = 0; j < nowchar.Length; j++)
                {
                    array = Encoding.Default.GetBytes(nowchar[j].ToString());
                    i1 = (short)(array[0]);
                    i2 = (short)(array[1]);

                    chrasc = i1 * 256 + i2 - 65536;
                    if (chrasc > 0 && chrasc < 160)
                    {
                        returnstr += nowchar[j];
                    }
                    else
                    {
                        for (int i = (pyvalue.Length - 1); i >= 0; i--)
                        {
                            if (pyvalue[i] < chrasc)
                            {
                                returnstr += pystr[i];
                                break;
                            }
                        }
                    }
                }

                return returnstr;
            }




        //}
    #endregion
    }
}
