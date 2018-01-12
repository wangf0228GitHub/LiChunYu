using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KeyTools.wf
{
    public class HexMaskedTextBox : MaskedTextBox
    {
        public HexMaskedTextBox()
        {
            this.Enter+=new EventHandler(maskedTextBox_Enter);
            this.Click += new EventHandler(maskedTextBox_Click);
            this.Mask = ">AA AA AA AA AA AA AA AA";
            this.AsciiOnly = true;
            this.PromptChar = ' ';
            this.BeepOnError = true;
        }
        bool bEnter = false;
        private void maskedTextBox_Enter(object sender, EventArgs e)
        {
            //maskedTextBox1.Select(0,0);
            bEnter = true;
            //this.SelectionStart = 0;
        }
        private void maskedTextBox_Click(object sender, EventArgs e)
        {
            if (bEnter)
            {
                bEnter = false;
                string str = this.Text.Replace(" ", "");
                int x;
                if (str.Length % 2 == 0)
                {
                    x = str.Length / 2 * 3;
                    if (x > this.TextLength)
                        return;
                }
                else
                {
                    x = str.Length / 2 * 3 + 1;
                }
                this.SelectionStart = x;
            }
        }
        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            //mList[mIndex++] = m;
            if (m.Msg == 0x0102)
            {
                char c = (char)m.WParam;
                if ((c >= 'f' && c <= 'z') || (c >= 'F' && c <= 'Z'))
                {
                    return;
                }
            }
            base.WndProc(ref m);
        }
    }
}
