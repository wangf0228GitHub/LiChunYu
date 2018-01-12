using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using WFNetLib;
using System.Net;
using WFNetLib.StringFunc;
using DAL;
using System.Configuration;
using WFNetLib.Log;

namespace 服务器端.webAPI
{
    public partial class HPassStatus : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string FileName;
            string ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
            string LinuxServer = ConfigurationManager.AppSettings["LinuxServer"];
            try
            {
                FileName = Request.QueryString["FileName"];
                //信息|用户id|实验id，实验名，实验室;
                if (string.IsNullOrEmpty(FileName))//没有这两个变量
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissParam);
                    Response.Write(ret);
                    return;
                }
                HPassWorkLogData hl = HPassWorkLogDataDBOption.GetByFileName(FileName);
                if (hl == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.ErrParam);
                    Response.Write(ret);
                    return;
                }
                WFHttpWebResponse web = new WFHttpWebResponse();
                HttpWebResponse webRet = web.CreateGetHttpResponse(LinuxServer);
                if (webRet == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissServer);
                    Response.Write(ret);
                    return;
                }
                webRet = web.CreatePostHttpResponse(LinuxServer+@"/login", "username=root&password=root");
                if (webRet == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissServer);
                    Response.Write(ret);
                    return;
                }
                webRet = web.CreateGetHttpResponse(LinuxServer+@"/m1_user_search_get_status");
                if (webRet == null)
                {
                    ret = webAPIFunc.GetRetString(ErrType.MissServer); 
                    Response.Write(ret);
                    return;
                }
                m1_Status[] results = WFJSON.JsonDeserializeBySingleData<m1_Status[]>(web.Content);
                FileName=FileName+"_0";
                foreach (m1_Status m1 in results)
                {
                    if (m1.search_id == FileName)
                    {
                        if (m1.status == "inprogress")
                        {
                            if (m1.progress == 100)
                            {
                                string strResult="";
                                if (m1.results != null)
                                {                                     
                                    foreach(m1_result result in m1.results)
                                    {
                                        byte[] q = StringsFunction.strToHexByte(result.query, " ");
                                        byte[] a = StringsFunction.strToHexByte(result.answer, " ");
                                        string sq = StringsFunction.byteToHexStr(q, "");
                                        string sa = StringsFunction.byteToHexStr(a, "");
                                        strResult += sq + "-" + sa + "|";
                                    }
                                }
                                hl.eTime = DateTime.Now;
                                hl.Ret = strResult;
                                HPassWorkLogDataDBOption.Update(hl);
                                ret = webAPIFunc.GetRetString(ErrType.retOK,strResult);
                                //ret = "1,100,"+strResult;
                                Response.Write(ret);
                                return;
                            }
                            else
                            {
                                ret = "2," + m1.progress.ToString();
                                Response.Write(ret);
                                return;   
                            }
                        }
                        else if (m1.status == "pendding")
                        {
                            ret = "2,-1";
                            Response.Write(ret);
                            return;  
                        }
                        else
                        {
                            ret = webAPIFunc.GetRetString(ErrType.ErrServerRet);
                            Response.Write(ret);
                            return; 
                        }
                    }
                }
                ret = "3,";
                Response.Write(ret);
                return; 
            }
            catch (System.Exception ex)
            {
                ret = webAPIFunc.GetRetString(ErrType.UnkownErr);
                TextLog.AddTextLog("Satus_Unkown:" + ex.Message, Global.txtLogFolder + "HPass.txt", true);
            }
            Response.Write(ret);
        }
    }
    public class m1_result
    {
        public string id;
        public string query;
        public string answer;
        public string createdAt;
        public string updatedAt;
        public string request_id;
    }
    public class m1_user_request
    {
        public bool show;
        public string createdAt;
        public string updatedAt;
        public string userID;
        public string requestID;
    }
    public class m1_Status
    {
        public string id;
        public string status;
        public string search_id;
        public int progress;
        public string type;
        public byte[] data;
        public string source_ip;
        public string createdAt;
        public string updatedAt;
        public string dvice_id;
        public m1_user_request user_request;
        public m1_result[] results;
    }
}