using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib;

namespace KeyTools
{
    public class EISData
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
        public string VIN;
        public EISData()
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
        }
        public void SetAllData(byte[] pBuf,int len)
        {
            AllFile=new byte[len];
            Array.Copy(pBuf, AllFile, len);
            Array.Copy(pBuf, 0, SSID, 0,4);

            Array.Copy(pBuf, 0x0270, Key0, 0, 8);
            Array.Copy(pBuf, 0x0280, Key1, 0, 8);
            Array.Copy(pBuf, 0x0290, Key2, 0, 8);
            Array.Copy(pBuf, 0x02a0, Key3, 0, 8);
            Array.Copy(pBuf, 0x02b0, Key4, 0, 8);
            Array.Copy(pBuf, 0x02c0, Key5, 0, 8);
            Array.Copy(pBuf, 0x02d0, Key6, 0, 8);
            Array.Copy(pBuf, 0x02e0, Key7, 0, 8);

            Array.Copy(pBuf, 0x0230, SerialNum, 0, 8);

            Array.Copy(pBuf, 0x10, Dealer, 0, 8);

            Array.Copy(pBuf, 0x0260, Transport, 0, 8);

            if (BytesOP.GetBit(pBuf[4], 7))
                Key0Status = KeyStatus.Lock;
            else
                Key0Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 6))
                Key1Status = KeyStatus.Lock;
            else
                Key1Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 5))
                Key2Status = KeyStatus.Lock;
            else
                Key2Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 4))
                Key3Status = KeyStatus.Lock;
            else
                Key3Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 3))
                Key4Status = KeyStatus.Lock;
            else
                Key4Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 2))
                Key5Status = KeyStatus.Lock;
            else
                Key5Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 1))
                Key6Status = KeyStatus.Lock;
            else
                Key6Status = KeyStatus.Used;

            if (BytesOP.GetBit(pBuf[4], 0))
                Key7Status = KeyStatus.Lock;
            else
                Key7Status = KeyStatus.Used;

            bInitialised = BytesOP.GetBit(pBuf[5], 0);
            bTPremoved = BytesOP.GetBit(pBuf[5], 1);
            bPersonalised = BytesOP.GetBit(pBuf[5], 2);
            bActivated = BytesOP.GetBit(pBuf[5], 3);

            LastKey = BytesOP.GetLowNibble(pBuf[7]);
            PreviousKey = BytesOP.GetHighNibble(pBuf[7]);


        }
    }
    public enum KeyStatus
    {
        Used,
        Lock,
        NoUsed
    }
}
