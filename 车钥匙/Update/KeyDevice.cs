using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib.USB.HID;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;
using System.IO;
using System.Threading;

namespace Update
{
    public partial class HIDDevice : Win32Usb, IDisposable
    {
        public static HIDDevice TheHIDDevice=null;
        public static readonly ushort VendorID = 0x03D8;
        public static readonly ushort ProductID = 0x000D;
        public static byte[] SwVer = new byte[2] { 0x20, 0x01 };
        private FileStream HIDStream;
        private int m_nInputReportLength;
        private int m_nOutputReportLength;
        public SafeFileHandle m_hHandle;        
        public byte[] HexVer;
        public byte[] HexID;
        public HIDDevice()
        {
            HexVer = new byte[2];          
            HexID = new byte[4];
        }
        string hidPath;
        private void Initialise(string strPath)
        {
            // Create the file from the device path//FILE_FLAG_OVERLAPPED      
            hidPath = strPath;
            m_hHandle = CreateFile(strPath, GENERIC_READ | GENERIC_WRITE, 0, IntPtr.Zero, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, IntPtr.Zero);
            if (!m_hHandle.IsInvalid)	// if the open worked...
            {
                IntPtr lpData;
                if (HidD_GetPreparsedData(m_hHandle.DangerousGetHandle(), out lpData))	// get windows to read the device data into an internal buffer
                {
                    try
                    {
                        HidCaps oCaps;
                        HidP_GetCaps(lpData, out oCaps);	// extract the device capabilities from the internal buffer
                        m_nInputReportLength = oCaps.InputReportByteLength;	// get the input...
                        m_nOutputReportLength = oCaps.OutputReportByteLength;	// ... and output report lengths
                        HIDStream = new FileStream(m_hHandle, FileAccess.Read | FileAccess.Write, m_nInputReportLength, true);
                    }
                    finally
                    {
                        HidD_FreePreparsedData(ref lpData);	// before we quit the funtion, we must free the internal buffer reserved in GetPreparsedData
                    }
                }
                else	// GetPreparsedData failed? Chuck an exception
                {
                    throw HIDDeviceException.GenerateWithWinError("GetPreparsedData failed");
                }
            }
            else	// File open failed? Chuck an exception
            {
                //m_hHandle = IntPtr.Zero;
                m_hHandle.Close();
                throw HIDDeviceException.GenerateWithWinError("Failed to create device file");
            }
        }        
        public static HIDDevice FindDevice()
        {
            string strPath = string.Empty;
            string strSearch = string.Format("vid_{0:x4}&pid_{1:x4}", VendorID, ProductID); // first, build the path search string
            Guid gHid;
            HidD_GetHidGuid(out gHid);	// next, get the GUID from Windows that it uses to represent the HID USB interface
            IntPtr hInfoSet = SetupDiGetClassDevs(ref gHid, null, IntPtr.Zero, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);	// this gets a list of all HID devices currently connected to the computer (InfoSet)
            try
            {
                DeviceInterfaceData oInterface = new DeviceInterfaceData();	// build up a device interface data block
                oInterface.Size = Marshal.SizeOf(oInterface);
                // Now iterate through the InfoSet memory block assigned within Windows in the call to SetupDiGetClassDevs
                // to get device details for each device connected
                int nIndex = 0;
                while (SetupDiEnumDeviceInterfaces(hInfoSet, 0, ref gHid, (uint)nIndex, ref oInterface))	// this gets the device interface information for a device at index 'nIndex' in the memory block
                {
                    string strDevicePath = GetDevicePath(hInfoSet, ref oInterface);	// get the device path (see helper method 'GetDevicePath')
                    if (strDevicePath.IndexOf(strSearch) >= 0)	// do a string search, if we find the VID/PID string then we found our device!
                    {
                        HIDDevice oNewDevice = null;
                        try
                        {
                            int retry = 3;
                            while(retry!=0)
                            {
                                //Thread.Sleep(3000);
                                oNewDevice = new HIDDevice();	// create an instance of the class for this device
                                oNewDevice.Initialise(strDevicePath);	// initialise it with the device path
                                if (oNewDevice.CheckDevice())
                                    return oNewDevice;	// and return it
                                else
                                {
                                    oNewDevice.Dispose();
                                    oNewDevice = null;
                                }
                                retry--;
                            }
                        }
                        catch
                        {
                            if (oNewDevice != null)
                            {
                                oNewDevice.Dispose();
                                oNewDevice = null;
                            }
                        }
                    }
                    nIndex++;	// if we get here, we didn't find our device. So move on to the next one.
                }
            }
            finally
            {
                // Before we go, we have to free up the InfoSet memory reserved by SetupDiGetClassDevs
                SetupDiDestroyDeviceInfoList(hInfoSet);
            }
            return null;	// oops, didn't find our device
        }
        #region IDisposable Members
        /// <summary>
        /// Dispose method
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        /// <summary>
        /// Disposer called by both dispose and finalise
        /// </summary>
        /// <param name="bDisposing">True if disposing</param>
        protected virtual void Dispose(bool bDisposing)
        {
            if (bDisposing)	// if we are disposing, need to close the managed resources
            {
                if (HIDStream != null)
                {
                    HIDStream.Close();
                    HIDStream = null;
                }
            }
            if (!m_hHandle.IsInvalid)	// Dispose and finalize, get rid of unmanaged resources
            {
                m_hHandle.Close();
                m_hHandle.Dispose();
                m_hHandle = null;
                //CloseHandle(m_hHandle.DangerousGetHandle());
            }
        }
        #endregion
    }    
}
