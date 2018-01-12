using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class HPassWorkLogData
    {

        public const string DEVICEIDField = @"DeviceID";

        public const string IPField = @"IP";

        public const string PASSField = @"Pass";

        public const string RETField = @"Ret";

        public const string SSIDField = @"SSID";

        public const string STIMEField = @"sTime";

        public const string ETIMEField = @"eTime";

        public const string FILENAMEField = @"FileName";

        private String _DeviceID;

        private String _IP;

        private String _Pass;

        private String _Ret;

        private String _SSID;

        private DateTime _sTime;

        private DateTime _eTime;

        private String _FileName;

        public String DeviceID
        {
            get { return _DeviceID; }
            set { _DeviceID = value; }
        }
        public String IP
        {
            get { return _IP; }
            set { _IP = value; }
        }
        public String Pass
        {
            get { return _Pass; }
            set { _Pass = value; }
        }
        public String Ret
        {
            get { return _Ret; }
            set { _Ret = value; }
        }
        public String SSID
        {
            get { return _SSID; }
            set { _SSID = value; }
        }
        public DateTime sTime
        {
            get { return _sTime; }
            set { _sTime = value; }
        }
        public DateTime eTime
        {
            get { return _eTime; }
            set { _eTime = value; }
        }
        public String FileName
        {
            get { return _FileName; }
            set { _FileName = value; }
        }
        public HPassWorkLogData()
        {
            _DeviceID = "";
            _IP = "";
            _Pass = "";
            _Ret = "";
            _SSID = "";
            _sTime = DateTime.Now;
            _eTime = DateTime.Now;
            _FileName = "";
        }
    }

    public class HPassWorkLogDataFactory
    {
        public static HPassWorkLogData Construct(IDataReader reader)
        {
            HPassWorkLogData data = new HPassWorkLogData();

            try { data.DeviceID = Convert.ToString(reader["DeviceID"]); }
            catch { }

            try { data.IP = Convert.ToString(reader["IP"]); }
            catch { }

            try { data.Pass = Convert.ToString(reader["Pass"]); }
            catch { }

            try { data.Ret = Convert.ToString(reader["Ret"]); }
            catch { }

            try { data.SSID = Convert.ToString(reader["SSID"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(reader["sTime"]); }
            catch { }

            try { data.eTime = Convert.ToDateTime(reader["eTime"]); }
            catch { }

            try { data.FileName = Convert.ToString(reader["FileName"]); }
            catch { }

            return data;
        }
        public static HPassWorkLogData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static HPassWorkLogData Construct(DataTable dt, int rowIndex)
        {
            HPassWorkLogData data = new HPassWorkLogData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.Pass = Convert.ToString(dr["Pass"]); }
            catch { }

            try { data.Ret = Convert.ToString(dr["Ret"]); }
            catch { }

            try { data.SSID = Convert.ToString(dr["SSID"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.eTime = Convert.ToDateTime(dr["eTime"]); }
            catch { }

            try { data.FileName = Convert.ToString(dr["FileName"]); }
            catch { }

            return data;
        }
        public static HPassWorkLogData Construct(DataRow dr)
        {
            HPassWorkLogData data = new HPassWorkLogData();

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.Pass = Convert.ToString(dr["Pass"]); }
            catch { }

            try { data.Ret = Convert.ToString(dr["Ret"]); }
            catch { }

            try { data.SSID = Convert.ToString(dr["SSID"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.eTime = Convert.ToDateTime(dr["eTime"]); }
            catch { }

            try { data.FileName = Convert.ToString(dr["FileName"]); }
            catch { }

            return data;
        }
    }
    public class HPassWorkLogDataDBOption
    {
        public static int Insert(HPassWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into HPassWorkLog (");
            sql.Append("DeviceID, ");
            sql.Append("IP, ");
            sql.Append("Pass, ");
            sql.Append("Ret, ");
            sql.Append("SSID, ");
            sql.Append("sTime, ");
            sql.Append("eTime, ");
            sql.Append("FileName");
            sql.Append(") values (");
            sql.Append("'");
            sql.Append(d.DeviceID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.IP.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Pass.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Ret.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.SSID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.sTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.eTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.FileName.ToString());
            sql.Append("'");
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int Update(HPassWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("update HPassWorkLog set ");
            sql.Append("eTime=");
            sql.Append("'");
            sql.Append(d.eTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Ret=");
            sql.Append("'");
            sql.Append(d.Ret);
            sql.Append("'");            
            sql.Append(" where FileName=");
            sql.Append("'");
            sql.Append(d.FileName.ToString()); sql.Append("'");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from HPassWorkLog ");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static int GetIndex(string DeviceID,string SSID)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select count(*) from HPassWorkLog where DeviceID='"+DeviceID+"' and SSID='"+SSID+"'");
            DataTable dt= WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
            {
                return 0;
            }
            else
            {
                return int.Parse(dt.Rows[0][0].ToString());
            }
        }
        public static int GetUseTimes(string DeviceID)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select count(*) from HPassWorkLog where DeviceID='" + DeviceID + "' and convert(varchar(10),sTime,120) = '" + DateTime.Now.ToString("yyyy-MM-dd") + "' and [Ret]<>''");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
            {
                return 0;
            }
            else
            {
                return int.Parse(dt.Rows[0][0].ToString());
            }
        }
        public static HPassWorkLogData GetByFileName(string fileName)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from HPassWorkLog where FileName='" + fileName + "'");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
            {
                return null;
            }
            else
            {
                return HPassWorkLogDataFactory.Construct(dt.Rows[0]);
            }
        }
        public static DataTable DataTableSelect_addrownum()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select ROW_NUMBER() over(order by eTime desc) as DeviceIndex,* from HPassWorkLog order by eTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect_byID(string id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select ROW_NUMBER() over(order by eTime desc) as DeviceIndex,* from HPassWorkLog where DeviceID like '%" + id + "%' order by eTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from HPassWorkLog ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(HPassWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from HPassWorkLog ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
    }
}