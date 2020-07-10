#include "LF_RX.h"
#include "../HardwareProfile.h"
#include "../Variables.h"
ISR(TPINT_vect)
{
	LED_Toggle();
	TPFR =0x0f;
}
ISR(LFPBD_vect)//检测到125k则唤醒
{
	LED_Toggle();
	LFFR = 0x0F;  // clear all flags
}
ISR(LFEOT_vect)//LF Decoder Error Interrupt
{
	LED_Toggle();
	/* Check if EOT has been detected */
	//End of Telegram 
	if ( (LFFR & _BM(LFEOF)) != 0x00U )
	{
		/* Clear flag */
		LFFR |= _BM(LFEOF);
		ID0_Wake=0x02;		
		LFIMR = 0x00;//关中断
	}

	/* Check if a timeout has been detected */
	// Telegram Time Out
	if ( (LFFR & _BM(LFTOF)) != 0x00U )
	{
		/* Clear flag */
		LFFR |= _BM(LFTOF);
		ID0_Wake=0x03;		
		LFIMR = 0x00;//关中断
	}

	/* Check if a CRC error has been detected */
	if ( (PHFR & _BM(CRCEF)) != 0x00U )
	{
		/* Clear flag */
		PHFR |= _BM(CRCEF);
	}
}
ISR(LFID0INT_vect)
{
	ID0_Wake=0x01;
	LFFR = 0x0F;  // clear all flags
	LFSTOP = 0x30;        // set 3 symbols = low as stop condition
	LFIMR = 0x04;         // enable End of Telegram
	LED_Toggle();
	//g_bSleepModeConfig_flash = 3;	
}

void ATA_lfRxEnableWakeup_flash_C()
{
	//LDFCKSW – LF Data FIFO Clock Switch Register
	LDFCKSW |= (1<<LDFSCSW); //LF Data FIFO Synchronous Clock Switch
	// wait until clock source is switched
	while ((LDFCKSW & (1<<LDFSCKS)) ==0); //等待FIFO clock source switched to AVR clock domain
  
	//  LFQC1 = LFRX_R_Trim117k;
	//  LFQC2 = LFRX_R_Trim90k;
	//  LFQC3 = LFRX_R_Trim90k;
	//  LFQC1 = LFRX_R_Trim36k;
	//  LFQC2 = LFRX_R_Trim36k;
	//  LFQC3 = LFRX_R_Trim36k;
	//  LFDSR6 = 0x0F;                // only for 7.8 kbit
	
	
	//LFCR0 – Low Frequency Control Register 0
	LFCR0 = 0x80 | 0x08 | _BM(LFCE1) | _BM(LFCE2) | _BM(LFCE3);    //192us,3.9k, activate all channels and set baudrate
	LFCR1 = _BM(LFRE);// | _BM(LFPEEN);  // enable RX, ID and Data Mode
	LFCR2 = 0x00;//bSense;  // select sensitivity
	//  LFCR3 |= 0x01; // at first without trimming function, enable trim function
	//LDFFL – LF Data FIFO Fill Level Register
	LDFFL =0x80;//LF Data FIFO Clear
  
	// Settings for the protocol handler
	PHID00 = 0x80; //*pLf_Id++;
	PHID01 = 0x84;//*pLf_Id++;
	PHID02 = 0x4d; //*pLf_Id++;
	PHID03 = 0x04; //*pLf_Id++;

	PHID0L = 30;//Identifier 0 Lenght
	PHIDFR = 30;//Identifier Frame 
  
    PHDFR = 200;//Protocol Handler Data Frame
	PHTBLR = 0xFF; //Protocol Handler Telegram Bit Length
	//PHDFR = 16; 
	LFSYSY3 = 0x00;
	LFSYSY2 = 0x03;
	LFSYSY1 = 0x8b;
	LFSYSY0 = 0x32;
	LFSYLE = 0x12;//LF Synchronization Word (S4) example
  
	//  PHCRPH = 0; 
	//  PHCRPL = 0x07; 
	//  PHCSTH = 0;
	//  PHCSTL = 0; 
	//  PHCRCR = (1<<CRCEN)|0x20;     // enable 8 bit CRC 
  
	//PHIMR – Protocol Handler Interrupt Mask Register
	PHIMR |= _BM(PHID0IM);//(1<<4);// Set PHID0IM=1 (ID0 IRQ mask)
	//LTEMR |= (1<<ID0EM);   // Set ID0EM event mask to '1'
	//  LTEMR |= (1<<FLEM);
	//  LDFIM |= (1<<LDFFLIM); 
	//LFIMR |= _BM(LFSYDIM);
	//LDFC – LF Data FIFO Configuration Register
	LDFC = 0x7f;//LDFMSB – LF Data FIFO MSB Bit Order

	LDFCKSW &= ~(1<<LDFSCSW); //switch to the LF protocol handler clock domain
}

void ATA_lfRxInit_C(void)
{
    uint8_t bSreg = SREG;
    uint8_t* pDestValue;

    /* LLR-Ref: 010 */
    //g_sLfRx.bFlags = LFRX_FLAGS_RESET;

    /* LLR-Ref: 020 */
    ATA_POWERON_C(PRR1, PRLFR);//低频接收模块供电
    ATA_POWERON_C(PRR1, PRLFPH);//低频协议处理模块供电

    /* LLR-Ref: 025 */
    while(EECR & _BM(NVMBSY)){}//等待之前的eeprom操作完成

    /* lint -e923 GeWi (23nov2011)*/
    /* disable lint error 923 - Cast between pointer type and an integral type.
     * Misra Rule 11.3 says: Casting between a pointer and an integer type should be avoided where possible,
     * but may be unavoidable when addressing memory mapped registers or other hardware specific features.
     */
    /* LLR-Ref: 030 */
	//EEPROM地址：0x8ae,eepSRCTCAL, SRC temperature calibration setting
    EEARH = 0x08;//(uint8_t)((uint16_t)&g_sAtmelEEPromSection.eepSRCTCAL >> 8U);
    EEARL = 0xae;//(uint8_t)((uint16_t)&g_sAtmelEEPromSection.eepSRCTCAL >> 0U);
    /*lint -restore */

    EECR2 |= _BM(EEBRE);//启动eeprom连续读

    /* LLR-Ref: 040 */
	//EEPROM地址：0x8ae,eepSRCTCAL, SRC temperature calibration setting
    SRCTCAL = EEDR;
	//EEPROM地址：0x08AF eepSRCCAL SRC calibration setting
    SRCCAL  = EEDR;

    /* LLR-Ref: 050 */	
	//EEPROM地址：0x08C0
    EEARH = 0x08;//(uint8_t)((uint16_t)&g_sAtmelEEPromSection.eepLFCALR[0] >> 8U);
    EEARL = 0xc0;//(uint8_t)((uint16_t)&g_sAtmelEEPromSection.eepLFCALR[0] >> 0U);
    
    /* LLR-Ref: 060 */
	//EEPROM地址：0x08C0-0x08F4,	eepLFCALR[53], LF receiver calibration settings
    pDestValue = (uint8_t*)&LFCALR1;
    for( uint8_t i = 0x00U; i < 53; i++)
    {
        *pDestValue++ = EEDR;
    }
    
    /* LLR-Ref: 065 */
	//EEPROM地址：0x08F5-0x08F7 eepTPCALR11\2\3 Transponder calibration settings
    TPCALR11 = EEDR;
    TPCALR12 = EEDR;
    TPCALR13 = EEDR;
    
    /* LLR-Ref: 070 */
	//EEPROM地址：0x08F8-0x0902	eepLFDSR[11] LF Freq decoder settings
    pDestValue = (uint8_t*)&LFDSR1;
    for( uint8_t i = 0x00U; i < 11; i++)
    {
        *pDestValue++ = EEDR;
    }
    
    /* LLR-Ref: 080 */
    if ( EECR2 & _BM(E2FF) )
    {
		//eeprom读出错
//         g_sLfRx.bFlags |= LFRX_FLAGS__BM(ERROR_FLAG;
//         g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRX_EEPROM_READ_ERROR;
        EECR2 |= _BM(E2FF);
    }
    
    /* LLR-Ref: 090 */
    if(EECR2 & _BM(E2AVF))
    {
		//eeprom非法访问错误
//         g_sLfRx.bFlags |= LFRX_FLAGS__BM(ERROR_FLAG;
//         g_sDebug.bErrorCode = DEBUG_ERROR_CODE_LFRX_EEPROM_ACCESS_ERROR;
        EECR2 |= _BM(E2AVF);
    }

    /* LLR-Ref: 090 */
    EECR2 &= ~_BM(EEBRE);//关闭eeprom联系读

    /* LLR-Ref: 100 */
    cli();
    LFCPR = _BM(LFCPCE);// LF Calibration Protect Change Enable
    LFCPR = (_BM(LFCALRY) | _BM(LFCALP));//LF Calibration Ready && LF Calibration Protection
    SREG = bSreg;
}
