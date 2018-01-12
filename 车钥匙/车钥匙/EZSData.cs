using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib;

namespace 车钥匙
{
    public class EZSData
    {
        public byte[] SSID;
        public byte[] PSW;
        public byte[] Key0;
        public byte[] Key1;
        public byte[] Key2;
        public byte[] Key3;
        public byte[] Key4;
        public byte[] Key5;
        public byte[] Key6;
        public byte[] Key7;
        public byte[] SerialNum;
        public byte[] Dealer;
        public byte[] Transport;
        public KeyStatus Key0Status;
        public KeyStatus Key1Status;
        public KeyStatus Key2Status;
        public KeyStatus Key3Status;
        public KeyStatus Key4Status;
        public KeyStatus Key5Status;
        public KeyStatus Key6Status;
        public KeyStatus Key7Status;
        public byte Status;
        public bool bInitialised;
        public bool bTPremoved;
        public bool bPersonalised;
        public bool bActivated;
        public byte LastKey;
        public byte PreviousKey;
        public byte[] AllFile;
        public void SetPSW(byte[] pBuf)
        {
            for (int i = 0; i < 8; i++)
            {
                PSW[i] = pBuf[1 + i];
            }
        }
        public EZSData()
        {
            SSID = new byte[4];
            PSW = new byte[8];
            Key0 = new byte[8];
            Key1 = new byte[8];
            Key2 = new byte[8];
            Key3 = new byte[8];
            Key4 = new byte[8];
            Key5 = new byte[8];
            Key6 = new byte[8];
            Key7 = new byte[8];
            SerialNum = new byte[8];
            Dealer = new byte[8];
            Transport = new byte[8];
            //AllFile=new byte[0x400];
        }
        public EZSData(byte[] pBuf)
        {
            SSID = new byte[4];
            PSW = new byte[8];
            Key0 = new byte[8];
            Key1 = new byte[8];
            Key2 = new byte[8];
            Key3 = new byte[8];
            Key4 = new byte[8];
            Key5 = new byte[8];
            Key6 = new byte[8];
            Key7 = new byte[8];
            SerialNum = new byte[8];
            Dealer = new byte[8];
            Transport = new byte[8];
            for (int i = 0; i < 4; i++)
            {
                SSID[i] = pBuf[0x0BE + i];
            }
            int BaseAddress = 0x1ac - 0x12;
            for (int i = 0; i < 8; i++)
            {
                Dealer[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key0[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key1[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key2[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key3[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key4[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key5[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key6[i] = pBuf[BaseAddress + i];
            }
            BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Key7[i] = pBuf[BaseAddress + i];
            }
            //BaseAddress += 0x12;
            for (int i = 0; i < 8; i++)
            {
                Transport[i] = pBuf[0xa6 + i];
            }
            for (int i = 0; i < 8; i++)
            {
                SerialNum[i] = pBuf[0xb6 + i];
            }
            for (int i = 0; i < 8; i++)
            {
                PSW[i] = pBuf[0x8e + i];
            }
            byte KeyStatus1, KeyStatus2;
            KeyStatus1 = 0;//pBuf[4];
            KeyStatus2 = 0;//pBuf[6];
            if (BytesOP.GetBit(KeyStatus1, 7))
                Key0Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 7))
                    Key0Status = KeyStatus.NoUsed;
                else
                    Key0Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 6))
                Key1Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 6))
                    Key1Status = KeyStatus.NoUsed;
                else
                    Key1Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 5))
                Key2Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 5))
                    Key2Status = KeyStatus.NoUsed;
                else
                    Key2Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 4))
                Key3Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 4))
                    Key3Status = KeyStatus.NoUsed;
                else
                    Key3Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 3))
                Key4Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 3))
                    Key4Status = KeyStatus.NoUsed;
                else
                    Key4Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 2))
                Key5Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 2))
                    Key5Status = KeyStatus.NoUsed;
                else
                    Key5Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 1))
                Key6Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 1))
                    Key6Status = KeyStatus.NoUsed;
                else
                    Key6Status = KeyStatus.Used;
            }
            if (BytesOP.GetBit(KeyStatus1, 0))
                Key7Status = KeyStatus.Lock;
            else
            {
                if (BytesOP.GetBit(KeyStatus2, 0))
                    Key7Status = KeyStatus.NoUsed;
                else
                    Key7Status = KeyStatus.Used;
            }
            Status = pBuf[0x080]; ;
            if (BytesOP.GetBit(Status, 0))
                bInitialised = true;
            else
                bInitialised = false;
            if (BytesOP.GetBit(Status, 1))
                bTPremoved = true;
            else
                bTPremoved = false;
            if (BytesOP.GetBit(Status, 2))
                bPersonalised = true;
            else
                bPersonalised = false;
            if (BytesOP.GetBit(Status, 3))
                bActivated = true;
            else
                bActivated = false;
            //byte usedkey = BytesOP.MakeByte(BytesOP.GetLowNibble(pBuf[0x0DA + 8]), BytesOP.GetLowNibble(pBuf[0x0D8 + 8]));
            LastKey = BytesOP.GetLowNibble(pBuf[0xd8+8]);
            PreviousKey = BytesOP.GetHighNibble(pBuf[0xda+8]);
        }
        public byte[] SaveEZS()
        {
           return SaveEZS(true);
        }
        public byte[] SaveEZS(bool bBig)
        {
            byte[] outBuf = new byte[0x400];	//最大64M内存
            for (int i = 0; i < 0x400; i++)
                outBuf[i] = 0xff;
            byte[] outBufL = new byte[0x800];	//最大64M内存
            for (int i = 0; i < 0x800; i++)
                outBufL[i] = 0xff;
            //
            for (int i = 0; i < 0x100; i++)
            {
                outBuf[i] = 0x00;
            }
            //
            int BaseAddress = 0x100;
            for (int i = 0; i < 10; i++)
            {
                outBuf[BaseAddress] = 0x07;
                BaseAddress += 0x12;
            }
            //
            BaseAddress = 0x1A2;
            for (int i = 0; i < 10; i++)
            {
                outBuf[BaseAddress] = 0x0F;
                BaseAddress += 0x12;
            }
            //
            BaseAddress = 0x244;
            for (int i = 0; i < 6; i++)
            {
                outBuf[BaseAddress] = 0x03;
                BaseAddress += 0x12;
            }
            //
            BaseAddress = 0x2B0;
            for (int i = 0; i < 11; i++)
            {
                outBuf[BaseAddress] = 0x03;
                BaseAddress += 0x12;
            }
            //
            BaseAddress = 0x104;
            for (int i = 0; i < 18; i++)
            {
                outBuf[BaseAddress] = 0x00;
                BaseAddress += 0x12;
            }
            //
            BaseAddress = 0x108;
            for (int i = 0; i < 18; i++)
            {
                outBuf[BaseAddress] = 0x3F;
                BaseAddress += 0x12;
            }
            //
            for (int i = 0; i < (0x3EF - 0x376); i++)
            {
                outBuf[0x376 + i] = 0x00;
            }
            outBuf[0x001] = 0x5A;
            outBuf[0x0C2] = 0xAA;
            outBuf[0x0C3] = 0xFF;
            outBuf[0x0C5] = 0xAA;
            outBuf[0x3EE] = 0x03;
            //PSW
            for (int i = 0; i < 8; i++)
            {
                outBuf[0x8E + i] = PSW[i];
            }
            //SSID
            for (int i = 0; i < 4; i++)
            {
                outBuf[0x0BE + i] = SSID[i];
            }
            //KEY0,...7,dealer
            BaseAddress = 0x10A;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key0[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key0);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key1[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key1);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key2[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key2);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key3[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key3);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key4[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key4);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key5[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key5);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key6[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key6);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key7[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key7);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Dealer[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Dealer);
            BaseAddress += 0x12;
            //KEY0,...7,
            //BaseAddress=0x10A;
            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key0[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key0);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key1[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key1);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key2[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key2);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key3[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key3);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key4[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key4);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key5[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key5);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key6[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key6);
            BaseAddress += 0x12;

            for (int i = 0; i < 8; i++)
            {
                outBuf[BaseAddress + i] = Key7[i];
            }
            outBuf[BaseAddress - 0x008] = Verify.GetVerify_byteSum(Key7);
            BaseAddress += 0x12;


            //Transport protec
            if(Transport[0]==0x00 &&
                Transport[1]==0x00 &&
                Transport[2]==0x00 &&
                Transport[3]==0x00 &&
                Transport[4]==0x00 &&
                Transport[5]==0x00 &&
                Transport[6]==0x00 &&
                Transport[7] == 0x00)
            {
                //C2 E9 01 F0 ED BA 7C 5C
                outBuf[0x0A6 + 0]=0xC2;
                outBuf[0x0A6 + 1]=0xE9;
                outBuf[0x0A6 + 2]=0x01;
                outBuf[0x0A6 + 3]=0xF0;
                outBuf[0x0A6 + 4]=0xED;
                outBuf[0x0A6 + 5]=0xBA;
                outBuf[0x0A6 + 6]=0x7C;
                outBuf[0x0A6 + 7]=0x5C;
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    outBuf[0x0A6 + i] = Transport[i];
                }
            }
            //Serial Numbe
            for (int i = 0; i < 8; i++)
            {
                outBuf[0x0B6 + i] = SerialNum[i];
            }
            //used keys
            //CvtIn[0]=RDData[0x08*0x00+7];
            //CNT = 1;
            //cal_asic(CNT,CvtIn,CvtOut);
            outBuf[0x0D8] = LastKey;
            outBuf[0x0DA] = PreviousKey;
            //Operational status
            outBuf[0x080] = Status;
            //
            for (int i = 0; i < 0x10; i++)
            {
                outBuf[0x0C6 + i] = 0x00;
            }
            //
            for (int i = 0, k = 0; i < 0x400; )
            {
                outBufL[k++] = outBuf[i++];
                outBufL[k++] = outBuf[i++];
                k += 2;
            }
            if (bBig)
                return outBufL;
            else
                return outBuf;
        }
    }
    public enum KeyStatus
    {
        Used,
        Lock,
        NoUsed
    }
}
