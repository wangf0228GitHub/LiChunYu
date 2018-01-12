using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace 车钥匙
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            bool createNew;
            using (System.Threading.Mutex mutex = new System.Threading.Mutex(true, Application.ProductName, out createNew))
            {
                if (createNew)
                {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);                   
                    Application.Run(new Form2());
                    
                }
                else
                {
                    MessageBox.Show("Application is running...");
                    System.Threading.Thread.Sleep(1000);
                    System.Environment.Exit(1);
                }
            }            
        }
    }
}
