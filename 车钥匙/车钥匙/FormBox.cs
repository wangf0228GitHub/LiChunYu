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
    public partial class FormBox : Form
    {
        public FormBox(string str,string title)
        {
            InitializeComponent();
            label1.Text = str;
            this.Text = title;
        }

        private void FormBox_Load(object sender, EventArgs e)
        {

        }
    }
}
