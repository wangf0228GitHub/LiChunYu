using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace PCTest
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Form1 f = new Form1();
            f.ShowDialog();
        }
        public static ComProc comProc;
        private void button2_Click(object sender, EventArgs e)
        {
            ESLForm f = new ESLForm();
            f.ShowDialog();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            comProc = new ComProc("COM5");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            CANForm f = new CANForm();
            f.ShowDialog();
        }
    }
}
