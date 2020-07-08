#include "eep.h"
#include "globals.h"
#include "Function.h"
/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_eepReadBytes_C</b>
    The purpose of this function is to read multiple bytes from the EEPROM in
    accordance with the applicable EEPROM Access Right Map.
    To copy the bytes, the EEPROM burst read mode is used.

    \param[out] pDes  Pointer pointing to the destination to store the read data to
    \param[in]  wSrc  Specifying the EEProm address to start reading from
    \param[in]  bCount amount of data to be read from the given EEProm address
    \return     Status of the performed EEPROM operation (::eEepErrorCode)
    
    \internal
    \li 001: Store SREG state to be able to restore it at the end of the function 
              call
    \li 005: Update HW Trace Unit with specific function information
    \li 010: Check for the addresses being within range without overflow protection

             Note 1:
             The HW/SW access right management will prevent any locked/protected 
             data to be read. Address overflow will also be detected.

             Note 2:
             The computation of max. address and the following check is correct,
             since the last valid address to read one byte from is 
             EEP_ADDR_START_OUT_OF_RANGE_SEC - 1. This check approach does not 
             require to subract -1 from max. address, which would be the most
             natural way to compute the real max. address.

    \li 020: Check EEP write feature for being busy. Wait in case until it is able
             to deal with the request.
    \li 030: Disable interrupts via global interrupt flag in SREG
    \li 040: Setup EEProm address for start the reading request.
    \li 050: Enable the EEPROM burst read feature by setting bit "EEBRE" in
             register EECR2 to 1
    \li 060: read desired data from EEProm and store it to the destination address
    \li 070: If EEP FEC (EEP error correction flag) flag is, set the return status 
              to EEProm error correction occured and clear the flag afterwards
    \li 080: If EEP AVF (Access violation Flag) flag is, set the return status to
              EEProm address locked for reading and clear the flag afterwards
    \li 090: Disable the EEPROM burst read mode by setting bit "EEBRE" in register
             EECR2 to 0.
    \li 100: Restore SREG to state before function call.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-3143, Primus2P-3146, Primus2P-3150, Primus2P-3153,\
                  Primus2P-3184}
    \endinternal
\n
*/
/*---------------------------------------------------------------------------*/
void RomData_ReadBytes(uint16_t Addr, uint8_t* pBuf, uint8_t bCount)
{
	uint8_t retry=3;
    /* LLR-Ref: 001 */
    uint8_t bSreg = SREG;
	while(retry!=0)
    {
	    /* LLR-Ref: 020 */
	    while(EECR&(BITMASK(NVMBSY)));
	    /* LLR-Ref: 030 */
	    __disable_interrupt();
	        
	    /* LLR-Ref: 040 */
	    EEAR=Addr;
	    //     EEARH = (uint8_t)(wSrc >> SHIFT_HIGH_TO_LOW_BYTE);
	    //     EEARL = (uint8_t)(wSrc & MASK_HIGH_BYTE);
	        
	    /* LLR-Ref: 050 */
	    EECR2 = _BM(EEBRE);
	        
	    /* LLR-Ref: 060 */
	    for(uint8_t i=0;i<bCount;i++)
	    {
		    *pBuf++ = EEDR;
	    }
		/* LLR-Ref: 090 */
		EECR2 &= ~_BM(EEBRE);
	    /* LLR-Ref: 070 */
	    if(EECR2 & _BM(E2FF)) 
		{
		    retry--;
			EECR2 |= _BM(E2FF);
			continue;
	    }
		SREG = bSreg;
		return;
	}
	SystemReset();   
}
uint8_t RomData_ReadByte(uint16_t Addr)
{
	uint8_t ret;
	uint8_t retry=3;
	/* LLR-Ref: 001 */
	uint8_t bSreg = SREG;
	while(retry!=0)
	{
		/* LLR-Ref: 020 */
		while(EECR&(BITMASK(NVMBSY)));
		while(EECR & (1<<EEWE));
		/* LLR-Ref: 030 */
		__disable_interrupt();		
		/* LLR-Ref: 040 */
		EEAR=Addr;		
		/* LLR-Ref: 060 */
		EECR |= (1<<EERE);
		
		ret=EEDR;
		/* LLR-Ref: 070 */
		if(EECR2 & _BM(E2FF))
		{
			retry--;
			EECR2 |= _BM(E2FF);
			continue;
		}
		SREG = bSreg;
		return ret;
	}
	SystemReset();
	return 0;
}

/*---------------------------------------------------------------------------*/
/** \brief <b>ATA_eepWriteBytes_C</b>
    shall write data to the device internal EEProm.

    \param[in] pSrc   Pointer pointing to the source of the written data
    \param[in] wDes   Specifying the EEP address to start writing to
    \param[in] bCount Specifying the amount of data to write to the given EEP address
    \return     Status of the performed EEPROM write access (:.eEepErrorCode)

    \internal
    \li 001: Store SREG state to be able to restore it at the end of the function 
              call
    \li 010: Update HW Trace Unit with specific function information
    \li 020: Check for the addresses being within range with overflow protection

             Note 1:
             The HW/SW access right management will prevent any locked/protected 
             data to be read. Address overflow will also be detected.

             Note 2:
             The computation of max. address and the following check is correct,
             since the last valid address to read one byte from is 
             EEP_ADDR_START_OUT_OF_RANGE_SEC - 1. This check approach does not 
             require to subract -1 from max. address, which would be the most
             natural way to compute the real max. address.

    \li 030: Loop writing the desired data to EEProm by using the page write feature.
    \li 040: Check EEP write feature for being busy. Wait in case untill it is able
              to deal with the request.
    \li 050: Disable interrupts via global interrupt flag in SREG
    \li 060: Reset WDT counter
    \li 070: Setup EEP address and enalbe the page writing feature.
    \li 080: Loop to write the data to the EEP page buffer.
    \li 090: Break in case page border is reached or all bytes have been transfered.
    \li 100: Enable the atomic EEPROM write operation by resetting bits "EEPM1" and
              "EEPM0" in register EECR. Enable the master write mode by setting bit
              "EEMWE" in register EECR. Then Start the EEPROM write by setting bit
              "EEWE" in register EECR.
    \li 110: Restore SREG to state before function call.
    \li 130: Check error flag caused by a wrong access.

    \Derived{No}

    \Rationale{N/A}

    \Traceability{Primus2P-3143, Primus2P-3146, Primus2P-3150, Primus2P-3153\
                  Primus2P-3185}
    \endinternal
\n
*/
/*---------------------------------------------------------------------------*/
void RomData_WriteBytes(uint16_t Addr,uint8_t *pBuf,  uint8_t bCount)
{
    /* LLR-Ref: 001 */
    uint8_t bSreg = SREG;
	/* LLR-Ref: 030 */
	while(bCount>0)
	{
		/* LLR-Ref: 040 */
		while(EECR&(BITMASK(NVMBSY)));
		
		/* LLR-Ref: 050 */
		__disable_interrupt();
		
		/* LLR-Ref: 070 */
		EECR |= _BM(EEPAGE);
		EEARH = (uint8_t)(Addr >> SHIFT_HIGH_TO_LOW_BYTE);
		
		/* LLR-Ref: 080 */
		do{
			EEARL = (uint8_t)(Addr & MASK_HIGH_BYTE);
			EEDR = *pBuf++;
			Addr++;
			bCount--;
			/* LLR-Ref: 090 */
		}while((bCount>0)&&((EEARL&MASK_HIGH_NIBBLE)!=MASK_HIGH_NIBBLE));
		
		/* LLR-Ref: 100 */
		EECR &= ~(_BM(EEPM1) | _BM(EEPM0));
		EECR |= _BM(EEMWE);
		EECR |= _BM(EEWE);
		
		/* LLR-Ref: 110 */
		SREG = bSreg;
	}
	while(EECR&(BITMASK(NVMBSY)));
}
void RomData_FillBytes(uint16_t Addr,uint8_t data,  uint8_t bCount)
{
	/* LLR-Ref: 001 */
	uint8_t bSreg = SREG;
	/* LLR-Ref: 030 */
	while(bCount>0)
	{
		/* LLR-Ref: 040 */
		while(EECR&(BITMASK(NVMBSY)));
		
		/* LLR-Ref: 050 */
		__disable_interrupt();
		
		/* LLR-Ref: 070 */
		EECR |= _BM(EEPAGE);
		EEARH = (uint8_t)(Addr >> SHIFT_HIGH_TO_LOW_BYTE);
		
		/* LLR-Ref: 080 */
		do{
			EEARL = (uint8_t)(Addr & MASK_HIGH_BYTE);
			EEDR = data;
			Addr++;
			bCount--;
			/* LLR-Ref: 090 */
		}while((bCount>0)&&((EEARL&MASK_HIGH_NIBBLE)!=MASK_HIGH_NIBBLE));
		
		/* LLR-Ref: 100 */
		EECR &= ~(_BM(EEPM1) | _BM(EEPM0));
		EECR |= _BM(EEMWE);
		EECR |= _BM(EEWE);
		
		/* LLR-Ref: 110 */
		SREG = bSreg;
	}
	while(EECR&(BITMASK(NVMBSY)));
}
void RomData_WriteByte(uint16_t Addr,uint8_t Data)
{
	/* LLR-Ref: 001 */
	uint8_t bSreg = SREG;
	/* LLR-Ref: 030 */
	/* LLR-Ref: 040 */
	while(EECR&(BITMASK(NVMBSY)));	
	/* LLR-Ref: 050 */
	__disable_interrupt();
	
	/* LLR-Ref: 070 */
	EEAR=Addr;
	EEDR = Data;	
	/* LLR-Ref: 100 */
	EECR &= ~(_BM(EEPM1) | _BM(EEPM0));
	EECR |= _BM(EEMWE);
	EECR |= _BM(EEWE);	
	/* LLR-Ref: 110 */
	SREG = bSreg;
	while(EECR&(BITMASK(NVMBSY)));
}