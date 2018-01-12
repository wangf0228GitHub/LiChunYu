using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Common
{
    public enum ZhiFang_TiZhi
    {
        偏低 = 1,
        标准,
        偏高,
        高
    }
    public enum ZhiFang_TiXing
    {
        消瘦 = 1,
        标准,
        隐藏性肥胖,
        肌肉性肥胖或健壮,
        肥胖
    }
    public enum XinDianAnalyseResult
    {
        波形未见异常 = 0,
        波形疑似心跳稍快请注意休息, //2
        波形疑似心跳过快请注意休息, //3
        波形疑似阵发性心跳过快请咨询医生, //4
        波形疑似心跳稍缓请注意休息, //5
        波形疑似心跳过缓请注意休息, //6
        波形疑似偶发心跳间期缩短请咨询医生, //7
        波形疑似心跳间期不规则请咨询医生, //8
        波形疑似心跳稍快伴有偶发心跳间期缩短请咨询医生, //9
        波形疑似心跳稍缓伴有偶发心跳间期缩短请咨询医生, //10
        波形疑似心跳稍缓伴有心跳间期不规则请咨询医生, //11
        波形有漂移请重新测量, //12
        波形疑似心跳过快伴有波形漂移请咨询医生, //13
        波形疑似心跳过缓伴有波形漂移请咨询医生, //14
        波形疑似偶发心跳间期缩短伴有波形漂移请咨询医生, //15
        波形疑似心跳间期不规则伴有波形漂移请咨询医生, //16
        信号较差请重新测量 = 0xff //17
    }
    public enum FJQDIndex
    {
        bHaveShenGao = 0,
        bHaveTiWen,
        bHaveXueYa,
        bHaveXinDian,
        bHaveXueYang,
        bHaveXueTang,
        bHaveXueZhi,
        bHaveTimer,
        bHaveZhiFangYi
    }
    public class RegResult
    {
        public string message;
        public string DateTime;
    }
    public class TEPartList
    {
        public string workerID;
        public string message;
        public List<TEInfoPart> list;
    }
    public class TEInfoPart
    {
        public string TEID;
        public string zhuCeID;
        public string shiyongdanwei;
        public string bufangweizhi;
        public string LastPulse;
    }
    public class MedicalResultTitle
    {
        public string teid;
        public string time;
    }
    public class MedicalResultList
    {
        public string message;
        public string id;
        public int TemporaryUserID;
        public List<MedicalResultTitle> list;
    }
    public class NurseList
    {
        public string message;
        public string teid;
        public List<NurseData> list;
    }
}
