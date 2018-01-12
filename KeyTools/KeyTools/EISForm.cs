using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using WFNetLib.StringFunc;
using System.Threading;

namespace KeyTools
{
    public partial class EISForm : Form
    {
        MainForm mainForm;
        EISData eisData;
        public EISForm(MainForm _mainForm)
        {
            InitializeComponent();
            mainForm = _mainForm;
        }
        private void EISForm_Load(object sender, EventArgs e)
        {
            cbModelType.SelectedIndex = 0;
            eisData = new EISData();
        }        
        int ModelType;
        private void cbModelType_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cbModelType.SelectedIndex)
            {
                case 0://W164-NEC-old
                    ModelType = 0;
                    break;
                case 1://W164-NEC-new
                    ModelType = 4;//05款==164new                    
                    break;
                case 2://W166-NEC
                    ModelType = 7;//08款==166 09款==246 166和246这两个是一样的
                    break;
                case 3://W204-NEC
                    ModelType = 1;//02款==204
                    break;
                case 4://W207-NEC
                    ModelType = 1;//02款==204
                    break;
                case 5://W212-NEC-old
                    ModelType = 2;//03款==212old
                    break;
                case 6://W212-NEC-new
                    ModelType = 5;//06款==212new
                    break;
                case 7://W221-NEC-old
                    ModelType = 3;//04款==221old
                    break;
                case 8://W221-NEC-new
                    ModelType = 6;//07款==221new
                    break;
                case 9://W246-NEC
                    ModelType = 8;//09款==246
                    break;
            }
            ModelType++;
        }
        private void btReadEIS_Click(object sender, EventArgs e)
        {
            clearData();
            mainForm.SetWorkStatus("开始读取EIS");
            if (ModelType == 1 || ModelType == 4)
                DLLWork.SetCANBaudRate(0);
            else
                DLLWork.SetCANBaudRate(1);
            if (!DLLWork.CANReadEEPROMProc(ModelType))
            {
                int errCode = DLLWork.GetErrorCode();
                mainForm.SetWorkStatus(ErrorCodeTranslate.GetLastError(1, errCode));
                WinAPI.MessageBeep(WinAPI.MB_ICONHAND);
                return;
            }
            byte[] rx = new byte[1024 * 10];
            int len = 0;
            DLLWork.GetEEPROMData(rx, ref len);

            eisData.SetAllData(rx, len);

            string fileName = System.Windows.Forms.Application.StartupPath + "\\" + ModelType.ToString("X2") + ".bin";
            FileInfo f = new FileInfo(fileName);
            Stream stream = File.OpenWrite(f.FullName);
            stream.Write(rx, 0, len);
            stream.Flush();
            stream.Close();
            ShowData();
            WinAPI.MessageBeep(WinAPI.MB_OK);
            mainForm.SetWorkStatus("读取点火开关完成");
        }
        void clearData()
        {
            tbSSID.Text = "";

            tbKey0.Text = "";
            tbKey1.Text = "";
            tbKey2.Text = "";
            tbKey3.Text = "";
            tbKey4.Text = "";
            tbKey5.Text = "";
            tbKey6.Text = "";
            tbKey7.Text = "";

            tbDealer.Text = "";

            cbInitialised.Set(false);
            cbTPremoved.Set(false);
            cbPersonalised.Set(false);
            cbActivated.Set(false);


            cbKey0Used.Set(false);
            cbKey0Locked.Set(false);

            cbKey1Used.Set(false);
            cbKey1Locked.Set(false);

            cbKey2Used.Set(false);
            cbKey2Locked.Set(false);

            cbKey3Used.Set(false);
            cbKey3Locked.Set(false);

            cbKey4Used.Set(false);
            cbKey4Locked.Set(false);

            cbKey5Used.Set(false);
            cbKey5Locked.Set(false);

            cbKey6Used.Set(false);
            cbKey6Locked.Set(false);

            cbKey7Used.Set(false);
            cbKey7Locked.Set(false);

            tbLastKey.Text = " ";
            
            tbPreviousKey.Text = " ";            
        }
        void ShowData()
        {
            tbSSID.Text = StringsFunction.byteToHexStr(eisData.SSID, " ");

            tbKey0.Text = StringsFunction.byteToHexStr(eisData.Key0, " ");
            tbKey1.Text = StringsFunction.byteToHexStr(eisData.Key1, " ");
            tbKey2.Text = StringsFunction.byteToHexStr(eisData.Key2, " ");
            tbKey3.Text = StringsFunction.byteToHexStr(eisData.Key3, " ");
            tbKey4.Text = StringsFunction.byteToHexStr(eisData.Key4, " ");
            tbKey5.Text = StringsFunction.byteToHexStr(eisData.Key5, " ");
            tbKey6.Text = StringsFunction.byteToHexStr(eisData.Key6, " ");
            tbKey7.Text = StringsFunction.byteToHexStr(eisData.Key7, " ");

            tbDealer.Text = StringsFunction.byteToHexStr(eisData.Dealer, " ");
            
            cbInitialised.Set(eisData.bInitialised);
            cbTPremoved.Set(eisData.bTPremoved);
            cbPersonalised.Set(eisData.bPersonalised);
            cbActivated.Set(eisData.bActivated);


            cbKey0Used.Set(eisData.Key0Status == KeyStatus.Used);
            cbKey0Locked.Set(eisData.Key0Status == KeyStatus.Lock);

            cbKey1Used.Set(eisData.Key1Status == KeyStatus.Used);
            cbKey1Locked.Set(eisData.Key1Status == KeyStatus.Lock);

            cbKey2Used.Set(eisData.Key2Status == KeyStatus.Used);
            cbKey2Locked.Set(eisData.Key2Status == KeyStatus.Lock);

            cbKey3Used.Set(eisData.Key3Status == KeyStatus.Used);
            cbKey3Locked.Set(eisData.Key3Status == KeyStatus.Lock);

            cbKey4Used.Set(eisData.Key4Status == KeyStatus.Used);
            cbKey4Locked.Set(eisData.Key4Status == KeyStatus.Lock);

            cbKey5Used.Set(eisData.Key5Status == KeyStatus.Used);
            cbKey5Locked.Set(eisData.Key5Status == KeyStatus.Lock);

            cbKey6Used.Set(eisData.Key6Status == KeyStatus.Used);
            cbKey6Locked.Set(eisData.Key6Status == KeyStatus.Lock);

            cbKey7Used.Set(eisData.Key7Status == KeyStatus.Used);
            cbKey7Locked.Set(eisData.Key7Status == KeyStatus.Lock);

            if (eisData.LastKey > 7)
                tbLastKey.Text = " ";
            else
                tbLastKey.Text = eisData.LastKey.ToString();

            if (eisData.PreviousKey > 7)
                tbPreviousKey.Text = " ";
            else
                tbPreviousKey.Text = eisData.PreviousKey.ToString();
        }

        private void btSaveData_Click(object sender, EventArgs e)
        {
            DLLWork.IREn(1);
            Thread.Sleep(200);
            DLLWork.IRWork(1);
            DLLWork.IREn(0);
        }

        private void btLoadData_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            FileInfo f;
            byte[] pBuf, pBufL;
            openFileDialog.Filter = "BIN Files(*.BIN)|*.BIN|All Files(*.*)|*.*";
            if (openFileDialog.ShowDialog() != DialogResult.OK)
                return;
            f = new FileInfo(openFileDialog.FileName);
            Stream stream = File.OpenRead(f.FullName);
            pBufL = new byte[stream.Length];
            stream.Read(pBufL, 0, (int)stream.Length);
            stream.Close();
            eisData.SetAllData(pBufL, pBufL.Length);
            ShowData();
        }
    }
}
