using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class DPassWorkLogData
    {

        public const string DEVICEIDField = @"DeviceID";

        public const string IPField = @"IP";

        public const string PASSField = @"Pass";

        public const string RETField = @"Ret";

        public const string SSIDField = @"SSID";

        public const string STIMEField = @"sTime";

        public const string ETIMEField = @"eTime";

        private String _DeviceID;

        private String _IP;

        private String _Pass;

        private String _Ret;

        private String _SSID;

        private DateTime _sTime;

        private DateTime _eTime;

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
        public DPassWorkLogData()
        {
            _DeviceID = "";
            _IP = "";
            _Pass = "";
            _Ret = "";
            _SSID = "";
            _sTime = DateTime.Now;
            _eTime = DateTime.Now;
        }
    }

    public class DPassWorkLogDataFactory
    {
        public static DPassWorkLogData Construct(IDataReader reader)
        {
            DPassWorkLogData data = new DPassWorkLogData();

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

            return data;
        }
        public static DPassWorkLogData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static DPassWorkLogData Construct(DataTable dt, int rowIndex)
        {
            DPassWorkLogData data = new DPassWorkLogData();

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

            return data;
        }
        public static DPassWorkLogData Construct(DataRow dr)
        {
            DPassWorkLogData data = new DPassWorkLogData();

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

            return data;
        }
    }
    public class DPassWorkLogDataDBOption
    {
        public static int Insert(DPassWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into DPassWorkLog (");
            sql.Append("DeviceID, ");
            sql.Append("IP, ");
            sql.Append("Pass, ");
            sql.Append("Ret, ");
            sql.Append("SSID, ");
            sql.Append("sTime, ");
            sql.Append("eTime");
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
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassWorkLog ");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect_addrownum()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select ROW_NUMBER() over(order by eTime desc) as DeviceIndex,* from DPassWorkLog order by eTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect_byID(string id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select ROW_NUMBER() over(order by eTime desc) as DeviceIndex,* from DPassWorkLog where DeviceID like '%" + id + "%' order by eTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static int GetUseTimes(string DeviceID)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select count(*) from DPassWorkLog where DeviceID='" + DeviceID + "' and convert(varchar(10),sTime,120) = '" + DateTime.Now.ToString("yyyy-MM-dd") + "'"); ;
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
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassWorkLog ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(DPassWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassWorkLog ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
    }
}