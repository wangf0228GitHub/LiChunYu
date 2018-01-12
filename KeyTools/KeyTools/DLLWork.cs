using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using WFNetLib;
using System.Reflection;
using System.IO;

namespace KeyTools
{
    public class DLLWork
    {
        public const string DLLName = "KeyDAL.dll";
        
        //public delegate bool SerialTxCallback(IntPtr pBuf, int offset, int count);
        public delegate bool SerialProcCallback(ushort command,IntPtr pBuf,int offset, int count,int rxTimeout,int retry);
        public delegate void SetProgressCallback(int mode, int value);


        [DllImport(DLLName)]
        public static extern void InitDllWork(SerialProcCallback _SerialProcCallback,SetProgressCallback _SetProgressCallback);

        [DllImport(DLLName)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        public static extern bool ProcRxByte(byte rx);

        [DllImport(DLLName)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        public static extern bool CheckDevice();

        [DllImport(DLLName)]
        public static extern void GetRxData(byte[] pBuf,ref int len);

        [DllImport(DLLName)]
        public static extern int GetErrorCode();

        /************************************************************************/
        /* can读写相关                                                          */
        /************************************************************************/
        [DllImport(DLLName)]
        public static extern bool SetCANBaudRate(int brType);

        [DllImport(DLLName)]
        [return: MarshalAsAttribute(UnmanagedType.I1)]
        public static extern bool CANReadEEPROMProc(int mode);

        [DllImport(DLLName)]
        public static extern void GetEEPROMData(byte[] pBuf, ref int len);


        /************************************************************************/
        /* 红外相关                                                             */
        /************************************************************************/
        [DllImport(DLLName)]
        public static extern bool IREn(int bOpen);


        [DllImport(DLLName)]
        public static extern bool IRWork(int bOpen);

        public static void MakeDLL()
        {
            if (FileOP.IsExist(DLLWork.DLLName, FileOPMethod.File))
            {
                FileOP.Delete(DLLWork.DLLName, FileOPMethod.File);
            }
            Type type = MethodBase.GetCurrentMethod().DeclaringType;
            string _namespace = type.Namespace;
            //获得当前运行的Assembly   
            Assembly _assembly = Assembly.GetExecutingAssembly();
            //根据名称空间和文件名生成资源名称   
#if DEBUG
            string resourceName = _namespace + ".DebugDLL.KeyDAL.dll";// +DLLWork.DLLName;
            System.IO.Stream so = new System.IO.FileStream(DLLWork.DLLName, System.IO.FileMode.Create);
#else
            string resourceName = _namespace + ".ReleaseDLL.KeyDAL.vmp.dll";// + DLLWork.DLLName;
            System.IO.Stream so = new System.IO.FileStream(DLLWork.DLLName, System.IO.FileMode.Create);
#endif
            //根据资源名称从Assembly中获取此资源的Stream   
            Stream stream = _assembly.GetManifestResourceStream(resourceName);

            //byte[] dll=Properties.Resources.KeyDAL;
            
            //so.Write(dll, 0, dll.Length);
            stream.CopyTo(so);
            so.Close();
        }
// #if DEBUG
//         string resourceName = _namespace + ".DebugDLL." + DLLWork.DLLName;
//         System.IO.Stream so = new System.IO.FileStream(DLLWork.DLLName, System.IO.FileMode.Create);
// #else
//             string resourceName = _namespace + ".DebugDLL." + DLLWork.DLLName;
//             System.IO.Stream so = new System.IO.FileStream(DLLWork.DLLName, System.IO.FileMode.Create);
// #endif 
    }
}
