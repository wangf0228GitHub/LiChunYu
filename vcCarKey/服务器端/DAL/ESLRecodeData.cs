using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class ESLRecodeData
    {

        public const string PASSField = @"Pass";

        public const string RETField = @"Ret";

        private String _Pass;

        private String _Ret;

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
        public ESLRecodeData()
        {
            _Pass = "";
            _Ret = "";
        }
    }

    public class ESLRecodeDataFactory
    {
        public static ESLRecodeData Construct(IDataReader reader)
        {
            ESLRecodeData data = new ESLRecodeData();

            try { data.Pass = Convert.ToString(reader["Pass"]); }
            catch { }

            try { data.Ret = Convert.ToString(reader["Ret"]); }
            catch { }

            return data;
        }
        public static ESLRecodeData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static ESLRecodeData Construct(DataTable dt, int rowIndex)
        {
            ESLRecodeData data = new ESLRecodeData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.Pass = Convert.ToString(dr["Pass"]); }
            catch { }

            try { data.Ret = Convert.ToString(dr["Ret"]); }
            catch { }

            return data;
        }
        public static ESLRecodeData Construct(DataRow dr)
        {
            ESLRecodeData data = new ESLRecodeData();

            try { data.Pass = Convert.ToString(dr["Pass"]); }
            catch { }

            try { data.Ret = Convert.ToString(dr["Ret"]); }
            catch { }

            return data;
        }
    }
    public class ESLRecodeDataDBOption
    {
        public static int Insert(ESLRecodeData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into ESLRecode (");
            sql.Append("Pass, ");
            sql.Append("Ret");
            sql.Append(") values (");
            sql.Append("'");
            sql.Append(d.Pass.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.Ret.ToString());
            sql.Append("'");
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLRecode ");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect(string pass)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLRecode where [Pass]='"+pass+"'");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLRecode ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(ESLRecodeData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from ESLRecode ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
    }
}