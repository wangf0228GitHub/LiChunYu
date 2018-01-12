using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class ESLQueueData
    {

        public const string IPField = @"IP";

        public const string PASSField = @"Pass";

        public const string STIMEField = @"sTime";

        public const string DEVICEIDField = @"DeviceID";

        private String _IP;

        private String _Pass;

        private DateTime _sTime;

        private String _DeviceID;

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
        public ESLQueueData()
        {
            _IP = "";
            _Pass = "";
            _sTime = DateTime.Now;
            _DeviceID = "";
        }
    }

    public class ESLQueueDataFactory
    {
        public static ESLQueueData Construct(IDataReader reader)
        {
            ESLQueueData data = new ESLQueueData();

            try { data.IP = Convert.ToString(reader["IP"]); }
            catch { }

            try { data.Pass = Convert.ToString(reader["Pass"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(reader["sTime"]); }
            catch { }

            try { data.DeviceID = Convert.ToString(reader["DeviceID"]); }
            catch { }

            return data;
        }
        public static ESLQueueData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static ESLQueueData Construct(DataTable dt, int rowIndex)
        {
            ESLQueueData data = new ESLQueueData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.Pass = Convert.ToString(dr["Pass"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            return data;
        }
        public static ESLQueueData Construct(DataRow dr)
        {
            ESLQueueData data = new ESLQueueData();

            try { data.IP = Convert.ToString(dr["IP"]); }
            catch { }

            try { data.Pass = Convert.ToString(dr["Pass"]); }
            catch { }

            try { data.sTime = Convert.ToDateTime(dr["sTime"]); }
            catch { }

            try { data.DeviceID = Convert.ToString(dr["DeviceID"]); }
            catch { }

            return data;
        }
    }
    public class ESLQueueDataDBOption
    {
        public static int Insert(ESLQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into ESLQueue (");
            sql.Append("IP, ");
            sql.Append("Pass, ");
            sql.Append("sTime, ");
            sql.Append("DeviceID");
            sql.Append(") values (");
            sql.Append("'");
            sql.Append(d.IP.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Pass.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.sTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.DeviceID.ToString());
            sql.Append("'");
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLQueue order by sTime");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static ESLQueueData GetFirst()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select TOP 1 * from ESLQueue order by sTime");
            DataTable dt= WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
            {
                return ESLQueueDataFactory.Construct(dt.Rows[0]);
            }
        }
        public static void delete(string pass, string ip, string deviceID,string sTime)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from ESLQueue where [Pass]='" + pass + "' and IP='" + ip + "' and DeviceID='" + deviceID + "' and sTime='"+sTime+"'");
            WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static void deleteAll()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from ESLQueue");
            WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int GetIndex(string pass)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select QueueIndex from ");
            sql.Append("( SELECT [Pass] ");
            sql.Append(",[sTime],ROW_NUMBER() over(order by sTime) as QueueIndex ");
            sql.Append("FROM [LCY_Server].[dbo].[ESLQueue] ) as QueueTable ");
            sql.Append("where [Pass]='" + pass + "'");
            sql.Append("select * from ESLQueue where Pass='"+pass+"'");
            DataTable dt= WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return -1;
            else
            {
                return int.Parse(dt.Rows[0][0].ToString());
            }
        }
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLQueue order by sTime");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(ESLQueueData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLQueue ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
    }
}