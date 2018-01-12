using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace KeyTools
{
    public class WinAPI
    {
        [DllImport("Kernel32.dll")]
        public static extern bool Beep(int frequency, int duration);

        
        public const int MB_ICONASTERISK=0x40;//   SystemAsterisk

        public const int MB_ICONEXCLAMATION=0x30;//   SystemExclamation

        public const int MB_ICONHAND=0x10;//   SystemHand 警告音

        public const int MB_ICONQUESTION=0x20;//   SystemQuestion

        public const int MB_OK=0;

        [DllImport("user32.dll", EntryPoint = "MessageBeep")]
        public static extern bool MessageBeep(uint uType);
    }
}
