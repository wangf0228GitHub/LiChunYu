#ifndef RF_TX_Defs_H
#define RF_TX_Defs_H

// state machine defines for RFTX SW state machine if buffered mode is used
#define RFTX_BUF_STATE_INIT_SSM                     0x00U
#define RFTX_BUF_STATE_WAIT_AVCC                    RFTX_BUF_STATE_INIT_SSM + 0x01U
#define RFTX_BUF_STATE_INIT_FE                      RFTX_BUF_STATE_WAIT_AVCC + 0x01U
#define RFTX_BUF_STATE_WAIT_XTO                     RFTX_BUF_STATE_INIT_FE + 0x01U
#define RFTX_BUF_STATE_START_SSM                    RFTX_BUF_STATE_WAIT_XTO + 0x01U
#define RFTX_BUF_STATE_WAIT_SSMRDY                  RFTX_BUF_STATE_START_SSM + 0x01U
#define RFTX_BUF_STATE_WAIT_FILLLEVEL               RFTX_BUF_STATE_WAIT_SSMRDY + 0x01U
#define RFTX_BUF_STATE_START_TXMODULATOR            RFTX_BUF_STATE_WAIT_FILLLEVEL + 0x01U
#define RFTX_BUF_STATE_WAIT_TRANSMISSIONCOMPLETE    RFTX_BUF_STATE_START_TXMODULATOR + 0x01U
#define RFTX_BUF_STATE_SHUTDOWN                     RFTX_BUF_STATE_WAIT_TRANSMISSIONCOMPLETE + 0x01U

// state machine defines for RFTX SW state machine if transparent mode is used
#define RFTX_TRANS_STATE_INIT_SSM                     0x00U
#define RFTX_TRANS_STATE_WAIT_AVCC                    RFTX_TRANS_STATE_INIT_SSM + 0x01U
#define RFTX_TRANS_STATE_INIT_FE                      RFTX_TRANS_STATE_WAIT_AVCC + 0x01U
#define RFTX_TRANS_STATE_WAIT_XTO                     RFTX_TRANS_STATE_INIT_FE + 0x01U
#define RFTX_TRANS_STATE_START_SSM                    RFTX_TRANS_STATE_WAIT_XTO + 0x01U
#define RFTX_TRANS_STATE_WAIT_SSMRDY                  RFTX_TRANS_STATE_START_SSM + 0x01U
#define RFTX_TRANS_STATE_TXMODE                       RFTX_TRANS_STATE_WAIT_SSMRDY + 0x01U
#define RFTX_TRANS_STATE_SHUTDOWN                     RFTX_TRANS_STATE_TXMODE + 0x01U

// state machine defines for RFTX SW state machine if VCO tuning is selected
#define RFTX_VCO_STATE_INIT_SSM                     0x00U
#define RFTX_VCO_STATE_WAIT_AVCC                    RFTX_VCO_STATE_INIT_SSM + 0x01U
#define RFTX_VCO_STATE_INIT_FE                      RFTX_VCO_STATE_WAIT_AVCC + 0x01U
#define RFTX_VCO_STATE_WAIT_XTO                     RFTX_VCO_STATE_INIT_FE + 0x01U
#define RFTX_VCO_STATE_START_SSM                    RFTX_VCO_STATE_WAIT_XTO + 0x01U
#define RFTX_VCO_STATE_WAIT_SSMRDY                  RFTX_VCO_STATE_START_SSM + 0x01U
#define RFTX_VCO_STATE_SHUTDOWN                     RFTX_VCO_STATE_WAIT_SSMRDY + 0x01U

// state machine defines for RFTX SW state machine if ANTENNA tuning is selected
#define RFTX_ANT_STATE_INIT_SSM                     0x00U
#define RFTX_ANT_STATE_WAIT_AVCC                    RFTX_ANT_STATE_INIT_SSM + 0x01U
#define RFTX_ANT_STATE_INIT_FE                      RFTX_ANT_STATE_WAIT_AVCC + 0x01U
#define RFTX_ANT_STATE_WAIT_XTO                     RFTX_ANT_STATE_INIT_FE + 0x01U
#define RFTX_ANT_STATE_START_SSM                    RFTX_ANT_STATE_WAIT_XTO + 0x01U
#define RFTX_ANT_STATE_WAIT_SSMRDY                  RFTX_ANT_STATE_START_SSM + 0x01U
#define RFTX_ANT_STATE_SHUTDOWN                     RFTX_ANT_STATE_WAIT_SSMRDY + 0x01U

// sRfTxConfig
// sRfTxConfig.bFlags
#define RFTXCONFIG_BFLAGS_EOT             BIT_0
#define RFTXCONFIG_BFLAGS_SFIFO_FILLLEVEL BIT_1
#define RFTXCONFIG_BFLAGS_DFIFO_FILLLEVEL BIT_2
#define RFTXCONFIG_BFLAGS_SFIFO_ERROR     BIT_3
#define RFTXCONFIG_BFLAGS_DFIFO_ERROR     BIT_4
#define RFTXCONFIG_BFLAGS_BIT_5           BIT_5
#define RFTXCONFIG_BFLAGS_RDY4TX          BIT_6
#define RFTXCONFIG_BFLAGS_ERROR           BIT_7

#define BM_RFTXCONFIG_BFLAGS_EOT              BITMASK(RFTXCONFIG_BFLAGS_EOT)
#define BM_RFTXCONFIG_BFLAGS_SFIFO_FILLLEVEL  BITMASK(RFTXCONFIG_BFLAGS_SFIFO_FILLLEVEL)
#define BM_RFTXCONFIG_BFLAGS_DFIFO_FILLLEVEL  BITMASK(RFTXCONFIG_BFLAGS_DFIFO_FILLLEVEL)
#define BM_RFTXCONFIG_BFLAGS_SFIFO_ERROR      BITMASK(RFTXCONFIG_BFLAGS_SFIFO_ERROR)
#define BM_RFTXCONFIG_BFLAGS_DFIFO_ERROR      BITMASK(RFTXCONFIG_BFLAGS_DFIFO_ERROR)
#define BM_RFTXCONFIG_BFLAGS_BIT_5            BITMASK(RFTXCONFIG_BFLAGS_BIT_5)
#define BM_RFTXCONFIG_BFLAGS_RDY4TX           BITMASK(RFTXCONFIG_BFLAGS_RDY4TX)
#define BM_RFTXCONFIG_BFLAGS_ERROR            BITMASK(RFTXCONFIG_BFLAGS_ERROR)

// sRfTxConfig.bTuneFlags
#define RFTXCONFIG_BTUNEFLAGS_VCO_TUNE_RDY    BIT_6
#define RFTXCONFIG_BTUNEFLAGS_ANT_TUNE_RDY    BIT_7

#define BM_RFTXCONFIG_BTUNEFLAGS_VCO_TUNE_RDY     BITMASK(RFTXCONFIG_BTUNEFLAGS_VCO_TUNE_RDY)
#define BM_RFTXCONFIG_BTUNEFLAGS_ANT_TUNE_RDY     BITMASK(RFTXCONFIG_BTUNEFLAGS_ANT_TUNE_RDY)


// sRfTxConfig.bStatus
#define RFTXCONFIG_BSTATUS_SWSTATE0              BIT_0
#define RFTXCONFIG_BSTATUS_SWSTATE1              BIT_1
#define RFTXCONFIG_BSTATUS_SWSTATE2              BIT_2
#define RFTXCONFIG_BSTATUS_SWSTATE3              BIT_3
#define RFTXCONFIG_BSTATUS_ACTIVE                BIT_4
#define RFTXCONFIG_BSTATUS_TRANSMISSION_COMPLETE BIT_5
#define RFTXCONFIG_BSTATUS_SSMREADY              BIT_6
#define RFTXCONFIG_BSTATUS_DIRECT_SWITCH         BIT_7

#define BM_RFTXCONFIG_BSTATUS_SWSTATE0   BITMASK(RFTXCONFIG_BSTATUS_SWSTATE0)
#define BM_RFTXCONFIG_BSTATUS_SWSTATE1   BITMASK(RFTXCONFIG_BSTATUS_SWSTATE1)
#define BM_RFTXCONFIG_BSTATUS_SWSTATE2   BITMASK(RFTXCONFIG_BSTATUS_SWSTATE2)
#define BM_RFTXCONFIG_BSTATUS_SWSTATE3   BITMASK(RFTXCONFIG_BSTATUS_SWSTATE3)

#define BM_RFTXCONFIG_BSTATUS_SWSTATE   ( BM_RFTXCONFIG_BSTATUS_SWSTATE0 | BM_RFTXCONFIG_BSTATUS_SWSTATE1 | BM_RFTXCONFIG_BSTATUS_SWSTATE2 | BM_RFTXCONFIG_BSTATUS_SWSTATE3 )

#define BM_RFTXCONFIG_BSTATUS_ACTIVE                 BITMASK(RFTXCONFIG_BSTATUS_ACTIVE)
#define BM_RFTXCONFIG_BSTATUS_TRANSMISSION_COMPLETE  BITMASK(RFTXCONFIG_BSTATUS_TRANSMISSION_COMPLETE)
#define BM_RFTXCONFIG_BSTATUS_SSMREADY               BITMASK(RFTXCONFIG_BSTATUS_SSMREADY)
#define BM_RFTXCONFIG_BSTATUS_DIRECT_SWITCH          BITMASK(RFTXCONFIG_BSTATUS_DIRECT_SWITCH)
//#define BM_RFTXCONFIG_BSTATUS_CurFinish BITMASK(3)

// sRfTxConfig.bConfig
#define RFTXCONFIG_BCONFIG_CHANNEL0          BIT_0
#define RFTXCONFIG_BCONFIG_CHANNEL1          BIT_1
#define RFTXCONFIG_BCONFIG_STAY_TX           BIT_2
#define RFTXCONFIG_BCONFIG_SVC_LOCATION      BIT_3
#define RFTXCONFIG_BCONFIG_SHUTDOWN_MODE     BIT_4
#define RFTXCONFIG_BCONFIG_TRANSPARENT_MODE  BIT_5
#define RFTXCONFIG_BCONFIG_VCO_TUNING        BIT_6
#define RFTXCONFIG_BCONFIG_ANT_TUNING        BIT_7

#define BM_RFTXCONFIG_BCONFIG_CHANNEL0          BITMASK(RFTXCONFIG_BCONFIG_CHANNEL0)
#define BM_RFTXCONFIG_BCONFIG_CHANNEL1          BITMASK(RFTXCONFIG_BCONFIG_CHANNEL1)

#define BM_RFTXCONFIG_BCONFIG_CHANNEL           ( BM_RFTXCONFIG_BCONFIG_CHANNEL0 | BM_RFTXCONFIG_BCONFIG_CHANNEL1 )

#define BM_RFTXCONFIG_BCONFIG_STAY_TX           BITMASK(RFTXCONFIG_BCONFIG_STAY_TX)
#define BM_RFTXCONFIG_BCONFIG_SVC_LOCATION      BITMASK(RFTXCONFIG_BCONFIG_SVC_LOCATION)
#define BM_RFTXCONFIG_BCONFIG_SHUTDOWN_MODE     BITMASK(RFTXCONFIG_BCONFIG_SHUTDOWN_MODE)
#define BM_RFTXCONFIG_BCONFIG_TRANSPARENT_MODE  BITMASK(RFTXCONFIG_BCONFIG_TRANSPARENT_MODE)
#define BM_RFTXCONFIG_BCONFIG_VCO_TUNING        BITMASK(RFTXCONFIG_BCONFIG_VCO_TUNING)
#define BM_RFTXCONFIG_BCONFIG_ANT_TUNING        BITMASK(RFTXCONFIG_BCONFIG_ANT_TUNING)

#define BM_KeyRF_MODE  BITMASK(1)
/* ------------------------------------------------------------------------- */
/* sRfTxServicePathConfig.bTxSetPath[0]                                       */
/* sRfTxServicePathConfig.bTxSetPath[1]                                       */
#define BM_RFTXSERVICE_BTXSETPATH_GAUS               BIT_MASK_7
#define BM_RFTXSERVICE_BTXSETPATH_PREE               BIT_MASK_6

#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL   (  BIT_MASK_5 | BIT_MASK_4 | BIT_MASK_3 | BIT_MASK_2 | BIT_MASK_1 | BIT_MASK_0 )
#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL5  BIT_MASK_5
#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL4  BIT_MASK_4
#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL3  BIT_MASK_3
#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL2  BIT_MASK_2
#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL1  BIT_MASK_1
#define BM_RFTXSERVICE_BTXSETPATH_STARTTXFILLLEVEL0  BIT_MASK_0

#define RFTXSERVICE_BTXSETPATH_MODULATION                BIT_7
#define RFTXSERVICE_BTXSETPATH_ASK_SHAPING               BIT_6
#define RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL4   BIT_4
#define RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL3   BIT_3
#define RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL2   BIT_2
#define RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL1   BIT_1
#define RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL0   BIT_0

#define BM_RFTXSERVICE_BTXSETPATH_MODULATION                 BIT_MASK_7
#define BM_RFTXSERVICE_BTXSETPATH_ASK_SHAPING                BIT_MASK_6
#define BM_RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL     ( BIT_MASK_4 | BIT_MASK_3 | BIT_MASK_2 | BIT_MASK_1 | BIT_MASK_0 )
#define BM_RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL4    BIT_MASK_4
#define BM_RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL3    BIT_MASK_3
#define BM_RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL2    BIT_MASK_2
#define BM_RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL1    BIT_MASK_1
#define BM_RFTXSERVICE_BTXSETPATH_STARTPREAMBLEFILLLEVEL0    BIT_MASK_0


/*===========================================================================*/
/*  DEFINES                                                                  */
/*===========================================================================*/
/* SSM state definitions */

/** \brief <b>SSM_END_STATE</b> is used as define for end state */
#define SSM_END_STATE               0U

/** \brief <b>SSM_PLL_EN_STATE</b> is used as define for PLL enable sub state machine */
#define SSM_PLL_EN_STATE            (SSM_END_STATE           + 1U)

/** \brief <b>SSM_PLL_LOCK_STATE</b> is used as define for PLL lock sub state machine */
#define SSM_PLL_LOCK_STATE          (SSM_PLL_EN_STATE        + 1U)

/** \brief <b>SSM_TX_DSP_EN_STATE</b> is used as define for TX DSP enable sub state machine */
#define SSM_TX_DSP_EN_STATE         (SSM_PLL_LOCK_STATE    + 1U)

/** \brief <b>SSM_TX_DSP_DIS_STATE</b> is used as define for TX DSP disable sub state machine */
#define SSM_TX_DSP_DIS_STATE        (SSM_TX_DSP_EN_STATE     + 1U)

/** \brief <b>SSM_SEND_TELEGRAM_STATE</b> is used as define for send telegram sub state machine */
#define SSM_SEND_TELEGRAM_STATE     (SSM_TX_DSP_DIS_STATE  + 1U)

/** \brief <b>SSM_SHUTDOWN_STATE</b> is used as define for shut down sub state machine */
#define SSM_SHUTDOWN_STATE          (SSM_SEND_TELEGRAM_STATE + 1U)

/** \brief <b>SSM_VCO_TUNING_STATE</b> is used as define for vco tuning sub state machine */
#define SSM_VCO_TUNING_STATE        (SSM_SHUTDOWN_STATE      + 1U)

/** \brief <b>SSM_ANTENNA_TUNING_STATE</b> is used as define for antenna tuning sub state machine */
#define SSM_ANTENNA_TUNING_STATE    (SSM_VCO_TUNING_STATE    + 1U)

/** \brief <b>BM_DFFLS</b> is used as data fifo fill level bit mask */
#define BM_DFFLS					(_BM(DFFLS0)|_BM(DFFLS1)|_BM(DFFLS2)|_BM(DFFLS3)|_BM(DFFLS4)|_BM(DFFLS5))

/** \brief <b>BM_SFFLS</b> is used as support fifo fill level bit mask */
#define BM_SFFLS					(_BM(SFFLS0) |_BM(SFFLS1) |_BM(SFFLS2) |_BM(SFFLS3) |_BM(SFFLS4) )


typedef struct {
    /** \brief <b>bFlags</b>
        contains the RF Tx module external flags
    
        \li Bit 7: RF Tx module error flag (0: Not Set, 1: Set)
        \li Bit 6: RF Tx module ready for transmission (0: Not Ready, 1: Ready)
        \li Bit 5: rfu
        \li Bit 4: DFIFO overflow/underflow error (0: Not Set, 1: Set)
        \li Bit 3: SFIFO overflow/underflow error (0: Not Set, 1: Set)
        \li Bit 2: DFIFO fill level reached (0: Not reached, 1: Reached)
        \li Bit 1: SFIFO fill level reached (0: Not reached, 1: Reached)
        \li Bit 0: End of telegram reached (0: Not reached, 1: Reached)
     */
    uint8_t bFlags;

    /** \brief <b>bTuneFlags</b>
        contains the RF Tx module tuning status 
    
        \li Bit 7: Antenna Tuning ready flag (0: No, 1: Yes)
        \li Bit 6: VCO Tuning ready flag (0: No, 1: Yes)
        \li Bit 5..0: rfu
     */
    uint8_t bTuneFlags;
    
     /** \brief <b>bStatus</b>
        contains the RF Tx module status
    
        \li Bit 7: Direct Switching (0: No, 1: Yes)
        \li Bit 6: RF Tx SSM ready flag (0: SSM not ready, 1: SSM ready)
        \li Bit 5: RF Tx module transmission complete (0: No, 1: Yes)
        \li Bit 4: RF Tx module active flag (0: Running, 1: finished/not started)
        \li Bit 3: 当前数据发送完成，仍然留在发送状态，返回主程序，准备其他数据
        \li Bit 2..0: rfu
     */
    uint8_t bStatus;

    /** \brief <b>bConfig</b>
        contains the RF Tx module configuration
    
        \li Bit 7: Antenna Tuning (0: No, 1: Yes)
        \li Bit 6: VCO Tuning (0: No, 1: Yes)
        \li Bit 5: TX Mode (0: Buffered, 1: Transparent)
        \li Bit 4: ShutDown TXMode (0: IDLEMode(RC), 1: IDLEMode(XTO))
        \li Bit 3: Service location (0: SRAM, 1: EEPROM)
        \li Bit 2: stay in TX after transmission complete (0: No, 1: Yes)
        \li Bit 1: 0:数据10k,   1:按键射频,1k
     */
    uint8_t bConfig;

    /** \brief <b>pAddress</b>
        contains the address of the service configuration (SRAM or EEPROM)
     */
    uint8_t *pAddress;
    
    /* --P2P-3655-- */
    /** \brief <b>bCmcrSetting</b>
        contains the original cmcr setting
     */
    uint8_t bCmcrSetting;
}sRfTxConfig;

typedef struct{
    /** \brief <b>bSSMFBR</b>
        contains the SSMFBR register setting
     */
    uint8_t bSSMFBR;

    /** \brief <b>bFEALR_FEANT</b>
        this variable contains a combination of FEANT and FEALR register
        \li Bit 7:    rfu
        \li Bit 6:    rfu
        \li Bit 5:    FEALR.RNGE1
        \li Bit 4:    FEALR.RNGE0
        \li Bit 3:    FEANT.LVLC3
        \li Bit 2:    FEANT.LVLC2
        \li Bit 1:    FEANT.LVLC1
        \li Bit 0:    FEANT.LVLC0
     */
    uint8_t bFEALR_FEANT;
    /** \brief <b>bFEAT</b>
        Frontend Amplifier Bias and Antenna Tuning Register settings
     */
    uint8_t bFEAT;
    /** \brief <b>bFEPAC</b>
        Frontend Power Amplifier Register settings
     */
    uint8_t bFEPAC;
    /** \brief <b>bFEVCO</b>
        Frontend VCO and PLL Control Register settings
     */
    uint8_t bFEVCO;
    /** \brief <b>bFEVCT</b>
        Frontend VCO Tuning Register settings
     */
    uint8_t bFEVCT;
    /** \brief <b>bIF</b>
        IF setting (HighByte and LowByte) will be used for SW calculations
     */
    uint8_t bIF[2];
}sRfTxServiceSpecificConfig;
typedef struct {
    /** \brief <b>btxDev</b>
        frequency deviation settings (HighByte and LowByte), will be used for SW calculations
     */
    uint8_t btxDev[2];
     /** \brief <b>bGACDIV</b>
        Gauss Clock Divider Register settings
     */
    uint8_t bGACDIV[2];
    /** \brief <b>bFSFCR</b>
        Tx DSP Filter Control Register settings
     */
    uint8_t bFSFCR;
    /** \brief <b>bTXDR</b>
        TX Data Rate setting (HighByte and LowByte) will be used for SW
        calculations
     */
    uint8_t bTXDR[2];
    /** \brief <b>txSetPath</b>
        is used for TXMode configuration

        <b>bTxSetPath[0]</b>
        \li Bit 7: GAUS
        \li Bit 6: PREE
        \li Bit 5..Bit 0: StartTxDataFillLevel<br>

        <b>bTxSetPath[1]</b>
        \li Bit 7: TX modulation (0=FSK/1=ASK)
        \li Bit 6: ASK shaping enable
        \li Bit 5: rfu
        \li Bit 4..0: StartPreambleFillLevel
     */
    uint8_t bTxSetPath[2];
    /** \brief <b>bTxSysEvent</b>
        is used for event configuration in TXMode for transmission
        \li Bit 7: rfu
        \li Bit 6: rfu
        \li Bit 5..Bit 0: TxBufFillLevel
     */
    uint8_t bTxSysEvent;
    /** \brief <b>bTxPreambleSysEvent</b>
        is used preamble buffer event configuration in TXMode for transmission
        \li Bit 7: rfu
        \li Bit 6: rfu
        \li Bit 5: rfu
        \li Bit 4..Bit 0: PreambleBufFillLevel
     */
    uint8_t bTxPreambleSysEvent;
    /** \brief <b>bTMCR2</b>
        Tx Modulator Control Register 2
     */
    uint8_t bTMCR2;
    /** \brief <b>bTMSSC</b>
        Tx Modulator Stop Sequence Configuration
     */
    uint8_t bTMSSC;
    /** \brief <b>bTMTL</b>
        Tx Modulator Telegram Length
     */
    uint8_t bTMTL[2];
    /** \brief <b>bTMCP</b>
        Tx Modulator CRC Polynomial
     */
    uint8_t bTMCP[2];
    /** \brief <b>bTMCI</b>
        Tx Modulator CRC Init value
     */
    uint8_t bTMCI[2];
    /** \brief <b>bTMCSB</b>
        Tx Modulator CRC Skip Bit number
     */
    uint8_t bTMCSB;
}sRfTxServicePathConfig;
typedef struct{
    /** \brief  <b>bFFREQ</b>
        3 bytes FFRQ base setting
     */
    uint8_t bFFREQ[3];
    /** \brief  <b>bFEMS</b>
        Frontend Mode and Swallow Register setting
        \li Bit 7..4: PLL Mode
        \li Bit 3..0: PLL Swallow
     */
    uint8_t bFEMS;
    /** \brief <b>bFECR</b>
        Frontend Control Register settings for temperature measurement channel
        \li  Bit 7..6:  rfu
        \li  Bit 5:     ANPS
        \li  Bit 4:     PLCKG
        \li  Bit 3:     ADHS
        \li  Bit 2:     ANDP
        \li  Bit 1:     S4N3
        \li  Bit 0:     LBNHB
     */
    uint8_t bFECR;
}sRfTxChannelConfig;
typedef struct
{
	sRfTxServiceSpecificConfig  sService;
	sRfTxServicePathConfig      sPath;
	sRfTxChannelConfig          sChannel;
}sRfTxCurrentServiceChannelConfiguration;
#endif /* LFRX_H */
