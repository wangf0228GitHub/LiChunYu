using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib.USB.HID;

namespace 车钥匙
{
    public partial class HIDDevice : Win32Usb, IDisposable
    {
        byte[] KEY = new byte[8];
        byte[] HOP = new byte[4];
        byte[] B=new byte[4]{0x2E,0x74,0x5C,0x3A};
        public bool Check_mima()
        {
            InteractionData tx = new InteractionData();

            tx.buff[0] = 0x03;
            tx.buff[1] = 0x55;	//主机-->设备
            InteractionData rx = DeviceInteraction(tx);
            if (rx == null || rx.CheckDataList() == false)
                return false;
            KEY[0] = 0x02;
            KEY[1] = 0x58;
            KEY[2] = 0xBE;
            KEY[3] = 0x24;
//             KEY[4] = 0x8A;
//             KEY[5] = 0x95;
//             KEY[6] = 0x60;
//             KEY[7] = 0x3D;

            KEY[4] = (byte)(HexID[0] - 0x77);
            KEY[5] = (byte)(HexID[1] - 0x55);
            KEY[6] = (byte)(HexID[2] - 0x33);
            KEY[7] = (byte)(HexID[3] - 0x11);

            HOP[0] = rx.buff[0x14];
            HOP[1] = rx.buff[0x15];
            HOP[2] = rx.buff[0x16];
            HOP[3] = rx.buff[0x17];

            DECRYPT();
            if ((HOP[0x02] == rx.buff[0x18]) && (HOP[3] == rx.buff[0x19]))
                return true;
            else
                return false;
        }
        public void DECRYPT()
        {
            int CNT0 = 0;
            byte j = 5;
            byte i;
            byte MASK;
            byte tmp_K;
            tmp_K = 0;
            while (CNT0 < 528) //0x210
            {
                if ((CNT0 & 0x0007) == 0)
                {
                    if (j == 7)
                    {
                        j = 0;
                    }
                    else
                    {
                        j++;
                    }
                    tmp_K = KEY[j];
                }

                if ((HOP[1] & 0x08) == 0x08)
                {
                    MASK = 0x10;
                }
                else
                {
                    MASK = 0x01;
                }
                if ((HOP[2] & 0x01) == 0x01)
                {
                    MASK = (byte)(MASK << 2);
                }
                if ((HOP[3] & 0x01) == 0x01)
                {
                    MASK = (byte)(MASK << 1);
                }

                if ((HOP[0] & 0x02) == 0x02)
                {
                    i = 0x01;
                }
                else
                {
                    i = 0x00;
                }

                if ((HOP[0] & 0x40) == 0x40)
                {
                    i = (byte)(i + 0x02);
                }

                if ((MASK & B[i]) == 0)
                {
                    MASK = 0x00;
                }
                else
                {
                    MASK = 0x80;
                }
                MASK = (byte)(MASK ^ HOP[2] ^ HOP[0] ^ tmp_K);

                HOP[0] = (byte)(HOP[0] << 1);
                if ((HOP[1] & 0x80) == 0x80)
                {
                    HOP[0] = (byte)(HOP[0] | 0x01);
                }
                else
                {
                    HOP[0] = (byte)(HOP[0] & 0xFE);
                }

                HOP[1] = (byte)(HOP[1] << 1);
                if ((HOP[2] & 0x80) == 0x80)
                {
                    HOP[1] = (byte)(HOP[1] | 0x01);
                }
                else
                {
                    HOP[1] = (byte)(HOP[1] & 0xFE);
                }

                HOP[2] = (byte)(HOP[2] << 1);
                if ((HOP[3] & 0x80) == 0x80)
                {
                    HOP[2] = (byte)(HOP[2] | 0x01);
                }
                else
                {
                    HOP[2] = (byte)(HOP[2] & 0xFE);
                }

                HOP[3] = (byte)(HOP[3] << 1);
                if ((MASK & 0x80) == 0x80)
                {
                    HOP[3] = (byte)(HOP[3] | 0x01);
                }
                else
                {
                    HOP[3] = (byte)(HOP[3] & 0xFE);
                }

                tmp_K = (byte)(tmp_K << 1);
                CNT0++;
            }
        }
        //-------------------------------------------------------------------------
        /*---------------------------------------------------------------------*/
        /*---HOP4----HOP3----HOP2----HOP1---*/
        /*---HOP[0]--HOP[1]--HOP[2]--HOP[3]-*/
        /*---KEY7----KEY6----KEY5----KEY4----KEY3----KEY2----KEY1----KEY0---*/
        /*---KEY[0]--KEY[1]--KEY[2]--KEY[3]--KEY[4]--KEY[5]--KEY[6]--KEY[7]-*/
        /*---------------------------------------------------------------------*/
        void ENCODE()
        {
            int CNT0 = 0;
            byte j = 0;
            byte i;
            byte MASK;
            byte tmp_K;
            tmp_K = 0;
            while (CNT0 < 528) //0x210
            {
                if ((CNT0 & 0x0007) == 0)
                {
                    if (j == 0)
                    {
                        j = 7;
                    }
                    else
                    {
                        j--;
                    }
                    tmp_K = KEY[j];
                }

                if ((HOP[1] & 0x10) == 0x10)
                {
                    MASK = 0x10;
                }
                else
                {
                    MASK = 0x01;
                }
                if ((HOP[2] & 0x02) == 0x02)
                {
                    MASK = (byte)(MASK << 2);
                }
                if ((HOP[3] & 0x02) == 0x02)
                {
                    MASK = (byte)(MASK << 1);
                }

                if ((HOP[0] & 0x04) == 0x04)
                {
                    i = 0x01;
                }
                else
                {
                    i = 0x00;
                }

                if ((HOP[0] & 0x80) == 0x80)
                {
                    i = (byte)(i + 0x02);
                }

                if ((MASK & B[i]) == 0)
                {
                    MASK = 0x00;
                }
                else
                {
                    MASK = 0x01;
                }

                MASK = (byte)(MASK ^ HOP[1] ^ HOP[3] ^ tmp_K);

                HOP[3] = (byte)(HOP[3] >> 1);
                if ((HOP[2] & 0x01) == 0x01)
                {
                    HOP[3] = (byte)(HOP[3] | 0x80);
                }
                else
                {
                    HOP[3] = (byte)(HOP[3] & 0x7F);
                }

                HOP[2] = (byte)(HOP[2] >> 1);
                if ((HOP[1] & 0x01) == 0x01)
                {
                    HOP[2] = (byte)(HOP[2] | 0x80);
                }
                else
                {
                    HOP[2] = (byte)(HOP[2] & 0x7F);
                }

                HOP[1] = (byte)(HOP[1] >> 1);
                if ((HOP[0] & 0x01) == 0x01)
                {
                    HOP[1] = (byte)(HOP[1] | 0x80);
                }
                else
                {
                    HOP[1] = (byte)(HOP[1] & 0x7F);
                }

                HOP[0] = (byte)(HOP[0] >> 1);
                if ((MASK & 0x01) == 0x01)
                {
                    HOP[0] = (byte)(HOP[0] | 0x80);
                }
                else
                {
                    HOP[0] = (byte)(HOP[0] & 0x7F);
                }

                tmp_K = (byte)(tmp_K >> 1);
                CNT0++;
            }

        }
    }
}
