using System;
using System.Text;
using System.ComponentModel;
using WFNetLib.Strings;
using WFNetLib.Strings.CryptoService;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Threading;
using System.IO.Ports;
using WFNetLib.StringFunc;
// 
// this.Invoke((EventHandler)(delegate
//                     {
//                         
//                     }));
namespace WFNetLib
{    
    public class WFGlobal
    {        
        static public TValue ParseValue<TValue>(string value)
        {
            TypeConverter converter = TypeDescriptor.GetConverter(typeof(TValue));
            return (TValue)converter.ConvertFromInvariantString(value);
        }
        static public void GetMaxMinIndex(double[] values, out int minIndex, out int maxIndex)
        {
            if (values == null)
                throw new ArgumentNullException("values");

            minIndex = maxIndex = 0;
            double minimum = Double.MaxValue;
            double maximum = Double.MinValue;

            for (int i = 0; i < values.Length; ++i)
            {
                double currentValue = values[i];

                if (currentValue < minimum)
                {
                    minimum = currentValue;
                    minIndex = i;
                }

                if (currentValue > maximum)
                {
                    maximum = currentValue;
                    maxIndex = i;
                }
            }
        }
        static public void GetMaxMinValue(double[] values, out double minimum, out double maximum)
        {
            if (values == null)
                throw new ArgumentNullException("values");

            minimum = Double.MaxValue;
            maximum = Double.MinValue;

            for (int i = 0; i < values.Length; ++i)
            {
                double currentValue = values[i];

                if (currentValue < minimum)
                    minimum = currentValue;

                if (currentValue > maximum)
                    maximum = currentValue;
            }
        }
        static public void WaitMS(int ms)
        {
            Thread t = new Thread(o => Thread.Sleep(ms));
            t.Start(new object());
            while (t.IsAlive)
            {
                Application.DoEvents();
            }
//             while (ms != 0)
//             {
//                 Application.DoEvents();
//                 Thread.Sleep(1);
//                 ms--;
//             } 
        }
        static public double GetAverageValue(double[] values)
        {
            if (values == null)
                throw new ArgumentNullException("values");

            double sum = 0.0;
            for (int i = 0; i < values.Length; ++i)
                sum += values[i];

            return sum / values.Length;
        }
        public static Boolean GetTimeRange(string str, out DateTime StartTime, out DateTime EndTime, string timeformat)
        {
            string[] time = str.Split('-');
            if (time.GetLength(0) != 2)
            {
                StartTime = DateTime.Now;
                EndTime = DateTime.Now;
                return false;
            }
            StartTime = DateTime.ParseExact(time[0], timeformat, null);
            EndTime = DateTime.ParseExact(time[1], timeformat, null);
            return true;
        }
        //*****************************************************
        //16进制转化为字符串函数
        //*****************************************************
        public static int HexToStr(string hex, byte[] str)//将16进制字符串转换放到BYTE数组里，并返回转换后的数据长度，参数是16进制字符串和字                                            节数组
        {
            int asc = 0;
            int len = 0;
            string s = "";
            string ss = "";
            for (int i = 0; i < hex.Length; i++)
            {
                if (hex[i] != ' ')
                {
                    s = s + hex[i];
                    asc = (byte)hex[i];
                    if ((asc < 48) || ((asc > 57) && (asc < 65)) || ((asc > 70) && (asc < 97)) || (asc > 102))
                    {
                        Exception e = new Exception("字符串中有字超过16进制范围！");
                        throw e;                       
                    }
                }

            }
            for (int j = 0; j < (s.Length - 1); j = j + 2)
            {
                ss = "";
                ss = ss + s[j] + s[j + 1];
                str[len] = (byte)(System.Convert.ToInt32(ss, 16));
                len++;
            }
            return len;
        }

        //****************************************************************
        //字符串转化为16进制函数
        //****************************************************************
        public static string StrToHex(byte[] str, int len)//将BYTE数组里的数据转换为16进制，参数是BYTE数组，和数组里的数据长度
        {
            string hex = "";
            string s;
            int asc;
            for (int i = 0; i < len; i++)
            {
                s = "";
                asc = str[i];
                //hex = hex + System.Convert.ToString(asc,16);
                s = System.Convert.ToString(asc, 16);
                for (int j = 0; j < s.Length; j++)
                {
                    if (s.Length == 1)
                        hex = hex + '0';
                    if (s[j] == 'a')
                        hex = hex + 'A';
                    else if (s[j] == 'b')
                        hex = hex + 'B';
                    else if (s[j] == 'c')
                        hex = hex + 'C';
                    else if (s[j] == 'd')
                        hex = hex + 'D';
                    else if (s[j] == 'e')
                        hex = hex + 'E';
                    else if (s[j] == 'f')
                        hex = hex + 'F';
                    else
                        hex = hex + s[j];
                }
                if (i < (len - 1))
                    hex = hex + " ";
            }
            return hex;
        }
        public static DialogResult MessageYseOrNo(string text, string caption)
        {
            return MessageBox.Show(text, caption, MessageBoxButtons.YesNo, MessageBoxIcon.Question);
        }
        public static void MessageWaring(string text, string caption)
        {
            MessageBox.Show(text, caption, MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
        

        
        public static bool OpenSerialPort(ref SerialPort mySerialPort,string name)
        {
            while (true)
            {
                try
                {
                    mySerialPort.Open();
                    break;
                }
                catch (System.Exception ex)
                {
                    if(name!="")
                    {
                        if (MessageBox.Show(name + "的串口无法打开:" + ex.Message + "，是否重试?", "错误", MessageBoxButtons.RetryCancel) == DialogResult.Cancel)
                            return false;
                    }
                    return false;
                }
            }
            return true;
        }
        public static bool OpenSerialPort(IWin32Window owner, ref SerialPort mySerialPort, string name)
        {
            while (true)
            {
                try
                {
                    mySerialPort.Open();
                    break;
                }
                catch (System.Exception ex)
                {
                    if (MessageBox.Show(owner,name + "的串口无法打开:" + ex.Message + "，是否重试?", "错误", MessageBoxButtons.RetryCancel) == DialogResult.Cancel)
                        return false;
                }
            }
            return true;
        }
        /// <summary>
        /// 初始化程序执行时间
        /// </summary>
        public static DateTime dtStartTime = DateTime.MinValue;

        
        /// <summary>
        /// 系统运行时信息，用于调试
        /// </summary>
        public static StringBuilder writeLine = new StringBuilder();
    }
}
