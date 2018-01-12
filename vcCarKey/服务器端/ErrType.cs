using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace 服务器端
{
    public enum ErrType
    {
        retOK=1,
        MaxUseTimes=100,
        MissParam=-1,
        ErrParam=-2,
        MissServer = -3,
        ErrServerRet = -4,
        NoHexBin=-11,
        ErrFileLen = -12,
        NoRegDevice=-50,
        UnkownErr=-100
        //MOT
    }
}