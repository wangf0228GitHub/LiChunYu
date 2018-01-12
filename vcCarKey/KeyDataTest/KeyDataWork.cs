using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Collections;
using System.Threading;
using DAL;
using System.Data;
using WFNetLib;
using System.IO;
using WFNetLib.StringFunc;
namespace 服务器端
{
    public class KeyDataWork
    {
        public static bool bCalc;
        public static AutoResetEvent autoResetEvent = new AutoResetEvent(false);
        public KeyDataWork()
        {            
        }        
        public static void KeyDataWorkThread()
        {
            int retryMax = 10;
            int retry = retryMax;
            iniFileOP.iniFilePath = "c:\\LCY.ini";
            while (true)
            {
                bCalc = false;
                autoResetEvent.WaitOne();
                bCalc = true;
                while (true)
                {
                    bool bOK = false;
                    KeyDataQueueData eq = KeyDataQueueDataDBOption.GetFirst();
                    if (eq == null)
                        break;
                    KeyDataWorkLogData kl = new KeyDataWorkLogData();
                    kl.IP = eq.IP;
                    kl.DeviceID = eq.DeviceID;
                    kl.sTime = eq.sTime;
                    kl.SSID = eq.SSID;
                    
                    try
                    {
                        byte[] data = WFNetLib.StringFunc.StringsFunction.strToHexByte(eq.BIN, "");
                        string folder="e:\\mb2\\" + eq.SSID;
                        if(WFNetLib.FileOP.IsExist(folder,FileOPMethod.Folder))
                        {
                            WFNetLib.FileOP.Delete(folder, FileOPMethod.Folder);
                        }                        
                        GetST12(data);
                        uint keyoption = (uint)eq.KeyOption;
                        for (int i = 0; i < 8; i++)
                        {
                            if (BytesOP.GetBit(keyoption, i))
                                iniFileOP.Write("DuoXuan", i.ToString(), "1");
                            else
                                iniFileOP.Write("DuoXuan", i.ToString(), "0");
                        }
                        if (BytesOP.GetBit(keyoption, 8))
                            iniFileOP.Write("DuoXuan", "all", "1");
                        else
                            iniFileOP.Write("DuoXuan", "all".ToString(), "0");
                        iniFileOP.Write("KaiGuan", "KG", "1");
                        retry = 60;
                        bool bStart = false;
                        while (retry != 0)
                        {
                            Thread.Sleep(1000);
                            retry--;
                            try
                            {
                                int kg = int.Parse(iniFileOP.Read("KaiGuan", "KG"));
                                if (kg == 2)
                                {
                                    bStart = true;
                                    break;
                                }
                            }
                            catch
                            {

                            }
                        }
                        if (bStart)
                        {
                            retry = 5*60;
                            bool bFinish = false;
                            while (retry != 0)
                            {
                                Thread.Sleep(1000);
                                retry--;
                                try
                                {
                                    int kg = int.Parse(iniFileOP.Read("KaiGuan", "KG"));
                                    if (kg == 3)
                                    {
                                        bFinish = true;
                                        break;
                                    }
                                }
                                catch
                                {

                                }
                            }
                            if (bFinish)
                            {
                                if (WFNetLib.FileOP.IsExist(folder, FileOPMethod.Folder))
                                {
                                    byte format = (byte)eq.format;
                                    string filename;
                                    if (BytesOP.GetBit(format, 0))//11格式
                                    {
                                        if (BytesOP.GetBit(keyoption, 0))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_0_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key0_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 1))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_1_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key1_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 2))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_2_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key2_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 3))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_3_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key3_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 4))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_4_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key4_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 5))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_5_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key5_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 6))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_6_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key6_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 7))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_7_" + eq.SSID + "_11.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key7_11 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                    }
                                    if (BytesOP.GetBit(format, 1))//41格式
                                    {
                                        if (BytesOP.GetBit(keyoption, 0))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_0_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key0_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 1))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_1_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key1_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 2))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_2_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key2_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 3))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_3_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key3_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 4))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_4_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key4_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 5))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_5_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key5_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 6))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_6_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key6_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 7))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_7_" + eq.SSID + "_41.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key7_41 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                    }
                                    if (BytesOP.GetBit(format, 2))//51格式
                                    {
                                        if (BytesOP.GetBit(keyoption, 0))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_0_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key0_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 1))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_1_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key1_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 2))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_2_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key2_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 3))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_3_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key3_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 4))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_4_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key4_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 5))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_5_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key5_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 6))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_6_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key6_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                        if (BytesOP.GetBit(keyoption, 7))
                                        {
                                            FileInfo f;
                                            filename = folder + "\\key_7_" + eq.SSID + "_51.bin";
                                            f = new FileInfo(filename);
                                            Stream stream = File.OpenRead(f.FullName);
                                            byte[] bin = new byte[stream.Length];
                                            stream.Read(bin, 0, (int)stream.Length);
                                            stream.Close();
                                            kl.Key7_51 = StringsFunction.byteToHexStr(bin, "");
                                        }
                                    }
                                    kl.bOK = true;
                                    KeyDataWorkLogDataDBOption.Insert(kl);
                                    //WFNetLib.FileOP.Delete(folder, FileOPMethod.Folder);
                                }
                                else
                                {
                                    kl.bOK = false;
                                    KeyDataWorkLogDataDBOption.Insert(kl);
                                }
                            }
                            else
                            {
                                kl.bOK = false;
                                KeyDataWorkLogDataDBOption.Insert(kl);
                            }
                        }
                        else
                        {
                            kl.bOK = false;
                            KeyDataWorkLogDataDBOption.Insert(kl);
                        }
                        KeyDataQueueDataDBOption.delete(eq);
                        
                    }
                    catch// (System.Exception ex)
                    {
                        kl.bOK = false;
                        KeyDataWorkLogDataDBOption.Insert(kl);
                        KeyDataQueueDataDBOption.delete(eq);
                    } 
                }
                               
            }
        }
        static void GetST12(byte[] AllFile)
        {
            FileInfo f;
            f = new FileInfo("e:\\mb2\\st12.bin");
            Stream stream = File.OpenRead(f.FullName);
            byte[] bin = new byte[stream.Length];
            stream.Read(bin, 0, (int)stream.Length);
            stream.Close();
            byte[] pBuf=new byte[0x400];
	        for(int i=0;i<0x400;i++)
		        pBuf[i]=bin[i];//AllFile[i];
	
	        for(int i=0;i<17;i++)
		        pBuf[0x2e+i]=AllFile[0x0300+i];	

	        for(int i=0;i<8;i++)
		        pBuf[0x8e+i]=AllFile[0x3f8+i];

	        for(int i=0;i<8;i++)
		        pBuf[0xa6+i]=AllFile[0x0260+i];
	        for(int i=0;i<8;i++)
		        pBuf[0xb6+i]=AllFile[0x0230+i];
	        for(int i=0;i<4;i++)
		        pBuf[0xbe+i]=AllFile[0x0000+i];
	        pBuf[0x00d8]=(byte)(AllFile[7]&0x0f);
	        pBuf[0x00d8]=(byte)((AllFile[7]&0xf0)>>4);
	        int offset=0;
	        //key0~7
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x0270+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x0270+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x0270,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x0270,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x0280+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x0280+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x0280,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x0280,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x0290+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x0290+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x0290,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x0290,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x02a0+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x02a0+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02a0,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02a0,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x02b0+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x02b0+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02b0,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02b0,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x02c0+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x02c0+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02c0,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02c0,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x02d0+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x02d0+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02d0,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02d0,8);
	        offset=offset+0x12;
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x10a+i+offset]=AllFile[0x02e0+i];
		        pBuf[0x1ac+i+offset]=AllFile[0x02e0+i];
	        }
	        pBuf[0x10a+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02e0,8);
	        pBuf[0x1ac+offset - 0x008] = Verify.GetVerify_byteSum(AllFile,0x02e0,8);
	        //dealer
	        for(int i=0;i<8;i++)
	        {
		        pBuf[0x19a+i]=AllFile[0x10+i];
		        pBuf[0x23c+i]=AllFile[0x10+i];
	        }
	        pBuf[0x19a - 0x008] = Verify.GetVerify_byteSum(AllFile,0x10,8);
            pBuf[0x23c - 0x008] = Verify.GetVerify_byteSum(AllFile, 0x10, 8);

	        byte[] save=new byte[0x800];
	        for (int i = 0; i < 0x800; i++)
		        save[i] = 0xff;
	        for (int i = 0, k = 0; i < 0x400; )
	        {
		        save[k++] = pBuf[i++];
		        save[k++] = pBuf[i++];
		        k += 2;
	        }
            FileInfo f1;
            f1 = new FileInfo("e:\\mb2\\Calc.bin");
            Stream stream1 = File.OpenWrite(f1.FullName);
            stream1.Write(save, 0, (int)save.Length);
            stream1.Flush();
            stream1.Close();
        }
    }
}