using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using WFNetLib;
using WFNetLib.USB.HID;
using System.Threading;

namespace 车钥匙
{
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }
        void HIDProcCallback(int Proc)
        {
            if (Proc>0)
            {
                try
                { 
                    progressBar1.Value = 100 - (int)(Proc * 2.5);
                    if(Proc%2==0)
                        toolStripStatusLabel2.Text = "请在 " + (20 - Proc / 2).ToString() + " 秒内将红外钥匙插入点火开关中";                  
                }
                catch { }
            }
        }
        IntPtr m_hUsbEventHandle;
        private void Form3_Load(object sender, EventArgs e)
        {
            //saveFileDialog1.ShowDialog();
            this.Icon = Properties.Resources.MI;
            if (HIDDevice.TheHIDDevice != null)
            {
                toolStripStatusLabel1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(HIDDevice.TheHIDDevice.HexID, "");
            }
            else
            {
                this.Close();
            }
            toolStripComboBox1.SelectedIndex = 0;
            progressBar1.Visible = false;
            progressBar1.Value = 100;
            m_hUsbEventHandle = Win32Usb.RegisterForUsbEvents(Handle, Win32Usb.HIDGuid);
        }        
        EZSData ezs = null;
        private void button3_Click(object sender, EventArgs e)
        {
            FileInfo f;
            byte[] pBuf,pBufL;
            openFileDialog1.Filter = "EZS Files(*.EZS)|*.EZS|BIN Files(*.BIN)|*.BIN|All Files(*.*)|*.*";
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            pBufL = new byte[stream.Length];
            stream.Read(pBufL, 0, (int)stream.Length);
            stream.Close();
            if (pBufL.Length == 0x800)
            {
                pBuf = new byte[0x400];
                for (int i = 0, k = 0; k < 0x400; )
                {
                    pBuf[k++] = pBufL[i++];
                    pBuf[k++] = pBufL[i++];
                    i += 2;
                }
                int BaseAddress = 0x10A;
                for (int j = 0; j < 9; j++)
                {
                    byte tmpSUM;
                    tmpSUM = Verify.GetVerify_byteSum(pBuf, BaseAddress, 8);
                    if (pBuf[BaseAddress - 0x008] != tmpSUM)
                    {
                        MessageBox.Show("数据校验错误!");
                        return;
                    }
                    BaseAddress += 0x12;
                }
                for (int j = 0; j < 8; j++)
                {
                    byte tmpSUM;
                    tmpSUM = Verify.GetVerify_byteSum(pBuf, BaseAddress, 8);
                    if (pBuf[BaseAddress - 0x008] != tmpSUM)
                    {
                        MessageBox.Show("数据校验错误!");
                        return;
                    }
                    BaseAddress += 0x12;
                }
                ezs = new EZSData(pBuf);
                textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, " ");
                textBox2.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.PSW, " ");
                textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key0, " ");
                textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key1, " ");
                textBox5.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key2, " ");
                textBox6.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key3, " ");
                textBox7.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key4, " ");
                textBox8.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key5, " ");
                textBox9.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key6, " ");
                textBox10.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key7, " ");
                textBox11.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SerialNum, " ");
                textBox12.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Dealer, " ");
                textBox13.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Transport, " ");

                textBox16.Text = ezs.Key0Status.ToString();
                textBox17.Text = ezs.Key1Status.ToString();
                textBox18.Text = ezs.Key2Status.ToString();
                textBox19.Text = ezs.Key3Status.ToString();
                textBox20.Text = ezs.Key4Status.ToString();
                textBox21.Text = ezs.Key5Status.ToString();
                textBox22.Text = ezs.Key6Status.ToString();
                textBox23.Text = ezs.Key7Status.ToString();

                checkBox1.Checked = ezs.bInitialised;
                checkBox2.Checked = ezs.bTPremoved;
                checkBox3.Checked = ezs.bPersonalised;
                checkBox4.Checked = ezs.bActivated;

                if (ezs.LastKey>0x07)
                    textBox14.Text = "";
                else
                    textBox14.Text = ezs.LastKey.ToString();
                if (ezs.PreviousKey > 0x07)
                    textBox15.Text = "";
                else
                    textBox15.Text = ezs.LastKey.ToString();
                //textBox15.Text = ezs.PreviousKey.ToString();
                this.Text = "电子点火开关 - " + f.FullName;
            }
//             else if (pBufL.Length == 0x400)
//             {                
// //                 int BaseAddress = 0x10A;
// //                 for (int j = 0; j < 9; j++)
// //                 {
// //                     byte tmpSUM;
// //                     tmpSUM = Verify.GetVerify_byteSum(pBufL, BaseAddress, 8);
// //                     if (pBufL[BaseAddress - 0x008] != tmpSUM)
// //                     {
// //                         MessageBox.Show("File check Error!");
// //                         return;
// //                     }
// //                     BaseAddress += 0x12;
// //                 }
// //                 for (int j = 0; j < 8; j++)
// //                 {
// //                     byte tmpSUM;
// //                     tmpSUM = Verify.GetVerify_byteSum(pBufL, BaseAddress, 8);
// //                     if (pBufL[BaseAddress - 0x008] != tmpSUM)
// //                     {
// //                         MessageBox.Show("File check Error!");
// //                         return;
// //                     }
// //                     BaseAddress += 0x12;
// //                 }
//                 ezs = new EZSData(pBufL);
//                 textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, " ");
//                 textBox2.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.PSW, " ");
//                 textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key0, " ");
//                 textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key1, " ");
//                 textBox5.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key2, " ");
//                 textBox6.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key3, " ");
//                 textBox7.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key4, " ");
//                 textBox8.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key5, " ");
//                 textBox9.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key6, " ");
//                 textBox10.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key7, " ");
//                 textBox11.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SerialNum, " ");
//                 textBox12.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Dealer, " ");
//                 textBox13.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Transport, " ");
// 
//                 textBox16.Text = ezs.Key0Status.ToString();
//                 textBox17.Text = ezs.Key1Status.ToString();
//                 textBox18.Text = ezs.Key2Status.ToString();
//                 textBox19.Text = ezs.Key3Status.ToString();
//                 textBox20.Text = ezs.Key4Status.ToString();
//                 textBox21.Text = ezs.Key5Status.ToString();
//                 textBox22.Text = ezs.Key6Status.ToString();
//                 textBox23.Text = ezs.Key7Status.ToString();
// 
//                 checkBox1.Checked = ezs.bInitialised;
//                 checkBox2.Checked = ezs.bTPremoved;
//                 checkBox3.Checked = ezs.bPersonalised;
//                 checkBox4.Checked = ezs.bActivated;
// 
//                 if (ezs.LastKey > 0x07)
//                     textBox14.Text = "";
//                 else
//                     textBox14.Text = ezs.LastKey.ToString();
//                 if (ezs.PreviousKey > 0x07)
//                     textBox15.Text = "";
//                 else
//                     textBox15.Text = ezs.LastKey.ToString();
//                 //textBox15.Text = ezs.PreviousKey.ToString();
//                 this.Text = "电子点火开关 - " + f.FullName;
//             }
            else
            {
                MessageBox.Show("文件错误!!");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (ezs == null)
            {
                MessageBox.Show("没有数据用于保存!!","错误");
                return;
            }
            saveFileDialog1.FileName=WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, 0,4,"")+"_SKC";
            if (textBox2.Text != "")
            {                
                saveFileDialog1.Filter = "BIN Files(*.BIN)|*.BIN|EZS Files(*.EZS)|*.EZS|All Files(*.*)|*.*";
                saveFileDialog1.DefaultExt = "BIN";
            }
            else
            {
                if (MessageBox.Show("没有密码数据，是否继续保存？", "注意", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.No)
                    return;
                saveFileDialog1.Filter = "EZS Files(*.EZS)|*.EZS|BIN Files(*.BIN)|*.BIN|All Files(*.*)|*.*";
                saveFileDialog1.DefaultExt = "EZS";
            }
            FileInfo f;
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(saveFileDialog1.FileName);
            Stream stream = File.OpenWrite(f.FullName);    
            byte[] w=ezs.SaveEZS();
            stream.Write(w, 0, (int)w.Length);
            stream.Flush();
            stream.Close();
            if(ezs.AllFile!=null)
            {
                string sf = f.DirectoryName;
                sf = sf+"\\"+WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.AllFile, 0,4,"") + ".EZS";
                f = new FileInfo(sf);
                stream = File.OpenWrite(f.FullName);
                stream.Write(ezs.AllFile, 0, 0x400);
                stream.Flush();
                stream.Close();
            }
            toolStripStatusLabel2.Text = "保存成功!!";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
        }

        
        HIDResult ReadDataStep1(EZSMode mode)
        {
            toolStripStatusLabel2.Text = "Please insert IR Key in EZS within 20 s ";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.OrangeRed;
            HIDDevice.HIDProcCallback = new HIDDevice.HIDProcCallbackDelegate(HIDProcCallback);
            InteractionData rx;
            HIDResult hr = HIDDevice.TheHIDDevice.EZSReadDataStep(0, mode,out rx);
            HIDDevice.HIDProcCallback = null;
            if (hr == HIDResult.OK)
            {
                for (int i = 0; i < 4; i++)
                    ezs.SSID[i] = rx.buff[0x10 + i];
                if(ezs.AllFile!=null)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        if (mode == EZSMode.NEC)
                        {
                            ezs.AllFile[HIDDevice.DTnec[0] * 16 + i] = rx.buff[0x10 + i];
                        }
                        else if (mode == EZSMode.MOT)
                        {
                            ezs.AllFile[HIDDevice.DTmot[0] * 16 + i] = rx.buff[0x10 + i];
                        }
                    }
                }
                byte KeyStatus1, KeyStatus2;
                KeyStatus1 = rx.buff[0x10 + 4];
                KeyStatus2 = rx.buff[0x10 + 6];
                if (BytesOP.GetBit(KeyStatus1, 7))
                    ezs.Key0Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 7))
                        ezs.Key0Status = KeyStatus.NoUsed;
                    else
                        ezs.Key0Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 6))
                    ezs.Key1Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 6))
                        ezs.Key1Status = KeyStatus.NoUsed;
                    else
                        ezs.Key1Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 5))
                    ezs.Key2Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 5))
                        ezs.Key2Status = KeyStatus.NoUsed;
                    else
                        ezs.Key2Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 4))
                    ezs.Key3Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 4))
                        ezs.Key3Status = KeyStatus.NoUsed;
                    else
                        ezs.Key3Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 3))
                    ezs.Key4Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 3))
                        ezs.Key4Status = KeyStatus.NoUsed;
                    else
                        ezs.Key4Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 2))
                    ezs.Key5Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 2))
                        ezs.Key5Status = KeyStatus.NoUsed;
                    else
                        ezs.Key5Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 1))
                    ezs.Key6Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 1))
                        ezs.Key6Status = KeyStatus.NoUsed;
                    else
                        ezs.Key6Status = KeyStatus.Used;
                }
                if (BytesOP.GetBit(KeyStatus1, 0))
                    ezs.Key7Status = KeyStatus.Lock;
                else
                {
                    if (BytesOP.GetBit(KeyStatus2, 0))
                        ezs.Key7Status = KeyStatus.NoUsed;
                    else
                        ezs.Key7Status = KeyStatus.Used;
                }
                ezs.Status = rx.buff[0x10 + 5];
                if (BytesOP.GetBit(ezs.Status, 0))
                    ezs.bInitialised = true;
                else
                    ezs.bInitialised = false;
                if (BytesOP.GetBit(ezs.Status, 1))
                    ezs.bTPremoved = true;
                else
                    ezs.bTPremoved = false;
                if (BytesOP.GetBit(ezs.Status, 2))
                    ezs.bPersonalised = true;
                else
                    ezs.bPersonalised = false;
                if (BytesOP.GetBit(ezs.Status, 3))
                    ezs.bActivated = true;
                else
                    ezs.bActivated = false;
                ezs.LastKey = BytesOP.GetLowNibble(rx.buff[0x10 + 7]);
                ezs.PreviousKey = BytesOP.GetHighNibble(rx.buff[0x10 + 7]);
                textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, " ");
                textBox16.Text = ezs.Key0Status.ToString();
                textBox17.Text = ezs.Key1Status.ToString();
                textBox18.Text = ezs.Key2Status.ToString();
                textBox19.Text = ezs.Key3Status.ToString();
                textBox20.Text = ezs.Key4Status.ToString();
                textBox21.Text = ezs.Key5Status.ToString();
                textBox22.Text = ezs.Key6Status.ToString();
                textBox23.Text = ezs.Key7Status.ToString();

                checkBox1.Checked = ezs.bInitialised;
                checkBox2.Checked = ezs.bTPremoved;
                checkBox3.Checked = ezs.bPersonalised;
                checkBox4.Checked = ezs.bActivated;

                if (ezs.LastKey > 0x07)
                    textBox14.Text = "";
                else
                    textBox14.Text = ezs.LastKey.ToString();
                if (ezs.PreviousKey > 0x07)
                    textBox15.Text = "";
                else
                    textBox15.Text = ezs.PreviousKey.ToString();
//                 textBox14.Text = ezs.LastKey.ToString();
//                 textBox15.Text = ezs.PreviousKey.ToString();
            }
            return hr;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            ClearTextbox();
            
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
                MessageBox.Show("Authorization information error!!", "Error");
                return;
            }            
            EZSMode mode;
            if(toolStripComboBox1.SelectedIndex==0)
                mode=EZSMode.NEC;
            else if(toolStripComboBox1.SelectedIndex==1)
                mode=EZSMode.MOT;
            else
            {
                toolStripStatusLabel2.Text = "No EZS Mode!!";
                toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                return;
            }
            ezs = new EZSData();
            progressBar1.Visible = true;
            progressBar1.Value = 100;
            toolStripStatusLabel2.Text = "请在 20 秒内将红外钥匙插入点火开关中";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.OrangeRed;
            buttonState(false);
            //this.Enabled = false;
            //WFGlobal.WaitMS(500);
            int readCount;
            if (radioButton1.Checked)
            {
                readCount = 12;
                ezs.AllFile = null;
            }
            else
            {
                readCount = 0x30;
                ezs.AllFile = new byte[0x400];
            }
            HIDResult hr = ReadDataStep1(mode);           
            progressBar1.Value = 2;            
            for (int step = 1; step < readCount; step++)
            {                
                if (hr == HIDResult.OK)
                {
                    WFGlobal.WaitMS(1);
                    toolStripStatusLabel2.Text = "读取数据....";// +(step).ToString();
                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
                    InteractionData rx;
                    progressBar1.Value += 2;
                    int readStep=0;
                    if (radioButton1.Checked)
                    {
                        if (mode == EZSMode.NEC)
                        {
                            readStep = HIDDevice.DTnec[step];
                        }
                        else if (mode == EZSMode.MOT)
                        {
                            readStep = HIDDevice.DTmot[step];
                        }                        
                    }
                    else
                    {
                        readStep = step;
                    }
                    hr = HIDDevice.TheHIDDevice.EZSReadDataStep(readStep, mode, out rx);
                    int ezsStep = 100;
                    if (radioButton1.Checked)
                    {
                        ezsStep = step;
                    }
                    else
                    {
                        for (int i = 0; i < 8; i++)
                        {
                            ezs.AllFile[step * 16 + i] = rx.buff[0x10 + i];
                        }
                        if (mode == EZSMode.NEC)
                        {
                            for (int i = 0; i < 12; i++)
                            {
                                if (step == HIDDevice.DTnec[i])
                                {
                                    ezsStep = i;
                                    break;
                                }
                            }
                        }
                        else if (mode == EZSMode.MOT)
                        {
                            for (int i = 0; i < 12; i++)
                            {
                                if (step == HIDDevice.DTmot[i])
                                {
                                    ezsStep = i;
                                    break;
                                }
                            }
                        }
                    }
                    switch (ezsStep)
                    {                        
                        case 1:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key0[i] = rx.buff[0x10 + i];
                             textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key0, " ");
                             break;
                        case 2:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key1[i] = rx.buff[0x10 + i];
                             textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key1, " ");
                             break;
                        case 3:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key2[i] = rx.buff[0x10 + i];
                             textBox5.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key2, " ");
                             break;
                        case 4:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key3[i] = rx.buff[0x10 + i];
                             textBox6.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key3, " ");
                             break;
                        case 5:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key4[i] = rx.buff[0x10 + i];
                             textBox7.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key4, " ");
                             break;
                        case 6:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key5[i] = rx.buff[0x10 + i];
                             textBox8.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key5, " ");
                             break;
                        case 7:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key6[i] = rx.buff[0x10 + i];
                             textBox9.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key6, " ");
                             break;
                        case 8:
                             for (int i = 0; i < 8; i++)
                                 ezs.Key7[i] = rx.buff[0x10 + i];
                             textBox10.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Key7, " ");
                             break;
                        case 9:
                             for (int i = 0; i < 8; i++)
                                 ezs.SerialNum[i] = rx.buff[0x10 + i];
                             textBox11.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SerialNum, " ");
                             break;
                        case 10:
                             for (int i = 0; i < 8; i++)
                                 ezs.Dealer[i] = rx.buff[0x10 + i];
                             textBox12.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Dealer, " ");
                             break;
                        case 11:
                             for (int i = 0; i < 8; i++)
                                 ezs.Transport[i] = rx.buff[0x10 + i];
                             textBox13.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.Transport, " ");
                             break;
                        default:
                             break;
                    }
                }
                else
                {                   
                    ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.ReadEZSError1)
                    {
                        toolStripStatusLabel2.Text = "Read EZS Error 1 !!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.ReadEZSError2)
                    {
                        toolStripStatusLabel2.Text = "Read EZS Time Out!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.ReadEZSError3)
                    {
                        toolStripStatusLabel2.Text = "Read EZS Error 3 !!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.ReadEZSError4)
                    {
                        toolStripStatusLabel2.Text = "Read EZS Error 4 !!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.ReadEZSError5)
                    {
                        toolStripStatusLabel2.Text = "Read EZS Error 5 !!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    //this.Enabled = true;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            progressBar1.Value = 100;
            //this.Enabled = true;
            buttonState(true);
            toolStripStatusLabel2.Text = "读取数据完成!!";
            MessageBox.Show("读取数据完成!!", "");
            progressBar1.Visible = false;
            progressBar1.Value = 100;
        }
        protected override void WndProc(ref Message m)
        {
            try
            {
                if (m.Msg == 0x0219)
                {
                    switch (m.WParam.ToInt32())
                    {
                        case 0x8004:
                            try
                            {
                                HIDDevice.TheHIDDevice.Dispose();
                                HIDDevice.TheHIDDevice = null;
                            }
                            catch { }
                            this.Close();
                            break;
                        default:
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            base.WndProc(ref m);
        }

        private void Form3_FormClosing(object sender, FormClosingEventArgs e)
        {
            Win32Usb.UnregisterForUsbEvents(m_hUsbEventHandle);
            HIDDevice.HIDProcCallback = null;
        }
        private void button4_Click(object sender, EventArgs e)
        {
            FileInfo f;
            byte[] pBuf;
            if (openFileDialog2.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog2.FileName);
            Stream stream = File.OpenRead(f.FullName);
            pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            if (pBuf.Length != 0xa0)
            {
                MessageBox.Show("File Error!!", "Error");
                return;
            }
            if (ezs == null)
            {
                textBox2.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(pBuf, 1, 8, " ");
            }
            else
            {
                ezs.SetPSW(pBuf);
                textBox2.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.PSW, " ");
            }
            toolStripStatusLabel2.Text = "Load PSW OK!!";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void textBox16_TextChanged(object sender, EventArgs e)
        {
            TextBox tb=(TextBox)sender;
            if (tb.Text == "Used")
            { 
                tb.BackColor = System.Drawing.Color.Gold; 
            }
            else if (tb.Text == "NoUsed")
            { 
                tb.BackColor = System.Drawing.Color.Lime; 
            }
            else if (tb.Text == "Lock")
            { 
                tb.BackColor = System.Drawing.Color.Crimson; 
            }
            else
            {
                tb.BackColor = System.Drawing.SystemColors.ControlLightLight;
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckStateChanged(object sender, EventArgs e)
        {
//             if (ezs == null)
//                 checkBox1.Checked = false;
//             else
//                 checkBox1.Checked = ezs.bInitialised;
        }

        private void checkBox2_CheckStateChanged(object sender, EventArgs e)
        {
//             if (ezs == null)
//                 checkBox2.Checked = false;
//             else
//                 checkBox2.Checked = ezs.bTPremoved;
        }

        private void checkBox3_CheckStateChanged(object sender, EventArgs e)
        {
//             if (ezs == null)
//                 checkBox3.Checked = false;
//             else
//                 checkBox3.Checked = ezs.bPersonalised;
        }

        private void checkBox4_CheckStateChanged(object sender, EventArgs e)
        {
//             if (ezs == null)
//                 checkBox4.Checked = false;
//             else
//                 checkBox4.Checked = ezs.bActivated;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            ClearTextbox();
            byte[] pBuf;
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
                MessageBox.Show("Authorization information error!!", "Error");
                return;
            }
            
            //ezs = new EZSData();
            progressBar1.Visible = true;
            progressBar1.Value = 100;
//             toolStripStatusLabel2.Text = "Please insert IR Key in EZS within 20 s ";
//             toolStripStatusLabel2.ForeColor = System.Drawing.Color.OrangeRed;
            buttonState(false);
            //this.Enabled = false;
            //WFGlobal.WaitMS(500);
            int readCount = 9;
            HIDResult hr;// = ReadDataStep1(mode);
            progressBar1.Value = 0;
            pBuf = new byte[0x40];
            for (int i = 0; i < 0x40;i++ )
            {
                pBuf[i]=0xff;
            }
            int pBufIndex=0;
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);
                toolStripStatusLabel2.Text = "读取信息......";// +(step).ToString();
                toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
                InteractionData rx;
                progressBar1.Value += 11;
                hr = HIDDevice.TheHIDDevice.CANInfoDataStep(step, out rx);
                if (hr == HIDResult.OK)
                {
                    if (HIDDevice.CANInfo_AT[step, 15] != 0)
                    {
                        int ix = 8 * HIDDevice.CANInfo_AT[step, 15];
                        for (int i = 0; i < ix; i++)
                        {
                            pBuf[pBufIndex++] = rx.buff[i + 0x10];
                        }
                    }
                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }                    
                    else
                    {
                        toolStripStatusLabel2.Text = "读取信息错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    //this.Enabled = true;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }

            progressBar1.Value = 100;
            buttonState(true);
            toolStripStatusLabel2.Text = "读取信息完成";
            //MessageBox.Show("CAN Get Info Finish!!", "");
            progressBar1.Visible = false;
            progressBar1.Value = 100;
            byte[] ljh = new byte[10];
            ljh[0] = pBuf[0x0f];
            for (int i = 0; i < 7; i++)
                ljh[1 + i] = pBuf[0x11 + i];
            ljh[8] = pBuf[0x19];
            ljh[9] = pBuf[0x1a];
            //ljh[10] = 0;
            string str1, str2, str3, str4;
            if (BytesOP.GetBit(pBuf[0x2c], 0))
                str1 = "初始化：是";
            else
                str1 = "初始化：否";
            if (BytesOP.GetBit(pBuf[0x2c], 1))
                str2 = "运输模式已解除：是";
            else
                str2 = "运输模式已解除：否";
            if (BytesOP.GetBit(pBuf[0x2c], 2))
                str3 = "个性化设置：是";
            else
                str3 = "个性化设置：否"; 
            if (BytesOP.GetBit(pBuf[0x2c], 3))
                str4 = "激活：是";
            else
                str4 = "激活：否";
            MessageBox.Show("EIS 信息读取完成!!\r\nMB零件号:" + Encoding.ASCII.GetString(ljh) + "\r\n" + str1 + "\r\n" + str2 + "\r\n" + str3 + "\r\n" + str4, "");
//             saveFileDialog1.FileName = "CANInfo.BIN";//WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, 0, 4, "") + "_SKC";
//             saveFileDialog1.Filter = "BIN Files(*.BIN)|*.BIN|EZS Files(*.EZS)|*.EZS|All Files(*.*)|*.*";
//             saveFileDialog1.DefaultExt = "BIN";            
//             FileInfo f;
//             if (saveFileDialog1.ShowDialog() != DialogResult.OK)
//                 return;
//             f = new FileInfo(saveFileDialog1.FileName);
//             Stream stream = File.OpenWrite(f.FullName);            
//             stream.Write(pBuf, 0, 0x40);
//             stream.Flush();
//             stream.Close();
            //this.Enabled = true;
            
        }

        private void button6_Click(object sender, EventArgs e)
        {
            ClearTextbox();
            if (HIDDevice.TheHIDDevice.Check_mima() == false)
            {
                MessageBox.Show("Authorization information error!!", "Error");
                return;
            }
            ezs = new EZSData();
            progressBar1.Visible = true;
            progressBar1.Value = 100;
            //             toolStripStatusLabel2.Text = "Please insert IR Key in EZS within 20 s ";
            //             toolStripStatusLabel2.ForeColor = System.Drawing.Color.OrangeRed;
            buttonState(false);
            //this.Enabled = false;
            //WFGlobal.WaitMS(500);
            int readCount = 2;
            int pros=0;
            HIDResult hr;// = ReadDataStep1(mode);
            progressBar1.Value = 0;
            toolStripStatusLabel2.Text = "读取EEPROM.....";// +(step).ToString();
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
            byte[] pBuf = new byte[0x400];
            for (int i = 0; i < 0x400; i++)
            {
                pBuf[i] = 0xff;
            }
            int pBufIndex = 0;
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);                
                InteractionData rx;                
                hr = HIDDevice.TheHIDDevice.CANEeprom1Step(step, out rx);
                pros++;
                progressBar1.Value = (int)(pros*0.87);
                if (hr == HIDResult.OK)
                {
                    
                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }                    
                    else
                    {
                        toolStripStatusLabel2.Text = "读取EEPROM错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    //this.Enabled = true;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            readCount = 0x24*3;
            //toolStripStatusLabel2.Text = "CAN EEPROM 2";// +(step).ToString();
            //toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);                
                InteractionData rx;
                int rxCount;
                hr = HIDDevice.TheHIDDevice.CANEeprom2Step(step, out rx,out rxCount);
                pros++;
                progressBar1.Value = (int)(pros * 0.87);
                if (hr == HIDResult.OK)
                {
                    if (rxCount != 0)
                    {
                        int ix = 8 * rxCount;
                        for (int i = 0; i < ix; i++)
                        {
                            pBuf[pBufIndex++] = rx.buff[i + 0x10];
                        }
                    }
                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "读取EEPROM错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    //this.Enabled = true;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            readCount = 4;
            //toolStripStatusLabel2.Text = "CAN EEPROM 3";// +(step).ToString();
            //toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);                
                InteractionData rx;
                int rxCount;
                hr = HIDDevice.TheHIDDevice.CANEeprom3Step(step, out rx,out rxCount);
                pros++;
                progressBar1.Value = (int)(pros * 0.87);
                if (hr == HIDResult.OK)
                {
                    if (rxCount != 0)
                    {
                        int ix = 8 * rxCount;
                        for (int i = 0; i < ix; i++)
                        {
                            pBuf[pBufIndex++] = rx.buff[i + 0x10];
                        }
                    }
                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "读取EEPROM错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    //this.Enabled = true;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            byte[] outBuf = new byte[0x400];
            for (int i = 0; i < 0x400; i++)
            {
                outBuf[i] = 0x00;
            }
            for (int i = 0; i < 0x24; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    outBuf[i * 0x10 + j] = pBuf[i * 0x10 + 0x06 + j];
                }
                for (int j = 0; j < 6; j++)
                {
                    outBuf[i * 0x10 + 0x02 + j] = pBuf[i * 0x10 + 0x09 + j];
                }
            }
            //
            for (int j = 0; j < 4; j++)
            {
                outBuf[0x30 * 0x10 + j] = pBuf[0x24 * 0x10 + 0x04 + j];
            }
            for (int j = 0; j < 7; j++)
            {
                outBuf[0x30 * 0x10 + 0x04 + j] = pBuf[0x24 * 0x10 + 0x09 + j];
            }
            for (int j = 0; j < 6; j++)
            {
                outBuf[0x30 * 0x10 + 0x0B + j] = pBuf[0x24 * 0x10 + 0x11 + j];
            }
            //
            for (int i = 0; i < 0x80; i++)
            {
                outBuf[0x27 * 0x10 + i] = outBuf[0x02 * 0x10 + i];
            }
            for (int i = 0; i < 0x10; i++)
            {
                outBuf[0x26 * 0x10 + i] = outBuf[0x23 * 0x10 + i];
            }
            for (int i = 0; i < 0x10; i++)
            {
                outBuf[0x23 * 0x10 + i] = outBuf[0x22 * 0x10 + i];
                outBuf[0x22 * 0x10 + i] = 0x00;
            }
            //ezs = new EZSData(outBuf);
            SetCANShow(outBuf);
            //textBox15.Text = ezs.PreviousKey.ToString();
            //this.Text = "EZS Module";
            progressBar1.Value = 100;
            //this.Enabled = true;
            buttonState(true);
            toolStripStatusLabel2.Text = "读取EEPROM完成";
            MessageBox.Show("读取EEPROM完成!!", "");
            progressBar1.Visible = false;
            progressBar1.Value = 100;
            
            saveFileDialog1.FileName = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0,4,"")+"_EEPROM.BIN";//WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, 0, 4, "") + "_SKC";
            saveFileDialog1.Filter = "BIN Files(*.BIN)|*.BIN|EZS Files(*.EZS)|*.EZS|All Files(*.*)|*.*";
            saveFileDialog1.DefaultExt = "BIN";
            FileInfo f;
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(saveFileDialog1.FileName);
            Stream stream = File.OpenWrite(f.FullName);
            stream.Write(outBuf, 0, 0x400);
            stream.Flush();
            stream.Close();
//             saveFileDialog1.FileName = "wfCANEEPROM.BIN";//WFNetLib.Strings.StringsFunction.byteToHexStr(ezs.SSID, 0, 4, "") + "_SKC";
//             saveFileDialog1.Filter = "BIN Files(*.BIN)|*.BIN|EZS Files(*.EZS)|*.EZS|All Files(*.*)|*.*";
//             saveFileDialog1.DefaultExt = "BIN";
//             if (saveFileDialog1.ShowDialog() != DialogResult.OK)
//                 return;
//             f = new FileInfo(saveFileDialog1.FileName);
//             stream = File.OpenWrite(f.FullName);
//             stream.Write(ezs.SaveEZS(false), 0, 0x400);
//             stream.Flush();
//             stream.Close();
            this.Text = "电子点火开关 - " + f.FullName;
        }
        void ClearTextbox()
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox3.Text = "";
            textBox4.Text = "";
            textBox5.Text = "";
            textBox6.Text = "";
            textBox7.Text = "";
            textBox8.Text = "";
            textBox9.Text = "";
            textBox10.Text = "";
            textBox11.Text = "";
            textBox12.Text = "";
            textBox13.Text = "";
            textBox14.Text = "";
            textBox15.Text = "";
            textBox16.Text = "";
            textBox17.Text = "";
            textBox18.Text = "";
            textBox19.Text = "";
            textBox20.Text = "";
            textBox21.Text = "";
            textBox22.Text = "";
            textBox23.Text = "";

            checkBox1.Checked = false;
            checkBox2.Checked = false;
            checkBox3.Checked = false;
            checkBox4.Checked = false;
        }
        void buttonState(bool bEn)
        {
            button1.Enabled = bEn;
            button2.Enabled = bEn;
            button3.Enabled = bEn;
            button5.Enabled = bEn;
            button6.Enabled = bEn;
            button7.Enabled = bEn;
        }
        void SetCANShow(byte[] outBuf)
        {
            textBox1.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0, 4, " ");//(ezs.SSID, " ");
            //textBox2.Text = "";//WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 20, 8, " ");//(ezs.PSW, " ");
            textBox3.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x20, 8, " ");//(ezs.Key0, " ");
            textBox4.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x30, 8, " ");//(ezs.Key1, " ");
            textBox5.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x40, 8, " ");//(ezs.Key2, " ");
            textBox6.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x50, 8, " ");//(ezs.Key3, " ");
            textBox7.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x60, 8, " ");//(ezs.Key4, " ");
            textBox8.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x70, 8, " ");//(ezs.Key5, " ");
            textBox9.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x80, 8, " ");//(ezs.Key6, " ");
            textBox10.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x90, 8, " ");//(ezs.Key7, " ");
            textBox11.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x230, 8, " ");//(ezs.SerialNum, " ");
            textBox12.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x10, 8, " ");//(ezs.Dealer, " ");
            textBox13.Text = WFNetLib.Strings.StringsFunction.byteToHexStr(outBuf, 0x260, 8, " ");//(ezs.Transport, " ");

            KeyStatus Key0Status;
            KeyStatus Key1Status;
            KeyStatus Key2Status;
            KeyStatus Key3Status;
            KeyStatus Key4Status;
            KeyStatus Key5Status;
            KeyStatus Key6Status;
            KeyStatus Key7Status;
            byte KeyStatus1, KeyStatus2;
            KeyStatus1 = outBuf[4];
            KeyStatus2 = outBuf[6];
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
            textBox16.Text = Key0Status.ToString();
            textBox17.Text = Key1Status.ToString();
            textBox18.Text = Key2Status.ToString();
            textBox19.Text = Key3Status.ToString();
            textBox20.Text = Key4Status.ToString();
            textBox21.Text = Key5Status.ToString();
            textBox22.Text = Key6Status.ToString();
            textBox23.Text = Key7Status.ToString();

            bool bInitialised;
            bool bTPremoved;
            bool bPersonalised;
            bool bActivated;
            byte LastKey;
            byte PreviousKey;
            byte Status = outBuf[5];
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
            LastKey = BytesOP.GetLowNibble(outBuf[7]);
            PreviousKey = BytesOP.GetHighNibble(outBuf[7]);

            checkBox1.Checked = bInitialised;
            checkBox2.Checked = bTPremoved;
            checkBox3.Checked = bPersonalised;
            checkBox4.Checked = bActivated;

            if (LastKey > 0x07)
                textBox14.Text = "";
            else
                textBox14.Text = LastKey.ToString();
            if (PreviousKey > 0x07)
                textBox15.Text = "";
            else
                textBox15.Text = PreviousKey.ToString();
        }
        private void button7_Click(object sender, EventArgs e)
        {
            FileInfo f;
            byte[] psw;
            try
            {
                psw = WFNetLib.Strings.StringsFunction.strToHexByte(textBox2.Text, " ");
                if (psw.Length != 8)
                {
                    MessageBox.Show("密码区内容格式不对");
                    return;
                }
                if (psw[0] == 0 && psw[1] == 0 && psw[2] == 0 && psw[3] == 0 && psw[4] == 0 && psw[5] == 0 && psw[6] == 0 && psw[7] == 0)
                {
                    if (MessageBox.Show("没发现密码，是否继续", "警告", MessageBoxButtons.YesNo) == DialogResult.No)
                    {
                        return;
                    }
                }
            }
            catch
            {
                MessageBox.Show("密码文件内容格式不对");
                return;
            }
            byte[] pBuf;
            openFileDialog1.Filter = "BIN Files(*.BIN)|*.BIN|All Files(*.*)|*.*";
            if (openFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog1.FileName);
            Stream stream = File.OpenRead(f.FullName);
            pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            if (pBuf.Length == 0x400)
            {
                for (int i = 0, j = 0; i < 0x08 * 0x08; i++)
                {
                    HIDDevice.TheHIDDevice.DTwr[i] = pBuf[(0x20 + (i / 0x08) * 0x10) + j];
                    //pBuf2[i]=DTwr[i];
                    j++;
                    j = j % 0x08;
                }
                for (int i = 0; i < 8; i++)
                {
                    HIDDevice.TheHIDDevice.DTwr[0x40 + i] = pBuf[0x10 + i];
                    //	pBuf2[0x40+i]=DTwr[0x40+i];
                }
                for (int i = 0; i < 8; i++)
                {
                    HIDDevice.TheHIDDevice.DTwr[0x50 + i] = (byte)i;
                    //	pBuf2[0x50+i]=DTwr[0x50+i];
                }
                for (int i = 0; i < 4; i++)
                {
                    HIDDevice.TheHIDDevice.DTwr[0x58 + i] = pBuf[0x00 + i];
                    //	pBuf2[0x58+i]=DTwr[0x58+i];
                }
                HIDDevice.TheHIDDevice.DTwr[0x58 + 0x04] = 0x00;
                //	pBuf2[0x58+0x04]=DTwr[0x58+0x04];
                HIDDevice.TheHIDDevice.DTwr[0x58 + 0x05] = 0x00;
                //	pBuf2[0x58+0x05]=DTwr[0x58+0x05];
                HIDDevice.TheHIDDevice.DTwr[0x58 + 0x06] = 0x00;
                //	pBuf2[0x58+0x06]=DTwr[0x58+0x06];
                HIDDevice.TheHIDDevice.DTwr[0x58 + 0x07] = 0x1c;
                //	pBuf2[0x58+0x07]=DTwr[0x58+0x07];                
                //ezs = new EZSData(pBuf);
                
                for (int i = 0; i < 8; i++)
                {
                    HIDDevice.TheHIDDevice.DTwr[0x48 + i] = psw[i];
                    //pBuf2[0x48+i]=DTwr[0x48+i];
                }
                SetCANShow(pBuf);
            }            
            else
            {
                MessageBox.Show("File Error!!");
                return;
            }
            if (MessageBox.Show("是否写入当前数据", "确认", MessageBoxButtons.YesNo) == DialogResult.No)
            {
                return;
            }
            //textBox15.Text = ezs.PreviousKey.ToString();
            this.Text = "电子点火开关 - " + f.FullName;
//             progressBar1.Visible = true;
//             progressBar1.Value = 100;
            //             toolStripStatusLabel2.Text = "Please insert IR Key in EZS within 20 s ";
            //             toolStripStatusLabel2.ForeColor = System.Drawing.Color.OrangeRed;
            buttonState(false);
            int readCount = 6;
            int pros = 0;
            HIDResult hr;// = ReadDataStep1(mode);
//             progressBar1.Value = 0;
//             toolStripStatusLabel2.Text = "CAN Write 1";// +(step).ToString();
//             toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;

            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);
                InteractionData rx;
                hr = HIDDevice.TheHIDDevice.CANWrite1Step(step, out rx);
                pros++;
                progressBar1.Value = (int)(pros * 0.87);
                if (hr == HIDResult.OK)
                {

                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "写入EIS错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            progressBar1.Visible = true;
            progressBar1.Value = 100;
            toolStripStatusLabel2.Text = "请在 20 秒内将红外钥匙插入点火开关中";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.OrangeRed;
            int waitstep = 0;
            bool Shk = false;
            readCount = 0;
            while (true != Shk)
            {
                InteractionData rx;
                hr = HIDDevice.TheHIDDevice.CANWrite2Step(waitstep, out rx);                
                if (hr == HIDResult.OK)
                {

                }
                else if (hr == HIDResult.CANWriteWait && waitstep == 0)
                {

                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "写入EIS错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    //this.Enabled = true;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
                readCount++;
                if (readCount > 200)
                {
                    toolStripStatusLabel2.Text = "Insert IR Key Time out!!";
                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
                else
                {
                    progressBar1.Value = 100 - (int)(readCount * 0.5);
                    if (readCount % 10 == 0)
                        toolStripStatusLabel2.Text = "请在 " + (20 - readCount / 10).ToString() + " 秒内将红外钥匙插入点火开关中";
                }
                for (int i = 0; i < HIDDevice.CANwr_bChk[waitstep,0]; i++)
                {
                    if (HIDDevice.CANwr_bChk[waitstep,i + 0x01] != rx.buff[i + 0x10])
                    {
                        Shk = true;
                    }
                }
                if (Shk == true)
                {
                    for (int i = 0; i < HIDDevice.CANwr_bChk[3,0x00]; i++)
                    {
                        if (HIDDevice.CANwr_bChk[3,i + 0x01] != rx.buff[i + 0x10])
                        {
                            Shk = false;
                            toolStripStatusLabel2.Text = "红外钥匙插入超时!!";
                            toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                            buttonState(true);
                            progressBar1.Visible = false;
                            progressBar1.Value = 100;
                            return;
                        }
                    }
                }
                //
                waitstep++;
                if (waitstep >= 0x03)
                {
                    waitstep = 0;
                }
            }
            readCount = 2;
            pros = 0;
            toolStripStatusLabel2.Text = "写入数据.....";
            toolStripStatusLabel2.ForeColor = System.Drawing.Color.Green;
            InteractionData rx1 = new InteractionData();
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);
                hr = HIDDevice.TheHIDDevice.CANWrite3Step(step, out rx1);
                pros++;
                progressBar1.Value = (int)(pros * 3.33);
                if (hr == HIDResult.OK)
                {
                    
                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "写入EIS错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            for (int i = 0; i < HIDDevice.CANwr_bChk[4, 0x00]; i++)
            {
                if (HIDDevice.CANwr_bChk[4, i + 0x01] != rx1.buff[i + 0x10])
                {
                    toolStripStatusLabel2.Text = "写入EIS错误!!";
                    toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            readCount = 0x18;
            HIDDevice.DTwrIndex = 0;
            //toolStripStatusLabel2.Text = "CAN Write 4";
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);
                InteractionData rx;
                hr = HIDDevice.TheHIDDevice.CANWrite4Step(step, out rx);
                pros++;
                progressBar1.Value = (int)(pros * 3.33);
                if (hr == HIDResult.OK)
                {
                    
                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "写入EIS错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }
            }
            readCount = 0x04;
            HIDDevice.DTwrIndex = 0;
            //toolStripStatusLabel2.Text = "CAN Write 5";
            for (int step = 0; step < readCount; step++)
            {
                WFGlobal.WaitMS(1);
                InteractionData rx;
                hr = HIDDevice.TheHIDDevice.CANWrite5Step(step, out rx);
                pros++;
                progressBar1.Value = (int)(pros * 3.33);
                if (hr == HIDResult.OK)
                {

                }
                else
                {
                    //ezs = null;
                    if (hr == HIDResult.RxError)
                    {
                        toolStripStatusLabel2.Text = "Reciece Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else if (hr == HIDResult.USBError)
                    {
                        toolStripStatusLabel2.Text = "USB Error!!";
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    else
                    {
                        toolStripStatusLabel2.Text = "写入EIS错误";//hr.ToString();
                        toolStripStatusLabel2.ForeColor = System.Drawing.Color.Red;
                    }
                    buttonState(true);
                    progressBar1.Visible = false;
                    progressBar1.Value = 100;
                    return;
                }

            }
            progressBar1.Value = 100;
            //this.Enabled = true;
            buttonState(true);
            toolStripStatusLabel2.Text = "写入EIS完成";
            MessageBox.Show("写入EIS完成!!", "");
            progressBar1.Visible = false;
            progressBar1.Value = 100;
        }
        void CANWriteStep2_ProcCallback(int Proc)
        {
            if (Proc > 0)
            {
                try
                {
                    progressBar1.Value = 100 - (int)(Proc * 2.5);
                    if (Proc % 2 == 0)
                        toolStripStatusLabel2.Text = "Please insert IR Key in EZS within " + (20 - Proc / 2).ToString() + " s";
                }
                catch { }
            }
        }        
    }    
}
