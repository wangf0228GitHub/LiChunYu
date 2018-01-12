using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PCTest
{
    public class EIS_Mode1_ReadData
    {
        public CANCommand[] canCommand=new CANCommand[1+46*3];
        public EIS_Mode1_ReadData()
        {
            canCommand[0] = new CANCommand();
            canCommand[0].bTxOnly = 1;
            canCommand[0].TxStdId = 0x01c;
            canCommand[0].TxData = new byte[] { 0x02, 0x10, 0x92, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            canCommand[0].TxRetry = 10;
            canCommand[0].TxTimeOut = 10;

            for (int i = 0; i < 46; i++)
            {
                canCommand[1 + i * 3] = new CANCommand();
                canCommand[1 + i * 3].bTxOnly = 1;
                canCommand[1 + i * 3].TxStdId = 0x01c;
                canCommand[1 + i * 3].TxData = new byte[] { 0x02, 0x3E, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
                canCommand[1 + i * 3].TxRetry = 10;
                canCommand[1 + i * 3].TxTimeOut = 10;

                canCommand[1 + i * 3+1] = new CANCommand();
                canCommand[1 + i * 3+1].bTxOnly = 0;
                canCommand[1 + i * 3+1].TxStdId = 0x4e0;
                canCommand[1 + i * 3 + 1].TxData = new byte[] { 0x04, 0x31, 0x22, 0x6F, 0x00, 0xFF, 0xFF, 0xFF };
                //命令字
                canCommand[1 + i * 3 + 1].TxData[4] =(byte)(i);

                canCommand[1 + i * 3+1].TxRetry = 10;
                canCommand[1 + i * 3+1].TxTimeOut = 10;

                canCommand[1 + i * 3 + 1].NeedStdId = 0x5ff;
                canCommand[1 + i * 3 + 1].RxRetry = 3;
                canCommand[1 + i * 3 + 1].RxTimeOut = 500;
                canCommand[1 + i * 3 + 1].RxIDE = CANCommand.CAN_ID_STD;


                canCommand[1 + i * 3 + 2] = new CANCommand();
                canCommand[1 + i * 3 + 2].bTxOnly = 0;
                canCommand[1 + i * 3 + 2].TxStdId = 0x4e0;
                canCommand[1 + i * 3 + 2].TxData = new byte[] { 0x30, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };
                canCommand[1 + i * 3 + 2].TxRetry = 10;
                canCommand[1 + i * 3 + 2].TxTimeOut = 10;

                canCommand[1 + i * 3 + 2].NeedStdId = 0x5ff;
                canCommand[1 + i * 3 + 2].RxRetry = 3;
                canCommand[1 + i * 3 + 2].RxTimeOut = 500;
                canCommand[1 + i * 3 + 2].RxIDE = CANCommand.CAN_ID_STD;
            }
        }
    }
}
