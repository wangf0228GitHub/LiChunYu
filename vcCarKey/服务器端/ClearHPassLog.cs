using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Threading;
using WFNetLib;
using 服务器端.webAPI;
using System.Net;
using WFNetLib.Log;

namespace 服务器端
{
    public class ClearHPassLog
    {
        public ClearHPassLog()
        {
        }
        public static void ClearHPassLogThread()
        {
            while (Global.bRun)
            {
                try
                {
                    WFHttpWebResponse web = new WFHttpWebResponse();
                    web.CreateGetHttpResponse(@"http://192.168.0.158/");
                    web.CreatePostHttpResponse(@"http://192.168.0.158/login", "username=root&password=root");
                    HttpWebResponse hr = web.CreateGetHttpResponse(@"http://192.168.0.158/m1_user_search_get_status");
                    m1_Status[] ret = WFJSON.JsonDeserializeBySingleData<m1_Status[]>(web.Content);
                    foreach (m1_Status m1 in ret)
                    {
                        m1.updatedAt = m1.updatedAt.Replace('T', ' ');
                        m1.updatedAt = m1.updatedAt.Replace("Z", "");
                        DateTime dt = Convert.ToDateTime(m1.updatedAt);
                        if (m1.progress==100 && dt.AddMinutes(30) < DateTime.Now)
                            web.CreateGetHttpResponse(@"http://192.168.0.158/m1_search_user_remove?request_id=" + m1.id);
                        //http://192.168.0.158/m1_search_user_remove?request_id=484
                    }
                    Thread.Sleep(30 * 60 * 1000);
                }
                catch (System.Exception ex)
                {
                    TextLog.AddTextLog("Unkown:" + ex.Message, Global.txtLogFolder + "ClearHPassLog.txt", true);
                }                
            }
        }
    }
}