using System;
using System.Windows;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace WFNetLib
{
    public partial class WaitingForm : Form
    {
//         public WaitingForm()
//         {
//             InitializeComponent();
//         }
        public bool bCancelled;
        public ManualResetEvent ReadyEvent;
        public WaitingType type;
        public object LockWatingThread;
        bool CloseSelf = false;
        /************************************************************************/
        /* 外部设定鼠标样式                                                     */
        /************************************************************************/
        private delegate void SetCursorStyleDelegate(Cursor c);
        private void SetCursorStyleDelegateProc(Cursor c)
        {
            this.Cursor = c;
        }
        public void ExternSetCursorStyle(Cursor c)
        {
            while (!this.IsHandleCreated) ;
            this.Invoke(new SetCursorStyleDelegate(SetCursorStyleDelegateProc), c);
        }
        /************************************************************************/
        /* 外部设定进度条                                                       */
        /************************************************************************/
        private delegate void SetProcessBarDelegate(int i);
        private void SetProcessBarDelegateProc(int i)
        {
            progressBar1.Value = i;
        }
        public void ExternSetProcessBar(int i)
        {
            while (!this.IsHandleCreated) ;
            this.Invoke(new SetProcessBarDelegate(SetProcessBarDelegateProc), i);
        }
        /************************************************************************/
        /* 外部关闭窗体                                                         */
        /************************************************************************/
        private delegate void CloseDelegate();      
        private void CloseDelegateProc()
        {
            CloseSelf = true;
            this.Close();
            
        }
        public void ExternClose()
        {
            while (!this.IsHandleCreated) ;
            this.Invoke(new CloseDelegate(CloseDelegateProc));
        }
        
        public WaitingForm(WaitingType type)
        {
            InitializeComponent();
            LockWatingThread = new object();
            ReadyEvent = new ManualResetEvent(false);
            this.type = type;
            if (type == WaitingType.None)
            {
                button1.Visible = false;
                progressBar1.Width = 530;
            }
            bCancelled = false;
        }
        private string _ConfirmPrompt;

        public string ConfirmPrompt
        {
            get { return _ConfirmPrompt; }
            set { _ConfirmPrompt = value; }
        }

        private void WaitingForm_Shown(object sender, EventArgs e)
        {
            ReadyEvent.Set();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            switch (type)
            {
                case WaitingType.None:
                    break;
                case WaitingType.WithCancel:
                    bCancelled = true;
                    break;

                case WaitingType.With_ConfirmCancel:
                    lock (LockWatingThread)
                    {
                        if(MessageBox.Show(ConfirmPrompt, "waring", MessageBoxButtons.YesNo)==DialogResult.Yes)                       
                        {
                            bCancelled = true;
                        } 
                    }
                    break;
            }
        }

        private void WaitingForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (button1.Visible || CloseSelf)
                return;
            e.Cancel = true;
        }        
    }

}
