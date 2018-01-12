using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
    [Serializable]
    public class DPassRecodeData
    {

        public const string DEALERField = @"Dealer";

        public const string DPASSField = @"DPass";

        private String _Dealer;

        private String _DPass;

        public String Dealer
        {
            get { return _Dealer; }
            set { _Dealer = value; }
        }
        public String DPass
        {
            get { return _DPass; }
            set { _DPass = value; }
        }
        public DPassRecodeData()
        {
            _Dealer = "";
            _DPass = "";
        }
    }

    public class DPassRecodeDataFactory
    {
        public static DPassRecodeData Construct(IDataReader reader)
        {
            DPassRecodeData data = new DPassRecodeData();

            try { data.Dealer = Convert.ToString(reader["Dealer"]); }
            catch { }

            try { data.DPass = Convert.ToString(reader["DPass"]); }
            catch { }

            return data;
        }
        public static DPassRecodeData Construct(DataTable dt)
        {
            return Construct(dt, 0);
        }
        public static DPassRecodeData Construct(DataTable dt, int rowIndex)
        {
            DPassRecodeData data = new DPassRecodeData();

            if (dt.Rows.Count < rowIndex + 1)

                return null;

            DataRow dr = dt.Rows[rowIndex];

            if (dr == null)

                return null;

            try { data.Dealer = Convert.ToString(dr["Dealer"]); }
            catch { }

            try { data.DPass = Convert.ToString(dr["DPass"]); }
            catch { }

            return data;
        }
        public static DPassRecodeData Construct(DataRow dr)
        {
            DPassRecodeData data = new DPassRecodeData();

            try { data.Dealer = Convert.ToString(dr["Dealer"]); }
            catch { }

            try { data.DPass = Convert.ToString(dr["DPass"]); }
            catch { }

            return data;
        }
    }
    public class DPassRecodeDataDBOption
    {
        public static int Insert(DPassRecodeData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("insert into DPassRecode (");
            sql.Append("Dealer, ");
            sql.Append("DPass");
            sql.Append(") values (");
            sql.Append("'");
            sql.Append(d.Dealer.ToString());
            sql.Append("'");
            sql.Append(", ");
            sql.Append("'");
            sql.Append(d.DPass.ToString());
            sql.Append("'");
            sql.Append(")");
            return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
        }
        public static DataTable DataTableSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassRecode ");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataTable DataTableSelect(string Dealer)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassRecode where Dealer='" + Dealer + "'");
            return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
        }
        public static DataSet DataSetSelect()
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassRecode ");
            return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
        }
        public static SqlDataReader ReaderSelect(DPassRecodeData d)
        {
            StringBuilder sql = new StringBuilder();
            sql.Append("select * from DPassRecode ");
            return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
        }
    }
}