#include "avr\pgmspace.h"

#define RF_bGACDIV_0	0xF4	// 
#define RF_bGACDIV_1	0x03	// 
#define RF_bTXDR_0	0xDE	//
#define RF_bTXDR_1	0x0B	// 
#define RF_bTMCR2	0x70	// 高低位在前不一致

#define RF315_bIF_0	0xd5	// 
#define RF315_bFFREQ_0	0x82
#define RF315_bFFREQ_1	0xcc
#define RF315_bFFREQ_2	0x01
#define RF315_bFEMS	0x62
#define RF315_bFECR	0x01
PROGMEM const uint8_t RF_RAM_Service[]=
{
0x14,// bSSMFBR
0x00,// bFEALR_FEANT
0x07,// bFEAT
0x2e,// bFEPAC
0x10,// bFEVCO
0x07,// bFEVCT
0x94,// bIF[0]
0x0A,// bIF[1]
0xA3,// btxDev[0]
0x01,// btxDev[1]
0x64,// bGACDIV[0]
0x00,// bGACDIV[1]
0x00,// bFSFCR
0x30,// bTXDR[0]
0x01,// bTXDR[1]
0xC0,// bTxSetPath[0]
0x00,// bTxSetPath[1]
0x00,// bTxSysEvent
0x00,// bTxPreambleSysEvent
0x30,// bTMCR2
0x80,// bTMSSC
0x00,// bTMTL[0]
0x00,// bTMTL[1]
0x01,// bTMCP[0]
0x00,// bTMCP[1]
0x00,// bTMCI[0]
0x00,// bTMCI[1]
0x00,// bTMCSB
0x03,// bFFREQ[0]
0x5F,// bFFREQ[1]
0x02,// bFFREQ[2]
0x85,// bFEMS
0x03,// bFECR

};