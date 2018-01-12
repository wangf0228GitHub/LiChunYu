using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WFNetLib
{
    //0为最低位,其它顺次为高位
    [StructLayout(LayoutKind.Explicit)]
    public struct Union_UInt16
    {
        //taken from c struct in platform sdk 
        [FieldOffset(0)]
        public ushort ofs_16;
        [FieldOffset(0)]
        public byte ofs_l;
        [FieldOffset(1)]
        public byte ofs_h;
    }
    [StructLayout(LayoutKind.Explicit)]
    public struct Union_UInt32
    {
        //taken from c struct in platform sdk 
        //32位无符号数据
        [FieldOffset(0)]
        public uint ofs_32;
        [FieldOffset(0)]
        public Union_UInt16 ofs_l;
        [FieldOffset(2)]
        public Union_UInt16 ofs_h;                
    }

    [StructLayout(LayoutKind.Explicit)]
    public struct Union_Int16
    {
        //taken from c struct in platform sdk 
        [FieldOffset(0)]
        public short ofs_16;
        [FieldOffset(0)]
        public byte ofs_l;
        [FieldOffset(1)]
        public sbyte ofs_h;        
    }
    [StructLayout(LayoutKind.Explicit)]
    public struct Union_Int32
    {
        //taken from c struct in platform sdk 
        [FieldOffset(0)]
        public int ofs_32;
        [FieldOffset(0)]
        public Union_UInt16 ofs_l;
        [FieldOffset(2)]
        public Union_Int16 ofs_h; 
    }    
}
