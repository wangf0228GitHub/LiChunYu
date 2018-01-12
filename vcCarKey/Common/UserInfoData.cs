using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Common
{
    [Serializable]
    public class UserInfoData
    {

        public const string IDField = @"ID";

        public const string PRIMARYKEY_Field = @"ID";

        public const string NAMEField = @"Name";

        public const string SEXField = @"Sex";

        public const string NATIONField = @"Nation";

        public const string BORNField = @"Born";

        public const string ADDRESSField = @"Address";

        public const string GRANTDEPTField = @"GrantDept";

        public const string USERLIFEBEGINField = @"UserLifeBegin";

        public const string USERLIFEENDField = @"UserLifeEnd";

        public const string PHONENUMField = @"PhoneNum";

        public const string BJIANHURENField = @"bJianHuRen";

        private String _ID;

        private String _Name;

        private String _Sex;

        private String _Nation;

        private String _Born;

        private String _Address;

        private String _GrantDept;

        private String _UserLifeBegin;

        private String _UserLifeEnd;

        private String _PhoneNum;

        private Boolean _bJianHuRen;

        public String ID
        {
            get { return _ID; }
            set { _ID = value; }
        }
        public String Name
        {
            get { return _Name; }
            set { _Name = value; }
        }
        public String Sex
        {
            get { return _Sex; }
            set { _Sex = value; }
        }
        public String Nation
        {
            get { return _Nation; }
            set { _Nation = value; }
        }
        public String Born
        {
            get { return _Born; }
            set { _Born = value; }
        }
        public String Address
        {
            get { return _Address; }
            set { _Address = value; }
        }
        public String GrantDept
        {
            get { return _GrantDept; }
            set { _GrantDept = value; }
        }
        public String UserLifeBegin
        {
            get { return _UserLifeBegin; }
            set { _UserLifeBegin = value; }
        }
        public String UserLifeEnd
        {
            get { return _UserLifeEnd; }
            set { _UserLifeEnd = value; }
        }
        public String PhoneNum
        {
            get { return _PhoneNum; }
            set { _PhoneNum = value; }
        }
        public Boolean bJianHuRen
        {
            get { return _bJianHuRen; }
            set { _bJianHuRen = value; }
        }
        public UserInfoData()
        {
            _ID = "";
            _Name = "";
            _Sex = "";
            _Nation = "";
            _Born = "";
            _Address = "";
            _GrantDept = "";
            _UserLifeBegin = "";
            _UserLifeEnd = "";
            _PhoneNum = "";
            _bJianHuRen = false;
        }
    } 
}
