using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WFNetLib;

namespace 钥匙写入数据加密
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] sin = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox1.Text, " ");
            if (sin.Length == 8)
            {
                byte[] sout = lcyHexDecode.Decode(sin);
                textBox2.Text=WFNetLib.StringFunc.StringsFunction.byteToHexStr(sout, " ");
            }
            else
            {
                MessageBox.Show("输入的数据格式有误");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
//             byte[] sin = new byte[8];
//             for(int i=0;i<8;i++)
//                 sin[i]=(byte)(i+1);
//             byte[] sout = lcyHexDecode.Decode(sin);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] sin = WFNetLib.StringFunc.StringsFunction.strToHexByte(textBox1.Text, " ");
            if (sin.Length == 8)
            {
                byte[] sout = lcyHexDecode1.Decode(sin);
                textBox2.Text = WFNetLib.StringFunc.StringsFunction.byteToHexStr(sout, " ");
            }
            else
            {
                MessageBox.Show("输入的数据格式有误");
            }
        }
    }
    public class lcyHexDecode1
    {
        static readonly ushort[] TCodeKey = new ushort[316]
        {
	        0x3139,0x2129,0x1119,0x0109,0x333B,0x232B,0x131B,0x030B,0x353D,0x252D,0x151D,0x050D,0x373F,0x272F,0x171F,
	        0x070F,0x3038,0x2028,0x1018,0x0008,0x323A,0x222A,0x121A,0x020A,0x343C,0x242C,0x141C,0x040C,0x363E,0x262E,0x161E,
	        0x060E,0x2707,0x2F0F,0x3717,0x3F1F,0x2606,0x2E0E,0x3616,0x3E1E,0x2505,0x2D0D,0x3515,0x3D1D,0x2404,0x2C0C,0x3414,
	        0x3C1C,0x2303,0x2B0B,0x3313,0x3B1B,0x2202,0x2A0A,0x3212,0x3A1A,0x2101,0x2909,0x3111,0x3919,0x2000,0x2808,0x3010,
	        0x3818,0x8080,0x041F,0x0100,0x0302,0x8080,0x0803,0x0504,0x0706,0x8080,0x0C07,0x0908,0x0B0A,0x8080,0x100B,0x0D0C,
	        0x0F0E,0x8080,0x140F,0x1110,0x1312,0x8080,0x1813,0x1514,0x1716,0x8080,0x1C17,0x1918,0x1B1A,0x8080,0x001B,0x1D1C,
	        0x1F1E,0x060F,0x1413,0x0B1C,0x101B,0x0E00,0x1916,0x1104,0x091E,0x0701,0x0D17,0x1A1F,0x0802,0x0C12,0x051D,0x0A15,
	        0x1803,0x3038,0x2028,0x1018,0x0008,0x3139,0x2129,0x1119,0x0109,0x323A,0x222A,0x121A,0x020A,0x333B,0x232B,0x8080,
	        0x8080,0x363E,0x262E,0x161E,0x060E,0x353D,0x252D,0x151D,0x050D,0x343C,0x242C,0x141C,0x040C,0x131B,0x030B,0x8080,
	        0x8080,0x8080,0x040D,0x0A10,0x0017,0x8080,0x0902,0x0E1B,0x1405,0x8080,0x0716,0x0B12,0x1903,0x8080,0x010F,0x1A06,
	        0x0C13,0x8080,0x3A2C,0x2237,0x3228,0x8080,0x3321,0x362B,0x2430,0x8080,0x382F,0x2A34,0x253B,0x8080,0x2331,0x352D,
	        0x2027,0x84D2,0xB16F,0x3EA9,0xC750,0xD81F,0x74A3,0x6BC5,0x920E,0x417B,0xE29C,0xAD06,0x58F3,0xE721,0x8D4A,0x90FC,
	        0x6B35,0x2E4B,0x8DF0,0x973C,0x615A,0xB7D0,0x1A49,0x5CE3,0x862F,0xBD14,0x7EC3,0x68AF,0x9205,0xD86B,0xA714,0x0F95,
	        0x3CE2,0xAFC1,0x6892,0x340D,0x5BE7,0x42AF,0x957C,0xDE61,0x380B,0xF59E,0xC328,0x4A70,0xB61D,0x2C43,0xFA95,0x17BE,
	        0x8D60,0x412C,0xB67A,0x3F85,0xE9D0,0x2CEB,0xD147,0xFA50,0x8639,0x1B42,0x78AD,0xC5F9,0x0E63,0xC7B8,0x2D1E,0x096F,
	        0x53A4,0xE37D,0x9A06,0x8512,0x4FBC,0xB5D8,0x036F,0x2C47,0xE91A,0x90A6,0x7DCB,0x3EF1,0x8452,0x063F,0xD8A1,0x5B94,
	        0x2EC7,0x9EA0,0xF563,0xC71D,0x28B4,0x09D7,0x6A34,0x5E28,0xF1CB,0x49D6,0x308F,0x2CB1,0xE75A,0xD01A,0x8769,0xE34F,
	        0x2CB5,0x8EF1,0x346B,0x2D97,0x5AC0,0x473D,0x8EF2,0x1AC0,0xB569,0x7B0E,0xD1A4,0xC658,0x2F93,0xA1D8,0x423F,0x7CB6,
	        0xE905,0xD1E4,0xB82F,0x6C3A,0x0759,0x740F,0xD1E2,0xCBA6,0x3895,0xE841,0x2BD6,0x97FC,0x503A,0x82FC,0x1749,0x3E5B,
	        0x6DA0,0x0201,0x0202,0x0202,0x0102,0x0202,0x0202,0x0202,0x0001,0x4080,0x1020,0x0408,0x0102
        };
        static byte[] RAMBUF = new byte[56];
        static byte R0XC0, R0XC1 = 0;
        static byte R0XBB;
        static byte R0XBC;
        static byte R0X52;
        static byte R0X53;
        static byte R0XBD;
        static byte R0XBE;
        static byte R0X40, R0X41;
        static byte R0XCB;            // bit6 读FLASH标识位		
        static int FlashAddr;
        static uint ulR0XAB;
        static uint ulR0XAF;
        static byte ReadFshData(byte c)
        {
            int addr;
            addr = FlashAddr + c - 0x1e14;
            int i = addr / 2;
            int j = addr % 2;
            if (j == 0)
            {
                return BytesOP.GetLowByte(TCodeKey[i]);
            }
            else
                return BytesOP.GetHighByte(TCodeKey[i]);
        }
        static void GetFshKey()
        {
            byte t;
            int p0, p1;

            R0XBD = R0X52;
            R0XBE = R0X53;

            for (; ; )
            {
                R0X40 = R0XBE;

                R0X40 = (byte)((R0X40 >> 3) & 0X1F);

                t = (byte)(R0XBC + R0X40);
                p0 = t;

                FlashAddr = 0x1E14;
                if (BytesOP.GetBit(R0XCB, 6))
                    FlashAddr = 0X1EF4;

                R0X41 = ReadFshData(R0XBD);

                if ((R0X41 & 0x80) == 0)
                {
                    R0XC0 = (byte)(R0X41 & 0X7);

                    FlashAddr = 0X1E14;
                    if (BytesOP.GetBit(R0XCB, 6))
                        FlashAddr = 0X1EF4;
                    R0X40 = ReadFshData(R0XBD);

                    R0X40 = (byte)((R0X40 >> 3) & 0x1F);            // 去掉了高三位，即去掉了循环右移影响

                    t = (byte)(R0XBB + R0X40);
                    p1 = t;

                    FlashAddr = 0X2084;
                    t = ReadFshData(R0XC0);//(A.R0XC0);
                    t = (byte)(t & (RAMBUF[p1-0x83]));

                    if (t != 0)
                        t = 0x80;
                }
                else
                {
                    t = 0x80;
                }

                RAMBUF[p0-0x83] = (byte)(((RAMBUF[p0-0x83]) >> 1) | t);

                R0XBD--;
                R0XBE--;
                if ((R0XBE & 0X80) != 0)
                    break;
            }

        }
        static void ShiftFshKey()
        {
            FlashAddr = 0X2074;
            R0X41 = ReadFshData(R0XC1);

            if (R0X41 != 0)
            {
                for (; ; )
                {
                    //			asm("BANKSEL _A.R0XAB");
                    //			asm("RRCF _A.R0XAB, F, B");
                    //			asm("RRCF _A.R0XAC, F, B");
                    //			asm("RRCF _A.R0XAD, F, B");
                    //			asm("RRCF _A.R0XAE, F, B ");
                    ulR0XAB >>= 1;

                    BytesOP.ClrBit(ref ulR0XAB, 7);
                    if (BytesOP.GetBit(ulR0XAB, 27))
                    { BytesOP.SetBit(ref ulR0XAB, 7); };

                    //           asm("BANKSEL _A.R0XAF");
                    //		   asm("RRCF _A.R0XAF, F, B");
                    //		   asm("RRCF _A.R0XB0, F, B");
                    //		   asm("RRCF _A.R0XB1, F, B");
                    //		   asm("RRCF _A.R0XB2, F, B");
                    ulR0XAF >>= 1;

                    BytesOP.ClrBit(ref ulR0XAF, 7);
                    if (BytesOP.GetBit(ulR0XAF, 27))
                    { BytesOP.SetBit(ref ulR0XAF, 7); };

                    R0X41--;
                    if (R0X41 == 0)
                        break;
                }
            }

            R0XBB = 0XAB;
            R0XBC = 0XB3;
            R0X52 = 0X7F;
            R0X53 = 0X3F;

            BytesOP.SetBit(ref R0XCB, 6);
            GetFshKey();

        }
        static void ShiftMixKey()
        {
            byte t;
            int p0;

            R0XBD = 0XE0;

            for (; ; )
            {
                FlashAddr = 0X1F74;
                p0 = R0XBB;
                t = RAMBUF[p0-0x83];

                t = (byte)((t >> 1) | R0XBD);

                R0X52 = ReadFshData(t);

                t = RAMBUF[p0-0x83];

                if ((t & 1) == 0)
                {
                    R0X52 = (byte)(R0X52 >> 4);
                }

                R0X52 &= 0XF;
                p0 = R0XBC;

                if (!BytesOP.GetBit(R0XBD, 5))
                {
                    R0XBC++;

                    t = RAMBUF[p0-0x83];
                    t <<= 4;

                    R0X52 |= t;
                }

                RAMBUF[p0-0x83] = R0X52;

                R0XBB++;

                t = R0XBD;
                R0XBD = (byte)(R0XBD - 0X20);

                //if (CARRY==0) break;
                if (t < 0x20)
                    break;
            }
        }
        static void MixFshKey()
        {
            int p0, p1, p2;

            R0XBB = 0XA7;
            R0XBC = 0X9B;
            R0X52 = 0XBF;
            R0X53 = 0X3F;

            BytesOP.ClrBit(ref R0XCB, 6);
            GetFshKey();
            ShiftFshKey();

            R0X53 = 8;
            p0 = 0x9b;
            p1 = 0xb3;

            for (; ; )
            {
                RAMBUF[p0-0x83] ^= (RAMBUF[p1-0x83]);
                p0++;
                p1++;

                R0X53--;
                if (R0X53 == 0)
                    break;
            }

            R0XBB = 0X9B;
            R0XBC = 0X9B;
            ShiftMixKey();

            R0XBB = 0X9B;
            R0XBC = 0XB3;
            R0X52 = 0XDF;
            R0X53 = 0X1F;

            BytesOP.ClrBit(ref R0XCB, 6);
            GetFshKey();

            R0X53 = 4;
            p0 = 0xa6;
            p1 = 0xaa;
            p2 = 0xb6;

            for (; ; )
            {
                RAMBUF[p2-0x83] ^= (RAMBUF[p0-0x83]);
                RAMBUF[p0 - 0x83] = RAMBUF[p1 - 0x83];
                RAMBUF[p1 - 0x83] = RAMBUF[p2 - 0x83];

                p0--;
                p1--;
                p2--;

                R0X53--;
                if (R0X53 == 0)
                    break;

            }

        }
        public static byte[] Decode(byte[] sin)
        {
            byte[] sout = new byte[8];

            for (int i = 0; i < 8; i++)
            {
                RAMBUF[i + 0x83 - 0x83] = sin[i];
                RAMBUF[i + 0x8b - 0x83] = sin[i];
            }

            R0XBB = 0X8B;
            R0XBC = 0XA3;
            R0X52 = 0X3F;
            R0X53 = 0X3F;

            BytesOP.ClrBit(ref R0XCB, 6);

            GetFshKey();

            R0XBB = 0X93;
            R0XBC = 0XAB;
            R0X52 = 0X3F;
            R0X53 = 0X3F;

            BytesOP.SetBit(ref R0XCB, 6);

            GetFshKey();

            for (int i = 16; i != 0; i--)     // 16次循环
            {
                MixFshKey();
            }

            R0XBB = 0XA3;
            R0XBC = 0X8B;
            R0X52 = 0X7F;
            R0X53 = 0X3F;

            BytesOP.ClrBit(ref R0XCB, 6);

            GetFshKey();
            for (int i = 0; i < 8; i++)
                sout[i] = RAMBUF[0x8b - 0x83 + i];
            return sout;
        }
    }
}
