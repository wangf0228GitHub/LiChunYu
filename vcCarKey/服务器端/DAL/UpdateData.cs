using System;
using System.Data;
using System.Text;
using System.Data.SqlClient;

namespace DAL
{
     [Serializable]
     public class UpdateData
     {

         public const string HEXVERField =@"HexVer";

         public const string HEXBINField =@"HexBin";

         public const string SOFTVERField =@"SoftVer";

         public const string SOFTWAREField =@"Software";

         private Double _HexVer;

         private byte[] _HexBin;

         private Double _SoftVer;

         private byte[] _Software;

         public Double HexVer
         {
             get{ return _HexVer;}
             set{ _HexVer=value;}
         }
         public byte[] HexBin
         {
             get{ return _HexBin;}
             set{ _HexBin=value;}
         }
         public Double SoftVer
         {
             get{ return _SoftVer;}
             set{ _SoftVer=value;}
         }
         public byte[] Software
         {
             get{ return _Software;}
             set{ _Software=value;}
         }
         public UpdateData()
         {
             _HexVer=0;
             _SoftVer=0;
         }
     }

     public class UpdateDataFactory
     {
//          public static UpdateData Construct(IDataReader reader)
//          {
//              UpdateData data = new UpdateData();
// 
//              try{  data.HexVer = Convert.ToDouble(reader["HexVer"])  ;} catch { } 
// 
//              try{  data.HexBin = Convert.to(reader["HexBin"])  ;} catch { } 
// 
//              try{  data.SoftVer = Convert.ToDouble(reader["SoftVer"])  ;} catch { } 
// 
//              try{  data.Software = Convert.To(reader["Software"])  ;} catch { } 
// 
//              return data;
//          }
//          public static UpdateData Construct(DataTable dt)
//          {
//              return Construct(dt,0);
//          }
//          public static UpdateData Construct(DataTable dt,int rowIndex)
//          {
//              UpdateData data = new UpdateData();
// 
//              if(dt.Rows.Count<rowIndex+1)
// 
//                  return null;
// 
//              DataRow dr=dt.Rows[rowIndex];
// 
//              if(dr==null)
// 
//                  return null;
// 
//              try{ data.HexVer = Convert.ToDouble(dr["HexVer"]) ;} catch { } 
// 
//              try{ data.HexBin = Convert.To(dr["HexBin"]) ;} catch { } 
// 
//              try{ data.SoftVer = Convert.ToDouble(dr["SoftVer"]) ;} catch { } 
// 
//              try{ data.Software = Convert.To(dr["Software"]) ;} catch { } 
// 
//              return data;
//          }
//          public static UpdateData Construct(DataRow dr)
//          {
//              UpdateData data = new UpdateData();
// 
//              try{ data.HexVer = Convert.ToDouble(dr["HexVer"]) ;} catch { } 
// 
//              try{ data.HexBin = Convert.To(dr["HexBin"]) ;} catch { } 
// 
//              try{ data.SoftVer = Convert.ToDouble(dr["SoftVer"]) ;} catch { } 
// 
//              try{ data.Software = Convert.To(dr["Software"]) ;} catch { } 
// 
//              return data;
//          }
     }
     public class UpdateDataDBOption
     {
         public static int Insert(UpdateData d)
         {
             StringBuilder sql=new StringBuilder();
             sql.Append("insert into Update (");
             sql.Append("HexVer, ");
             sql.Append("HexBin, ");
             sql.Append("SoftVer, ");
             sql.Append("Software");
             sql.Append(") values (");
             sql.Append("");
             sql.Append(d.HexVer.ToString());
             sql.Append("");
             sql.Append(", ");
             sql.Append("");
             sql.Append(d.HexBin.ToString());
             sql.Append("");
             sql.Append(", ");
             sql.Append("");
             sql.Append(d.SoftVer.ToString());
             sql.Append("");
             sql.Append(", ");
             sql.Append("");
             sql.Append(d.Software.ToString());
             sql.Append("");
             sql.Append(")");
             return WFNetLib.ADO.SQLServerOP.NonQuerySQL(sql.ToString());
         }
         public static DataTable DataTableSelect()
         {
             StringBuilder sql=new StringBuilder();
             sql.Append("select * from Update ");
             return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
         }
         public static DataTable SoftDataTableSelect()
         {
             StringBuilder sql = new StringBuilder();
             sql.Append("select [SoftVer],[Software] from [Update] ");
             return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
         }
         public static DataTable HexDataTableSelect()
         {
             StringBuilder sql = new StringBuilder();
             sql.Append("select [HexVer],[HexBin] from [Update] ");
             return WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
         }
         public static Double GetHexVer()
         {
             StringBuilder sql = new StringBuilder();
             sql.Append("select HexVer from [Update] ");             
             DataTable dt= WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
             if (dt.Rows.Count == 0)
                 return 0;
             else
                 return Convert.ToDouble(dt.Rows[0][0]);
         }
         public static Double GetSoftVer()
         {
             StringBuilder sql = new StringBuilder();
             sql.Append("select SoftVer from [Update] ");
             DataTable dt = WFNetLib.ADO.SQLServerOP.DataTableSQL(sql.ToString());
             if (dt.Rows.Count == 0)
                 return 0;
             else
                 return Convert.ToDouble(dt.Rows[0][0]);
         }
         public static DataSet DataSetSelect()
         {
             StringBuilder sql=new StringBuilder();
             sql.Append("select * from Update ");
             return WFNetLib.ADO.SQLServerOP.DataSetSQL(sql.ToString());
         }
         public static SqlDataReader ReaderSelect(UpdateData d)
         {
             StringBuilder sql=new StringBuilder();
             sql.Append("select * from Update ");
             return WFNetLib.ADO.SQLServerOP.ReaderExecuteSQL(sql.ToString());
         }
     }
}