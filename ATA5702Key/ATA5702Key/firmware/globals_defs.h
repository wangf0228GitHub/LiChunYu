#ifndef GLOBALS_DEFS_H
#define GLOBALS_DEFS_H
#define _BM(bit) (uint8_t)(1U<<bit)
#define _SLEEP        asm ("sleep")
#define _NOP          asm ("nop")
#define _IJMP         asm ("ijmp")	//
#define _SEI          asm ("sei")		// set Global Interrupt Enable
#define _CLI          asm ("cli") 	// disable global interrupt
#define _WDR          asm ("wdr")
#define __disable_interrupt() _CLI

#define ClrBit_uint8(b,index) b &= (uint8_t)(~(1 << index))
#define SetBit_uint8(b,index) b |= (uint8_t)(1 << index)

#define ClrBit_uint32(b,index) b &= (uint32_t)(~(1 << index))
#define SetBit_uint32(b,index) b |= (uint32_t)(1 << index)

#define GetBit_uint8(b,index) ((((b) & (((uint8_t)1)<<index)) != 0) ? 1 : 0)
#define GetBit_uint32(b,index) ((((b) & (((uint32_t)1)<<index)) != 0) ? 1 : 0)
/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_SETBIT_C</b>
    sets the bit bit_number in variable data
 */
/*---------------------------------------------------------------------------*/
#define ATA_SETBIT_C(data, bit_number) {  \
    data |= (1 << bit_number);          \
    }

/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_CLEARBIT_C</b>
    clears the bit bit_number in variable data
 */
/*---------------------------------------------------------------------------*/
#define ATA_CLEARBIT_C(data, bit_number) { \
    data &= (uint8_t)~(1<<bit_number);     \
    }

/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_SETBIT_C</b>
    sets the bits of bit_mask in variable data
 */
/*---------------------------------------------------------------------------*/
#define ATA_SETBITMASK_C(data, bit_mask) {  \
    data |= (bit_mask);                 \
    }

/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_CLEARBITMASK_C</b>
    clears the bits of bit_mask in variable data
 */
/*---------------------------------------------------------------------------*/
#define ATA_CLEARBITMASK_C(data, bit_mask) { \
    data &= (uint8_t)~(bit_mask);        \
    }
/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_POWERON_C</b>
    enables the Power via PRR register
 */
/*---------------------------------------------------------------------------*/
#define ATA_POWERON_C(data, bit_number)   ATA_CLEARBIT_C(data, bit_number)
/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_POWERON_C</b>
    disables the Power via PRR register
 */
/*---------------------------------------------------------------------------*/
#define ATA_POWEROFF_C(data, bit_number)  ATA_SETBIT_C(data, bit_number)

#define BITMASK(name)   (uint8_t)(1U << name)

/* bits and masks */
#define BIT_7      7
#define BIT_6      6
#define BIT_5      5
#define BIT_4      4
#define BIT_3      3
#define BIT_2      2
#define BIT_1      1
#define BIT_0      0

#define BIT_MASK_7     BITMASK(BIT_7)
#define BIT_MASK_6     BITMASK(BIT_6)
#define BIT_MASK_5     BITMASK(BIT_5)
#define BIT_MASK_4     BITMASK(BIT_4)
#define BIT_MASK_3     BITMASK(BIT_3)
#define BIT_MASK_2     BITMASK(BIT_2)
#define BIT_MASK_1     BITMASK(BIT_1)
#define BIT_MASK_0     BITMASK(BIT_0)

/* Bits and Byte masking */
#define LOW_NIBBLE(x)	(uint8_t)(x&0x0F)
#define HIGH_NIBBLE(x)	(uint8_t)(x>>4)
#define LOW_BYTE(x)     ((uint8_t)((x)&0xFF))
#define HIGH_BYTE(x)    ((uint8_t)(((x)>>8)&0xFF))

#define MAKE_BYTE(h,l)	((h<<4)+(l&0x0f))
#define MAKE_SHORT(h,l)	(((uint16_t)h<<8)+l)
#define MAKE_INT(h,l)	(((uint32_t)h<<16)+l)

#define MASK_HIGH_NIBBLE            (uint8_t)0x0FU
#define MASK_LOW_NIBBLE             (uint8_t)0xF0U
#define MASK_HIGH_BYTE              (uint16_t)0x00FFU
#define MASK_LOW_BYTE               (uint16_t)0xFF00U

/** \brief <b>SHIFT_HIGH_TO_LOW_BYTE</b>
    defines the number of bits to shift a high byte to low byte position
    (Integer with 16-Bit)
*/
#define SHIFT_HIGH_TO_LOW_BYTE       (uint8_t)0x08U

/** \brief <b>SHIFT_LOW_TO_HIGH_BYTE</b>
    defines the number of bits to shift a low byte to high byte position
    (Integer with 16-Bit)
*/
#define SHIFT_LOW_TO_HIGH_BYTE      (uint8_t)0x08U

/** \brief <b>NUM_EEPROM_SERVICES</b>
    is the number of services located in EEPROM
*/
#define NUM_EEPROM_SERVICES          3U

/** \brief <b>NUM_SRAM_SERVICES</b>
    is the number of services located in SRAM
*/
#define NUM_SRAM_SERVICES            2U

/** \brief <b>NUM_SERVICES</b>
    is the total number of services in the application
*/
#define NUM_SERVICES                (NUM_EEPROM_SERVICES + NUM_SRAM_SERVICES)

/** \brief <b>NUM_CHANNELS_PER_SERVICE</b>
    is the number of channels located in one service
*/
#define NUM_CHANNELS_PER_SERVICE    3U

/** \brief <b>NUM_PATHES_PER_SERVICE</b>
    is the number of pathes located in one service
*/
#define NUM_PATHES_PER_SERVICE      1U

/** \brief <b>DEBUG_ERROR_CODE_xxx</b>
    are the errorCodes for ATA5700 SW
*/
#define DEBUG_ERROR_CODE_SYSTEM_ERROR_EEPROM_NOT_VALID                  0U                                                                  // 000<->00
#define DEBUG_ERROR_CODE_SETIDLEMODE_MISS_SSMRDY                        DEBUG_ERROR_CODE_SYSTEM_ERROR_EEPROM_NOT_VALID + 1U                 // 001<->01
#define DEBUG_ERROR_CODE_SETIDLEMODE_TIMER_LOCKED                       DEBUG_ERROR_CODE_SETIDLEMODE_MISS_SSMRDY + 1U                       // 002<->02
#define DEBUG_ERROR_CODE_RX_STATESTARTSSM_TIMER_LOCKED                  DEBUG_ERROR_CODE_SETIDLEMODE_TIMER_LOCKED + 1U                      // 003<->03
#define DEBUG_ERROR_CODE_RX_STATEWAIT4SSMSTATE_GETTELEGRAM_TIMEOUT      DEBUG_ERROR_CODE_RX_STATESTARTSSM_TIMER_LOCKED + 1U                 // 004<->04
#define DEBUG_ERROR_CODE_TX_STATESTARTSSM_TIMER_LOCKED                  DEBUG_ERROR_CODE_RX_STATEWAIT4SSMSTATE_GETTELEGRAM_TIMEOUT + 1U     // 005<->05
#define DEBUG_ERROR_CODE_TX_STATEWAIT4SSMRDY_TIMEOUT                    DEBUG_ERROR_CODE_TX_STATESTARTSSM_TIMER_LOCKED + 1U                 // 006<->06
#define DEBUG_ERROR_CODE_POLL_STATESTARTSSM_TIMER_LOCKED                DEBUG_ERROR_CODE_TX_STATEWAIT4SSMRDY_TIMEOUT + 1U                   // 007<->07
#define DEBUG_ERROR_CODE_POLL_STATEWAIT4SSMSTATE_GETTELEGRAM_TIMEOUT    DEBUG_ERROR_CODE_POLL_STATESTARTSSM_TIMER_LOCKED + 1U               // 008<->08
#define DEBUG_ERROR_CODE_ANTTUNE_STATESTARTSSM_TIMER_LOCKED             DEBUG_ERROR_CODE_POLL_STATEWAIT4SSMSTATE_GETTELEGRAM_TIMEOUT + 1U   // 009<->09
#define DEBUG_ERROR_CODE_ANTTUNE_STATEWAIT4SSMRDY_TIMEOUT               DEBUG_ERROR_CODE_ANTTUNE_STATESTARTSSM_TIMER_LOCKED + 1U            // 010<->0A
#define DEBUG_ERROR_CODE_VCOCAL_STATESTARTSSM_TIMER_LOCKED              DEBUG_ERROR_CODE_ANTTUNE_STATEWAIT4SSMRDY_TIMEOUT + 1U              // 011<->0B
#define DEBUG_ERROR_CODE_VCOCAL_STATEWAIT4SSMRDY_TIMEOUT                DEBUG_ERROR_CODE_VCOCAL_STATESTARTSSM_TIMER_LOCKED + 1U             // 012<->0C
#define DEBUG_ERROR_CODE_SHUTDOWNTRX_TIMER_LOCKED                       DEBUG_ERROR_CODE_VCOCAL_STATEWAIT4SSMRDY_TIMEOUT + 1U               // 013<->0D
#define DEBUG_ERROR_CODE_SHUTDOWNTRX_TIMEOUT                            DEBUG_ERROR_CODE_SHUTDOWNTRX_TIMER_LOCKED + 1U                      // 014<->0E
#define DEBUG_ERROR_CODE_POLLING_TIMER1_LOCKED                          DEBUG_ERROR_CODE_SHUTDOWNTRX_TIMEOUT + 1U                           // 015<->0F
#define DEBUG_ERROR_CODE_INVALID_OPM_SWITCHING                          DEBUG_ERROR_CODE_POLLING_TIMER1_LOCKED + 1U                         // 016<->10
#define DEBUG_ERROR_CODE_INVALID_OPM_MODE_DURING_TUNE_AND_CHECK         DEBUG_ERROR_CODE_INVALID_OPM_SWITCHING + 1U                         // 017<->11
#define DEBUG_ERROR_CODE_AVCCLOW_DURING_TX                              DEBUG_ERROR_CODE_INVALID_OPM_MODE_DURING_TUNE_AND_CHECK + 1U        // 018<->12
#define DEBUG_ERROR_CODE_SERVICE_INIT_FAILURE                           DEBUG_ERROR_CODE_AVCCLOW_DURING_TX + 1U                             // 019<->13
#define DEBUG_ERROR_CODE_DFIFO_OVER_UNDER_FLOW                          DEBUG_ERROR_CODE_SERVICE_INIT_FAILURE + 1U                          // 020<->14
#define DEBUG_ERROR_CODE_SFIFO_OVER_UNDER_FLOW                          DEBUG_ERROR_CODE_DFIFO_OVER_UNDER_FLOW + 1U                         // 021<->15
#define DEBUG_ERROR_CODE_RSSI_STATESTARTSSM_TIMER_LOCKED                DEBUG_ERROR_CODE_SFIFO_OVER_UNDER_FLOW + 1U                         // 022<->16
#define DEBUG_ERROR_CODE_SRC_FRC_CALIB_FAILED                           DEBUG_ERROR_CODE_RSSI_STATESTARTSSM_TIMER_LOCKED + 1U               // 023<->17
#define DEBUG_ERROR_CODE_GETRXTELEGRAM_SSM_ERROR                        DEBUG_ERROR_CODE_SRC_FRC_CALIB_FAILED + 1U                          // 024<->18
#define DEBUG_ERROR_CODE_TEMPMEAS_STATESTARTSSM_TIMER_LOCKED            DEBUG_ERROR_CODE_GETRXTELEGRAM_SSM_ERROR + 1U                       // 025<->19
#define DEBUG_ERROR_CODE_TEMPMEAS_STATEWAIT4SSMRDY_TIMEOUT              DEBUG_ERROR_CODE_TEMPMEAS_STATESTARTSSM_TIMER_LOCKED + 1U           // 026<->1A
#define DEBUG_ERROR_CODE_AVCCLOW_TIMEOUT                                DEBUG_ERROR_CODE_TEMPMEAS_STATEWAIT4SSMRDY_TIMEOUT + 1U             // 027<->1B
// error codes for module SPI
#define DEBUG_ERROR_CODE_SPI_NOT_INITIALIZED                            DEBUG_ERROR_CODE_AVCCLOW_TIMEOUT + 1U                               // 028<->1C
#define DEBUG_ERROR_CODE_SPI_ALREADY_OPENED                             DEBUG_ERROR_CODE_SPI_NOT_INITIALIZED + 1U                           // 029<->1D
// error codes for module ATA5831
#define DEBUG_ERROR_CODE_ATA5831_SPI_CHECKSUM_ERROR                     DEBUG_ERROR_CODE_SPI_ALREADY_OPENED + 1U                            // 030<->1E
// error codes for module CALIB
#define DEBUG_ERROR_CODE_CALIB_TIMER_SRCCAL_LOCKED                      DEBUG_ERROR_CODE_ATA5831_SPI_CHECKSUM_ERROR + 1U                    // 031<->1F
#define DEBUG_ERROR_CODE_CALIB_TIMER_FRCCAL_LOCKED                      DEBUG_ERROR_CODE_CALIB_TIMER_SRCCAL_LOCKED + 1U                     // 032<->20
#define DEBUG_ERROR_CODE_CALIB_EXTERNAL_CLOCK_FAILURE                   DEBUG_ERROR_CODE_CALIB_TIMER_FRCCAL_LOCKED + 1U                     // 033<->21
// error codes for module RFTX
#define DEBUG_ERROR_CODE_RFTX_STARTSSM_TIMER_LOCKED                     DEBUG_ERROR_CODE_CALIB_EXTERNAL_CLOCK_FAILURE + 1U                  // 034<->22
#define DEBUG_ERROR_CODE_RFTX_SHUTDOWN_ERROR                            DEBUG_ERROR_CODE_RFTX_STARTSSM_TIMER_LOCKED + 1U                    // 035<->23
#define DEBUG_ERROR_CODE_RFTX_WAIT4SSMRDY_TIMEOUT                       DEBUG_ERROR_CODE_RFTX_SHUTDOWN_ERROR + 1U                           // 036<->24
#define DEBUG_ERROR_CODE_RFTX_SFIFO_ERROR                               DEBUG_ERROR_CODE_RFTX_WAIT4SSMRDY_TIMEOUT + 1U                      // 037<->25
#define DEBUG_ERROR_CODE_RFTX_DFIFO_ERROR                               DEBUG_ERROR_CODE_RFTX_SFIFO_ERROR + 1U                              // 038<->26
#define DEBUG_ERROR_CODE_RFTX_WAIT4VCOSSMRDY_TIMEOUT                    DEBUG_ERROR_CODE_RFTX_DFIFO_ERROR + 1U                              // 039<->27
#define DEBUG_ERROR_CODE_RFTX_WAIT4ANTSSMRDY_TIMEOUT                    DEBUG_ERROR_CODE_RFTX_WAIT4VCOSSMRDY_TIMEOUT + 1U                   // 040<->28
// error codes for module LF RSSI
#define DEBUG_ERROR_CODE_LFRSSI_EEPROM_READ_ERROR                       DEBUG_ERROR_CODE_RFTX_WAIT4ANTSSMRDY_TIMEOUT + 1U                   // 041<->29
#define DEBUG_ERROR_CODE_LFRSSI_EEPROM_ACCESS_ERROR                     DEBUG_ERROR_CODE_LFRSSI_EEPROM_READ_ERROR + 1U                      // 042<->2A
#define DEBUG_ERROR_CODE_LFRSSI_ANALOGPART_NOTREADY                     DEBUG_ERROR_CODE_LFRSSI_EEPROM_ACCESS_ERROR + 1U                    // 043<->2B
#define DEBUG_ERROR_CODE_LFRSSI_INVALID_MEASUREMENT_CHANNELS            DEBUG_ERROR_CODE_LFRSSI_ANALOGPART_NOTREADY + 1U                    // 044<->2C
#define DEBUG_ERROR_CODE_LFRSSI_NULL_POINTER_PASSED                     DEBUG_ERROR_CODE_LFRSSI_INVALID_MEASUREMENT_CHANNELS + 1U           // 045<->2D
#define DEBUG_ERROR_CODE_LFRSSI_OPERATION_ACTIVE                        DEBUG_ERROR_CODE_LFRSSI_NULL_POINTER_PASSED + 1U                    // 046<->2E
#define DEBUG_ERROR_CODE_LFRSSI_CHANNELS_OUTOFRANGE                     DEBUG_ERROR_CODE_LFRSSI_OPERATION_ACTIVE + 1U                       // 047<->2F
// error codes for module LF RX
#define DEBUG_ERROR_CODE_LFRX_EEPROM_READ_ERROR                         DEBUG_ERROR_CODE_LFRSSI_CHANNELS_OUTOFRANGE + 1U                    // 048<->30
#define DEBUG_ERROR_CODE_LFRX_EEPROM_ACCESS_ERROR                       DEBUG_ERROR_CODE_LFRX_EEPROM_READ_ERROR + 1U                        // 049<->31
// error codes for module AES
#define DEBUG_ERROR_CODE_AES_ALREADY_IN_USE_ERROR                       DEBUG_ERROR_CODE_LFRX_EEPROM_ACCESS_ERROR + 1U                      // 050<->32
#define DEBUG_ERROR_CODE_AES_INVALID_KEY_ERROR                          DEBUG_ERROR_CODE_AES_ALREADY_IN_USE_ERROR + 1U                      // 051<->33
#define DEBUG_ERROR_CODE_AES_RUN_ERROR                                  DEBUG_ERROR_CODE_AES_INVALID_KEY_ERROR + 1U                         // 052<->34
// error codes for module RFRCC
#define DEBUG_ERROR_CODE_RFRCC_EEP_READ_ERROR                           DEBUG_ERROR_CODE_AES_RUN_ERROR + 1U                                 // 053<->35
#define DEBUG_ERROR_CODE_RFRCC_EEP_WRITE_ACCESS_ERROR                   DEBUG_ERROR_CODE_RFRCC_EEP_READ_ERROR + 1U                          // 054<->36
#define DEBUG_ERROR_CODE_RFRCC_MAC_ERROR                                DEBUG_ERROR_CODE_RFRCC_EEP_WRITE_ACCESS_ERROR + 1U                  // 055<->37
#define DEBUG_ERROR_CODE_RFRCC_SUBKEY_ERROR                             DEBUG_ERROR_CODE_RFRCC_MAC_ERROR + 1U                               // 056<->38
// error codes for module TWI
#define DEBUG_ERROR_CODE_TWI_NACK_SLAW_ERROR                            DEBUG_ERROR_CODE_RFRCC_SUBKEY_ERROR + 1U                            // 057<->39
#define DEBUG_ERROR_CODE_TWI_NACK_DATA_TX_ERROR                         DEBUG_ERROR_CODE_TWI_NACK_SLAW_ERROR + 1U                           // 058<->3A
#define DEBUG_ERROR_CODE_TWI_STATUS_CODE_ERROR                          DEBUG_ERROR_CODE_TWI_NACK_DATA_TX_ERROR + 1U                        // 059<->3B
#define DEBUG_ERROR_CODE_TWI_NACK_SLAR_ERROR                            DEBUG_ERROR_CODE_TWI_STATUS_CODE_ERROR + 1U                         // 060<->3C
#define DEBUG_ERROR_CODE_TWI_INVALID_REQUEST_ERROR                      DEBUG_ERROR_CODE_TWI_NACK_SLAR_ERROR + 1U                           // 061<->3D

// error codes for module CALIB (extended)
#define DEBUG_ERROR_CODE_CALIB_EEPROM_READ_ERROR                        DEBUG_ERROR_CODE_TWI_INVALID_REQUEST_ERROR + 1U                     // 062<->3E

// error codes for module RFTX (extended)
#define DEBUG_ERROR_CODE_RFTX_EEPROM_READ_ERROR                         DEBUG_ERROR_CODE_CALIB_EEPROM_READ_ERROR + 1U                       // 063<->3F

// error codes for module Globals
#define DEBUG_ERROR_CODE_GLOBALS_EEPROM_READ_ERROR                      DEBUG_ERROR_CODE_RFTX_EEPROM_READ_ERROR + 1U                        // 064<->40

// error code indicating NOT USED
#define DEBUG_ERROR_CODE_SYSTEM_ERROR_NOT_USED                          0xFFU                                                               // 255<->FF

/* ------------------------------------------------------------------------- */
/* tuneCheckConfig_t                                                 */
/* ------------------------------------------------------------------------- */
#define TUNE_CHECK_CONFIG_ANTENNA_TUNING    BIT_7
#define TUNE_CHECK_CONFIG_TEMP_MEASUREMENT  BIT_6
#define TUNE_CHECK_CONFIG_SRC_CALIB         BIT_5
#define TUNE_CHECK_CONFIG_FRC_CALIB         BIT_4
#define TUNE_CHECK_CONFIG_VCO_CALIB         BIT_3
#define TUNE_CHECK_CONFIG_RF_CALIB          BIT_2
#define TUNE_CHECK_CONFIG_SELF_CHECK        BIT_1
#define TUNE_CHECK_CONFIG_REG_REFRESH       BIT_0

#define BM_TUNE_CHECK_CONFIG_ANTENNA_TUNING    BIT_MASK_7
#define BM_TUNE_CHECK_CONFIG_TEMP_MEASUREMENT  BIT_MASK_6
#define BM_TUNE_CHECK_CONFIG_SRC_CALIB         BIT_MASK_5
#define BM_TUNE_CHECK_CONFIG_FRC_CALIB         BIT_MASK_4
#define BM_TUNE_CHECK_CONFIG_VCO_CALIB         BIT_MASK_3
#define BM_TUNE_CHECK_CONFIG_RF_CALIB          BIT_MASK_2
#define BM_TUNE_CHECK_CONFIG_SELF_CHECK        BIT_MASK_1
#define BM_TUNE_CHECK_CONFIG_REG_REFRESH       BIT_MASK_0
/* ------------------------------------------------------------------------- */
/* sysModeConfig_t                                                           */
/* ------------------------------------------------------------------------- */
#define SYS_MODE_CONFIG_RF_CALIBRATION          BIT_7
#define SYS_MODE_CONFIG_ANTENNA_TUNING          BIT_6
#define SYS_MODE_CONFIG_VCO_TUNING              BIT_5
#define SYS_MODE_CONFIG_IDLE_MODE_SELECTOR      BIT_4
#define SYS_MODE_CONFIG_DIRECT_SWITCH           BIT_3
#define SYS_MODE_CONFIG_TRANSPARENT_MODE        BIT_2
#define SYS_MODE_CONFIG_OPM1                    BIT_1
#define SYS_MODE_CONFIG_OPM0                    BIT_0

#define BM_SYS_MODE_CONFIG_RF_CALIBRATION       BIT_MASK_7
#define BM_SYS_MODE_CONFIG_ANTENNA_TUNING       BIT_MASK_6
#define BM_SYS_MODE_CONFIG_VCO_TUNING           BIT_MASK_5
#define BM_SYS_MODE_CONFIG_IDLE_MODE_SELECTOR   BIT_MASK_4
#define BM_SYS_MODE_CONFIG_DIRECT_SWITCH        BIT_MASK_3
#define BM_SYS_MODE_CONFIG_TRANSPARENT_MODE     BIT_MASK_2

#define BM_SYS_MODE_CONFIG_OPM                  ( BIT_MASK_1 | BIT_MASK_0 )
#define BM_SYS_MODE_CONFIG_OPM1                 BIT_MASK_1
#define BM_SYS_MODE_CONFIG_OPM0                 BIT_MASK_0

/* ------------------------------------------------------------------------- */
/* System clock selections                                                   */
/* ------------------------------------------------------------------------- */
#define BM_SYS_CLOCK_CLOCK_SELECT_XTO4          (0x07U)
#define BM_SYS_CLOCK_CLOCK_SELECT_XTO6          (0x03U)

/* ------------------------------------------------------------------------- */
/* Operation modes                                                           */
/* ------------------------------------------------------------------------- */
#define OPM_IDLE        0x00U
#define OPM_TX          0x01U

/* ------------------------------------------------------------------------- */
/* TRXCONF_SERVICE_CHANNEL_CONFIG                                            */
/* ------------------------------------------------------------------------- */
#define SVC_CH_CONFIG_PATHB         BIT_7
#define SVC_CH_CONFIG_PATHA         BIT_6
#define SVC_CH_CONFIG_CH1           BIT_5
#define SVC_CH_CONFIG_CH0           BIT_4

#define SVC_CH_CONFIG_SER2          BIT_2
#define SVC_CH_CONFIG_SER1          BIT_1
#define SVC_CH_CONFIG_SER0          BIT_0

#define BM_SVC_CH_CONFIG_PATHB      BIT_MASK_7
#define BM_SVC_CH_CONFIG_PATHA      BIT_MASK_6

#define BM_SVC_CH_CONFIG_CH         ( BIT_MASK_5 | BIT_MASK_4 )
#define BM_SVC_CH_CONFIG_CH1        BIT_MASK_5
#define BM_SVC_CH_CONFIG_CH0        BIT_MASK_4

#define BM_SVC_CH_CONFIG_SER        ( BIT_MASK_2 | BIT_MASK_1 | BIT_MASK_0 )
#define BM_SVC_CH_CONFIG_SER2       BIT_MASK_2
#define BM_SVC_CH_CONFIG_SER1       BIT_MASK_1
#define BM_SVC_CH_CONFIG_SER0       BIT_MASK_0

/* ------------------------------------------------------------------------- */
/* pinEventConfig_t                                                          */
/* ------------------------------------------------------------------------- */
#define PIN_EVENT_CONFIG_POWERON    BIT_7
#define PIN_EVENT_CONFIG_DEBUG      BIT_6
#define PIN_EVENT_CONFIG_NPWR6      BIT_5
#define PIN_EVENT_CONFIG_NPWR5      BIT_4
#define PIN_EVENT_CONFIG_NPWR4      BIT_3
#define PIN_EVENT_CONFIG_NPWR3      BIT_2
#define PIN_EVENT_CONFIG_NPWR2      BIT_1
#define PIN_EVENT_CONFIG_NPWR1      BIT_0

#define BM_PIN_EVENT_CONFIG_POWERON    BIT_MASK_7
#define BM_PIN_EVENT_CONFIG_DEBUG      BIT_MASK_6
#define BM_PIN_EVENT_CONFIG_NPWR6      BIT_MASK_5
#define BM_PIN_EVENT_CONFIG_NPWR5      BIT_MASK_4
#define BM_PIN_EVENT_CONFIG_NPWR4      BIT_MASK_3
#define BM_PIN_EVENT_CONFIG_NPWR3      BIT_MASK_2
#define BM_PIN_EVENT_CONFIG_NPWR2      BIT_MASK_1
#define BM_PIN_EVENT_CONFIG_NPWR1      BIT_MASK_0


/* ------------------------------------------------------------------------- */
/* sysEventConfig_t                                                          */
/* ------------------------------------------------------------------------- */
#define SYS_EVENT_CONFIG_SYS_ERR        BIT_7
#define SYS_EVENT_CONFIG_CMD_RDY        BIT_6
#define SYS_EVENT_CONFIG_SYS_RDY        BIT_5
#define SYS_EVENT_CONFIG_AVCCLOW        BIT_4
#define SYS_EVENT_CONFIG_LOWBATT        BIT_3
#define SYS_EVENT_CONFIG_EVENTPIN_POL   BIT_0

#define BM_SYS_EVENT_CONFIG_SYS_ERR         BIT_MASK_7
#define BM_SYS_EVENT_CONFIG_CMD_RDY         BIT_MASK_6
#define BM_SYS_EVENT_CONFIG_SYS_RDY         BIT_MASK_5
#define BM_SYS_EVENT_CONFIG_AVCCLOW         BIT_MASK_4
#define BM_SYS_EVENT_CONFIG_LOWBATT         BIT_MASK_3
#define BM_SYS_EVENT_CONFIG_EVENTPIN_POL    BIT_MASK_0

#endif /* GLOBALS_DEFS_H */
