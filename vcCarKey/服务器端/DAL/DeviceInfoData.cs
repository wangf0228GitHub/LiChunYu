using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class DeviceInfoData
    {

        public const string IDField = @"ID";

        public const string PRIMARYKEY_Field = @"ID";

        public const string NEWTIMEField = @"NewTime";

        public const string HEXVERField = @"HexVer";

        public const string HEXUPTIMEField = @"HexUpTime";

        public const string SOFTVERField = @"SoftVer";

        public const string SOFTUPTIMEField = @"SoftUpTime";

        public const string BHPASSField = @"bHPass";

        public const string BDPASSField = @"bDPass";

        public const string BESLPASSField = @"bESLPass";

        public const string BKEYDATAField = @"bKeyData";

        public const string HPASSTIMESField = @"HPassTimes";

        public const string DPASSTIMESField = @"DPassTimes";

        public const string BAK1Field = @"bak1";

        public const string BAK2Field = @"bak2";

        public const string BAK3Field = @"bak3";

        public const string BAK4Field = @"bak4";

        public const string BAK5Field = @"bak5";

        public const string BAK6Field = @"bak6";

        private String _ID;

        private DateTime _NewTime;

        private Double _HexVer;

        private DateTime _HexUpTime;

        private Double _SoftVer;

        private DateTime _SoftUpTime;

        private Boolean _bHPass;

        private Boolean _bDPass;

        private Boolean _bESLPass;

        private Boolean _bKeyData;

        private Int32 _HPassTimes;

        private Int32 _DPassTimes;

        private String _bak1;

        private String _bak2;

        private String _bak3;

        private String _bak4;

        private String _bak5;

        private String _bak6;

        public String ID
        {
            get { return _ID; }
            set { _ID = value; }
        }
        public DateTime NewTime
        {
            get { return _NewTime; }
            set { _NewTime = value; }
        }
        public Double HexVer
        {
            get { return _HexVer; }
            set { _HexVer = value; }
        }
        public DateTime HexUpTime
        {
            get { return _HexUpTime; }
            set { _HexUpTime = value; }
        }
        public Double SoftVer
        {
            get { return _SoftVer; }
            set { _SoftVer = value; }
        }
        public DateTime SoftUpTime
        {
            get { return _SoftUpTime; }
            set { _SoftUpTime = value; }
        }
        public Boolean bHPass
        {
            get { return _bHPass; }
            set { _bHPass = value; }
        }
        public Boolean bDPass
        {
            get { return _bDPass; }
            set { _bDPass = value; }
        }
        public Boolean bESLPass
        {
            get { return _bESLPass; }
            set { _bESLPass = value; }
        }
        public Boolean bKeyData
        {
            get { return _bKeyData; }
            set { _bKeyData = value; }
        }
        public Int32 HPassTimes
        {
            get { return _HPassTimes; }
            set { _HPassTimes = value; }
        }
        public Int32 DPassTimes
        {
            get { return _DPassTimes; }
            set { _DPassTimes = value; }
        }
        public String bak1
        {
            get { return _bak1; }
            set { _bak1 = value; }
        }
        public String bak2
        {
            get { return _bak2; }
            set { _bak2 = value; }
        }
        public String bak3
        {
            get { return _bak3; }
            set { _bak3 = value; }
        }
        public String bak4
        {
            get { return _bak4; }
            set { _bak4 = value; }
        }
        public String bak5
        {
            get { return _bak5; }
            set { _bak5 = value; }
        }
        public String bak6
        {
            get { return _bak6; }
            set { _bak6 = value; }
        }
        public DeviceInfoData()
        {
            _ID = "";
            _NewTime = DateTime.Now;
            _HexVer = 0;
            _HexUpTime = DateTime.Now;
            _SoftVer = 0;
            _SoftUpTime = DateTime.Now;
            _bHPass = true;
            _bDPass = true;
            _bESLPass = true;
            _bKeyData = true;
            _HPassTimes = 10;
            _DPassTimes = 10;
            _bak1 = "";
            _bak2 = "";
            _bak3 = "";
            _bak4 = "";
            _bak5 = "";
            _bak6 = "";
        }
    }

    public class DeviceInfoDataFactory
    {
        public static DeviceInfoData Construct(IDataReader reader)
        {
            DeviceInfoData data = new DeviceInfoData();

            try { data.ID = Convert.ToString(reader["ID"]); }
            catch { }

            try { data.NewTime = Convert.ToDateTime(reader["NewTime"]); }
            catch { }

            try { data.HexVer = Convert.ToDouble(reader["HexVer"]); }
            catch { }

            try { data.HexUpTime = Convert.ToDateTime(reader["HexUpTime"]); }
            catch { }

            try { data.SoftVer = Convert.ToDouble(reader["SoftVer"]); }
            catch { }

            try { data.SoftUpTime = Convert.ToDateTime(reader["SoftUpTime"]); }
            catch { }

            try { data.bHPass = Convert.ToBoolean(reader["bHPass"]); }
            catch { }

            try { data.bDPass = Convert.ToBoolean(reader["bDPass"]); }
            catch { }

            try { data.bESLPass = Convert.ToBoolean(reader["bESLPass"]); }
            catch { }

            try { data.bKeyData = Convert.ToBoolean(reader["bKeyData"]); }
            catch { }

            try { data.HPassTimes = Convert.ToInt32(reader["HPassTimes"]); }
            catch { }

            try { data.DPassTimes = Convert.ToInt32(reader["DPassTimes"]); }
            catch { }

            try { data.bak1 = Convert.ToString(reader["bak1"]); }
            catch { }

            try { data.bak2 = Convert.ToString(reader["bak2"]); }
            catch { }

            try { data.bak3 = Convert.ToString(reader["bak3"]); }
            catch { }

            try { data.bak4 = Convert.ToString(reader["bak4"]); }
            catch { }

            try { data.bak5 = Convert.ToString(reader["bak5"]); }
            catch { }

            try { data.bak6 = Convert.ToString(reader["bak6"]); }
            catch { }

            return data;
        }
        public static String GetPrimaryKey(DataTable dt)
        {
            return GetPrimaryKey(dt, 0);
        }
        public static String GetPrimaryKey(DataTable dt, int rowIndex)
        {
            String PrimaryKey = "";

            if (dt.Rows.Count < rowIndex + 1)

                return "";

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return "";

            try { PrimaryKey = Convert.ToString(dr[DeviceInfoData.PRIMARYKEY_Field]); }
            catch { }

            return PrimaryKey;
        }
        public static DeviceInfoData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static DeviceInfoData Construct(DataTable dt, int rowIndex)
        {
            DeviceInfoData data = new DeviceInfoData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.ID = Convert.ToString(dr["ID"]); }
            catch { }

            try { data.NewTime = Convert.ToDateTime(dr["NewTime"]); }
            catch { }

            try { data.HexVer = Convert.ToDouble(dr["HexVer"]); }
            catch { }

            try { data.HexUpTime = Convert.ToDateTime(dr["HexUpTime"]); }
            catch { }

            try { data.SoftVer = Convert.ToDouble(dr["SoftVer"]); }
            catch { }

            try { data.SoftUpTime = Convert.ToDateTime(dr["SoftUpTime"]); }
            catch { }

            try { data.bHPass = Convert.ToBoolean(dr["bHPass"]); }
            catch { }

            try { data.bDPass = Convert.ToBoolean(dr["bDPass"]); }
            catch { }

            try { data.bESLPass = Convert.ToBoolean(dr["bESLPass"]); }
            catch { }

            try { data.bKeyData = Convert.ToBoolean(dr["bKeyData"]); }
            catch { }

            try { data.HPassTimes = Convert.ToInt32(dr["HPassTimes"]); }
            catch { }

            try { data.DPassTimes = Convert.ToInt32(dr["DPassTimes"]); }
            catch { }

            try { data.bak1 = Convert.ToString(dr["bak1"]); }
            catch { }

            try { data.bak2 = Convert.ToString(dr["bak2"]); }
            catch { }

            try { data.bak3 = Convert.ToString(dr["bak3"]); }
            catch { }

            try { data.bak4 = Convert.ToString(dr["bak4"]); }
            catch { }

            try { data.bak5 = Convert.ToString(dr["bak5"]); }
            catch { }

            try { data.bak6 = Convert.ToString(dr["bak6"]); }
            catch { }

            return data;
        }
        public static DeviceInfoData Construct(DataRow dr)
        {
            DeviceInfoData data = new DeviceInfoData();

            try { data.ID = Convert.ToString(dr["ID"]); }
            catch { }

            try { data.NewTime = Convert.ToDateTime(dr["NewTime"]); }
            catch { }

            try { data.HexVer = Convert.ToDouble(dr["HexVer"]); }
            catch { }

            try { data.HexUpTime = Convert.ToDateTime(dr["HexUpTime"]); }
            catch { }

            try { data.SoftVer = Convert.ToDouble(dr["SoftVer"]); }
            catch { }

            try { data.SoftUpTime = Convert.ToDateTime(dr["SoftUpTime"]); }
            catch { }

            try { data.bHPass = Convert.ToBoolean(dr["bHPass"]); }
            catch { }

            try { data.bDPass = Convert.ToBoolean(dr["bDPass"]); }
            catch { }

            try { data.bESLPass = Convert.ToBoolean(dr["bESLPass"]); }
            catch { }

            try { data.bKeyData = Convert.ToBoolean(dr["bKeyData"]); }
            catch { }

            try { data.HPassTimes = Convert.ToInt32(dr["HPassTimes"]); }
            catch { }

            try { data.DPassTimes = Convert.ToInt32(dr["DPassTimes"]); }
            catch { }

            try { data.bak1 = Convert.ToString(dr["bak1"]); }
            catch { }

            try { data.bak2 = Convert.ToString(dr["bak2"]); }
            catch { }

            try { data.bak3 = Convert.ToString(dr["bak3"]); }
            catch { }

            try { data.bak4 = Convert.ToString(dr["bak4"]); }
            catch { }

            try { data.bak5 = Convert.ToString(dr["bak5"]); }
            catch { }

            try { data.bak6 = Convert.ToString(dr["bak6"]); }
            catch { }

            return data;
        }
        public static String GetPrimaryKey(DataRow dr)
        {
            String PrimaryKey = "";

            try { PrimaryKey = Convert.ToString(dr[DeviceInfoData.PRIMARYKEY_Field]); }
            catch { }
            return PrimaryKey;
        }

    }
    public class DeviceInfoDataDBOption
    {
        public static int Insert(DeviceInfoData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into DeviceInfo (");
            sql.Append("ID, ");
            sql.Append("NewTime, ");
            sql.Append("HexVer, ");
            sql.Append("HexUpTime, ");
            sql.Append("SoftVer, ");
            sql.Append("SoftUpTime, ");
            sql.Append("bHPass, ");
            sql.Append("bDPass, ");
            sql.Append("bESLPass, ");
            sql.Append("bKeyData, ");
            sql.Append("HPassTimes, ");
            sql.Append("DPassTimes, ");
            sql.Append("bak1, ");
            sql.Append("bak2, ");
            sql.Append("bak3, ");
            sql.Append("bak4, ");
            sql.Append("bak5, ");
            sql.Append("bak6");
            sql.Append(") values (");
            sql.Append("'");
            sql.Append(d.ID);
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.NewTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.HexVer.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.HexUpTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.SoftVer.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.SoftUpTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("");
            if (d.bHPass)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            if (d.bDPass)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            if (d.bESLPass)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            if (d.bKeyData)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.HPassTimes.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("");
            sql.Append(d.DPassTimes.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.bak1.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.bak2.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.bak3.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.bak4.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.bak5.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.bak6.ToString());
            sql.Append("'");
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int Update(DeviceInfoData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("update DeviceInfo set ");
            sql.Append("NewTime=");
            sql.Append("'");
            sql.Append(d.NewTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("HexVer=");
            sql.Append("");
            sql.Append(d.HexVer.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("HexUpTime=");
            sql.Append("'");
            sql.Append(d.HexUpTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("SoftVer=");
            sql.Append("");
            sql.Append(d.SoftVer.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("SoftUpTime=");
            sql.Append("'");
            sql.Append(d.SoftUpTime.ToString("yyyy-MM-dd HH:mm:ss"));
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bHPass=");
            sql.Append("");
            if (d.bHPass)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("bDPass=");
            sql.Append("");
            if (d.bDPass)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("bESLPass=");
            sql.Append("");
            if (d.bESLPass)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("bKeyData=");
            sql.Append("");
            if (d.bKeyData)
                sql.Append("1");
            else
                sql.Append("0");
            sql.Append("");
            sql.Append(", ");
            sql.Append("HPassTimes=");
            sql.Append("");
            sql.Append(d.HPassTimes.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("DPassTimes=");
            sql.Append("");
            sql.Append(d.DPassTimes.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("bak1=");
            sql.Append("'");
            sql.Append(d.bak1.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bak2=");
            sql.Append("'");
            sql.Append(d.bak2.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bak3=");
            sql.Append("'");
            sql.Append(d.bak3.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bak4=");
            sql.Append("'");
            sql.Append(d.bak4.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bak5=");
            sql.Append("'");
            sql.Append(d.bak5.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("bak6=");
            sql.Append("'");
            sql.Append(d.bak6.ToString());
            sql.Append("'");
            sql.Append(" where ID=");
            sql.Append("'");
            sql.Append(d.ID.ToString()); sql.Append("'");

            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static int Delete(DeviceInfoData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("delete from DeviceInfo ");
            sql.Append(" where ID=");
            sql.Append("'");
            sql.Append(d.ID.ToString());
            sql.Append("'");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DeviceInfo order by NewTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect_addrownum()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select ROW_NUMBER() over(order by NewTime desc) as DeviceIndex,* from DeviceInfo order by NewTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect_byID(string id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select ROW_NUMBER() over(order by NewTime desc) as DeviceIndex,* from DeviceInfo where ID like '%" + id + "%' order by NewTime desc");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect(DeviceInfoData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DeviceInfo where ID=");
            sql.Append("'");
            sql.Append(d.ID.ToString());
            sql.Append("'");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static int SetTimes(int d1,int d2)
        {
            StringBuilder sql = new StringBuilder();            
            sql.Append("update DeviceInfo set ");            
            sql.Append("HPassTimes=");
            sql.Append("");
            sql.Append(d1.ToString());
            sql.Append("");
            sql.Append(", ");
            sql.Append("DPassTimes=");
            sql.Append("");
            sql.Append(d2.ToString());
            sql.Append(""); 
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DeviceInfoData Get(String _id)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DeviceInfo where ID=");
            sql.Append("'");
            sql.Append(_id.ToString());
            sql.Append("'");
            DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
            if (dt.Rows.Count == 0)
                return null;
            else
                return DeviceInfoDataFactory.Construct(dt.Rows[0]);
        }
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DeviceInfo ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(DeviceInfoData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DeviceInfo ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
    }
}