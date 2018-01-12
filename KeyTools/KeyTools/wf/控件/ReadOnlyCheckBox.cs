using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KeyTools.wf
{
    public class ReadOnlyCheckBox : CheckBox
    {
        public ReadOnlyCheckBox()
        {
            this.CheckedChanged+=new EventHandler(ReadOnlyCheckBox_CheckedChanged);
        }
        private void ReadOnlyCheckBox_CheckedChanged(object sender, EventArgs e)
        {            
            if (this.Tag == null)
            {
                this.Checked = false;
            }
            else if (this.Tag.ToString() == "True")
            {
                this.Checked = true;
            }
            else
            {
                this.Checked = false;
            }
        }
        public void Check()
        {
            this.Tag = true;
            this.Checked = true;
        }
        public void Uncheck()
        {
            this.Tag = false;
            this.Checked = false;
        }
        public void Set(bool bCheck)
        {
            if (bCheck)
                Check();
            else
                Uncheck();
        }
    }
}
