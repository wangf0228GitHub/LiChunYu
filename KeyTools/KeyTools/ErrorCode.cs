using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KeyTools
{
    public enum ErrorCode
    {
        ErrorCode_NoErr = 0,


        ErrorCode_NoSerialProcCallback = 256,

        ErrorCode_SerialProcError = 300,

        ErrorCode_CanReadEEPROM_nErr = 400
    };
    public class ErrorCodeTranslate
    {
        public static string GetLastError(int work,int code)
        {
            ErrorCode err=(ErrorCode)code;
            if (code < 256)
            {
                switch (work)
                {
                    case 1://eis
                        switch (code)
                        {
                            case 2:
                                return "CAN发送不成功，请确认底盘类型是否正确";
                            case 3:
                                return "CAN接收超时";
                        }
                        break;
                }
            }
            switch (err)
            {
                case ErrorCode.ErrorCode_NoSerialProcCallback:
                    return "未设置回调函数";
                case ErrorCode.ErrorCode_SerialProcError:
                    return "与设备通信超时";
                //case ErrorCode.ErrorCode_CanReadEEPROM_nErr:
            }
            return "";
        }
    }
}
