using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using WFNetLib;
using System.IO;
using System.IO.Ports;
using System.Diagnostics;
using System.Runtime.InteropServices;
namespace KeyTools
{    
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }
        public static SerialPort serialPort;
        EISForm eisForm;
        KeyForm keyForm;
        private void btKeyWork_Click(object sender, EventArgs e)
        {
            panel1.Controls.Clear();
            panel1.Controls.Add(keyForm);
            keyForm.Show();
        }

        private void Form1_Load(object sender, EventArgs e)
        {            
            eisForm = new EISForm(this);
            eisForm.TopLevel = false;

            keyForm = new KeyForm();
            keyForm.TopLevel = false;

            panel1.Controls.Clear();
            panel1.Controls.Add(eisForm);
            eisForm.Show();
        }

        private void btEISWork_Click(object sender, EventArgs e)
        {
            panel1.Controls.Clear();
            panel1.Controls.Add(eisForm);
            eisForm.Show();
        }
        DLLWork.SerialProcCallback serialProcCallback;
        DLLWork.SetProgressCallback setProgressCallback;
        public static bool bConnectedDevice;
        private void MainForm_Shown(object sender, EventArgs e)
        {
            WFGlobal.WaitMS(200);
            DLLWork.MakeDLL();            

            serialProcCallback = new DLLWork.SerialProcCallback(serialProc);
            setProgressCallback = new DLLWork.SetProgressCallback(SetProgressCallBackProc);
            //serialTxCallback = new DLLWork.SerialTxCallback(serialTxProc);
            DLLWork.InitDllWork(serialProcCallback,setProgressCallback);
            bConnectedDevice = false;
             ConnectForm f = new ConnectForm();
             f.ShowDialog();            
        }
        public void SetProgressCallBackProc(int mode, int value)
        {
            this.Invoke((EventHandler)(delegate
            {
                switch (mode)
                {
                    case 0://设定进度条值
                        progressBar1.Value = value;
                        break;
                    case 1:
                        progressBar1.Maximum = value;
                        break;
                    case 2:
                        progressBar1.Minimum = value;
                        break;
                    case 3:
                        progressBar1.Step = value;
                        break;
                    case 4:
                        progressBar1.PerformStep();
                        break;

                }
            }));            
        }
        public void SetWorkStatus(string str)
        {
            this.Invoke((EventHandler)(delegate
            {
                lWorkStatus.Text = str;
            }));
        }
        public bool serialProc(ushort command, IntPtr pBuf, int offset, int count, int rxTimeout, int retry)
        {
            //comm.WriteLine("123213");
            try
            {
                serialPort.DiscardInBuffer();
            }
            catch
            {
                if (!bConnectedDevice)
                {
                    ConnectForm f = new ConnectForm();
                    f.ShowDialog();
                }
                if (!bConnectedDevice)
                    return false;
            }
            serialPort.DiscardInBuffer();
            serialPort.DiscardOutBuffer();
            int nRetry = retry;
            byte[] data = new byte[count];
            Marshal.Copy(pBuf, data, offset, count);
            while (nRetry != 0)
            {
                Debug.WriteLine("发送:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(data, " "));
                serialPort.Write(data, 0, data.Length);
                serialPort.ReadTimeout = rxTimeout;
                while (true)
                {
                    try
                    {
                        if (DLLWork.ProcRxByte((byte)serialPort.ReadByte()))
                        {
                            byte[] rx = new byte[1024];
                            int len = 0;
                            DLLWork.GetRxData(rx, ref len);
                            Debug.WriteLine("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rx, 0, len, " "));
                            //serialPort.Close();
                            return true;
                        }
                    }
                    catch// (Exception ex)
                    {
                        //serialPort.Close();
                        break;
                    }
                }
                nRetry--;
            }
            return false;            
        }
//         public static bool serialTxProc(IntPtr pBuf, int offset, int count)
//         {            
//             byte[] data = new byte[count];
//             Marshal.Copy(pBuf,data,offset, count); 
//             Debug.WriteLine("发送:"+WFNetLib.StringFunc.StringsFunction.byteToHexStr(data, " "));
//             serialPort.Write(data, 0, data.Length);
//             return true;
//         }
    }
}
