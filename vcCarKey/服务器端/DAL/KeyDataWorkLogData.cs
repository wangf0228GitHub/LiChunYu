using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class KeyDataWorkLogData
    {

        public const string DEVICEIDField = @"DeviceID";

        public const string SSIDField = @"SSID";

        public const string IPField = @"IP";

        public const string QUEUEIDField = @"QueueID";

        public const string PRIMARYKEY_Field = @"QueueID";

        public const string STIMEField = @"sTime";

        public const string ETIMEField = @"eTime";

        public const string BOKField = @"bOK";

        public const string KEY7_51Field = @"Key7_51";

        public const string KEY6_51Field = @"Key6_51";

        public const string KEY5_51Field = @"Key5_51";

        public const string KEY4_51Field = @"Key4_51";

        public const string KEY3_51Field = @"Key3_51";

        public const string KEY2_51Field = @"Key2_51";

        public const string KEY1_51Field = @"Key1_51";

        public const string KEY0_51Field = @"Key0_51";

        public const string KEY7_41Field = @"Key7_41";

        public const string KEY6_41Field = @"Key6_41";

        public const string KEY5_41Field = @"Key5_41";

        public const string KEY4_41Field = @"Key4_41";

        public const string KEY3_41Field = @"Key3_41";

        public const string KEY2_41Field = @"Key2_41";

        public const string KEY1_41Field = @"Key1_41";

        public const string KEY0_41Field = @"Key0_41";

        public const string KEY7_11Field = @"Key7_11";

        public const string KEY6_11Field = @"Key6_11";

        public const string KEY5_11Field = @"Key5_11";

        public const string KEY4_11Field = @"Key4_11";

        public const string KEY3_11Field = @"Key3_11";

        public const string KEY2_11Field = @"Key2_11";

        public const string KEY1_11Field = @"Key1_11";

        public const string KEY0_11Field = @"Key0_11";

        private String _DeviceID;

        private String _SSID;

        private String _IP;

        private Int32 _QueueID;

        private DateTime _sTime;

        private DateTime _eTime;

        private Boolean _bOK;

        private String _Key7_51;

        private String _Key6_51;

        private String _Key5_51;

        private String _Key4_51;

        private String _Key3_51;

        private String _Key2_51;

        private String _Key1_51;

        private String _Key0_51;

        private String _Key7_41;

        private String _Key6_41;

        private String _Key5_41;

        private String _Key4_41;

        private String _Key3_41;

        private String _Key2_41;

        private String _Key1_41;

        private String _Key0_41;

        private String _Key7_11;

        private String _Key6_11;

        private String _Key5_11;

        private String _Key4_11;

        private String _Key3_11;

        private String _Key2_11;

        private String _Key1_11;

        private String _Key0_11;

        public String DeviceID
        {
            get { return _DeviceID; }
            set { _DeviceID = value; }
        }
        public String SSID
        {
            get { return _SSID; }
            set { _SSID = value; }
        }
        public String IP
        {
            get { return _IP; }
            set { _IP = value; }
        }
        public Int32 QueueID
        {
            get { return _QueueID; }
            set { _QueueID = value; }
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
        public Boolean bOK
        {
            get { return _bOK; }
            set { _bOK = value; }
        }
        public String Key7_51
        {
            get { return _Key7_51; }
            set { _Key7_51 = value; }
        }
        public String Key6_51
        {
            get { return _Key6_51; }
            set { _Key6_51 = value; }
        }
        public String Key5_51
        {
            get { return _Key5_51; }
            set { _Key5_51 = value; }
        }
        public String Key4_51
        {
            get { return _Key4_51; }
            set { _Key4_51 = value; }
        }
        public String Key3_51
        {
            get { return _Key3_51; }
            set { _Key3_51 = value; }
        }
        public String Key2_51
        {
            get { return _Key2_51; }
            set { _Key2_51 = value; }
        }
        public String Key1_51
        {
            get { return _Key1_51; }
            set { _Key1_51 = value; }
        }
        public String Key0_51
        {
            get { return _Key0_51; }
            set { _Key0_51 = value; }
        }
        public String Key7_41
        {
            get { return _Key7_41; }
            set { _Key7_41 = value; }
        }
        public String Key6_41
        {
            get { return _Key6_41; }
            set { _Key6_41 = value; }
        }
        public String Key5_41
        {
            get { return _Key5_41; }
            set { _Key5_41 = value; }
        }
        public String Key4_41
        {
            get { return _Key4_41; }
            set { _Key4_41 = value; }
        }
        public String Key3_41
        {
            get { return _Key3_41; }
            set { _Key3_41 = value; }
        }
        public String Key2_41
        {
            get { return _Key2_41; }
            set { _Key2_41 = value; }
        }
        public String Key1_41
        {
            get { return _Key1_41; }
            set { _Key1_41 = value; }
        }
        public String Key0_41
        {
            get { return _Key0_41; }
            set { _Key0_41 = value; }
        }
        public String Key7_11
        {
            get { return _Key7_11; }
            set { _Key7_11 = value; }
        }
        public String Key6_11
        {
            get { return _Key6_11; }
            set { _Key6_11 = value; }
        }
        public String Key5_11
        {
            get { return _Key5_11; }
            set { _Key5_11 = value; }
        }
        public String Key4_11
        {
            get { return _Key4_11; }
            set { _Key4_11 = value; }
        }
        public String Key3_11
        {
            get { return _Key3_11; }
            set { _Key3_11 = value; }
        }
        public String Key2_11
        {
            get { return _Key2_11; }
            set { _Key2_11 = value; }
        }
        public String Key1_11
        {
            get { return _Key1_11; }
            set { _Key1_11 = value; }
        }
        public String Key0_11
        {
            get { return _Key0_11; }
            set { _Key0_11 = value; }
        }
        public KeyDataWorkLogData()
        {
            _DeviceID = "";
            _SSID = "";
            _IP = "";
            _QueueID = 0;
            _sTime = DateTime.Now;
            _eTime = DateTime.Now;
            _bOK = false;
            _Key7_51 = "";
            _Key6_51 = "";
            _Key5_51 = "";
            _Key4_51 = "";
            _Key3_51 = "";
            _Key2_51 = "";
            _Key1_51 = "";
            _Key0_51 = "";
            _Key7_41 = "";
            _Key6_41 = "";
            _Key5_41 = "";
            _Key4_41 = "";
            _Key3_41 = "";
            _Key2_41 = "";
            _Key1_41 = "";
            _Key0_41 = "";
            _Key7_11 = "";
            _Key6_11 = "";
            _Key5_11 = "";
            _Key4_11 = "";
            _Key3_11 = "";
            _Key2_11 = "";
            _Key1_11 = "";
            _Key0_11 = "";
        }
    }

    public class KeyDataWorkLogDataFactory
    {
        public static KeyDataWorkLogData Construct(IDataReader reader)
        {
            KeyDataWorkLogData data = new KeyDataWorkLogData();

            try { data.DeviceID = Convert.ToString(reader["DeviceID"]); }
            catch { }

            try { data.SSID = Convert.ToString(reader["SSID"]); }
            catch { }

            try { data.IP = Convert.ToString(reader["IP"]); }
            catch { }

            try { data.QueueID = Convert.ToInt32(reader["QueueID"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(reader["sTime"]); }
            catch { }

            try { data.eTime = Convert.ToDateTime(reader["eTime"]); }
            catch { }

            try { data.bOK = Convert.ToBoolean(reader["bOK"]); }
            catch { }

            try { data.Key7_51 = Convert.ToString(reader["Key7_51"]); }
            catch { }

            try { data.Key6_51 = Convert.ToString(reader["Key6_51"]); }
            catch { }

            try { data.Key5_51 = Convert.ToString(reader["Key5_51"]); }
            catch { }

            try { data.Key4_51 = Convert.ToString(reader["Key4_51"]); }
            catch { }

            try { data.Key3_51 = Convert.ToString(reader["Key3_51"]); }
            catch { }

            try { data.Key2_51 = Convert.ToString(reader["Key2_51"]); }
            catch { }

            try { data.Key1_51 = Convert.ToString(reader["Key1_51"]); }
            catch { }

            try { data.Key0_51 = Convert.ToString(reader["Key0_51"]); }
            catch { }

            try { data.Key7_41 = Convert.ToString(reader["Key7_41"]); }
            catch { }

            try { data.Key6_41 = Convert.ToString(reader["Key6_41"]); }
            catch { }

            try { data.Key5_41 = Convert.ToString(reader["Key5_41"]); }
            catch { }

            try { data.Key4_41 = Convert.ToString(reader["Key4_41"]); }
            catch { }

            try { data.Key3_41 = Convert.ToString(reader["Key3_41"]); }
            catch { }

            try { data.Key2_41 = Convert.ToString(reader["Key2_41"]); }
            catch { }

            try { data.Key1_41 = Convert.ToString(reader["Key1_41"]); }
            catch { }

            try { data.Key0_41 = Convert.ToString(reader["Key0_41"]); }
            catch { }

            try { data.Key7_11 = Convert.ToString(reader["Key7_11"]); }
            catch { }

            try { data.Key6_11 = Convert.ToString(reader["Key6_11"]); }
            catch { }

            try { data.Key5_11 = Convert.ToString(reader["Key5_11"]); }
            catch { }

            try { data.Key4_11 = Convert.ToString(reader["Key4_11"]); }
            catch { }

            try { data.Key3_11 = Convert.ToString(reader["Key3_11"]); }
            catch { }

            try { data.Key2_11 = Convert.ToString(reader["Key2_11"]); }
            catch { }

            try { data.Key1_11 = Convert.ToString(reader["Key1_11"]); }
            catch { }

            try { data.Key0_11 = Convert.ToString(reader["Key0_11"]); }
            catch { }

            return data;
        }
        public static Int32 GetPrimaryKey(DataTable dt)
        {
            return GetPrimaryKey(dt, 0);
        }
        public static Int32 GetPrimaryKey(DataTable dt, int rowIndex)
        {
            Int32 PrimaryKey = 0;

            if (dt.Rows.Count < rowIndex + 1)

                return 0;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return 0;

            try { PrimaryKey = Convert.ToInt32(dr[KeyDataWorkLogData.PRIMARYKEY_Field]); }
            catch { }

            return PrimaryKey;
        }
        public static KeyDataWorkLogData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static KeyDataWorkLogData Construct(DataTable dt, int rowIndex)
        {
            KeyDataWorkLogData data = new KeyDataWorkLogData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            try { data.SSID = Convert.ToString(dr["SSID"]); }
            catch { }

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.QueueID = Convert.ToInt32(dr["QueueID"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.eTime = Convert.ToDateTime(dr["eTime"]); }
            catch { }

            try { data.bOK = Convert.ToBoolean(dr["bOK"]); }
            catch { }

            try { data.Key7_51 = Convert.ToString(dr["Key7_51"]); }
            catch { }

            try { data.Key6_51 = Convert.ToString(dr["Key6_51"]); }
            catch { }

            try { data.Key5_51 = Convert.ToString(dr["Key5_51"]); }
            catch { }

            try { data.Key4_51 = Convert.ToString(dr["Key4_51"]); }
            catch { }

            try { data.Key3_51 = Convert.ToString(dr["Key3_51"]); }
            catch { }

            try { data.Key2_51 = Convert.ToString(dr["Key2_51"]); }
            catch { }

            try { data.Key1_51 = Convert.ToString(dr["Key1_51"]); }
            catch { }

            try { data.Key0_51 = Convert.ToString(dr["Key0_51"]); }
            catch { }

            try { data.Key7_41 = Convert.ToString(dr["Key7_41"]); }
            catch { }

            try { data.Key6_41 = Convert.ToString(dr["Key6_41"]); }
            catch { }

            try { data.Key5_41 = Convert.ToString(dr["Key5_41"]); }
            catch { }

            try { data.Key4_41 = Convert.ToString(dr["Key4_41"]); }
            catch { }

            try { data.Key3_41 = Convert.ToString(dr["Key3_41"]); }
            catch { }

            try { data.Key2_41 = Convert.ToString(dr["Key2_41"]); }
            catch { }

            try { data.Key1_41 = Convert.ToString(dr["Key1_41"]); }
            catch { }

            try { data.Key0_41 = Convert.ToString(dr["Key0_41"]); }
            catch { }

            try { data.Key7_11 = Convert.ToString(dr["Key7_11"]); }
            catch { }

            try { data.Key6_11 = Convert.ToString(dr["Key6_11"]); }
            catch { }

            try { data.Key5_11 = Convert.ToString(dr["Key5_11"]); }
            catch { }

            try { data.Key4_11 = Convert.ToString(dr["Key4_11"]); }
            catch { }

            try { data.Key3_11 = Convert.ToString(dr["Key3_11"]); }
            catch { }

            try { data.Key2_11 = Convert.ToString(dr["Key2_11"]); }
            catch { }

            try { data.Key1_11 = Convert.ToString(dr["Key1_11"]); }
            catch { }

            try { data.Key0_11 = Convert.ToString(dr["Key0_11"]); }
            catch { }

            return data;
        }
        public static KeyDataWorkLogData Construct(DataRow dr)
        {
            KeyDataWorkLogData data = new KeyDataWorkLogData();

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            try { data.SSID = Convert.ToString(dr["SSID"]); }
            catch { }

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.QueueID = Convert.ToInt32(dr["QueueID"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.eTime = Convert.ToDateTime(dr["eTime"]); }
            catch { }

            try { data.bOK = Convert.ToBoolean(dr["bOK"]); }
            catch { }

            try { data.Key7_51 = Convert.ToString(dr["Key7_51"]); }
            catch { }

            try { data.Key6_51 = Convert.ToString(dr["Key6_51"]); }
            catch { }

            try { data.Key5_51 = Convert.ToString(dr["Key5_51"]); }
            catch { }

            try { data.Key4_51 = Convert.ToString(dr["Key4_51"]); }
            catch { }

            try { data.Key3_51 = Convert.ToString(dr["Key3_51"]); }
            catch { }

            try { data.Key2_51 = Convert.ToString(dr["Key2_51"]); }
            catch { }

            try { data.Key1_51 = Convert.ToString(dr["Key1_51"]); }
            catch { }

            try { data.Key0_51 = Convert.ToString(dr["Key0_51"]); }
            catch { }

            try { data.Key7_41 = Convert.ToString(dr["Key7_41"]); }
            catch { }

            try { data.Key6_41 = Convert.ToString(dr["Key6_41"]); }
            catch { }

            try { data.Key5_41 = Convert.ToString(dr["Key5_41"]); }
            catch { }

            try { data.Key4_41 = Convert.ToString(dr["Key4_41"]); }
            catch { }

            try { data.Key3_41 = Convert.ToString(dr["Key3_41"]); }
            catch { }

            try { data.Key2_41 = Convert.ToString(dr["Key2_41"]); }
            catch { }

            try { data.Key1_41 = Convert.ToString(dr["Key1_41"]); }
            catch { }

            try { data.Key0_41 = Convert.ToString(dr["Key0_41"]); }
            catch { }

            try { data.Key7_11 = Convert.ToString(dr["Key7_11"]); }
            catch { }

            try { data.Key6_11 = Convert.ToString(dr["Key6_11"]); }
            catch { }

            try { data.Key5_11 = Convert.ToString(dr["Key5_11"]); }
            catch { }

            try { data.Key4_11 = Convert.ToString(dr["Key4_11"]); }
            catch { }

            try { data.Key3_11 = Convert.ToString(dr["Key3_11"]); }
            catch { }

            try { data.Key2_11 = Convert.ToString(dr["Key2_11"]); }
            catch { }

            try { data.Key1_11 = Convert.ToString(dr["Key1_11"]); }
            catch { }

            try { data.Key0_11 = Convert.ToString(dr["Key0_11"]); }
            catch { }

            return data;
        }
        public static Int32 GetPrimaryKey(DataRow dr)
        {
            Int32 PrimaryKey = 0;

            try { PrimaryKey = Convert.ToInt32(dr[KeyDataWorkLogData.PRIMARYKEY_Field]); }
            catch { }
            return PrimaryKey;
        }

    }
    public class KeyDataWorkLogDataDBOption
    {
        public static int Insert(KeyDataWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into KeyDataWorkLog (");
            sql.Append("QueueID,DeviceID, ");
            sql.Append("SSID, ");
            sql.Append("IP, ");
            sql.Append("sTime, ");
            sql.Append("eTime, ");
            sql.Append("bOK, ");
            sql.Append("Key7_51, ");
            sql.Append("Key6_51, ");
            sql.Append("Key5_51, ");
            sql.Append("Key4_51, ");
            sql.Append("Key3_51, ");
            sql.Append("Key2_51, ");
            sql.Append("Key1_51, ");
            sql.Append("Key0_51, ");
            sql.Append("Key7_41, ");
            sql.Append("Key6_41, ");
            sql.Append("Key5_41, ");
            sql.Append("Key4_41, ");
            sql.Append("Key3_41, ");
            sql.Append("Key2_41, ");
            sql.Append("Key1_41, ");
            sql.Append("Key0_41, ");
            sql.Append("Key7_11, ");
            sql.Append("Key6_11, ");
            sql.Append("Key5_11, ");
            sql.Append("Key4_11, ");
            sql.Append("Key3_11, ");
            sql.Append("Key2_11, ");
            sql.Append("Key1_11, ");
            sql.Append("Key0_11");
            sql.Append(") values (");
            sql.Append(d.QueueID.ToString());
            sql.Append(",");
            sql.Append("'");
            sql.Append(d.DeviceID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.SSID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.IP.ToString());
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
            sql.Append("");
            if (d.bOK)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key7_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key6_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key5_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key4_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key3_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key2_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key1_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key0_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key7_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key6_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key5_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key4_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key3_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key2_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key1_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key0_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key7_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key6_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key5_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key4_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key3_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key2_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key1_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Key0_11.ToString());
            sql.Append("'");
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int Update(KeyDataWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("update KeyDataWorkLog set ");
            sql.Append("DeviceID=");
            sql.Append("'");
            sql.Append(d.DeviceID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("SSID=");
            sql.Append("'");
            sql.Append(d.SSID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("IP=");
            sql.Append("'");
            sql.Append(d.IP.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("sTime=");
            sql.Append("'");
            sql.Append(d.sTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("eTime=");
            sql.Append("'");
            sql.Append(d.eTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bOK=");
            sql.Append("");
            if (d.bOK)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("Key7_51=");
            sql.Append("'");
            sql.Append(d.Key7_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key6_51=");
            sql.Append("'");
            sql.Append(d.Key6_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key5_51=");
            sql.Append("'");
            sql.Append(d.Key5_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key4_51=");
            sql.Append("'");
            sql.Append(d.Key4_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key3_51=");
            sql.Append("'");
            sql.Append(d.Key3_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key2_51=");
            sql.Append("'");
            sql.Append(d.Key2_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key1_51=");
            sql.Append("'");
            sql.Append(d.Key1_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key0_51=");
            sql.Append("'");
            sql.Append(d.Key0_51.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key7_41=");
            sql.Append("'");
            sql.Append(d.Key7_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key6_41=");
            sql.Append("'");
            sql.Append(d.Key6_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key5_41=");
            sql.Append("'");
            sql.Append(d.Key5_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key4_41=");
            sql.Append("'");
            sql.Append(d.Key4_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key3_41=");
            sql.Append("'");
            sql.Append(d.Key3_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key2_41=");
            sql.Append("'");
            sql.Append(d.Key2_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key1_41=");
            sql.Append("'");
            sql.Append(d.Key1_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key0_41=");
            sql.Append("'");
            sql.Append(d.Key0_41.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key7_11=");
            sql.Append("'");
            sql.Append(d.Key7_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key6_11=");
            sql.Append("'");
            sql.Append(d.Key6_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key5_11=");
            sql.Append("'");
            sql.Append(d.Key5_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key4_11=");
            sql.Append("'");
            sql.Append(d.Key4_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key3_11=");
            sql.Append("'");
            sql.Append(d.Key3_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key2_11=");
            sql.Append("'");
            sql.Append(d.Key2_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key1_11=");
            sql.Append("'");
            sql.Append(d.Key1_11.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("Key0_11=");
            sql.Append("'");
            sql.Append(d.Key0_11.ToString());
            sql.Append("'");
            sql.Append(" where QueueID=");
            sql.Append("");
            sql.Append(d.QueueID.ToString()); sql.Append("");

            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int Delete(KeyDataWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from KeyDataWorkLog ");
            sql.Append(" where QueueID=");
            sql.Append("");
            sql.Append(d.QueueID.ToString());
            sql.Append("");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataWorkLog ");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect(KeyDataWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataWorkLog where QueueID=");
            sql.Append("");
            sql.Append(d.QueueID.ToString());
            sql.Append("");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }        
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataWorkLog ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(KeyDataWorkLogData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataWorkLog ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
        public static KeyDataWorkLogData Get(Int32 _id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataWorkLog where QueueID=");
            sql.Append("");
            sql.Append(_id.ToString());
            sql.Append("");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return KeyDataWorkLogDataFactory.Construct(dt.Rows[0]);
        }
        public static string GetFile(string queueID, string keyIndex, string format)
        {
            //Key7_51
            string file = "key" + keyIndex + "_"+format;
            //if (format == "1")
            //{
            //    file += "11";
            //}
            //else if (format == "2")
            //{
            //    file += "41";
            //}
            //else if (format == "3")
            //{
            //    file += "51";
            //}
            //else
            //{
            //    return null;
            //}
            string sql = "select " + file + " from KeyDataWorkLog where QueueID=" + queueID;

            DataTable dt= WFNetLib.ADO.SQLServerOP.DataTableSQL(sql);
            if (dt.Rows.Count == 0)
            {
                return null;
            }
            else
            {
                return dt.Rows[0][0].ToString();
            }
        }
    }
}