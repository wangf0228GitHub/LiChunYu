using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WFNetLib.Strings.CryptoService;
using System.IO;
using WFNetLib;

namespace 车钥匙
{
    public class RunInfo
    {
        
    }
    public class SoftVerifyData
    {
//        public DateTime FirstTime;
//        public DateTime CurTime;
//         public int UsedDays;
//         public int UsedTimes;
        public float SoftVersion;
        public DateTime SoftUpdateTime;
        public DateTime LicUpdateTime;
        public DateTime BINUpdateTime;
        const string sKey = "wfsoftvf";
        const string sIV_BINUpdateTime = "verifybu";
        const string sIV_SoftUpdateTime = "verifysu";
        const string sIV_LicUpdateTime = "verifylu";
        const string sIV_SoftVerion = "verifysv";
        const string sIV_Len = "verifyle";
        const int lenlen = 12;
        public void SetVerifyData_SoftVersion()
        {
            DESCrypto des = new DESCrypto();
            string strDes = "", strLen;
            FileStream fs;
            fs = new FileStream("Runinfo.mkt", FileMode.Open);
            SoftVersion = HIDDevice.SoftVersion;
            strDes = des.EncryptString(HIDDevice.SoftVersion.ToString("f2"), sKey, sIV_SoftVerion);
            BinaryWriter sw = new BinaryWriter(fs);
            //开始写入            
            fs.Seek(0, SeekOrigin.Begin);
            strLen = des.EncryptString(strDes.Length.ToString("d4"), sKey, sIV_Len);
            strLen += GenerateCheckCode(100 - lenlen);
            strDes += GenerateCheckCode(100 - strDes.Length);
            sw.Write(Encoding.UTF8.GetBytes(strLen + strDes));
            sw.Flush();
            //关闭流
            sw.Close();
            fs.Close();/**/
        }
        public void SetVerifyData_SoftUpdateTime(DateTime value)
        {
            SoftUpdateTime = value;
            DESCrypto des = new DESCrypto();
            string strDes = "", strLen;
            FileStream fs;
            fs = new FileStream("Runinfo.mkt", FileMode.Open);
            strDes = des.EncryptString(SoftUpdateTime.ToString("yyyy-MM-dd HH:mm:ss"), sKey, sIV_SoftUpdateTime);
            BinaryWriter sw = new BinaryWriter(fs);
            //开始写入            
            fs.Seek(200, SeekOrigin.Begin);
            strLen = des.EncryptString(strDes.Length.ToString("d4"), sKey, sIV_Len);
            strLen += GenerateCheckCode(100 - lenlen);
            strDes += GenerateCheckCode(100 - strDes.Length);
            sw.Write(Encoding.UTF8.GetBytes(strLen + strDes));
            sw.Flush();
            //关闭流
            sw.Close();
            fs.Close();/**/            
        }
        public void SetVerifyData_LicUpdateTime(DateTime value)
        {
            LicUpdateTime = value;
            DESCrypto des = new DESCrypto();
            string strDes = "", strLen;
            FileStream fs;
            fs = new FileStream("Runinfo.mkt", FileMode.Open);
            strDes = des.EncryptString(LicUpdateTime.ToString("yyyy-MM-dd HH:mm:ss"), sKey, sIV_LicUpdateTime);
            BinaryWriter sw = new BinaryWriter(fs);
            //开始写入            
            fs.Seek(400, SeekOrigin.Begin);
            strLen = des.EncryptString(strDes.Length.ToString("d4"), sKey, sIV_Len);
            strLen += GenerateCheckCode(100 - lenlen);
            strDes += GenerateCheckCode(100 - strDes.Length);
            sw.Write(Encoding.UTF8.GetBytes(strLen + strDes));
            sw.Flush();
            //关闭流
            sw.Close();
            fs.Close();/**/      
        }
        public void SetVerifyData_BINUpdateTime(DateTime value)
        {
            BINUpdateTime = value;
            DESCrypto des = new DESCrypto();
            string strDes = "", strLen;
            FileStream fs;
            fs = new FileStream("Runinfo.mkt", FileMode.Open);
            strDes = des.EncryptString(BINUpdateTime.ToString("yyyy-MM-dd HH:mm:ss"), sKey, sIV_BINUpdateTime);
            BinaryWriter sw = new BinaryWriter(fs);
            //开始写入            
            fs.Seek(600, SeekOrigin.Begin);
            strLen = des.EncryptString(strDes.Length.ToString("d4"), sKey, sIV_Len);
            strLen += GenerateCheckCode(100 - lenlen);
            strDes += GenerateCheckCode(100 - strDes.Length);
            sw.Write(Encoding.UTF8.GetBytes(strLen + strDes));
            sw.Flush();
            //关闭流
            sw.Close();
            fs.Close();/**/      
        }
        public void GetVerifyData()
        {
            DESCrypto des = new DESCrypto();
            try
            {
                //SetVerifyData_SoftVersion();
                FileStream fs = new FileStream("Runinfo.mkt", FileMode.Open);                
                BinaryReader sw = new BinaryReader(fs);
                //开始写入            
                fs.Seek(0, SeekOrigin.Begin);
                byte[] rd = sw.ReadBytes(800);                
                sw.Close();
                fs.Close();
                File.SetAttributes("Runinfo.mkt", FileAttributes.System | FileAttributes.Hidden);
                string strLen = Encoding.UTF8.GetString(rd, 0, lenlen);
                strLen = des.DecryptString(strLen, sKey, sIV_Len);
                int len = int.Parse(strLen);
                string strDes = Encoding.UTF8.GetString(rd, 100, len);
                strDes = des.DecryptString(strDes, sKey, sIV_SoftVerion);
                SoftVersion = float.Parse(strDes);

                strLen = Encoding.UTF8.GetString(rd, 200, lenlen);
                strLen = des.DecryptString(strLen, sKey, sIV_Len);
                len = int.Parse(strLen);
                strDes = Encoding.UTF8.GetString(rd, 300, len);
                strDes = des.DecryptString(strDes, sKey, sIV_SoftUpdateTime);
                SoftUpdateTime = DateTime.Parse(strDes);

                strLen = Encoding.UTF8.GetString(rd, 400, lenlen);
                strLen = des.DecryptString(strLen, sKey, sIV_Len);
                len = int.Parse(strLen);
                strDes = Encoding.UTF8.GetString(rd, 500, len);
                strDes = des.DecryptString(strDes, sKey, sIV_LicUpdateTime);
                LicUpdateTime = DateTime.Parse(strDes);

                strLen = Encoding.UTF8.GetString(rd, 600, lenlen);
                strLen = des.DecryptString(strLen, sKey, sIV_Len);
                len = int.Parse(strLen);
                strDes = Encoding.UTF8.GetString(rd, 700, len);
                strDes = des.DecryptString(strDes, sKey, sIV_BINUpdateTime);
                BINUpdateTime = DateTime.Parse(strDes);
            }
            catch (FileNotFoundException)
            {
//                 FileStream fs = new FileStream("Runinfo.mkt", FileMode.Create);                
//                 fs.Close();
//                 File.SetAttributes("Runinfo.mkt", FileAttributes.System | FileAttributes.Hidden);
//                 BINUpdateTime = DateTime.Now.AddDays(-32);
//                 SoftUpdateTime = DateTime.Now.AddDays(-32);
//                 LicUpdateTime = DateTime.Now.AddDays(-32);
//                 //SoftVersion = 车钥匙.HIDDevice.SoftVersion;
//                 SetVerifyData_SoftVersion();
//                 SetVerifyData_BINUpdateTime(DateTime.Now.AddDays(-32));
//                 SetVerifyData_LicUpdateTime(DateTime.Now.AddDays(-32));
//                 SetVerifyData_SoftUpdateTime(DateTime.Now.AddDays(-32));
                Make_mkt();
            }
            catch
            {
                throw;
            }
        }
        public void Make_mkt()
        {
            try
            {
                FileStream fs = new FileStream("Runinfo.mkt", FileMode.OpenOrCreate);
                fs.Close();
            }
            catch (FileNotFoundException)
            {
                FileStream fs = new FileStream("Runinfo.mkt", FileMode.Create);
                fs.Close();
            }
            BINUpdateTime = DateTime.Now.AddDays(-32);
            SoftUpdateTime = DateTime.Now.AddDays(-32);
            LicUpdateTime = DateTime.Now.AddDays(-32);
            //SoftVersion = 车钥匙.HIDDevice.SoftVersion;
            SetVerifyData_SoftVersion();
            SetVerifyData_BINUpdateTime(DateTime.Now.AddDays(-32));
            SetVerifyData_LicUpdateTime(DateTime.Now.AddDays(-32));
            SetVerifyData_SoftUpdateTime(DateTime.Now.AddDays(-32));
//             if (ProcessHelper.IsRunningProcess("Update MKT"))
//             {
//                 //                     if (MessageBox.Show("是否更新到最新版本" + calldata.to_version, "是否更新", MessageBoxButtons.YesNo, MessageBoxIcon.Information) == DialogResult.Yes)
//                 //                     {
//                 //                         
//                 //                     }
//                 ProcessHelper.CloseProcess("Update MKT");
//             }
//             byte[] update = Properties.Resources.Update_MKT;
//             FileStream fs1 = new FileStream("Update MKT.exe", FileMode.Create);
//             fs1.Seek(0, SeekOrigin.Begin);
//             BinaryWriter sw = new BinaryWriter(fs1);
//             sw.Write(update);
//             sw.Flush();
//             //关闭流
//             sw.Close();
//             fs1.Close();/**/
        }
        private int rep = 0;
        //方法一：随机生成不重复数字字符串
        /// 
        /// 生成随机数字字符串
        /// 
        /// 待生成的位数
        /// 生成的数字字符串
        private string GenerateCheckCodeNum(int codeCount)
        {
            string str = string.Empty;
            long num2 = DateTime.Now.Ticks + rep;
            rep++;
            Random random = new Random(((int)(((ulong)num2) & 0xffffffffL)) | ((int)(num2 >> rep)));
            for (int i = 0; i < codeCount; i++)
            {
                int num = random.Next();
                str = str + ((char)(0x30 + ((ushort)(num % 10)))).ToString();
            }
            return str;
        }
        //方法二：随机生成字符串（数字和字母混和） 
        /// 
        /// 生成随机字母字符串(数字字母混和)
        /// 
        /// 待生成的位数
        /// 生成的字母字符串
        private string GenerateCheckCode(int codeCount)
        {
            string str = string.Empty;
            long num2 = DateTime.Now.Ticks + rep;
            rep++;
            Random random = new Random(((int)(((ulong)num2) & 0xffffffffL)) | ((int)(num2 >> rep)));
            for (int i = 0; i < codeCount; i++)
            {
                char ch;
                int num = random.Next();
                if ((num % 2) == 0)
                {
                    ch = (char)(0x30 + ((ushort)(num % 10)));
                }
                else
                {
                    ch = (char)(0x41 + ((ushort)(num % 0x1a)));
                }
                str = str + ch.ToString();
            }
            return str;
        }
    }
}
