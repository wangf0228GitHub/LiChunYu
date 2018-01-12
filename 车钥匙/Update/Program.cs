using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Update
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            if (args.Length > 0)
                Form1.args = args[0];
            else
                Form1.args = "";
            Application.Run(new Form1());
        }
    }
}
