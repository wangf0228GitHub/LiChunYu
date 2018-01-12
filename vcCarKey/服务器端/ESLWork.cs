using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Collections;
using System.Threading;
using DAL;
using System.Data;
using FTD2XX_NET;
using WFNetLib.Log;
namespace 服务器端
{
    public class ESLWork
    {
        public static bool bCalc;
        public static byte[] PSW;
        public static AutoResetEvent autoResetEvent = new AutoResetEvent(false);
        public ESLWork()
        {
            PSW = new byte[8];
        }
        public static void ESLPass()
        {
            UInt32 numBytesRead = 0;
            int retryMax = 10;
            int retry = retryMax;
            while (Global.bRun)
            {
                bCalc = false;
                autoResetEvent.WaitOne();
                bCalc = true;
                while (true)
                {
                    bool bOK = false;
                    ESLQueueData eq = ESLQueueDataDBOption.GetFirst();
                    if (eq == null)
                        break;
                    ESLWorkLogData elog = new ESLWorkLogData();
                    elog.Pass = eq.Pass;
                    elog.IP = eq.IP;
                    elog.sTime = eq.sTime;
                    elog.DeviceID = eq.DeviceID;
                    try
                    {
                        byte[] bPass = WFNetLib.StringFunc.StringsFunction.strToHexByte(eq.Pass, "");
                        byte[] rxbuf = new byte[100];
                        retry = retryMax;
                        while (retry != 0)
                        {
                            retry--;
                            try
                            {
                                UInt32 ftdiDeviceCount = 0;
                                FTDI.FT_STATUS ftStatus = FTDI.FT_STATUS.FT_OK;
                                FTDI myFtdiDevice = new FTDI();
//                                 ftStatus = myFtdiDevice.GetNumberOfDevices(ref ftdiDeviceCount);
//                                 if (ftStatus != FTDI.FT_STATUS.FT_OK)
//                                 {
//                                     continue;
//                                 }
//                                 if (ftdiDeviceCount == 0)
//                                 {
//                                     continue;
//                                 }
//                                 FTDI.FT_DEVICE_INFO_NODE[] ftdiDeviceList = new FTDI.FT_DEVICE_INFO_NODE[ftdiDeviceCount];
//                                 ftStatus = myFtdiDevice.GetDeviceList(ftdiDeviceList);
//                                 if (ftStatus == FTDI.FT_STATUS.FT_OK)
//                                 {
//                                     bool bFind = false;
//                                     for (int i = 0; i < ftdiDeviceCount;i++ )
//                                     {
//                                         if (ftdiDeviceList[i].SerialNumber.ToString() == "001054")
//                                         {
//                                             ftStatus = myFtdiDevice.OpenBySerialNumber(ftdiDeviceList[i].SerialNumber.ToString());
//                                             if (ftStatus != FTDI.FT_STATUS.FT_OK)
//                                             {
//                                                 //continue;
//                                                 break;
//                                             }
//                                             bFind = true;
//                                             break;
//                                         }
//                                     }
//                                     if(!bFind)
//                                         continue;
//                                 }
//                                 else
//                                 {
//                                     continue;
//                                 }
                                ftStatus = myFtdiDevice.OpenBySerialNumber("DA00UTH1");
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                ftStatus = myFtdiDevice.ResetDevice();
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                ftStatus = myFtdiDevice.SetBaudRate(300);
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                ftStatus = myFtdiDevice.SetTimeouts(30000, 30000);
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                byte[] txbuf = new byte[100];
                                txbuf[0] = 0xf0;
                                ftStatus = myFtdiDevice.Write(txbuf, 1, ref numBytesRead);
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                Thread.Sleep(200);
                                txbuf[0] = 0x30;
                                txbuf[1] = 0x00;
                                for (int i = 0; i < 8; i++)
                                    txbuf[2 + i] = bPass[i];
                                txbuf[10] = WFNetLib.Verify.GetVerify_byteSum(txbuf, 10);
                                ftStatus = myFtdiDevice.Write(txbuf, 11, ref numBytesRead);
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                ftStatus = myFtdiDevice.Read(rxbuf, 11, ref numBytesRead);
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                ftStatus = myFtdiDevice.Close();
                                if (ftStatus != FTDI.FT_STATUS.FT_OK)
                                {
                                    continue;
                                }
                                if (rxbuf[2] == 0x80
                                    && rxbuf[3] == 0x00
                                    && rxbuf[4] == 0x00
                                    && rxbuf[5] == 0x00
                                    && rxbuf[6] == 0x00
                                    && rxbuf[7] == 0x00
                                    && rxbuf[8] == 0x00
                                    && rxbuf[9] == 0x00)
                                {
                                    Thread.Sleep(5000);
                                    continue;
                                }
                                if (rxbuf[2] == 0x00
                                    && rxbuf[3] == 0x00
                                    && rxbuf[4] == 0x00
                                    && rxbuf[5] == 0x00
                                    && rxbuf[6] == 0x00
                                    && rxbuf[7] == 0x00
                                    && rxbuf[8] == 0x00
                                    && rxbuf[9] == 0x00)
                                {
                                    Thread.Sleep(5000);
                                    continue;
                                }
                                bOK = true;
                                break;
                            }
                            catch
                            {

                            }
                        }                        
                        if (bOK)
                        {
                            ESLRecodeData esl = new ESLRecodeData();
                            esl.Pass = eq.Pass;
                            esl.Ret = WFNetLib.StringFunc.StringsFunction.byteToHexStr(rxbuf, 2, 8, "");
                            ESLRecodeDataDBOption.Insert(esl);
                            elog.Ret = esl.Ret;
                        }
                        else
                        {
                            elog.Ret = "err";
                        }
                        ESLWorkLogDataDBOption.Insert(elog);
                        ESLQueueDataDBOption.delete(eq.Pass, eq.IP, eq.DeviceID, eq.sTime.ToString());
                    }
                    catch (System.Exception ex)
                    {
                        elog.Ret = "err";
                        ESLWorkLogDataDBOption.Insert(elog);  
                        ESLQueueDataDBOption.delete(eq.Pass, eq.IP, eq.DeviceID, eq.sTime.ToString());
                        TextLog.AddTextLog("Thread_Unkown:" + ex.Message, Global.txtLogFolder + "ESLPass.txt", true);
                    }
                    
                }              
            }
        }
    }
}