using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class KeyDataQueueData
    {

        public const string IDField = @"ID";

        public const string PRIMARYKEY_Field = @"ID";

        public const string IPField = @"IP";

        public const string BINField = @"BIN";

        public const string STIMEField = @"sTime";

        public const string DEVICEIDField = @"DeviceID";

        public const string SSIDField = @"SSID";

        public const string KGField = @"KG";

        public const string KEYOPTIONField = @"KeyOption";

        public const string FORMATField = @"format";

        private Int32 _ID;

        private String _IP;

        private String _BIN;

        private DateTime _sTime;

        private String _DeviceID;

        private String _SSID;

        private Int32 _KG;

        private Int32 _KeyOption;

        private Int32 _format;

        public Int32 ID
        {
            get { return _ID; }
            set { _ID = value; }
        }
        public String IP
        {
            get { return _IP; }
            set { _IP = value; }
        }
        public String BIN
        {
            get { return _BIN; }
            set { _BIN = value; }
        }
        public DateTime sTime
        {
            get { return _sTime; }
            set { _sTime = value; }
        }
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
        public Int32 KG
        {
            get { return _KG; }
            set { _KG = value; }
        }
        public Int32 KeyOption
        {
            get { return _KeyOption; }
            set { _KeyOption = value; }
        }
        public Int32 format
        {
            get { return _format; }
            set { _format = value; }
        }
        public KeyDataQueueData()
        {
            _ID = 0;
            _IP = "";
            _BIN = "";
            _sTime = DateTime.Now;
            _DeviceID = "";
            _SSID = "";
            _KG = 0;
            _KeyOption = 0;
            _format = 0;
        }
    }

    public class KeyDataQueueDataFactory
    {
        public static KeyDataQueueData Construct(IDataReader reader)
        {
            KeyDataQueueData data = new KeyDataQueueData();

            try { data.ID = Convert.ToInt32(reader["ID"]); }
            catch { }

            try { data.IP = Convert.ToString(reader["IP"]); }
            catch { }

            try { data.BIN = Convert.ToString(reader["BIN"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(reader["sTime"]); }
            catch { }

            try { data.DeviceID = Convert.ToString(reader["DeviceID"]); }
            catch { }

            try { data.SSID = Convert.ToString(reader["SSID"]); }
            catch { }

            try { data.KG = Convert.ToInt32(reader["KG"]); }
            catch { }

            try { data.KeyOption = Convert.ToInt32(reader["KeyOption"]); }
            catch { }

            try { data.format = Convert.ToInt32(reader["format"]); }
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

            try { PrimaryKey = Convert.ToInt32(dr[KeyDataQueueData.PRIMARYKEY_Field]); }
            catch { }

            return PrimaryKey;
        }
        public static KeyDataQueueData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static KeyDataQueueData Construct(DataTable dt, int rowIndex)
        {
            KeyDataQueueData data = new KeyDataQueueData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.ID = Convert.ToInt32(dr["ID"]); }
            catch { }

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.BIN = Convert.ToString(dr["BIN"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            try { data.SSID = Convert.ToString(dr["SSID"]); }
            catch { }

            try { data.KG = Convert.ToInt32(dr["KG"]); }
            catch { }

            try { data.KeyOption = Convert.ToInt32(dr["KeyOption"]); }
            catch { }

            try { data.format = Convert.ToInt32(dr["format"]); }
            catch { }

            return data;
        }
        public static KeyDataQueueData Construct(DataRow dr)
        {
            KeyDataQueueData data = new KeyDataQueueData();

            try { data.ID = Convert.ToInt32(dr["ID"]); }
            catch { }

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.BIN = Convert.ToString(dr["BIN"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            try { data.SSID = Convert.ToString(dr["SSID"]); }
            catch { }

            try { data.KG = Convert.ToInt32(dr["KG"]); }
            catch { }

            try { data.KeyOption = Convert.ToInt32(dr["KeyOption"]); }
            catch { }

            try { data.format = Convert.ToInt32(dr["format"]); }
            catch { }

            return data;
        }
        public static Int32 GetPrimaryKey(DataRow dr)
        {
            Int32 PrimaryKey = 0;

            try { PrimaryKey = Convert.ToInt32(dr[KeyDataQueueData.PRIMARYKEY_Field]); }
            catch { }
            return PrimaryKey;
        }

    }
    public class KeyDataQueueDataDBOption
    {
        public static int Insert(KeyDataQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into KeyDataQueue (");
            sql.Append("IP, ");
            sql.Append("BIN, ");
            sql.Append("sTime, ");
            sql.Append("DeviceID, ");
            sql.Append("SSID, ");
            sql.Append("KG, ");
            sql.Append("KeyOption, ");
            sql.Append("format");
            sql.Append(") values (");
            sql.Append("'");
            sql.Append(d.IP.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.BIN.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.sTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.DeviceID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.SSID.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.KG.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.KeyOption.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.format.ToString());
            sql.Append("");
            sql.Append(")");
            sql.Append(" select @@identity");
            object o = WFNetLib.ADO.SQLServerOP.ScalarSQL(sql.ToString());
            return Convert.ToInt32(o);
        }
        public static int Update(KeyDataQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("update KeyDataQueue set ");
            sql.Append("IP=");
            sql.Append("'");
            sql.Append(d.IP.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("BIN=");
            sql.Append("'");
            sql.Append(d.BIN.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("sTime=");
            sql.Append("'");
            sql.Append(d.sTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
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
            sql.Append("KG=");
            sql.Append("");
            sql.Append(d.KG.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("KeyOption=");
            sql.Append("");
            sql.Append(d.KeyOption.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("format=");
            sql.Append("");
            sql.Append(d.format.ToString());
            sql.Append("");
            sql.Append(" where ID=");
            sql.Append("");
            sql.Append(d.ID.ToString()); sql.Append("");

            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int Delete(KeyDataQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from KeyDataQueue ");
            sql.Append(" where ID=");
            sql.Append("");
            sql.Append(d.ID.ToString());
            sql.Append("");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataQueue ");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect(KeyDataQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataQueue where ID=");
            sql.Append("");
            sql.Append(d.ID.ToString());
            sql.Append("");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static KeyDataQueueData Get(Int32 _id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataQueue where ID=");
            sql.Append("");
            sql.Append(_id.ToString());
            sql.Append("");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return KeyDataQueueDataFactory.Construct(dt.Rows[0]);
        }
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataQueue ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(KeyDataQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from KeyDataQueue ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
        public static void delete(KeyDataQueueData kd)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from KeyDataQueue where ID=" + kd.ID+ "");
            WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static void deleteAll()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from KeyDataQueue");
            WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static KeyDataQueueData GetFirst()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select TOP 1 * from KeyDataQueue order by sTime");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                return KeyDataQueueDataFactory.Construct(dt.Rows[0]);
            }
        }
        public static int GetIndex(string id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select QueueIndex from ");
            sql.Append("( SELECT [ID] ");
            sql.Append(",ROW_NUMBER() over(order by sTime) as QueueIndex ");
            sql.Append("FROM KeyDataQueue ) as QueueTable ");
            sql.Append("where [ID]=" + id + "");
            //sql.Append("select * from ESLQueue where Pass='" + pass + "'");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return -1;
            else
            {
                return int.Parse(dt.Rows[0][0].ToString());
            }
        }
    }
}