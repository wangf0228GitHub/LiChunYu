#ifndef GLOBALS_H
#define GLOBALS_H

#include "HardwareProfile.h"

extern void ATA_globalsClkStartXTO_C(void);
extern void ATA_globalsClkSwitchFrc_C(void);
extern void ATA_globalsClkSwitchMrc_C(void);
extern void ATA_globalsWdtDisable_C(void);
extern void ATA_globalsWdtEnable_C(uint8_t uConfWDTCR);


/**********************
SMCR
0x01 -> Idle
0x05 -> Power Down
0x09 -> Ext. Power Down
************************/
#define Idle_Sleep_Mode 1
#define Power_Save_Mode 3
#define LF_SLEEP_MODE    9//Power-Down
extern void ATA_globalsSleep_C(uint8_t bSleepModeConfig);
/*----------------------------------------------------------------------------- */
/** \brief <b>sEEPromSrcCalibrationConfig</b>
    \brief <b>sSrcCalibrationConfig</b>
    contains the configuration data for SRC Calibration
 */
/*----------------------------------------------------------------------------- */
typedef struct {
    /** \brief <b>xtoCyclesPerStep</b>
        contains the number of XTO/4(XTO/8) cycles for SRC/FRC calibration
        measurement gate
     */
    uint16_t wXtoCyclesPerMeasurement;
    /** \brief <b>gradient</b>
        contains the gradient in unsigned fractional format q2.6
     */
    uint8_t bGradient;
}sSrcCalibrationConfig, sEEPromSrcCalibrationConfig;
/*----------------------------------------------------------------------------- */
/** \brief <b>sEEPromFrcCalibrationConfig</b>
    \brief <b>sFrcCalibrationConfig</b>
    contains the configuration data for FRC Calibration
 */
/*----------------------------------------------------------------------------- */
typedef struct {
    /** \brief <b>T3CORL</b>
     */
    uint8_t bT3CORL;
    /** \brief <b>T3CORH</b>
     */
    uint8_t bT3CORH;
}sFrcCalibrationConfig, sEEPromFrcCalibrationConfig;
typedef struct{
    /*
        bit 7:      calib Module error flag
        bit 6..0:   tbd
     */
    uint8_t bFlags;
    /*
        bit 7..0:   tbd
     */
    uint8_t bStatus;
    /*
        bit 7:  FRC calibration enable/disable flag
                    0: FRC calibration disabled
                    1: FRC calibration enabled
        bit 6:  SRC calibration enable/disable flag
                    0: SRC calibration disabled
                    1: SRC calibration enabled
        bit 6:  rfu
        bit 5:  rfu
        bit 4:  rfu
        bit 2:  rfu
        bit 1:  XTO Active after SRC/FRC calibration flag
                    0: XTO is deactivated after SRC/FRC calibration
                    1: XTO is still activated after SRC/FRC calibration
        bit 0:  SRC/FRC calibration clock
                    0: SRC/FRC calibration via XTO_CLK
                    1: SRC/FRC calibration via EXT_CLK
     */
    uint8_t bConfig;
    
    /** \brief <b>wSrcResult</b>
        contains the measured number of XTO/4(XTO/8) cycles for SRC 
        calibration
     */
    uint16_t wSrcResult;
    
    /** \brief <b>sSrcCalibrationConfig</b>
        contains the EEPROM SRC calibration configuration
     */
    sSrcCalibrationConfig sSrcCalibrationConfig;
    
    /** \brief <b>wFrcResult</b>
        contains the measured number of XTO/4(XTO/8) cycles for FRC 
        calibration
     */
    uint16_t wFrcResult;
    
    /** \brief <b>sFrcCalibrationConfig</b>
        contains the EEPROM FRC calibration configuration
     */
    sFrcCalibrationConfig sFrcCalibrationConfig;

}sCalibConfig;
typedef struct{
    /** \brief <b>bErrorCode</b>
        contains the last occured system error
     */
    uint8_t bErrorCode;
    /** \brief <b>bSsmErrorCode</b>
        contains the last occured ssm error
     */
    uint8_t bSsmErrorCode;
}sDebugErrorCodes;
extern sDebugErrorCodes g_sDebug;
extern sCalibConfig g_sCalibConfig;
extern void ATA_calibInit_C(void);
extern void ATA_globalsClkSwitchXTO_C(uint8_t bXtoClockSelect);
extern void ATA_globalsClkSwitchSrc_C(void);
extern void ATA_globalsCopySramSpace_C(uint8_t *pDestination, uint8_t *pSource, uint8_t bLength);
#endif /* GLOBALS_H */
