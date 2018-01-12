using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WFNetLib
{
    public class BytesOP
    {
        static public byte GetLowNibble(byte x)
        {
            return (byte)(x & 0x0f);
        }
        static public byte GetHighNibble(byte x)
        {
            return (byte)(x>>4);
        }
        static public byte GetLowByte(ushort x)
        {
            return (byte)(x&0xff);
        }
        static public byte GetHighByte(ushort x)
        {
            return (byte)((x>>8)&0xff);
        }
        static public ushort GetLowShort(uint x)
        {
            return (ushort)(x&0xffff);
        }
        static public ushort GetHighShort(uint x)
        {
            return (ushort)((x>>16)&0xffff);
        }
        static public byte MakeByte(byte h,byte l)
        {
            return (byte)((h<<4)+(l&0x0f));
        }
        static public ushort MakeShort(byte h,byte l)
        {
            return (ushort)((((ushort)h)<<8)+l);
        }
        static public uint MakeInt(ushort h,ushort l)
        {
            return ((((uint)h)<<16)+l);
        }
        static public uint MakeInt(byte b4, byte b3, byte b2, byte b1)
        {
            return ((((uint)b4) << 24) + (((uint)b3) << 16) + (((uint)b2) << 8) + (uint)b1);
        }
        static public void SetBit(ref byte b, int index)
        {
            if (index > 7)
                return;
            b |= (byte)(1 << index);
        }
        static public void SetBit(ref ushort b, int index)
        {
            if (index > 15)
                return;
            b |= (ushort)(1 << index);
        }
        static public void SetBit(ref uint b, int index)
        {
            if (index > 31)
                return;
            b |= (uint)(1 << index);
        }
        static public void ClrBit(ref byte b, int index)
        {
            if (index > 7)
                return;
            b &= (byte)(~(1 << index));
        }
        static public void ClrBit(ref ushort b, int index)
        {
            if (index > 15)
                return;
            b &= (ushort)(~(1 << index));
        }
        static public void ClrBit(ref uint b, int index)
        {
            if (index > 31)
                return;
            b &= (uint)(~(1 << index));
        }
        static public Boolean GetBit(byte b, int index)
        {
            if (index > 7)
                return false;
            if ((byte)((b & (1 << index)) >> index) == 0x01)
                return true;
            else
                return false;
        }
        static public Boolean GetBit(ushort b, int index)
        {
            if (index > 15)
                return false;
            if ((byte)((b & (1 << index)) >> index) == 0x01)
                return true;
            else
                return false;
        }
        static public Boolean GetBit(uint b, int index)
        {
            if (index > 31)
                return false;
            if ((byte)((b & (1 << index)) >> index) == 0x01)
                return true;
            else
                return false;
        }
        static public byte Byte2BCD(byte b)
        {
            if (b > 99)
                throw new ArgumentOutOfRangeException();
            byte l,h;
            l = (byte)(b % 10);
            h = (byte)(b / 10);
            return MakeByte(h, l);
        }
        static public byte BCD2Byte(byte b)
        {
            return (byte)(GetHighNibble(b)*10+GetLowNibble(b));
        }
        static public ushort BCD2Short(byte[] bs)//默认降序排列，即高位为bs[0]
        {
            return BCD2Short(bs, false);
        }
        static public ushort BCD2Short(byte[] bs,bool bASC)//是否升序排列,即低位为bs[0]
        {
            ushort ret=0;
            if (bASC)//升序排列,即低位为bs[0]
            {
                ret += GetLowNibble(bs[0]);
                ret += (ushort)(GetHighNibble(bs[0]) * ((ushort)Math.Pow(10, 1)));
                ret += (ushort)(GetLowNibble(bs[1]) * ((ushort)Math.Pow(10, 2)));
                ret += (ushort)(GetHighNibble(bs[1]) * ((ushort)Math.Pow(10, 3)));
            }
            else//降序排列，即高位为bs[0]
            {
                ret += GetLowNibble(bs[1]);
                ret += (ushort)(GetHighNibble(bs[1]) * ((ushort)Math.Pow(10, 1)));
                ret += (ushort)(GetLowNibble(bs[0]) * ((ushort)Math.Pow(10, 2)));
                ret += (ushort)(GetHighNibble(bs[0]) * ((ushort)Math.Pow(10, 3)));
            }
            return ret;
        }
        static public uint BCD2int(byte[] bs,string format)//默认降序排列，即高位为bs[0]
        {
            return BCD2int(bs, format,false);
        }
        static public uint BCD2int(byte[] bs, string format, bool bASC)//是否升序排列,即低位为bs[0]
        {
            uint ret = 0;
            int len=format.Length/2;
            if (bASC)//升序排列,即低位为bs[0]
            {
                for (int i = 0; i < len; i++)
                {
                    ret += GetLowNibble(bs[len-1-i]) * ((uint)Math.Pow(10, i<<1));
                    ret += GetHighNibble(bs[len-1-i]) * ((uint)Math.Pow(10, (i<<1)+1));
                }
                if (format.Length % 2 == 1)
                {
                    ret = ret / 10;
                    ret += GetLowNibble(bs[len]) * ((uint)Math.Pow(10, len << 1));
                    ret += GetHighNibble(bs[len]) * ((uint)Math.Pow(10, (len << 1) + 1));
                }
            }
            else//降序排列，即高位为bs[0]
            {
                for (int i = 0; i < len; i++)
                {
                    ret += GetHighNibble(bs[i]) * ((uint)Math.Pow(10, ((len - 1) << 1) + 1));
                    ret += GetLowNibble(bs[i]) * ((uint)Math.Pow(10, (len - 1) << 1));
                }
                if (format.Length % 2 == 1)
                {
                    ret = ret * 100;
                    ret += GetHighNibble(bs[len]) * (uint)10;
                    ret += GetLowNibble(bs[len]);
                }
            }
            return ret;
        }
        static public void Byte2ASCII(byte b, out byte h, out byte l)
        {
            h = GetHighNibble(b);
            l = GetLowNibble(b);
            if (h > 9)
                h = (byte)(h - 0x0a + (byte)'A');
            else
                h = (byte)(h + (byte)'0');
            if (l > 9)
                l = (byte)(l - 0x0a + (byte)'A');
            else
                l = (byte)(l + (byte)'0');
        }
        static public byte ASCII2Byte(byte h, byte l)
        {
            if (h > '9')
                h = (byte)(h + 0x0a - (byte)'A');
            else
                h = (byte)(h - (byte)'0');
            if (l > 9)
                l = (byte)(l + 0x0a - (byte)'A');
            else
                l = (byte)(l - (byte)'0');
            return MakeByte(h, l);
        }
    }
}
