using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace hash计算
{
    class lcyHashCal
    {
        public byte[] HashCal(byte[] _HashIn, int n)
        {
            byte[] ret;
            ret = HashCal(_HashIn);
            for (int i = 1; i < n; i++)
            {
                ret = HashCal(ret);
            }
            return ret;
        }
        public byte[] HashCal(byte[] _HashIn)
        {
            for (int k = 0; k < 8; k++)
                HashIn[k] = _HashIn[k];
            byte i;
            byte[] Sa = new byte[0x08];
            byte[] Sb = new byte[0x08];
            //--
            F_calsel = false;
            sub_half();
            for (i = 0; i < 8; i++)
            {
                Sa[i] = Ht[i];
            }
            //--
            F_calsel = true;
            sub_half();
            for (i = 0; i < 8; i++)
            {
                Sb[i] = Ht[i + 0x04];
            }
            //--
            sub_add(Sa, Sb);
            byte[] ret = new byte[0x08];
            for (int j = 0; j < 8; j++)
                ret[j] = HashOut[j];
            return ret;
        }
        public void HashOnce()
        {
            HashCal(HashIn);
        }
        public byte[] HashIn = new byte[0x08];
        public byte[] HashOut = new byte[0x08];
        byte[] Ht = new byte[0x10];
        byte[] InTP = new byte[0x08];
        bool F_calsel = false;//false is fisrt,true is second;

        void loadTab()
        {
            byte[] TAB0 = new byte[0x10]{
		        0x45,0x03,0x11,0x4F,0x85,0x4C,0x7A,0x31,
		        0xF6,0x7C,0xCB,0xB8,0x15,0xA4,0xD0,0x42};
            byte[] TAB1 = new byte[0x10]{
		        0x51,0x9B,0x49,0x14,0xB1,0x2E,0x8F,0xB5,
		        0x95,0xDE,0x62,0x7A,0xD8,0x0D,0x44,0x1C};

            int i;
            for (i = 0; i < 0x10; i++)
            {
                if (false == F_calsel)
                {
                    Ht[i] = TAB0[i];
                }
                else
                {
                    Ht[i] = TAB1[i];
                }
            }
        }
        void sub_half()
        {
            byte n, m;
            loadTab();
            sub_cal0();//0AFE
            n = 0x00;
            sub_cal1(n);//0CDF
            m = 1;
            sub_cal_L_A(m);//0A9C
            sub_cal0();//0AFE
            n = 0x04;
            sub_cal1(n);//0CDF
            sub_Mixed_A();
            //00
            sub_cal2();//0B4B
            m = 1;
            sub_cal_R_A(m);//0A95
            //01
            sub_cal2();//0B4B
            m = 2;
            sub_cal_R_A(m);//
            //02
            sub_cal2();//0B4B
            sub_Mixed_A();
            //03
            sub_cal2();//0B4B
            m = 3;
            sub_cal_R_B(m);
            //04
            sub_cal2();//0B4B
            m = 3;
            sub_cal_L_A(m);
            //05
            sub_cal2();//0B4B
            m = 1;
            sub_cal_L_A(m);
            //06
            sub_cal2();//0B4B
            n = 0x04;
            sub_cal1(n);//0CDF
            m = 1;
            sub_cal_R_A(m);//
            //07
            sub_cal2();//0B4B
            m = 1;
            sub_cal_R_B(m);
            //08
            sub_cal2();//0B4B
            m = 1;
            sub_cal_R_A(m);//
            //09
            sub_cal2();//0B4B
            m = 4;
            sub_cal_L_A(m);
            //0A
            sub_cal2();//0B4B
            m = 1;
            sub_cal_R_B(m);
            //0B
            sub_cal2();//0B4B
            m = 1;
            sub_cal_L_A(m);
            //0C
            sub_cal2();//0B4B
            n = 0x00;
            sub_cal1(n);//0CDF
            m = 1;
            sub_cal_R_A(m);//
            //0D
            sub_cal2();//0B4B
            m = 3;
            sub_cal_L_A(m);
            //0E
            sub_cal2();//0B4B
            m = 3;
            sub_cal_R_B(m);//
            //0F
            sub_cal2();//0B4B
            m = 4;
            sub_cal_L_A(m);
            //10
            sub_cal3();//0BD9
            m = 3;
            sub_cal_L_A(m);
            //11
            sub_cal3();//0BD9
            m = 3;
            sub_cal_R_B(m);
            //12
            sub_cal3();//0BD9
            m = 2;
            sub_cal_R_B(m);
            //13
            sub_cal3();//0BD9
            m = 1;
            sub_cal_R_A(m);
            //14
            sub_cal3();//0BD9
            m = 2;
            sub_cal_R_B(m);
            //15
            sub_cal3();//0BD9
            n = 0x04;
            sub_cal1(n);//0CDF
            m = 1;
            sub_cal_L_A(m);
            //16
            sub_cal3();//0BD9
            m = 3;
            sub_cal_R_B(m);
            //17
            sub_cal3();//0BD9
            m = 1;
            sub_cal_R_B(m);
            //18
            sub_cal3();//0BD9
            n = 0x00;
            sub_cal1(n);//0CDF
            m = 2;
            sub_cal_R_A(m);
            //19
            sub_cal3();//0BD9
            sub_Mixed_A();
            //1A
            sub_cal3();//0BD9
            m = 3;
            sub_cal_R_B(m);
            //1B
            sub_cal3();//0BD9
            m = 2;
            sub_cal_R_A(m);
            //1C
            sub_cal3();//0BD9
            m = 4;
            sub_cal_L_A(m);
            //1D
            sub_cal3();//0BD9
            m = 3;
            sub_cal_R_A(m);
            //1E
            sub_cal3();//0BD9
            m = 1;
            sub_cal_R_A(m);
            //1F
            sub_cal3();//0BD9
            m = 3;
            sub_cal_R_A(m);
        }
        void sub_add_4B(byte[] p)
        {
            byte i, C;
            uint Sum;
            C = 0;
            for (i = 0; i < 0x04; i++)
            {
                Sum = (uint)(Ht[0x0B - i] + p[0x03 - i] + C);
                Ht[0x0B - i] = (byte)(Ht[0x0B - i] + p[0x03 - i] + C);
                if (Sum > 0xFF)
                {
                    C = 0x01;
                }
                else
                {
                    C = 0x00;
                }
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_Shift_L(byte N)//N is cnt
        {
            byte C, Ct, i, j;
            for (i = 0; i < N; i++)
            {
                C = (byte)(Ht[0x08] & 0x80);
                for (j = 0; j < 0x04; j++)
                {
                    Ct = (byte)(Ht[0x0B - j] & 0x80);
                    Ht[0x0B - j] = (byte)(Ht[0x0B - j] << 1);
                    if (0x00 == C)
                    {
                        Ht[0x0B - j] = (byte)(Ht[0x0B - j] & 0xFE);
                    }
                    else
                    {
                        Ht[0x0B - j] = (byte)(Ht[0x0B - j] | 0x01);
                    }
                    C = Ct;
                }
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_Shift_R(byte N)//N is cnt
        {
            byte C, Ct, i, j;
            for (i = 0; i < N; i++)
            {
                C = (byte)(Ht[0x0B] & 0x01);
                for (j = 0; j < 0x04; j++)
                {
                    Ct = (byte)(Ht[0x08 + j] & 0x01);
                    Ht[0x08 + j] = (byte)(Ht[0x08 + j] >> 1);
                    if (0x00 == C)
                    {
                        Ht[0x08 + j] = (byte)(Ht[0x08 + j] & 0x7F);
                    }
                    else
                    {
                        Ht[0x08 + j] = (byte)(Ht[0x08 + j] | 0x80);
                    }
                    C = Ct;
                }
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_Mixed_A()//
        {
            //              +0   +1   +2   +3   +4   +5   +6   +7   +8   +9   +A   +B   +C   +D   +E   +F
            byte[] Ta = new byte[0x10] { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0F, 0x0C, 0x0D, 0x0E, 0x00, 0x01, 0x02, 0x03 };
            byte[] tmp = new byte[0x10];
            byte i, j;
            for (i = 0; i < 0x10; i++)
            {
                j = Ta[i];
                tmp[j] = Ht[i];
            }
            //
            for (i = 0; i < 0x10; i++)
            {
                Ht[i] = tmp[i];
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_Mixed_B()//
        {
            //                      +0   +1   +2   +3   +4   +5   +6   +7   +8   +9   +A   +B   +C   +D   +E   +F
            byte[] Tb = new byte[0x10] { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0E, 0x0F, 0x0C, 0x0D, 0x00, 0x01, 0x02, 0x03 };
            byte[] tmp = new byte[0x10];
            byte i, j;

            for (i = 0; i < 0x10; i++)
            {
                j = Tb[i];
                tmp[j] = Ht[i];
            }
            //
            for (i = 0; i < 0x10; i++)
            {
                Ht[i] = tmp[i];
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_cal0()//0AFE
        {
            byte i;
            byte[] Biao_a = new byte[0x04] { 0x50, 0xA2, 0x8B, 0xE6 };
            byte[] TP = new byte[0x04];
            for (i = 0; i < 0x04; i++)
            {
                TP[i] = (byte)(((Ht[0x00 + i] ^ Ht[0x04 + i]) & Ht[0x0C + i]) ^ Ht[0x04 + i]);
            }
            //--
            sub_add_4B(TP);
            if (true == F_calsel)
            {
                sub_add_4B(Biao_a);
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_cal1(byte s)//0CDF
        {
            byte i;
            byte[] T = new byte[0x04];
            s = (byte)(s & 0x04);
            for (i = 0; i < 0x04; i++)
            {
                T[0x03 - i] = HashIn[i + s];
            }
            sub_add_4B(T);
        }
        /*---------------------------------------------------------------------*/
        void sub_cal2()//0B4B
        {
            byte i;
            byte[] Biao_b = new byte[0x04] { 0x5A, 0x82, 0x79, 0x99 };
            byte[] TP = new byte[0x04];
            byte m, n;
            for (i = 0; i < 0x04; i++)
            {
                m = (byte)(Ht[0x00 + i] & Ht[0x04 + i]);
                n = (byte)((Ht[0x00 + i] | Ht[0x04 + i]) & Ht[0x0C + i]);
                TP[i] = (byte)(m | n);
            }
            //--
            sub_add_4B(TP);
            if (false == F_calsel)
            {
                sub_add_4B(Biao_b);
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_cal3()//0BD9
        {
            byte i;
            byte[] Biao_c = new byte[0x04] { 0x6E, 0xD9, 0xEB, 0xA1 };
            byte[] Biao_d = new byte[0x04] { 0x5C, 0x4D, 0xD1, 0x24 };
            byte[] TP = new byte[0x04];

            for (i = 0; i < 0x04; i++)
            {
                TP[i] = (byte)(Ht[0x00 + i] ^ Ht[0x04 + i] ^ Ht[0x0C + i]);
            }
            //--
            sub_add_4B(TP);
            if (false == F_calsel)
            {
                sub_add_4B(Biao_c);
            }
            else
            {
                sub_add_4B(Biao_d);
            }
        }
        /*---------------------------------------------------------------------*/
        void sub_cal_L_A(byte x)//
        {
            sub_Shift_L(x);
            sub_Mixed_A();
        }
        /*---------------------------------------------------------------------*/
        void sub_cal_L_B(byte x)//
        {
            sub_Shift_L(x);
            sub_Mixed_B();
        }
        /*---------------------------------------------------------------------*/
        void sub_cal_R_A(byte x)//
        {
            sub_Shift_R(x);
            sub_Mixed_A();
        }
        /*---------------------------------------------------------------------*/
        void sub_cal_R_B(byte x)//
        {
            sub_Shift_R(x);
            sub_Mixed_B();
        }
        void sub_add(byte[] p, byte[] s)
        {
            byte i, C;
            uint Sum;
            byte[] B = new byte[0x08] { 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98 };
            //--
            C = 0;
            for (i = 0; i < 0x04; i++)
            {
                Sum = (uint)(p[0x03 - i] + s[0x03 - i] + C);
                HashOut[0x00 + i] = (byte)(p[0x03 - i] + s[0x03 - i] + C);
                if (Sum > 0xFF)
                {
                    C = 0x01;
                }
                else
                {
                    C = 0x00;
                }
            }
            //
            C = 0;
            for (i = 0; i < 0x04; i++)
            {
                Sum = (uint)(HashOut[0x00 + i] + B[i] + C);
                HashOut[0x00 + i] = (byte)(HashOut[0x00 + i] + B[i] + C);
                if (Sum > 0xFF)
                {
                    C = 0x01;
                }
                else
                {
                    C = 0x00;
                }
            }
            //--
            C = 0;
            for (i = 0; i < 0x04; i++)
            {
                Sum = (uint)(p[0x07 - i] + s[0x07 - i] + C);
                HashOut[0x04 + i] = (byte)(p[0x07 - i] + s[0x07 - i] + C);
                if (Sum > 0xFF)
                {
                    C = 0x01;
                }
                else
                {
                    C = 0x00;
                }
            }
            //
            C = 0;
            for (i = 0; i < 0x04; i++)
            {
                Sum = (uint)(HashOut[0x04 + i] + B[0x04 + i] + C);
                HashOut[0x04 + i] = (byte)(HashOut[0x04 + i] + B[0x04 + i] + C);
                if (Sum > 0xFF)
                {
                    C = 0x01;
                }
                else
                {
                    C = 0x00;
                }
            }
            //--
        }
    }
}
