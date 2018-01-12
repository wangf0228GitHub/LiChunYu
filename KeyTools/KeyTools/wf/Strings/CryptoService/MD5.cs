using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

namespace WFNetLib.Strings.CryptoService
{
    public static class MD5Encrypt
    {
        public static string GetMD5(string s,bool bA)
        {
            return GetMD5(Encoding.Default.GetBytes(s),bA);
        }
        public static string GetMD5(string s)
        {
            return GetMD5(Encoding.Default.GetBytes(s));
        }
        public static string GetMD5(byte[] bs)
        {
            return GetMD5(bs, false);
        }
        public static string GetMD5(byte[] bs,bool bA)
        {
            MD5 m = MD5.Create();
            byte[] data = m.ComputeHash(bs);            
            StringBuilder sbd = new StringBuilder();
            foreach (byte b in data)
            {
                if (bA)
                    sbd.Append(b.ToString("X").PadLeft(2, '0'));
                else
                    sbd.Append(b.ToString("x").PadLeft(2, '0'));
            }
            return sbd.ToString();  
        }
    }
}
