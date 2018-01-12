using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace DAL
{
    public class GlobalFunc
    {
        public static int EncryptOffset = 5;
        public static byte[] Encrypt(byte[] pBuf, byte[] DeviceID, byte[] SSID)
        {
            return Encrypt(pBuf, DeviceID, SSID, 0);
        }
        public static byte[] Encrypt(byte[] pBuf, byte[] DeviceID,byte[] SSID,int offset)
        {
            byte[] ret = new byte[pBuf.Length];
            byte[] key = new byte[8];
            for (int i = 0; i < 4; i++)
            {
                key[i] = (byte)(DeviceID[i] + EncryptOffset+offset);
                key[i+4] = (byte)(SSID[i] + EncryptOffset+offset);
            }
            int c = pBuf.Length / 8;
            for (int i = 0; i < c; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    ret[i * 8 + j] = (byte)(key[j] ^ pBuf[i * 8 + j]);
                }
            }
            return ret;
        }
    }
}