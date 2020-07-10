#include "RF_TX.h"
#include "Variables.h"
#include "RF_RAM.h"
#include "RF_TX_Defs.h"
#include <util\delay.h>
#include "eep.h"
#include "EEProc.h"
uint8_t g_bVcoTuningResult;
uint8_t g_bAntennaTuningResult;
sRfTxConfig g_sRfTx;
sRfTxCurrentServiceChannelConfiguration g_sRfTxCurrentService;
volatile uint8_t g_sRfTxFlowCtrl_bIndex;
ISR(SSM_vect)
{
	ATA_SETBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_SSMREADY)
}
ISR(TMTCF_vect)//TxModulatorTelegramFinish
{
	/* LLR-Ref: 010 */
	SSMRR = _BM(SSMST);
	/* LLR-Ref: 020 */
	ATA_SETBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_TRANSMISSION_COMPLETE)
	/* LLR-Ref: 030 */
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_EOT)
}
ISR(SFFLR_vect)
{	
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_SFIFO_FILLLEVEL)
}
ISR(SFOUE_vect)
{
	/* LLR-Ref: 010 */
	ATA_CLEARBITMASK_C(SFI,_BM(SFERIM))
	/* LLR-Ref: 020 */
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_SFIFO_ERROR)
	/* LLR-Ref: 030 */
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_ERROR)
	/* LLR-Ref: 040 */
	g_sDebug.bErrorCode    = DEBUG_ERROR_CODE_RFTX_SFIFO_ERROR;
	g_sDebug.bSsmErrorCode = 0U;
}
ISR(DFFLR_vect)
{
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_DFIFO_FILLLEVEL)
}
ISR(DFOUE_vect)
{
	/* LLR-Ref: 010 */
	ATA_CLEARBITMASK_C(DFI,_BM(DFERIM))
	/* LLR-Ref: 020 */
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_DFIFO_ERROR)
	/* LLR-Ref: 030 */
	ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_ERROR)
	/* LLR-Ref: 040 */
	g_sDebug.bErrorCode    = DEBUG_ERROR_CODE_RFTX_DFIFO_ERROR;
	g_sDebug.bSsmErrorCode = 0U;
}
void ATA_rfTxInit_C(void)
{
	/* LLR-Ref: 010 */
	//FIFO & Support FIFO供电
	ATA_POWERON_C(PRR2, PRDF);
	ATA_POWERON_C(PRR2, PRSF);

	/* LLR-Ref: 020 */
	//开启fifo的错误中断
	ATA_SETBITMASK_C(DFI,_BM(DFERIM));//Data FIFO Error Interrupt Mask
	ATA_SETBITMASK_C(SFI,_BM(SFERIM));//Support FIFO Error Interrupt Mask
	
	/* LLR-Ref: 040 */
	//Initialize RFTX Module global variables which contains
	g_bVcoTuningResult = 0U;
	g_bAntennaTuningResult = 0U;

	/* LLR-Ref: 050 */
	//Initialize RFTX Module configuration
	g_sRfTx.bFlags      = 0x00U;
	g_sRfTx.bTuneFlags  = 0x00U;
	g_sRfTx.bStatus     = 0x00U;
	g_sRfTx.bConfig     = 0x00U;
	g_sRfTx.pAddress   = 0x0000U;
}
void ATA_rfTxFillSFifo_C(uint8_t bLen, uint8_t *pData)
{
	/* LLR-Ref: 010 */
	uint8_t tmp = SFC & _BM(SFDRA);//记录当前sFIFO的访问模式
	ATA_SETBITMASK_C(SFC,_BM(SFDRA))//avr写，rf读
	/* LLR-Ref: 020 */
	for(uint8_t i=0; i<bLen;i++)
	{
		SFD = *pData++;
	}
	/* LLR-Ref: 030 */
	//回复原sFIFO的访问模式
	if (!tmp) 
	{
		ATA_CLEARBITMASK_C(SFC,_BM(SFDRA))
	}
}
void ATA_rfTxFillDFifo_C(uint8_t bLen, uint8_t *pData)
{
	/* LLR-Ref: 010 */
	uint8_t tmp = DFC & _BM(DFDRA);//记录FIFO的访问模式
	ATA_SETBITMASK_C(DFC,_BM(DFDRA))//avr写，rf读
	/* LLR-Ref: 020 */
	for(uint8_t i=0; i<bLen;i++)
	{
		DFD = *pData++;
	}
	/* LLR-Ref: 030 */
	//回复原FIFO的访问模式
	if (!tmp) 
	{
		ATA_CLEARBITMASK_C(DFC,_BM(DFDRA))
	}
}

void ATA_rfTxStartTx_C(uint8_t bConfig)
{
	uint8_t i,x;
	uint8_t* pService;
	uint8_t bDirectSwitching = 0U;
	/* LLR-Ref: 010 */
	SSMRR = _BM(SSMST);//先停止状态机
	//记录原状态
	//RF Tx module ready for transmission (0: Not Ready, 1: Ready)
	bDirectSwitching = g_sRfTx.bFlags & BM_RFTXCONFIG_BFLAGS_RDY4TX;

	/* LLR-Ref: 020 */
	//射频前段和晶振供电
	ATA_SETBITMASK_C(SUPCR,_BM(AVEN))

	/* LLR-Ref: 030 */
	g_sRfTx.bFlags = 0x00U;
	g_sRfTx.bStatus = BM_RFTXCONFIG_BSTATUS_ACTIVE;
	g_sRfTx.bConfig = bConfig;
	/* --P2P-3655-- */
	g_sRfTx.bCmcrSetting = CMCR;

	/* LLR-Ref: 040 */
	//从eeprom或SRAM中获取rf配置
	//ATA_rfTxInitCurrentService_C();
	pService=(uint8_t *)&g_sRfTxCurrentService;
	for(i=0;i<33;i++)
	{
		*pService++=pgm_read_byte(&RF_RAM_Service[i]);
	}	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	RomData_ReadBytes(EEDataOriginAddr+0xf9,RomDatas, 2);
	x = RomDatas[0];
	x += RomDatas[1];
	if((RomDatas[0]==0x00) || (x!=0x00))//内存状态校验失败
	{
		ChangeRF433315State(ROM_9E);
		RomDatas[0] = ROM_9E;
	}
	if(RomDatas[0]==ROM_9E+1)//315
	{	
 		g_sRfTxCurrentService.sService.bIF[0]=RF315_bIF_0;
 		g_sRfTxCurrentService.sChannel.bFFREQ[0]=RF315_bFFREQ_0;
 		g_sRfTxCurrentService.sChannel.bFFREQ[1]=RF315_bFFREQ_1;
 		g_sRfTxCurrentService.sChannel.bFFREQ[2]=RF315_bFFREQ_2;
 		g_sRfTxCurrentService.sChannel.bFEMS=RF315_bFEMS;
 		g_sRfTxCurrentService.sChannel.bFECR=RF315_bFECR;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	if (g_sRfTx.bConfig & BM_KeyRF_MODE)//按键射频：1k 
	{
		g_sRfTxCurrentService.sPath.bGACDIV[0]=RF_bGACDIV_0;
		g_sRfTxCurrentService.sPath.bGACDIV[1]=RF_bGACDIV_1;
		g_sRfTxCurrentService.sPath.bTXDR[0]=RF_bTXDR_0;
		g_sRfTxCurrentService.sPath.bTXDR[1]=RF_bTXDR_1;
		g_sRfTxCurrentService.sPath.bTMCR2=RF_bTMCR2;
	}	
	if (g_sRfTx.bFlags & BM_RFTXCONFIG_BFLAGS_ERROR)
	{
		/* LLR-Ref: 050 */
		ATA_CLEARBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_ACTIVE)
		ATA_CLEARBITMASK_C(SUPCR,_BM(AVEN))//有错误，则关闭射频供电
	}
	else
	{
		/* LLR-Ref: 060 */
		if(bDirectSwitching)
		{
			ATA_SETBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_DIRECT_SWITCH)
		}
		g_sRfTxFlowCtrl_bIndex = 0U;		
	}
}
void ATA_rfTxFrequencySettings_C(void)
{
    /* LLR-Ref: 010 */
    uint32_t dwFrequency = ((uint32_t)g_sRfTxCurrentService.sChannel.bFFREQ[2] << 16U)
                        |((uint32_t)g_sRfTxCurrentService.sChannel.bFFREQ[1] <<  8U)
                        |((uint32_t)g_sRfTxCurrentService.sChannel.bFFREQ[0] <<  0U);
    /* --- F_TX = F_LO + F_IF --- */
    dwFrequency +=  ((uint32_t)g_sRfTxCurrentService.sService.bIF[1] <<  8U)
                   |((uint32_t)g_sRfTxCurrentService.sService.bIF[0] <<  0U);

    /* LLR-Ref: 020 */
    if (g_sRfTxCurrentService.sPath.bTxSetPath[1] & BM_RFTXSERVICE_BTXSETPATH_MODULATION) 
	{
        /* ASK Modulation */
        /* --- F_TX_ASK = F_LO + F_IF --- */
        /* LLR-Ref: 030 */
        FFREQ2H = FFREQ1H;
        FFREQ2M = FFREQ1M;
        FFREQ2L = FFREQ1L;

        /* LLR-Ref: 040 */
        FSCR    = _BM(TXMOD) | _BM(SFM);

        /* LLR-Ref: 050 */
        FFREQ1H = (uint8_t)(dwFrequency >> 16U);
        FFREQ1M = (uint8_t)(dwFrequency >>  8U);
        FFREQ1L = (uint8_t)(dwFrequency >>  0U);

        /* LLR-Ref: 060 */
        FSCR    = 0x00U;
    }
    else 
	{   /* FSK Modulation */
        uint32_t dwFreqdev = (  ((uint32_t)g_sRfTxCurrentService.sPath.btxDev[1] <<  8U)
                               |((uint32_t)g_sRfTxCurrentService.sPath.btxDev[0] <<  0U));
        /* LLR-Ref: 070 */
        FFREQ2H = FFREQ1H;
        FFREQ2M = FFREQ1M;
        FFREQ2L = FFREQ1L;

        /* LLR-Ref: 080 */
        FSCR    = _BM(TXMOD) | _BM(SFM);

        /* LLR-Ref: 090 */
        dwFrequency -= dwFreqdev/2U;

        /* LLR-Ref: 100 */
        FFREQ1H = (uint8_t)(dwFrequency >> 16U);
        FFREQ1M = (uint8_t)(dwFrequency >>  8U);
        FFREQ1L = (uint8_t)(dwFrequency >>  0U);

        /* LLR-Ref: 110 */
        FSCR    = 0x00U;

        /* LLR-Ref: 120 */
        dwFrequency += dwFreqdev;

        /* LLR-Ref: 130 */
        FFREQ2H = (uint8_t)(dwFrequency >> 16U);
        FFREQ2M = (uint8_t)(dwFrequency >>  8U);
        FFREQ2L = (uint8_t)(dwFrequency >>  0U);
    }
}
void ATA_rfTxSetupTxModulator_C(void)
{
	/* LLR-Ref: 010 */
	TMCR1 = _BM(TMCIM) | _BM(TMSCS);
	TMCR2 = g_sRfTxCurrentService.sPath.bTMCR2;
	TMSR  = _BM(TMTCF);
	TMSSC = g_sRfTxCurrentService.sPath.bTMSSC;
	TMTLL = g_sRfTxCurrentService.sPath.bTMTL[0];
	TMTLLH = g_sRfTxCurrentService.sPath.bTMTL[1];
	TMCPL = g_sRfTxCurrentService.sPath.bTMCP[0];
	TMCPH = g_sRfTxCurrentService.sPath.bTMCP[1];
	TMCIL = g_sRfTxCurrentService.sPath.bTMCI[0];
	TMCIH = g_sRfTxCurrentService.sPath.bTMCI[1];
	TMCSB = g_sRfTxCurrentService.sPath.bTMCSB;
}
void ATA_rfTxConfigureTxSSM_C(void)
{
	/* LLR-Ref: 010 */
	uint8_t config = g_sRfTx.bConfig & (BM_RFTXCONFIG_BCONFIG_ANT_TUNING | BM_RFTXCONFIG_BCONFIG_VCO_TUNING);

	/* LLR-Ref: 020 */
	SSMRR = _BM(SSMST);

	if (config == 0U) 
	{
		/* NO VCO TUNING */
		/* NO ANTENNA TUNING */
		/* LLR-Ref: 030 */
		MSMCR1 = SSM_PLL_EN_STATE           | (uint8_t)(SSM_PLL_LOCK_STATE << 4U);
		MSMCR2 = SSM_TX_DSP_EN_STATE        | (uint8_t)(SSM_END_STATE << 4U);
		MSMCR3 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
		MSMCR4 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
	}
	else if (config == BM_RFTXCONFIG_BCONFIG_VCO_TUNING) 
	{
		/* ONLY VCO TUNING */
		/* NO ANTENNA TUNING */
		/* LLR-Ref: 040 */
		MSMCR1 = SSM_PLL_EN_STATE           | (uint8_t)(SSM_VCO_TUNING_STATE << 4U);
		MSMCR2 = SSM_PLL_LOCK_STATE         | (uint8_t)(SSM_TX_DSP_EN_STATE << 4U);
		MSMCR3 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
		MSMCR4 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
	}
	else if (config == BM_RFTXCONFIG_BCONFIG_ANT_TUNING) 
	{
		/* NO VCO TUNING */
		/* ONLY ANTENNA TUNING */
		/* LLR-Ref: 050 */
		MSMCR1 = SSM_PLL_EN_STATE           | (uint8_t)(SSM_PLL_LOCK_STATE << 4U);
		MSMCR2 = SSM_TX_DSP_EN_STATE        | (uint8_t)(SSM_ANTENNA_TUNING_STATE << 4U);
		MSMCR3 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
		MSMCR4 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
	}
	else 
	{
		/* VCO TUNING */
		/* ANTENNA TUNING */
		/* LLR-Ref: 060 */
		MSMCR1 = SSM_PLL_EN_STATE           | (uint8_t)(SSM_VCO_TUNING_STATE << 4U);
		MSMCR2 = SSM_PLL_LOCK_STATE         | (uint8_t)(SSM_TX_DSP_EN_STATE << 4U);
		MSMCR3 = SSM_ANTENNA_TUNING_STATE   | (uint8_t)(SSM_END_STATE << 4U);
		MSMCR4 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
	}

	/* LLR-Ref: 070 */
	if (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_DIRECT_SWITCH)
	{
		/* in case of a direct switch -> replace PLL_EN SSM with TX_DSP_DIS */
		MSMCR1 &= (uint8_t)(_BM(MSMSM13) | _BM(MSMSM12) | _BM(MSMSM11) | _BM(MSMSM10));
		MSMCR1 |= SSM_TX_DSP_DIS_STATE;
	}

	/* LLR-Ref: 080 */
	SSMFBR = g_sRfTxCurrentService.sService.bSSMFBR;

	/* LLR-Ref: 090 */
	SSMCR = 0U;

	/* LLR-Ref: 100 */
	/* SSMCR.SSMTGE - Sequencer State Machine Tx Gauss Enable */
	if (g_sRfTxCurrentService.sPath.bTxSetPath[0] & BM_RFTXSERVICE_BTXSETPATH_GAUS) 
	{
		ATA_SETBITMASK_C(SSMCR,_BM(SSMTGE))
	}
	/* LLR-Ref: 110 */
	/* SSMCR.SSMTPE - Sequencer State Machine Tx Preemphasis Enable */
	if (g_sRfTxCurrentService.sPath.bTxSetPath[0] & BM_RFTXSERVICE_BTXSETPATH_PREE) 
	{
		ATA_SETBITMASK_C(SSMCR,_BM(SSMTPE))
	}
	/* LLR-Ref: 120 */
	/* SSMCR.SSMTAE - Sequencer State Machine Tx Ask-Shaping Enable */
	if (g_sRfTxCurrentService.sPath.bTxSetPath[1] & BM_RFTXSERVICE_BTXSETPATH_ASK_SHAPING) 
	{
		ATA_SETBITMASK_C(SSMCR,_BM(SSMTAE))
	}
}
void ATA_rfTxInitTxSSM_C(void)
{
	/* LLR-Ref: 010 */
	ATA_POWERON_C(PRR0, PRTXDC)//TX DSP
	ATA_POWERON_C(PRR2, PRSSM)//Tx Sequencer State Machine

	/* LLR-Ref: 020 */
	ATA_rfTxFrequencySettings_C();

	/* LLR-Ref: 030 */
	ATA_SETBITMASK_C(FSEN,_BM(SDEN))

	/* LLR-Ref: 040 */
	ATA_CLEARBITMASK_C(FSCR,_BM(TXMOD))

	if (g_sRfTx.bConfig & BM_RFTXCONFIG_BCONFIG_TRANSPARENT_MODE) 
	{
		/* Transparent mode */
		ATA_SETBITMASK_C(FSCR,_BM(TXMS0))
	}
	else 
	{
		/* Buffered mode */
		ATA_SETBITMASK_C(FSCR,_BM(TXMS1))
	}

	/* LLR-Ref: 050 */
	if( g_sRfTxCurrentService.sPath.bTxSetPath[1] & BM_RFTXSERVICE_BTXSETPATH_MODULATION ) 
	{
		/* ASK modulation */
		ATA_SETBITMASK_C(FSCR,_BM(TXMOD))
	}
	else 
	{
		/* FSK modulation */
		ATA_SETBITMASK_C(FSCR,_BM(SFM))
	}

	/* LLR-Ref: 060 */
	FSFCR   = g_sRfTxCurrentService.sPath.bFSFCR;
	GACDIVL = g_sRfTxCurrentService.sPath.bGACDIV[0];
	GACDIVH = g_sRfTxCurrentService.sPath.bGACDIV[1];

	/* LLR-Ref: 070 */
	ATA_rfTxSetupTxModulator_C();

	/* LLR-Ref: 080 */
	ATA_rfTxConfigureTxSSM_C();

	/* LLR-Ref: 090 */
	g_sRfTxFlowCtrl_bIndex++;
}
void ATA_rfTxWait4AVCC_C(void)
{
	/* LLR-Ref: 010 */
	ATA_SETBITMASK_C(SUPFR,(_BM(AVCCLF) | _BM(AVCCRF)))
	/* LLR-Ref: 020 */
	if ( (SUPFR & (_BM(AVCCLF)|_BM(AVCCRF))) == 0 )
	{
		g_sRfTxFlowCtrl_bIndex++;
	}
}
uint8_t ATA_rfTxFevcoCalculation_C(void)
{
	uint8_t bRetVal      = 0U;
	int8_t  bResult      = 0;
	/* LLR-Ref: 010 */
	uint8_t bFevcoOffset = 0U;
	bFevcoOffset = RomData_ReadByte(0x0905);//eepFEVCOoffset, 1U);
	/* LLR-Ref: 020 */
	uint8_t bVcob        = (g_sRfTxCurrentService.sService.bFEVCO & 0xF0U) >> 4U;
	uint8_t bVcobOffset  = (bFevcoOffset & 0xF0U) >> 4U;
	/* LLR-Ref: 030 */
	uint8_t bCpcc        = g_sRfTxCurrentService.sService.bFEVCO & 0x0FU;
	uint8_t bCpccOffset  = bFevcoOffset & 0x0FU;
	/* LLR-Ref: 040 */
	if (bVcob > 7U) 
	{
		ATA_SETBITMASK_C(bVcob,0xF0U)
	}
	/* LLR-Ref: 050 */
	if (bCpcc > 7U) 
	{
		ATA_SETBITMASK_C(bCpcc,0xF0U)
	}
	/* LLR-Ref: 060 */
	bResult = 7 + (int8_t)bVcob + (int8_t)bVcobOffset;
	if(bResult < 0) 
	{
		bResult = 0;
	}
	/* LLR-Ref: 070 */
	bVcob = (uint8_t)((uint8_t)bResult << 4U);
	bVcob &= 0xF0U;
	/* LLR-Ref: 080 */
	bResult = 7 + (int8_t)bCpcc + (int8_t)bCpccOffset;
	if (bResult < 0)
	{
		bResult = 0;
	}
	/* LLR-Ref: 090 */
	bCpcc = (uint8_t)bResult;
	bCpcc &= 0x0FU;
	/* LLR-Ref: 100 */
	bRetVal = bVcob | bCpcc;
	return bRetVal;
}
void ATA_rfTxInitFrontEnd_C(void)
{
	/* LLR-Ref: 010 */
	//FETN4 = eeprom_read_byte((const uint8_t *)0x0904);//eepFETN4, 1U);
	/* LLR-Ref: 020 */
	//ATA_SETBITMASK_C(FEEN1,_BM(XTOEN))
	/* LLR-Ref: 040 */
	FEAT = g_sRfTxCurrentService.sService.bFEAT;
	/* LLR-Ref: 050 */
	//FEBT = eeprom_read_byte((const uint8_t *)0x0903);//eepFEBT, 1U);	
	/* LLR-Ref: 060 */
	FEPAC = g_sRfTxCurrentService.sService.bFEPAC;
	/* LLR-Ref: 070 */
	FEVCO = ATA_rfTxFevcoCalculation_C();

	/* LLR-Ref: 090 */
	FECR  = g_sRfTxCurrentService.sChannel.bFECR;
	/* LLR-Ref: 100 */
	FEMS  = g_sRfTxCurrentService.sChannel.bFEMS;
	/* LLR-Ref: 110 */
	FEVCT = g_sRfTxCurrentService.sService.bFEVCT;
	/* LLR-Ref: 120 */
	if (g_sRfTxCurrentService.sService.bFEPAC < 0x2BU)
	{}
	else
	{
		ATA_SETBITMASK_C(SUPCR,_BM(AVDIC))
	}
	/* setting of FEALR.RNGE necessary for Antenna tuning (SSM review 22jul2011) */
	/* LLR-Ref: 130 */
	FEANT = (g_sRfTxCurrentService.sService.bFEALR_FEANT & 0x0FU);
	/* LLR-Ref: 140 */
	FEALR = ((g_sRfTxCurrentService.sService.bFEALR_FEANT & 0x30U) >> 4U);
	/* LLR-Ref: 150 */
	g_sRfTxFlowCtrl_bIndex++;
}
void ATA_rfTxWait4XTO_C(void)
{
	/* LLR-Ref: 010 */
	if (FESR & _BM(XRDY)) //XTO Ready
	{
		/* LLR-Ref: 020 */
		if (  ((CMCR & _BM(CMM2)) == 0)
			||((CMCR & _BM(CCS) ) == 0))
		{
			/* LLR-Ref: 030 */
			ATA_globalsClkSwitchXTO_C(0x07U);
		}
		/* LLR-Ref: 040 */
		g_sRfTxFlowCtrl_bIndex++;
	}
}
uint8_t ATA_rfTxStartSsmWatchdog_C(void)
{
	ATA_POWERON_C(PRR1, PRT2)
	T2CR &= ~_BM(T2ENA);
	T2CR = _BM(T2RES);	
	T2CR=_BM(T2ENA);
	T2MR=0x00;
	T2COR=0xff;
	T2IMR=0x00;
// 	sTimerAsyn8BitParams  sTimer2Params = {
// 		BM_T2ENA,                   /* T2CR */
// 		0x00U,                      /* T2MR */
// 		0xFFU,                      /* T2COR */
// 		0x00U,                      /* T2IMR */
// 		(timerIRQHandler)0x0000,    /* g_sTimer2.ovfIsr */
// 		(timerIRQHandler)0x0000     /* g_sTimer2.compIsr */
// 	};
	/* LLR-Ref: 010 */
	return 0;//OK;//ATA_timer2Open_C(&sTimer2Params);
}
void ATA_rfTxStartSSM_C(uint8_t bState)
{
	/* LLR-Ref: 010 */
	SSMIFR = 0x00U;
	/* LLR-Ref: 020 */
	ATA_SETBITMASK_C(SSMSR,_BM(SSMERR))
	/* LLR-Ref: 030 */
	ATA_CLEARBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_SSMREADY)
	/* LLR-Ref: 040 */
	SSMIMR = _BM(SSMIM);
	/* LLR-Ref: 050 */
	if( ATA_rfTxStartSsmWatchdog_C() )
	{
		/* LLR-Ref: 060 */
		ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_ERROR)
		/* LLR-Ref: 080 */
		g_sRfTxFlowCtrl_bIndex = bState;
	}
	else
	{
		/* LLR-Ref: 090 */
		SSMRR= _BM(SSMR);
		/* LLR-Ref: 100 */
		g_sRfTxFlowCtrl_bIndex++;
	}
}
void ATA_rfTxStartTxBufSSM_C(void)
{
	/* LLR-Ref: 010 */
	ATA_rfTxStartSSM_C(RFTX_BUF_STATE_SHUTDOWN);
}
void ATA_rfTxWait4SSMrdy_C(void)
{
	/* LLR-Ref: 010 */
	if( (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_SSMREADY) && (SSMSR == 0x00U) )
	{
		/* LLR-Ref: 020 */
		//ATA_rfTxStopSsmWatchdog_C();
		 ATA_POWERON_C(PRR1, PRT2)	
		 T2CR &= ~_BM(T2ENA);	 
		 T2CR = _BM(T2RES);
		 ATA_POWEROFF_C(PRR1, PRT2);
		/* LLR-Ref: 030 */
		ATA_CLEARBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_SSMREADY)
		/* LLR-Ref: 040 */
		ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_RDY4TX)
		/* LLR-Ref: 045 */
		g_sRfTxCurrentService.sService.bFEAT = FEAT;
		g_bAntennaTuningResult = g_sRfTxCurrentService.sService.bFEAT;
		g_sRfTxCurrentService.sService.bFEVCT = FEVCT;
		g_bVcoTuningResult = g_sRfTxCurrentService.sService.bFEVCT;
		/* LLR-Ref: 050 */
		if (g_sRfTx.bConfig & BM_RFTXCONFIG_BCONFIG_TRANSPARENT_MODE) 
		{
			ATA_SETBITMASK_C(FSCR,_BM(PAOER))
		}
		/* LLR-Ref: 060 */
		g_sRfTxFlowCtrl_bIndex++;
	}
	else 
	{
		/* LLR-Ref: 070 */
		if( (T2IFR & _BM(T2COF)) || SSMSR )
		{
			/* LLR-Ref: 080 */
			SSMRR = _BM(SSMST);       /* reset current state machine */

			/* LLR-Ref: 100 */
			ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_ERROR)

 			/* LLR-Ref: 110 */
 			g_sDebug.bErrorCode    = DEBUG_ERROR_CODE_RFTX_WAIT4SSMRDY_TIMEOUT;
 			g_sDebug.bSsmErrorCode = SSMSR;

			/* LLR-Ref: 120 */
			if (g_sRfTx.bConfig & BM_RFTXCONFIG_BCONFIG_TRANSPARENT_MODE) 
			{
				g_sRfTxFlowCtrl_bIndex = RFTX_TRANS_STATE_SHUTDOWN;
			}
			else 
			{
				g_sRfTxFlowCtrl_bIndex = RFTX_BUF_STATE_SHUTDOWN;
			}
		}
	}
}
void ATA_rfTxWait4FillLevel_C(void)
{
	/* LLR-Ref: 010 */
	uint8_t bTxFifoStartFillLevel = g_sRfTxCurrentService.sPath.bTxSetPath[0] & 0x3F;
	uint8_t bPreambleFifoStartFillLevel = g_sRfTxCurrentService.sPath.bTxSetPath[1] & 0x1F;

	/* LLR-Ref: 020 */
	if(
		(bTxFifoStartFillLevel <= (DFL & BM_DFFLS) )
	 && ( bPreambleFifoStartFillLevel <= (SFL & BM_SFFLS) )
	  ) 
	{
		/* LLR-Ref: 030 */
		g_sRfTxFlowCtrl_bIndex++;
	}
}
void ATA_rfTxOpenTimer3forTxing_C(void)
{
	/* LLR-Ref: 010 */
	//ATA_SETBITMASK_C(g_sTimer3.bStatus,TMR3LOCK)
	/* LLR-Ref: 020 */
	ATA_POWERON_C(PRR1, PRT3)
	/* LLR-Ref: 030 */
	T3IMR = 0U;//无中断
	T3MRB = 0U;
	T3MRA = _BM(T3CS1);//CLKXTO4
	/* LLR-Ref: 040 *///0x0190,约为50us
	T3CORL = g_sRfTxCurrentService.sPath.bTXDR[0];
	T3CORH = g_sRfTxCurrentService.sPath.bTXDR[1];

	/* LLR-Ref: 050 */
	ATA_SETBITMASK_C(T3CR,(_BM(T3ENA) | _BM(T3CRM) | _BM(T3CTM)))
}
void ATA_rfTxStartSendTelegramSSM_C(void)
{
	/* LLR-Ref: 010 */
	SSMRR = _BM(SSMST);
	/* LLR-Ref: 020 */
	MSMCR1 = SSM_SEND_TELEGRAM_STATE    | (uint8_t)(SSM_END_STATE << 4U);
	MSMCR2 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
	MSMCR3 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);
	MSMCR4 = SSM_END_STATE              | (uint8_t)(SSM_END_STATE << 4U);

	/* LLR-Ref: 030 */
	SSMIFR = 0x00U;

	/* LLR-Ref: 040 */
	ATA_SETBITMASK_C(SSMSR,_BM(SSMERR))

	/* LLR-Ref: 050 */
	ATA_CLEARBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_SSMREADY)

	/* LLR-Ref: 060 */
	SSMRR = _BM(SSMR);
}
void ATA_rfTxStartTxModulator_C(void)
{
	/* LLR-Ref: 010 */
	SFC = _BM(SFDRA);
	
	/* LLR-Ref: 020 */
	ATA_SETBITMASK_C(SFC,g_sRfTxCurrentService.sPath.bTxPreambleSysEvent & 0x1F)
	
	/* LLR-Ref: 030 */
	ATA_SETBITMASK_C(SFI,_BM(SFFLIM))

	/* LLR-Ref: 040 */
	DFC = _BM(DFDRA);
	
	/* LLR-Ref: 050 */
	ATA_SETBITMASK_C(DFC,g_sRfTxCurrentService.sPath.bTxSysEvent & 0x3F)
	
	/* LLR-Ref: 060 */
	ATA_SETBITMASK_C(DFI,_BM(DFFLIM))

	/* LLR-Ref: 070 */
	ATA_SETBITMASK_C(FSCR,_BM(PAOER))

	/* LLR-Ref: 080 */
	ATA_POWEROFF_C(PRR2, PRTM)
	ATA_POWERON_C(PRR2, PRTM)

	/* LLR-Ref: 090 */
	ATA_rfTxOpenTimer3forTxing_C();

	/* LLR-Ref: 100 */
	ATA_rfTxStartSendTelegramSSM_C();

	/* LLR-Ref: 110 */
	g_sRfTxFlowCtrl_bIndex++;
}
void ATA_rfTxWait4TransmissionComplete_C(void)
{
	/* LLR-Ref: 010 */
	if(g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_TRANSMISSION_COMPLETE)
	{
		/* LLR-Ref: 020 */
		g_sRfTx.bStatus &= (uint8_t)~BM_RFTXCONFIG_BSTATUS_TRANSMISSION_COMPLETE;
		/* LLR-Ref: 030 */
		ATA_CLEARBITMASK_C(SFI,_BM(SFFLIM))
		ATA_CLEARBITMASK_C(DFI,_BM(DFFLIM))
		/* LLR-Ref: 040 */
		if (g_sRfTx.bConfig & BM_RFTXCONFIG_BCONFIG_STAY_TX) 
		{
			g_sRfTxFlowCtrl_bIndex = RFTX_BUF_STATE_WAIT_FILLLEVEL;
		}
		else
		{
			g_sRfTxFlowCtrl_bIndex++;
		}
	}
}
void ATA_rfTxStartShutDownSSM_C(void)
{
	/* LLR-Ref: 010 */
	ATA_CLEARBITMASK_C(FSCR,_BM(PAOER))

	/* LLR-Ref: 020 */
	ATA_CLEARBITMASK_C(FSEN,_BM(ANTT))

	/* LLR-Ref: 030 */
	ATA_POWERON_C(PRR2, PRSSM)

	/* LLR-Ref: 040 */
	SSMRR = _BM(SSMST);

	/* LLR-Ref: 050 */
	SSMCR = 0x00U;

	/* LLR-Ref: 060 */
	MSMCR1 = SSM_SHUTDOWN_STATE | (uint8_t)(SSM_END_STATE << 4U);

	/* LLR-Ref: 070 */
	SSMIFR = 0x00U;

	/* LLR-Ref: 080 */
	ATA_SETBITMASK_C(SSMSR,_BM(SSMERR))

	/* LLR-Ref: 090 */
	SSMIMR = _BM(SSMIM);

	/* LLR-Ref: 100 */
	SSMRR  = _BM(SSMR);
}
void ATA_rfTxEnd_C(void)
{
	/* LLR-Ref: 010 */
	//ATA_timer3Close_C();
	ATA_POWERON_C(PRR1, PRT3)
	T3CR &= ~_BM(T3ENA);
	T3CR = _BM(T3RES);
	ATA_POWEROFF_C(PRR1, PRT3);
	/* LLR-Ref: 020 */
	FSCR = 0x00U;
	FSEN = 0x00U;

	/* LLR-Ref: 030 */
	SSMIMR = 0x00U;
	SSMRR  = _BM(SSMST);

	/* LLR-Ref: 040 */
	ATA_POWEROFF_C(PRR2,PRSSM)
	ATA_POWEROFF_C(PRR2,PRTM)
	ATA_POWEROFF_C(PRR0,PRCRC)
	ATA_POWEROFF_C(PRR0,PRTXDC)

	/* LLR-Ref: 050 */
	if (g_sRfTx.bConfig & BM_RFTXCONFIG_BCONFIG_SHUTDOWN_MODE) 
	{
		/* LLR-Ref: 060 */
		ATA_CLEARBITMASK_C(SUPCR,_BM(AVDIC))
		FEEN2 = 0U;
		FEEN1 = _BM(XTOEN);
	}
	else
	{
		/* LLR-Ref: 070 */
		/* --P2P-3655-- */
		if( g_sRfTx.bCmcrSetting & _BM(CCS))
		{
			uint8_t bClockSource = g_sRfTx.bCmcrSetting & (_BM(CMM2)|_BM(CMM1)|_BM(CMM0));
			if ( bClockSource== 0)  // CLK_SRC
			{
				ATA_globalsClkSwitchSrc_C();
			}
			else if (bClockSource == 2) // CLK_EXT
			{
				ATA_globalsClkSwitchFrc_C();
// 				if (SUPCR & BM_DVHEN)
// 				{
// 					ATA_globalsClkSwitchExt_C(TRUE);
// 				} 
// 				else 
// 				{
// 					ATA_globalsClkSwitchExt_C(FALSE);
// 				}
			}
			else // CLK_FRC
			{
				ATA_globalsClkSwitchFrc_C();
			}
		} 
		else 
		{    // CLK_MRC
			ATA_globalsClkSwitchMrc_C();
		}
		FEEN1 = 0U;
		ATA_CLEARBITMASK_C(SUPCR,(_BM(AVEN) | _BM(AVCCRM) | _BM(AVCCLM) | _BM(AVDIC)))
	}
	/* LLR-Ref: 080 */
	g_sRfTxFlowCtrl_bIndex    = 0U;
}
void ATA_rfTxShutdown_C(void)
{
	/* LLR-Ref: 010 */
	g_sRfTx.bStatus &= (uint8_t)~BM_RFTXCONFIG_BSTATUS_SSMREADY;	
	/* LLR-Ref: 020 */
	//ATA_rfTxStopSsmWatchdog_C();
	//关闭T2
	ATA_POWERON_C(PRR1, PRT2)
	T2CR &= ~_BM(T2ENA);
	T2CR = _BM(T2RES);
	ATA_POWEROFF_C(PRR1, PRT2);
	
	ATA_rfTxStartShutDownSSM_C();

	/* LLR-Ref: 030 */
	_delay_us(10);
	//ATA_globalsWaitNus_ASM(5U);

	/* LLR-Ref: 040 */
	if (   (g_sRfTx.bStatus & BM_RFTXCONFIG_BSTATUS_SSMREADY)
	    && (SSMSR == 0U) ) 
	{
		/* LLR-Ref: 050 */
		ATA_CLEARBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_SSMREADY)
	}
	else
	{
		/* LLR-Ref: 060 */
		ATA_SETBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_ERROR)
		
		/* LLR-Ref: 070 */
		g_sDebug.bErrorCode    = DEBUG_ERROR_CODE_RFTX_SHUTDOWN_ERROR;
		g_sDebug.bSsmErrorCode = SSMSR;
	}

	/* LLR-Ref: 080 */
	ATA_SETBITMASK_C(SFL,_BM(SFCLR))
	ATA_SETBITMASK_C(DFL,_BM(DFCLR))

	/* LLR-Ref: 090 */
	ATA_rfTxEnd_C();

	/* LLR-Ref: 100 */
	ATA_CLEARBITMASK_C(g_sRfTx.bStatus,BM_RFTXCONFIG_BSTATUS_ACTIVE)

	/* LLR-Ref: 110 */
	ATA_CLEARBITMASK_C(g_sRfTx.bFlags,BM_RFTXCONFIG_BFLAGS_RDY4TX)
}
void ATA_rfTxProcessing_C(void)
{
	switch(g_sRfTxFlowCtrl_bIndex)
	{
	case 0:
		ATA_rfTxInitTxSSM_C();
		break;
	case 1:
		ATA_rfTxWait4AVCC_C();
		break;
	case 2:
		ATA_rfTxInitFrontEnd_C();
		break;
	case 3:
		ATA_rfTxWait4XTO_C();
		break;
	case 4:
		ATA_rfTxStartTxBufSSM_C();
		break;
	case 5:
		ATA_rfTxWait4SSMrdy_C();
		break;
	case 6:
		ATA_rfTxWait4FillLevel_C();
		break;
	case 7:
		ATA_rfTxStartTxModulator_C();
		break;
	case 8:
		ATA_rfTxWait4TransmissionComplete_C();
		break;
	case 9:
		ATA_rfTxShutdown_C();
		break;
	}
}