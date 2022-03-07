#ifndef __STDAFX_H__
#define __STDAFX_H__

#define _XTAL_FREQ 4000000
#include "mcc_generated_files/mcc.h"
#include "FlagDefine.h"

#define Verify_Sum
#include "Verify.h"

#define CP1616_Client_AddrLen 1
#define CP1616_Client_DataBufLen 1

#define CP1616_Client_NeedCheckVerify
#define CP1616_Client_Tx_OneByOne
#define CP1616_Client_TxByteWithVerify(x) EUSART1_Write(x);sum=(unsigned char)(sum+x)
#define CP1616_Client_TxByte(x) EUSART1_Write(x)
#define CP1616_Client_VerifyProc(pBuff,Count) GetVerify_Sum(pBuff,Count)


#define CP1616_Client_RxList_LenMax 20
#define CP1616_Client_TxList_LenMax 20
#include "CP1616_Client.h"


#endif
