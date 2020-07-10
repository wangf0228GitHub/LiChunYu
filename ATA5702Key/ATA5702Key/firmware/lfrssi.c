#include "lfrssi.h"
volatile sLfRssi g_sLfRssi;
sLfRssiRegConfig g_sLfRssiRegConfig_flash;

/*===========================================================================*/
/*  IMPLEMENTATION                                                           */
/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_lfRssiInit_C</b>
    initializes the LF RSSI component data with default values.

    Variable Usage;:
    \li [in,out] ::g_sLfRssi    Global LF RSSI component data

    \image html ATA_lfRssiInit_C.png

    \internal
    \li 010: Initialize component variables with default values.

    \Derived{Yes}

    \Rationale{N/A}

    \Traceability{N/A}
    \endinternal
\n
*/
/*---------------------------------------------------------------------------*/
void ATA_lfRssiInit_C(void)
{
    /* LLR-Ref: 010 */
    g_sLfRssi.bFlags  = LFRSSI_FLAGS_RESET;
    g_sLfRssi.bStatus = LFRSSI_STATUS_RESET;

    g_sLfRssi.bOutOfRangeMask = 0U;
}


/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiOpen_C</b>
    is used to power-up the LF RSSI module, LF transponder and LF receiver.
    It disables the digital part of the LF receiver and selects the LF RSSI
    measurement mode.
    Note: LF channels for LF RSSI measurement has to be already enabled.

    Variable Usage:
    \li [in,out] ::g_sLfRssi    Global LF RSSI component data

    \image html ATA_lfRssiOpen_C.png

    \internal
    \li 010:  Disable interrupts globally due to changes on PRR1, transponder
              and LF receiver settings.

    \li 020:  Save PRR1 register setting due to configuration changes when using
              the LF RSSI module. Content will be restored when leaving the module.
              No need to save PRR0 register affected bit only used within module.

    \li 030:  Enable power to the RSSI module and LF receiver (the analog
              RSSI block is also powered on). Enable power to the LF transponder
              to access TPCR2 register.

    \li 040:  Save TPCR2 register setting before disabling the transponder mode to
              prevent system reset in case a present LF field will be turn off.

    \li 050:  Save LFCR1 register setting and disable LF receiver for mode switching.\n
              Note: Between disable and enable the LF receiver a waiting time of
                    at least 3 SRC clock cycles is required.

    \li 060:  Save SRCCAL register setting.
              Note: Reading SRCCAL register possible without enabling the clock (PRLFPH).

    \li 070:  Enable interrupts globally.

    \li 080:  Reset component variables except error flag.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-2000}
    \endinternal
\n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiOpen_C(void)
{
    /* LLR-Ref: 010 */
    _CLI;

    /* LLR-Ref: 020 */
    g_sLfRssi.bPrr1 = PRR1;

    /* LLR-Ref: 030 */
    /* no power reduction bits to enable the digital LF RSSI register block, clocked by cp2io
       Note: It is assumed that LF receiver has been already enabled. But if not, it is enabled here. */
    PRR0 &= ~_BM(PRLFRS);
    PRR1 &= ~(_BM(PRLFTP) | _BM(PRLFR));

    /* LLR-Ref: 040 */
    g_sLfRssi.bTpcr2 = TPCR2;

    /* LLR-Ref: 050 */
    g_sLfRssi.bLfcr1 = LFCR1;
    LFCR1 |= _BM(LFFM1);
    LFCR1 &= ~_BM(LFRE);

    /* LLR-Ref: 060 */
    g_sLfRssi.bSrcCal = SRCCAL;

    /* LLR-Ref: 070 */
    _SEI;

    /* LLR-Ref: 080 */
    g_sLfRssi.bFlags &= LFRSSI_FLAGS_BM_ERROR_FLAG;
    g_sLfRssi.bStatus = LFRSSI_STATUS_RESET;
}


/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiClose_C</b>
    is used to power-down the LF RSSI module and the LF receiver including the
    analog RSSI blocks. It also restores temporarily saved register settings.

    Variable Usage:
    \li [in,out] ::g_sLfRssi    Global LF RSSI component data

    \image html ATA_lfRssiClose_C.png

    \internal
    \li 010:  Disable interrupts globally to restore register settings.

    \li 010:  Enable power to the LF protocol handler to restore SRCCAL value.

    \li 030:  Restore saved register settings.
              Note: The LF receiver is disabled when leaving LF RSSI module.

    \li 040:  Disable power to the LF RSSI module.

    \li 050:  Enable interrupts globally.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-2001}
    \endinternal
\n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiClose_C(void)
{
    /* LLR-Ref: 010 */
    _CLI;

    /* LLR-Ref: 020 */
    ATA_POWERON_C(PRR1, PRLFPH);

    /* LLR-Ref: 030 */
    SRCCAL = g_sLfRssi.bSrcCal;
    LFCR1  = g_sLfRssi.bLfcr1 & ~(_BM(LFRE) | _BM(LFFM1));
    TPCR2  = g_sLfRssi.bTpcr2 & ~_BM(TPD);

    /* no power reduction bits to disable the digital RSSI register block, clocked by cp2io
       disabling power of the LF receiver disables power of the analog RSSI block */
    PRR1  = g_sLfRssi.bPrr1;

    /* LLR-Ref: 040 */
    PRR0 |= _BM(PRLFRS);

    /* LLR-Ref: 050 */
    _SEI;
}

/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiSetEepromConfig_C</b>
    is used to initialize LF RSSI registers with associated EEPROM configuration.

    \param[in]      bEepRssiSrcCalVal   Contains the SRC calibration setting used during LF RSSI measurement.

    Variable Usage:
    \li [out] ::g_sLfRssi   Global LF RSSI component data
    \li [out] ::g_sDebug    Global Debug component data

    \image html ATA_lfRssiSetEepromConfig_C.png

    \internal
    \li 010:  Enable power to the RSSI module before accessing the registers.

    \li 020:  Wait until any previous EEPROM access has been finished.

    \li 030:  Reset the digital LF RSSI block.

    \li 040:  Setup base address to read from EEPROM first.

    \li 050:  Enable EEPROM burst read mode.

    \li 060:  Signalize writing of the LF RSSI calibration configuration.

    \li 070:  Copy settings from EEPROM: 10 bytes for range correction,
              1 byte pre-divider switch and 1 byte nameless.

    \li 080:  Configure LF RSSI with the passed SRC calibration value.

    \li 090:  Clear bit to signalize end of LF RSSI register calibration
              sequence.

    \li 100:  ON occurrence of uncorrectable errors during EEPROM reads,
              THEN

    \li 110:    Set component global error flag and the global debug error
                variable with error code for EEPROM read error.
              ENDIF

    \li 120:  ON occurrence of access violation errors during EEPROM reads,
              THEN

    \li 130:    Set component global error flag and the global debug error
                variable with error code for EEPROM access error.
              ENDIF

    \li 140:  Clear Burst Read Mode flag and implicitly clear both EEPROM
              error flags.

    \li 150:  Disable power to the LF RSSI module within protected section.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-2002,Primus2P-2478}
    \endinternal
\n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiSetEepromConfig_C(uint8_t bEepRssiSrcCalVal)  // registers in use: R16 registers unused: R17, R18, R19, R20, R21, R22, R23
{
    uint8_t i;
    uint8_t prr0;
    /* LLR-Ref: 010 */
    _CLI;
    prr0 = PRR0;
    PRR0 &= ~_BM(PRLFRS);
    _SEI;

    /* LLR-Ref: 020 */
    while(EECR & _BM(NVMBSY)){}

    /* LLR-Ref: 030 */
    RSCR |= _BM(RSRES);

    /* LLR-Ref: 040 */
    /*lint -e923 GeWi (23nov2011)*/
    /* disable lint error 923 - Cast between pointer type and an integral type.
     * Misra Rule 11.3 says: Casting between a pointer and an integer type should be avoided where possible,
     * but may be unavoidable when addressing memory mapped registers or other hardware specific features.
     */
    EEARH = 0x08;//(uint8_t)(((uint16_t)&g_sAtmelEEPromSection.eepRssiRangeCorrectionValues[0]) >> 8U);
    EEARL = 0xb4;//(uint8_t)(((uint16_t)&g_sAtmelEEPromSection.eepRssiRangeCorrectionValues[0]) >> 0U);
    /*lint -restore */

    /* LLR-Ref: 050 */
    EECR2 |= _BM(EEBRE);

    /* LLR-Ref: 060 */
    //RSMS2R = 0;       // already done by reset
    RSMS1R |= _BM(RSSCAL);

    /* LLR-Ref: 070 */
    for( i = 1; i < 12; i++ )
    {
        RSCALIB = EEDR;
        RSMS2R = i;
        while( !(RSSR & _BM(RSSVLD)) ){}
    }

    /* LLR-Ref: 080 */
    RSCALIB = bEepRssiSrcCalVal;
    RSMS2R  = 0x0C;
    while( !(RSSR & _BM(RSSVLD)) ){}

    /* LLR-Ref: 090 */
    RSMS1R &= ~_BM(RSSCAL);

    /* LLR-Ref: 100 */
    if( EECR2 & _BM(E2FF) )
    {
        /* LLR-Ref: 110 */
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_EEPROM_READ_ERROR;
        EECR2 |= _BM(E2FF);
    }

    /* LLR-Ref: 120 */
    if(EECR2 & _BM(E2AVF))
    {
        /* LLR-Ref: 130 */
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_EEPROM_ACCESS_ERROR;
        EECR2 |= _BM(E2AVF);
    }

    /* LLR-Ref: 140 */
    EECR2 &= ~_BM(EEBRE);

    /* LLR-Ref: 150 */
    _CLI;
    PRR0 = prr0;
    _SEI;
}

/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiMeasStart_C</b>
    is used to start the external LF RSSI measurement.

    \param[in]      pRegConfig  Contains the configuration for the registers RSCR, RSMS1R, RSMS2R, RSDLYR and RSSRCR
    \param[in]      bMode       Contains internal or external LF RSSI measurement request
    \param[in]      bSign       Contains sign detection request, only applicable in conjunction with external LF RSSI measurement

    Variable Usage:
    \li [in,out] ::g_sLfRssi    Global LF RSSI component data
    \li [out]    ::g_sDebug     Global Debug component data

    \image html ATA_lfRssiMeasStart_C.png

    \internal
    \li 010:  Reset component flags except the error flag. All previous information
              are cleared each time a new LF RSSI measurement is started.

    \li 020:  IF an LF RSSI module operation is still in progress,
              THEN

    \li 030:    Set component global error flag and the global debug error
                variable with error code for operation still active error.
              ELSEIF

    \li 040:  IF the analog LF RSSI part has not finished its initialization,
              THEN

    \li 050:    Set component global error flag and the global debug error
                variable with error code for RSSI analog part not ready error.
              ELSEIF

    \li 060:  IF no LF channel is activated to participate the measurement,
              THEN

    \li 070:    Set component global error flag and the global debug error
                variable with error code for invalid measurement channels error.
              ELSE

    \li 080:    Clear bits RSSCAL and RSINTM of input parameter used to configure
                register RSMS1R.

    \li 090:    IF an internal LF RSSI measurement shall be started,
                THEN

    \li 100:      Set bit RSINTM in register RSMS1R and signalize internal
                  LF RSSI measurement by setting internal status flag.
                ENDIF

    \li 110:    Clear bit RSOFM of input parameter used to configure
                register RSCR.

    \li 120:    IF an external LF RSSI measurement with sign detection
                shall be started,
                THEN

    \li 130:      Enable sign detection by setting bit SDEN in register RSCR and
                  enable all LF channels to participate the LF RSSI measurement
                  regardless RSCHxE bit settings of input parameter.

    \li 140:      Signalize sign detection LF RSSI measurement by setting internal
                  status flag.
                ENDIF

    \li 150:    IF there is a mismatch in the LF receiver channels participating
                the LF RSSI measurement and its enable configuration,
                THEN

    \li 160:      Signalize the mismatch by setting component global warning flag.
                ENDIF

    \li 170:    Configure RSMS2R, RSDLYR and RSSRCR register with input parameters.

    \li 180:    Clear LF RSSI operation finished flag (RSOFF) in register RSFR.

    \li 190:    Reset the transponder watchdog timer.

    \li 200:    Disable interrupts globally before start the measurement.

    \li 210:    Signalize LF RSSI operation is in progress.

    \li 220:    Trigger start of LF RSSI measurement by setting bit RSOS in
                register RSCR.

    \li 230:    Enable interrupts globally.
              ENDIF

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-2004}
    \endinternal
\n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiMeasStart_C(void *pRegConfig, uint8_t bMode, uint8_t bSign)  // registers in use: R17:R16, R18, R19 registers unused:  R20, R21, R22, R23
{
    uint8_t tmpReg;
    sLfRssiRegConfig *psLfRssiRegConfig = (sLfRssiRegConfig*)pRegConfig;

    /* LLR-Ref: 010 */
    g_sLfRssi.bFlags &= LFRSSI_FLAGS_BM_ERROR_FLAG;
    g_sLfRssi.bStatus = LFRSSI_STATUS_RESET;

    tmpReg = psLfRssiRegConfig->bRsms1r & 0x07;     // mask RSSI channel enable bits

    /* LLR-Ref: 020 */
    if( RSCR & _BM(RSOS) )
    {
        /* LLR-Ref: 030 */
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_OPERATION_ACTIVE;
    }
    /* LLR-Ref: 040 */
    else if( !(RSSR & _BM(RSRDY)) )
    {
        /* LLR-Ref: 050 */
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_ANALOGPART_NOTREADY;
    }
    /* LLR-Ref: 060 */
    else if( !tmpReg )
    {
        /* LLR-Ref: 070 */
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_INVALID_MEASUREMENT_CHANNELS;
    }
    else
    {
        /* LLR-Ref: 080 */
        RSMS1R = psLfRssiRegConfig->bRsms1r & ~(_BM(RSSCAL) | _BM(RSINTM));

        /* LLR-Ref: 090 */
        if( bMode )
        {
            /* LLR-Ref: 100 */
            RSMS1R            |= _BM(RSINTM);
            g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_INTERNAL_MEASUREMENT_FLAG;
        }

        /* LLR-Ref: 110 */
        RSCR = psLfRssiRegConfig->bRscr & _BM(RSOFM);

        /* LLR-Ref: 120 */
        if( !bMode && bSign )
        {
            /* LLR-Ref: 130 */
            RSCR   |= _BM(RSSDEN);
            RSMS1R |= (_BM(RSCH3E) | _BM(RSCH2E) | _BM(RSCH1E));

            /* LLR-Ref: 140 */
            g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_SIGNDETECT_ENA_FLAG;
            tmpReg = 0x07;
        }

        /* LLR-Ref: 150 */
        if( (LFCR0 & tmpReg) != tmpReg )    // check desired measurement channels vs. its enable configuration, if mismatching set warning flag
        {
            /* LLR-Ref: 160 */
            g_sLfRssi.bFlags |= LFRSSI_FLAGS_BM_CHANNEL_ENABLE_MISMATCH_FLAG;
        }

        /* LLR-Ref: 170 */
        RSMS2R = psLfRssiRegConfig->bRsms2r;    // check influence of bits RSSADR for measurement
        RSDLYR = psLfRssiRegConfig->bRsdlyr;    // may be load from EEPROM???
        RSSRCR = psLfRssiRegConfig->bRssrcr;    // obsolete for internal measurement

        /* LLR-Ref: 180 */
        RSFR  = _BM(RSOFF);                       // all other flags are read-only

        /* LLR-Ref: 190 */
        TPCR2 |= _BM(TPD);                        // reset transponder watchdog

        /* LLR-Ref: 200 */
        _CLI;

        /* LLR-Ref: 210 */
        g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG;

        /* LLR-Ref: 220 */
        RSCR |= _BM(RSOS);                        // start measurement

        /* LLR-Ref: 230 */
        _SEI;
    }
}


/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiMeasStop_C</b>
    is used to signalize to end an active LF RSSI operation (mainly internal or
    external measurement). In case no operation is in progress, no unintended
    behaviour occurs. Measurement end request flag is cleared without side
    effects.

    Variable Usage:
    \li [in,out] ::g_sLfRssi    Global LF RSSI component data

    \image html ATA_lfRssiMeasStop_C.png

    \internal
    \li 010:  Disable interrupts globally to prevent .

    \li 020:  IF an LF RSSI operation is still in progress,
              THEN

    \li 030:    Set bit to finish the current operation and set internal flag
                signalizing that operation has been aborted.
              ENDIF

    \li 040:  Enable interrupts globally.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{N/A}
    \endinternal
    \n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiMeasStop_C(void)
{
    /* LLR-Ref: 010 */
    _CLI;

    /* LLR-Ref: 020 */
    if( RSCR & _BM(RSOS) )
    {
        /* LLR-Ref: 030 */
        RSCR |= _BM(RSEOR);
        g_sLfRssi.bFlags |= LFRSSI_FLAGS_BM_MEASUREMENT_ABORTED_FLAG;
    }

    /* LLR-Ref: 040 */
    _SEI;
}


/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiGetAverageResult_C</b>
    is used to read out the LF RSSI measurement average result for each LF channel
    and to store it to SRAM location. Furthermore, the results of a sign detection
    is also read and stored to SRAM location.

    \param[out]     pResMeas        Base address to data structure to store 3 x 16 bit LF RSSI averaging results
    \param[out]     pResSignDetect  Base address to data structure to store 4 x 8 bit LF RSSI sign detection results

    Variable Usage:
    \li [in,out] ::g_sLfRssi    Global LF RSSI component data
    \li [out]    ::g_sDebug     Global Debug component data

    \image html ATA_lfRssiGetAverageResult_C.png

    \internal
    \li 010:  Signalize LF RSSI operation is in progress.

    \li 020:  IF valid pointer to store the averaging results is used,
              THEN

    \li 030:    Clear bit RSSSV in register RSMS1R to be sure to read the LF channel
                average results from result registers RSRES1..3.

    \li 040:    Wait as long as the result registers are not containing the
                average results.

    \li 050:    Read the LF channel average result registers and store content
                to SRAM location.
              ENDIF

    \li 060:  IF valid pointer to store the sign detection results is used,
              THEN

    \li 070:    Read sign detection result registers and store content to
                SRAM location.
              ENDIF

    \li 080:  Signalize LF RSSI operation has been finished/is not in progress.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-2503}
    \endinternal
\n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiGetAverageResult_C(uint8_t *pResMeas, uint8_t *pResSignDetect)  // registers in use: R17:R16, R19:R18 registers unused: R20, R21, R22, R23
{
     /* LLR-Ref: 020 */
    if( pResMeas )
    {
        /* LLR-Ref: 030 */
        RSMS1R &= ~(_BM(RSSCAL) | _BM(RSSSV));

        /* LLR-Ref: 040 */
        // in case RSSVLD never occurs, transponder watchdog expires and should do a software reset
        while( !(RSSR & _BM(RSSVLD)) ){}

        /* LLR-Ref: 050 */
        *(pResMeas+LFRSSI_BYTEOFFSET_RESULT_RSC1L) = RSRES1L;
        *(pResMeas+LFRSSI_BYTEOFFSET_RESULT_RSC1H) = RSRES1H;
        *(pResMeas+LFRSSI_BYTEOFFSET_RESULT_RSC2L) = RSRES2L;
        *(pResMeas+LFRSSI_BYTEOFFSET_RESULT_RSC2H) = RSRES2H;
        *(pResMeas+LFRSSI_BYTEOFFSET_RESULT_RSC3L) = RSRES3L;
        *(pResMeas+LFRSSI_BYTEOFFSET_RESULT_RSC3H) = RSRES3H;
    }

    /* LLR-Ref: 060 */
    if( pResSignDetect )
    {
        /* LLR-Ref: 070 */
        // content of registers only valid, if sign detection has been enabled
        // for external measurement
        *(pResSignDetect+LFRSSI_BYTEOFFSET_RESULT_SD12RR) = SD12RR;
        *(pResSignDetect+LFRSSI_BYTEOFFSET_RESULT_SD13RR) = SD13RR;
        *(pResSignDetect+LFRSSI_BYTEOFFSET_RESULT_SD23RR) = SD23RR;
        *(pResSignDetect+LFRSSI_BYTEOFFSET_RESULT_SD360R) = SD360R;
    }

    /* LLR-Ref: 080 */
    g_sLfRssi.bStatus &= ~LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG;
}


/*----------------------------------------------------------------------------- */
/** \brief <b>ATA_lfRssiGetSamplesResult_C</b>
    is used to read out a certain number of LF RSSI sample values for
    each LF channel and to store it to SRAM location.

    \param[out]     pResMeas    Base address to data structure to store 3 x 16 bit LF RSSI sample value(s)
    \param[in]      bNum        Number of LF RSSI sample values to read
    \param[in]      bIndex      Index to read the LF RSSI sample value(s) firstly

    Variable Usage:
    \li [in,out]  ::g_sLfRssi   Global LF RSSI component data
    \li [out]     ::g_sDebug    Global Debug component data

    \image html ATA_lfRssiGetSamplesResult_C.png

    \internal
    \li 010:  IF valid pointer to store the sample value(s) is used,
              THEN

    \li 020:    Set bit RSSSV in register RSMS1R to be sure to read
                the LF channel sample results from result registers RSRES1..3.

    \li 030:    IF the desired number of samples to read exceeds the maximum
                number without index wrap around,
                THEN

    \li 040:      Limit number to 16 samples to read.
                ENDIF

    \li 050:    Signalize LF RSSI operation is in progress.

    \li 060:    IF the number of samples to read is not reached,
                THEN

    \li 070:      Set read index to register RSMS2R.

    \li 080:      Wait as long as the result registers are not containing the
                  sample values.

    \li 090:      Read the LF channel result registers and store sample value
                  to SRAM location.
                ENDIF

    \li 100:    Signalize LF RSSI operation has been finished/is not in progress.
              ELSE

    \li 110:    Set component global error flag and the global debug error
                variable with error code for invalid output pointers are used error.
              ENDIF

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-2503}
    \endinternal
\n
*/
/*----------------------------------------------------------------------------- */
void ATA_lfRssiGetSamplesResult_C(uint8_t *pResMeas, uint8_t bNum, uint8_t bIndex)  // registers in use: R17:R16, R18, R19 registers unused: R20, R21, R22, R23
{
    /* LLR-Ref: 010 */
    if( pResMeas )
    {
        /* LLR-Ref: 020 */
        RSMS1R &= ~_BM(RSSCAL);
        RSMS1R |= _BM(RSSSV);

        /* LLR-Ref: 030 */
        if( bNum > 16 )
        {
            /* LLR-Ref: 040 */
            bNum = 16;
        }

        /* LLR-Ref: 050 */
        g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG;

        /* LLR-Ref: 060 */
        for( ; bNum > 0; bNum--, bIndex++ )
        {
            /* LLR-Ref: 070 */
            RSMS2R = bIndex & 0x0F; // possibility to apply OR operation to preserve bits RSAVGS[3..0],
                                    // RSAVGS[3..0] are overwritten when start a new measurement

            /* LLR-Ref: 080 */
            // in case RSSVLD never occurs, transponder watchdog expires and should do a software reset
            while( !(RSSR & _BM(RSSVLD)) ){}

            /* LLR-Ref: 090 */
            *(pResMeas++) = RSRES1L;
            *(pResMeas++) = RSRES1H;
            *(pResMeas++) = RSRES2L;
            *(pResMeas++) = RSRES2H;
            *(pResMeas++) = RSRES3L;
            *(pResMeas++) = RSRES3H;
        }

        /* LLR-Ref: 100 */
        g_sLfRssi.bStatus &= ~LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG;
    }
    else
    {
        /* LLR-Ref: 110 */
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_NULL_POINTER_PASSED;
    }

}
/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_lfRssiMeasReady_ISR_C</b>
    interrupt function is used to store the measured LF RSSI values from
    LF RSSI data registers to internal array and signals availability of data
    and signals also the end of current measurement. Furthermore it masks the
    participated measurement channels with its associated timeout flags. In
    case channel timeout(s) is(are) detected, the error flag inside the status
    variable is set. The associated channel timeout mask is stored to
    component variable.

    Variable Usage:
    \li [in,out] ::g_sLfRssi Global LF RSSI component data

    \image html ATA_lfRssiMeasReady_ISR_C.png

    \internal
    \li 010:  Disable occurrences of further LF RSSI operation finished interrupts
              by clearing associated interrupt mask bit.

    \li 020:  Create channel out of range mask. LF Channels which signals out of
              range are AND masked with its digital enable/disable bit settings.
              The resulting mask is stored to component variable.

    \li 030:  IF the measurement participating LF channel(s) signalizes out of range,
              THEN

    \li 040:    Set internal status flag indicating field strenght value outside
                the accessible range.
                Set component global error flag and the global debug error
                variable with error code for field strenght value outside the
                accessible range error.
              ENDIF

    \li 050:  Update component flag and status variables:\n
              Signalize LF RSSI measurement has finished and raw RSSI data are
              available in the result register. Additionally signalize
              that all previous calculation data are no longer available by the
              result registers.

    \Derived{No}

    \Rationale{N/A}

    \Traceability   N/A
    \endinternal
\n
*/
/*---------------------------------------------------------------------------*/
/* #pragma vector=LFRSCO_vect */
//lint -esym(714, ATA_lfRssiMeasReady_ISR_C) FlSc (10.06.2014)
//lint -esym(765, ATA_lfRssiMeasReady_ISR_C) FlSc (10.06.2014)
/* disable lint rule 714 - symbol 'ATA_lfRssiMeasReady_ISR_C' not referenced
 * interrupt assignment to Interrupt Vector Table is done by Flash application
 *
 * disable lint rule 765 - external symbol 'ATA_lfRssiMeasReady_ISR_C' could be made static
 * variable shall be accessible from outside via flash software or other library
 * modules
 */
 ISR(LFRSCO_vect)
{
    uint8_t tmpReg, mask;

    /* LLR-Ref: 010 */
    RSCR &= ~(_BM(RSOFM) | _BM(RSOS));

    /* LLR-Ref: 020 */
    mask    = RSMS1R & (_BM(RSCH3E) | _BM(RSCH2E) | _BM(RSCH1E)); // result range is 0x00 - 0x07
    tmpReg  = RSFR & mask;                                  // result range is 0x00 - 0x07
    tmpReg |= (RSFR & (mask<<5U));                          // result range is 0x00 - 0xE7

    g_sLfRssi.bOutOfRangeMask = tmpReg;                     // result is max. 0xE7 absolutely

    /* LLR-Ref: 030 */
    if( tmpReg )
    {
        /* LLR-Ref: 040 */
        g_sLfRssi.bStatus  |= LFRSSI_STATUS_BM_CHANNELS_OUTOFRANGE_FLAG;
        g_sLfRssi.bFlags   |= LFRSSI_FLAGS_BM_ERROR_FLAG;
        g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRSSI_CHANNELS_OUTOFRANGE;
    }

    /* LLR-Ref: 050 */
    g_sLfRssi.bStatus &= ~(LFRSSI_STATUS_BM_CHANCORR_DATA_AVAILABLE_FLAG | LFRSSI_STATUS_BM_3DVEC_DATA_AVAILABLE_FLAG | LFRSSI_STATUS_BM_LINEAR_DATA_AVAILABLE_FLAG | LFRSSI_STATUS_BM_OPERATION_ACTIVE_FLAG);
    g_sLfRssi.bStatus |= LFRSSI_STATUS_BM_MEAS_DATA_AVAILABLE_FLAG;
    g_sLfRssi.bFlags  |= LFRSSI_FLAGS_BM_MEASUREMENT_READY_FLAG;
}





