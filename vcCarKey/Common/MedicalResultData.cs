using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Common
{
    [Serializable]
    public class MedicalResultData
    {

        public const string IDField = @"ID";

        public const string TEIDField = @"TEID";

        public const string TEMPORARYUSERIDField = @"TemporaryUserID";

        public const string FUJIANQINGDANField = @"fujianqingdan";

        public const string TIMEField = @"Time";

        public const string SHENGAOField = @"ShenGao";

        public const string TIZHONGField = @"TiZhong";

        public const string ERWENField = @"ErWen";

        public const string GAOYAField = @"GaoYa";

        public const string DIYAField = @"DiYa";

        public const string XINLVField = @"XinLv";

        public const string XINDIANDATASField = @"XindianDatas";

        public const string XINDIANRESULTField = @"XinDianResult";

        public const string XINDIANXINLVField = @"XinDianXinLv";

        public const string SPO2Field = @"SpO2";

        public const string PRField = @"PR";

        public const string PIField = @"PI";

        public const string XUETANG1Field = @"XueTang1";

        public const string XUETANG2Field = @"XueTang2";

        public const string NIAOSUANField = @"NiaoSuan";

        public const string DANGUCHUNField = @"DanGuChun";

        public const string ZHIFANGField = @"ZhiFang";

        public const string BMIField = @"BMI";

        public const string XINCHENDAIXIEField = @"XinChenDaiXie";

        public const string TIZHIField = @"TiZhi";

        public const string TIXINGField = @"TiXing";

        public const string YAOWEIField = @"YaoWei";

        public const string TUNWEIField = @"TunWei";

        public const string BAK20Field = @"Bak20";

        public const string BAK19Field = @"Bak19";

        public const string BAK18Field = @"Bak18";

        public const string BAK17Field = @"Bak17";

        public const string BAK16Field = @"Bak16";

        public const string BAK15Field = @"Bak15";

        public const string BAK14Field = @"Bak14";

        public const string BAK13Field = @"Bak13";

        public const string BAK12Field = @"Bak12";

        public const string BAK11Field = @"Bak11";

        public const string BAK10Field = @"Bak10";

        public const string BAK9Field = @"Bak9";

        public const string BAK8Field = @"Bak8";

        public const string BAK7Field = @"Bak7";

        public const string BAK6Field = @"Bak6";

        public const string BAK5Field = @"Bak5";

        public const string BAK4Field = @"Bak4";

        public const string BAK3Field = @"Bak3";

        public const string BAK2Field = @"Bak2";

        public const string BAK1Field = @"Bak1";

        public const string BAK29Field = @"Bak29";

        public const string BAK28Field = @"Bak28";

        public const string BAK27Field = @"Bak27";

        public const string BAK26Field = @"Bak26";

        public const string BAK25Field = @"Bak25";

        public const string BAK24Field = @"Bak24";

        public const string BAK23Field = @"Bak23";

        public const string BAK22Field = @"Bak22";

        public const string BAK21Field = @"Bak21";

        public const string BAK30Field = @"Bak30";

        private String _ID;

        private String _TEID;

        private String _TemporaryUserID;

        private Int32 _fujianqingdan;

        private DateTime _Time;

        private Double _ShenGao;

        private Double _TiZhong;

        private Double _ErWen;

        private Int32 _GaoYa;

        private Int32 _DiYa;

        private Int32 _XinLv;

        private String _XindianDatas;

        private Int32 _XinDianResult;

        private Int32 _XinDianXinLv;

        private Int32 _SpO2;

        private Int32 _PR;

        private Int32 _PI;

        private Double _XueTang1;

        private Double _XueTang2;

        private Double _NiaoSuan;

        private Double _DanGuChun;

        private Double _ZhiFang;

        private Double _BMI;

        private Int32 _XinChenDaiXie;

        private Int32 _TiZhi;

        private Int32 _TiXing;

        private Double _YaoWei;

        private Double _TunWei;

        private String _Bak20;

        private String _Bak19;

        private String _Bak18;

        private String _Bak17;

        private String _Bak16;

        private String _Bak15;

        private String _Bak14;

        private String _Bak13;

        private String _Bak12;

        private String _Bak11;

        private String _Bak10;

        private String _Bak9;

        private String _Bak8;

        private String _Bak7;

        private String _Bak6;

        private String _Bak5;

        private String _Bak4;

        private String _Bak3;

        private String _Bak2;

        private String _Bak1;

        private String _Bak29;

        private String _Bak28;

        private String _Bak27;

        private String _Bak26;

        private String _Bak25;

        private String _Bak24;

        private String _Bak23;

        private String _Bak22;

        private String _Bak21;

        private String _Bak30;

        public String ID
        {
            get { return _ID; }
            set { _ID = value; }
        }
        public String TEID
        {
            get { return _TEID; }
            set { _TEID = value; }
        }
        public String TemporaryUserID
        {
            get { return _TemporaryUserID; }
            set { _TemporaryUserID = value; }
        }
        public Int32 fujianqingdan
        {
            get { return _fujianqingdan; }
            set { _fujianqingdan = value; }
        }
        public DateTime Time
        {
            get { return _Time; }
            set { _Time = value; }
        }
        public Double ShenGao
        {
            get { return _ShenGao; }
            set { _ShenGao = value; }
        }
        public Double TiZhong
        {
            get { return _TiZhong; }
            set { _TiZhong = value; }
        }
        public Double ErWen
        {
            get { return _ErWen; }
            set { _ErWen = value; }
        }
        public Int32 GaoYa
        {
            get { return _GaoYa; }
            set { _GaoYa = value; }
        }
        public Int32 DiYa
        {
            get { return _DiYa; }
            set { _DiYa = value; }
        }
        public Int32 XinLv
        {
            get { return _XinLv; }
            set { _XinLv = value; }
        }
        public String XindianDatas
        {
            get { return _XindianDatas; }
            set { _XindianDatas = value; }
        }
        public Int32 XinDianResult
        {
            get { return _XinDianResult; }
            set { _XinDianResult = value; }
        }
        public Int32 XinDianXinLv
        {
            get { return _XinDianXinLv; }
            set { _XinDianXinLv = value; }
        }
        public Int32 SpO2
        {
            get { return _SpO2; }
            set { _SpO2 = value; }
        }
        public Int32 PR
        {
            get { return _PR; }
            set { _PR = value; }
        }
        public Int32 PI
        {
            get { return _PI; }
            set { _PI = value; }
        }
        public Double XueTang1
        {
            get { return _XueTang1; }
            set { _XueTang1 = value; }
        }
        public Double XueTang2
        {
            get { return _XueTang2; }
            set { _XueTang2 = value; }
        }
        public Double NiaoSuan
        {
            get { return _NiaoSuan; }
            set { _NiaoSuan = value; }
        }
        public Double DanGuChun
        {
            get { return _DanGuChun; }
            set { _DanGuChun = value; }
        }
        public Double ZhiFang
        {
            get { return _ZhiFang; }
            set { _ZhiFang = value; }
        }
        public Double BMI
        {
            get { return _BMI; }
            set { _BMI = value; }
        }
        public Int32 XinChenDaiXie
        {
            get { return _XinChenDaiXie; }
            set { _XinChenDaiXie = value; }
        }
        public Int32 TiZhi
        {
            get { return _TiZhi; }
            set { _TiZhi = value; }
        }
        public Int32 TiXing
        {
            get { return _TiXing; }
            set { _TiXing = value; }
        }
        public Double YaoWei
        {
            get { return _YaoWei; }
            set { _YaoWei = value; }
        }
        public Double TunWei
        {
            get { return _TunWei; }
            set { _TunWei = value; }
        }
        public String Bak20
        {
            get { return _Bak20; }
            set { _Bak20 = value; }
        }
        public String Bak19
        {
            get { return _Bak19; }
            set { _Bak19 = value; }
        }
        public String Bak18
        {
            get { return _Bak18; }
            set { _Bak18 = value; }
        }
        public String Bak17
        {
            get { return _Bak17; }
            set { _Bak17 = value; }
        }
        public String Bak16
        {
            get { return _Bak16; }
            set { _Bak16 = value; }
        }
        public String Bak15
        {
            get { return _Bak15; }
            set { _Bak15 = value; }
        }
        public String Bak14
        {
            get { return _Bak14; }
            set { _Bak14 = value; }
        }
        public String Bak13
        {
            get { return _Bak13; }
            set { _Bak13 = value; }
        }
        public String Bak12
        {
            get { return _Bak12; }
            set { _Bak12 = value; }
        }
        public String Bak11
        {
            get { return _Bak11; }
            set { _Bak11 = value; }
        }
        public String Bak10
        {
            get { return _Bak10; }
            set { _Bak10 = value; }
        }
        public String Bak9
        {
            get { return _Bak9; }
            set { _Bak9 = value; }
        }
        public String Bak8
        {
            get { return _Bak8; }
            set { _Bak8 = value; }
        }
        public String Bak7
        {
            get { return _Bak7; }
            set { _Bak7 = value; }
        }
        public String Bak6
        {
            get { return _Bak6; }
            set { _Bak6 = value; }
        }
        public String Bak5
        {
            get { return _Bak5; }
            set { _Bak5 = value; }
        }
        public String Bak4
        {
            get { return _Bak4; }
            set { _Bak4 = value; }
        }
        public String Bak3
        {
            get { return _Bak3; }
            set { _Bak3 = value; }
        }
        public String Bak2
        {
            get { return _Bak2; }
            set { _Bak2 = value; }
        }
        public String Bak1
        {
            get { return _Bak1; }
            set { _Bak1 = value; }
        }
        public String Bak29
        {
            get { return _Bak29; }
            set { _Bak29 = value; }
        }
        public String Bak28
        {
            get { return _Bak28; }
            set { _Bak28 = value; }
        }
        public String Bak27
        {
            get { return _Bak27; }
            set { _Bak27 = value; }
        }
        public String Bak26
        {
            get { return _Bak26; }
            set { _Bak26 = value; }
        }
        public String Bak25
        {
            get { return _Bak25; }
            set { _Bak25 = value; }
        }
        public String Bak24
        {
            get { return _Bak24; }
            set { _Bak24 = value; }
        }
        public String Bak23
        {
            get { return _Bak23; }
            set { _Bak23 = value; }
        }
        public String Bak22
        {
            get { return _Bak22; }
            set { _Bak22 = value; }
        }
        public String Bak21
        {
            get { return _Bak21; }
            set { _Bak21 = value; }
        }
        public String Bak30
        {
            get { return _Bak30; }
            set { _Bak30 = value; }
        }
        public MedicalResultData()
        {
            _ID = "";
            _TEID = "";
            _TemporaryUserID = "";
            _fujianqingdan = 0;
            _Time = DateTime.Now;
            _ShenGao = 0;
            _TiZhong = 0;
            _ErWen = 0;
            _GaoYa = 0;
            _DiYa = 0;
            _XinLv = 0;
            _XindianDatas = "";
            _XinDianResult = 0;
            _XinDianXinLv = 0;
            _SpO2 = 0;
            _PR = 0;
            _PI = 0;
            _XueTang1 = 0;
            _XueTang2 = 0;
            _NiaoSuan = 0;
            _DanGuChun = 0;
            _ZhiFang = 0;
            _BMI = 0;
            _XinChenDaiXie = 0;
            _TiZhi = 0;
            _TiXing = 0;
            _YaoWei = 0;
            _TunWei = 0;
            _Bak20 = "";
            _Bak19 = "";
            _Bak18 = "";
            _Bak17 = "";
            _Bak16 = "";
            _Bak15 = "";
            _Bak14 = "";
            _Bak13 = "";
            _Bak12 = "";
            _Bak11 = "";
            _Bak10 = "";
            _Bak9 = "";
            _Bak8 = "";
            _Bak7 = "";
            _Bak6 = "";
            _Bak5 = "";
            _Bak4 = "";
            _Bak3 = "";
            _Bak2 = "";
            _Bak1 = "";
            _Bak29 = "";
            _Bak28 = "";
            _Bak27 = "";
            _Bak26 = "";
            _Bak25 = "";
            _Bak24 = "";
            _Bak23 = "";
            _Bak22 = "";
            _Bak21 = "";
            _Bak30 = "";
        }
    }
}
