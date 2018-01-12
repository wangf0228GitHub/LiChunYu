using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 车钥匙
{
    public partial class AboutForm : Form
    {
        public AboutForm()
        {
            InitializeComponent();
        }

        private void AboutForm_Load(object sender, EventArgs e)
        {
            this.Icon = Properties.Resources.MI;
            label4.Text = HIDDevice.SoftVersion.ToString("f2");
            if (HIDDevice.TheHIDDevice == null)
                label3.Text = "Unkwon";
            else
            {
                label3.Text = HIDDevice.TheHIDDevice.HexVer[0].ToString("X2") + "." + HIDDevice.TheHIDDevice.HexVer[1].ToString("X2");
            }
        }
    }
}
