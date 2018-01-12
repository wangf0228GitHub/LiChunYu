using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib.USB.HID;
using System.Security.Cryptography;
using System.IO;

namespace 车钥匙
{
    public partial class HIDDevice : Win32Usb, IDisposable
    {
        public delegate void HIDProcCallbackDelegate(int Proc);
        public static HIDProcCallbackDelegate HIDProcCallback = null;
        //public byte[] SwVer=new byte[2]{0x20,0x01};
        public bool CheckDevice()
        {
            InteractionData tx = new InteractionData();

            tx.buff[0] = 0x03;
            tx.buff[1] = 0x55;	//主机-->设备
            InteractionData rx = DeviceInteraction(tx);
            if (rx == null || rx.CheckDataList() == false)
                return false;
            HexID[0] = rx.buff[0x10];
            HexID[1] = rx.buff[0x11];
            HexID[2] = rx.buff[0x12];
            HexID[3] = rx.buff[0x13];
//             if (HexID[0] != 0x01)
//                 return false;
//             if (HexID[0] != 0x04 || HexID[1] != 0x45 || HexID[2] != 0x61 || HexID[3] != 0x67)
//                 return false;
            tx = new InteractionData();
            rx = DeviceInteraction(tx);
            if (rx == null || rx.CheckDataList() == false)
                return false;
            HexVer[0] = rx.buff[0x10];
            HexVer[1] = rx.buff[0x11];
            return true;
        }
        public bool UpdateDevice(byte[] pBuf)
        {
            InteractionData tx = new InteractionData();

            tx.buff[0] = 0x03;
            tx.buff[1] = 0x55;	//主机-->设备
            InteractionData rx = DeviceInteraction(tx);
            if (rx == null || rx.CheckDataList() == false)
                return false;
            HexID[0] = rx.buff[0x10];
            HexID[1] = rx.buff[0x11];
            HexID[2] = rx.buff[0x12];
            HexID[3] = rx.buff[0x13];

            tx = new InteractionData();
            rx = DeviceInteraction(tx);
            if (rx == null || rx.CheckDataList() == false)
                return false;
            HexVer[0] = rx.buff[0x10];
            HexVer[1] = rx.buff[0x11];
            return true;
        }
        public InteractionData DeviceInteraction(InteractionData tx,int n)
        {
            try
            {
                //int rxCount = HIDStream.Read(InputReport, 0, m_nInputReportLength);
                if (HIDProcCallback != null)
                    HIDProcCallback(-2);
                HIDStream.Write(tx.GetDataList(), 0, m_nOutputReportLength);
                byte[] InputReport = new byte[m_nInputReportLength];
                IAsyncResult ia = HIDStream.BeginRead(InputReport, 0, m_nInputReportLength, null, null);
                for (int s = 0; s < n; s++)
                {
                    if (ia.AsyncWaitHandle.WaitOne(500, false))
                    {
                        if (HIDProcCallback != null)
                            HIDProcCallback(-1);
                        InteractionData rx = new InteractionData(InputReport);
                        return rx;
                    }
                    else
                    {
                        if (HIDProcCallback != null)
                            HIDProcCallback(s + 1);
                    }
                }
                if (HIDProcCallback != null)
                    HIDProcCallback(-1);
                HIDStream.Close();
                HIDStream.Dispose();
                HIDStream = null;
                m_hHandle = CreateFile(hidPath, GENERIC_READ | GENERIC_WRITE, 0, IntPtr.Zero, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, IntPtr.Zero);
                HIDStream = new FileStream(m_hHandle, FileAccess.Read | FileAccess.Write, m_nInputReportLength, true);
                return null;
            }
            catch (IOException ex)
            {
                // The device was removed!
                throw new HIDDeviceException(ex.Message + ":Device was removed");
            }
        }
        public InteractionData DeviceInteraction(InteractionData tx)
        {
            return DeviceInteraction(tx, 80);
        }
    }
    public class InteractionData
    {
        public byte first;
        public byte[] buff=new byte[0x40];
        public InteractionData()
        {
            first = 0;
            for (int i = 0; i < 0x40; i++)
            {
                buff[i] = 0;
            }
            buff[8] = 0x01;//HIDDevice.SwVer[1];//L
            buff[9] = 0x20;//HIDDevice.SwVer[0];//H
        }
        public InteractionData(byte[] pBuf)
        {
            first = pBuf[0];
            for (int i = 0; i < 0x40; i++)
            {
                buff[i] = pBuf[i + 1];
            }
        }
        public byte[] GetDataList()
        {
            byte[] list=new byte[65];
            list[0] = first;
            for (int i = 0; i < 0x40; i++)
            {
                list[i + 1] = buff[i];
            }
            return list;
        }
        public bool CheckDataList()
        {
            if (buff[2] != buff[0])
            {
                //GetDlgItem(IDC_Program)->EnableWindow(TRUE);
                //MessageBox("Receive Data Error", "Error", MB_OK);
                return false;
            }
            else
                return true;
        }
    }
    public enum HIDResult
    {
        OK,
        USBError,
        RxError,
        ReadError,
        ProgramError,
        GetPSWError1,
        GetPSWError2,
        GetPSWError3,
        ReadEZSError1,
        ReadEZSError2,
        ReadEZSError3,
        ReadEZSError4,
        ReadEZSError5,
        CANErr,
        CANWriteWait
    }
    public enum KeyOptions
    {
        IRPreparedKey,
        ChipOnThePCB,
        NewPCB,
        ReNew
    }
}
