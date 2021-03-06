#include "LF_RX.h"
#include "HardwareProfile.h"
#include "Variables.h"
ISR(LFEOT_vect)//LF Decoder Error Interrupt
{
	/* Check if a timeout has been detected */
	// Telegram Time Out
	if ( (LFFR & _BM(LFTOF)) != 0x00U )
	{
		/* Clear flag */
		LFFR |= _BM(LFTOF);
		if(!bSleep_IsSet())
			bLFRxFinish_Set();
		//LFIMR = 0x00;
	}
}
ISR(LFID0INT_vect)
{
	//ID0_Wake=0x01;
	//PHIMR = 0x00;
	LFFR = 0x0F;  // clear all flags
	if(!bSleep_IsSet())
		bLFID0Wake_Set();
	//bLFRxFinish_Reset();
	//LFIMR = 0x04;         // enable End of Telegram
}
void ATA_lfRxEnableWakeup_flash_C(void)
{
	LDFCKSW |= (1<<LDFSCSW);
	while ((LDFCKSW & (1<<LDFSCKS)) ==0); //等待FIFO clock source switched to AVR clock domain
	//192us,3.9k, activate all channels
	LFCR0 = 0x8f;//0x80 | 0x08 | _BM(LFCE1) | _BM(LFCE2) | _BM(LFCE3);    
	//使能低频接收
	LFCR1 = 0x80;//_BM(LFRE);// | _BM(LFPEEN);  // enable RX, ID and Data Mode
	//低频高灵敏度模式
	LFCR2 = 0x00;//bSense;  // select sensitivity
	//低频监听模式，不使用巡检模式
	LFCR3 =0x00;
	LDFFL =0x80;//LF Data FIFO Clear  
	// 30位的ID
	PHID00 = SSID[3];//0x80; //*pLf_Id++;
	PHID01 = SSID[2];//*pLf_Id++;
	PHID02 = SSID[1]; //*pLf_Id++;
	PHID03 =SSID[0]>>3;
	PHID0L = 30;//Identifier 0 Lenght
	PHIDFR = 30;//Identifier Frame 
	//PHTCR |= _BM(CSM);
    PHDFR = 0xff;//Protocol Handler Data Frame
	PHTBLR = 0xFF; //Protocol Handler Telegram Bit Length
	//PHDFR = 16; 
	LFSYSY3 = 0x00;
	LFSYSY2 = 0x03;
	LFSYSY1 = 0x8b;
	LFSYSY0 = 0x32;
	LFSYLE = 0x12;//LF Synchronization Word (S4) example
	
	//LFQC1=0xf1;
	//LFQC2=0xf1;
	//LFQC3=0xf1;
	//ID0接收中断
	//LFIMR = 0x00;
	PHIMR = _BM(PHID0IM);//(1<<4);// Set PHID0IM=1 (ID0 IRQ mask)
	LFIMR = 0x04; 
	//MSB,不使用FIFO填充满中断
	LDFC = 0x7f;//(1<<LDFMSB) | 2;//LDFMSB – LF Data FIFO MSB Bit Order

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
