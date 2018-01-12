using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace WFNetLib
{
    public delegate void WaitingProcFunc(object LockWatingThread);
    public enum WaitingType
    {
        None,
        WithCancel,
        With_ConfirmCancel
    }
    public class WaitingProc
    {
        private WaitingProcFunc Func;
        private Thread WaitingThread;
        private WaitingForm form;        
        public WaitingProc()
        {            
            
        }
        public bool Execute(WaitingProcFunc func, string Title, WaitingType type, string ConfirmPrompt)
        {
            Func = func;
            form = new WaitingForm(type);
            form.ConfirmPrompt = ConfirmPrompt;
            form.Text = Title;

            WaitingThread = new Thread(new ThreadStart(Waiting));
            WaitingThread.Name = "等待执行线程";
            WaitingThread.Start();
            form.ShowDialog();
            return !form.bCancelled;
        }
        private void Waiting()
        {
            Func(form.LockWatingThread);
            form.ReadyEvent.WaitOne();
            form.ExternClose();
        }
        public bool HasBeenCancelled()
        {
            return form.bCancelled;
        }
        public void SetProcessBar(int i)
        {
            form.ExternSetProcessBar(i);
        }
        public void SetCursorStyle(Cursor c)
        {
            form.ExternSetCursorStyle(c);
        }
    }
}
