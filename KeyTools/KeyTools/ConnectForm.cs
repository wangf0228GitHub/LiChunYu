using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using WFNetLib;
using System.Diagnostics;

namespace KeyTools
{
    public partial class ConnectForm : Form
    {
        public ConnectForm()
        {
            InitializeComponent();
        }

        private void ConnectForm_Shown(object sender, EventArgs e)
        {
            MainForm.serialPort = new SerialPort();
            MainForm.serialPort.PortName = Properties.Settings.Default.SerialPortName;
            MainForm.serialPort.BaudRate = 115200;
            if (WFGlobal.OpenSerialPort(ref MainForm.serialPort,""))
            {
                if (DLLWork.CheckDevice())
                {
                    MainForm.bConnectedDevice = true;
                    label1.Text = "成功连接到硬件设备";
                    progressBar1.Value = progressBar1.Maximum;
                    WFGlobal.WaitMS(2000);
                    this.Close();
                    return;
                }
                else
                    MainForm.serialPort.Close();
            }
            string[] ports = SerialPort.GetPortNames();
            Array.Sort(ports);
            progressBar1.Maximum = ports.Length-1;
            progressBar1.Value = 0;
            foreach (string port in ports)
            {
                Debug.WriteLine(port);
                progressBar1.PerformStep();
                WFGlobal.WaitMS(1);
                MainForm.serialPort.PortName = port;
                MainForm.serialPort.BaudRate = 115200;
                if (WFGlobal.OpenSerialPort(ref MainForm.serialPort, ""))
                {
                    if (DLLWork.CheckDevice())
                    {
                        Properties.Settings.Default.SerialPortName = port;
                        Properties.Settings.Default.Save();
                        MainForm.bConnectedDevice = true;
                        label1.Text = "成功连接到硬件设备";
                        progressBar1.Value = progressBar1.Maximum;
                        WFGlobal.WaitMS(2000);
                        this.Close();
                        return;
                    }
                    else
                        MainForm.serialPort.Close();
                }                
            }
            label1.Text = "未找到设备，请重新连接硬件设备";
            WFGlobal.WaitMS(2000);
            this.Close();
        }
    }
}
