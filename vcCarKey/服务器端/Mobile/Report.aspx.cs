using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using DAL;
using System.Text;
using WFNetLib;

namespace 服务器端.Mobile
{
    public partial class Report : System.Web.UI.Page
    {        
        protected void Page_Load(object sender, EventArgs e)
        {
            string time;
            time = Request.QueryString["time"];
            //信息|用户id|实验id，实验名，实验室;
            if (string.IsNullOrEmpty(time))//没有这两个变量
            {
                Response.Redirect("~/Mobile/ReportList.aspx");
                return;
            }
            if (!IsPostBack)
            {
                if (!HttpContext.Current.User.Identity.IsAuthenticated)
                {
                    Response.Redirect("~/Mobile/Login.aspx");
                    return;
                }
                if (!HttpContext.Current.User.IsInRole("会员"))
                {
                    Response.Redirect("~/Mobile/Login.aspx");
                    return;
                }
//                 UserInfoData userInfo = UserInfoDataDBOption.Get(HttpContext.Current.User.Identity.Name);
//                 lName.Text = userInfo.Name;
//                 lSex.Text = userInfo.Sex;
//                 lNation.Text = userInfo.Nation;
//                 lBirthday.Text = userInfo.Born;
//                 lTime.Text = time;
//                 StringBuilder sb = new StringBuilder();
//                 MedicalResultData MedicalResult = MedicalResultDataDBOption.Get("0",HttpContext.Current.User.Identity.Name, time);
//                 string str;
//                 if (MedicalResult != null)
//                 {
//                     ushort fjqd = (ushort)MedicalResult.fujianqingdan;
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveShenGao))
//                     {
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("身高\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         str = getstr(MedicalResult.ShenGao.ToString("F1") + " cm\r\n");
//                         sb.Append(str);
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("体重\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.TiZhong.ToString("F1") + " kg\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                     }
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveTiWen))
//                     {
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("体温\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.ErWen.ToString("F1") + " ℃\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n"); 
//                     }
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveXueYa))
//                     {
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("高压\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         str = getstr(MedicalResult.GaoYa.ToString() + " mmHg");
//                         sb.Append(str+"\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("低压\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.DiYa.ToString() + " mmHg" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("心率\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.XinLv.ToString() + " 次/分" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
// 
//                     }
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveXinDian))
//                     {
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("心电\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");                        
//                         sb.Append(getstr(((Common.XinDianAnalyseResult)MedicalResult.XinDianResult).ToString()) + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                     }
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveXueYang))
//                     {
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("血氧饱和度\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.SpO2.ToString() + " %" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("90-100\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("脉率\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.PR.ToString() + " bpm" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("50-120\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("血流灌注指数\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append((((float)MedicalResult.PI)/10).ToString("F1") + " %" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                     }
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveXueTang))
//                     {
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("餐前血糖\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.XueTang1.ToString("F1") + " umol/L" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("餐后血糖\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.XueTang2.ToString("F1") + " umol/L" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("尿酸\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.NiaoSuan.ToString("F1") + " umol/L" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("胆固醇\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.DanGuChun.ToString("F1") + " mmol/L" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                     }
//                     if (BytesOP.GetBit(fjqd, (int)Common.FJQDIndex.bHaveZhiFangYi))
//                     {
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("脂肪\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.ZhiFang.ToString("F1") + " %" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("BMI\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.BMI.ToString("F1") + "" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("新陈代谢\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.XinChenDaiXie.ToString() + " Kcal" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("腰围\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.YaoWei.ToString("F1") + " cm\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("臀围\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(MedicalResult.TunWei.ToString("F1") + " cm\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("体质\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(((Common.ZhiFang_TiZhi)MedicalResult.TiZhi).ToString() + "" + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                         //-----------------------------------------------------------
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("体形\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append(((Common.ZhiFang_TiXing)MedicalResult.TiXing).ToString() + "\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("<td align=\"center\">\r\n");
//                         sb.Append("&nbsp;\r\n");
//                         sb.Append("</td>\r\n");
// 
//                         sb.Append("</tr>\r\n");
// 
//                         sb.Append("<tr>\r\n");
//                         sb.Append("<td colspan=\"4\" style=\"border-style: solid  none none none; line-height: 1px; border-top-width: 1px; border-top-color: #f0f0f0;\">\r\n");
//                         sb.Append("</td>\r\n");
//                         sb.Append("</tr>\r\n");
//                         //-----------------------------------------------------------
//                     }                    
//                 }
//                 listText.Text = sb.ToString();
            }
        }
        public string getStr2(string s, int l, string endStr, ref int index)
        {
            string temp = s.Substring(0, (s.Length < l + 1) ? s.Length : l + 1);
            byte[] encodedBytes = System.Text.ASCIIEncoding.ASCII.GetBytes(temp);

            string outputStr = "";
            int count = 0;

            for (int i = 0; i < temp.Length; i++)
            {
                if ((int)encodedBytes[i] == 63)
                    count += 2;
                else
                    count += 1;

                if (count <= l)
                    outputStr += temp.Substring(i, 1);
                else if (count > l)
                {
                    index = i;
                    break;
                }
                index = -1;
            }

            if (count <= l)
            {
                outputStr = temp;
                endStr = "";
            }

            outputStr += endStr;

            return outputStr;
        }
        string getstr(string str)
        {
            string ret = "";
            int yy = 0;
            while (true)
            {
                ret += getStr2(str, 8, "<br/>", ref yy);
                if (yy == -1)
                    break;
                str = str.Substring(yy);
                yy = 0;
            }
            return ret;
        }
    }
}