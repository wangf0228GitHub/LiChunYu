using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PCTest
{
    public partial class CANForm : Form
    {
        public CANForm()
        {
            InitializeComponent();
        }       
        void setBR()
        {
            SetCANBaudRateCommand src = new SetCANBaudRateCommand();
            if (BRCombo.SelectedIndex == 0)//83.3K
            {
                src.Prescaler = 63;
                src.BS1 = 6;
                src.BS2 = 1;
                src.SJW = 1;
                src.RNum = 0;
            }
            else if (BRCombo.SelectedIndex == 1)//500K
            {
                src.Prescaler = 6;
                src.BS1 = 11;
                src.BS2 = 2;
                src.SJW = 1;
                src.RNum = 3;
            }
            textBox1.AppendText("设定波特率:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(src.GetTxData(), " ") + "\r\n");
            rxPacket = MainForm.comProc.CommandProc(100,src.GetTxData(),1000);
            if (rxPacket == null)
            {
                textBox1.AppendText("接收错误\r\n");
            }
            else if (rxPacket.Header.bError != 0)
            {
                textBox1.AppendText("通讯指令未能达到预期目的，错误代码为:" + rxPacket.Header.ErrCode.ToString("D02") + "\r\n");
            }
            else
            {
                textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rxPacket.Data, " ") + "\r\n");
            }
        }
        private void CANForm_Load(object sender, EventArgs e)
        {
            BRCombo.SelectedIndex = 1;
            EISCombo.SelectedIndex = 3;
        }
        stm32f4_RxPacket rxPacket;
        private void btReadInfo(object sender, EventArgs e)
        {
            textBox1.Clear();
            setBR();            
        }
        private void btReadData(object sender, EventArgs e)
        {
            textBox1.Clear();
            setBR();
            switch (EISCombo.SelectedIndex)
            {
                case 0://
                    EIS_Mode1_ReadData m1ReadData = new EIS_Mode1_ReadData();
                    for (int i = 0; i < m1ReadData.canCommand.Length; i++)
                    {
                        textBox1.AppendText("指令  TX_ID=" + m1ReadData.canCommand[i].TxStdId.ToString("X04") + ",数据:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(m1ReadData.canCommand[i].TxData, " ") + "\r\n");
                        rxPacket = MainForm.comProc.CommandProc(101, m1ReadData.canCommand[i].GetTxData(), m1ReadData.canCommand[i].TxRetry * m1ReadData.canCommand[i].TxTimeOut + m1ReadData.canCommand[i].RxRetry * m1ReadData.canCommand[i].RxTimeOut);
                        if (rxPacket == null)
                        {
                            textBox1.AppendText("接收错误\r\n");
                        }
                        else if (rxPacket.Header.bError != 0)
                        {
                            textBox1.AppendText("通讯指令未能达到预期目的，错误代码为:" + rxPacket.Header.ErrCode.ToString("D02") + "\r\n");
                        }
                        else
                        {
                            textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rxPacket.Data, " ") + "\r\n");
                        }
                    }
                    break;
                case 1://
                    EIS_Mode2_ReadData m2ReadData = new EIS_Mode2_ReadData();
                    for (int i = 0; i < m2ReadData.canCommand.Length; i++)
                    {
                        textBox1.AppendText("指令  TX_ID=" + m2ReadData.canCommand[i].TxStdId.ToString("X04") + ",数据:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(m2ReadData.canCommand[i].TxData, " ") + "\r\n");
                        rxPacket = MainForm.comProc.CommandProc(101, m2ReadData.canCommand[i].GetTxData(), m2ReadData.canCommand[i].TxRetry * m2ReadData.canCommand[i].TxTimeOut + m2ReadData.canCommand[i].RxRetry * m2ReadData.canCommand[i].RxTimeOut);
                        if (rxPacket == null)
                        {
                            textBox1.AppendText("接收错误\r\n");
                        }
                        else if (rxPacket.Header.bError != 0)
                        {
                            textBox1.AppendText("通讯指令未能达到预期目的，错误代码为:" + rxPacket.Header.ErrCode.ToString("D02") + "\r\n");
                        }
                        else
                        {
                            textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rxPacket.Data, " ") + "\r\n");
                        }
                    }
                    break;
                case 2://
                    EIS_Mode3_ReadData m3ReadData = new EIS_Mode3_ReadData();
                    for (int i = 0; i < m3ReadData.canCommand.Length; i++)
                    {
                        textBox1.AppendText("指令  TX_ID=" + m3ReadData.canCommand[i].TxStdId.ToString("X04") + ",数据:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(m3ReadData.canCommand[i].TxData, " ") + "\r\n");
                        rxPacket = MainForm.comProc.CommandProc(101, m3ReadData.canCommand[i].GetTxData(), m3ReadData.canCommand[i].TxRetry * m3ReadData.canCommand[i].TxTimeOut + m3ReadData.canCommand[i].RxRetry * m3ReadData.canCommand[i].RxTimeOut);
                        if (rxPacket == null)
                        {
                            textBox1.AppendText("接收错误\r\n");
                        }
                        else if (rxPacket.Header.bError != 0)
                        {
                            textBox1.AppendText("通讯指令未能达到预期目的，错误代码为:" + rxPacket.Header.ErrCode.ToString("D02") + "\r\n");
                        }
                        else
                        {
                            textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rxPacket.Data, " ") + "\r\n");
                        }
                    }
                    break;
                case 3://
                    EIS_Mode4_ReadData m4ReadData = new EIS_Mode4_ReadData();
                    for (int i = 0; i < m4ReadData.canCommand.Length; i++)
                    {
                        textBox1.AppendText("指令  TX_ID=" + m4ReadData.canCommand[i].TxStdId.ToString("X04") + ",数据:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(m4ReadData.canCommand[i].TxData, " ") + "\r\n");
                        rxPacket = MainForm.comProc.CommandProc(101, m4ReadData.canCommand[i].GetTxData(),0);
                        if (rxPacket == null)
                        {
                            textBox1.AppendText("接收错误\r\n");
                        }
                        else if (rxPacket.Header.bError != 0)
                        {
                            textBox1.AppendText("通讯指令未能达到预期目的，错误代码为:" + rxPacket.Header.ErrCode.ToString("D02") + "\r\n");
                        }
                        else
                        {
                            textBox1.AppendText("接收:" + WFNetLib.StringFunc.StringsFunction.byteToHexStr(rxPacket.Data, " ") + "\r\n");
                        }
                    }
                    break;
            }
                           
            textBox1.AppendText("\r\n完成于" + DateTime.Now.ToString());
        }
    }
}
