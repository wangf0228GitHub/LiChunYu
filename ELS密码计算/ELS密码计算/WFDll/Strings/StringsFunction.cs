using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.Configuration;
using System.Management;
using System.IO;
using System.Data;
using Microsoft.VisualBasic;

namespace WFNetLib.Strings
{
    public abstract class StringsFunction
    {
    

    

    

    

    #region "字符串的处理函数集"

       
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
                    for (int i = 0; i < (count-1);i++ )
                    {
                        strB.Append(bytes[offset+i].ToString("X2"));
                        strB.Append(spilt);
                    }
                    strB.Append(bytes[offset + count-1].ToString("X2"));
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
    #endregion
    }
}
