using System;
using System.Text;
using System.ComponentModel;
using WFNetLib.Strings;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Threading;

namespace WFNetLib
{    
    public class WFGlobal
    {        
        
        static public void WaitMS(int ms)
        {
            while (ms != 0)
            {
                Application.DoEvents();
                Thread.Sleep(1);
                ms--;
            } 
        }
        
    }
}
