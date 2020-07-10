#include "globals.h"
#include "../HardwareProfile.h"
sCalibConfig g_sCalibConfig;
void ATA_globalsClkStartXTO_C(void)
{
	/* LLR-Ref: 010 */
	uint8_t bSreg = SREG;
	uint8_t bCmcr = CMCR & 0x30U;
	SUPCR |= _BM(DVHEN);
	ATA_SETBITMASK_C(SUPCR,_BM(AVEN))//AVCC Enable
	while(1)//Wait4AVCC
	{
		ATA_SETBITMASK_C(SUPFR,(_BM(AVCCLF) | _BM(AVCCRF)))
		if ( (SUPFR & (_BM(AVCCLF)|_BM(AVCCRF))) == 0 )
		{
			break;
		}
	}
	FETN4 = eeprom_read_byte((const uint8_t *)0x0904);//eepFETN4, 1U);
	ATA_SETBITMASK_C(FEEN1,_BM(XTOEN))
	FEBT = eeprom_read_byte((const uint8_t *)0x0903);//eepFEBT, 1U);

	while(1)
	{
		if (FESR & _BM(XRDY))
		{
			break;
		}
	}
	/* LLR-Ref: 020 */
	bCmcr |= 0x07U;
	_CLI;
	CMCR = _BM(CMCCE);
	CMCR = bCmcr;

	/* LLR-Ref: 030 */
	bCmcr |= _BM(CCS);

	CMCR = _BM(CMCCE);
	CMCR = bCmcr;

	/* LLR-Ref: 040 */
	CLPR = _BM(CLPCE);
	CLPR = _BM(CLTPS0);
	SREG = bSreg;
}
/**********************
SMCR
0x01 -> Idle
0x05 -> Power Down
0x09 -> Ext. Power Down
************************/
void ATA_globalsSleep_C(uint8_t bSleepModeConfig)
{
	/* LLR-Ref: 010 */
	_CLI;

	/* LLR-Ref: 020 */
	SMCR = bSleepModeConfig & (_BM(SM2) | _BM(SM1) | _BM(SM0) );
	SMCR |= _BM(SE);

	/* LLR-Ref: 030 */
	_SEI;
	_SLEEP;

	/* LLR-Ref: 040 */
	SMCR &= (uint8_t)~_BM(SE);
}
void ATA_globalsClkSwitchFrc_C(void)
{
	/* LLR-Ref: 010 */
	uint8_t bSreg = SREG;
	uint8_t cmcr = CMCR & 0x30U;
	uint8_t bCmocrFrcao = CMOCR & _BM(FRCAO);
	
	/* LLR-Ref: 012 */
	SUPCR |= _BM(DVHEN);

	/* LLR-Ref: 015 */
	CMOCR |= _BM(FRCAO);
	do
	{
		_NOP;
	} while((CMOCR & _BM(FRCACT)) == 0U);

	/* LLR-Ref: 020 */
	cmcr |= _BM(CMM0);

	cli();
	CMCR = _BM(CMCCE);
	CMCR = cmcr;

	/* LLR-Ref: 030 */
	cmcr |= _BM(CCS);

	CMCR = _BM(CMCCE);
	CMCR = cmcr;

	/* LLR-Ref: 040 */
	CLPR = _BM(CLPCE);
	CLPR = _BM(CLTPS0);

	/* LLR-Ref: 050 */
	if ( bCmocrFrcao == 0x00U )
	{
		CMOCR &= ~_BM(FRCAO);
	}

	SREG = bSreg;
}
void ATA_globalsClkSwitchMrc_C(void)
{
	uint8_t bSreg = SREG;
	uint8_t cmcr = 0x00U;

	/* LLR-Ref: 010 */
	_CLI;
	CMCR = _BM(CMCCE);
	CMCR = cmcr;

	/* LLR-Ref: 015 */
	SUPCR &= ~_BM(DVHEN);

	/* LLR-Ref: 020 */
	CLPR = _BM(CLPCE);
	CLPR = _BM(CLTPS0);
	SREG = bSreg;
}

void ATA_globalsWdtDisable_C(void)
{
	uint8_t bSreg;

	/* LLR-Ref: 010 */
	bSreg = SREG;
	_CLI;

	/* LLR-Ref: 020 */
	MCUSR &= (uint8_t)(~_BM(WDRF));

	/* LLR-Ref: 030 */
	WDTCR = (_BM(WDCE) | _BM(WDE));
	WDTCR = (_BM(WDPS2) | _BM(WDPS1) | _BM(WDPS0));

	/* LLR-Ref: 040 */
	SREG = bSreg;
}

extern void ATA_globalsWdtEnable_C(uint8_t uConfWDTCR)
{
	uint8_t bSreg = SREG;

	/* LLR-Ref: 010 */
	_CLI;

	/* LLR-Ref: 020 */
	WDTCR |= (_BM(WDCE) | _BM(WDE));

	/* LLR-Ref: 030 */
	WDTCR = uConfWDTCR;
	WDTCR = uConfWDTCR;

	/* LLR-Ref: 040 */
	SREG = bSreg;
}

void ATA_calibInit_C(void)
{
	/* LLR-Ref: 017 */
	uint8_t ary[2];
	//0x086A eepCalibConfPtr_l Ptr to the SRC/FRC calibration configuration location
	//0x086B eepCalibConfPtr_h Ptr to the SRC/FRC calibration configuration location
	eeprom_read_block(&ary[0],(const void *)0x086a,2U);
	uint16_t wEepCalibAddr = (uint16_t)((ary[1]<<8|ary[0]));

	g_sCalibConfig.wSrcResult = 0x0000U;
	uint8_t *pDestData = (uint8_t*)(&g_sCalibConfig.sSrcCalibrationConfig);

	eeprom_read_block(pDestData, (const void *)wEepCalibAddr, sizeof(g_sCalibConfig.sSrcCalibrationConfig));

	/* LLR-Ref: 030 */
	g_sCalibConfig.wFrcResult = 0x0000U;
	wEepCalibAddr += (uint16_t)sizeof(g_sCalibConfig.sSrcCalibrationConfig);
	pDestData = (uint8_t *)(&g_sCalibConfig.sFrcCalibrationConfig);

	eeprom_read_block(pDestData, (const void *)wEepCalibAddr, sizeof(g_sCalibConfig.sFrcCalibrationConfig));
}