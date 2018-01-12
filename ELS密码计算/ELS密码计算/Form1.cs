using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using FTD2XX_NET;
using System.Threading;
using WFNetLib;

namespace ELS密码计算
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            RefreshDevice();   
        }
        string[] FTDISerialNumber;
        void RefreshDevice()
        {
            FTDI_combo.Items.Clear();
            FTDISerialNumber = null;
            UInt32 ftdiDeviceCount = 0;
            FTDI.FT_STATUS ftStatus = FTDI.FT_STATUS.FT_OK;
            FTDI myFtdiDevice = new FTDI();

            // Determine the number of FTDI devices connected to the machine
            ftStatus = myFtdiDevice.GetNumberOfDevices(ref ftdiDeviceCount);
            // Check status
            if (ftStatus == FTDI.FT_STATUS.FT_OK)
            {

            }
            else
            {
                // Wait for a key press   
                MessageBox.Show("error :" + ftStatus.ToString());
                return;
            }
            if (ftdiDeviceCount == 0)
            {
                // Wait for a key press                
                return;
            }
            FTDI.FT_DEVICE_INFO_NODE[] ftdiDeviceList = new FTDI.FT_DEVICE_INFO_NODE[ftdiDeviceCount];

            // Populate our device list
            ftStatus = myFtdiDevice.GetDeviceList(ftdiDeviceList);
            if (ftStatus == FTDI.FT_STATUS.FT_OK)
            {
                FTDISerialNumber = new string[ftdiDeviceCount];
                comboBox1.Items.Clear();
                for (UInt32 i = 0; i < ftdiDeviceCount; i++)
                {
                    FTDI_combo.Items.Add(ftdiDeviceList[i].Description.ToString());
                    FTDISerialNumber[i] = ftdiDeviceList[i].SerialNumber.ToString();
                    comboBox1.Items.Add(FTDISerialNumber[i]);
                }
            }
            else
            {
                MessageBox.Show("error :" + ftStatus.ToString());
                return;
            }
            FTDI_combo.SelectedIndex = 0;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {

            RefreshDevice();
        }
        byte[] psw;
        private void button1_Click(object sender, EventArgs e)
        {
            //001054
            textBox3.Text = FTDISerialNumber[DeviceIndex];
            try
            {
                if (FTDISerialNumber == null)
                {
                    MessageBox.Show("未找到硬件设备");
                    return;
                }
                psw = WFNetLib.Strings.StringsFunction.strToHexByte(textBox1.Text, " ");
                if (psw.Length != 8)
                {
                    MessageBox.Show("密码区内容格式不对");
                    return;
                }
                Thread thread = new Thread(new ThreadStart(runMethod));
                bFind = false;
                thread.Start();
                DeviceIndex = FTDI_combo.SelectedIndex;                
                
                DateTime dt1, dt2;
                dt1 = DateTime.Now;
                int s = 28;
                progressBar1.Maximum = 28;
                progressBar1.Value = 28;
                while (!bFind)
                {
                    dt2 = DateTime.Now;
                    if (dt2 > dt1.AddSeconds(1))
                    {
                        dt1 = DateTime.Now;
                        s--;
                        progressBar1.Value = s;
                        textBox2.Text = "";
                    }
                }
                if (numBytesRead != 11)
                {
                    MessageBox.Show("计算密码失败");
                    return;
                }
                textBox2.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(rxbuf, 2, 8, " ");
                progressBar1.Value = 0;
            }
            catch
            {
            	
            }
        }
        bool bFind;
        int DeviceIndex;
        byte[] rxbuf;
        UInt32 numBytesRead = 0;
        void runMethod()
        {
            try
            {
                FTDI.FT_STATUS ftStatus = FTDI.FT_STATUS.FT_OK;
                FTDI myFtdiDevice = new FTDI();
                ftStatus = myFtdiDevice.OpenBySerialNumber(FTDISerialNumber[DeviceIndex]);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                ftStatus = myFtdiDevice.ResetDevice();
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                ftStatus = myFtdiDevice.SetBaudRate(300);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                ftStatus = myFtdiDevice.SetTimeouts(30000, 30000);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                byte[] txbuf = new byte[100];
                txbuf[0] = 0xf0;
                ftStatus = myFtdiDevice.Write(txbuf, 1, ref numBytesRead);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                Thread.Sleep(200);
                txbuf[0] = 0x30;
                txbuf[1] = 0x00;
                for (int i = 0; i < 8; i++)
                    txbuf[2 + i] = psw[i];
                txbuf[10] = WFNetLib.Verify.GetVerify_byteSum(txbuf, 10);
                ftStatus = myFtdiDevice.Write(txbuf, 11, ref numBytesRead);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                rxbuf = new byte[100];
                ftStatus = myFtdiDevice.Read(rxbuf, 11, ref numBytesRead);
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                }
                ftStatus = myFtdiDevice.Close();
                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                {
                    MessageBox.Show("error :" + ftStatus.ToString());
                    return;
                } 
            }
            catch (System.Exception ex)
            {
            	
            }
            finally
            {
                bFind = true;
            }       
        }
    }
}
