using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace 服务器端.webAPI
{
    public class webAPIFunc
    {
        public static string GetRetString(ErrType err)
        {
            return GetRetString(err, "");
        }
        public static string GetRetString(ErrType err, string result)
        {
            return ((int)err).ToString() + ","+result;
        }

        public static string GetRetString(int err, string result)
        {
            return err.ToString() + "," + result;
        }
    }
}